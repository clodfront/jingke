// Project:	jingke
// File:    socket_point.h
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

#ifndef _SOCKET_POINT_H_
#define _SOCKET_POINT_H_

#include <string>

namespace jingke {

class CSocketPoint
{
public:
    CSocketPoint();
    CSocketPoint(const std::string& addr_, uint16_t port_);
    void SetAddr(const std::string& addr_);
	void SetPort(uint16_t port_);
	const std::string& GetAddr();
	uint16_t GetPort();
private:
    std::string _addr;
    uint16_t _port;
};

} ///namespace jingke

#endif  ///_LOCAL_PORT_H_

