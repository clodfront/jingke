// Project:	jingke
// File:    test_protoc.h
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
#ifndef _test_protoc_h_
#define _test_protoc_h_

#include <stdint.h>
#include "google/protobuf/message.h"
#include "omygod.h"



//Communication protocol example desc

///*-------------------------------------*---------------------------*
///|       head                          |        body               |
///|-----------------------------------------------------------------|
///| len     |   cmd       |   asyncid   |        pb message data    |
///|---------|-------------|-------------|---------------------------|
///| 4byte   |   4byte     |   8byte     |                           |
///*---------*-------------*-------------*---------------------------*
#pragma pack(1)

struct SPackHead
{
	uint32_t len;
	uint32_t cmd;
	uint64_t asyncid;
};

#pragma pack()

class ExampleMessageCoder
{
public:
	static bool encode(void* buff_, size_t buff_len_, const SPackHead& head_, const google::protobuf::Message* body_, size_t* use_len_=NULL)
	{
		if(!buff_ || buff_len_ < sizeof(SPackHead)) 
			return false;
		size_t pack_len = sizeof(SPackHead);
		if(body_)
		{
			const size_t body_size = body_->ByteSizeLong();
			pack_len += body_size;
			if(buff_len_ < pack_len)
				return false;
			if(!body_->SerializeToArray((char*)buff_ + sizeof(SPackHead), buff_len_ - sizeof(SPackHead)))
				return false;		
		}

		SPackHead& head = *(SPackHead*)buff_;
		head.len = htonl(pack_len);
		head.cmd = htonl(head_.cmd);
		head.asyncid = hton64(head_.asyncid); 
		
		if(use_len_)
			*use_len_ = pack_len;

		return true;
	}
	static bool decode_head(const void* buff_, size_t buff_len_, SPackHead& head_)
	{
		if(!buff_ || buff_len_ < sizeof(SPackHead)) 
			return false;
		const SPackHead& head = *(const SPackHead*)buff_;
		head_.len = ntohl(head.len);
		head_.cmd = ntohl(head.cmd);
		head_.asyncid = ntoh64(head.asyncid);
		return true;
	}
	static bool decode_body(const void* buff_, size_t buff_len_, google::protobuf::Message& body_)
	{
		if(!buff_ || buff_len_ < sizeof(SPackHead)) 
			return false;
		return body_.ParseFromArray((const char*)buff_ + sizeof(SPackHead), buff_len_- sizeof(SPackHead));
	}
};

#endif  ///_test_protoc_h_

