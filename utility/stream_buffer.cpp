// Project:	jingke
// File:    stream_buffer.cpp
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
#include "stream_buffer.h"
#include <string.h>

using namespace jingke;

CStreamBuffer::CStreamBuffer()
	:_buffer_ptr(NULL)
	,_buffer_len(0)
	,_data_pos(0)
	,_data_len(0)
{
}

CStreamBuffer::~CStreamBuffer()
{
}

int CStreamBuffer::Init(char* buffer_ptr_, size_t buffer_len_)
{
	if(!buffer_ptr_ || !buffer_len_)
		return -1;
	_buffer_ptr = buffer_ptr_;
	_buffer_len = buffer_len_;
	_data_pos = 0;
	_data_len = 0;	
	return 0;
}

void CStreamBuffer::Reset()
{
	_buffer_ptr = NULL;
	_buffer_len = 0;
	_data_pos = 0;
	_data_len = 0;	
}

void CStreamBuffer::ClearData()
{
	_data_pos = 0;
	_data_len = 0;
}

char* CStreamBuffer::GetFrontPtr()
{
	return _buffer_ptr;
}

size_t CStreamBuffer::GetCapacity()
{
	return _buffer_len;
}

int CStreamBuffer::WritePossible(const void *data_, size_t data_len_)
{
	if(!data_ || !data_len_)
		return -1;
	if(_buffer_len == _data_len)
		return -2; //full
	size_t real_len = data_len_;
	if(_buffer_len - _data_len < data_len_)	
		real_len = _buffer_len - _data_len;
	
	size_t np = (_data_pos + _data_len) % _buffer_len;

	if(_data_pos > np )
	{
		memcpy(_buffer_ptr + np, data_, real_len);
	}
	else
	{
		if(_buffer_len - np >= real_len)
		{
			memcpy(_buffer_ptr + np, data_, real_len);
		}
		else
		{
			size_t first = _buffer_len - np;
			memcpy(_buffer_ptr + np, data_, first);
			size_t second = real_len - first;
			memcpy(_buffer_ptr, (char*)data_ + first, second);
		}
	}
	_data_len += real_len;
	return (int)real_len;
}


int CStreamBuffer::ReadPossible(void *buffer_, size_t buffer_len_)
{
	if(!buffer_ || !buffer_len_)
		return -1;
	if(0 == _data_len)
		return -2;
	size_t real_len = buffer_len_;
	if(buffer_len_ > _data_len)
		real_len = _data_len;
	size_t np = (_data_pos + _data_len) % _buffer_len;
	if(_data_pos < np)
	{
		memcpy(buffer_, _buffer_ptr + _data_pos, real_len);
	}
	else
	{
		if(_buffer_len - _data_pos >= real_len)
		{
			memcpy(buffer_, _buffer_ptr + _data_pos, real_len);
		}
		else
		{
			size_t first = _buffer_len - _data_pos;
			memcpy(buffer_, _buffer_ptr + _data_pos, first);
			size_t second = real_len - first;
			memcpy((char*)buffer_ + first, _buffer_ptr, second);
		}
	}
	_data_pos = (_data_pos + real_len) % _buffer_len;
	_data_len -= real_len;
	if(0 == _data_len && _data_pos != 0) //make sure _data_pos is front when empty.
		_data_pos = 0;
	return (int)real_len;
}


int CStreamBuffer::ConsumePossible(size_t data_len_)
{
	if(!data_len_)
		return -1;
	size_t real_len = data_len_;
	if(data_len_ > _data_len)
		real_len = _data_len;
	_data_pos = (_data_pos + real_len) % _buffer_len;
	_data_len -= real_len;	
	if(0 == _data_len && _data_pos != 0)
		_data_pos = 0;
	return (int)real_len;
}


int CStreamBuffer::Write(const void *data_, size_t data_len_)
{
	if(!data_ || !data_len_)
		return -1;
	if(_buffer_len == _data_len)
		return -2; //full
	if(_buffer_len - _data_len < data_len_)	
		return -3;
	if(WritePossible(data_, data_len_) != (int)data_len_)
		return -4; //like bug?
	return 0;
}

int CStreamBuffer::Read(void *buffer_, size_t buffer_len_)
{
	if(!buffer_ || !buffer_len_)
		return -1;
	if(0 == _data_len)
		return -2; //empty
	if(buffer_len_ > _data_len)
		return -3;
	if(ReadPossible(buffer_, buffer_len_) != (int)buffer_len_)
		return -4;
	return 0;
}

int CStreamBuffer::Consume(size_t data_len_)
{
	if(!data_len_)
		return -1;
	if(data_len_ > _data_len)
		return -2;
	if(ConsumePossible(data_len_) != (int)data_len_)
		return -3;
	return 0;
}

bool CStreamBuffer::Empty()
{
	return 0 == _data_len;
}

bool CStreamBuffer::Full()
{
	return _data_len == _buffer_len;
}


char* CStreamBuffer::ValidData()
{
	if(Empty())
		return NULL;
	return _buffer_ptr + _data_pos;
}

size_t CStreamBuffer::ValidLength()
{
	return _data_len;
}

CStreamBuffer::ChunkPair CStreamBuffer::GetDataChunk()
{
	CStreamBuffer::ChunkPair chunk_pair;
	if(0 == _data_len) return chunk_pair;
	size_t np = (_data_pos + _data_len) % _buffer_len;
	if(_data_pos < np)
	{
		chunk_pair.first._data_ptr = _buffer_ptr + _data_pos;
		chunk_pair.first._data_len = _data_len;
		return chunk_pair;
	}
	else
	{
		chunk_pair.first._data_ptr = _buffer_ptr + _data_pos;
		chunk_pair.first._data_len = _buffer_len - _data_pos;

		if(_data_len > chunk_pair.first._data_len)
		{
			chunk_pair.second._data_ptr = _buffer_ptr;
			chunk_pair.second._data_len = np;
		}
	}
	return chunk_pair;
}

char* CStreamBuffer::FreeData()
{
	if(Full())
		return NULL;
	size_t np = (_data_pos + _data_len) % _buffer_len;
	return _buffer_ptr + np;
}

size_t CStreamBuffer::FreeLength()
{
	return _buffer_len - _data_len;
}

int CStreamBuffer::ProducePossible(size_t data_len_)
{
	if(!data_len_)
		return -1;
	size_t real_len = data_len_;
	if(data_len_ > _buffer_len - _data_len)
		real_len = _buffer_len - _data_len;
	_data_len += real_len;
	return (int)real_len;
}

int CStreamBuffer::Produce(size_t data_len_)
{
	if(!data_len_)
		return -1;
	if(data_len_ > _buffer_len - _data_len)
		return -2;
	if(ProducePossible(data_len_) != (int)data_len_)
		return -3;
	return 0;
}

int CStreamBuffer::MoveFront()
{
	if(0 == _data_pos)
		return 0;
	if(!Empty())
	{
		size_t np = (_data_pos + _data_len) % _buffer_len;
		if(_data_pos < np )
		{
			memmove(_buffer_ptr, _buffer_ptr + _data_pos, _data_len);
		}
		else
		{
			//it 
			if(_buffer_len - _data_pos >= _data_len)
			{
				memmove(_buffer_ptr, _buffer_ptr + _data_pos, _data_len);
			}
			else
			{
				return -1; // if wanna implement this case, it need another large enough buffer to move data, it seem to cost performance too many. so I did not implement it.
			}
		}
	}
	_data_pos = 0;
	return 0;
}



