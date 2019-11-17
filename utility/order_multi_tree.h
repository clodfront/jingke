// Project:	jingke
// File:    order_multi_tree.h
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

#ifndef OrderMultiTree_h__
#define OrderMultiTree_h__

#include "order_multi_tree_iterator.inl"

namespace jingke {

template<typename T>
class SOrderMultiTreeNode
{
public:
	typedef T subclass_t;
	typedef subclass_t* subclass_ptr_t;
	typedef subclass_t node_t;
	typedef subclass_ptr_t node_ptr_t;
	typedef struct __list_t{
		node_ptr_t _elem;
		struct __list_t *next, *prev;
	}list_t;
	typedef list_t list_node_t;
	typedef list_node_t* list_node_ptr_t;
	typedef __children_iterator<node_t> children_iterator;
	typedef std::reverse_iterator<children_iterator> r_children_iterator;
	typedef __tree_iterator<node_t> tree_iterator;
	typedef std::reverse_iterator<tree_iterator> r_tree_iterator;
public:
	children_iterator children_begin()
	{
		if (!this->has_children())
		{
			return children_end();
		}
		children_iterator it;
		it._elem = _childrens_list.next->_elem;
		it._parent_elem = subclassed_self();
		return it;
	}
	children_iterator children_end()
	{
		children_iterator it;
		it._elem = _childrens_list._elem;
		it._parent_elem = subclassed_self();
		return it;
	}
	tree_iterator tree_begin()
	{
		tree_iterator it;
		it._root_elem = subclassed_self();
		it._elem = subclassed_self()->get_left_deepest();
		return it;
	}
	tree_iterator tree_end()
	{
		tree_iterator it;
		it._elem = NULL;
		it._root_elem = subclassed_self();
		return it;
	}
	r_tree_iterator r_tree_begin()
	{
		return r_tree_iterator(tree_end());
	}
	r_tree_iterator r_tree_end()
	{
		return r_tree_iterator(tree_begin());
	}
public:
	SOrderMultiTreeNode()
	{
		this->reset();
	}
	void reset()
	{
		_parent = NULL;

		_node_in_parent._elem = subclassed_self();
		_node_in_parent.next = &_node_in_parent;
		_node_in_parent.prev = &_node_in_parent;

		_childrens_list._elem = subclassed_self();
		_childrens_list.next = &_childrens_list;
		_childrens_list.prev = &_childrens_list;
	}
	bool is_tree_root()
	{
		return !get_parent();
	}
	node_ptr_t get_tree_root()
	{
		return is_tree_root() ? subclassed_self() : _parent->get_tree_root();
	}
	node_ptr_t get_parent()
	{
		return _parent;
	}
	bool has_children()
	{
		return !__list_empty(&_childrens_list);
	}
	node_ptr_t first_children()
	{
		return __list_get_first(&_childrens_list) ? __list_get_first(&_childrens_list)->_elem : NULL;
	}
	node_ptr_t last_children()
	{
		return __list_get_last(&_childrens_list) ? __list_get_last(&_childrens_list)->_elem : NULL;
	}
	node_ptr_t add_children(node_ptr_t node)
	{
		if (!node || !node->is_tree_root() || get_tree_root()==node)
			return NULL;
		node->_parent = subclassed_self();
		__list_add_tail(&node->_node_in_parent, &_childrens_list);
		return node;
	}
	void del_children(node_ptr_t node)
	{
		if (node && node->get_parent() == this)
		{
			node->break_away_from_tree();
		}
	}
	void clear_children()
	{
		for (list_node_ptr_t p = _childrens_list.next; p != &_childrens_list; p = p->next)
			p->_elem->_parent = NULL;
		__clear_list(&_childrens_list);
	}
	node_ptr_t get_prev()
	{
		if (is_tree_root()) return NULL;
		if (__list_is_first(&_node_in_parent, &_parent->_childrens_list))
			return NULL;
		return _node_in_parent.prev->_elem;
	}
	node_ptr_t get_next()
	{
		if (is_tree_root()) return NULL;
		if (__list_is_last(&_node_in_parent, &_parent->_childrens_list))
			return NULL;
		return _node_in_parent.next->_elem;
	}
	void break_away_from_tree()
	{
		if (!is_tree_root())
		{
			this->_parent = NULL;
			__list_del(&this->_node_in_parent);
		}
	}
	
public:
	node_ptr_t subclassed_self()
	{
		return (node_ptr_t)this;
	}
	node_ptr_t get_left_deepest()
	{
		node_ptr_t p = subclassed_self();
		while (p->has_children())
		{
			p = p->first_children();
		}
		return p;
	}
	node_ptr_t get_first_prve_of_foreparent(node_ptr_t root_node)
	{
		node_ptr_t p = this->get_parent();
		if (!p)
			return NULL;
		while (p->get_prev() == NULL)
		{
			if (p == root_node)
				return NULL;
			p = p->get_parent();
			if (!p)
				return NULL;
		}
		return p->get_prev();
	}
private:
	static void __list_add(list_node_t *new_entry, list_node_t *prev, list_node_t *next)
	{
		next->prev = new_entry;
		new_entry->next = next;
		new_entry->prev = prev;
		prev->next = new_entry;
	}
	static void __list_add_tail(list_node_t *new_entry, list_t *head)
	{
		__list_add(new_entry, head->prev, head);
	}
	static void __list_add_front(list_node_t *new_entry, list_t *head)
	{
		__list_add(new_entry, head, head->next);
	}
	static void __list_del(list_node_t *entry)
	{
		entry->next->prev = entry->prev;
		entry->prev->next = entry->next;

		entry->next = entry;
		entry->prev = entry;
	}
	static void __clear_list(list_t *head)
	{
		list_node_t* entry = head->next;
		while (entry != head)
		{
			list_node_t* next = entry->next;
			__list_del(entry);
			entry = next;
		}
	}
	static bool __list_empty(const list_t *head)
	{
		return head->next == head;
	}
	static bool __list_is_first(const list_node_t *entry, const list_t *head)
	{
		return entry->prev == head;
	}
	static bool __list_is_last(const list_node_t *entry, const list_t *head)
	{
		return entry->next == head;
	}
	static list_node_t* __list_get_first(list_t *head)
	{
		return head->next != head ? head->next : NULL;
	}
	static list_node_t* __list_get_last(list_t *head)
	{
		return head->next != head ? head->prev : NULL;
	}
private:
	node_ptr_t	_parent;
	list_node_t _node_in_parent;
	list_t		_childrens_list;
};

} ///namespace jingke

#endif // OrderMultiTree_h__
