// Project:	jingke
// File:    buffer_pool_group.h
// Created: 11/2019
// Author:  Goof
//
// Copyright 2019-2020 Goof one_uip@foxmail.com
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
#ifndef _BUFFER_POOL_GROUP_H_
#define _BUFFER_POOL_GROUP_H_

#include "buffer_pool.h"
#include <unordered_map>
#include <string>
#include "string_op.h"

namespace jingke {

class CBufferPoolGroup
{
public:
	CBufferPoolGroup() = default;
	~CBufferPoolGroup()
	{
		for(std::unordered_map<size_t, CBufferPool*>::iterator it=_buffer_pools.begin(); it!=_buffer_pools.end(); ++it)
		{
			if(it->second)
			{
				delete it->second;
			}
		}
		_buffer_pools.clear();
	}
public:
	bool Join(size_t buff_size_, size_t buff_count_, bool auto_increase_=false)
	{
		if(_buffer_pools.find(buff_size_) != _buffer_pools.end())
			return false;
		CBufferPool* pool = new(std::nothrow) CBufferPool; 
		if(!pool)
			return false;
		if(0 != pool->Init(buff_size_, buff_count_, auto_increase_))
		{
			delete pool;
			return false;
		}
		auto ret = _buffer_pools.insert(std::make_pair(buff_size_, pool));
		if(!ret.second)
		{
			delete pool;
			return false;
		}
		return true;
	}
	char* New(size_t buff_size_)
	{
		std::unordered_map<size_t, CBufferPool*>::iterator it = _buffer_pools.find(buff_size_);
		if(it != _buffer_pools.end() && it->second)
			return (char*)it->second->New();
		return NULL;
	}
	void Free(size_t buff_size_, char* buff_ptr_)
	{
		std::unordered_map<size_t, CBufferPool*>::iterator it = _buffer_pools.find(buff_size_);
		if(it != _buffer_pools.end() && it->second)
			it->second->Free(buff_ptr_);
	}

	bool Has(size_t buff_size_)
	{
		return _buffer_pools.find(buff_size_) != _buffer_pools.end();
	}

	std::string DebugInfoString()
    {
       std::string info;
	   for(auto& i : _buffer_pools)
	   {
		   	std::string tmp;
			tmp += type_to_str(i.first);
			tmp += ":";
			if(i.second)
				tmp += type_to_str(i.second->FreeCount());
			tmp += "; ";
			info += tmp;
	   }
	   return info;
    }
private:
	std::unordered_map<size_t, CBufferPool*> _buffer_pools;
};

} ///namespace jingke

#endif  ///_BUFFER_POOL_GROUP_H_


