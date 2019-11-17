// Project:	jingke
// File:    epoller.cpp
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
#include "epoller.h"
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <utility>
#include <string>
#include "sys_op.h"
#include "socket_base.h"
#include "omygod.h"
#include "socket_eventer.h"
#include "log_writer.h"
#include "socket_evneter_type.h"

using namespace jingke;

#ifndef MAX_EPOLL_HINT_SIZE
#define MAX_EPOLL_HINT_SIZE 40960
#endif


std::string DebugEventBitSet(uint32_t event_)
{
	std::string ctx;
	if(event_ & EPOLLIN)	ctx += "EPOLLIN\t";
	if(event_ & EPOLLOUT)	ctx += "EPOLLOUT\t";
	if(event_ & EPOLLERR)	ctx += "EPOLLERR\t";
	if(event_ & EPOLLHUP)	ctx += "EPOLLHUP\t";
	if(event_ & EPOLLRDHUP)	ctx += "EPOLLRDHUP\t";
	if(event_ & EPOLLET)	ctx += "EPOLLET\t";
	return ctx;
}

struct SEpollDataExplain 
{
    SEpollDataExplain(epoll_data_t* ed_):_ed(ed_){}
    int& fd() {return _ed->fd;}
    int& type() {return *(((int*)&_ed->fd) + 1);}	//for debug
    epoll_data_t* _ed;
};


CEpoller::CEpoller()
	:_epfd(INVALID_SOCKET)
	,_evtlist(NULL)
	,_evtlist_size(0)
	,_evtlist_cnt(0)
    ,_sck_eventer_map(NULL)
	,_mc(NULL)
	,_mc_still_running(0)
	,_mc_timer_id(0)
{
}

CEpoller::~CEpoller()
{
	this->Close();
}

int CEpoller::Init()
{
	int ret = 0;

	do
	{
		// Check if epoll functions are just stubs
		struct epoll_event ev_check;
		ev_check.events = EPOLLIN;
    	ev_check.data.ptr = NULL;
    	epoll_ctl(-1, EPOLL_CTL_ADD, -1, &ev_check);
		if(ENOSYS == errno)
		{
			ret = -1;
			break;
		}

		// Ignore SIGPIPE
		if(0 != (ret = CSysOp::IgnoreSignal(SIGPIPE)))
		{
			ret = -2;
			break;
		}

		rlim_t fdlim = CSysOp::GetFdLimit();
		size_t fd_hint = (fdlim > 0 && fdlim < MAX_EPOLL_HINT_SIZE) ? fdlim : MAX_EPOLL_HINT_SIZE;

		// Since Linux 2.6.8, the size argument of epoll_create is unused.  (The kernel dynamically sizes the required data structures without needing this initial hint.)
		if ((_epfd = epoll_create((int)fd_hint)) < 0) 
		{	
			ret = -3;
			break;
	    }

		// Close on exec, not on fork
		fcntl(_epfd, F_SETFD, FD_CLOEXEC);

	    // Allocate event lists
	    _evtlist_size = fd_hint; //initialize evnet list size by hint
	    _evtlist = (struct epoll_event *)malloc(_evtlist_size * sizeof(struct epoll_event));
	    if (!_evtlist) 
		{
			ret = -4;
			break;
	    }
        memset(_evtlist, 0, _evtlist_size * sizeof(struct epoll_event));
        //initialize evneter map size by hint
        _sck_eventer_map = (CSocketEventer**)malloc(_evtlist_size * sizeof(CSocketEventer*));
        if(!_sck_eventer_map)
        {
            ret = -5;
            break;
        }
        memset(_sck_eventer_map, 0, _evtlist_size * sizeof(CSocketEventer*));

		//initialize async http system
		if(0 != InitMc())
		{
			ret = -6;
			break;
		}
	}while(0);

	if(0 != ret)
		this->Close();

	return ret;
}

void CEpoller::Close()
{
	if(_epfd != INVALID_SOCKET)
	{
        ::close(_epfd);
		_epfd = INVALID_SOCKET;
	}
	safe_free(_evtlist);
	safe_free(_sck_eventer_map);
	_evtlist_size = 0;
	_evtlist_cnt = 0;
	
	DestoryMc();
}

