// Project:	jingke
// File:    socket_rsbuffer_pool.h
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

#ifndef _socket_rsbuffer_pool_
#define _socket_rsbuffer_pool_

#include "buffer_pool_group.h"
#include <string>

namespace jingke {

class CSocketRSBufferPool
{
    enum {e_auto_join_count = 16};
public:
    CSocketRSBufferPool() = default;
    ~CSocketRSBufferPool() = default;
public:
    bool Join(size_t buff_size_, size_t buff_count_, bool auto_increase_=true)
    {
        return _pool_group.Join(buff_size_, buff_count_, auto_increase_);
    }
	char* New(size_t buff_size_)
	{
        if(!_pool_group.Has(buff_size_))
        {
            if(!_pool_group.Join(buff_size_, e_auto_join_count, true))
                return NULL;
        }
		return _pool_group.New(buff_size_);
	}
	void Free(size_t buff_size_, char* buff_ptr_)
    {
        return _pool_group.Free(buff_size_, buff_ptr_);
    }
    std::string DebugInfoString()
    {
       return _pool_group.DebugInfoString();
    }
private:
    CBufferPoolGroup _pool_group;
};

} ///namespace jingke

#endif //_socket_rsbuffer_pool_