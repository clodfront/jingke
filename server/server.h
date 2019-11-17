// Project:	jingke
// File:    server.h
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

#ifndef _SERVER_H_
#define _SERVER_H_

#include <stdint.h>
#include <stdlib.h>
#include "timer.h"
#include "clock_time.h"
#include "protocol_parser.h"

namespace jingke {

class IServer
{
public:
	IServer();
	virtual ~IServer();
public:
	int Init(const char* std_cfg_file_, bool daemon_=true);
	void Run();
	int SendToClient(uint32_t session_id_, const void* data_, size_t data_len_, bool close_after_send_ = false);
	int SendToService(uint16_t service_id_, const void* data_, size_t data_len_, bool close_after_send_ = false);
    uint32_t SetWallTimer(delay_call_t call_back_, uint32_t delay_time_, uint32_t interval_ = 0, uint16_t repeat_times_ = 0);
    uint32_t SetMonoTimer(delay_call_t call_back_, uint32_t delay_time_, uint32_t interval_ = 0, uint16_t repeat_times_ = 0);
	mstime_t GetNow(bool mono_ = false, bool exact_ = false);
public:
	virtual CProtocolParser* GetClientParser(uint16_t listener_id_) = 0;
	virtual CProtocolParser* GetServiceParser(uint16_t service_id_) = 0;
	virtual void	OnRecvClient(const void* data_, size_t data_len_, uint32_t session_id_, uint16_t listener_id_) = 0;
	virtual void	OnRecvService(const void* data_, size_t data_len_, uint16_t service_id_) = 0;
protected:
	virtual int OnInit() { return 0; }
	virtual void OnFini() {}
	virtual	void OnUpData() {}
};

} ///namespace jingke

#endif  ///_SERVER_H_