int CEpoller::AddSocketEventer(int skt_fd_, unsigned int event_flag_, CSocketEventer* evrnter_)
{
	if(_epfd < 0 || skt_fd_ < 0 || !evrnter_)
		return -1;

	if(event_flag_ & ~(EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLET))
		return -2;

    if((size_t)skt_fd_ >= _evtlist_size ||  _evtlist_cnt >= _evtlist_size)
        return -3;

    if(skt_fd_ != evrnter_->Socket().GetFd()) //ckeck again, is it need to remove skt_fd_ and just use evrnter_ ?
        return -4;
    
	struct epoll_event ev;
	ev.events = event_flag_;
    SEpollDataExplain ev_data(&ev.data);
    ev_data.fd() = skt_fd_;
    ev_data.type() = evrnter_->Type();
	
	if(epoll_ctl(_epfd, EPOLL_CTL_ADD, skt_fd_, &ev) < 0/* && errno != EEXIST*/) //is it need to update ev.data to epoll_data_t when EEXIST ?
		return -4;
	++_evtlist_cnt;
    _sck_eventer_map[skt_fd_] = evrnter_;

	if(_add_socket_eventer_callbak)
		_add_socket_eventer_callbak(evrnter_);
	LOG_DEBUG("epolladd fd:%d type:%s, event:%s", skt_fd_, GetSocketEventTypeDesc(evrnter_->Type()), DebugEventBitSet(event_flag_).c_str());
	return 0;
}

int CEpoller::ModSocketEventer(int skt_fd_, unsigned int event_flag_, CSocketEventer* evrnter_)
{
	if(_epfd < 0 || skt_fd_ < 0 || !evrnter_)
		return -1;
	if(event_flag_ & ~(EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLET))
		return -2;
    if(skt_fd_ != evrnter_->Socket().GetFd()) //ckeck again, is it need to remove skt_fd_ and just use evrnter_ ?
        return -3;

	struct epoll_event ev;
	ev.events = event_flag_;
    SEpollDataExplain ev_data(&ev.data);
    ev_data.fd() = skt_fd_;
    ev_data.type() = evrnter_->Type();

	if(epoll_ctl(_epfd, EPOLL_CTL_MOD, skt_fd_, &ev) < 0)
		return -4;
	LOG_DEBUG("epollmod fd:%d type:%s, event:%s", skt_fd_, GetSocketEventTypeDesc(evrnter_->Type()), DebugEventBitSet(event_flag_).c_str());
	return 0;
}

int CEpoller::DelSocketEventer(int skt_fd_)
{
	if(_epfd < 0 || skt_fd_ < 0 )
		return -1;
	
	// In  kernel versions before 2.6.9, the EPOLL_CTL_DEL operation required a non-NULL pointer in event, even though this argument is ignored.  Since Linux 2.6.9, event can be specified as
    // NULL when using EPOLL_CTL_DEL.  Applications that need to be portable to kernels before 2.6.9 should specify a non-NULL pointer in event.
	static struct epoll_event ev;
	if(0 == epoll_ctl(_epfd, EPOLL_CTL_DEL, skt_fd_, &ev))
	{
		--_evtlist_cnt;
	}

	if((size_t)skt_fd_ < _evtlist_size)
	{
		if(_del_socket_eventer_callbak)
			_del_socket_eventer_callbak(_sck_eventer_map[skt_fd_]);
		_sck_eventer_map[skt_fd_] = NULL;
	}
	LOG_DEBUG("epolldel fd:%d", skt_fd_);
	return 0;
}

