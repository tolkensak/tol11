
#ifndef _INC_TOLCPP_LIST_INL
#define _INC_TOLCPP_LIST_INL

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <new.h>    // ::new
#include <crtdbg.h> // _ASSERTE
#include <tolcpp_exception.h>

TOLNS_BEGIN


/////////////////////////////////////////////////////////////////////////////
// List<T, A> inline functions

template<typename T, typename A>
List<T, A>::Node::Node(typename List<T, A>::Node* prev, typename List<T, A>::Node* next)
	: pPrev(prev)
	, pNext(next)
{
#pragma push_macro("new")
#undef new
	::new((void*)(&data)) T;
#pragma pop_macro("new")
}

template<typename T, typename A>
List<T, A>::Node::~Node()
{
	data.~T();
}

template<typename T, typename A>
List<T, A>::List()
	: m_nCount(0)
	, m_pHead(NULL)
	, m_pTail(NULL)
{
}

template<typename T, typename A>
List<T, A>::List(const List<T, A>& list)
	: m_nCount(0)
	, m_pHead(NULL)
	, m_pTail(NULL)
{
	AddTail(list);
}

template<typename T, typename A>
List<T, A>::~List()
{
	RemoveAll();
}

template<typename T, typename A>
TOLINL
int
List<T, A>::Count() const
{
	return m_nCount;
}

template<typename T, typename A>
TOLINL
Bool
List<T, A>::IsEmpty() const
{
	return m_nCount==0;
}

template<typename T, typename A>
TOLINL
T&
List<T, A>::Data(NODE node)
{
	_ASSERTE(node);

	if(!node)
		throw ArgException();

	return ((Node*)node)->data;
}

template<typename T, typename A>
TOLINL
const T&
List<T, A>::Data(NODE node) const
{
	_ASSERTE(node);

	if(!node)
		throw ArgException();

	return ((Node*)node)->data;
}

template<typename T, typename A>
TOLINL
NODE
List<T, A>::Head() const
{
	return m_pHead;
}

template<typename T, typename A>
TOLINL
NODE
List<T, A>::Tail() const
{
	return m_pTail;
}

template<typename T, typename A>
TOLINL
T&
List<T, A>::Prev(NODE& node)
{
	_ASSERTE(node);

	if(!node)
		throw ArgException();

	Node* pNode=(Node*)node;
	node=(NODE)pNode->pPrev;
	return pNode->data;
}

template<typename T, typename A>
TOLINL
const T&
List<T, A>::Prev(NODE& node) const
{
	_ASSERTE(node);

	if(!node)
		throw ArgException();

	Node* pNode=(Node*)node;
	node=(NODE)pNode->pPrev;
	return pNode->data;
}

template<typename T, typename A>
TOLINL
T&
List<T, A>::Next(NODE& node)
{
	_ASSERTE(node);

	if(!node)
		throw ArgException();

	Node* pNode=(Node*)node;
	node=(NODE)pNode->pNext;
	return pNode->data;
}

template<typename T, typename A>
TOLINL
const T&
List<T, A>::Next(NODE& node) const
{
	_ASSERTE(node);

	if(!node)
		throw ArgException();

	Node* pNode=(Node*)node;
	node=(NODE)pNode->pNext;
	return pNode->data;
}

/////////////////////////////////////////////////////////////////////////////
// Node helpers
//

template<typename T, typename A>
typename List<T, A>::Node*
List<T, A>::MakeNode(typename List<T, A>::Node* pPrev, typename List<T, A>::Node* pNext)
{
	Node* pNode=new Node(pPrev, pNext);

	m_nCount++;
	_ASSERTE(m_nCount>0);  // make sure we don't overflow

	return pNode;
}

template<typename T, typename A>
void
List<T, A>::KillNode(typename List<T, A>::Node* pNode)
{
	delete pNode;

	m_nCount--;
	_ASSERTE(m_nCount>=0);  // make sure we don't overflow

	if(m_nCount==0)
		RemoveAll();
}

template<typename T, typename A>
NODE
List<T, A>::AddHead(A a)
{
	Node* pNew=MakeNode(NULL, m_pHead);
	pNew->data=a;

	if(m_pHead)
		m_pHead->pPrev=pNew;
	else
		m_pTail=pNew;

	m_pHead=pNew;

	return pNew;
}

template<typename T, typename A>
void
List<T, A>::AddHead(const List<T, A>& list)
{
	for(Node* pNode=list.m_pTail; pNode; pNode=pNode->pPrev)
		AddHead(pNode->data);
}

template<typename T, typename A>
NODE
List<T, A>::AddTail(A a)
{
	Node* pNew=MakeNode(m_pTail, NULL);
	pNew->data=a;

	if(m_pTail)
		m_pTail->pNext=pNew;
	else
		m_pHead=pNew;

	m_pTail=pNew;

	return pNew;
}

template<typename T, typename A>
void
List<T, A>::AddTail(const List<T, A>& list)
{
	for(Node* pNode=list.m_pHead; pNode; pNode=pNode->pNext)
		AddTail(pNode->data);
}

