// Project:	jingke
// File:    singleton.h
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

#ifndef _SINGLETON_H_
#define _SINGLETON_H_


namespace jingke {

#ifndef SINGLETON_DEFAULT_ID
#define SINGLETON_DEFAULT_ID 0
#endif

template <typename T, int N=SINGLETON_DEFAULT_ID>
struct singleton
{
public:
	typedef T object_type;

	static object_type & instance()
	{
		static object_type obj;
		create_object.make_sure_malloc_befor_main();
		return obj;
	}

private:
	struct object_creator
	{
		object_creator() { singleton<T,N>::instance(); }
		inline void make_sure_malloc_befor_main() const { }
	};
	
	singleton();

	static object_creator create_object;
};
template <typename T, int N>
typename singleton<T, N>::object_creator singleton<T, N>::create_object;

} ///namespace jingke

#endif  ///_SINGLETON_H_