int CEpoller::Wait(const int wait_timeout_milliseconds_)
{
	_mc_timeouter.try_call();

	int n = 0;
	uint32_t events = 0;
	int fd = 0;
	int eventer_type = 0;
	CSocketEventer* eventer_ptr = NULL;

	while ((n = epoll_wait(_epfd, _evtlist, std::min(256,(int)_evtlist_size), wait_timeout_milliseconds_)) < 0 && errno == EINTR);
	
	if(n > 0)
	{
        for (int i=0; i<n; ++i) 
		{
			events = _evtlist[i].events;
			SEpollDataExplain ev_data(&_evtlist[i].data);
			fd = ev_data.fd();
			eventer_type = ev_data.type();
			(void)eventer_type;

			//LOG_DEBUG("fd:%d type:%s, event:%s", fd, GetSocketEventTypeDesc(eventer_type), DebugEventBitSet(events).c_str());
			if(e_se_type_libcurl_peer_connection == eventer_type) //http socket
			{
				McOnSocketEvent(fd, events);
				continue;
			}

			if(fd<0)
			{
				LOG_ERROR("skt_fd:%d is illegal.", fd);
				continue;
			}

			eventer_ptr = _sck_eventer_map[fd];
			if(!eventer_ptr)
			{
				LOG_ERROR("communicator ptr of socket skt_fd:%d is NULL.", fd);
				continue;
			}
	
			if(events & EPOLLERR)	//后知后觉的错误，当向对端执行send、write等操作时才能收到，可视为对端异常关闭
			{
				LOG_ERROR("socket skt_fd:%d Error by EPOLLERR.", fd);
				eventer_ptr->OnError();
            	continue;
			}

			if(events & EPOLLHUP)	//本端错误
			{
				LOG_ERROR("socket skt_fd:%d Error by EPOLLHUP.", fd);
				eventer_ptr->OnError();
            	continue;
			}

			if(EPOLLRDHUP & events)	//(since Linux 2.6.17)对端正常关闭（或关闭了写通道），类似老的EPOLLIN依赖read返回EOF(0)判定对端关闭，这个位码会和EPOLLIN同时到达
			{
				LOG_ERROR("socket skt_fd:%d Error by EPOLLRDHUP.", fd);
				eventer_ptr->OnError();
            	continue;
			}

			if(EPOLLIN & events) 	//连接到达，或者有可读数据
			{
				int ret = eventer_ptr->OnRecvble();
				if(ret < 0)
				{
					LOG_ERROR("socket skt_fd:%d Error by OnRecvble. ret=%d", fd, ret);
					eventer_ptr->OnError();
					continue;
				}
			}

			if(EPOLLOUT & events)	//可写
			{
				int ret = eventer_ptr->OnSendble();
				if(ret < 0)
				{
					LOG_ERROR("socket skt_fd:%d Error by OnSendble. ret=%d", fd, ret);
					eventer_ptr->OnError();
					continue;
				}
			}
        }
	}
	return 0;
}

int CEpoller::InitMc()
{
	CURLcode ret = curl_global_init(CURL_GLOBAL_ALL);
	if (CURLE_OK != ret)
        return -1;
	_mc = curl_multi_init();
	if(!_mc)	
		return -2;

	curl_multi_setopt(_mc, CURLMOPT_SOCKETFUNCTION, CEpoller::McSocketCallBack);
	curl_multi_setopt(_mc, CURLMOPT_SOCKETDATA, this);
	curl_multi_setopt(_mc, CURLMOPT_TIMERFUNCTION, CEpoller::McTimeoutCallBack);
	curl_multi_setopt(_mc, CURLMOPT_TIMERDATA, this);

	_mc_still_running = 0;
	return 0;
}
void CEpoller::DestoryMc()
{
	curl_multi_cleanup(_mc);
	_mc = NULL;
	_mc_still_running = 0;
	_mc_timeouter.cleanup();
	_mc_timer_id = 0;
	curl_global_cleanup();
}

void CEpoller::McCancelTimer()
{
	if(_mc_timer_id)
	{
		_mc_timeouter.remove(_mc_timer_id);
		_mc_timer_id = 0;
	}
}
void CEpoller::McSetTimer(uint32_t delay_time_)
{
	_mc_timer_id = _mc_timeouter.insert(std::bind(&CEpoller::McOnTimer, this, std::placeholders::_1, std::placeholders::_2), delay_time_);
}

