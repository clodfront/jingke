// Project:	jingke
// File:    string_op.h
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

#ifndef _STRING_OP_H_
#define _STRING_OP_H_

#include <stdlib.h>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include "type_traits.h"

namespace jingke {

extern std::string lower(const std::string &str_);
extern std::string upper(const std::string &str_);
extern bool isdigit_all(const std::string &str_);

extern std::string trim(const std::string &str_, const std::string &unnecessary_str_ = "\r\n\t ", bool any_unnecessary_char_ = true);
extern std::string trim_head(const std::string &str_, const std::string &unnecessary_str_ = "\r\n\t ", bool any_unnecessary_char_ = true);
extern std::string trim_tail(const std::string &str_, const std::string &unnecessary_str_ = "\r\n\t ", bool any_unnecessary_char_ = true);
extern std::map<std::string,std::string> parse_param(const std::string& str_, const std::string& delimiters_, const std::string& indelimiters_);


/// str_to_arithmetic

template<typename T>
struct str_to_arithmetic
{
	T operator()(const std::string &str_)
	{
		if(str_.empty())
			return 0;
		std::istringstream isstream(str_);
		T t;
		isstream >> t;
		return t;
	}
};

template<>
struct str_to_arithmetic<char>
{
	char operator()(const std::string &str_)
	{
		if(!str_.empty())
		{
			return (char)atoi(str_.c_str());
		}
		return 0;
	}
};

template<>
struct str_to_arithmetic<unsigned char>
{
	char operator()(const std::string &str_)
	{
		if(!str_.empty())
		{
			return (unsigned char)strtoul(str_.c_str(), NULL, 10);
		}
		return 0;
	}
};


template<>
struct str_to_arithmetic<short>
{
	short operator()(const std::string &str_)
	{
		if(!str_.empty())
		{
			return (short)atoi(str_.c_str());
		}
		return 0;
	}
};

template<>
struct str_to_arithmetic<unsigned short>
{
	unsigned short operator()(const std::string &str_)
	{
		if(!str_.empty())
		{
			return (unsigned short)strtoul(str_.c_str(), NULL, 10);
		}
		return 0;
	}
};

template<>
struct str_to_arithmetic<int>
{
	int operator()(const std::string &str_)
	{
		if(!str_.empty())
		{
			return atoi(str_.c_str());
		}
		return 0;
	}
};

template<>
struct str_to_arithmetic<unsigned int>
{
	unsigned int operator()(const std::string &str_)
	{
		if(!str_.empty())
		{
			return (unsigned int)strtoul(str_.c_str(), NULL, 10);
		}
		return 0;
	}
};

template<>
struct str_to_arithmetic<long>
{
	long operator()(const std::string &str_)
	{
		if(!str_.empty())
		{
			return atol(str_.c_str());
		}
		return 0;
	}
};

template<>
struct str_to_arithmetic<unsigned long>
{
	unsigned long operator()(const std::string &str_)
	{
		if(!str_.empty())
		{
			return strtoul(str_.c_str(), NULL, 10);
		}
		return 0;
	}
};

template<>
struct str_to_arithmetic<long long>
{
	long long operator()(const std::string &str_)
	{
		if(!str_.empty())
		{
			return atoll(str_.c_str());
		}
		return 0;
	}
};


template<>
struct str_to_arithmetic<unsigned long long>
{
	unsigned long operator()(const std::string &str_)
	{
		if(!str_.empty())
		{
			return strtoull(str_.c_str(), NULL, 10);
		}
		return 0;
	}
};

template<>
struct str_to_arithmetic<float>
{
	float operator()(const std::string &str_)
	{
		if(!str_.empty())
		{
			return (float)(atof(str_.c_str()));
		}
		return 0;
	}
};

template<>
struct str_to_arithmetic<double>
{
	double operator()(const std::string &str_)
	{
		if(!str_.empty())
		{
			return atof(str_.c_str());
		}
		return 0;
	}
};


/// str_to_other

template<typename T>
struct str_to_other
{
	T operator()(const std::string &str_)
	{
		std::istringstream isstream(str_);
		T t;
		isstream >> t;
		return t;
	}
};

template<>
struct str_to_other<std::string>
{
	std::string operator()(const std::string &str_)
	{
		return str_;
	}
};


/// str_to_type

template<typename T>
T str_to_type(const std::string &str_)
{
	typedef typename TT::ifthenelse<(bool)TT::is_arithmetic<T>::value, str_to_arithmetic<T>, str_to_other<T> >::type t;
	return t()(str_);
}

template<typename T>
T str_to_type(const std::string &str_, const std::string &default_str_)
{
	std::string s;
	if(!str_.empty())
	{
		s = str_;
	}
	else
	{
		s = default_str_;
	}
	return str_to_type<T>(s);
}


/// type_to_str

template<typename T>
std::string type_to_str(const T &t)
{
	std::ostringstream str_buffer;
	str_buffer << t;
	return str_buffer.str();
}


/// tokenize

template<typename T>
std::vector<T> tokenize(const std::string& str_, const std::string& delimiters_, bool with_empty_  = true)
{
	std::vector<T> tokens;
	std::string::size_type last_pos = str_.find_first_not_of(delimiters_, 0);
	std::string::size_type pos = str_.find_first_of(delimiters_, last_pos);
	std::string cell;
	while (std::string::npos != pos || std::string::npos != last_pos)
	{
		cell = str_.substr(last_pos, pos - last_pos);
		if(cell.empty())
		{
			if(with_empty_)
				tokens.push_back(str_to_type<T>(cell));
		}
		else
		{
			tokens.push_back(str_to_type<T>(cell));
		}
			
		last_pos = str_.find_first_not_of(delimiters_, pos);
		pos = str_.find_first_of(delimiters_, last_pos);
	}
	return tokens;
}


} ///namespace jingke


#endif  ///_STRING_OP_H_



