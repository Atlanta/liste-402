//
//  list.h
//  Projet INFO0402
//
//  Created by Julien Hubert on 24/03/2016.
//  Copyright © 2016 Julien Hubert. All rights reserved.
//
#pragma once

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
		ConstListIterator& operator++() { node = node->next
			; return *this; };
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

template class List<int>;

/* Constructeurs / destructeur */

#pragma region Constructors/Destructors
template<class T>
List<T>::List() : root(new Node()), elementCount(0) {
	//DEBUG("Default constructor");
}

template<class T>
List<T>::List(size_type n) : root(new Node()), elementCount(0) {
	//DEBUG("Constructor (Fill 1)");

	//DEBUG("Creating list with " << n << " elements...");
	while (elementCount < n)
		push_back(value_type());
}

template<class T>
List<T>::List(size_type n, const value_type& val) : root(new Node()), elementCount(0) {
	//DEBUG("Constructor (Fill 2, with obj)");

	//DEBUG("Creating list with " << n << " elements...");
	while (elementCount < n)
		push_back(val);
}

template<class T>
template<class InputIterator>
List<T>::List(InputIterator first, InputIterator last) : root(new Node()), elementCount(0) {
	//DEBUG("Constructor (Input iteraors)");

	insert(begin(), first, last);
}

template<class T>
List<T>::List(const List& x) : root(new Node()), elementCount(0) {
	//DEBUG("Constructor (copy)");

	//DEBUG("Creating list with " << x.elementCount << " elements...");
	for (const_iterator it = x.cbegin(); it != x.cend(); it++) {
		push_back(*it);
	}
}

template<class T>
List<T>::List(List&& x) : root(std::move(x.root)), elementCount(x.elementCount) {
	//DEBUG("Constructor (move)");
	x.root = nullptr;
	x.elementCount = (size_type)0;
}

template<class T>
List<T>::List(std::initializer_list<value_type> il) : root(new Node()), elementCount(0) {
	//DEBUG("Constructor (Initializer list)");

	//DEBUG("Creating list with " << il.size() << " elements...");
	for (std::initializer_list<value_type>::iterator it = il.begin(); it != il.end(); it++) {
		push_back(*it);
	}
}

template<class T>
List<T>::~List() {
	//DEBUG("Destructor");
	clear();
}
#pragma endregion


/* Overloading */

#pragma region Operators overloading
template<class T>
List<T>& List<T>::operator=(const List& x) {
	if (this != &x) {
		clear();

		root = new Node();

		for (const_iterator it = x.cbegin(); it != x.cend(); it++) {
			push_back(*it);
		}

		elementCount = x.elementCount;
	}
	return *this;
}
template<class T>
List<T>& List<T>::operator=(List&& x) {
	if (this != &x) {
		DEBUG("Move assignement");

		clear();

		root = std::move(x.root);
		elementCount = x.elementCount;
		delete x.root;// = nullptr;
	}
	return *this;
}
template<class T>
List<T>& List<T>::operator=(std::initializer_list<value_type> il) {
	clear();

	root = new Node();

	for (std::initializer_list<value_type>::iterator it = il.begin(); it != il.end(); it++) {
		push_back(*it);
	}

	elementCount = il.size();

	return *this;
}
#pragma endregion


/* Iterators */

#pragma region Iterators
template<class T>
typename List<T>::iterator List<T>::begin() {
	return iterator(root->next);
}
template<class T>
typename List<T>::const_iterator List<T>::begin() const {
	return const_iterator(root->next);
}
template<class T>
typename List<T>::reverse_iterator List<T>::rbegin() noexcept
{
	return reverse_iterator(root->previous);
}
/*template<class T>
typename List<T>::const_reverse_iterator List<T>::rbegin() const noexcept
{
Node* ptr = root;
for (size_type i = 0; i < elementCount; i++)
if (i != 0) ptr = ptr->next;
return const_reverse_iterator(ptr);
}*/
template<class T>
typename List<T>::iterator List<T>::end() {
	return iterator(root);
}
/*template<class T>
typename List<T>::const_iterator List<T>::end() const {
return (const_iterator)end();
}*/
template<class T>
typename List<T>::reverse_iterator List<T>::rend()
{
	return reverse_iterator(this->begin());
}
/*template<class T>
typename List<T>::const_reverse_iterator List<T>::rend() const
{
return (const_reverse_iterator)rend();
}*/
template<class T>
typename List<T>::const_iterator List<T>::cbegin() const noexcept
{
	return const_iterator(root);
}
template<class T>
typename List<T>::const_iterator List<T>::cend() const noexcept
{
	Node* ptr = root;
	for (size_type i = 0; i < elementCount; i++) ptr = ptr->next;
	return const_iterator(ptr);
}
template<class T>
typename List<T>::const_reverse_iterator List<T>::crbegin() const noexcept
{
	return const_reverse_iterator();
}
template<class T>
typename List<T>::const_reverse_iterator List<T>::crend() const noexcept
{
	return const_reverse_iterator();
}
#pragma endregion

