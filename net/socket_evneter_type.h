// Project:	jingke
// File:    socket_evneter_type.h
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

#ifndef _socket_eventer_type_h_
#define _socket_eventer_type_h_

namespace jingke {

enum ESKTEVTERTYPE
{
	e_se_type_begin = 0,
	e_se_type_tcp_listener,
	e_se_type_tcp_connection,
	e_se_type_tcp_peer_connection,
	e_se_type_udp_connection,
	e_se_type_udp_peer_connection,
	e_se_type_libcurl_peer_connection,
	e_se_type_end,
};

const char* GetSocketEventTypeDesc(int type_);

} ///namespace jingke

#endif  ///_socket_eventer_type_h_

