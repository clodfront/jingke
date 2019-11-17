// Project:	jingke
// File:    delay_queue.h
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
#ifndef delay_queue_h__
#define delay_queue_h__

#include <iostream>
#include <vector>
#include <unordered_map>
#include <functional>
#include "clock_time.h"
#include "obj_pool.h"
#include "string_op.h"

namespace jingke {

typedef std::function<void (mstime_t, uint16_t)> delay_call_t;

enum{e_invalid_timer_id = 0, e_node_pool_init_size = 256};

//超时器节点数据
class DelayNode
{
public:
    DelayNode() : _id(e_invalid_timer_id), _repeat_times(0), _repeat_count(0), _abs_time(0), _interval(0), _delay_time(0) {}
    ~DelayNode(){ _id = e_invalid_timer_id; _repeat_times = 0; _repeat_count = 0; _abs_time = 0; _interval = 0; _delay_time = 0; }
public:
    // 延时器ID,0为无效ID
    uint32_t _id;
    // 往复次数，0表示无限次，_interval>0时有意义
    uint16_t _repeat_times; 
    // 往复次数计数器
    uint16_t _repeat_count; 
    // 超时绝对时间戳，单位毫秒
    mstime_t _abs_time; 
    // 超时间隔，单位毫秒。若为<=0表示不是往复任务；否则为往复定时器，表示间隔时间
    uint32_t _interval;
    // 超时相对时间，相对于当前时间来讲
    uint32_t _delay_time;
    // 超时回调
    delay_call_t _call_back;
};

//CDelayQueue<false>    表示依赖系统时间
//CDelayQueue<true>     表示依赖mono时间
template<bool usemonotime=false>
class CDelayQueue
{
public:
	CDelayQueue():_id_cursor(0)
    {
        _node_pool.Init(e_node_pool_init_size,true);
    }
	~CDelayQueue(){}
public:
    uint32_t insert(delay_call_t call_back_, uint32_t delay_time_, uint32_t interval_ = 0, uint16_t repeat_times_ = 0)
    {
        DelayNode* node = _node_pool.New();
        if(!node)
            return e_invalid_timer_id;
        mstime_t msnow = get_mstime_stamp();
        node->_id = gen_timer_id();
        node->_repeat_times = repeat_times_;
        node->_abs_time = delay_time_ + msnow;
        node->_interval = interval_;
        node->_delay_time = delay_time_;
        node->_call_back = call_back_;
        push(node);

        return node->_id;
    }

    bool peek(DelayNode& node_, mstime_t now_ = 0)
    {
        if(_map_id2index.empty() || _heap.empty())
            return false;
        if(!now_)
            now_ = get_mstime_stamp();
        if(_heap.front()->_abs_time > now_)
            return false;
        DelayNode* node = pop();
        if(!node) return false;
        ++node->_repeat_count; //may overflow
        node_ = *node;

        if(node->_interval > 0)
        {
            if(0 == node->_repeat_times || node->_repeat_count < node->_repeat_times)
            {
                node->_abs_time += node->_interval;
                push(node);
            }
        }
        else
        {
            _node_pool.Free(node);
        }
        return true;
    }

    void remove(uint32_t timer_id_)
    {
        if (_map_id2index.end() == _map_id2index.find(timer_id_))
            return;
        size_t index = _map_id2index[timer_id_];
        DelayNode* node = pop(index);
        _node_pool.Free(node);
    }

