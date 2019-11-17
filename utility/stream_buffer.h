// Project:	jingke
// File:    stream_buffer.h
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

#ifndef _STREAM_BUFFER_H_
#define _STREAM_BUFFER_H_

#include <utility>
#include <stddef.h>

using std::pair;

namespace jingke {

class CStreamBuffer
{
public:
	struct SDataChunk
	{
		SDataChunk():_data_ptr(NULL),_data_len(0){}
		bool Valid(){return _data_ptr!=NULL && _data_len!=0;}
		char*  _data_ptr;
		size_t _data_len;
	};
	typedef pair<SDataChunk, SDataChunk> ChunkPair;
public:
	CStreamBuffer();
	~CStreamBuffer();
	int Init(char* buffer_ptr_, size_t buffer_len_);
	void Reset();
	void ClearData();
	char* GetFrontPtr();
	size_t GetCapacity();

	int WritePossible(const void *data_, size_t data_len_);
	int ReadPossible(void *buffer_, size_t buffer_len_);
	int ConsumePossible(size_t data_len_);
	int ProducePossible(size_t data_len_);
	
	int Write(const void *data_, size_t data_len_);
	int Read(void *buffer_, size_t buffer_len_);
	int Consume(size_t data_len_);
	int Produce(size_t data_len_);

	bool 	Empty();
	bool 	Full();
	char* 	ValidData();
	size_t 	ValidLength();
	char* 	FreeData();
	size_t 	FreeLength();

	ChunkPair GetDataChunk();
public:
	//if wanna make sure business packge is continuous data, it useful below interface MoveFront
	int MoveFront();
	
private:
	char*  _buffer_ptr;
	size_t _buffer_len;
	size_t _data_pos;
	size_t _data_len;
};

} ///namespace jingke

#endif  ///_STREAM_BUFFER_H_

