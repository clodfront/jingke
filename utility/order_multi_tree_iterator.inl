// Project:	jingke
// File:    order_multi_tree_iterator.inl
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
#ifndef OrderMultiTreeIterator_h__
#define OrderMultiTreeIterator_h__

#include <iterator>
#include <utility>

namespace jingke {

template <typename T>
struct __children_iterator : public std::iterator<std::bidirectional_iterator_tag, T>
{
	typedef T node_t;
	typedef T* node_ptr_t;
	__children_iterator() :_elem(NULL), _parent_elem(NULL){}
	node_ptr_t operator -> ()
	{
		return _elem;
	}
	node_t& operator * ()
	{
		return *_elem;
	}
	bool operator != (const __children_iterator& other)
	{
		return this->_elem != other._elem || this->_parent_elem != other._parent_elem;
	}
	bool operator == (const __children_iterator& other) const
	{
		return !(*this != other);
	}
	__children_iterator operator ++ (int)
	{
		__children_iterator snapshot = *this;
		++*this;
		return snapshot;
	}
	__children_iterator& operator ++ ()
	{
		node_ptr_t next = _elem->get_next();
		_elem = next ? next : _parent_elem;
		return *this;
	}
	__children_iterator operator -- (int)
	{
		__children_iterator snapshot = *this;
		--*this;
		return snapshot;
	}
	__children_iterator& operator -- ()
	{
		
		if (_parent_elem == _elem)
		{
			if (_parent_elem->has_children())
			{
				_elem = _parent_elem->last_children();
				return *this;
			}
		}
		else
		{
			node_ptr_t prev = _elem->get_prev();
			_elem = prev ? prev : _parent_elem;
		}
		return *this;
	}

	node_ptr_t _elem;
	node_ptr_t _parent_elem;
};

template <typename T>
struct __tree_iterator : public std::iterator<std::bidirectional_iterator_tag, T>
{
	typedef T node_t;
	typedef T* node_ptr_t;
	__tree_iterator() :_elem(NULL), _root_elem(NULL){}
	node_ptr_t operator -> () const
	{
		return _elem;
	}
	node_t& operator * ()
	{
		return *_elem;
	}
	bool operator != (const __tree_iterator& other) const
	{
		return this->_elem != other._elem || this->_root_elem != other._root_elem;
	}
	bool operator == (const __tree_iterator& other) const
	{
		return !(*this != other);
	}
	__tree_iterator operator ++ (int)
	{
		__tree_iterator snapshot = *this;
		++*this;
		return snapshot;
	}
	__tree_iterator& operator ++ ()
	{
		do
		{
			if (_elem == _root_elem)
			{
				_elem = NULL;
				break;
			}
			node_ptr_t next = _elem->get_next();
			if (!next)
			{
				_elem = _elem->get_parent();
				break;
			}
			else
			{
				_elem = next->get_left_deepest();
				break;
			}
		} while (0);
		return *this;
	}
	__tree_iterator operator -- (int)
	{
		__tree_iterator snapshot = *this;
		--*this;
		return snapshot;
	}
	__tree_iterator& operator -- ()
	{
		do
		{
			if (_elem == NULL){
				_elem = _root_elem;
				break;
			}
			if (_elem->has_children())
			{
				_elem = _elem->last_children();
				break;
			}
			else
			{
				node_ptr_t prev = _elem->get_prev();
				if (prev)
				{
					_elem = prev;
					break;
				}
				else
				{
					if(!(_elem = _elem->get_first_prve_of_foreparent(_root_elem)))
						break;
				}
			}
		} while (0);
		return *this;
	}
	node_ptr_t _elem;
	node_ptr_t _root_elem;
};

} ///namespace jingke

#endif // OrderMultiTreeIterator_h__