// Project:	jingke
// File:    server_cfg.h
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

#ifndef _SERVER_CFG_H_
#define _SERVER_CFG_H_

#include <stdint.h>
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include "net_config.h"

namespace jingke {

class CServerCfg
{
public:
	std::string		_std_cfg_file;
	std::string		_log_cfg_file;
	std::unordered_map<uint16_t, STcpListenerCfg> _tcp_listeners;
	std::unordered_map<uint16_t, STcpPeerServiceCfg> _tcp_peer_svrs;
};

} ///namespace jingke

#endif  ///_SERVER_CFG_H_

