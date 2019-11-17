// Project:	jingke
// File:    protocol_parser.h
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

#ifndef _PROTOCOL_PARSER_H_
#define _PROTOCOL_PARSER_H_

#include <stdlib.h>
#include <stdint.h>

namespace jingke {

#define UNDETERMINED_PACK_SIZE 0
#define INVALID_PACK_SIZE (~size_t(0))

class CProtocolParser
{
public:
	CProtocolParser(){};
	virtual ~CProtocolParser(){};

	virtual size_t GetPackSize(const void* data_, size_t length_) = 0;	//0表示未确定大小;INVALID_PACK_SIZE表示非法尺寸，会导致连接断开
	virtual uint64_t GetAsyncKey(const void* data_, size_t length_) = 0;
};

} ///namespace jingke

#endif  ///_PROTOCOL_PARSER_H_

