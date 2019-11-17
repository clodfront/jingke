// Project:	jingke
// File:    task_get_power_from_db.cpp
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
#include "task_get_power_from_db.h"
#include "logicserver.h"
#include "test_protoc.h"
#include "log_writer.h"
#include "test_ss.pb.h"


int CGetPowerFromDB::Init(CLogicServerLike* logic_svr_, uint32_t fighterid_)
{
    if(!logic_svr_ || !fighterid_)
        return -1;
    _logic_svr = logic_svr_;
    _fighterid = fighterid_; 
    return 0;
}

bool CGetPowerFromDB::on_start()
{
    SSP::SsQueryPowerReq req;
    req.set_fighterid(_fighterid);

    SPackHead req_head;
    req_head.cmd = SSP::ss_cmd_query_power_req;
    req_head.asyncid = get_async_id();
    size_t encode_size = 0;
    static char req_pack_buff[1024];
    if(!ExampleMessageCoder::encode(req_pack_buff, sizeof(req_pack_buff), req_head, &req, &encode_size))
    {
        set_stop_code(-1);
        return true;
    }

#define DBSVR1_CFG_ID 1
#define DBSVR2_CFG_ID 2

    uint16_t db_server_id = (rand()%100 < 50) ? DBSVR1_CFG_ID : DBSVR2_CFG_ID;
    int ret = _logic_svr->SendToService(db_server_id, req_pack_buff, encode_size);
    if(ret != 0)
    {
        set_stop_code(-2);
        return true;
    }

    return false;
}

bool CGetPowerFromDB::on_awaken(const void* data_, size_t len_)
{
    if(!data_ || !len_)
    {
        LOG_ERROR("Invalid Input");
        set_stop_code(-1);
        return true;
    }
    SPackHead pack_head;
    if(!ExampleMessageCoder::decode_head(data_, len_, pack_head))
    {   
        set_stop_code(-2);
        return true;
    }
    if(pack_head.cmd != SSP::ss_cmd_query_power_rsp || pack_head.asyncid != get_async_id())
    {
        set_stop_code(-3);
        return true;
    }

    SSP::SsQueryPowerRsp rsp;
    if(!ExampleMessageCoder::decode_body(data_, len_, rsp))
    {
        set_stop_code(-4);
        return true;  
    }

    if(rsp.errcode())
    {
        set_stop_code(-5);
        return true;      
    }

    _power = rsp.power();
    return true;
}

