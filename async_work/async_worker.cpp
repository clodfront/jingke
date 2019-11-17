// Project:	jingke
// File:    async_worker.cpp
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
#include "async_worker.h"
#include "singleton.h"
#include <stdio.h>
#include "log_writer.h"

using namespace jingke;

IAsnycTaskTreeNode::IAsnycTaskTreeNode()
{
}

IAsnycTaskTreeNode::~IAsnycTaskTreeNode()
{

}

bool IAsnycTaskTreeNode::__add_sub_task(IAsnycTaskTreeNode * sub_)
{
	if (!sub_)
		return false;

	if (e_node_type_task == node_type())
		return false;
	if (sub_->node_type() != e_node_type_task && sub_->node_type() != e_node_type_concurrenter)
		return false;
	if (e_node_type_concurrenter == sub_->node_type() && node_type() != e_node_type_root)
		return false;
	if (sub_->has_children() && sub_->node_type() != e_node_type_concurrenter)
		return false;
	if (!sub_->has_children() && sub_->node_type() != e_node_type_task)
		return false;
	if (!add_children(sub_))
		return false;
	return true;
}

const char* IAsnycTaskTreeNode::name() const
{
	return "";
}

///////////////////////////////////////////////////////////////////////////////////////////////

IAsyncTask::IAsyncTask() :_run_state(e_task_wait_start), _stop_code(0), _async_id(0), _time_out_timer_id(0)
{
}

IAsyncTask::~IAsyncTask()
{
	_run_state = e_task_wait_start;
	_stop_code = 0;
	if(_async_id != 0)
	{
		singleton<CAsyncWorkerCenter>::instance().unmark_suspended_task(_async_id);
		_async_id = 0;
	}
	if(_time_out_timer_id != 0)
	{
		if(singleton<CAsyncWorkerCenter>::instance().get_timer_mgr())
		{
			singleton<CAsyncWorkerCenter>::instance().get_timer_mgr()->remove(_time_out_timer_id);
			_time_out_timer_id = 0;	
		}
	}
}

e_task_state IAsyncTask::get_task_state() const
{
	return _run_state;
}

int IAsyncTask::get_stop_code() const
{
	return _stop_code;
}

void IAsyncTask::set_stop_code(int stop_code_)
{
	_stop_code = stop_code_;
}

e_task_state IAsyncTask::start()
{
	static async_id_t id_seed = 0;
	if (!++id_seed) ++id_seed;
	this->__set_async_id(id_seed);

	if (on_start()) //如果执行完毕了，设置stop，否则等待
		_run_state = e_task_stoped;
	else
	{
		if(time_out_time() != 0)
		{
			if(singleton<CAsyncWorkerCenter>::instance().get_timer_mgr())
				_time_out_timer_id = singleton<CAsyncWorkerCenter>::instance().get_timer_mgr()->insert(std::bind(&IAsyncTask::__timeouter_call, this, std::placeholders::_1
				, std::placeholders::_2), time_out_time());			
		}
		_run_state = e_task_suspended;			
	}
	return _run_state;
}

e_task_state IAsyncTask::awaken(const void* data_, size_t len_)
{
	if (on_awaken(data_, len_))
		_run_state = e_task_stoped;
	else
		_run_state = e_task_suspended;
	return _run_state;
}

e_task_state IAsyncTask::__timeout_awaken()
{
	_run_state = e_task_stoped;
	_stop_code = e_stop_code_timeout;
	return _run_state;
}

async_id_t IAsyncTask::get_async_id() const
{
	return _async_id;
}

void IAsyncTask::__set_async_id(async_id_t async_id_)
{
	_async_id = async_id_;
}

bool IAsyncTask::on_awaken(const void* data_, size_t len_)
{
	return true;
}

void IAsyncTask::__timeouter_call(mstime_t mono_now_, uint16_t round_)
{
	_time_out_timer_id = 0;
	singleton<CAsyncWorkerCenter>::instance().continue_task_timeout(this);
}

void IAsyncTask::__timeouter_cancel()
{
	if(_time_out_timer_id)
	{
		if(singleton<CAsyncWorkerCenter>::instance().get_timer_mgr())
			singleton<CAsyncWorkerCenter>::instance().get_timer_mgr()->remove(_time_out_timer_id);
		_time_out_timer_id = 0;
	}
}

