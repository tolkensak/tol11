
#ifndef _INC_TOLCPP_MAP_H
#define _INC_TOLCPP_MAP_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <tolcpp.h>

typedef LPVOID POS;

TOLNS_BEGIN

/////////////////////////////////////////////////////////////////////////////
// Map<KEY, VALUE, ARG_KEY, ARG_VALUE>

template<class KEY, class VALUE, class ARG_KEY=const KEY&, class ARG_VALUE=const VALUE&>
class TOLTEMPLDECL Map
{
public:
	typedef struct tagPair
	{
		const KEY key;
		VALUE value;

	protected:
		tagPair(ARG_KEY keyVal) : key(keyVal){}
	} PAIR, *PPAIR;

protected:
	class Assoc : public tagPair
	{
		friend class Map<KEY, VALUE, ARG_KEY, ARG_VALUE>;
		Assoc* prev;
		Assoc* next;

	public:
		Assoc(ARG_KEY key) : tagPair(key){}
	};

public:
	/* explicit */ Map();
	~Map();

	int Num() const;
	BOOL IsEmpty() const;

	VALUE& operator[](ARG_KEY key);
	//VALUE& At(ARG_KEY key) const;
	//void SetAt(ARG_KEY key, ARG_VALUE value);

	BOOL Remove(ARG_KEY key);
	void RemoveAll();

	//POS First() const;
	//POS Next(POS pos) const;
	//PPAIR At(POS pos) const;

	POS First(KEY& key, VALUE& value);
	POS Next(POS pos, KEY& key, VALUE& value);

	BOOL Find(ARG_KEY key, VALUE& rValue) const;

protected:
	Assoc* m_first;
	Assoc* m_last;
	int m_num;

	Assoc* MakeAssoc(ARG_KEY key);
	void KillAssoc(Assoc* pAssoc);
	Assoc* AssocAt(ARG_KEY key) const;
};

TOLNS_END

#include <tolcpp_map.inl>

#endif // _INC_TOLCPP_MAP_H
