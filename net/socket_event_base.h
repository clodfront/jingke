// Project:	jingke
// File:    socket_event_base.h
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

#ifndef _socket_event_base_h_
#define _socket_event_base_h_

namespace jingke {

class ISocketEvent
{
public:
    ISocketEvent() = default;
    virtual ~ISocketEvent() = default;
public:
    virtual int OnRecvble() = 0;
	virtual int OnSendble() = 0;
    virtual void OnError() = 0;
};

} ///namespace jingke

#endif//_socket_event_base_h_