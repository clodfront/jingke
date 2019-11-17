// Project:	jingke
// File:    socket_base.h
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

#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <sys/socket.h>
#include <string>
#include "omygod.h"

namespace jingke {

#ifndef INVALID_SOCKET
#define INVALID_SOCKET -1
#endif

class ISocket
{
public:
	ISocket();
	virtual ~ISocket();
public: //UDP&TCP公共接口
	int		Create();
	bool	Valid();
	void	Close();
	int 	Shutdown(int iHow);
	int		GetFd();
	void	SetFd(int fd);
	int 	SetNonblock();
	int 	SetReuseAddr();
	int		SetSendBufferSize(int size);
	int		SetRecvBufferSize(int size);
	int		Bind(const std::string &addr, uint16_t port);
	int		Connect(const std::string &remote_addr, uint16_t port);
	int		Recv(void *buf, size_t len, int flags=0);
	int		Send(const void *buf, size_t len, int flags=0);
public:
	virtual int	Type() = 0;
protected:
	//File descriptor
	int 	_fd;
};

} ///namespace jingke

#endif  ///_SOCKET_H_


