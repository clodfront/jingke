// Project:	jingke
// File:    tcp_socket.cpp
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
#include "tcp_socket.h"
#include <netinet/tcp.h> 

using namespace jingke;

CTcpSocket::CTcpSocket()
{

}

CTcpSocket::~CTcpSocket()
{

}

int	CTcpSocket::Listen(int backlog)
{
	return ::listen(_fd, backlog);
}

int CTcpSocket::Accept(CTcpSocket &sock, struct sockaddr *sock_addr, socklen_t &sock_len)
{
	assert(!sock.Valid());
    int fd = INVALID_SOCKET;
    while ((fd = ::accept(_fd, sock_addr, &sock_len)) < 0 && errno == EINTR);
	if(fd < 0)
		return INVALID_SOCKET;
	sock._fd = fd;
    return fd;
}

int CTcpSocket::GetPeerAddr(std::string &peer_addr, uint16_t &peer_port)
{
	int ret = 0;
    struct sockaddr sock_addr;
    bzero(&sock_addr, sizeof(struct sockaddr));
    socklen_t sock_len = sizeof(struct sockaddr);

    ret = getpeername(_fd, &sock_addr, &sock_len);
    if(ret < 0)
		return -1;
	
    char str_addr[INET_ADDRSTRLEN]; //#define INET_ADDRSTRLEN 16 /* for IPv4 dotted-decimal */
    memset(str_addr, 0, sizeof(str_addr));
    struct sockaddr_in *p = (struct sockaddr_in *)&sock_addr;
    inet_ntop(AF_INET, &p->sin_addr, str_addr, sizeof(str_addr));
    peer_addr = str_addr;
    peer_port = ntohs(p->sin_port);
	return ret;
}

int CTcpSocket::GetSelfAddr(std::string &self_addr, uint16_t &self_port)
{
	int ret = 0;
    struct sockaddr sock_addr;
    bzero(&sock_addr, sizeof(struct sockaddr));
    socklen_t sock_len = sizeof(struct sockaddr);

	ret = getsockname(_fd, &sock_addr, &sock_len);
	if(ret < 0)
		return -1;

    char str_addr[INET_ADDRSTRLEN];
	memset(str_addr, 0, sizeof(str_addr));
    struct sockaddr_in *p = (struct sockaddr_in *)&sock_addr;
    inet_ntop(AF_INET, &p->sin_addr, str_addr, sizeof(str_addr));
    self_addr = str_addr;
    self_port = ntohs(p->sin_port);
	return ret;
}

int	CTcpSocket::SetNoDelay()
{
	static int flag = 1;
	return ::setsockopt(_fd, IPPROTO_TCP, TCP_NODELAY, (void *)&flag, sizeof(flag));
}

int	CTcpSocket::SetLinger(int onoff, int linger_time)
{
	struct linger stlinger;
	stlinger.l_onoff = onoff;
	stlinger.l_linger = linger_time;
	return ::setsockopt(_fd, SOL_SOCKET, SO_LINGER, &stlinger, sizeof(stlinger));
}

int	CTcpSocket::SetKeepAlive(int onoff, int keep_idle, int keep_interval, int keep_times)
{
	if(::setsockopt(_fd, SOL_SOCKET, SO_KEEPALIVE, (void *)&onoff, sizeof(onoff)) < 0)
		return -1;
	if(0 != onoff)
	{
		if(::setsockopt(_fd, SOL_TCP, TCP_KEEPIDLE, (void*)&keep_idle, sizeof(keep_idle)) < 0)
			return -2;
		if(::setsockopt(_fd, SOL_TCP, TCP_KEEPINTVL, (void *)&keep_interval, sizeof(keep_interval)) < 0)
			return -3;
		if(::setsockopt(_fd, SOL_TCP, TCP_KEEPCNT, (void *)&keep_times, sizeof(keep_times)) < 0)
			return -4;
	}
	return 0;
}
