// Project:	jingke
// File:    logicserver.h
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
#ifndef __logic_server_h__
#define __logic_server_h__

#include "server.h"

using namespace jingke;

class CLogicServerLike : public IServer
{
public:
	virtual CProtocolParser* GetClientParser(uint16_t listener_id_) override;
	virtual CProtocolParser* GetServiceParser(uint16_t service_id_) override;
	virtual void OnRecvClient(const void* data_, size_t data_len_, uint32_t session_id_, uint16_t listener_id_) override;
	virtual void OnRecvService(const void* data_, size_t data_len_, uint16_t service_id_) override;
protected:
	virtual int OnInit() override;
	virtual void OnFini() override;
};


#endif ///__logic_server_h__
