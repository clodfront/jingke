// Project:	jingke
// File:    omygod.h
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

#ifndef _OMYGOD_H_
#define _OMYGOD_H_

#include <stdlib.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <assert.h>
#include <string.h>
#include "type_traits.h"

namespace jingke {

#ifndef likely
#define likely(x) __builtin_expect(!!(x), 1)
#endif

#ifndef unlikely
#define unlikely(x) __builtin_expect(!!(x), 0)
#endif

/// assert_static, compile time assert
template<bool x> struct _st_assert_failure;
template<> struct _st_assert_failure<true> {enum {val=1}; };
template<int x> struct _st_static_assert_test{};
#define _macro_join(l,r) l##r
#define assert_static(b)\
typedef _st_static_assert_test< \
	sizeof(_st_assert_failure<(bool)(b)>) > \
	_macro_join(_static_assert_typedef_,__COUNTER__)

/// safe delete & free
template<typename T> 
inline void safe_delete(T *& x)
{
    typedef char type_must_be_complete[ sizeof(T)? 1: -1 ];
    (void) sizeof(type_must_be_complete);
	if(x){delete x;x = NULL;}
}

template<typename T> 
inline void safe_array_delete(T *& x)
{
    typedef char type_must_be_complete[ sizeof(T)? 1: -1 ];
    (void) sizeof(type_must_be_complete);
	if(x){delete [] x;x = NULL;}
}

template<typename T> 
inline void safe_free(T *& x)
{
    if(x){free(x);x = NULL;}
}

/// auto delete & free
template<typename T>
struct auto_delete
{
public:
    auto_delete(T* x):_p(x){}
    ~auto_delete(){if(_p){delete _p;}}
private:
    T* _p;
};

template<typename T>
struct auto_array_delete
{
public:
    auto_array_delete(T* x):_p(x){}
    ~auto_array_delete(){if(_p){delete [] _p;}}
private:
    T* _p;
};

struct auto_free
{
public:
    auto_free(void *x):_p(x){};
    ~auto_free() { if(_p){free(_p);}}
private:
    void *_p;
};

/// get array element capacity
template<typename T, size_t N>
inline size_t array_len( T(&)[N] ) 
{ 
	return N; 
}

/// do be zero if T is arithmetic type
template<typename T, bool is_arithmetic>
struct bezero
{
public:
	void operator()(T* p) {}
};

template<typename T>
struct bezero<T, true>
{
public:
	void operator()(T* p) { if(p) *p=0; }
};

template<typename T>
inline void set_arithmetic_type_zero(T* p)
{
	bezero<T, (bool)TT::is_arithmetic<T>::value >  __bezero;
	__bezero(p);
}

/// get pointer aligned size, at least 8 byte aligned
inline size_t ptr_aligned_size(size_t original_size)
{
	const size_t align = (sizeof(void*) > 8) ? sizeof(void*) : 8;
	assert((align & (align-1)) == 0);	// Pointer size should be a power of 2
	const size_t remainder = original_size % align;
	return remainder ? original_size + align - remainder : original_size;
};


/// ntoh64 &  hton64
#ifndef SWAP64
	#define SWAP64(x) \
	    ( (((x) & (uint64_t)0xff00000000000000LL) >> 56)  \
	    | (((x) & (uint64_t)0x00ff000000000000LL) >> 40)  \
	    | (((x) & (uint64_t)0x0000ff0000000000LL) >> 24)  \
	    | (((x) & (uint64_t)0x000000ff00000000LL) >>  8)  \
	    | (((x) & (uint64_t)0x00000000ff000000LL) <<  8)  \
	    | (((x) & (uint64_t)0x0000000000ff0000LL) << 24)  \
	    | (((x) & (uint64_t)0x000000000000ff00LL) << 40)  \
	    | (((x) & (uint64_t)0x00000000000000ffLL) << 56))
#endif ///SWAP64

#if __BYTE_ORDER == __LITTLE_ENDIAN
	#ifndef ntoh64
		#define ntoh64(x) 	SWAP64(x)
	#endif ///ntoh64
	#ifndef hton64
		#define hton64(x)   SWAP64(x)
	#endif ///hton64
#elif  __BYTE_ORDER == __BIG_ENDIAN
	#ifndef ntoh64
		#define ntoh64(x) 	(x)
	#endif ///ntoh64
	#ifndef hton64
		#define hton64(x)   (x)
	#endif ///hton64
#endif

} ///namespace jingke

#endif  ///_OMYGOD_H_




