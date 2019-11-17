// Project:	jingke
// File:    sys_op.cpp
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
#include "sys_op.h"
#include <stdlib.h>
#include <cstddef>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace jingke;

rlim_t CSysOp::GetFdLimit()
{
	struct rlimit rlim;
	getrlimit(RLIMIT_NOFILE, &rlim); //it never failed.
	return rlim.rlim_cur;
}

int CSysOp::SetFdLimit(rlim_t limit, rlim_t& curr_limit, bool surrender/* = true*/)
{
	if(0 == limit) return -1;
	struct rlimit rlim;
	getrlimit(RLIMIT_NOFILE, &rlim);
	
	if (limit > rlim.rlim_max)
	{
		if(surrender)
			limit = rlim.rlim_max;
		else
			return -2;
	}
	
	rlim.rlim_cur = limit;
	if (setrlimit(RLIMIT_NOFILE, &rlim) < 0)
	{
		return -3;
	}
	curr_limit = limit;
	return 0;
}

int CSysOp::IgnoreSignal(int sig)
{
	/*
	struct sigaction sigact;
	sigact.sa_handler = SIG_IGN;
	sigemptyset(&sigact.sa_mask);
	sigact.sa_flags = 0;
	if (sigaction(sig, &sigact, NULL) < 0)
	{
		return -1;
	}
	*/
	if(SIG_ERR == signal(sig, SIG_IGN))
		return -1;
	return 0;
}

int CSysOp::OverrideSignal(int sig, sighandler_t handler)
{
	if(SIG_ERR == signal(sig, handler))
		return -1;
	return 0;
}

int CSysOp::StartDaemon()
{
	pid_t pid;

	// Start forking
	if ((pid = fork()) < 0)
	{
		return -1;
	}
	if (pid > 0)
	  exit(0);// parent

	// First child process
	setsid(); // become session leader

	if ((pid = fork()) < 0)
	{
		return -2;
	}
	if (pid > 0)				
	  exit(0);// first child

	// Create file default attr[-rw-r--r--]
	umask(022);

	return 0;
}



