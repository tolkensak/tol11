
#ifndef _INC_TOLCPP_MAP_INL
#define _INC_TOLCPP_MAP_INL

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include<tolc_memory.h>
#include<tolc_debug.h>

TOLNS_BEGIN

/////////////////////////////////////////////////////////////////////////////
// Map<KEY, VALUE, ARG_KEY, ARG_VALUE> inline functions

template<class KEY, class VALUE, class ARG_KEY, class ARG_VALUE>
TOLINL
int Map<KEY, VALUE, ARG_KEY, ARG_VALUE>::Num() const
{ return m_num; }

template<class KEY, class VALUE, class ARG_KEY, class ARG_VALUE>
TOLINL
BOOL Map<KEY, VALUE, ARG_KEY, ARG_VALUE>::IsEmpty() const
{ return m_num==0; }

//template<class KEY, class VALUE, class ARG_KEY, class ARG_VALUE>
//TOLINL
//VALUE& Map<KEY, VALUE, ARG_KEY, ARG_VALUE>::At(ARG_KEY key) const
//{ return (*this)[key]; }

//template<class KEY, class VALUE, class ARG_KEY, class ARG_VALUE>
//TOLINL
//void Map<KEY, VALUE, ARG_KEY, ARG_VALUE>::SetAt(ARG_KEY key, ARG_VALUE value)
//{ (*this)[key]=value; }

//template<class KEY, class VALUE, class ARG_KEY, class ARG_VALUE>
//TOLINL
//POS Map<KEY, VALUE, ARG_KEY, ARG_VALUE>::First() const
//{ return m_first; }

//template<class KEY, class VALUE, class ARG_KEY, class ARG_VALUE>
//TOLINL
//POS Map<KEY, VALUE, ARG_KEY, ARG_VALUE>::Next(POS pos) const
//{ return ((Assoc*)pos)->next; }

//template<class KEY, class VALUE, class ARG_KEY, class ARG_VALUE>
//TOLINL
//typename Map<KEY, VALUE, ARG_KEY, ARG_VALUE>::PPAIR Map<KEY, VALUE, ARG_KEY, ARG_VALUE>::At(POS pos) const
//{ return (PPAIR)pos; }

template<class KEY, class VALUE, class ARG_KEY, class ARG_VALUE>
POS Map<KEY, VALUE, ARG_KEY, ARG_VALUE>::First(KEY& key, VALUE& value)
{
	if(m_first)
	{
		key=m_first->key;
		value=m_first->value;
	}

	return m_first; 
}

template<class KEY, class VALUE, class ARG_KEY, class ARG_VALUE>
POS Map<KEY, VALUE, ARG_KEY, ARG_VALUE>::Next(POS pos, KEY& key, VALUE& value)
{
	Assoc* p=((Assoc*)pos)->next;
	if(p)
	{
		key=p->key;
		value=p->value;
	}

	return p; 
}

/////////////////////////////////////////////////////////////////////////////
// Map<KEY, VALUE, ARG_KEY, ARG_VALUE> out-of-line functions

template<class KEY, class VALUE, class ARG_KEY, class ARG_VALUE>
Map<KEY, VALUE, ARG_KEY, ARG_VALUE>::Map()
{
	m_first=NULL;
	m_last=NULL;
	m_num=0;
}

template<class KEY, class VALUE, class ARG_KEY, class ARG_VALUE>
Map<KEY, VALUE, ARG_KEY, ARG_VALUE>::~Map()
{ RemoveAll(); }

template<class KEY, class VALUE, class ARG_KEY, class ARG_VALUE>
void Map<KEY, VALUE, ARG_KEY, ARG_VALUE>::RemoveAll()
{
	for(Assoc* p=m_first; p; p=p->next)
		p->Assoc::~Assoc();

	m_first=NULL;
	m_last=NULL;
	m_num=0;
}

template<class KEY, class VALUE, class ARG_KEY, class ARG_VALUE>
typename Map<KEY, VALUE, ARG_KEY, ARG_VALUE>::Assoc* Map<KEY, VALUE, ARG_KEY, ARG_VALUE>::MakeAssoc(ARG_KEY key)
{
	Assoc* p=(Assoc*)malloc(sizeof(Assoc));
	Mem_Set(p, 0, sizeof(Assoc));

#pragma push_macro("new")
#undef new
	::new(p) Assoc(key);
#pragma pop_macro("new")

	if(m_last)
	{
		p->prev=m_last->prev;
		m_last->next=p;
	}

	m_last=p;

	if(!m_first)
		m_first=m_last;

	m_num++;
	return p;
}

template<class KEY, class VALUE, class ARG_KEY, class ARG_VALUE>
void Map<KEY, VALUE, ARG_KEY, ARG_VALUE>::KillAssoc(Assoc* pAssoc)
{
	if(pAssoc==m_last)
		m_last=pAssoc->prev;

	pAssoc->prev->next=pAssoc->next;
	pAssoc->next->prev=pAssoc->prev;

	pAssoc->~Assoc();
	Mem_Free(pAssoc);

	m_num--;
	if(m_num==0)
		RemoveAll();
}

template<class KEY, class VALUE, class ARG_KEY, class ARG_VALUE>
typename Map<KEY, VALUE, ARG_KEY, ARG_VALUE>::Assoc* Map<KEY, VALUE, ARG_KEY, ARG_VALUE>::AssocAt(ARG_KEY key) const
{
	for(Assoc* p=m_first; p; p=p->next)
		if(p->key==key)
			return p;

	return NULL;
}

template<class KEY, class VALUE, class ARG_KEY, class ARG_VALUE>
BOOL Map<KEY, VALUE, ARG_KEY, ARG_VALUE>::Find(ARG_KEY key, VALUE& rValue) const
{
	Assoc* p=AssocAt(key);
	if(!p)
		return FALSE;

	rValue=p->value;
	return TRUE;
}

template<class KEY, class VALUE, class ARG_KEY, class ARG_VALUE>
VALUE& Map<KEY, VALUE, ARG_KEY, ARG_VALUE>::operator[](ARG_KEY key)
{
	Assoc* p=AssocAt(key);
	if(!p)
		p=MakeAssoc(key);

	return p->value;
}

template<class KEY, class VALUE, class ARG_KEY, class ARG_VALUE>
BOOL Map<KEY, VALUE, ARG_KEY, ARG_VALUE>::Remove(ARG_KEY key)
{
	Assoc* p=AssocAt(key);
	if(!p)
		return FALSE;

	KillAssoc(p);
	return TRUE;
}

TOLNS_END

#endif // _INC_TOLCPP_MAP_INL
