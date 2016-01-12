/********************************************************** 
ChaiOS 0.05 Copyright (C) 2012-2015 Nathaniel Cleland
Licensed under the ChaiOS License
See License for full details

Project: kcstdlib
File: list.h
Path: C:\Users\Nathaniel\Documents\Visual Studio 2013\Projects\ChaiOS\kcstdlib\list.h
Created by Nathaniel on 24/12/2015 at 19:16

Description: Linked list implementation
**********************************************************/
#ifndef CHAIOS_KCSTDLIB_LIST_H
#define CHAIOS_KCSTDLIB_LIST_H

#include <iostream.h>

typedef void* iterator_t;

template<class T>
class LinkedList
{
public:
	LinkedList(const LinkedList& LL);
	LinkedList& operator=(LinkedList byValList);
	LinkedList() : head(NULL), tail(NULL) {}
	~LinkedList();

	static LinkedList<int> sumLists(const LinkedList<int>& LL1, LinkedList<int>& LL2);

	void insertToTail(T val);
	void insertToHead(T val);
	iterator_t getHead();
	iterator_t getTail();
	iterator_t getNext(iterator_t itr)
	{
		return (iterator_t)(((Node<T>*)itr)->next);
	}
	iterator_t getPrev(iterator_t itr)
	{
		return (iterator_t)(((Node<T>*)itr)->prev);
	}
	T getValue(iterator_t itr);
	iterator_t findValue(T val);
	void removeAt(iterator_t itr);
	bool isEntry(iterator_t itr) { 
		return itr != 0; 
	}
	void print();
	void printBackwards();

private:
	template<class T>
	class Node
	{
	public:
		T data;
		Node<T> * prev;
		Node<T> * next;

		Node<T>(const T& d, Node<T>* prev, Node<T> * next) : data(d), prev(prev), next(next) {}
		Node<T>(const Node<T>& copyNode) : data(copyNode.data), prev(copyNode.prev), next(copyNode.next) {}

	private:
		Node<T>& operator=(const Node<T>&) = delete;
	};

	Node<T> * head;
	Node<T> * tail;
};

template<class T>
LinkedList<T>::LinkedList(const LinkedList<T>& LL) : head(NULL), tail(NULL)
{
	const Node<T> * curr = LL.head;

	if (!head && curr)
	{
		head = new Node<T>(curr->data, NULL, NULL);
		tail = head;
		curr = curr->next;
	}

	while (curr)
	{
		Node<T> * newNode = new Node<T>(curr->data, tail, NULL);
		tail->next = newNode;
		tail = newNode;
		curr = curr->next;
	}
}

template<class T>
LinkedList<T>& LinkedList<T>::operator=(LinkedList byValList)
{
	Node<T>* temp = head;
	head = byValList.head;
	byValList.head = temp;
	return *this;
}

template<class T>
LinkedList<T>::~LinkedList()
{
	Node<T> * curr = head;
	while (head)
	{
		head = head->next;
		delete curr;
		curr = head;
	}
}

template<class T>
void LinkedList<T>::insertToTail(T val)
{
	Node<T> * newNode = new Node<T>(val, NULL, NULL);
	if (tail == NULL)
	{
		newNode->next = tail;
		newNode->prev = NULL;
		tail = newNode;
		head = newNode;
		return;
	}
	tail->next = newNode;
	newNode->prev = tail;
	tail = tail->next;
}

template<class T>
void LinkedList<T>::insertToHead(T val)
{
	Node<T>* newNode = new Node<T>(val, NULL, NULL);
	newNode->next = head;
	Node<T>* old = head;
	head = newNode;
	if (head->next == NULL)
		tail = newNode;
	else
	{
		old->prev = head;
	}
}

template<class T>
iterator_t LinkedList<T>::getHead()
{
	return (iterator_t)head;
}

template<class T>
iterator_t LinkedList<T>::getTail()
{
	return (iterator_t)tail;
}

template<class T>
T LinkedList<T>::getValue(iterator_t itr)
{
	return ((Node<T>*)itr)->data;
}

template <class T>
void LinkedList<T>::removeAt(iterator_t itr)
{
	Node<T>* iter = (Node<T>*)itr;
	Node<T>* prev = iter->prev;
	Node<T>* next = iter->next;
	prev->next = next;
	next->prev = prev;
	delete iter;
}

template<class T>
iterator_t LinkedList<T>::findValue(T val)
{
	iterator_t itr = getHead();
	while (isEntry(itr))
	{
		if (val == getValue(itr))
			return itr;
		itr = getNext(itr);
	}
	return itr;
}

template<class T>
void LinkedList<T>::print()
{
	Node<T> * curr = head;
	while (curr)
	{
		kout << curr->data << " --> ";
		curr = curr->next;
	}
	kout << "NULL" << "\n";
}

template<class T>
void LinkedList<T>::printBackwards()
{
	Node<T> * curr;
	LinkedList ReversedList(new Node<T>(head->data));
	curr = head->next;
	while (curr)
	{
		ReversedList.insertToHead(curr->data);
		curr = curr->next;
	}

	curr = ReversedList.head;
	while (curr)
	{
		kout << curr->data << " --> ";
		curr = curr->next;
	}
	kout << "NULL\n";
}

template<class T>
LinkedList<int> LinkedList<T>::sumLists(const LinkedList<int>& LL1, LinkedList<int>& LL2)
{
	Node<T>* curr1 = LL1.head;
	Node<T>* curr2 = LL2.head;

	LinkedList<int> ResultList;

	int newData;
	int carry = 0;

	while (curr1 && curr2)
	{
		newData = (curr1->data + curr2->data + carry) % 10;
		carry = (curr1->data + curr2->data + carry) / 10;
		ResultList.insertToTail(newData);

		curr1 = curr1->next;
		curr2 = curr2->next;
	}

	while (curr1 || curr2)
	{
		if (carry)
		{
			if (curr1)
				ResultList.insertToTail(curr1->data + carry);
			if (curr2)
				ResultList.insertToTail(curr2->data + carry);
			carry = 0;
			continue;
		}
		if (curr1)
		{
			ResultList.insertToTail(curr1->data);
			curr1 = curr1->next;
		}
		if (curr2)
		{
			ResultList.insertToTail(curr2->data + carry);
			curr2 = curr2->next;

		}


	}

	return ResultList;
}

template <class T> class ListEntry {
private:
	T entry;
	ListEntry<T>* prev;
	ListEntry<T>* next;
	
	template <class T> friend class LinkedList;
	template <class T> friend class iterator;
};

#endif
