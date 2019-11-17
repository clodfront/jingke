// Project:	jingke
// File:    udp_socket.h
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

#ifndef _UDP_SOCKET_H_
#define _UDP_SOCKET_H_

#include "socket_base.h"

namespace jingke {

class CUdpSocket : public ISocket
{
public:
	CUdpSocket();
	~CUdpSocket();
public:
	virtual int		Type() final {return SOCK_STREAM;}
public:		//TCP专属
	int		RecvUdp(void *buf, size_t len, const std::string& ip, uint16_t port, int flags=0);
	int		SendUdp(const void *buf, size_t len, const std::string& ip, uint16_t port, int flags=0);
};

} ///namespace jingke

#endif  ///_UDP_SOCKET_H_


