// Project:	jingke
// File:    worker_top3.cpp
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
#include "worker_top3.h"
#include "log_writer.h"
#include "singleton.h"
#include "obj_pool.h"
#include "logicserver.h"
#include "test_cs.pb.h"

int CWorkerRankTop3::OnInit()
{
	int ec = 0;
	ec = _top3_rank.Init(_logic_svr);
	if(ec) return -1;
	ec = _http_test.Init();
	if(ec) return -2;
	if(!this->push_task(&_top3_rank)) return -3;
	LOG_DEBUG("push task _top3_rank");
	if(!this->push_task(&_http_test)) return -4;
	LOG_DEBUG("push task _http_test");
	return 0;
}

bool CWorkerRankTop3::on_task_stop(IAsyncTask* task_)
{
	if(task_ == &_top3_rank)
	{
		if( _top3_rank.get_stop_code() != 0)
		{
			this->set_stop_code(-5);
			LOG_ERROR("get top3 rank failed.");
			return true;
		}
	}
	if(task_ == &_http_test)
	{
		if( _http_test.get_stop_code() != 0)
		{
			this->set_stop_code(-6);
			LOG_ERROR("http test failed.");
			return true;
		}
	}
	return false;
}

void CWorkerRankTop3::on_work_stop()
{
	CSP::CsRankTop3Rsp rsp;
	if(int ec = get_stop_code())
	{
		LOG_ERROR("top3 work failed. ec=%d", ec);
		rsp.set_errcode(ec);
	}
	else
	{
		const std::vector<uint32_t> & top3 = _top3_rank.GetTop3();
		rsp.set_errcode(0);
		uint32_t validnum = 0;
		for(size_t i=0; i<3 && i<top3.size(); ++i)
		{
			rsp.add_ids(top3[i]);
			validnum++;
		}
		rsp.set_validnum(validnum);
	}

	SPackHead rsp_head;
	rsp_head.cmd = CSP::cs_cmd_query_rank_top3_rsp;
	rsp_head.asyncid = ReqHead().asyncid;
	size_t encode_size = 0;
	static char rsp_pack_buff[1024];

	do
	{
		if(!ExampleMessageCoder::encode(rsp_pack_buff, sizeof(rsp_pack_buff), rsp_head, &rsp, &encode_size))
			break;
		if(_logic_svr->SendToClient(_session_id, &rsp_pack_buff, encode_size) != 0)
			LOG_ERROR("send to session failed.");
	}while(0);

	_http_test.WebConnection().Close();

	singleton<CObjPool<CWorkerRankTop3>>::instance().Free(this);
}