/////////////////////////////////
namespace jingke {
struct WorkerHelper
{
	static void modify_state(IAsyncWorker* worker_, e_work_state st_)
	{
		if (worker_)
		{
			worker_->_work_state = st_;
		}
	}
	static void drive_work(IAsyncWorker* work_, IAsnycTaskTreeNode* start_node_ = NULL)
	{
		if (!work_)
			return;

		IAsnycTaskTreeNode* start_node = start_node_;
		do
		{
			IAsnycTaskTreeNode* node = start_node ? start_node : work_->get_left_deepest();

			switch (node->node_type())
			{
			case e_node_type_task:
			{
				IAsyncTask* task = (IAsyncTask*)node;
				IAsnycTaskTreeNode* parent = node->get_parent();
				e_task_state st = task->start();

				if (e_node_type_root == parent->node_type())//不是并发类节点任务
				{
					if (e_task_stoped == st)
					{
						work_->erase_task(node);
						if (work_->on_task_stop(task))
						{
							WorkerHelper::modify_state(work_, e_work_stoped);
							work_->on_work_stop();
							return;
						}
						start_node = NULL;
						continue;
					}
					else if (e_task_suspended == st)
					{
						//标记到挂起集合中 
						singleton<CAsyncWorkerCenter>::instance().mark_suspended_task(task);
						return;
					}
				}
				else if (e_node_type_concurrenter == parent->node_type()) //并发类节点任务
				{
					IAsnycTaskTreeNode* next = node->get_next();
					if (e_task_stoped == st)
					{
						work_->erase_task(node);
						if (work_->on_task_stop(task))
						{
							WorkerHelper::modify_state(work_, e_work_stoped);
							work_->on_work_stop();
							return;
						}
					}
					else if (e_task_suspended == st)
					{
						singleton<CAsyncWorkerCenter>::instance().mark_suspended_task(task);
					}
					if (next)
					{
						start_node = next;
						continue;
					}
					else
					{
						start_node = parent;
						continue;
					}
				}
			}
			break;
			case e_node_type_concurrenter:
			{
				//判定其下所有子任务是否都已完毕
				CAsyncTaskConcurrenter* concurrenter = (CAsyncTaskConcurrenter*)node;
				for (IAsnycTaskTreeNode::children_iterator sub_it = concurrenter->children_begin(); sub_it != concurrenter->children_end(); ++sub_it)
				{
					IAsyncTask* task = (IAsyncTask*)&(*sub_it);
					if (e_task_stoped != task->get_task_state()) //有子任务没有完毕
						return;
				}
				work_->erase_task(node);
				if (work_->on_concurrenter_stop(concurrenter))
				{
					WorkerHelper::modify_state(work_, e_work_stoped);
					work_->on_work_stop();
					return;
				}
				start_node = NULL;
				continue;
			}
			break;
			case e_node_type_root:
				work_->set_stop_code(0);
				work_->on_work_stop();
				return;
			}

		} while (true);
		
	}

};
} ///namespace jingke 

IAsyncWorker::IAsyncWorker() :_work_state(e_work_wait_start), _stop_code(0)
{
}

IAsyncWorker::~IAsyncWorker()
{
	_work_state = e_work_wait_start;
	_stop_code = 0;
}

e_work_state IAsyncWorker::get_work_state() const
{
	return _work_state;
}

int IAsyncWorker::get_stop_code() const
{
	return _stop_code;
}

void IAsyncWorker::set_stop_code(int stop_code_)
{
	_stop_code = stop_code_;
}

bool IAsyncWorker::push_task(IAsnycTaskTreeNode* task_)
{
	return __add_sub_task(task_);
}

bool IAsyncWorker::erase_task(IAsnycTaskTreeNode* task_)
{
	//只允许删除根节点上的子节点
	if (!task_ || task_->get_parent() != this)
		return false;
	//如果工作还未开始或者已经结束可以删除
	if (e_work_running != _work_state)
	{
		task_->clear_children();
		this->del_children(task_);
	}
	else
	{
		//不允许删除被挂起的普通节点和有子任务被挂起的并发类节点
		switch (task_->node_type())
		{
		case e_node_type_task:
			if (e_task_suspended == ((IAsyncTask*)task_)->get_task_state())
				return false;
			break;
		case e_node_type_concurrenter:
			for (IAsnycTaskTreeNode::children_iterator sub_it = task_->children_begin(); sub_it != task_->children_end(); ++sub_it)
			{
				IAsyncTask* task = (IAsyncTask*)&(*sub_it);
				if (e_task_suspended == task->get_task_state())
					return false;
			}
			break;
		default:
			return false;
		}
		task_->clear_children();
		this->del_children(task_);
	}
	return true;
}

