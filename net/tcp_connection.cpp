// Project:	jingke
// File:    tcp_connection.cpp
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

#include "tcp_connection.h"
#include "singleton.h"
#include "tcp_connection_pool.h"

using namespace jingke;

CTcpConnection* CTcpConnection::Create()
{
    return singleton<CTcpConnectionPool>::instance().New();
}

void CTcpConnection::Free(CTcpConnection* conn_obj_)
{
    singleton<CTcpConnectionPool>::instance().Free(conn_obj_);
}

void CTcpConnection::Close()
{
    ISocketConnectionBase::Close();
    _listener_id = 0;
    _session_id = 0;
    CTcpConnection::Free(this);
}

