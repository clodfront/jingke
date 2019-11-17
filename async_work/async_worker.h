// Project:	jingke
// File:    async_worker.h
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

#ifndef __async_worker_h__
#define __async_worker_h__

#include <stdint.h>
#include <unordered_map>
#include "order_multi_tree.h"
#include "timer.h"

namespace jingke {

	enum e_task_tree_node_type
	{
		e_node_type_task,
		e_node_type_root,
		e_node_type_concurrenter,
	};

	class IAsnycTaskTreeNode : public SOrderMultiTreeNode<IAsnycTaskTreeNode>
	{
	public:
		IAsnycTaskTreeNode();
		virtual ~IAsnycTaskTreeNode();
	protected:
		bool __add_sub_task(IAsnycTaskTreeNode * sub_);
	public:
		virtual e_task_tree_node_type node_type() const = 0;
		virtual const char* name() const;		//默认返回空串
	};

	enum e_stop_code
	{
		e_stop_code_timeout = -2147483648,
		e_stop_code_ok		= 0,
	};

	enum e_task_state
	{
		e_task_wait_start = 0,
		e_task_suspended,
		e_task_stoped,
	};

	typedef uint64_t async_id_t;

	class IAsyncTask : public IAsnycTaskTreeNode
	{
	public:
		IAsyncTask();
		virtual ~IAsyncTask();
		e_task_state	get_task_state() const;
		int				get_stop_code() const;
		void			set_stop_code(int stop_code_);
		e_task_state	start();
		e_task_state	awaken(const void* data_, size_t len_);
		e_task_state	__timeout_awaken();
		void			__timeouter_call(mstime_t mono_now_, uint16_t round_);
		void			__timeouter_cancel();
		async_id_t		get_async_id() const;
		void			__set_async_id(async_id_t async_id_);
	public:
		virtual bool on_start() = 0;			//开始任务 返回true表示任务终止，否则表示被挂起
		virtual bool on_awaken(const void* data_, size_t len_);	//唤醒任务 返回值意义同on_start， 默认返回true
		virtual uint32_t time_out_time() {return 0;}	//超时时间, 0表示不超时，单位毫秒
	protected:
		e_task_tree_node_type node_type() const final{ return e_node_type_task; }
	private:
		e_task_state _run_state;
		int	_stop_code;
		async_id_t _async_id;
		uint32_t   _time_out_timer_id;
	};

	class CAsyncTaskConcurrenter : public IAsnycTaskTreeNode
	{
	public:
		CAsyncTaskConcurrenter(){}
		virtual ~CAsyncTaskConcurrenter(){}
	protected:
		e_task_tree_node_type node_type() const final{ return e_node_type_concurrenter; }
	public:
		bool add_task(IAsnycTaskTreeNode* task_) { return __add_sub_task(task_); }
	};


	enum e_work_state
	{
		e_work_wait_start = 0,
		e_work_running,
		e_work_stoped,
	};

	class IAsyncWorker : public IAsnycTaskTreeNode
	{
		friend struct WorkerHelper;
	public:
		IAsyncWorker();
		virtual ~IAsyncWorker();
		e_work_state get_work_state() const;
		int	 get_stop_code() const;
		void set_stop_code(int stop_code_);
		bool push_task(IAsnycTaskTreeNode* task_);
		bool erase_task(IAsnycTaskTreeNode* task_);
		int  start_work();
	protected:
		e_task_tree_node_type node_type() const final{ return e_node_type_root; }
	public:
		virtual bool on_task_stop(IAsyncTask* task_) = 0;			//当某个子任务结束时调用，返回true表示工作终止，否则继续
		virtual bool on_concurrenter_stop(CAsyncTaskConcurrenter* concurrenter_) = 0;	//当某个并发任务集结束时调用，返回true表示工作终止，否则继续
		virtual void on_work_stop();								//当工作执行完毕的时候调用，默认空实现
	private:
		e_work_state _work_state;
		int	_stop_code;
	};


	class CAsyncWorkerCenter
	{
	public:
		CAsyncWorkerCenter():_timer_mgr(NULL){}
		~CAsyncWorkerCenter(){_timer_mgr = NULL;}
		int  start_work(IAsyncWorker* work_);
		bool continue_task(async_id_t task_id_, const void* data_, size_t len_);
		bool continue_task(IAsyncTask* task_, const void* data_, size_t len_);
		bool continue_task_timeout(IAsyncTask* task_);
		void set_timer_mgr(CTimerMono* timer_mgr_){_timer_mgr = timer_mgr_;}
		CTimerMono* get_timer_mgr() {return _timer_mgr;}
	public:
		bool mark_suspended_task(IAsyncTask* task_);
		bool unmark_suspended_task(async_id_t task_id_);
	private:
		std::unordered_map<async_id_t, IAsyncTask*> _suspended_task_map; //被挂起的任务集
		CTimerMono* _timer_mgr;
	};

} ///namespace jingke

#endif // __async_worker_h__

