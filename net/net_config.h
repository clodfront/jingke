// Project:	jingke
// File:    net_config.h
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

#ifndef _net_config_h_
#define _net_config_h_

#include "socket_point.h"
#include <string.h>

namespace jingke {

struct STcpListenerCfg
{
    CSocketPoint _local_point;
    uint16_t    _listener_id;
    int32_t 	_backlog;
    uint32_t 	_client_max_cnt;
    int32_t		_client_sock_recv_buff_size;
    int32_t		_client_sock_send_buff_size;
    int32_t 	_client_keepalive_onoff;
    int32_t 	_client_keepalive_idle;
    int32_t 	_client_keepalive_interval;
    int32_t 	_client_keepalive_times;
    STcpListenerCfg(){ memset((char*)this + sizeof(CSocketPoint), 0, sizeof(STcpListenerCfg)-sizeof(CSocketPoint)); }
};

struct STcpPeerServiceCfg
{
	CSocketPoint 	_service_end_point;
	uint16_t    	_service_id;
	int32_t			_service_sock_recv_buff_size;
	int32_t			_service_sock_send_buff_size;
	STcpPeerServiceCfg(){ memset((char*)this + sizeof(CSocketPoint), 0, sizeof(STcpPeerServiceCfg)-sizeof(CSocketPoint)); }
};

} ///namespace jingke

#endif  ///_net_config_h_

