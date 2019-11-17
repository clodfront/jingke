// Project:	jingke
// File:    obj_pool.h
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

#ifndef _OBJ_POOL_H_
#define _OBJ_POOL_H_

#include <new>
#include "buffer_pool.h"
#include "omygod.h"

namespace jingke {

template<typename T>
class CObjPool
{

public:
	
	typedef T obj_type_t;
	typedef T* obj_type_ptr;
	enum{ e_elem_size = sizeof(T) };

public:
	
    CObjPool()
    {
    }
    
    ~CObjPool()
    {
        Destroy();
    }
	
private:
	//No copying allowed
	CObjPool(const CBufferPool&); 
	CObjPool& operator=(const CObjPool&);  
	
public:
    
    int Init(size_t elem_count_, bool auto_increase_=false)
    {
		return _buffer_pool.Init(e_elem_size, elem_count_, auto_increase_);
    }
	
    void Destroy()
    {
		_buffer_pool.Destroy();
    }


    T* New()
    {
		void* p = _buffer_pool.New();
		if(!p)
			return NULL;
		T* pt = new(p) T;
		if(!pt)
			return NULL;
		return pt;
    }

    void Free(T* p)
    {	
    	if(!p)
			return;
    	p->~T();
		set_arithmetic_type_zero<T>(p);
    	_buffer_pool.Free(p);
    }
	
    size_t Capacity() const
    {
        return _buffer_pool.Capacity();
    }

	size_t ElemSize() const
	{
		return _buffer_pool.ElemSize();
	}

	size_t FreeCount() const
	{
		return _buffer_pool.FreeCount();
	}

	size_t BusyCount() const
	{
		return _buffer_pool.BusyCount();
	}

private:
	
    CBufferPool _buffer_pool; //imp
};

} ///namespace jingke

#endif  ///_OBJ_POOL_H_


