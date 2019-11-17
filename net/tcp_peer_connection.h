// Project:	jingke
// File:    tcp_peer_connection.h
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

#ifndef _tcp_peer_connection_h_
#define _tcp_peer_connection_h_

#include "socket_connection_base.h"
#include "socket_evneter_type.h"
#include "tcp_socket.h"
#include "timer.h"

namespace jingke {

class CTcpPeerConnection : public ISocketConnectionBase
{
public:
    CTcpPeerConnection():_state(0),_timer_mgr(NULL),_reconnect_timer_id(CTimerMono::bad_timer_id),_delay_time(0),_service_id(0){}
    virtual ~CTcpPeerConnection();
public:
    void    SetTimerMgr(CTimerMono* timer_mgr_){_timer_mgr = timer_mgr_;}
    int     GetState() {return _state;}
    int     ConnectToService(); //support ipv4 addr、domain name
    void     SetServiceID(uint16_t id_) { _service_id = id_; }
    uint16_t GetServiceID() { return _service_id; }
public:
    virtual int Type() final { return e_se_type_tcp_peer_connection; }
    virtual ISocket& Socket() final { return _sock; }
    virtual int OnSendble() override;
    virtual void OnError() override;
    virtual void Close() override;
    virtual int Send(const void* data_, size_t len_, bool close_after_send_ = false) override;
private:
    void    AutoReConnect(mstime_t now_, uint16_t round_);
    void    DelayReconnect();
protected:
    CTcpSocket _sock;
    int _state; enum{e_close = 0, e_wait_reconnect, e_connecting, e_connected};
    CTimerMono* _timer_mgr;
    uint32_t _reconnect_timer_id;
    uint32_t _delay_time;
    uint16_t _service_id;
};

} ///namespace jingke

#endif//_tcp_peer_connection_h_
