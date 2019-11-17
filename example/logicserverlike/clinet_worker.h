// Project:	jingke
// File:    clinet_worker.h
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
#ifndef __clinet_work_h__
#define __clinet_work_h__

#include "async_worker.h"
#include "test_protoc.h"

using namespace jingke;

class CLogicServerLike;

class IClinetWork : public IAsyncWorker
{
public:
    IClinetWork():_session_id(0),_logic_svr(NULL){}
    virtual ~IClinetWork(){_session_id=0;_logic_svr = NULL;}
    int Init(uint32_t session_id_, CLogicServerLike* logic_svr_, const SPackHead& req_head_, const void* req_data_, size_t req_data_len_);
    const SPackHead& ReqHead() {return _req_head;}
    virtual google::protobuf::Message* ReqBody() = 0;
protected:
    virtual int OnInit() = 0;
protected:
    uint32_t    _session_id;
    CLogicServerLike* _logic_svr;
    SPackHead   _req_head;
};

#endif //__clinet_work_h__


