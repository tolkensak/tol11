
#include "stdafx.h"
#include <tolc_memory.h>
#include <tolc_debug.h>

TOLEXTC_BEGIN


typedef PVoid LDATA, *PLDATA;
typedef const PVoid CLDATA, *PCLDATA;

typedef struct tagNode NODE, *POS;
struct tagNode
{
	POS prev;
	POS next;
	LDATA data;
};

POS Node_Make()
{ return Mem_Alloc(MEM_ZERO_MEMORY, sizeof(NODE)); }

void Node_Kill(POS pos)
{ Mem_Free(pos); }

typedef struct tagList
{
	POS head;
	POS tail;
	int num;
} T_LIST, *TLIST;


TOLEXTC_END


#define _TOLC_TLIST_DEFINED
#include <tolc_list.h>


TOLEXTC_BEGIN


TOLCDECL TLIST TOLAPI List_Make()
{ return Mem_Alloc(MEM_ZERO_MEMORY, sizeof(T_LIST)); }

TOLCDECL void TOLAPI List_Kill(TLIST tList)
{
	if(tList)
	{
		List_RemoveAll(tList);
		Mem_Free(tList);
	}
}

TOLCDECL void TOLAPI List_KillCallback(TLIST tList, PFNTLISTKILLITEM pFn, LPARAM param)
{
	if(tList && pFn)
	{
		List_RemoveAllCallback(tList, pFn, 0);
		Mem_Free(tList);
	}
}

TOLCDECL int TOLAPI List_Num(TLIST tList)
{ return tList?tList->num:0; }

TOLCDECL LDATA TOLAPI List_At(TLIST tList, POS pos)
{ return (tList && pos)?pos->data:0; }

TOLCDECL LDATA TOLAPI List_SetAt(TLIST tList, POS pos, LDATA data)
{
	LDATA old=0;

	if(tList && pos)
	{
		old=pos->data;
		pos->data=data;
	}

	return old;
}

TOLCDECL POS TOLAPI List_Head(TLIST tList)
{ return tList?tList->head:NULL; }

TOLCDECL POS TOLAPI List_Tail(TLIST tList)
{ return tList?tList->tail:NULL; }

TOLCDECL POS TOLAPI List_Next(TLIST tList, POS pos)
{ return (tList && pos)?pos->next:NULL; }

TOLCDECL POS TOLAPI List_Prev(TLIST tList, POS pos)
{ return (tList && pos)?pos->prev:NULL; }

TOLCDECL BOOL TOLAPI List_AddHead(TLIST tList, LDATA data)
{
	if(tList)
	{
		POS pos=Node_Make();
		if(pos)
		{
			pos->data=data;

			if(tList->head)
			{
				pos->next=tList->head;
				tList->head->prev=pos;
			}
			else
				tList->tail=pos;

			tList->head=pos;
			tList->num++;
			return TRUE;
		}
	}

	return FALSE;
}

TOLCDECL BOOL TOLAPI List_AddTail(TLIST tList, LDATA data)
{
	if(tList)
	{
		POS pos=Node_Make();
		if(pos)
		{
			pos->data=data;

			if(tList->tail)
			{
				pos->prev=tList->tail;
				tList->tail->next=pos;
			}
			else
				tList->head=pos;

			tList->tail=pos;
			tList->num++;
			return TRUE;
		}
	}

	return FALSE;
}

TOLCDECL BOOL TOLAPI List_InsertBefore(TLIST tList, POS pos, LDATA data)
{
	if(tList && pos)
	{
		if(pos==tList->head)
			return List_AddHead(tList, data);
		else
		{
			POS one=Node_Make();
			if(one)
			{
				one->data=data;

				one->prev=pos->prev;
				one->next=pos;
				pos->prev=one;

				tList->num++;
				return TRUE;
			}
		}
	}

	return FALSE;
}

TOLCDECL BOOL TOLAPI List_InsertAfter(TLIST tList, POS pos, LDATA data)
{
	if(tList && pos)
	{
		if(pos==tList->tail)
			return List_AddTail(tList, data);
		else
		{
			POS one=Node_Make();
			if(one)
			{
				one->data=data;

				one->next=pos->next;
				one->prev=pos;
				pos->next=one;

				tList->num++;
				return TRUE;
			}
		}
	}

	return FALSE;
}

TOLCDECL LDATA TOLAPI List_RemoveHead(TLIST tList)
{
	if(tList && tList->head)
	{
		POS pos=tList->head;
		LDATA data=pos->data;

		tList->head=tList->head->next;
		if(tList->head)
			tList->head->prev=NULL;
		else
			tList->tail=NULL;

		tList->num--;
		Node_Kill(pos);
		return data;
	}

	return 0;
}