int IAsyncWorker::start_work()
{
	if (_work_state != e_work_wait_start)
		return -1;
	if (!has_children())
		return -2;
	_work_state = e_work_running;
	WorkerHelper::drive_work(this);
	return 0;
}

void IAsyncWorker::on_work_stop()
{

}

////////////////////////////////////////////////////////////////////////////
int CAsyncWorkerCenter::start_work(IAsyncWorker* work_)
{
	if (!work_)
		return -1;
	if (work_->start_work() != 0)
		return -2;
	return 0;
}

bool CAsyncWorkerCenter::continue_task(IAsyncTask* task_, const void* data_, size_t len_)
{
	if (!task_ || task_->get_task_state() != e_task_suspended)
		return false;
	e_task_state st = task_->awaken(data_, len_);
	if (e_task_suspended == st) //唤醒过程中继续被挂起了
		return true;
	if (e_task_stoped == st)
	{
		task_->__timeouter_cancel();
		IAsnycTaskTreeNode* root_node = task_->get_tree_root();
		if (!root_node || root_node->node_type() != e_node_type_root)
			return false;
		IAsyncWorker* worker = (IAsyncWorker*)root_node;

		IAsnycTaskTreeNode* parent = task_->get_parent();
		unmark_suspended_task(task_->get_async_id()); //从挂起集合中消除掉 
		worker->erase_task(task_);
		if (worker->on_task_stop(task_))
		{
			WorkerHelper::modify_state(worker, e_work_stoped);
			worker->on_work_stop();
			return true;
		}
		WorkerHelper::drive_work(worker, parent->node_type() == e_node_type_root ? NULL : parent);
	}
	return true;
}

bool CAsyncWorkerCenter::continue_task(async_id_t task_id_, const void* data_, size_t len_)
{
	auto r = _suspended_task_map.find(task_id_);
	if (r == _suspended_task_map.end() || !r->second)
	{
		LOG_ERROR("asyncid:%lu is not in suspended map!", task_id_);
		return false;
	}
		
	return continue_task(r->second, data_, len_);
}

bool CAsyncWorkerCenter::continue_task_timeout(IAsyncTask* task_)
{
	if (!task_ || task_->get_task_state() != e_task_suspended)
		return false;
	e_task_state st = task_->__timeout_awaken(); 
	assert(e_task_stoped == st);//only return e_task_stoped

	IAsnycTaskTreeNode* root_node = task_->get_tree_root();
	if (!root_node || root_node->node_type() != e_node_type_root)
		return false;
	IAsyncWorker* worker = (IAsyncWorker*)root_node;

	IAsnycTaskTreeNode* parent = task_->get_parent();
	unmark_suspended_task(task_->get_async_id()); //从挂起集合中消除掉 
	worker->erase_task(task_);
	if (worker->on_task_stop(task_))
	{
		WorkerHelper::modify_state(worker, e_work_stoped);
		worker->on_work_stop();
		return true;
	}
	WorkerHelper::drive_work(worker, parent->node_type() == e_node_type_root ? NULL : parent);
	return true;
}

bool CAsyncWorkerCenter::mark_suspended_task(IAsyncTask* task_)
{
	if (!task_)
		return false;
	if (task_->get_task_state() != e_task_suspended)
		return false;
	if (task_->get_async_id() == 0)
		return false;
	auto r = _suspended_task_map.insert(std::make_pair(task_->get_async_id(), task_));
	if (!r.second) //真要能走到这里也是醉了
		return false;
	//printf("mark_suspended_task:%s:%lu\n", task_->name(), id_seed);
	return true;
}

bool CAsyncWorkerCenter::unmark_suspended_task(async_id_t task_id_)
{
	auto r = _suspended_task_map.find(task_id_);
	if (r == _suspended_task_map.end())
		return false;
	if (r->second)
		r->second->__set_async_id(0);
	_suspended_task_map.erase(task_id_);
	//printf("unmark_suspended_task:%s:%lu\n", r->second->name(), task_id_);
	return true;
}