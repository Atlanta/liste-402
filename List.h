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

template <typename T>
struct sfinae_true : std::true_type {};

struct is_iterator_tester {
	template <typename T>
	static sfinae_true<typename std::iterator_traits<T>::iterator_category> test(int);

	template <typename>
	static std::false_type test(...);
};

template <typename T>
struct is_iterator : decltype(is_iterator_tester::test<T>(0)) {};

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

	enum class enabler {};

	template <typename Condition>
	using EnableIf = typename std::enable_if<Condition::value, enabler>::type;

	/* SFINAE */
	

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
	explicit List() : root(new Node()), elementCount(0) { DEBUG(cout << "Construct default" << endl;) }; // Default
	explicit List(size_type n) : root(new Node()), elementCount(0) {
		while (elementCount < n)
			push_back(value_type());
		DEBUG(cout << "Construct fill no value" << endl;)
	};
	List(size_type n, const value_type& val = value_type()) : root(new Node()), elementCount(0) {
		while (elementCount < n)
			push_back(val);
		DEBUG(cout << "Construct fill" << endl;)
	};
	template <class InputIterator>
	List(InputIterator first, InputIterator last, typename std::iterator_traits<InputIterator>::iterator_category* = nullptr) : root(new Node()), elementCount(0) {
		insert(begin(), first, last);
		DEBUG(cout << "Construct it" << endl;)
	};
	List(const List& x) : root(new Node()), elementCount(0) {
		for (const_iterator it = x.cbegin(); it != x.cend(); it++) {
			push_back(*it);
		}
		DEBUG(cout << "Construct copy" << endl;)
	};
	List(List&& x) : root(std::move(x.root)), elementCount(x.elementCount) {
		x.root = nullptr;
		x.elementCount = (size_type)0;
		DEBUG(cout << "Construct move" << endl;)
	};
	List(std::initializer_list<value_type> il) : root(new Node()), elementCount(0) {
		for (typename std::initializer_list<value_type>::iterator it = il.begin(); it != il.end(); it++) {
			push_back(*it);
		}
		DEBUG(cout << "Construct initialiser listw" << endl;)
	};

	// Destructors
	virtual ~List() {
		clear();
	};

	// Overloading
	List& operator=(const List& x) {
		if (this != &x) {
			clear();

			root = new Node();

			for (const_iterator it = x.cbegin(); it != x.cend(); it++) {
				push_back(*it);
			}

			elementCount = x.elementCount;
		}
		return *this;
	};
	List& operator=(List&& x) {
		if (this != &x) {
			DEBUG(cout << "Move assignement" << endl);

			clear();

			root = std::move(x.root);
			elementCount = x.elementCount;
			delete x.root;// = nullptr;
		}
		return *this;
	};
	List& operator=(std::initializer_list<value_type> il) {
		clear();

		root = new Node();

		for (typename std::initializer_list<value_type>::iterator it = il.begin(); it != il.end(); it++) {
			push_back(*it);
		}

		elementCount = il.size();

		return *this;
	};

	// Iterators
	iterator begin() {
		return iterator(root->next);
	};
	const_iterator begin() const {
		return const_iterator(root->next);
	};
	reverse_iterator rbegin() noexcept {
		return reverse_iterator(root->previous);
	};
	//const_reverse_iterator rbegin() const noexcept;
	iterator end() {
		return iterator(root);
	};
	//const_iterator end() const;
	reverse_iterator rend() {
		return reverse_iterator(this->begin());
	};
	//const_reverse_iterator rend() const;
	const_iterator cbegin() const noexcept {
		return const_iterator(root);
	};
	const_iterator cend() const noexcept {
		Node* ptr = root;
		for (size_type i = 0; i < elementCount; i++) ptr = ptr->next;
		return const_iterator(ptr);
	};
	const_reverse_iterator crbegin() const noexcept {
		return const_reverse_iterator();
	};
	const_reverse_iterator crend() const noexcept {
		return const_reverse_iterator();
	};

	// Capacity
	bool empty() const {
		return (elementCount == 0) ? true : false;
	};
	size_type size() const {
		return elementCount;
	};
	size_type max_size() const {
		return std::numeric_limits<List<T>::size_type>::max();
	};

	// Element access
	reference front() {
		assert(elementCount>0);
		return *this->begin();
	};
	const_reference front() const {
		assert(elementCount>0);
		return (const_reference)*this->cbegin();
	};
	reference back() {
		assert(elementCount>0);
		iterator it = end();
		it--;
		return (reference)*it;
	};
	const_reference back() const {
		assert(elementCount>0);
		const_iterator it = cend();
		it--;
		return (const_reference)*it;
	};

	// Modifiers
	template <class InputIterator> void assign(InputIterator first, InputIterator last) {
		clear();

		Node* ptr = nullptr;
		elementCount = (size_type)0;

		for (InputIterator it = first; it != last; it++) {
			if (it == first) {
				this->root = new Node(*it);
				ptr = this->root;
				elementCount++;
			}
			else {
				ptr->next = new Node(*it, ptr);
				ptr = ptr->next;
				elementCount++;
			}
		}
	};
	void assign(size_type n, const value_type& val) {
	};
	void assign(std::initializer_list<value_type> il) {
	};
	void push_front(const value_type& val) {
		insert(begin(), val);
	};
	void push_front(value_type&& val) {
		insert(begin(), std::move(val));
	};
	void pop_front() {
		if (empty()) return;
		else if (elementCount == 1) {
			delete this->root;
		}
		else {
			Node* tmp = this->root->next;
			delete this->root;
			this->root = tmp;
			this->root->previous = nullptr;
		}
		elementCount--;
	};
	void push_back(const value_type& val) {
		insert(end(), val);
	};
	void push_back(value_type&& val) {
		insert(end(), std::move(val));
	};
	void pop_back() {
		const_iterator it(end());
		it--;
		erase(it);
	};
	template <class... Args>
	iterator emplace(const_iterator position, Args&&... args) {
		return iterator(insert(position, value_type(std::forward<Args>(args)...)));
	};
	iterator insert(const_iterator position, const value_type& val) {
		DataNode* data_node = new DataNode(val); // pass data
		Node* current = position.node;
		data_node->next = current;
		data_node->previous = current->previous;
		current->previous->next = data_node;
		current->previous = data_node;
		elementCount++;
		return iterator(data_node);
	};
	iterator insert(const_iterator position, size_type n, const value_type& val) {
		for (size_type i = 0; i < n; i++) {
			position = insert(position, val);
		}
		return iterator(position.node);
	};
	template<class InputIterator>
	iterator insert(const_iterator position, InputIterator first, InputIterator last, typename std::iterator_traits<InputIterator>::iterator_category* = nullptr) {
		while (first != last) {
			insert(position, *first);
			first++;
		}
		return iterator(position.node);
	};
	iterator insert(const_iterator position, value_type&& val) {
		DataNode* data_node = new DataNode(std::move(val)); // pass data
		Node* current = position.node;
		data_node->next = current;
		data_node->previous = current->previous;
		current->previous->next = data_node;
		current->previous = data_node;
		elementCount++;
		return iterator(current);
	};
	iterator insert(const_iterator position, std::initializer_list<value_type> il) {
		iterator it;
		for (typename std::initializer_list<value_type>::iterator ii = il.begin(); ii < il.end(); ii++)
		{
			it = this->insert(position, *ii);
		}
		for (size_type i = il.size(); i > 0; i--) it--;
		elementCount++;
		return it;
	};
	iterator erase(const_iterator position) {
		assert(position != end());
		Node *current = position.node, *tmp = position.node->next;
		current->previous->next = current->next;
		current->next->previous = current->previous;
		delete current;
		elementCount--;
		return iterator(tmp);
	};
	iterator erase(const_iterator first, const_iterator last) {
		while (first != last)
			first = erase(first);
		return iterator(first.node);
	};
	void swap(List& x) {
		// TODO
	};
	void resize(size_type n) {
		if (n < elementCount) {
			while (elementCount > n)
				pop_back();
		}
		else {
			while (elementCount < n)
				push_back(value_type());
		}
	};
	void resize(size_type n, const value_type& val) {
		if (n < elementCount) {
			while (elementCount > n)
				pop_back();
		}
		else {
			while (elementCount < n)
				push_back(val);
		}
	};
	void clear() noexcept {
		while (elementCount > 0)
			pop_back();
	};

	/* Partie du fdp */
	void merge(List &x) {
		if (&x != this) {
			iterator middle = end();
			for (int i 0; i < elementCount / 2; i++) {
				middle--;
			}
			std::inplace_merge(begin(), middle, end());
		}
	};
	void merge(List &&x) {
		if (&x != this) {
			iterator middle = end();
			for (int i = 0; i < elementCount / 2; i++) {
				middle--;
			}
			std::inplace_merge(begin(), middle, end());
		}
	};
	template <class Compare>
	void merge(List &x, Compare comp) {
		if (&x != this) {
			iterator middle = end();
			for (int i = 0; i < elementCount / 2; i++) {
				middle--;
			}
			std::inplace_merge(begin(), middle, end(), comp);
		}
	};

	template <class Compare>
	void merge(List &&x, Compare comp) {
		if (&x != this) {
			iterator middle = end();
			for (int i = 0; i < elementCount / 2; i++) {
				middle--;
			}
			std::inplace_merge(begin(), middle, end(), comp);
		}
	};

	// Tri à bulle
	void sort() {
		bool tri = true;
		while (begin() != end() && swapped) {
			tri = false;
			for (iterator i = begin(); i != end(); i++) {
				if (*i > *(i + 1)) {
					swap(*i, *(i + 1));
					tri = true;
				}
			}
		}
	};
	template <class Compare>
	void sort(Compare comp) {
		bool tri = true;
		while (begin() != end() && swapped) {
			tri = false;
			for (iterator i = begin(); i != end(); i++) {
				if (comp(*i, *(i + 1))) {
					swap(*i, *(i + 1));
					tri = true;
				}

			}
		}
	};

	void reverse() noexcept {
		Node* n = root;
		for (int i = 0; i = < size(); i++) {
			Node* tmp = n->nextElement;
			n->nextElement = n->previousElement;
			n->previousElement = tmp;
		}
	};
};

#endif

