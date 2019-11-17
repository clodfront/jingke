// Project:	jingke
// File:    type_traits.h
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

#ifndef _TYPE_TRAITS_H_
#define _TYPE_TRAITS_H_

namespace jingke {

namespace TT
{
	/// null type
	struct _null_type;
	
	/// type list
	template<typename Head, typename Tail>
	struct _type_list
	{
		typedef Head H;
		typedef Tail T;
	};

	#define _type_list_1(T1) _type_list<T1, TT::_null_type>
	#define _type_list_2(T1, T2) _type_list<T1, TT::_type_list_1(T2) >
	#define _type_list_3(T1, T2, T3) _type_list<T1, TT::_type_list_2(T2, T3) >
	#define _type_list_4(T1, T2, T3, T4) _type_list<T1, TT::_type_list_3(T2, T3, T4) >
	#define _type_list_5(T1, T2, T3, T4, T5) _type_list<T1, TT::_type_list_4(T2, T3, T4, T5) >
	#define _type_list_6(T1, T2, T3, T4, T5, T6) _type_list<T1, TT::_type_list_5(T2, T3, T4, T5, T6) >
	#define _type_list_7(T1, T2, T3, T4, T5, T6, T7) _type_list<T1, TT::_type_list_6(T2, T3, T4, T5, T6, T7) >
	#define _type_list_8(T1, T2, T3, T4, T5, T6, T7, T8) _type_list<T1, TT::_type_list_7(T2, T3, T4, T5, T6, T7, T8) >
	#define _type_list_9(T1, T2, T3, T4, T5, T6, T7, T8, T9) _type_list<T1, TT::_type_list_8(T2, T3, T4, T5, T6, T7, T8, T9) >
	#define _type_list_10(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10) _type_list<T1, TT::_type_list_9(T2, T3, T4, T5, T6, T7, T8, T9, T10) >
	#define _type_list_11(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11) _type_list<T1, TT::_type_list_10(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11) >
	#define _type_list_12(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12) _type_list<T1, TT::_type_list_11(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12) >
	#define _type_list_13(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13) _type_list<T1, TT::_type_list_12(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13) >
	#define _type_list_14(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14) _type_list<T1, TT::_type_list_13(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14) >
	#define _type_list_15(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15) _type_list<T1, TT::_type_list_14(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15) >
	#define _type_list_16(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16) _type_list<T1, TT::_type_list_15(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16) >
	#define _type_list_17(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17) _type_list<T1, TT::_type_list_16(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17) >
	#define _type_list_18(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18) _type_list<T1, TT::_type_list_17(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18) >
	#define _type_list_19(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19) _type_list<T1, TT::_type_list_18(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19) >
	#define _type_list_20(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20) _type_list<T1, TT::_type_list_19(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20) >

	/// has type

	template<typename L, class T> 
	struct _type_index;
	
	template<typename T> 
	struct _type_index<_null_type, T>
	{
		enum { value = -1 };
	};
	
	template<typename Tail, typename T> 
	struct _type_index<_type_list<T,Tail>, T>
	{
		enum { value = 0 };
	};
	
	template<typename Head, typename Tail, typename T> 
	struct _type_index<_type_list<Head, Tail>, T>
	{
	private:
		enum { tag = _type_index<Tail, T>::value };
	public:
		enum { value = tag != -1 ? tag+1 : tag };
	};
	
	template<typename L, typename T> 
	struct _has_type
	{
		enum { value = _type_index<L,T>::value != -1 };
	};


	/// erase type
	template<typename L, typename T> 
	struct _erase_type;
	
	template<typename T> struct _erase_type<_null_type, T>
	{
		typedef _null_type type;
	};
	
	template<typename T, typename Tail> struct _erase_type<_type_list<T, Tail>, T>
	{
		typedef typename _erase_type<Tail, T>::type type;
	};
	
	template<typename Head, typename Tail, typename T> struct _erase_type<_type_list<Head, Tail>, T>
	{
		typedef _type_list<Head, typename _erase_type<Tail, T>::type> type;
	};


	template<typename T1=_null_type,  typename T2=_null_type,  typename T3=_null_type,  typename T4=_null_type,  typename T5=_null_type,
			 	typename T6=_null_type,  typename T7=_null_type,  typename T8=_null_type,  typename T9=_null_type,  typename T10=_null_type,
			 		typename T11=_null_type, typename T12=_null_type, typename T13=_null_type, typename T14=_null_type, typename T15=_null_type,
			 			typename T16=_null_type, typename T17=_null_type, typename T18=_null_type, typename T19=_null_type, typename T20=_null_type>
	struct _type_list_construction
	{
		typedef typename _erase_type<_type_list_20(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10,T11, T12, T13, T14, T15, T16, T17, T18, T19, T20), _null_type>::type type;
	};