template<typename T, typename A>
NODE
List<T, A>::InsertBefore(NODE node, A a)
{
	if(!node)
		return AddHead(a);

	Node* pOld=(Node*)node;
	Node* pNew=MakeNode(pOld->pPrev, pOld);
	pNew->data=a;

	if(pOld->pPrev)
		pOld->pPrev->pNext=pNew;
	else
	{
		_ASSERTE(pOld==m_pHead);
		m_pHead=pNew;
	}

	pOld->pPrev=pNew;
	return pNew;
}

template<typename T, typename A>
NODE
List<T, A>::InsertAfter(NODE node, A a)
{
	if(!node)
		return AddTail(a);

	Node* pOld=(Node*)node;
	Node* pNew=MakeNode(pOld, pOld->pNext);
	pNew->data=a;

	if(pOld->pNext)
		pOld->pNext->pPrev=pNew;
	else
	{
		_ASSERTE(pOld==m_pTail);
		m_pTail=pNew;
	}

	pOld->pNext=pNew;
	return pNew;
}

template<typename T, typename A>
void
List<T, A>::RemoveHead()
{
	_ASSERTE(m_pHead);

	if(!m_pHead)
		return;

	Node* pOld=m_pHead;

	m_pHead=pOld->pNext;
	if(m_pHead)
		m_pHead->pPrev=NULL;
	else
		m_pTail=NULL;

	KillNode(pOld);
}

template<typename T, typename A>
void
List<T, A>::RemoveTail()
{
	_ASSERTE(m_pTail);

	if(!m_pTail)
		return;

	Node* pOld=m_pTail;

	m_pTail=pOld->pPrev;
	if(m_pTail)
		m_pTail->pNext=NULL;
	else
		m_pHead=NULL;

	KillNode(pOld);
}

template<typename T, typename A>
void
List<T, A>::Remove(NODE node)
{
	Node* pOld=(Node*)node;

	if(pOld==m_pHead)
		m_pHead=pOld->pNext;
	else
		pOld->pPrev->pNext=pOld->pNext;

	if(pOld==m_pTail)
		m_pTail=pOld->pPrev;
	else
		pOld->pNext->pPrev=pOld->pPrev;

	KillNode(pOld);
}

template<typename T, typename A>
void
List<T, A>::Empty()
{
	Node* pCur;
	Node* pNext;

	for(pCur=m_pHead; pCur; pCur=pNext)
	{
		pNext=pCur->pNext;
		delete pCur;
	}

	m_nCount=0;
	m_pHead=NULL;
	m_pTail=NULL;
}

template<typename T, typename A>
void
List<T, A>::RemoveAll()
{
	Node* pCur;
	Node* pNext;

	for(pCur=m_pHead; pCur; pCur=pNext)
	{
		pNext=pCur->pNext;
		delete pCur;
	}

	m_nCount=0;
	m_pHead=NULL;
	m_pTail=NULL;
}

template<typename T, typename A>
TOLINL
List<T, A>&
List<T, A>::operator=(const List<T, A>& list)
{
	RemoveAll();
	AddTail(list);
	return *this;
}

template<typename T, typename A>
TOLINL
List<T, A>&
List<T, A>::operator+=(A a)
{
	AddTail(a);
	return *this;
}

template<typename T, typename A>
TOLINL
List<T, A>&
List<T, A>::operator+=(const List<T, A>& list)
{
	AddTail(list);
	return *this;
}

template<typename T, typename A>
NODE
List<T, A>::Index(int nIndex) const
{
	DBG_ASSER(nIndex>=0 && nIndex<m_nCount);

	if(nIndex<0 || nIndex>=m_nCount)
		throw ArgException();

	Node* pNode=m_pHead;
	while(nIndex--)
		pNode=pNode->pNext;

	return pNode;
}

template<typename T, typename A>
NODE
List<T, A>::Find(A a, NODE nodeStartAfter) const
{
	Node* pNode=(Node*)nodeStartAfter;
	if(pNode)
		pNode=pNode->pNext; // start after the one specified
	else
		pNode=m_pHead; // start at head

	while(pNode)
	{
		if(pNode->data==a)
			return pNode;

		pNode=pNode->next;
	}

	return NULL;
}

template<typename T, typename A>
void
List<T, A>::Sort(Bool bAscending)
{
	if(m_nCount<2)
		return;

	T t;
	Node* pNode1;
	Node* pNode2;

	if(bAscending)
	{
		for(pNode1=m_pHead; pNode1!=NULL; pNode1=pNode1->pNext)
			for(pNode2=pNode1->pNext; pNode2!=NULL; pNode2=pNode2->pNext)
				if(pNode1->data>pNode2->data)
				{
					t=pNode1->data;
					pNode1->data=pNode2->data;
					pNode2->data=t;
				}
	}
	else
	{
		for(pNode1=m_pHead; pNode1!=NULL; pNode1=pNode1->pNext)
			for(pNode2=pNode1->pNext; pNode2!=NULL; pNode2=pNode2->pNext)
				if(pNode1->data<pNode2->data)
				{
					t=pNode1->data;
					pNode1->data=pNode2->data;
					pNode2->data=t;
				}
	}
}


TOLNS_END

#endif // _INC_TOLCPP_LIST_INL
