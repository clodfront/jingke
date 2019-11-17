// Project:	jingke
// File:    task_http_test.cpp
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
#include "task_http_test.h"
#include "log_writer.h"
#include "server_core.h"
#include "singleton.h"

int CTaskHttpTest::Init()
{
    return 0;
}

bool CTaskHttpTest::on_start()
{
    if(0 != _web_connection.Init(singleton<CServerCore>::instance().GetEpoller(), get_async_id()))
        return true;

    static const char* urls[6] = { "http://www.baidu.com"
                            , "https://www.baidu.com"
                            , "http://www.invalid6741.com"
                            , "https://www.invalid6741.com"
                            , "http://www.163.com/"
                            , "https://www.163.com/" };

    int index = rand()%6;
    int ret = _web_connection.Get(urls[index]);
    if(0 != ret)
    {
        set_stop_code(-1);
        return true;
    }
    return false;
}

bool CTaskHttpTest::on_awaken(const void* data_, size_t len_) 
{
    CURL* ec = _web_connection.GetCurlPtr();
    if(!ec)
    {
        set_stop_code(-1);
        return true;
    }
    char *eff_url = NULL;
    curl_off_t total = 0;
    long http_code = 0;
    curl_easy_getinfo(ec, CURLINFO_EFFECTIVE_URL, &eff_url);
    curl_easy_getinfo(ec, CURLINFO_TOTAL_TIME_T, &total);
    curl_easy_getinfo(ec, CURLINFO_RESPONSE_CODE, &http_code);
    // len_ means CURLMsg->data.result
    LOG_INFO("DONE Get %s => (%d) (%d) total(%dms) errormsg:%s", eff_url, (int)len_, (int)http_code, (int)total/1000, _web_connection.ErrorMsg());
    //LOG_INFO("Response:\n%s\n", _web_connection.GetResponse().c_str());
    return true;
}