	template<typename T>
	struct _type_traits_imp
	{
		// pointer
		template<typename U>
		struct _pointer_traits
		{
			enum { value = false };
			typedef U type;
		};

		template<typename U>
		struct _pointer_traits<U*>
		{
			enum { value = true };
			typedef U type;
		};

		// reference
		template<typename U>
		struct _reference_traits
		{
			enum { value = false };
			typedef U type;
		};

		template<typename U>
		struct _reference_traits<U&>
		{
			enum { value = true };
			typedef U type;
		};

		template<typename U>
		struct _pointer_traits<U*&>
		{
			enum { value = true };
			typedef U type;
		};

		// const
		template<typename U>
		struct _const_traits
		{
			enum { value = false };
			typedef U type;
		};
		template<typename U>
		struct _const_traits<const U>
		{
			enum { value = true };
			typedef U type;
		};
		template<typename U>
		struct _const_traits<const U&>
		{
			enum { value = true };
			typedef U& type;
		};

	public:
		enum { _is_pointer = _pointer_traits<T>::value };
		enum { _is_reference = _reference_traits<T>::value };
		enum { _is_const = _const_traits<T>::value };

		typedef typename _pointer_traits<T>::type 		    _pointer_type;
		typedef typename _reference_traits<T>::type         _reference_type;
		typedef typename _const_traits<T>::type          	_un_const_type;

	public:
		typedef TT::_type_list_construction<unsigned char, unsigned short int, unsigned int, unsigned long int, unsigned long long int>::type _unsigned_ints_list;
		typedef TT::_type_list_construction<char, short int, int, long int, long long int>::type _signed_ints_list;
		typedef TT::_type_list_construction<float, double, long double>::type _floats_list;
		typedef TT::_type_list_construction<bool>::type _bool_list;

		enum { _is_unsigned 			= TT::_has_type<_unsigned_ints_list, T>::value };
		enum { _is_signed 				= TT::_has_type<_signed_ints_list, T>::value };
		enum { _is_bool					= TT::_has_type<_bool_list, T>::value };
		enum { _is_integral  			= _is_unsigned || _is_signed || _is_bool };
		enum { _is_floating_point   	= TT::_has_type<_floats_list, T>::value };
		enum { _is_arithmetic	 		= _is_integral || _is_floating_point };
	};


	/// interface
	template <typename T>
	struct is_unsigned
	{
		enum { value = TT::_type_traits_imp<T>::_is_unsigned };
	};

	template <typename T>
	struct is_signed
	{
		enum { value = TT::_type_traits_imp<T>::_is_signed };
	};

	template <typename T>
	struct is_integral
	{
		enum { value = TT::_type_traits_imp<T>::_is_integral };
	};

	template <typename T>
	struct is_floating_point
	{
		enum { value = TT::_type_traits_imp<T>::_is_floating_point };
	};

	template <typename T>
	struct is_arithmetic
	{
		enum { value = TT::_type_traits_imp<T>::_is_arithmetic };
	};

	template <typename T>
	struct is_fundamental
	{
		enum { value = TT::_type_traits_imp<T>::_is_fundamental };
	};

	template <typename T>
	struct is_simple_type
	{
		enum { value = TT::_type_traits_imp<T>::_is_simple_type };
	};

	template <typename T>
	struct is_std_empty_type
	{
		enum { value = TT::_type_traits_imp<T>::_is_std_empty_type };
	};

	/// if T is a struct or class */
	template<typename T>
	struct is_class 
	{
	private:
	    typedef char One;
	    typedef struct { char a[2]; } Two;
	    template<typename C> static One test(int C::*);
	    template<typename C> static Two test(...);
	public:
		enum { value = sizeof(test<T>(0)) == 1 ? 1 : 0 };
	};

	/// type selector like if ? then : else
	template<bool b,typename T1,typename T2>
	struct ifthenelse;

	template<typename T1,typename T2>
	struct ifthenelse<true,T1,T2>
	{
		typedef T1 type;
	};

	template<typename T1,typename T2>
	struct ifthenelse<false,T1,T2>
	{
		typedef T2 type;
	};

}/* namespace TT */


} ///namespace jingke

#endif  ///_TYPE_TRAITS_H_



