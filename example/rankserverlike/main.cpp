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
#include <vector>
#include <unordered_map>
#include "server.h"
#include "log_writer.h"
#include "test_protoc.h"
#include "test_ss.pb.h"



using namespace std;
using namespace jingke;

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
		return 0;
	}
};

SSProtocolParser g_parser;

class CRankServerLike : public IServer
{
public:
	virtual CProtocolParser* GetClientParser(uint16_t listener_id_)
	{
		switch (listener_id_) //match cfg listener_id
		{
		case 1:
			return &g_parser;
		default:
			break;
		}
		return NULL;
	}
	virtual CProtocolParser* GetServiceParser(uint16_t service_id_)
	{
		//no peer service cfg
		return NULL;
	}
	virtual void OnRecvClient(const void* data_, size_t data_len_, uint32_t session_id_, uint16_t listener_id_)
	{
		if(!data_ || data_len_ < sizeof(SPackHead))
			return;
		if(listener_id_ != 1)
			return;
		SPackHead pack_head;
		if(!ExampleMessageCoder::decode_head(data_, data_len_, pack_head))
			return;
		//LOG_DEBUG("recv session:%u len:%u cmd:%u asyncid:%lu", session_id_, pack_head.len, pack_head.cmd, pack_head.asyncid);
		static char rsp_pack_buff[1024];
		switch(pack_head.cmd)
		{
			case SSP::ss_cmd_reportrank_req:
			{
				SSP::SsReportRankReq req;
				SSP::SsReportRankRsp rsp;
				if(!ExampleMessageCoder::decode_body(data_, data_len_, req))
					return;				
				if(!req.fighterid()) //suppose 0 is invalid
				{
					LOG_ERROR("invalid reoprt_rank_req. session_id=%u", session_id_);
					rsp.set_errcode(1);
				}
				else
				{
					this->Report(req.fighterid());
					rsp.set_errcode(0);
				}
				
				SPackHead rsp_head;
				rsp_head.cmd = SSP::ss_cmd_reportrank_rsp;
				rsp_head.asyncid = pack_head.asyncid;
				size_t encode_size = 0;
				if(!ExampleMessageCoder::encode(rsp_pack_buff, sizeof(rsp_pack_buff), rsp_head, &rsp, &encode_size))
					return;
				int ret = SendToClient(session_id_, rsp_pack_buff, encode_size);
				if(ret != 0)
				{
					LOG_ERROR("SendToClient session_id:%u cmd:%u size:%zu failed. ret=%d", session_id_, SSP::ss_cmd_reportrank_rsp, encode_size, ret);
				}
			}
			break;
			case SSP::ss_cmd_query_rank_top3_req:
			{
				SSP::SsDoRankTop3Req req;
				SSP::SsDoRankTop3Rsp rsp;
				if(!ExampleMessageCoder::decode_body(data_, data_len_, req))
					return;
				uint32_t validnum = 0;
				for (size_t i = 0; i < this->_top3.size() && i < 3; ++i)
				{
					rsp.add_ids(this->_top3[i]);
					validnum++;
				}
				rsp.set_validnum(validnum);
				rsp.set_errcode(0);

				SPackHead rsp_head;
				rsp_head.cmd = SSP::ss_cmd_query_rank_top3_rsp;
				rsp_head.asyncid = pack_head.asyncid;
				size_t encode_size = 0;
				if(!ExampleMessageCoder::encode(rsp_pack_buff, sizeof(rsp_pack_buff), rsp_head, &rsp, &encode_size))
					return;
				int ret = SendToClient(session_id_, rsp_pack_buff, encode_size);
				if(ret != 0)
				{
					LOG_ERROR("SendToClient session_id:%u cmd:%u size:%zu failed. ret=%d", session_id_, SSP::ss_cmd_query_rank_top3_rsp, encode_size, ret);
				}
			}
			break;
			default:
				LOG_ERROR("recv unknow cmd:%u failed. session_id=%u", pack_head.cmd, session_id_);
				return;
		}
	}
	virtual void OnRecvService(const void* data_, size_t data_len_, uint16_t service_id_)
	{
		//no peer service cfg
	}
protected:
	virtual int OnInit() { return 0; }
	virtual void OnFini() { LOG_INFO("CRankServerLike::OnFini finish"); }
private:
	void Report(uint32_t fid_)
	{
		uint32_t& times = _win_times_map[fid_];
		times++;
		for(auto& i : _top3)
		{
			if(i == fid_)
			{
				std::sort(_top3.begin(), _top3.end(), [this](uint32_t lhs, uint32_t rhs) {return this->_win_times_map[lhs] > this->_win_times_map[rhs];} );
				return;
			}
		}
		_top3.push_back(fid_);
		std::sort(_top3.begin(), _top3.end(), [this](uint32_t lhs, uint32_t rhs) {return this->_win_times_map[lhs] > this->_win_times_map[rhs];} );
		while(_top3.size() > 3)
			_top3.pop_back();
	}
	
private:
	std::unordered_map<uint32_t, uint32_t> _win_times_map;
	std::vector<uint32_t> _top3;
};




int main(int argc, char** argv)
{
	if(argc < 2) 
	{
		cout << "argc < 2" << endl;
		return -1;
	}

	CRankServerLike svr;
	int ret = svr.Init(argv[1]);
	if(ret != 0)
	{
		cout << "svr.Init fail. ret:" << ret << endl;
		return -2;
	}
	svr.Run();

	return 0;
}
