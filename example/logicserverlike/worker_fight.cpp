// Project:	jingke
// File:    worker_fight.cpp
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
#include "worker_fight.h"
#include "log_writer.h"
#include "singleton.h"
#include "obj_pool.h"
#include "logicserver.h"

int CWorkerFight::OnInit()
{
	int ec = 0;
	ec = _get_power_from_db1.Init(_logic_svr, _req_body.fighterid_a());
	if(ec) return -1;
	ec = _get_power_from_db2.Init(_logic_svr, _req_body.fighterid_b());
	if(ec) return -2;
	if(!_con_get_power.add_task(&_get_power_from_db1)) return -3;
	if(!_con_get_power.add_task(&_get_power_from_db2)) return -4;

	if(!this->push_task(&_con_get_power)) return -9;
	LOG_DEBUG("push task _con_get_power");
	return 0;
}

bool CWorkerFight::on_task_stop(IAsyncTask* task_)
{
	if(task_ == &_get_power_from_db1 || task_ == &_get_power_from_db2)
	{
		if( task_->get_stop_code() != 0)
		{
			LOG_ERROR("get fighter power failed.");
			this->set_stop_code(-1);
			return true;
		}
	}
	if(task_ == &_do_fight)
	{
		if( _do_fight.get_stop_code() != 0)
		{
			this->set_stop_code(-2);
			LOG_ERROR("do fight power failed.");
			return true;
		}
		int ret = _report_rank.Init(_logic_svr, _do_fight.GetWinner());
		if(ret != 0)
		{
			this->set_stop_code(-3);
			LOG_ERROR("do report rank init failed.");
			return true;
		}
		if(this->push_task(&_report_rank))
		{
			LOG_DEBUG("push task _report_rank");
		}
	}
	if(task_ == &_report_rank)
	{
		if( _report_rank.get_stop_code() != 0)
		{
			this->set_stop_code(-4);
			LOG_ERROR("report rank failed.");
			return true;
		}
	}
	return false;
}

bool CWorkerFight::on_concurrenter_stop(CAsyncTaskConcurrenter* concurrenter_)
{
	if(concurrenter_ == &_con_get_power)
	{
		int ret = _do_fight.Init(_logic_svr, _req_body.fighterid_a(), _req_body.fighterid_b()
							, _get_power_from_db1.GetPower(), _get_power_from_db2.GetPower());
		if(ret != 0)
		{
			this->set_stop_code(-5);
			LOG_ERROR("do fight init failed.");
			return true;
		}
		if(this->push_task(&_do_fight))
		{
			LOG_DEBUG("push task _do_fight");
		}
		else
		{
			this->set_stop_code(-6);
			LOG_ERROR("push task _do_fight failed.");
			return true;
		}
		
	}
	return false;
}

void CWorkerFight::on_work_stop()
{
	CSP::CsFightRsp rsp;
	if(int ec = get_stop_code())
	{
		LOG_ERROR("fight work failed. ec=%d", ec);
		rsp.set_errcode(ec);
	}
	else
	{
		LOG_DEBUG("fight work ok %u vs %u winner is %u and report winner to rank done.", _req_body.fighterid_a()
									, _req_body.fighterid_b(), _do_fight.GetWinner());
		rsp.set_errcode(0);
		rsp.set_winnerid(_do_fight.GetWinner());
	}

	SPackHead rsp_head;
	rsp_head.cmd = CSP::cs_cmd_fight_rsp;
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

	singleton<CObjPool<CWorkerFight>>::instance().Free(this);
}