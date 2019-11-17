// Project:	jingke
// File:    epoller.h
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

#ifndef _epoller_h_
#define _epoller_h_

#include <sys/epoll.h>
#include <functional>
#include "curl/curl.h"
#include "timer.h"

namespace jingke {

class CSocketEventer;

class CEpoller
{
public:
	typedef std::function<int (CSocketEventer*, const void*, size_t, uint64_t)> recv_package_call_t;
	typedef std::function<void (CSocketEventer*)> add_socket_eventer_call_t;
	typedef std::function<void (CSocketEventer*)> del_socket_eventer_call_t;
public:
	CEpoller();
	~CEpoller();
public:
	int Init();
	void Close();
	int AddSocketEventer(int skt_fd_, unsigned int event_flag_, CSocketEventer* evrnter_);
	int ModSocketEventer(int skt_fd_, unsigned int event_flag_, CSocketEventer* evrnter_);
	int DelSocketEventer(int skt_fd_);
	int Wait(const int wait_timeout_milliseconds_);
public:
	void SetRecvPackageCallBack(const recv_package_call_t& call_back_){ _recv_package_callback = call_back_; }
	const recv_package_call_t& GetRecvPackageCallBack() {return _recv_package_callback;}
	void SetAddSktEventerCallBack(const add_socket_eventer_call_t& call_back_){ _add_socket_eventer_callbak = call_back_; }
	const add_socket_eventer_call_t& GetAddSktEventerCallBack() {return _add_socket_eventer_callbak;}
	void SetDelSktEventerCallBack(const del_socket_eventer_call_t& call_back_){ _del_socket_eventer_callbak = call_back_; }
	const del_socket_eventer_call_t& GetDelSktEventerCallBack() {return _del_socket_eventer_callbak;}
private:
	int _epfd;
	struct epoll_event * _evtlist;
	size_t _evtlist_size;
	size_t _evtlist_cnt;
    CSocketEventer**  _sck_eventer_map;
	recv_package_call_t _recv_package_callback;
	add_socket_eventer_call_t _add_socket_eventer_callbak;
	del_socket_eventer_call_t _del_socket_eventer_callbak;

//async http action support(use with libcur)
public:
	int InitMc();
	void DestoryMc();
	CURLM* 	GetMc() const {return _mc;}
	void McCancelTimer();
	void McSetTimer(uint32_t delay_time_);
	void McOnTimer(mstime_t now_ms_, uint16_t repeat_num_);
	void McOnSocketEvent(int skt_, int event_);
	void McCeckInfo();
	static int McSocketCallBack(CURL* ec_, curl_socket_t skt_, int what_, void* cbp_, void* sockp_);
	static int McTimeoutCallBack(CURLM *mc_, long timeout_ms_, void* cbp_);
private:
	CURLM* 		_mc;
	int 		_mc_still_running;
	CTimerMono  _mc_timeouter;
	uint32_t 	_mc_timer_id;
};

} ///namespace jingke

#endif//_epoller_h_