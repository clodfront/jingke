// Project:	jingke
// File:    server_core.cpp
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
#include "server_core.h"
#include <unordered_map>
#include "server.h"
#include "socket_eventer.h"
#include "socket_evneter_type.h"
#include "tcp_connection.h"
#include "tcp_peer_connection.h"
#include "tcp_listener.h"
#include "async_worker.h"
#include "singleton.h"
#include "obj_pool.h"
#include "log_writer.h"
#include "socket_rsbuffer_pool.h"
#include "tcp_connection_pool.h"
#include "json.hpp"
#include <fstream>

using namespace jingke;

uint32_t GSessionIDGen()
{
    static uint32_t session_id_seed = 0;
		if (!++session_id_seed) ++session_id_seed;
    return session_id_seed;
}

/////////////////

CServerCore::CServerCore()
:_server_imp(NULL)
{

}

CServerCore::~CServerCore()
{
    if(_server_imp)
        _server_imp = NULL;
}

int CServerCore::Init(IServer* server_, const char* std_cfg_file_)
{
    int ret = 0;
    if(!server_)
        return -1;
    _server_imp = server_;
    //config
    ret = ParseCfgFile(std_cfg_file_);
    if(ret != 0)
        return -2;
    //logger
    if(!singleton<CLog_writer>::instance().init(_std_cfg._log_cfg_file))
        return -3;
    //pool
    if(!InitSocketBufferPool())
        return -4;
    if(!InitTcpConnectionPool())
        return -5;
    //epoller
    ret =_epoller.Init();
    if(ret != 0)
        return -6;
    _epoller.SetRecvPackageCallBack(std::bind(&CServerCore::OnRecvPackage, this, std::placeholders::_1
        ,std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
    _epoller.SetAddSktEventerCallBack(std::bind(&CServerCore::OnAddSktEventer, this, std::placeholders::_1));
    _epoller.SetDelSktEventerCallBack(std::bind(&CServerCore::OnDelSktEventer, this, std::placeholders::_1));
    //service
    if(!ConnectToTcpPeerService())
        return -7;
    //listen
    if(!TcpListenerDoListen())
        return -8;

    return 0;
}

void CServerCore::Fini()
{
    _wall_timer.cleanup();
    _mono_timer.cleanup();
    for(auto& it : _tcp_peer_connections)
    {
        if(it.second)
        {
            it.second->Close();
            delete it.second;
        }
    }
    for(auto& it : _tcp_listeners)
    {
        if(it.second)
        {
            it.second->Close();
            delete it.second;
        }
    }
    _epoller.SetDelSktEventerCallBack(std::function<void (CSocketEventer*)>());
    for(auto& it : _tcp_sessions)
    {
        if(it.second)
        {
            it.second->Close();
        }
    }
    _epoller.Close();
    if(_server_imp)
        _server_imp = NULL;
}

CEpoller* CServerCore::GetEpoller()
{
    return &_epoller;
}

int CServerCore::ParseCfgFile(const char* std_cfg_file_)
{
    if(!std_cfg_file_) return -1;
    std::ifstream fin(std_cfg_file_);
    std::stringstream m;
    m << fin.rdbuf();
    std::string ctx = m.str();
    cedar::json obj = cedar::json::parse(ctx);
    std::cout << "json ctx: " << obj.cast() << std::endl;
    _std_cfg._log_cfg_file = obj["log_cfg_file"].cast();
    if(obj["tcp_listeners"].cast() != "null")
        for (auto &i : obj["tcp_listeners"]) {
            STcpListenerCfg tlcfg;
            auto &j = i.second;
            tlcfg._listener_id = j["listener_id"].cast<uint16_t>();
            tlcfg._local_point.SetAddr(j["local_point"]["addr"].cast());
            tlcfg._local_point.SetPort(j["local_point"]["port"].cast<uint16_t>());
            tlcfg._backlog = j["backlog"].cast<int32_t>();
            tlcfg._client_max_cnt = j["client_max_cnt"].cast<uint32_t>();
            tlcfg._client_sock_recv_buff_size = j["client_sock_recv_buff_size"].cast<int32_t>();
            tlcfg._client_sock_send_buff_size = j["client_sock_send_buff_size"].cast<int32_t>();
            tlcfg._client_keepalive_onoff = j["client_keepalive_onoff"].cast<int32_t>();
            tlcfg._client_keepalive_idle = j["client_keepalive_idle"].cast<int32_t>();
            tlcfg._client_keepalive_interval = j["client_keepalive_interval"].cast<int32_t>();
            tlcfg._client_keepalive_times = j["client_keepalive_times"].cast<int32_t>();
            _std_cfg._tcp_listeners.insert(std::make_pair(tlcfg._listener_id, tlcfg));
        }
    if(obj["tcp_peer_svrs"].cast() != "null")
        for (auto &i : obj["tcp_peer_svrs"]) {
            STcpPeerServiceCfg tpcfg;
            auto &j = i.second;
            tpcfg._service_id = j["service_id"].cast<uint16_t>();
            tpcfg._service_end_point.SetAddr(j["service_end_point"]["addr"].cast());
            tpcfg._service_end_point.SetPort(j["service_end_point"]["port"].cast<uint16_t>());
            tpcfg._service_sock_recv_buff_size = j["service_sock_recv_buff_size"].cast<int32_t>();
            tpcfg._service_sock_send_buff_size = j["service_sock_send_buff_size"].cast<int32_t>();    
            _std_cfg._tcp_peer_svrs.insert(std::make_pair(tpcfg._service_id, tpcfg));
        }
    _std_cfg._std_cfg_file = std_cfg_file_;
    return 0;
}

bool CServerCore::InitSocketBufferPool()
{
    std::unordered_map<size_t, size_t> summary;
    for(auto& it : _std_cfg._tcp_listeners)
    {
        STcpListenerCfg& cfg = it.second;
        summary[cfg._client_sock_recv_buff_size] += cfg._client_max_cnt;
		summary[cfg._client_sock_send_buff_size] += cfg._client_max_cnt;
    }
    for(auto& it : _std_cfg._tcp_peer_svrs)
    {
        STcpPeerServiceCfg& cfg = it.second;
        summary[cfg._service_sock_recv_buff_size] += 1;
		summary[cfg._service_sock_send_buff_size] += 1;
    }
    for(auto& it : summary)
    {
        LOG_DEBUG("joining skt buffer pool: %u, %u", (uint32_t)it.first, (uint32_t)it.second);
        if(!singleton<CSocketRSBufferPool>::instance().Join(it.first, it.second, true))
            return false;
    }
    LOG_DEBUG("joined skt buffer pool summary : %s", singleton<CSocketRSBufferPool>::instance().DebugInfoString().c_str());
    return true;
}

bool CServerCore::InitTcpConnectionPool()
{
    if(_std_cfg._tcp_listeners.empty())
        return true;
    size_t total_connection_cnt = 0;
    for(auto& it : _std_cfg._tcp_listeners)
    {
        STcpListenerCfg& cfg = it.second;
        total_connection_cnt += cfg._client_max_cnt;
    }
    if(0 != singleton<CTcpConnectionPool>::instance().Init(total_connection_cnt,true))
        return false;
    return true;
}

bool CServerCore::ConnectToTcpPeerService()
{
    for(auto& it : _std_cfg._tcp_peer_svrs)
    {
        STcpPeerServiceCfg& cfg = it.second;
        if(_tcp_peer_connections.find(cfg._service_id) != _tcp_peer_connections.end())
        {
            LOG_ERROR("service_id:%u repeat.", cfg._service_id);
            return false;  
        }
        CTcpPeerConnection* connection = new(std::nothrow) CTcpPeerConnection();
        if(!connection)
        {
            LOG_ERROR("new CTcpPeerConnection failed. service_id:%u", cfg._service_id);
            return false;
        }
        LOG_INFO("Start Connect To Peer Service:%u[%s:%u]", cfg._service_id, cfg._service_end_point.GetAddr().c_str(), cfg._service_end_point.GetPort()); 
        connection->SetServiceID(cfg._service_id);
        connection->SetTimerMgr(&_mono_timer);
        connection->SetPeerPoint(cfg._service_end_point);

        if(0 != connection->SetRecvBuff(singleton<CSocketRSBufferPool>::instance().New(cfg._service_sock_recv_buff_size), cfg._service_sock_recv_buff_size))
        {
            delete connection;
            return false;
        }
           
        if(0 != connection->SetSendBuff(singleton<CSocketRSBufferPool>::instance().New(cfg._service_sock_send_buff_size), cfg._service_sock_send_buff_size))
        {
            delete connection;
            return false;
        }
        connection->SetEpoller(&_epoller);
        connection->SetParser(_server_imp->GetServiceParser(cfg._service_id));
        int ret = connection->ConnectToService();
        if(ret != 0)
        {
            LOG_ERROR("Connect To Peer Service:%u[%s:%u] failed. ret:%d", cfg._service_id, cfg._service_end_point.GetAddr().c_str(), cfg._service_end_point.GetPort(), ret);
            delete connection; 
            continue;
        }
        _tcp_peer_connections.insert(std::make_pair(cfg._service_id, (CSocketEventer*)connection));
        LOG_INFO("Connect To Peer Service:%u[%s:%u] State:%d", cfg._service_id, cfg._service_end_point.GetAddr().c_str(), cfg._service_end_point.GetPort(), connection->GetState()); 
    }
    return true;
}

bool CServerCore::TcpListenerDoListen()
{
    for(auto& it : _std_cfg._tcp_listeners)
    {
        STcpListenerCfg& cfg = it.second;
        if(_tcp_listeners.find(cfg._listener_id) != _tcp_listeners.end())
        {
            LOG_WARN("service_id:%u repeat.", cfg._listener_id);
            return false;   
        }
        CTcpListener* listener = new(std::nothrow) CTcpListener();
        listener->Init(cfg);
        listener->SetEpoller(&_epoller);
        listener->SetParser(_server_imp->GetClientParser(cfg._listener_id));
        if(0 != listener->Listen())
        {
            delete listener;
            return false;
        }
            
        _tcp_listeners.insert(std::make_pair(cfg._listener_id, (CSocketEventer*)listener));
    }
    return true;
}

void CServerCore::Drive(int wait_time_)
{
    _cached_time.update();
    _epoller.Wait(wait_time_);
    _wall_timer.try_call(_cached_time.wall_mstimestamp_now());
    _mono_timer.try_call(_cached_time.mono_mstimestamp_now());
}

int CServerCore::OnRecvPackage(CSocketEventer* socket_eventer_, const void* data_, size_t len_, uint64_t async_key_)
{
    if(!socket_eventer_)
        return -1;
    if(!_server_imp)
        return -2;
    int type = socket_eventer_->Type();
    switch(type)
    {
        case e_se_type_tcp_connection:
        {
            CTcpConnection* connection = (CTcpConnection*)socket_eventer_;
            _server_imp->OnRecvClient(data_, len_, connection->GetSessionID(), connection->GetListenerID());
        }
        break;
        case e_se_type_tcp_peer_connection:
        {
            //LOG_DEBUG("peer recv asyncid:%lu", async_key_);
            CTcpPeerConnection* connection = (CTcpPeerConnection*)socket_eventer_;
            if(async_key_ != 0)
                singleton<CAsyncWorkerCenter>::instance().continue_task(async_key_, data_, len_);
            else
                _server_imp->OnRecvService(data_, len_, connection->GetServiceID());
        }
        break;
        case e_se_type_libcurl_peer_connection:
        {
            singleton<CAsyncWorkerCenter>::instance().continue_task(async_key_, NULL, len_);
        }
        break;
        default:
            return -3;
    }
    return 0;
}

void CServerCore::OnAddSktEventer(CSocketEventer* socket_eventer_)
{
    if(!socket_eventer_) return;
    switch(socket_eventer_->Type())
    {
        case e_se_type_tcp_connection:
        {
            CTcpConnection* connection = (CTcpConnection*)socket_eventer_;
            uint32_t session_id = GSessionIDGen();
            auto r = _tcp_sessions.insert(std::make_pair(session_id, socket_eventer_));
            if(r.second)
            {
                connection->SetSessionID(session_id);     
            }
        }
        break;
        default:
            return;
    }
}

void CServerCore::OnDelSktEventer(CSocketEventer* socket_eventer_)
{
    if(!socket_eventer_) return;
    switch(socket_eventer_->Type())
    {
        case e_se_type_tcp_connection:
        {
            CTcpConnection* connection = (CTcpConnection*)socket_eventer_;
            _tcp_sessions.erase(connection->GetSessionID());
        }
        break;
        default:
            return;
    }
}

int CServerCore::SendToClient(uint32_t session_id_, const void* data_, size_t data_len_, bool close_after_send_)
{
    auto it = _tcp_sessions.find(session_id_);
    if(it == _tcp_sessions.end())
        return -1;
    CSocketEventer* evnter = it->second;
    if(!evnter)
        return -2;
    switch(evnter->Type())
    {
        case e_se_type_tcp_connection:
        {
            CTcpConnection* connection = (CTcpConnection*)evnter;
            return connection->Send(data_, data_len_, close_after_send_);               
        }
        break;
        default:
            return -3;
    }
    return 0;
}

int CServerCore::SendToService(uint16_t service_id_, const void* data_, size_t data_len_, bool close_after_send_)
{
    auto it = _tcp_peer_connections.find(service_id_);
    if(it == _tcp_peer_connections.end())
        return -1;
    CSocketEventer* evnter = it->second;
    if(!evnter)
        return -2;
    switch(evnter->Type())
    {
        case e_se_type_tcp_peer_connection:
        {
            CTcpPeerConnection* connection = (CTcpPeerConnection*)evnter;
            return connection->Send(data_, data_len_, close_after_send_);               
        }
        break;
        default:
            return -3;
    }
    return 0;
}

CTcpPeerConnection* CServerCore::CreateTcpPeerConnection(uint16_t service_id_, std::string addr_, uint16_t port_, CProtocolParser* parser_, size_t recv_buff_size_, size_t send_buff_size_)
{
    if(!service_id_ || addr_.empty() || !parser_ || !recv_buff_size_ || !send_buff_size_)
    {
        LOG_ERROR("new CreateTcpPeerConnection failed. parameters invalid.");
        return NULL;
    }
    if(_tcp_peer_connections.find(service_id_) != _tcp_peer_connections.end())
    {
        LOG_ERROR("service_id:%u repeat.", service_id_);
        return NULL;     
    }
    CTcpPeerConnection* connection = new(std::nothrow) CTcpPeerConnection();
    if(!connection)
    {
        LOG_ERROR("new CTcpPeerConnection failed. service_id:%u", service_id_);
        return NULL;
    }
    LOG_INFO("Start Connect To Peer Service:%u[%s:%u]", service_id_, addr_.c_str(), port_); 
    connection->SetServiceID(service_id_);
    connection->SetTimerMgr(&_mono_timer);
    connection->SetPeerPoint(CSocketPoint(addr_, port_));

    if(0 != connection->SetRecvBuff(singleton<CSocketRSBufferPool>::instance().New(recv_buff_size_), recv_buff_size_))
    {
        delete connection;
        return NULL;
    }
        
    if(0 != connection->SetSendBuff(singleton<CSocketRSBufferPool>::instance().New(send_buff_size_), send_buff_size_))
    {
        delete connection;
        return NULL;
    }
    connection->SetEpoller(&_epoller);
    connection->SetParser(parser_);
    int ret = connection->ConnectToService();
    if(ret != 0)
    {
        LOG_ERROR("Connect To Peer Service:%u[%s:%u] failed. ret:%d", service_id_, addr_.c_str(), port_, ret); 
        delete connection;
        return NULL;
    }
    _tcp_peer_connections.insert(std::make_pair(service_id_, (CSocketEventer*)connection));
    LOG_INFO("Connect To Peer Service:%u[%s:%u] State:%d", service_id_, addr_.c_str(), port_, connection->GetState());     
    return connection;
}

void CServerCore::CloseTcpPeerConnection(CTcpPeerConnection* connection_)
{
    if(!connection_) 
        return;
    uint16_t service_id = connection_->GetServiceID();
    auto it = _tcp_peer_connections.find(service_id);
    if(it != _tcp_peer_connections.end())
        _tcp_peer_connections.erase(service_id);
    connection_->Close();
    delete connection_;
}

uint32_t CServerCore::SetWallTimer(delay_call_t call_back_, uint32_t delay_time_, uint32_t interval_, uint16_t repeat_times_)
{
    return _wall_timer.insert(call_back_, delay_time_, interval_, repeat_times_);
}

uint32_t CServerCore::SetMonoTimer(delay_call_t call_back_, uint32_t delay_time_, uint32_t interval_, uint16_t repeat_times_)
{
    return _mono_timer.insert(call_back_, delay_time_, interval_, repeat_times_);
}

mstime_t CServerCore::GetNow(bool mono_, bool exact_)
{
    if(exact_)
        return mono_ ? mono_mstimestamp_now() : wall_mstimestamp_now();
    return mono_ ? _cached_time.mono_mstimestamp_now() : _cached_time.wall_mstimestamp_now();
}

