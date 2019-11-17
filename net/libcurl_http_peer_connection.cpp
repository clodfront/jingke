// Project:	jingke
// File:    libcurl_http_peer_connection.cpp
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
#include "libcurl_http_peer_connection.h"
#include "epoller.h"

using namespace jingke;

size_t CLibcurlHttpPeerConnection::RecvCallBack(void *ptr_, size_t size_, size_t nmemb_, void *data_)
{
	if(!data_)
		return -1;
	CLibcurlHttpPeerConnection* connection = (CLibcurlHttpPeerConnection*)data_;
	connection->OnRecvData(ptr_, size_*nmemb_);
	return size_*nmemb_;
}

int CLibcurlHttpPeerConnection::SockoptCreateCallback(void *clientp_, curl_socket_t curlfd_, curlsocktype purpose_)
{
	(void)purpose_;
	if(!clientp_)
		return CURL_SOCKOPT_ERROR;
	CLibcurlHttpPeerConnection* connection = (CLibcurlHttpPeerConnection*)clientp_;
	connection->OnSocketCreate(curlfd_);
	return 0;
}

void CLibcurlHttpPeerConnection::OnRecvData(void *ptr_, size_t size_)
{
	_response.append((char*)ptr_, size_);
}

void CLibcurlHttpPeerConnection::OnSocketCreate(int fd_)
{
	_sock.SetFd(fd_);
}

const char* CLibcurlHttpPeerConnection::ErrorMsg()
{
	return _error_msg;
}

int CLibcurlHttpPeerConnection::Init(CEpoller* epoller_, uint64_t async_key_, long timeout_)
{
	if(!epoller_)
		return -1;
	SetEpoller(epoller_);
	_async_key = async_key_;
	_ec = curl_easy_init();
	if(!_ec)
		return -2;

	//关闭向标准错误输出DEBUG信息，默认值0
	curl_easy_setopt(_ec, CURLOPT_VERBOSE, 0L); 
	//设置收数据回调
	curl_easy_setopt(_ec, CURLOPT_WRITEFUNCTION, CLibcurlHttpPeerConnection::RecvCallBack);
	curl_easy_setopt(_ec, CURLOPT_WRITEDATA, this);
	//设置错误文本输出位置
	curl_easy_setopt(_ec, CURLOPT_ERRORBUFFER, _error_msg);
	//关联私有数据
	curl_easy_setopt(_ec, CURLOPT_PRIVATE, (CSocketEventer*)this);
	//对301/2等的自动跳转，默认值0
	curl_easy_setopt(_ec, CURLOPT_FOLLOWLOCATION, 1L);
	//不传送回包的HTTP头部信息到CURLOPT_WRITEFUNCTION设置的函数中，默认值0
	curl_easy_setopt(_ec, CURLOPT_HEADER, 0L); 

	//设置套接字创建后回调
	curl_easy_setopt(_ec, CURLOPT_SOCKOPTFUNCTION, CLibcurlHttpPeerConnection::SockoptCreateCallback);	
	curl_easy_setopt(_ec, CURLOPT_SOCKOPTDATA, this);
	//设置交互完毕后关闭套接字，不进行同域名复用sockt。防止wait_close堆积，默认值0
	curl_easy_setopt(_ec, CURLOPT_FORBID_REUSE, 1L);
	//设置超时，默认超时为0表示永不超时
	curl_easy_setopt(_ec, CURLOPT_TIMEOUT, timeout_);

	return 0;
}

int CLibcurlHttpPeerConnection::Get(const std::string& url_)
{
	if(!_ec || !GetEpoller() || !GetEpoller()->GetMc()) 
		return -1;
	curl_easy_setopt(_ec, CURLOPT_URL, url_.c_str());
	if(0 == url_.compare(0, 5, "https"))
	{
		curl_easy_setopt(_ec, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1);		
		curl_easy_setopt(_ec, CURLOPT_SSL_VERIFYPEER, 0L);//设定为不验证证书和HOST
		curl_easy_setopt(_ec, CURLOPT_SSL_VERIFYHOST, 0L);
	}
	if(CURLM_OK != curl_multi_add_handle(GetEpoller()->GetMc(), _ec))
		return -2;
	return 0;
}

int CLibcurlHttpPeerConnection::Post(const std::string& url_, const std::string& post_data_)
{
	if(!_ec || !GetEpoller() || !GetEpoller()->GetMc()) 
		return -1;
	curl_easy_setopt(_ec, CURLOPT_URL, url_.c_str());
    curl_easy_setopt(_ec, CURLOPT_POST, 1);
    curl_easy_setopt(_ec, CURLOPT_POSTFIELDS, post_data_.c_str());
    curl_easy_setopt(_ec, CURLOPT_POSTFIELDSIZE, post_data_.size());
	if(0 == url_.compare(0, 5, "https"))
	{
		curl_easy_setopt(_ec, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1);		
		curl_easy_setopt(_ec, CURLOPT_SSL_VERIFYPEER, 0L);//设定为不验证证书和HOST
		curl_easy_setopt(_ec, CURLOPT_SSL_VERIFYHOST, 0L);
	}
	if(CURLM_OK != curl_multi_add_handle(GetEpoller()->GetMc(), _ec))
		return -2;
	return 0;
}

void CLibcurlHttpPeerConnection::Close()
{
	if(_ec)
	{
		if(GetEpoller() && GetEpoller()->GetMc())
			curl_multi_remove_handle(GetEpoller()->GetMc(), _ec);
		curl_easy_cleanup(_ec);
		_ec = NULL;
	}
	_error_msg[0] = '\0';
	_async_key = 0;
	_sock.SetFd(INVALID_SOCKET);
	SetEpoller(NULL);
}

