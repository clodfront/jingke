// Project:	jingke
// File:    task_do_fight.cpp
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
#include "task_do_fight.h"
#include "logicserver.h"
#include "test_protoc.h"
#include "test_ss.pb.h"
#include "log_writer.h"

int CTaskDoFight::Init(CLogicServerLike* logic_svr_, uint32_t fighterid_a_, uint32_t fighterid_b_, uint32_t power_a_, uint32_t power_b_)
{
    if(!logic_svr_ || !fighterid_a_ || !fighterid_b_)
        return -1;
    _logic_svr = logic_svr_;
    _fighterid_a = fighterid_a_;
    _fighterid_b = fighterid_b_;
    _power_a = power_a_;
    _power_b = power_b_;
    return 0;
}


bool CTaskDoFight::on_start()
{
    SSP::SsDoFightReq req;
    req.set_fighterid_a(_fighterid_a);
    req.set_fighterid_b(_fighterid_b);

    req.set_power_a(_power_a);
    req.set_power_b(_power_b);

    SPackHead req_head;
    req_head.cmd = SSP::ss_cmd_fight_req;
    req_head.asyncid = get_async_id();
    size_t encode_size = 0;
    static char req_pack_buff[1024];
    if(!ExampleMessageCoder::encode(req_pack_buff, sizeof(req_pack_buff), req_head, &req, &encode_size))
    {
        LOG_DEBUG("%s", __FUNCTION__);
        set_stop_code(-1);
        return true;
    }

#define FIGHTSVR1_CFG_ID 3
#define FIGHTSVR2_CFG_ID 4

    uint16_t server_id = (rand()%100 < 50) ? FIGHTSVR1_CFG_ID : FIGHTSVR2_CFG_ID;
    int ret = _logic_svr->SendToService(server_id, req_pack_buff, encode_size);
    if(ret != 0)
    {
        LOG_DEBUG("%s", __FUNCTION__);
        set_stop_code(-2);
        return true;
    }

    return false;
}

bool CTaskDoFight::on_awaken(const void* data_, size_t len_) 
{
    if(!data_ || !len_)
    {
        LOG_DEBUG("%s", __FUNCTION__);
        set_stop_code(-1);
        return true;
    }

    SPackHead pack_head;
    if(!ExampleMessageCoder::decode_head(data_, len_, pack_head))
    {   
        LOG_DEBUG("%s", __FUNCTION__);
        set_stop_code(-2);
        return true;
    }
    if(pack_head.cmd != SSP::ss_cmd_fight_rsp || pack_head.asyncid != get_async_id())
    {
        LOG_DEBUG("%s", __FUNCTION__);
        set_stop_code(-3);
        return true;
    }

    SSP::SsDoFightRsp rsp;
    if(!ExampleMessageCoder::decode_body(data_, len_, rsp))
    {
        LOG_DEBUG("%s", __FUNCTION__);
        set_stop_code(-4);
        return true;  
    }

    if(rsp.errcode())
    {
        LOG_DEBUG("%s", __FUNCTION__);
        set_stop_code(-5);
        return true;      
    }

    _winner = rsp.winnerid();
    return true;
}