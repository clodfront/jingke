// Project:	jingke
// File:    log_writer.cpp
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
#include "log_writer.h"

using namespace jingke;

CLog_writer::CLog_writer()
	:usable_(false)
{

}

CLog_writer::~CLog_writer()
{
	shutdown();
}

bool CLog_writer::init(const tstring& config_path)
{
	log4cplus::initialize();
	logger_ = Logger::getRoot();
	PropertyConfigurator::doConfigure(config_path);
	SharedAppenderPtrList p = logger_.getAllAppenders();
	if (p.empty())
	{
		usable_ = false;
	}
	else
	{
		usable_ = true;
	}
	return usable_;
}


void CLog_writer::shutdown()
{
	if(usable_)
	{
		Logger::shutdown();
		usable_ = false;
	}
}

bool CLog_writer::usable()
{
	return usable_;
}

bool CLog_writer::level_enable(int log_level)
{
	if (usable_)
	{
		return logger_.isEnabledFor(log_level);
	}
	return false;
}

Logger& CLog_writer::logger()
{
	return logger_;
}