void CEpoller::McOnTimer(mstime_t now_ms_, uint16_t repeat_num_)
{
	CURLMcode rc = curl_multi_socket_action(_mc, CURL_SOCKET_TIMEOUT, 0, &_mc_still_running);
	(void)rc;
	McCeckInfo();
}

void CEpoller::McOnSocketEvent(int skt_, int event_)
{
	if(skt_ < 0)
		return;
	int action = ((event_ & EPOLLIN) ? CURL_CSELECT_IN : 0) | ((event_ & EPOLLOUT) ? CURL_CSELECT_OUT : 0);
	CURLMcode rc = curl_multi_socket_action(_mc, skt_, action, &_mc_still_running);
	(void)rc;
	McCeckInfo();
}

void CEpoller::McCeckInfo()
{
	CURLMsg *msg = NULL;
	int msgs_left = 0;
	CURL *easy = NULL; 

	//fprintf(stdout, "REMAINING: %d\n", _mc_still_running);
	while((msg = curl_multi_info_read(_mc, &msgs_left))) 
	{
		if(msg->msg == CURLMSG_DONE) 
		{
			easy = msg->easy_handle;
			if(!easy)
			{
				//LOG_ERROR("!easy");
				continue;
			}
				
			void * private_data = NULL;
			CURLcode erc = curl_easy_getinfo(easy, CURLINFO_PRIVATE , &private_data);
			if(CURLE_OK != erc || !private_data)
			{
				//LOG_ERROR("CURLE_OK != erc || !private_data");
				continue;
			}
			CSocketEventer* evneter = (CSocketEventer*)private_data;
			if(evneter->Type() != e_se_type_libcurl_peer_connection)
			{
				//LOG_ERROR("evneter->Type() != e_se_type_libcurl_peer_connection");
				continue;
			}
				
			if(_recv_package_callback && evneter->PrivateData())
			{
				//LOG_INFO("asynckey=%lu", *(uint64_t*)(evneter->PrivateData()));
				_recv_package_callback(evneter, NULL, msg->data.result, *(uint64_t*)(evneter->PrivateData()));
			}	
		}
	}	
}

int CEpoller::McSocketCallBack(CURL* ec_, curl_socket_t skt_, int what_, void* cbp_, void* sockp_)
{
	if(!cbp_)
		return 0;
	CEpoller* epoller = (CEpoller*)cbp_;
	//const char *whatstr[]={ "none", "IN", "OUT", "INOUT", "REMOVE" };
	//fprintf(stdout, "socket callback: skt=%d ec=%p what=%s ", skt_, ec_, whatstr[what_]);
	if(what_ == CURL_POLL_REMOVE) 
	{
		epoller->DelSocketEventer(skt_);
	}
	else 
	{
		void * private_data = NULL;
		CURLcode erc = curl_easy_getinfo(ec_, CURLINFO_PRIVATE , &private_data);
		if(CURLE_OK != erc || !private_data)
			return 0;
		int kind = ((what_ & CURL_POLL_IN) ? EPOLLIN : 0) | ((what_ & CURL_POLL_OUT) ? EPOLLOUT : 0);
		if(!sockp_) 
		{
			epoller->AddSocketEventer(skt_, kind, (CSocketEventer*)private_data);
			if(epoller->GetMc())
				curl_multi_assign(epoller->GetMc(), skt_, private_data);
		}
		else 
		{
			epoller->ModSocketEventer(skt_, kind, (CSocketEventer*)private_data);
		}
	}
	return 0;
}

int CEpoller::McTimeoutCallBack(CURLM *mc_, long timeout_ms_, void* cbp_)
{
	//fprintf(stdout, "McTimeoutCallBack: Setting timeout to %ld ms\n", timeout_ms_);
	CEpoller* epoller = (CEpoller*)cbp_;
	uint32_t delay_time = 0;
	if(timeout_ms_ >= 0)
	{
		delay_time = (uint32_t)timeout_ms_;
		if(epoller)
		{
			epoller->McCancelTimer();
			epoller->McSetTimer(delay_time);
		}
	}
	else 
	{
		if(epoller)
			epoller->McCancelTimer();
	}
	return 0;
}
