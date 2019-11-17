// Project:	jingke
// File:    main.cpp
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
#include <stddef.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "server.h"
#include "log_writer.h"
#include "test_protoc.h"
#include "test_cs.pb.h"


using namespace std;
using namespace jingke;

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

CSProtocolParser g_parser;

class CClinetLike : public IServer
{
public:
	virtual CProtocolParser* GetClientParser(uint16_t listener_id_)
	{
		//no cfg listener_id
		return NULL;
	}
	virtual CProtocolParser* GetServiceParser(uint16_t service_id_)
	{
		//no peer service cfg
		if(1 == service_id_)
			return &g_parser;
		return NULL;
	}
	virtual void OnRecvClient(const void* data_, size_t data_len_, uint32_t session_id_, uint16_t listener_id_)
	{
	}
	virtual void OnRecvService(const void* data_, size_t data_len_, uint16_t service_id_)
	{
		if(!data_ || data_len_ < sizeof(SPackHead))
			return;
		if(service_id_ != 1)
			return;
		SPackHead pack_head;
		if(!ExampleMessageCoder::decode_head(data_, data_len_, pack_head))
			return;
		switch (pack_head.cmd)
		{
		case CSP::cs_cmd_fight_rsp:
			{
				CSP::CsFightRsp rsp;
				if(!ExampleMessageCoder::decode_body(data_, data_len_, rsp))
					return;
				if(rsp.errcode())
				{
					LOG_ERROR("fight failed ec:%d", rsp.errcode());
				}
				else
				{
					LOG_INFO("fight done winner:%u", rsp.winnerid());
				}
			}
			break;
		case CSP::cs_cmd_query_rank_top3_rsp:
			{
				CSP::CsRankTop3Rsp rsp;
				if(!ExampleMessageCoder::decode_body(data_, data_len_, rsp))
					return;
				if(rsp.errcode())
				{
					LOG_ERROR("query top3 failed ec:%d", rsp.errcode());
				}
				else
				{
					LOG_INFO("top3 info : valid num:%u", rsp.validnum());
					for(int i=0; i<rsp.ids_size(); ++i)
					{
						LOG_INFO("top3 %dth:%u", i+1, rsp.ids(i));
					}
				}
			}
			break;
		default:
			LOG_ERROR("recv unknow cmd:%u failed. service_id=%u", pack_head.cmd, service_id_);
		}
	}
protected:
	virtual int OnInit() 
	{
		srand(time(0));

		uint32_t id = SetMonoTimer(std::bind(&CClinetLike::ReqFight, this, std::placeholders::_1 ,std::placeholders::_2)
					,3000 ,2000);
		if(!id)
		{
			LOG_ERROR("SetMonoTimer failed");
			return -1;
		}
		id = SetMonoTimer(std::bind(&CClinetLike::GetTop3, this, std::placeholders::_1 ,std::placeholders::_2)
					,10000 ,60000);
		if(!id)
		{
			LOG_ERROR("SetMonoTimer failed");
			return -1;
		}
		LOG_INFO("CClinetLike::OnInit OK");
		return 0;
	}
	virtual void OnFini() { LOG_INFO("CClinetLike::OnFini finish"); }
private:
	void ReqFight(mstime_t mono_time_, uint16_t step_)
	{
		static char req_pack_buff[1024];
		CSP::CsFightReq req;
		uint32_t fighterid_a = 0;
		uint32_t fighterid_b = 0;
		fighterid_a = rand()%100 + 1;
		do
		{
			fighterid_b = rand()%100 + 1;
		} while (fighterid_b == fighterid_a);
		req.set_fighterid_a(fighterid_a);
		req.set_fighterid_b(fighterid_b);


		SPackHead req_head;
		req_head.cmd = CSP::cs_cmd_fight_req;
		req_head.asyncid = 0;
		size_t encode_size = 0;
		if(!ExampleMessageCoder::encode(req_pack_buff, sizeof(req_pack_buff), req_head, &req, &encode_size))
			return;
		int ret = SendToService(1, req_pack_buff, encode_size);
		if(ret != 0)
		{
			LOG_ERROR("SendToService failed, ret:%d", ret);
		}
	}

	void GetTop3(mstime_t mono_time_, uint16_t step_)
	{
		static char req_pack_buff[1024];
		CSP::CsRankTop3Req req;
		req.set_order(0);

		SPackHead req_head;
		req_head.cmd = CSP::cs_cmd_query_rank_top3_req;
		req_head.asyncid = 0;
		size_t encode_size = 0;
		if(!ExampleMessageCoder::encode(req_pack_buff, sizeof(req_pack_buff), req_head, &req, &encode_size))
			return;
		int ret = SendToService(1, req_pack_buff, encode_size);
		if(ret != 0)
		{
			LOG_ERROR("SendToService failed, ret:%d", ret);
		}
	}


};




int main(int argc, char** argv)
{
	if(argc < 2) 
	{
		cout << "argc < 2" << endl;
		return -1;
	}

	CClinetLike svr;
	int ret = svr.Init(argv[1]);
	if(ret != 0)
	{
		cout << "svr.Init fail. ret:" << ret << endl;
		return -2;
	}
	svr.Run();

	return 0;
}
