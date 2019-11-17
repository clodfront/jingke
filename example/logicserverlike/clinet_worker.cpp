// Project:	jingke
// File:    clinet_worker.cpp
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
#include "clinet_worker.h"
#include "log_writer.h"
#include "logicserver.h"

int IClinetWork::Init(uint32_t session_id_, CLogicServerLike* logic_svr_, const SPackHead& req_head_, const void* req_data_, size_t req_data_len_)
{
    if(!session_id_ || !req_data_ || !req_data_ || !logic_svr_)
        return -1;
    _session_id = session_id_;
    _logic_svr = logic_svr_;
    _req_head = req_head_;

    if(ReqBody())
    {
        ReqBody()->Clear();
        if(!ExampleMessageCoder::decode_body(req_data_, req_data_len_, *ReqBody()))
            return -1;
    }

    return OnInit();
}