    void cleanup()
    {
        _heap.clear();
        _map_id2index.clear();
        _node_pool.Destroy();
        _id_cursor = 0;
    }

public: //for test
    size_t  heap_capacity() const {return _heap.size();}
    size_t  useful_count() const {return _map_id2index.size();}
    bool peek_force(DelayNode& node_)
    {
        if(_map_id2index.empty() || _heap.empty())
            return false;
        DelayNode* node = pop();
        if(!node) return false;
        ++node->_repeat_count;
        node_ = *node;
        if(node->_interval > 0)
        {
            if(0 == node->_repeat_times || node->_repeat_count < node->_repeat_times)
            {
                node->_abs_time += node->_interval;
                push(node);
            }
        }
        else
        {
            _node_pool.Free(node);
        }
        return true;
    }
    std::string print_heap() const
    {
        std::string ret;
        for (size_t i = 0; i < _map_id2index.size() && i < _heap.size(); ++i, ret+="|")
        {
            ret += type_to_str<uint32_t>(_heap[i]->_delay_time);
        }
        return ret;
    }
private:
    uint32_t gen_timer_id() {return  ++_id_cursor ? _id_cursor : ++_id_cursor;}
    mstime_t get_mstime_stamp() const { return usemonotime ? mono_mstimestamp_now() : wall_mstimestamp_now();}
    void     makesure_heap_capacity()
    {
        if(useful_count() == heap_capacity()) //full
        {
            size_t old_cap = heap_capacity();
            size_t inc_size = old_cap + 1;
            size_t new_cap = old_cap + inc_size;
            for (size_t i = old_cap; i < new_cap; ++i)
                _heap.push_back(NULL);
        }
    }

    void go_up(size_t index_)
    {
        if (index_ >= _map_id2index.size() || index_ >= _heap.size())
            return;
        if (!index_)
            return;
        size_t father_index = (index_ - 1) >> 1;

        while (index_ != 0 &&  _heap[index_]->_abs_time < _heap[father_index]->_abs_time)
        {
            swap_by_index(index_, father_index);
            index_ = father_index;
            father_index = (index_ - 1) >> 1;
        }
    }

    void go_down(size_t index_)
    {
        if (index_ >= _map_id2index.size() || index_ >= _heap.size())
            return;
        size_t l_sub_index = (index_ << 1) + 1;
        size_t r_sub_index = (index_ + 1) << 1;
        size_t swap_index = 0;

        while(l_sub_index < _map_id2index.size())
        {
            swap_index = l_sub_index;
            if(r_sub_index < _map_id2index.size() && _heap[r_sub_index]->_abs_time < _heap[l_sub_index]->_abs_time)
                swap_index = r_sub_index;
            if(_heap[index_]->_abs_time < _heap[swap_index]->_abs_time)
                break;
            swap_by_index(index_, swap_index);
            index_ = swap_index;
            l_sub_index = (index_ << 1) + 1;
            r_sub_index = (index_ + 1) << 1;
        }
    }

    void swap_by_index(size_t l_index_, size_t r_index_) 
    {
        if (l_index_ >= _heap.size() || r_index_ >= _heap.size() )
            return;
        
        if (l_index_ == r_index_)
            return;

        uint32_t l_id = _heap[l_index_]->_id;
        uint32_t r_id = _heap[r_index_]->_id;

        std::swap(_heap[l_index_], _heap[r_index_]);
        
        _map_id2index[l_id] = r_index_;
        _map_id2index[r_id] = l_index_;
    }

    void push(DelayNode* node_)
    {
        makesure_heap_capacity();
        size_t index = useful_count();
        _heap[index] = node_;
        _map_id2index[node_->_id] = index;
        go_up(index);
    }

    DelayNode* pop(size_t index_ = 0)
    {
        if(_map_id2index.empty() || _heap.empty())
            return NULL;
        size_t useful_size = useful_count();
        if(index_ >= useful_size)
            return NULL;
        bool fix_up = false;
        swap_by_index(index_, useful_size-1);
        DelayNode* node = _heap[useful_size-1];
        _heap[useful_size-1] = NULL; 
        _map_id2index.erase(node->_id);

        if(index_ == useful_size-1)
            return node;

        if(index_ != 0)
        {
            size_t father_index = (index_ - 1) >> 1;
            if(_heap[index_]->_abs_time < _heap[father_index]->_abs_time)
                fix_up = true;
        }
        
        fix_up ? go_up(index_) : go_down(index_);
        return node;
    }

private:
    std::vector<DelayNode*> _heap;
    std::unordered_map<uint32_t, size_t> _map_id2index;
    CObjPool<DelayNode>     _node_pool;
    uint32_t _id_cursor;
};
/*
template<>
mstime_t CDelayQueue<true>::get_mstime_stamp() const {return mono_mstimestamp_now();}
*/
typedef CDelayQueue<false> CDelayQueueWall;
typedef CDelayQueue<true> CDelayQueueMono;

} ///namespace jingke

#endif // delay_queue_h__