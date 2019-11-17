// Project:	jingke
// File:    socket_connection_base.cpp
// Created: 11/2019
// Author:  Goof
//
// Copyright 2019-2020 Goof
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include "socket_connection_base.h"
#include "log_writer.h"
#include "epoller.h"
#include "singleton.h"
#include "socket_rsbuffer_pool.h"

using namespace jingke;

void ISocketConnectionBase::SetPeerPoint(const CSocketPoint& peer_point_)
{
	_peer_point = peer_point_;
}

const CSocketPoint& ISocketConnectionBase::GetPeerPoint()
{
	return _peer_point;
}

void ISocketConnectionBase::Close()
{
	CSocketEventer::Close();
	_close_on_send_over = false;
	FreeRSBuff();
}

int ISocketConnectionBase::OnSendble()
{
    if(!Socket().Valid())
		return -1;
	CStreamBuffer::ChunkPair chunk_pair = _send_buff.GetDataChunk();
	CStreamBuffer::SDataChunk* chunk_pair_ptr[2];
	chunk_pair_ptr[0] = &chunk_pair.first;
	chunk_pair_ptr[1] = &chunk_pair.second;

	if(_close_on_send_over && _send_buff.Empty())
	{
		Close();
		return 0;
	}

	for(int i=0; i<2; ++i)
	{
		CStreamBuffer::SDataChunk* chunk_ptr = chunk_pair_ptr[i];
		if(!chunk_ptr->Valid())
			break;
		int n = 0;
		while ((n = Socket().Send(chunk_ptr->_data_ptr, chunk_ptr->_data_len)) < 0 && errno == EINTR);
		if(n < 0)
		{
			if(EAGAIN == errno || EWOULDBLOCK == errno)
			{
				return 0;
			}
			else
			{
				LOG_ERROR("fd:%d send cache data failed. data len:%d, errno:%d.", Socket().GetFd(), (int)chunk_ptr->_data_len, errno);
				return -2;
			}
		}
		else
		{
			_send_buff.Consume(n);
			if((size_t)n < chunk_ptr->_data_len)
			{
				return 0;
			}
		}
	}
	return 0;
}

int ISocketConnectionBase::OnRecvble()
{
    if(!Socket().Valid())
		return -1;
    if(!_parser)
        return -2;
	bool need_to_recv_immediately = false;
	do
	{
		if(_recv_buff.Full())
		{
			LOG_ERROR("fd:%d recv cache is full.", Socket().GetFd());
			return -3;
		}
		char * recv_buff_free_prt = _recv_buff.FreeData();
		size_t recv_buff_free_len = _recv_buff.FreeLength();
		
		int n = 0;
		while ((n = Socket().Recv(recv_buff_free_prt, recv_buff_free_len)) < 0 && errno == EINTR);
		if(n < 0)
		{
			if(EAGAIN == errno || EWOULDBLOCK == errno)
			{
				return 0;
			}
			else
			{
				LOG_ERROR("fd:%d recv data failed. errno:%d.", Socket().GetFd(), errno);
				return -4;
			}
		}
		else
		{
			if(0 == n)
			{
				// the return value will be 0 when the peer has performed an orderly shutdown.
				LOG_DEBUG("fd:%d peer has shutdown.", Socket().GetFd());
				return -5;
			}

			_recv_buff.Produce(n);
			
			do
			{
                size_t packge_size = _parser->GetPackSize(_recv_buff.ValidData(), _recv_buff.ValidLength());
                if(INVALID_PACK_SIZE == packge_size)
                {
                    LOG_ERROR("fd:%d parser pack size error.", Socket().GetFd());
                    return -6;
                }
				if(UNDETERMINED_PACK_SIZE == packge_size || _recv_buff.ValidLength() < (size_t)packge_size)
				{
					// copy incomplete packge to recv buffer head
					_recv_buff.MoveFront();
					break;
				}

				uint64_t async_key = _parser->GetAsyncKey(_recv_buff.ValidData(), _recv_buff.ValidLength());

				// recv a complete packge 
				// do req logic or send msg to req deal queue
                this->OnRecvPack(_recv_buff.ValidData(), packge_size, async_key);
				
				_recv_buff.Consume(packge_size); // if execute Consume/Read _recv_buff change to empty, it will auto MoveFront
				
			}while(!_recv_buff.Empty());

			need_to_recv_immediately = ((size_t)n == recv_buff_free_len);
		}

	}while(need_to_recv_immediately);
	return 0;
}

