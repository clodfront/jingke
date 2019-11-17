// Project:	jingke
// File:    server_core.h
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

#ifndef _server_core_h_
#define _server_core_h_

#include "timer.h"
#include "epoller.h"
#include "clock_time.h"
#include "async_worker.h"
#include "server_cfg.h"

namespace jingke {

class IServer;
class CTcpPeerConnection;
class CProtocolParser;

class CServerCore
{
public:
    CServerCore();
    ~CServerCore();
    int Init(IServer* server_, const char* std_cfg_file_);
    void Drive(int wait_time_);
    void Fini();
    int  SendToClient(uint32_t session_id_, const void* data_, size_t data_len_, bool close_after_send_ = false);
	int  SendToService(uint16_t service_id_, const void* data_, size_t data_len_, bool close_after_send_ = false);
    CTcpPeerConnection* CreateTcpPeerConnection(uint16_t service_id_, std::string addr_, uint16_t port_, CProtocolParser* parser_, size_t recv_buff_size_, size_t send_buff_size_);
    void CloseTcpPeerConnection(CTcpPeerConnection* connection_);
    uint32_t SetWallTimer(delay_call_t call_back_, uint32_t delay_time_, uint32_t interval_ = 0, uint16_t repeat_times_ = 0);
    uint32_t SetMonoTimer(delay_call_t call_back_, uint32_t delay_time_, uint32_t interval_ = 0, uint16_t repeat_times_ = 0);
    mstime_t GetNow(bool mono_ = false, bool exact_ = false);
    CEpoller* GetEpoller();
private:
    int ParseCfgFile(const char* std_cfg_file_);
    bool InitSocketBufferPool();
    bool InitTcpConnectionPool();
    bool ConnectToTcpPeerService();
    bool TcpListenerDoListen();
    int OnRecvPackage(CSocketEventer* socket_eventer_, const void* data_, size_t len_, uint64_t async_key_);
    void OnAddSktEventer(CSocketEventer* socket_eventer_);
    void OnDelSktEventer(CSocketEventer* socket_eventer_);
private:
    cached_clock_time	_cached_time;
    CTimerWall  _wall_timer;
    CTimerMono  _mono_timer;
    CEpoller    _epoller;
    IServer*    _server_imp;
    CServerCfg  _std_cfg;
    std::unordered_map<uint16_t, CSocketEventer*> _tcp_peer_connections;
    std::unordered_map<uint16_t, CSocketEventer*> _tcp_listeners;
    std::unordered_map<uint32_t, CSocketEventer*> _tcp_sessions;
};

} ///namespace jingke

#endif//_server_core_h_