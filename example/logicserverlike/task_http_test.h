// Project:	jingke
// File:    task_http_test.h
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
#ifndef __http_test___
#define __http_test___

#include "async_worker.h"
#include "libcurl_http_peer_connection.h"

using namespace jingke;

class CLogicServerLike;

class CTaskHttpTest : public IAsyncTask
{
public:
	CTaskHttpTest(){}
	~CTaskHttpTest(){}
	int Init();
public:
	virtual bool on_start() override;			//开始任务 返回true表示任务终止，否则表示被挂起
	virtual bool on_awaken(const void* data_, size_t len_) override;	//唤醒任务 返回值意义同on_start， 默认返回true
	virtual uint32_t time_out_time() override {return 0;}	//超时时间, 0表示不超时，单位毫秒
	const char* name() const override { return "CTaskHttpTest";}
	CLibcurlHttpPeerConnection& WebConnection(){ return _web_connection; }
private:
	CLibcurlHttpPeerConnection _web_connection;
};

#endif //__http_test___


