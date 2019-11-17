// Project:	jingke
// File:    tcp_connection_pool.h
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

#ifndef _tcp_connection_pool_h_
#define _tcp_connection_pool_h_

#include "obj_pool.h"
#include "tcp_connection.h"

namespace jingke {

class CTcpConnectionPool
{
public:
    CTcpConnectionPool() = default;
    ~CTcpConnectionPool() = default;
public:
    int Init(size_t elem_count_, bool auto_increase_=false)
    {
        return _pool.Init(elem_count_, auto_increase_);
    }
    CTcpConnection* New()
    {
        return _pool.New();
    }
    void Free(CTcpConnection* ptr_)
    {
        return _pool.Free(ptr_);
    }
private:
    CObjPool<CTcpConnection> _pool;
};

} ///namespace jingke

#endif//_tcp_connection_pool_h_
