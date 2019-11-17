// Project:	jingke
// File:    socket_eventer.cpp
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
#include "socket_eventer.h"
#include "epoller.h"

using namespace jingke;

void CSocketEventer::Close() 
{
    if(_parser)
        _parser = NULL;
    if(_epoller)
    {
        _epoller->DelSocketEventer(Socket().GetFd());
        _epoller = NULL;
    }
    Socket().Close(); 
}

void CSocketEventer::SetEpoller(CEpoller* epoller_)
{
    _epoller = epoller_;
}

CEpoller* CSocketEventer::GetEpoller()
{
    return _epoller;
}


