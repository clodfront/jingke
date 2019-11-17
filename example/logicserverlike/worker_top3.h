// Project:	jingke
// File:    worker_top3.h
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
#ifndef __work_top3_h__
#define __work_top3_h__

#include "clinet_worker.h"
#include "task_top3_rank.h"
#include "task_http_test.h"
#include "test_cs.pb.h"

using namespace jingke;

class CWorkerRankTop3 : public IClinetWork
{
public:
	CWorkerRankTop3(){}
	virtual ~CWorkerRankTop3(){}
protected:
	virtual bool on_task_stop(IAsyncTask* task_) override;
	virtual bool on_concurrenter_stop(CAsyncTaskConcurrenter* concurrenter_) override { return false; }
	virtual void on_work_stop() override;
	virtual google::protobuf::Message* ReqBody() override {return &_req_body;}
	virtual int OnInit() override;
private:
	CSP::CsRankTop3Req _req_body;
	CTaskTop3Rank _top3_rank;
	CTaskHttpTest _http_test;
};

#endif //__work_top3_h__


