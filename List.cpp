#include "List.h"
#include <assert.h>
#include <iostream>
#include <limits>
#include <initializer_list>
#include <iterator>
#include <vector>
#include <numeric>

#ifdef _DEBUG
#define DEBUG(x) x
#else
#define DEBUG(x)
#endif

using namespace std;

/* Constructeurs / destructeur */

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
	DEBUG("Constructor (Initializer list)");
	//DEBUG("Creating list with " << il.size() << " elements...");
	for (typename std::initializer_list<value_type>::iterator it = il.begin(); it != il.end(); it++) {
		push_back(*it);
	}
}

template<class T>
List<T>::~List() {
	//DEBUG("Destructor");
	clear();
}


/* Overloading */

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

	for (typename std::initializer_list<value_type>::iterator it = il.begin(); it != il.end(); it++) {
		push_back(*it);
	}

	elementCount = il.size();

	return *this;
}


/* Iterators */

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

/* Capacity */

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


/* Access */

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

/* Modifiers */

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
typename List<T>::iterator List<T>::insert(const_iterator position, InputIterator first, InputIterator last)
{
	while (first != last) {
		insert(position, *first);
		first++;
	}
	return iterator(position.node);
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
	for (typename std::initializer_list<value_type>::iterator ii = il.begin(); ii < il.end(); ii++)
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

/* End */

template class List<int>;
template List<int>::iterator List<int>::insert<std::vector<int>::iterator>(List<int>::const_iterator, std::vector<int>::iterator, std::vector<int>::iterator);
