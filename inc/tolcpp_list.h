
#ifndef _INC_TOLCPP_LIST_H
#define _INC_TOLCPP_LIST_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <tolcpp_object.h>

TOLNS_BEGIN


/////////////////////////////////////////////////////////////////////////////
// List<TYPE, ARG_TYPE>

typedef PVoid NODE;

template<typename T, typename A=const T&>
class TOLTEMPLDECL List : public Object
{
protected:
	class Node : public Object
	{
	public:
		Node(Node* prev, Node* next);
		virtual ~Node();

		Node* pPrev;
		Node* pNext;
		T data;
	};

public:
	List();
	List(const List& list);
	virtual ~List();

	int Count() const;
	Bool IsEmpty() const;
	void Empty();

	T& Data(NODE node);
	const T& Data(NODE node) const;

	NODE Head() const;
	NODE Tail() const;

	T& Prev(NODE& node);
	const T& Prev(NODE& node) const;
	T& Next(NODE& node);
	const T& Next(NODE& node) const;

	NODE AddHead(A a);
	void AddHead(const List& list);
	NODE AddTail(A a);
	void AddTail(const List& list);

	NODE InsertBefore(NODE node, A a);
	NODE InsertAfter(NODE node, A a);

	void RemoveHead();
	void RemoveTail();
	void Remove(NODE node);
	void RemoveAll();

	List& operator=(const List& list);
	List& operator+=(A a);
	List& operator+=(const List& list);

	NODE Index(int nIndex) const; // get the 'nIndex'th element (may return NULL)
	NODE Find(A a, NODE nodeStartAfter=NULL) const; // defaults to starting at the HEAD, return NULL if not found
	void Sort(Bool bAscending=TRUE);

protected:
	Node* m_pHead;
	Node* m_pTail;
	int m_nCount;

	Node* MakeNode(Node* pPrev, Node* pNext);
	void KillNode(Node* pNode);
};


TOLNS_END

#include <tolcpp_list.inl>

#endif // _INC_TOLCPP_LIST_H
