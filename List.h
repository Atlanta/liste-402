//
//  list.h
//  Projet INFO0402
//
//  Created by Julien Hubert on 24/03/2016.
//  Copyright ? 2016 Julien Hubert. All rights reserved.
//
//#pragma once
#ifndef __LIST_H__
#define __LIST_H__

#ifdef _DEBUG
#define DEBUG(x) x
#else
#define DEBUG(x)
#endif

#include <assert.h>
#include <iostream>
#include <limits>
#include <initializer_list>
#include <iterator>
#include <vector>
#include <numeric>

using namespace std;

template <class T>
class List {
public:
	/* Typedefs men */
	typedef T value_type;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef std::size_t size_type;

	/* Structures / Classes */
	struct Node {
		Node* previous;
		Node* next;

		Node() : previous(this), next(this) {};
		Node(Node* previous) : previous(previous), next(this) {};
		virtual ~Node() {};
	};
	struct DataNode : public Node {
		value_type data;

		DataNode() : Node(), data(value_type()) {};
		DataNode(const_reference data) : Node(), data(data) {};
		virtual ~DataNode() {};
	};

	class ListIterator : public std::iterator<std::bidirectional_iterator_tag, value_type>
	{
		friend class List;
	private:
		Node* node;
	public:

		ListIterator() : node(0) {};
		ListIterator(Node* ptr) : node(ptr) {};
		ListIterator(const ListIterator& other) : node(other.node) {};
		virtual ~ListIterator() {};
		ListIterator& operator++() { node = node->next; return *this; };
		ListIterator operator++(int) {
			ListIterator tmp(*this);
			operator++();
			return tmp;
		};
		ListIterator& operator--() { node = node->previous; return *this; };
		ListIterator operator--(int) {
			ListIterator tmp(*this);
			operator--();
			return tmp;
		};
		bool operator==(const ListIterator& other) { return node == other.node; };
		bool operator!=(const ListIterator& other) { return node != other.node; };
		ListIterator& operator=(const ListIterator& other) {
			if (this != &other) this->node = other.node;
			return *this;
		}
		value_type& operator*() { return reinterpret_cast<DataNode*>(node)->data; }
	};
	class ConstListIterator : public std::iterator<std::bidirectional_iterator_tag, const value_type> {
		friend class List;
	private:
		Node* node;
	public:
		ConstListIterator() : node() {};
		ConstListIterator(Node* ptr) : node(ptr) {};
		ConstListIterator(const ListIterator& lit) : node(lit.node) {};
		ConstListIterator(const ConstListIterator& lit) : node(lit.node) {};
		virtual ~ConstListIterator() {};
		ConstListIterator& operator++() {
			node = node->next
				; return *this;
		};
		ConstListIterator operator++(int) {
			ConstListIterator tmp(*this);
			operator++();
			return tmp;
		};
		ConstListIterator& operator--() { node = node->previous; return *this; };
		ConstListIterator operator--(int) {
			ConstListIterator tmp(*this);
			operator--();
			return tmp;
		};
		bool operator==(const ConstListIterator& rhs) { return node == rhs.node; };
		bool operator!=(const ConstListIterator& rhs) { return node != rhs.node; };
		ConstListIterator& operator=(const ConstListIterator& other) {
			if (this != &other) {
				this->node = other.node;
			}
			return *this;
		}
		reference operator*() { return reinterpret_cast<DataNode*>(node)->data; }
	};

	/* More typedefs... */
	typedef ListIterator iterator;
	typedef ConstListIterator const_iterator;
	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
	typedef typename std::iterator_traits<iterator>::difference_type difference_type;

protected:
	/* Class members */
	Node* root; // at the end
	size_type elementCount;

public:
	// Constructors
	explicit List(); // Default
	explicit List(size_type n);
	List(size_type n, const value_type& val = value_type());
	template <class InputIterator> List(InputIterator first, InputIterator last);
	List(const List& x);
	List(List&& x); // Move
	List(std::initializer_list<value_type> il);

	// Destructors
	virtual ~List();

	// Overloading
	List& operator=(const List& x);
	List& operator=(List&& x);
	List& operator=(std::initializer_list<value_type> il);

	// Iterators
	iterator begin();
	const_iterator begin() const;
	reverse_iterator rbegin() noexcept;
	//const_reverse_iterator rbegin() const noexcept;
	iterator end();
	//const_iterator end() const;
	reverse_iterator rend();
	//const_reverse_iterator rend() const;
	const_iterator cbegin() const noexcept;
	const_iterator cend() const noexcept;
	const_reverse_iterator crbegin() const noexcept;
	const_reverse_iterator crend() const noexcept;

	// Capacity
	bool empty() const;
	size_type size() const;
	size_type max_size() const;

	// Element access
	reference front();
	const_reference front() const;
	reference back();
	const_reference back() const;

	// Modifiers
	template <class InputIterator> void assign(InputIterator first, InputIterator last);
	void assign(size_type n, const value_type& val);
	void assign(std::initializer_list<value_type> il);
	void push_front(const value_type& val);
	void push_front(value_type&& val);
	void pop_front();
	void push_back(const value_type& val);
	void push_back(value_type&& val);
	void pop_back();
	template <class... Args>
	iterator emplace(const_iterator position, Args&&... args);
	iterator insert(const_iterator position, const value_type& val);
	iterator insert(const_iterator position, size_type n, const value_type& val);
	template<class InputIterator>
	iterator insert(const_iterator position, InputIterator first, InputIterator last);
	iterator insert(const_iterator position, value_type&& val);
	iterator insert(const_iterator position, std::initializer_list<value_type> il);
	iterator erase(const_iterator position);
	iterator erase(const_iterator first, const_iterator last);
	void swap(List& x);
	void resize(size_type n);
	void resize(size_type n, const value_type& val);
	void clear() noexcept;
};

#endif
