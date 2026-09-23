
#ifndef _INC_TOLCPP_INI_H
#define _INC_TOLCPP_INI_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <tolcpp_string.h>
#include <tolcpp_array.h>
#include <tolcpp_map.h>

TOLNS_BEGIN


////////////////////////////////////////////////////////////////////////
//
// IniSetting

typedef void (*PFNINIENUM)(LPCTSTR pcKey, LPCTSTR pcVal, LPARAM lParam);

class TOLCPPDECL IniSetting : public Object
{
public:
	typedef Array<String> ValueArray;
	typedef Map<String, String> ValueMap;

	class Section
	{
	public:
		Section(){}
		virtual ~Section(){}

		ValueArray arrValue;
		ValueMap mapValue;
	};

	//typedef Pointer<Section> SectionPtr;
	typedef Map<String, Section*> SectionMap;

public:
	IniSetting(BOOL bForRead, LPCTSTR pcSeparator=NULL, LPCTSTR pcCommentMark=NULL);
	virtual ~IniSetting();

	String GetPath();

	String Read(LPCTSTR pcSec, LPCTSTR pcKey); 
	void Enum(LPCTSTR pcSec, PFNINIENUM pFn, LPARAM lParam);
	void Write(LPCTSTR pcSec, Section* pSec);

protected:
	BOOL m_bForRead;
	SectionMap m_mapSect;

	String m_strSeparator;
	String m_strCommentMark;
};


TOLNS_END

#endif // _INC_TOLCPP_INI_H
