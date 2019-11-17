// Project:	jingke
// File:    server.cpp
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
#include "server.h"
#include <stdio.h>
#include "log_writer.h"
#include "sys_op.h"
#include "singleton.h"
#include "server_core.h"

using namespace jingke;

////////////////////////////////////////////////////////////////
//s_server_need_stoped and s_server_need_reload will ctrl all the IServer implements
static bool s_server_need_stoped = false;
static bool s_server_need_reload = false;
void SignalHandler(int signo)
{
	switch (signo)
	{
	case SIGTERM:
		if(!s_server_need_stoped)
			s_server_need_stoped = true;
		break;
	case SIGUSR1:
		if(!s_server_need_reload)
			s_server_need_reload = true;
		break;
	}
}
////////////////////////////////////////////////////////////////


IServer::IServer()
{
}

IServer::~IServer()
{
}

int IServer::Init(const char* std_cfg_file_, bool daemon_)
{
	int ret = 0;

	if(daemon_)
	{
		CSysOp::StartDaemon();
	}
		
	CSysOp::OverrideSignal(SIGTERM, SignalHandler);
	CSysOp::OverrideSignal(SIGUSR1, SignalHandler);

	ret = singleton<CServerCore>::instance().Init(this, std_cfg_file_);
	if(ret != 0)
	{
		LOG_ERROR("init net core failed. ret:%d", ret);
		return -1;
	}

	return OnInit();
}

void IServer::Run()
{
	while(!s_server_need_stoped)
	{
		singleton<CServerCore>::instance().Drive(10); //TODO config it
		OnUpData();
	}

	singleton<CServerCore>::instance().Fini();
	OnFini();
	singleton<CLog_writer>::instance().shutdown();
}

int  IServer::SendToClient(uint32_t session_id_, const void* data_, size_t data_len_, bool close_after_send_)
{
	return singleton<CServerCore>::instance().SendToClient(session_id_, data_, data_len_, close_after_send_);
}

int  IServer::SendToService(uint16_t service_id_, const void* data_, size_t data_len_, bool close_after_send_)
{
	return singleton<CServerCore>::instance().SendToService(service_id_, data_, data_len_, close_after_send_);
}

uint32_t IServer::SetWallTimer(delay_call_t call_back_, uint32_t delay_time_, uint32_t interval_, uint16_t repeat_times_)
{
	return singleton<CServerCore>::instance().SetWallTimer(call_back_, delay_time_, interval_, repeat_times_);
}
uint32_t IServer::SetMonoTimer(delay_call_t call_back_, uint32_t delay_time_, uint32_t interval_, uint16_t repeat_times_)
{
	return singleton<CServerCore>::instance().SetMonoTimer(call_back_, delay_time_, interval_, repeat_times_);
}

mstime_t IServer::GetNow(bool mono_, bool exact_)
{
	return singleton<CServerCore>::instance().GetNow(mono_, exact_);
}
