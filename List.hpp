/**
 * List.hpp
 * Projet 2016 INFO0402
 *
 * ANIC-ANTIC Raphaël
 * HUBERT Julien
**/
#pragma once

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
			node = node->next;
			return *this;
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
	explicit List() : root(new Node()), elementCount(0) {
		DEBUG(std::cout << "Construct default" << std::endl;)
	}
	explicit List(size_type n) : root(new Node()), elementCount(0) {
		while (elementCount < n)
			push_back(value_type());
		DEBUG(std::cout << "Construct fill no value" << std::endl;)
	};
	List(size_type n, const value_type& val) : root(new Node()), elementCount(0) {
		while (elementCount < n)
			push_back(val);
		DEBUG(std::cout << "Construct fill" << std::endl;)
	};
	template <class InputIterator>
	List(InputIterator first, InputIterator last, typename std::iterator_traits<InputIterator>::iterator_category* = nullptr) : root(new Node()), elementCount(0) {
		insert(begin(), first, last);
		DEBUG(std::cout << "Construct it" << std::endl;)
	};
	List(const List& x) : root(new Node()), elementCount(0) {
		for (const_iterator it = x.cbegin(); it != x.cend(); it++) {
			push_back(*it);
		}
		DEBUG(std::cout << "Construct copy" << std::endl;)
	};
	List(List&& x) : root(nullptr), elementCount(0) {
		elementCount = x.elementCount;
		root = std::move(x.root);
		x.root = nullptr;
		x.elementCount = (size_type)0;
		DEBUG(std::cout << "Construct move" << std::endl;)
	};
	List(std::initializer_list<value_type> il) : root(new Node()), elementCount(0) {
		for (auto it : il) {
			push_back(it);
		}
		DEBUG(std::cout << "Construct initialiser list" << std::endl;)
	};

	// Destructors
	virtual ~List() {
		clear();
	};

	// Overloading
	List& operator=(const List& x) {
		if (this != &x) {
			DEBUG(std::cout << "Copy assignement" << std::endl;)
			clear();

			for (const_iterator it = x.cbegin(); it != x.cend(); it++) {
				push_back(*it);
			}
		}
		return *this;
	};
	List& operator=(List&& x) {
		if (this != &x) {
			DEBUG(std::cout << "Move assignement" << std::endl;)

			clear();

			elementCount = x.elementCount;
			root = std::move(x.root);

			x.root = nullptr;
			x.elementCount = (size_type)0;
		}
		return *this;
	};
	List& operator=(std::initializer_list<value_type> il) {
		DEBUG(std::cout << "Initializer list assignement" << std::endl;)
		clear();

		for (auto it : il) {
			push_back(it);
		}

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
	const_reverse_iterator rbegin() const noexcept {
		return const_reverse_iterator(root->previous);
	};
	iterator end() {
		return iterator(root);
	};
	const_iterator end() const {
		return const_iterator(root);
	};
	reverse_iterator rend() {
		return reverse_iterator(begin());
	};
	const_reverse_iterator rend() const {
		return const_reverse_iterator(begin());
	};;
	const_iterator cbegin() const noexcept {
		return const_iterator(root->next);
	};
	const_iterator cend() const noexcept {
		return const_iterator(root);
	};
	const_reverse_iterator crbegin() const noexcept {
		return const_reverse_iterator(root->previous);
	};
	const_reverse_iterator crend() const noexcept {
		return const_reverse_iterator(cbegin());
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
		assert(elementCount > 0);
		return *this->begin();
	};
	const_reference front() const {
		assert(elementCount > 0);
		return (const_reference)*this->cbegin();
	};
	reference back() {
		assert(elementCount > 0);
		iterator it = end();
		it--;
		return (reference)*it;
	};
	const_reference back() const {
		assert(elementCount > 0);
		const_iterator it = cend();
		it--;
		return (const_reference)*it;
	};

	// Modifiers
	template <class InputIterator>
	void assign(InputIterator first, InputIterator last, typename std::iterator_traits<InputIterator>::iterator_category* = nullptr) {
		clear();

		while (first != last) {
			push_back(*first);
			first++;
		}
	};
	void assign(size_type n, const value_type& val) {
		clear();

		for (size_type i = 0; i < n; i++) {
			push_back(val);
		}
	};
	void assign(std::initializer_list<value_type> il) {
		clear();

		for(auto i : il)
			push_back(i);
	};
	template <class... Args>
	void emplace_front(Args&&... args) {
		insert(begin(), value_type(std::forward<Args>(args)...));
	}
	void push_front(const value_type& val) {
		insert(begin(), val);
	};
	void push_front(value_type&& val) {
		insert(begin(), std::move(val));
	};
	void pop_front() {
		const_iterator it(cbegin());
		erase(it);
	};
	template <class... Args>
	void emplace_back(Args&&... args) {
		insert(end(), value_type(std::forward<Args>(args)...));
	}
	void push_back(const value_type& val) {
		insert(end(), val);
	};
	void push_back(value_type&& val) {
		insert(end(), std::move(val));
	};
	void pop_back() {
		const_iterator it(cend());
		erase(--it);
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
		for (auto it : il)
			insert(position, it);
		return iterator(position.node);
	};
	iterator erase(const_iterator position) {
		assert(position != cend());
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
		std::swap(*this, x);
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

	// Operations
	void splice(const_iterator position, List& x) {
		const_iterator first = x.begin(), last = x.end();
		while (first != last) {
			/* On garde la position dans x car first va être invalidé */
			const_iterator tmp = first;
			tmp++;
			splice(position, x, first);
			first = tmp;
		}
	};
	void splice(const_iterator position, List& x, const_iterator i) {
		// x
		i.node->previous->next = i.node->next;
		i.node->next->previous = i.node->previous;

		// this
		position.node->previous->next = i.node;
		i.node->previous = position.node->previous;
		position.node->previous = i.node;
		i.node->next = position.node;

		elementCount++;
		x.elementCount--;
	};
	void splice(const_iterator position, List& x, const_iterator first, const_iterator last) {
		while (first != last) {
			/* On garde la position dans x car first va être invalidé */
			const_iterator tmp = first;
			tmp++;
			splice(position, x, first);
			first = tmp;
		}
	};

	void remove(const_reference val)
	{
		for (iterator it = begin(); it != end(); ++it)
		{
			if (*it == val) {
				it = erase(it);
				--it;
			}
		}
	};

	template <class Predicate>
	void remove_if(Predicate pred)
	{
		for (iterator it = begin(); it != end(); ++it)
		{
			if (pred(*it))
			{
				it = erase(it);
				--it;
			}
		}
	};

	void unique() {
		iterator previous;
		for (iterator it = begin(); it != end(); ++it) {
			if (it != begin()) {
				previous = it;
				previous--;
				if (*it == *previous) {
					it = erase(it);
					--it;
				}
			}
		}
		DEBUG(std::cout << "Unique applicated : "; for (iterator it = begin(); it != end(); it++) std::cout << *it << " "; std::cout << std::endl;)
	};
	template <class BinaryPredicate>
	void unique(BinaryPredicate binary_pred)
	{
		iterator previous;
		for (iterator it = begin(); it != end(); ++it) {
			if (it != begin()) {
				previous = it;
				previous--;
				if (binary_pred(*it, *previous)) {
					it = erase(it);
					--it;
				}
			}
		}
		DEBUG(std::cout << "Unique (with BinaryPredicate) applicated : "; for (iterator it = begin(); it != end(); it++) std::cout << *it << " "; std::cout << std::endl;)
	};

	void merge(List &x) {
		if (&x != this) {
			Node* actual = root->next;
			while (x.elementCount > 0) {
				Node* firstX = x.root->next;

				if (actual == root || reinterpret_cast<DataNode*>(firstX)->data < reinterpret_cast<DataNode*>(actual)->data) {
					firstX->previous->next = firstX->next;
					firstX->next->previous = firstX->previous;

					actual->previous->next = firstX;

					firstX->previous = actual->previous;
					firstX->next = actual;

					actual->previous = firstX;

					x.elementCount--;
					elementCount++;
				}
				else {
					actual = actual->next;
				}
			}
		}

		DEBUG(std::cout << "List merged : "; for (iterator it = begin(); it != end(); it++) std::cout << *it << " "; std::cout << std::endl);
	};
	template <class Compare>
	void merge(List &x, Compare comp) {
		if (&x != this) {
			/* On part du premier élément de this */
			Node* actual = root->next;
			// Tant que x n'est pas vide
			while (x.elementCount > 0) {
				/* On prend toujours le premier élement de x, qui va changer au fur et à mesure*/
				Node* firstX = x.root->next;

				/* Si on est à la fin, on insère avant la fin sans besoin de comparer */
				/* Si on est pas à la fin, on vérifie si x est inférieur à la valeur actuelle, si oui on l'insère avant */
				if (actual == root || comp(reinterpret_cast<DataNode*>(firstX)->data, reinterpret_cast<DataNode*>(actual)->data)) {
					firstX->previous->next = firstX->next;
					firstX->next->previous = firstX->previous;

					actual->previous->next = firstX;

					firstX->previous = actual->previous;
					firstX->next = actual;

					actual->previous = firstX;

					x.elementCount--;
					elementCount++;
				}
				/* Sinon on avance */
				else {
					actual = actual->next;
				}
			}
		}

		DEBUG(std::cout << "List merged (with comparator) : "; for (iterator it = begin(); it != end(); it++) std::cout << *it << " "; std::cout << std::endl);
	};

	void sort()
	{
		// Tri à bulles
		for (size_type i = size() - 1; i > 0; i--) {
			Node* actual = root->next;
			bool sorted = true;
			for (size_type j = 0; j < i; j++) {
				Node* nextElement = actual->next;

				if (reinterpret_cast<DataNode*>(nextElement)->data < reinterpret_cast<DataNode*>(actual)->data) {
					// Swap
					Node* pTmp = actual->previous;
					Node* nTmp = nextElement->next;

					actual->previous->next = nextElement;
					nextElement->next->previous = actual;

					actual->previous = nextElement;
					actual->next = nTmp;

					nextElement->previous = pTmp;
					nextElement->next = actual;
					sorted = false;
				}
				else
					actual = actual->next;
			}
			if (sorted) break;
		}

		DEBUG(std::cout << "List sorted : "; for (iterator it = begin(); it != end(); it++) std::cout << *it << " "; std::cout << std::endl);
	}
	template <class Compare>
	void sort(Compare comp) {
		// Tri à bulles
		for (size_type i = size() - 1; i > 0; i--) {
			Node* actual = root->next;
			bool sorted = true;
			for (size_type j = 0; j < i; j++) {
				Node* nextElement = actual->next;

				if (comp(reinterpret_cast<DataNode*>(nextElement)->data, reinterpret_cast<DataNode*>(actual)->data)) {
					// Swap
					Node* pTmp = actual->previous;
					Node* nTmp = nextElement->next;

					actual->previous->next = nextElement;
					nextElement->next->previous = actual;

					actual->previous = nextElement;
					actual->next = nTmp;

					nextElement->previous = pTmp;
					nextElement->next = actual;
					sorted = false;
				}
				else
					actual = actual->next;
			}
			if (sorted) return;
		}

		DEBUG(std::cout << "List sorted (with Comparator) : "; for (iterator it = begin(); it != end(); it++) std::cout << *it << " "; std::cout << std::endl);
	};


	void reverse() noexcept {
		Node* n = root;
		Node* tmp;
		for (int i = 0; i <= size(); i++) {
			tmp = n->next;
			n->next = n->previous;
			n->previous = tmp;

			n = n->next;
		}
	}
};

// Relational operators

template <class T>
bool operator== (const List<T>& lhs, const List<T>& rhs) {
	if (lhs.size() == rhs.size()) {
		if (lhs.size() == 0) return true;

		typename List<T>::const_iterator itl = lhs.begin(), itr = rhs.begin();

		while (itl != lhs.end()) {
			if (*itl != *itr) return false;
			itl++; itr++;
		}
		return true;
	}
	return false;
};
template <class T>
bool operator!= (const List<T>& lhs, const List<T>& rhs) {
	return !(lhs == rhs);
};
template <class T>
bool operator< (const List<T>& lhs, const List<T>& rhs) {
	return std::lexicographical_compare(lhs.begin(), lhs.end(),rhs.begin(), rhs.end());
};
template <class T>
bool operator<= (const List<T>& lhs, const List<T>& rhs) {
	return !(rhs < lhs);
};
template <class T>
bool operator> (const List<T>& lhs, const List<T>& rhs) {
	return rhs < lhs;
}
template <class T>
bool operator>= (const List<T>& lhs, const List<T>& rhs) {
	return !(lhs < rhs);
};

template <class T>
void swap(List<T>& x, List<T>& y) {
	List<T> z(std::move(x));
	x = std::move(y);
	y = std::move(z);
};

#endif