TOLCDECL LDATA TOLAPI List_RemoveTail(TLIST tList)
{
	if(tList && tList->tail)
	{
		POS pos=tList->tail;
		LDATA data=pos->data;

		tList->tail=tList->tail->prev;
		if(tList->tail)
			tList->tail->next=NULL;
		else
			tList->head=NULL;

		tList->num--;
		Node_Kill(pos);
		return data;
	}

	return 0;
}


TOLCDECL LDATA TOLAPI List_Remove(TLIST tList, POS pos)
{
	if(tList && pos)
	{
		if(pos==tList->head)
			return List_RemoveHead(tList);
		else if(pos==tList->tail)
			return List_RemoveTail(tList);
		else
		{
			LDATA data=pos->data;

			pos->prev->next=pos->next;
			pos->next->prev=pos->prev;

			tList->num--;

			Node_Kill(pos);
			return data;
		}
	}

	return 0;
}

TOLCDECL void TOLAPI List_RemoveAll(TLIST tList)
{
	if(tList)
	{
		POS next, pos=tList->head;
		while(pos)
		{
			next=pos->next;
			Node_Kill(pos);
			pos=next;
		}

		tList->head=tList->tail=NULL;
		tList->num=0;
	}
}

TOLCDECL POS TOLAPI List_Index(TLIST tList, int i)
{
	if(tList && i>=0 && i<tList->num)
	{
		POS pos=tList->head;
		while(pos && i--)
			pos=pos->next;

		return pos;
	}

	return NULL;
}

TOLCDECL POS TOLAPI List_Find(TLIST tList, LDATA data)
{
	if(tList)
	{
		POS pos;
		for(pos=tList->head; pos; pos=pos->next)
			if(pos->data==data)
				return pos;
	}

	return NULL;
}

TOLCDECL POS TOLAPI List_Enum(TLIST tList, PFNTLISTENUM pFn, LPARAM param)
{
	if(tList && pFn)
	{
		POS pos;
		for(pos=tList->head; pos; pos=pos->next)
			if(pFn(pos->data, param))
				return pos;
	}

	return FALSE;
}

TOLCDECL POS TOLAPI List_EnumRev(TLIST tList, PFNTLISTENUM pFn, LPARAM param)
{
	if(tList && pFn)
	{
		POS pos;
		for(pos=tList->tail; pos; pos=pos->prev)
			if(pFn(pos->data, param))
				return pos;
	}

	return FALSE;
}

TOLCDECL void TOLAPI List_Swap(TLIST tList, POS pos1, POS pos2)
{
	if(tList && pos1 && pos2)
	{
		LDATA data=pos1->data;
		pos1->data=pos2->data;
		pos2->data=data;
	}
}

TOLCDECL void TOLAPI List_Sort(TLIST tList, PFNTLISTSORT pFn)
{
	if(tList && pFn && tList->num>1)
	{
		LDATA data;
		POS pos1, pos2;

		for(pos1=tList->head; pos1; pos1=pos1->next)
			for(pos2=pos1->next; pos2; pos2=pos2->next)
				if(pFn(pos1->data, pos2->data)>0)
				{
					data=pos1->data;
					pos1->data=pos2->data;
					pos2->data=data;
				}
	}
}


TOLCDECL void TOLAPI List_RemoveHeadCallback(TLIST tList, PFNTLISTKILLITEM pFn, LPARAM param)
{
	if(tList && pFn)
		pFn(List_RemoveHead(tList), param);
}

TOLCDECL void TOLAPI List_RemoveTailCallback(TLIST tList, PFNTLISTKILLITEM pFn, LPARAM param)
{
	if(tList && pFn)
		pFn(List_RemoveTail(tList), param);
}

TOLCDECL void TOLAPI List_RemoveCallback(TLIST tList, POS pos, PFNTLISTKILLITEM pFn, LPARAM param)
{
	if(tList && pos && pFn)
		pFn(List_Remove(tList, pos), param);
}

TOLCDECL void TOLAPI List_RemoveAllCallback(TLIST tList, PFNTLISTKILLITEM pFn, LPARAM param)
{
	if(tList && pFn)
	{
		POS next, pos=tList->head;
		while(pos)
		{
			next=pos->next;
			pFn(pos->data, param);
			Node_Kill(pos);
			pos=next;
		}

		tList->head=tList->tail=NULL;
		tList->num=0;
	}
}


TOLEXTC_END
