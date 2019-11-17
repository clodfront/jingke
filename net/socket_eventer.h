// Project:	jingke
// File:    socket_eventer.h
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

#ifndef _socket_eventer_h_
#define _socket_eventer_h_

#include "socket_event_base.h"
#include "protocol_parser.h"
#include "socket_base.h"

namespace jingke {

class CEpoller;

class CSocketEventer : public ISocketEvent
{
public:
    CSocketEventer():_parser(NULL), _epoller(NULL) {}
    virtual ~CSocketEventer() {if(_parser) _parser = NULL; if(_epoller) _epoller = NULL;}
public:
	void SetParser(CProtocolParser* parser_){_parser = parser_;}
	CProtocolParser* GetParser(){return _parser;}
    void SetEpoller(CEpoller* epoller_);
	CEpoller* GetEpoller();
public:
    virtual void* PrivateData() { return NULL; }
    virtual void Close();
    virtual int Type() = 0;
    virtual ISocket& Socket() = 0;
protected:
    CProtocolParser* _parser;
    CEpoller* _epoller;
};

} ///namespace jingke

#endif//_socket_eventer_h_