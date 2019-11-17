// Project:	jingke
// File:    clock_time.h
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

#ifndef _CLOCK_TIME_H_
#define _CLOCK_TIME_H_

#include <stdint.h>
#include <time.h>

namespace jingke {

typedef int64_t mstime_t;

extern timespec wall_time_now();
extern timespec mono_time_now();

extern time_t wall_timestamp_now();
extern time_t mono_timestamp_now();

extern mstime_t wall_mstimestamp_now();
extern mstime_t mono_mstimestamp_now();


//to reduce the system call, use cached_clock_time, but this is not accurate

class cached_clock_time
{
public:
	cached_clock_time();
	~cached_clock_time();
public:
	void update();
	timespec wall_time_now();
	timespec mono_time_now();
	time_t wall_timestamp_now();
	time_t mono_timestamp_now();
	mstime_t wall_mstimestamp_now();
	mstime_t mono_mstimestamp_now();
private:
	timespec ts_wall_;
	timespec ts_mono_;
};

} ///namespace jingke

#endif  ///_CLOCK_TIME_H_

