// Project:	jingke
// File:    tcp_socket.h
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

#ifndef _TCP_SOCKET_H_
#define _TCP_SOCKET_H_

#include "socket_base.h"

namespace jingke {

class CTcpSocket : public ISocket
{
public:
	CTcpSocket();
	~CTcpSocket();
public:
	virtual int		Type() final {return SOCK_STREAM;}
public:		//TCP专属
	int		Listen(int backlog);
	int 	Accept(CTcpSocket &sock, struct sockaddr *sock_addr, socklen_t &sock_len);
	int 	GetPeerAddr(std::string &peer_addr, uint16_t &peer_port);
	int 	GetSelfAddr(std::string &self_addr, uint16_t &self_port);
	int		SetNoDelay();
	int		SetKeepAlive(int onoff, int keep_idle, int keep_interval, int keep_times);
	int		SetLinger(int onoff, int linger_time);
};

} ///namespace jingke

#endif  ///_TCP_SOCKET_H_