int ISocketConnectionBase::Send(const void* data_, size_t len_, bool close_after_send_/* = false*/)
{
    if(!data_ || !len_ || !Socket().Valid())
		return -1;
	
	if(_close_on_send_over)
		return -2;
	if(close_after_send_)
		_close_on_send_over = true;
	
	//send cache data first if have
	CStreamBuffer::ChunkPair chunk_pair = _send_buff.GetDataChunk();
	CStreamBuffer::SDataChunk* chunk_pair_ptr[2];
	chunk_pair_ptr[0] = &chunk_pair.first;
	chunk_pair_ptr[1] = &chunk_pair.second;

	for(int i=0; i<2; ++i)
	{
		CStreamBuffer::SDataChunk* chunk_ptr = chunk_pair_ptr[i];
		if(!chunk_ptr->Valid())
			break;
		int n = 0;
		while ((n = Socket().Send(chunk_ptr->_data_ptr, chunk_ptr->_data_len)) < 0 && errno == EINTR);
		if(n < 0)
		{
			if(EAGAIN == errno || EWOULDBLOCK == errno)
			{
				int ret = _send_buff.Write(data_, len_);
				if(ret != 0)
				{
					LOG_ERROR("fd:%d write data to send buffer failed. data len:%d, ret:%d.", Socket().GetFd(), (int)len_, ret);
					return -2;
				}
				return 0;
			}
			else
			{
				LOG_ERROR("fd:%d send cache data failed. data len:%d, errno:%d.", Socket().GetFd(), (int)chunk_ptr->_data_len, errno);
				return -3;
			}
		}
		else
		{
			_send_buff.Consume(n);
			if((size_t)n < chunk_ptr->_data_len)
			{
				int ret = _send_buff.Write(data_, len_);
				if(ret != 0)
				{
					LOG_ERROR("fd:%d write data to send buffer failed. data len:%d, ret:%d.", Socket().GetFd(), (int)len_, ret);
					return -2;
				}
				return 0;
			}
		}		
	}

	//try to direct send
	int n = 0;
	while ((n = Socket().Send(data_, len_)) < 0 && errno == EINTR);
	if(n < 0)
	{
		if(EAGAIN == errno || EWOULDBLOCK == errno)
		{
			int ret = _send_buff.Write(data_, len_);
			if(ret != 0)
			{
				LOG_ERROR("fd:%d write data to send buffer failed. data len:%d, ret:%d.", Socket().GetFd(), (int)len_, ret);
				return -2;
			}
			return 0;
		}
		else
		{
			LOG_ERROR("fd:%d send direct data failed. data len:%d, errno:%d.", Socket().GetFd(), (int)len_, errno);
			return -3;
		}	
	}
	else
	{
		if((size_t)n < len_)
		{
			size_t remain = len_ - n;
			int ret = _send_buff.Write((char*)data_+n, remain);
			if(ret != 0)
			{
				LOG_ERROR("fd:%d write data to send buffer failed. data len:%d, ret:%d.", Socket().GetFd(), (int)remain, ret);
				return -2;
			}
		}
		else
		{
			if(_close_on_send_over)
			{
				Close();
			}
		}
		
	}
	return 0;
}

void ISocketConnectionBase::OnError()
{
    Close();
}

int ISocketConnectionBase::OnRecvPack(const void* data_, size_t len_, uint64_t async_key_)
{
	if(!_epoller)
		return -1;
	const CEpoller::recv_package_call_t& call_back = _epoller->GetRecvPackageCallBack();
	if(!call_back)
		return -2;
	return call_back(this, data_, len_, async_key_);
}

int ISocketConnectionBase::SetRecvBuff(char* buff_ptr_, size_t buff_len_)
{
	return _recv_buff.Init(buff_ptr_, buff_len_);
}

int ISocketConnectionBase::SetSendBuff(char* buff_ptr_, size_t buff_len_)
{
	return _send_buff.Init(buff_ptr_, buff_len_);
}

void ISocketConnectionBase::FreeRSBuff()
{
	if(_recv_buff.GetFrontPtr())
	{
		singleton<CSocketRSBufferPool>::instance().Free(_recv_buff.GetCapacity(), _recv_buff.GetFrontPtr());
		_recv_buff.Reset();
	}

	if(_send_buff.GetFrontPtr())
	{
		singleton<CSocketRSBufferPool>::instance().Free(_send_buff.GetCapacity(), _send_buff.GetFrontPtr());
		_send_buff.Reset();
	}
}

void ISocketConnectionBase::ClearRSBuff()
{
	_recv_buff.ClearData();
	_send_buff.ClearData();
}