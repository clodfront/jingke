// Project:	jingke
// File:    task_top3_rank.h
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
#ifndef __top3_rank__
#define __top3_rank__

#include "async_worker.h"
#include <vector>

using namespace jingke;

class CLogicServerLike;

class CTaskTop3Rank : public IAsyncTask
{
public:
	CTaskTop3Rank():_logic_svr(NULL){}
	~CTaskTop3Rank(){_logic_svr = NULL;}
	int Init(CLogicServerLike* logic_svr_);
public:
	virtual bool on_start() override;			//开始任务 返回true表示任务终止，否则表示被挂起
	virtual bool on_awaken(const void* data_, size_t len_) override;	//唤醒任务 返回值意义同on_start， 默认返回true
	virtual uint32_t time_out_time() override {return 1000;}	//超时时间, 0表示不超时，单位毫秒
	const char* name() const override { return "CTaskTop3Rank";}
	const std::vector<uint32_t>& GetTop3() const {return _top3;}
private:
	CLogicServerLike* _logic_svr;
	std::vector<uint32_t> _top3;
};

#endif //__top3_rank__


