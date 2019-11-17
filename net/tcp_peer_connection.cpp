// Project:	jingke
// File:    tcp_peer_connection.cpp
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
#include "tcp_peer_connection.h"
#include <algorithm>
#include "epoller.h"
#include "log_writer.h"

using namespace jingke;

CTcpPeerConnection::~CTcpPeerConnection()
{
	if(_reconnect_timer_id!=CTimerMono::bad_timer_id && _timer_mgr)
		_timer_mgr->remove(_reconnect_timer_id);
	_state=0;_timer_mgr=NULL; _reconnect_timer_id=CTimerMono::bad_timer_id; _delay_time=0;_service_id=0;
}

void CTcpPeerConnection::AutoReConnect(mstime_t now_, uint16_t round_)
{
    _reconnect_timer_id = CTimerMono::bad_timer_id;
    ConnectToService();
}

void CTcpPeerConnection::DelayReconnect()
{	
    if(!_timer_mgr) return;
    if(e_wait_reconnect == _state || _reconnect_timer_id != CTimerMono::bad_timer_id) return;
    CTimerMono::timer_call_t callbak = std::bind(&CTcpPeerConnection::AutoReConnect, this, std::placeholders::_1, std::placeholders::_2);
    _delay_time = std::max((uint32_t)1000, _delay_time);
    _delay_time = std::min((uint32_t)64000, _delay_time*2);
    _reconnect_timer_id = _timer_mgr->insert(callbak, _delay_time);
    _state = e_wait_reconnect;
	LOG_INFO("fd:%d delay connect [ip:%s port:%u] after %u ms.", _sock.GetFd(), _peer_point.GetAddr().c_str(), _peer_point.GetPort(), _delay_time);
}

int CTcpPeerConnection::ConnectToService()
{
    if(!_epoller)
		return -1;
    if(_state != e_close && _state != e_wait_reconnect)
        return -2;
	if(Socket().Valid())
		_epoller->DelSocketEventer(_sock.GetFd());
	int ret = Socket().Create();
	if(ret < 0)
		return -3;
	if(0 != Socket().SetNonblock())
		return -4;

	ClearRSBuff();

	while( -1 == (ret = Socket().Connect(_peer_point.GetAddr().c_str(), _peer_point.GetPort())) && errno == EINTR);
	if(ret != 0)
	{
		if(-1 != ret)
		{
			return -5;
		}
		else
		{
			switch(errno)
			{
			case EINPROGRESS:
			{
				if(0 != _epoller->AddSocketEventer(_sock.GetFd(), EPOLLOUT|EPOLLET, this))
				{
					return -6;
				}
                _state = e_connecting;
				return 0; // ing
			}
			//case EISCONN:
			default:
                //delay reconnect again
                DelayReconnect();
				return 0;
			}
		}
	}

	if(0 != _epoller->AddSocketEventer(_sock.GetFd(), EPOLLIN|EPOLLOUT|EPOLLRDHUP|EPOLLET, this))
	{
		return -7;
	}
	_state = e_connected;
	_delay_time = 0;
	LOG_INFO("fd:%d direct connect ok[ip:%s port:%u].", _sock.GetFd(), _peer_point.GetAddr().c_str(), _peer_point.GetPort());
	return 0;
}

int CTcpPeerConnection::OnSendble()
{
    if(!Socket().Valid())
	{
		LOG_ERROR("%s", "!Socket().Valid()");
		return -1;
	}
	if(e_connecting == _state)
	{
		int err = 0;
		socklen_t len = sizeof(int);
		int ret = ::getsockopt(_sock.GetFd(), SOL_SOCKET, SO_ERROR, &err, &len);
		if(0 == ret)
		{
			if(err != 0)
			{
				LOG_ERROR("fd:%d connect failed[ip:%s port:%u], try again... errno:%d.", _sock.GetFd(), _peer_point.GetAddr().c_str(), _peer_point.GetPort(), err);
				return -2; 
			}
			else
			{
				if(_epoller)
				{
					ret = _epoller->ModSocketEventer(_sock.GetFd(), EPOLLIN|EPOLLOUT|EPOLLRDHUP|EPOLLET, this);
					if(ret != 0)
					{
						LOG_ERROR("fd:%d connect failed[ip:%s port:%u], _epoller->ModSocketEventer.", _sock.GetFd(), _peer_point.GetAddr().c_str(), _peer_point.GetPort());
						return -3;
					}
				}
				_state = e_connected;
				_delay_time = 0;
				
				LOG_INFO("fd:%d connect ok[ip:%s port:%u].", _sock.GetFd(), _peer_point.GetAddr().c_str(), _peer_point.GetPort());
				return 0;
			}
		}
		else
		{
			LOG_ERROR("fd:%d getsockopt failed, errno:%d.", _sock.GetFd(), errno);
			return -4;
		}
	}
	else
	{
		return ISocketConnectionBase::OnSendble();
	}
	return 0;
}

void CTcpPeerConnection::OnError()
{
    DelayReconnect();
}

void CTcpPeerConnection::Close()
{
	ISocketConnectionBase::Close();
	if(_reconnect_timer_id!=CTimerMono::bad_timer_id && _timer_mgr)
		_timer_mgr->remove(_reconnect_timer_id);
	_state=0;
	_timer_mgr=NULL; 
	_reconnect_timer_id=CTimerMono::bad_timer_id; 
	_delay_time=0;
	_service_id=0;
}

int CTcpPeerConnection::Send(const void* data_, size_t len_, bool close_after_send_)
{
	if(_state != e_connected)
		return -1;
	return ISocketConnectionBase::Send(data_, len_, close_after_send_);
}

