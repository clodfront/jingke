// Project:	jingke
// File:    worker_fight.h
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
#ifndef __fight_work_h__
#define __fight_work_h__

#include "clinet_worker.h"
#include "task_get_power_from_db.h"
#include "task_do_fight.h"
#include "task_report_rank.h"
#include "test_cs.pb.h"

using namespace jingke;

class CWorkerFight : public IClinetWork
{
public:
	CWorkerFight(){}
	virtual ~CWorkerFight(){}
protected:
	virtual bool on_task_stop(IAsyncTask* task_) override;
	virtual bool on_concurrenter_stop(CAsyncTaskConcurrenter* concurrenter_) override;
	virtual void on_work_stop() override;
	virtual google::protobuf::Message* ReqBody() override {return &_req_body;}
	virtual int OnInit() override;
private:
	CSP::CsFightReq _req_body;
	CGetPowerFromDB _get_power_from_db1;
	CGetPowerFromDB _get_power_from_db2;
	CAsyncTaskConcurrenter _con_get_power;
	CTaskDoFight _do_fight;
	CTaskReportRank _report_rank;
};

#endif //__fight_work_h__


