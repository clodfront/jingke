// Project:	jingke
// File:    clock_time.cpp
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
#include "clock_time.h"

namespace jingke {

timespec wall_time_now()
{
	timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	return ts;
}

timespec mono_time_now()
{
	timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return ts;
}

time_t wall_timestamp_now()
{
	return wall_time_now().tv_sec;
}

time_t mono_timestamp_now()
{
	return mono_time_now().tv_sec;
}

extern mstime_t wall_mstimestamp_now()
{
	timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	return (mstime_t)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

extern mstime_t mono_mstimestamp_now()
{
	timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (mstime_t)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

} ///namespace jingke

///////////////////////

using namespace jingke;

cached_clock_time::cached_clock_time()
{
	update();
}

cached_clock_time::~cached_clock_time()
{
}

void cached_clock_time::update()
{
	ts_wall_ = jingke::wall_time_now();
	ts_mono_ = jingke::mono_time_now();
}

timespec cached_clock_time::wall_time_now()
{
	return ts_wall_;
}

timespec cached_clock_time::mono_time_now()
{
	return ts_mono_;
}

time_t cached_clock_time::wall_timestamp_now()
{
	return ts_wall_.tv_sec;
}

time_t cached_clock_time::mono_timestamp_now()
{
	return ts_mono_.tv_sec;
}

mstime_t cached_clock_time::wall_mstimestamp_now()
{
	return (mstime_t)ts_wall_.tv_sec * 1000 + ts_wall_.tv_nsec / 1000000;
}

mstime_t cached_clock_time::mono_mstimestamp_now()
{
	return (mstime_t)ts_mono_.tv_sec * 1000 + ts_mono_.tv_nsec / 1000000;
}
