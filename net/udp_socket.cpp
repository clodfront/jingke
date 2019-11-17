// Project:	jingke
// File:    udp_socket.cpp
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
#include "udp_socket.h"

using namespace jingke;

CUdpSocket::CUdpSocket()
{

}

CUdpSocket::~CUdpSocket()
{

}

int	CUdpSocket::RecvUdp(void *buf, size_t len, const std::string& ip, uint16_t port, int flags)
{
	struct sockaddr sock_addr;
	socklen_t socklen = sizeof(sock_addr);
	bzero(&sock_addr, sizeof(struct sockaddr));
	struct sockaddr_in* p = (struct sockaddr_in *)&sock_addr;
	p->sin_family = AF_INET;
	p->sin_addr.s_addr = inet_addr(ip.c_str());
	p->sin_port = htons(port);
    return ::recvfrom(_fd, buf, len, flags, &sock_addr, &socklen);
}

int	CUdpSocket::SendUdp(const void *buf, size_t len, const std::string& ip, uint16_t port, int flags)
{
	struct sockaddr sock_addr;
	bzero(&sock_addr, sizeof(struct sockaddr));
	struct sockaddr_in* p = (struct sockaddr_in *)&sock_addr;
	p->sin_family = AF_INET;
	p->sin_addr.s_addr = inet_addr(ip.c_str());
	p->sin_port = htons(port);
	return ::sendto(_fd, buf, len, flags, &sock_addr, sizeof(sock_addr));
}


