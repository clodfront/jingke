// Project:	jingke
// File:    socket_connection_base.h
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

#ifndef _socket_connection_base_h_
#define _socket_connection_base_h_

#include "socket_eventer.h"
#include "socket_point.h"
#include "stream_buffer.h"

namespace jingke {

class ISocketConnectionBase : public CSocketEventer
{
public:
    ISocketConnectionBase():_close_on_send_over(false){}
    virtual ~ISocketConnectionBase(){_close_on_send_over=false;FreeRSBuff();}
public:
    int	SetRecvBuff(char* buff_ptr_, size_t buff_len_);
	int	SetSendBuff(char* buff_ptr_, size_t buff_len_);
    void SetPeerPoint(const CSocketPoint& peer_point_);
    const CSocketPoint& GetPeerPoint();
public:
    virtual void Close() override;
    virtual int OnRecvble() override;
	virtual int OnSendble() override;
    virtual void OnError() override;
    virtual int Send(const void* data_, size_t len_, bool close_after_send_ = false);
public:
    virtual int OnRecvPack(const void* data_, size_t len_, uint64_t async_key_);
protected:
    void FreeRSBuff();
    void ClearRSBuff();
protected:
    CSocketPoint _peer_point;
	CStreamBuffer _recv_buff;
	CStreamBuffer _send_buff;
    bool _close_on_send_over;
};

} ///namespace jingke

#endif//_socket_connection_base_h_
