// Project:	jingke
// File:    tcp_listener.cpp
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
#include "tcp_listener.h"
#include "log_writer.h"
#include "tcp_connection.h"
#include "epoller.h"
#include "singleton.h"
#include "tcp_connection_pool.h"
#include "socket_rsbuffer_pool.h"

using namespace jingke;

int CTcpListener::Init(const STcpListenerCfg& cfg_)
{
    _cfg = cfg_;
    //need to check cfg here
    return 0;
}

int CTcpListener::Listen()
{
    if(!_epoller)
		return -1;
	
	int ret = _sock.Create();
	if(ret < 0)
	{
		return -2;
	}

	if(0 != _sock.SetReuseAddr())
	{
		return -3;
	}
	
	if(0 != _sock.Bind(_cfg._local_point.GetAddr(), _cfg._local_point.GetPort()))
	{
		return -4;
	}

	if(0 != _sock.Listen(_cfg._backlog))
	{
		return -5;
	}

	if(0 != _sock.SetNonblock())
	{
		return -6;
	}

	if(0 != _epoller->AddSocketEventer(_sock.GetFd(), EPOLLIN|EPOLLET, this))
	{
		return -7;
	}

    return 0;
}

int CTcpListener::OnRecvble()
{
	if(!_sock.Valid())
		return -1;
	
	if(!_epoller)
	{
		LOG_ERROR("_epoller of listener id:%u is null.", _cfg._listener_id);
		return -1;
	}

	struct sockaddr sock_addr;
	socklen_t sock_len;
	int ret = 0;

	do
	{
		CTcpSocket sock;
		while((ret = _sock.Accept(sock, &sock_addr, sock_len))<0 && EINTR == errno);
		if(ret < 0)
		{
			if(EAGAIN == errno || EWOULDBLOCK == errno)
			{
				return 0;
			}
			else
			{
				//it need to return 0 to ignore, otherwise listen will stop
				LOG_WARN("listener id:%u is accept error errno:%d.", _cfg._listener_id, errno);
				return 0;
			}
		}
		else
		{
			CTcpConnection* tcp_remote_client = CTcpConnection::Create();
			if(!tcp_remote_client)
			{
				LOG_WARN("listener id:%u can not alloc new tcp_remote_client obj.", _cfg._listener_id);
				sock.Close();
				continue;
			}

			sock.SetNonblock();
			sock.SetNoDelay();
			sock.SetKeepAlive(_cfg._client_keepalive_onoff, _cfg._client_keepalive_idle, _cfg._client_keepalive_interval, _cfg._client_keepalive_times);
			//_sock.SetLinger(0, 0); just use default linger

			tcp_remote_client->SetParser(this->GetParser());
			tcp_remote_client->SetEpoller(this->GetEpoller());
			tcp_remote_client->Socket() = sock;

			CSocketPoint peer_point;
			char str_addr[INET_ADDRSTRLEN]; //#define INET_ADDRSTRLEN 16  for IPv4 dotted-decimal 
			memset(str_addr, 0, sizeof(str_addr));
			struct sockaddr_in *p = (struct sockaddr_in *)&sock_addr;
			inet_ntop(AF_INET, &p->sin_addr, str_addr, sizeof(str_addr));
			peer_point.SetAddr(str_addr);
			peer_point.SetPort(ntohs(p->sin_port));
			tcp_remote_client->SetPeerPoint(peer_point);
			
			char* recv_buff_ptr = singleton<CSocketRSBufferPool>::instance().New(_cfg._client_sock_recv_buff_size);
			if(!recv_buff_ptr)
			{
				LOG_WARN("listener id:%u can not alloc new socket recv buffer length of %u.", _cfg._listener_id, _cfg._client_sock_recv_buff_size);
				sock.Close();
				continue;
			}
			
			char* send_buff_ptr = singleton<CSocketRSBufferPool>::instance().New(_cfg._client_sock_send_buff_size);
			if(!send_buff_ptr)
			{
				LOG_WARN("listener id:%u can not alloc new socket send buffer length of %u.", _cfg._listener_id, _cfg._client_sock_send_buff_size);
				sock.Close();
				continue;
			}
			
			tcp_remote_client->SetRecvBuff(recv_buff_ptr, _cfg._client_sock_recv_buff_size);
			tcp_remote_client->SetSendBuff(send_buff_ptr, _cfg._client_sock_send_buff_size);

			tcp_remote_client->SetListenerID(_cfg._listener_id);
			
			ret = _epoller->AddSocketEventer(sock.GetFd(), EPOLLIN|EPOLLOUT|EPOLLRDHUP|EPOLLET, tcp_remote_client);
			if(0 != ret)
			{
				LOG_DEBUG("AddSocket fd:%d fd_type:%d failed.", sock.GetFd(), tcp_remote_client->Type());
			}

			LOG_DEBUG("tcp_remote_client fd:%d connect on listener:%u.", sock.GetFd(), _cfg._listener_id);
		}
		
	}while(true);
	
	return 0;
}