// Project:	jingke
// File:    timer.h
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

#ifndef ___timer_h__
#define ___timer_h__

#include "delay_queue.h"

namespace jingke {

template <bool usemonotime=false>
class CTimer
{
public:
    typedef delay_call_t timer_call_t;
    enum {bad_timer_id = e_invalid_timer_id};
public:
    CTimer() = default;
    ~CTimer() = default;
    CTimer(const CTimer&) = delete;
    CTimer& operator=(const CTimer&) = delete;
public:
    uint32_t insert(delay_call_t call_back_, uint32_t delay_time_, uint32_t interval_ = 0, uint16_t repeat_times_ = 0)
    {
        return _dq.insert(call_back_, delay_time_, interval_, repeat_times_);
    }
    void remove(uint32_t timer_id_)
    {
        return _dq.remove(timer_id_);
    }
    void try_call(mstime_t now_ = 0)
    {
        DelayNode node;
        if(!now_)
            now_ = usemonotime ? mono_mstimestamp_now() : wall_mstimestamp_now();
        while(_dq.peek(node, now_))
        {
            if(node._call_back)
                node._call_back(now_, node._repeat_count);
        }
    }
    void cleanup()
    {
        _dq.cleanup();
    }
private:
    CDelayQueue<usemonotime> _dq;
};

typedef CTimer<false>   CTimerWall;
typedef CTimer<true>    CTimerMono;

} ///namespace jingke

#endif //___timer_h__