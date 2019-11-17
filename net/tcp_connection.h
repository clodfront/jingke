// Project:	jingke
// File:    tcp_connection.h
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

#ifndef _tcp_connection_h_
#define _tcp_connection_h_

#include "socket_connection_base.h"
#include "socket_evneter_type.h"
#include "tcp_socket.h"

namespace jingke {

class CTcpConnection : public ISocketConnectionBase
{
public:
    CTcpConnection():_listener_id(0),_session_id(0){}
    virtual ~CTcpConnection(){ _listener_id = 0; _session_id = 0;}
    void SetListenerID(uint16_t id_) { _listener_id = id_; }
    uint16_t GetListenerID() { return _listener_id; }
    void SetSessionID(uint32_t session_id_) {_session_id = session_id_;}
	uint32_t GetSessionID() {return _session_id;}
    static CTcpConnection* Create();
    static void Free(CTcpConnection* conn_obj_);
public:
    virtual int Type() final { return e_se_type_tcp_connection; }
    virtual ISocket& Socket() final { return _sock; }
    virtual void Close() override;
private:
    CTcpSocket  _sock;
    uint16_t    _listener_id;
    uint32_t    _session_id;
};

} ///namespace jingke

#endif//_tcp_connection_h_
