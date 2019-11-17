// Project:	jingke
// File:    socket_point.cpp
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
#include "socket_point.h"

using namespace jingke;

CSocketPoint::CSocketPoint():_port(0)
{
}

CSocketPoint::CSocketPoint(const std::string& addr_, uint16_t port_)
{
    _addr = addr_;
    _port = port_;
}

void CSocketPoint::SetAddr(const std::string& addr_)
{
    _addr = addr_;
}

void CSocketPoint::SetPort(uint16_t port_)
{
    _port = port_;
}

const std::string& CSocketPoint::GetAddr()
{
    return _addr;
}

uint16_t CSocketPoint::GetPort()
{
    return _port;
}

