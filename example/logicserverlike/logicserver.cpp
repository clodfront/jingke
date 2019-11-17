// Project:	jingke
// File:    logicserver.cpp
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
#include "logicserver.h"
#include <stddef.h>
#include <stdlib.h>
#include "test_protoc.h"
#include "log_writer.h"
#include "obj_pool.h"
#include "singleton.h"
#include "worker_fight.h"
#include "worker_top3.h"


class CSProtocolParser : public CProtocolParser
{
public:
	virtual size_t GetPackSize(const void* data_, size_t length_)
	{
		static SPackHead head;
		if(!data_ || !length_)
			return INVALID_PACK_SIZE;
		constexpr size_t needsize = offsetof(SPackHead, len) + sizeof(head.len);
		if(length_ < needsize)
			return UNDETERMINED_PACK_SIZE;
		uint32_t pack_len = ntohl(*((uint32_t*)((char*)data_ + offsetof(SPackHead, len))));
		return pack_len;
	}
	virtual uint64_t GetAsyncKey(const void* data_, size_t length_)
	{
		return 0;
	}
};

class SSProtocolParser : public CProtocolParser
{
public:
	virtual size_t GetPackSize(const void* data_, size_t length_)
	{
		static SPackHead head;
		if(!data_ || !length_)
			return INVALID_PACK_SIZE;
		constexpr size_t needsize = offsetof(SPackHead, len) + sizeof(head.len);
		if(length_ < needsize)
			return UNDETERMINED_PACK_SIZE;
		uint32_t pack_len = ntohl(*((uint32_t*)((char*)data_ + offsetof(SPackHead, len))));
		return pack_len;
	}
	virtual uint64_t GetAsyncKey(const void* data_, size_t length_)
	{
		return ntoh64(*((uint64_t*)((char*)data_ + offsetof(SPackHead, asyncid))));
	}
};

CSProtocolParser g_cs_parser;
SSProtocolParser g_ss_parser;

/////////////////

CProtocolParser* CLogicServerLike::GetClientParser(uint16_t listener_id_)
{
	switch (listener_id_) //match cfg listener_id
	{
	case 1:
		return &g_cs_parser;
	default:
		break;
	}
	return NULL;
}
CProtocolParser* CLogicServerLike::GetServiceParser(uint16_t service_id_)
{
	switch(service_id_)
	{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			return &g_ss_parser;
		default:
			break;
	}
	return NULL;
}

void CLogicServerLike::OnRecvService(const void* data_, size_t data_len_, uint16_t service_id_)
{
	LOG_ERROR("do by async work already!");
}

int CLogicServerLike::OnInit() 
{
	int ec = singleton<CObjPool<CWorkerFight>>::instance().Init(32, true); 
	if(ec)
	{
		LOG_ERROR("CObjPool<CWorkerFight> Init failed.");
		return ec;
	}
	ec = singleton<CObjPool<CWorkerRankTop3>>::instance().Init(32, true);
	if(ec)
	{
		LOG_ERROR("CObjPool<CWorkerRankTop3> Init failed.");
		return ec;
	}
	srand(time(0));
	return 0; 
}

void CLogicServerLike::OnFini() 
{ 
	LOG_INFO("CLogicServerLike::OnFini finish"); 
}

void CLogicServerLike::OnRecvClient(const void* data_, size_t data_len_, uint32_t session_id_, uint16_t listener_id_)
{
	if(!data_ || data_len_ < sizeof(SPackHead))
		return;
	if(listener_id_ < 1 || listener_id_ > 5)
		return;
	SPackHead pack_head;
	if(!ExampleMessageCoder::decode_head(data_, data_len_, pack_head))
		return;
	//LOG_DEBUG("recv session:%u len:%u cmd:%u asyncid:%lu", session_id_, pack_head.len, pack_head.cmd, pack_head.asyncid);
	switch(pack_head.cmd)
	{
		case CSP::cs_cmd_fight_req:
		{
			CWorkerFight* fight_work = singleton<CObjPool<CWorkerFight>>::instance().New();
			if(!fight_work)
			{
				LOG_ERROR("new CWorkerFight failed.");
				return;
			}
			int ec = fight_work->Init(session_id_, this, pack_head, data_, data_len_);
			if(ec)
			{
				LOG_ERROR("Init CWorkerFight failed. ec=%d", ec);
				return;
			}
			ec = fight_work->start_work();
			if(ec)
			{
				LOG_ERROR("Start CWorkerFight failed. ec=%d", ec);
				return;
			}
		}
		break;
		case CSP::cs_cmd_query_rank_top3_req:
		{
			CWorkerRankTop3* top_3 = singleton<CObjPool<CWorkerRankTop3>>::instance().New();
			if(!top_3)
			{
				LOG_ERROR("new CWorkerRankTop3 failed.");
				return;
			}
			int ec = top_3->Init(session_id_, this, pack_head, data_, data_len_);
			if(ec)
			{
				LOG_ERROR("Init CWorkerRankTop3 failed. ec=%d", ec);
				return;
			}
			ec = top_3->start_work();
			if(ec)
			{
				LOG_ERROR("Start CWorkerRankTop3 failed. ec=%d", ec);
				return;
			}			
		}
		break;
		default:
			LOG_ERROR("recv unknow cmd:%u failed. listener_id=%u", pack_head.cmd, listener_id_);
			break;
	}
}