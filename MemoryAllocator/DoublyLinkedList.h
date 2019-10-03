#pragma once

template <class T>
class DoublyLinkedList
{
public:
	DoublyLinkedList();

	T * front() const;
	T * back() const;

	void pushFront(T item);
	void pushBack(T item);
	void popBack();
	void popFront();
	void insert(int position, T& val);
	void erase(int position);
	void erase(T& val);


private:
	T * m_pHead;
	T * m_pTail;
};