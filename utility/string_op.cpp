// Project:	jingke
// File:    string_op.cpp
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
#include "string_op.h"
#include <algorithm>

using namespace jingke;

std::string lower(const std::string &str_)
{
	std::string str_ret = str_;
	std::transform(str_.begin(), str_.end(), str_ret.begin(), ::tolower);
	return str_ret;
}

std::string upper(const std::string &str_)
{
	std::string str_ret = str_;
	std::transform(str_.begin(), str_.end(), str_ret.begin(), ::toupper);
	return str_ret;
}

bool isdigit_all(const std::string &str_)
{
	if(str_.empty())
		return false;
	std::string::const_iterator iter = str_.begin();
	while(iter != str_.end())
	{
		if (!::isdigit(*iter))
		{
			return false;
		}
		++iter;
	}
	return true;
}

std::string trim(const std::string &str_, const std::string &unnecessary_str_ /* = "\r\n\t "*/, bool any_unnecessary_char_ /* = true*/)
{
	if(str_.empty())
		return str_;
	return trim_tail(trim_head(str_, unnecessary_str_, any_unnecessary_char_), unnecessary_str_, any_unnecessary_char_);
}

std::string trim_head(const std::string &str_, const std::string &unnecessary_str_ /* = "\r\n\t "*/, bool any_unnecessary_char_  /* = true*/)
{
	if(str_.empty())
		return str_;

	if(!any_unnecessary_char_)
	{
		if(str_.length() < unnecessary_str_.length())
			return str_;
		if(str_.compare(0, unnecessary_str_.length(), unnecessary_str_) == 0)
			return str_.substr(unnecessary_str_.length());
		return str_;
	}

	std::string::size_type pos = 0;
	while(pos < str_.length())
	{
		if(unnecessary_str_.find_first_of(str_[pos]) == std::string::npos)
			break;
		pos++;
	}
	
	return pos == 0 ? str_ : str_.substr(pos);
}

std::string trim_tail(const std::string &str_, const std::string &unnecessary_str_ /* = "\r\n\t "*/, bool any_unnecessary_char_  /* = true*/)
{
	if(str_.empty())
		return str_;

	if(!any_unnecessary_char_)
	{
		if(str_.length() < unnecessary_str_.length())
			return str_;
		if(str_.compare(str_.length() - unnecessary_str_.length(), unnecessary_str_.length(), unnecessary_str_) == 0)
			return str_.substr(0, str_.length() - unnecessary_str_.length());
		return str_;
	}

	std::string::size_type pos = str_.length();
	while(pos != 0)
	{
		if(unnecessary_str_.find_first_of(str_[pos-1]) == std::string::npos)
			break;
		pos--;
	}

	return pos == str_.length() ? str_ : str_.substr(0, pos);
}

//从类似"a=123&b=123&c=9990&d=yyy"的字符串中提取单元 parse_param(str,"&","=");
std::map<std::string,std::string> parse_param(const std::string& str_, const std::string& delimiters_, const std::string& indelimiters_)
{
	std::map<std::string,std::string> ret_param_map;
	std::vector<std::string> tokens = tokenize<std::string>(str_, delimiters_);
	for(size_t s=0; s<tokens.size(); ++s)
	{
		std::vector<std::string> cell = tokenize<std::string>(tokens[s], indelimiters_);
		if (cell.size()>=2)
		{
			ret_param_map[cell[0]] = cell[1];
		}
	}
	return ret_param_map;
}