/* Capacity */

#pragma region Capacity
template <class T>
bool List<T>::empty() const {
	return (elementCount == 0) ? true : false;
}
template <class T>
typename List<T>::size_type List<T>::size() const {
	return elementCount;
}
template <class T>
typename List<T>::size_type List<T>::max_size() const {
	return std::numeric_limits<List<T>::size_type>::max();
}
#pragma endregion


/* Access */

#pragma region Access
template <class T>
typename List<T>::reference List<T>::front() {
	assert(elementCount>0);
	return *this->begin();
}

template <class T>
typename List<T>::const_reference List<T>::front() const {
	assert(elementCount>0);
	return (const_reference)*this->cbegin();
}

template <class T>
typename List<T>::reference List<T>::back() {
	assert(elementCount>0);
	iterator it = end();
	it--;
	return (reference)*it;
}

template <class T>
typename List<T>::const_reference List<T>::back() const {
	assert(elementCount>0);
	const_iterator it = cend();
	it--;
	return (const_reference)*it;
}
#pragma endregion

/* Modifiers */

#pragma region Modifiers
template<class T>
template <class InputIterator> void List<T>::assign(InputIterator first, InputIterator last) {
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
}

template<class T>
void List<T>::assign(size_type n, const value_type & val)
{
}

template<class T>
void List<T>::assign(std::initializer_list<value_type> il)
{
}

template<class T>
void List<T>::push_front(const value_type& val)
{
	insert(begin(), val);
}

template<class T>
void List<T>::push_front(value_type&& val)
{
	insert(begin(), std::move(val));
}

template<class T>
void List<T>::pop_front()
{
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
}

template<class T>
void List<T>::push_back(const value_type& val)
{
	insert(end(), val);
}

template<class T>
void List<T>::push_back(value_type&& val)
{
	insert(end(), std::move(val));
}

template<class T>
void List<T>::pop_back()
{
	const_iterator it(end());
	it--;
	erase(it);
}

template<class T>
typename List<T>::iterator List<T>::insert(const_iterator position, const value_type& val)
{
	DataNode* data_node = new DataNode(val); // pass data
	Node* current = position.node;
	data_node->next = current;
	data_node->previous = current->previous;
	current->previous->next = data_node;
	current->previous = data_node;
	elementCount++;
	return iterator(data_node);
}
template<class T>
typename List<T>::iterator List<T>::insert(const_iterator position, size_type n, const value_type & val)
{
	for (size_type i = 0; i < n; i++) {
		position = insert(position, val);
	}
	return iterator(position.node);
}

template<class T>
template<class ...Args>
inline typename List<T>::iterator List<T>::emplace(const_iterator position, Args && ...args)
{
	return iterator(insert(position, value_type(std::forward<Args>(args)...)));
}

template <class T>
template <class InputIterator>
inline typename List<T>::iterator List<T>::insert(const_iterator position, InputIterator first, InputIterator last)
{
	distance(first, last);
	//return iterator(tmp);
}

template<class T>
typename List<T>::iterator List<T>::insert(const_iterator position, value_type&& val)
{
	DataNode* data_node = new DataNode(std::move(val)); // pass data
	Node* current = position.node;
	data_node->next = current;
	data_node->previous = current->previous;
	current->previous->next = data_node;
	current->previous = data_node;
	elementCount++;
	return iterator(current);
}
template<class T>
typename List<T>::iterator List<T>::insert(const_iterator position, std::initializer_list<value_type> il)
{
	iterator it;
	for (std::initializer_list<value_type>::iterator ii = il.begin(); ii < il.end(); ii++)
	{
		it = this->insert(position, *ii);
	}
	for (size_type i = il.size(); i > 0; i--) it--;
	elementCount++;
	return it;
}
template<class T>
typename List<T>::iterator List<T>::erase(const_iterator position)
{
	assert(position != end());
	Node *current = position.node, *tmp = position.node->next;
	current->previous->next = current->next;
	current->next->previous = current->previous;
	delete current;
	elementCount--;
	return iterator(tmp);
}
template<class T>
typename List<T>::iterator List<T>::erase(const_iterator first, const_iterator last)
{
	while (first != last)
		first = erase(first);
	return iterator(first.node);
}
template<class T>
void List<T>::swap(List & x)
{
	// TODO
}
template<class T>
void List<T>::resize(size_type n)
{
	if (n < elementCount) {
		while (elementCount > n)
			pop_back();
	}
	else {
		while (elementCount < n)
			push_back(value_type());
	}
}
template<class T>
void List<T>::resize(size_type n, const value_type& val)
{
	if (n < elementCount) {
		while (elementCount > n)
			pop_back();
	}
	else {
		while (elementCount < n)
			push_back(val);
	}
}
template<class T>
void List<T>::clear() noexcept
{
	while (elementCount > 0)
		pop_back();
}

#pragma endregion



/* End */