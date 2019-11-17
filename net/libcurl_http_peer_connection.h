// Project:	jingke
// File:    libcurl_http_peer_connection.h
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

#ifndef _libcurl_http_peer_connection_h_
#define _libcurl_http_peer_connection_h_

#include "socket_eventer.h"
#include "tcp_socket.h"
#include "socket_evneter_type.h"
#include <string>
#include "curl/curl.h"

namespace jingke {

class CEpoller;

class CLibcurlHttpPeerConnection : public CSocketEventer
{
public:
    CLibcurlHttpPeerConnection():_ec(NULL),_async_key(0){_error_msg[0] = '\0';}
    virtual ~CLibcurlHttpPeerConnection(){}
public:
    //not use them
    virtual int OnRecvble() final { return 0;}
	virtual int OnSendble() final { return 0;}
    virtual void OnError() final {}
public:
    virtual void* PrivateData() { return &_async_key; }
    virtual int Type() final { return e_se_type_libcurl_peer_connection; }
    virtual ISocket& Socket() final { return _sock; }
    virtual void Close() override;
protected:
    static size_t RecvCallBack(void *ptr_, size_t size_, size_t nmemb_, void *data_);
    static int SockoptCreateCallback(void *clientp_, curl_socket_t curlfd_, curlsocktype purpose_);
    void OnRecvData(void *ptr_, size_t size_);
    void OnSocketCreate(int fd_);
public:
    int Init(CEpoller* epoller_, uint64_t async_key_, long timeout_ = 0);
    int Get(const std::string& url_);
    int Post(const std::string& url_, const std::string& post_data_);
    CURL* GetCurlPtr(){return _ec;}
    const std::string& GetResponse(){return _response;}
    const char* ErrorMsg();
protected:
    CTcpSocket _sock;
    CURL* _ec;
    std::string _response;
    char _error_msg[CURL_ERROR_SIZE];
    uint64_t _async_key;
};

} ///namespace jingke

#endif//_libcurl_http_peer_connection_h_
