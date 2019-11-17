// Project:	jingke
// File:    buffer_pool.h
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

#ifndef _BUFFER_POOL_H_
#define _BUFFER_POOL_H_


#include <stdlib.h>
#include <assert.h>
#include "dlist.h"
#include "omygod.h"

namespace jingke {

class CBufferPool
{
public:
	CBufferPool()
	:_elem_size(0)
	,_elem_aligned_size(0)
	,_elem_count(0)
	,_elem_wrap_size(0)
	,_free_size(0)
	,_auto_increase(false)
	{
		INIT_LIST_HEAD(&_free_list);
		INIT_LIST_HEAD(&_destroy_list);
	}
	~CBufferPool()
	{
		Destroy();
	}
private:
	//No copying allowed
	CBufferPool(const CBufferPool&); 
	CBufferPool& operator=(const CBufferPool&);
public:
	int Init(size_t elem_size_, size_t elem_count_, bool auto_increase_=false)
	{
		if(!elem_size_ || !elem_count_)
			return -1;
		if(_elem_count != 0 /*already Init*/)
			return -2;
		
		_elem_aligned_size = ptr_aligned_size(elem_size_);
		
		_elem_wrap_size = sizeof(list_head) + _elem_aligned_size;
		void* ptr = malloc(_elem_wrap_size * elem_count_);
		if(!ptr)
			return -3;
		for(size_t i=0; i<elem_count_; ++i)
			list_add_tail((list_head*)((char*)ptr + i*_elem_wrap_size), &_free_list);

		list_item<void*> * destory_item_ptr = (list_item<void*> *)malloc(sizeof(list_item<void*>));
		if(!destory_item_ptr)
			return -4;
		destory_item_ptr->_data = ptr;
		list_add_tail(&destory_item_ptr->_list, &_destroy_list);
		
		_elem_size = elem_size_;
		_elem_count = elem_count_;
		_free_size += elem_count_;
		_auto_increase = auto_increase_;
		return 0;
	}
	void Destroy()
    {
		for(list_head* pos=_destroy_list.next; pos!=&_destroy_list;)
		{
			list_item<void*> * destory_item_ptr = list_entry(pos, list_item<void*>, _list);
			free(destory_item_ptr->_data);
			pos = pos->next;
			free(destory_item_ptr);
		}
		
		INIT_LIST_HEAD(&_free_list);
		INIT_LIST_HEAD(&_destroy_list);
		
		_elem_size = 0;
		_elem_aligned_size = 0;
		_elem_count = 0;
		_elem_wrap_size = 0;
		_free_size = 0;
		_auto_increase = 0;
    }

	void* New()
	{
		if(!_elem_size || !_elem_count || !_elem_wrap_size) // not Init
			return NULL;
		
		if(!list_empty(&_free_list))
		{
			assert(_free_list.next != NULL);
			char* p = (char*)_free_list.next + sizeof(list_head);
			list_del(_free_list.next);
			--_free_size;
			return p;
		}
		
		if(_auto_increase)
		{
			//just increase 1/2 capacity
			size_t inc_count = _elem_count >> 1;
			if(0 == inc_count)
				inc_count = _elem_count;
			void* ptr = malloc(_elem_wrap_size * inc_count);
			if(!ptr)
				return NULL;
			for(size_t i=0; i<inc_count; ++i)
				list_add_tail((list_head*)((char*)ptr + i*_elem_wrap_size), &_free_list);

			list_item<void*> * destory_item_ptr = (list_item<void*> *)malloc(sizeof(list_item<void*>));
			if(!destory_item_ptr)
				return NULL;
			destory_item_ptr->_data = ptr;
			list_add_tail(&destory_item_ptr->_list, &_destroy_list);
			
			_elem_count += inc_count; //did not think overflow
			_free_size += inc_count;
			return New();
		}
		
	    return NULL;
	}

	void Free(void* p)
	{
		if(p)
        {
        	list_head* entry = (list_head*)((char*)p - sizeof(list_head));
			list_add(entry, &_free_list);
			++_free_size;
        }
	}

    size_t Capacity() const
    {
        return _elem_count;
    }

	size_t ElemSize() const
	{
		return _elem_size;
	}

	size_t FreeCount() const
	{
		return _free_size;
	}

	size_t BusyCount() const
	{
		return _elem_count - _free_size;
	}
	
private:
	size_t _elem_size;
	size_t _elem_aligned_size;
	size_t _elem_count;
	size_t _elem_wrap_size;
	size_t _free_size;
	list_head _free_list;
	list_head _destroy_list;
	bool _auto_increase;
};

} ///namespace jingke

#endif  ///_BUFFER_POOL_H_


