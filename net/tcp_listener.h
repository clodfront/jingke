// Project:	jingke
// File:    tcp_listener.h
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

#ifndef _tcp_listener_h_
#define _tcp_listener_h_

#include <string.h>
#include "socket_eventer.h"
#include "socket_evneter_type.h"
#include "tcp_socket.h"
#include "socket_point.h"
#include "net_config.h"

namespace jingke {

class CTcpListener : public CSocketEventer
{
public:
    CTcpListener(){}
    virtual ~CTcpListener(){}
public:
    virtual int Type() final { return e_se_type_tcp_listener; }
    virtual ISocket& Socket() final { return _sock; }
    virtual int OnRecvble() override;
	virtual int OnSendble() override {return 0;}
    virtual void OnError() override { Close(); }
public:
    int Init(const STcpListenerCfg& cfg_);
	int Listen();
private:
    CTcpSocket _sock;
    STcpListenerCfg _cfg;
};

} ///namespace jingke

#endif//_tcp_listener_h_
