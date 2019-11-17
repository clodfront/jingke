// Project:	jingke
// File:    log_writer.h
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

#ifndef _LOG_WRITER_H_
#define _LOG_WRITER_H_


#include <log4cplus/logger.h>
#include <log4cplus/configurator.h>
#include <log4cplus/loggingmacros.h>
#include "singleton.h"

using namespace log4cplus;

namespace jingke {

class CLog_writer   
{
public:
	CLog_writer();
	~CLog_writer();
public:
	bool init(const tstring& config_path);
	void shutdown();
	bool usable();
	bool level_enable(int log_level);
	Logger& logger(); 
private:
	Logger logger_;
	bool   usable_;
};


//////////////define users marcs/////////////

#ifndef LOG_DEBUG
#define LOG_DEBUG(...) \
do\
{\
	if(singleton<CLog_writer>::instance().usable() && singleton<CLog_writer>::instance().level_enable(DEBUG_LOG_LEVEL))\
	{\
		LOG4CPLUS_DEBUG_FMT(singleton<CLog_writer>::instance().logger(), __VA_ARGS__);\
	}\
} while(0)
#endif


#ifndef LOG_INFO
#define LOG_INFO(...) \
do\
{\
	if(singleton<CLog_writer>::instance().usable() && singleton<CLog_writer>::instance().level_enable(INFO_LOG_LEVEL))\
{\
	LOG4CPLUS_INFO_FMT(singleton<CLog_writer>::instance().logger(), __VA_ARGS__);\
}\
} while(0)
#endif


#ifndef LOG_WARN
#define LOG_WARN(...) \
do\
{\
	if(singleton<CLog_writer>::instance().usable() && singleton<CLog_writer>::instance().level_enable(WARN_LOG_LEVEL))\
{\
	LOG4CPLUS_WARN_FMT(singleton<CLog_writer>::instance().logger(), __VA_ARGS__);\
}\
} while(0)
#endif


#ifndef LOG_ERROR
#define LOG_ERROR(...) \
do\
{\
	if(singleton<CLog_writer>::instance().usable() && singleton<CLog_writer>::instance().level_enable(ERROR_LOG_LEVEL))\
{\
	LOG4CPLUS_ERROR_FMT(singleton<CLog_writer>::instance().logger(), __VA_ARGS__);\
}\
} while(0)
#endif

} ///namespace jingke

#endif  ///_LOG_WRITER_H_

