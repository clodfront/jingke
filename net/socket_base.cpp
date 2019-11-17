// Project:	jingke
// File:    socket_base.cpp
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
#include "socket_base.h"
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <assert.h>


using namespace jingke;

ISocket::ISocket()
:_fd(INVALID_SOCKET)
{
}

ISocket::~ISocket()
{
    if(INVALID_SOCKET != _fd)
	{
		//::close(_fd); //CSocket is Copy to pass
		_fd = INVALID_SOCKET;
	}
}

bool ISocket::Valid()
{
	return _fd != INVALID_SOCKET;
}

int ISocket::Create()
{
	Close();
	//socket() On success, a file descriptor for the new socket is returned.  On error, -1 is returned, and errno is set appropriately.
	_fd = socket(PF_INET, Type(), 0);
	return _fd == -1 ? INVALID_SOCKET : _fd;
}

void ISocket::Close()
{
	if(INVALID_SOCKET != _fd)
	{
		::close(_fd);
		_fd = INVALID_SOCKET;
	}
}

int ISocket::Shutdown(int iHow)
{
	if(iHow<SHUT_RD || iHow>SHUT_RDWR)
		return -1;
	return ::shutdown(_fd, iHow);
}

int	ISocket::GetFd()
{
	return _fd;
}

void ISocket::SetFd(int fd)
{
    _fd = fd;
}

int ISocket::SetNonblock()
{
	int flags = 1;
    // Use just one system call
    if (ioctl(_fd, FIONBIO, &flags) != -1)
      	return 0;
    // Do it the Posix way
    if ((flags = fcntl(_fd, F_GETFL, 0)) < 0 || fcntl(_fd, F_SETFL, flags | O_NONBLOCK) < 0) 
    	return -1;
	return 0;
}

int ISocket::SetReuseAddr()
{
	// Set SO_REUSEADDR befor bind(), to reuse the listen addr
	static int flag = 1;
	return ::setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&flag, sizeof(flag));
}

int ISocket::SetSendBufferSize(int size)
{
	return ::setsockopt(_fd, SOL_SOCKET, SO_SNDBUF, (void *)&size, sizeof(size));	
}

int ISocket::SetRecvBufferSize(int size)
{
	return ::setsockopt(_fd, SOL_SOCKET, SO_RCVBUF, (void *)&size, sizeof(size));	
}

int ParseAddr(const std::string &addr, struct in_addr &addr_struct)
{
    int ret = inet_pton(AF_INET, addr.c_str(), &addr_struct);
    if(-1 == ret)
        return -1;
	else if(1 == ret)
		return 0;// direct success
    else if(0 == ret)
    {
        struct hostent hostenter;
        struct hostent *ptr_hostent = NULL;
        char buf[2048] = "\0";
        int err = 0;
        ::gethostbyname_r(addr.c_str(), &hostenter, buf, sizeof(buf), &ptr_hostent, &err);
        if (ptr_hostent == NULL || ptr_hostent->h_addr == NULL)
			return -2;
        addr_struct = *(struct in_addr *) ptr_hostent->h_addr;
    }
	return 0;
}

int ISocket::Bind(const std::string &addr, uint16_t port)
{
    struct sockaddr sock_addr;
    bzero(&sock_addr, sizeof(struct sockaddr));
    struct sockaddr_in *p = (struct sockaddr_in *)&sock_addr;
    p->sin_family   = AF_INET;
    p->sin_port     = htons(port);

    if (addr.empty())
    {
        p->sin_addr.s_addr = htonl(INADDR_ANY);
    }
    else
    {
		int ret = ParseAddr(addr, p->sin_addr);
		if(ret != 0)
			return ret;
    }
	return ::bind(_fd, &sock_addr, sizeof(sock_addr));
}

int ISocket::Connect(const std::string &remote_addr, uint16_t port)
{
    if (remote_addr.empty())
		return -2;
    struct sockaddr sock_addr;
    bzero(&sock_addr, sizeof(struct sockaddr));
    struct sockaddr_in *p = (struct sockaddr_in *)&sock_addr;
    p->sin_family = AF_INET;
	p->sin_port = htons(port);
    int ret = ParseAddr(remote_addr, p->sin_addr);
    if(ret != 0)
		return -3;
    return ::connect(_fd, &sock_addr, sizeof(sock_addr)); //-1 will return on error
}

int ISocket::Recv(void *buf, size_t len, int flags)
{
	return ::recv(_fd, buf, len, flags);
}

int ISocket::Send(const void *buf, size_t len, int flags)
{
	return ::send(_fd, buf, len, flags);
}

