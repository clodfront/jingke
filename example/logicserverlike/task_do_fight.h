// Project:	jingke
// File:    task_do_fight.h
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
#ifndef __task_do_fight__
#define __task_do_fight__

#include "async_worker.h"

using namespace jingke;

class CLogicServerLike;

class CTaskDoFight : public IAsyncTask
{
public:
	CTaskDoFight():_logic_svr(NULL),_fighterid_a(0),_fighterid_b(0), _power_a(0), _power_b(0),_winner(0){}
	~CTaskDoFight(){_logic_svr = NULL; _fighterid_a = 0; _fighterid_b=0; _power_a=0; _power_b=0;_winner=0;}
	int Init(CLogicServerLike* logic_svr_, uint32_t fighterid_a_, uint32_t fighterid_b_, uint32_t power_a_, uint32_t power_b_);
	uint32_t GetWinner(){return _winner;}
public:
	virtual bool on_start() override;			//开始任务 返回true表示任务终止，否则表示被挂起
	virtual bool on_awaken(const void* data_, size_t len_) override;	//唤醒任务 返回值意义同on_start， 默认返回true
	virtual uint32_t time_out_time() override {return 1000;}	//超时时间, 0表示不超时，单位毫秒
	const char* name() const override { return "CTaskDoFight";}
private:
	CLogicServerLike* _logic_svr;
	uint32_t _fighterid_a;
	uint32_t _fighterid_b;
	uint32_t _power_a;
	uint32_t _power_b;
	uint32_t _winner;
};

#endif //__task_do_fight__


