// Project:	jingke
// File:    socket_evneter_type.cpp
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
#include "socket_evneter_type.h"

namespace jingke{

const char* GetSocketEventTypeDesc(int type_)
{
	static const char* e_se_type[] = 
	{
		"tcp_listener",
		"tcp_connection",
		"tcp_peer_connection",
		"udp_connection",
		"udp_peer_connection",
		"libcurl_peer_connection"
	};
	if(type_ <= e_se_type_begin || type_ >= e_se_type_end)
		return "";
	return e_se_type[type_ - 1];
}

} ///namespace jingke


