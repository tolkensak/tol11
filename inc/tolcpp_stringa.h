
#ifndef _INC_TOLCPP_STRINGA_H
#define _INC_TOLCPP_STRINGA_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <tolcpp_object.h>

TOLNS_BEGIN


class TOLCPPDECL StringA : public Object
{
public:
	explicit StringA(int nBufferLen=0);
	StringA(const StringA& str, int nCount=-1);
	StringA(PCCharA pc, int nCount=-1);
	StringA(PCCharW pc, int nCount=-1, Uint uCodePage=0);
	StringA(CharA ch, int nCount=1);
	StringA(CharW ch, int nCount=1, Uint uCodePage=0);
	virtual ~StringA();

	int Len() const;
	Bool IsEmpty() const;
	void Empty();

	PCharA Buffer(int nBufferLen=-1);
	void ReleaseBuffer(int nLen=-1);
	void Shrink();

	operator PCCharA() const;
	CharA& operator[](int nIndex);
	CharA operator[](int nIndex) const;

	void Copy(const StringA& str, int nCount=-1);
	void Copy(PCCharA pc, int nCount=-1);
	void Copy(PCCharW pc, int nCount=-1, Uint uCodePage=0);
	void Copy(CharA ch, int nCount=1);
	void Copy(CharW ch, int nCount=1, Uint uCodePage=0);

	StringA& operator=(const StringA& str);
	StringA& operator=(PCCharA pc);
	StringA& operator=(PCCharW pc);
	StringA& operator=(CharA ch);
	StringA& operator=(CharW ch);

	void Cat(const StringA& str, int nCount=-1);
	void Cat(PCCharA pc, int nCount=-1);
	void Cat(PCCharW pc, int nCount=-1, Uint uCodePage=0);
	void Cat(CharA ch, int nCount=1);
	void Cat(CharW ch, int nCount=1, Uint uCodePage=0);

	StringA& operator+=(const StringA& str);
	StringA& operator+=(PCCharA pc);
	StringA& operator+=(PCCharW pc);
	StringA& operator+=(CharA ch);
	StringA& operator+=(CharW ch);

	void Format(PCCharA pcFmt, ...);
	void FormatCat(PCCharA pcFmt, ...);

	void Trim(PCCharA pcTrim=" ");
	void TrimLeft(PCCharA pcTrim=" ");
	void TrimRight(PCCharA pcTrim=" ");

	int Find(PCCharA pcFind, int nCount=-1, int nStart=0);
	int Find(CharA chFind, int nStart=0);

	int Comapre(const StringA& str, int nCount=-1) const;
	int Comapre(PCCharA pc, int nCount=-1) const;

	StringA Token(PCCharA pcToken, int& nStart);
	BOOL EnvVar(PCCharA pcVarName);

	StringA Left(int nCount);
	StringA Right(int nCount);
	StringA Middle(int nIndex, int nCount);

	friend TOLCPPDECL Bool TOLAPI operator==(const StringA& str1, const StringA& str2);
	friend TOLCPPDECL Bool TOLAPI operator==(const StringA& str, PCCharA pc);
	friend TOLCPPDECL Bool TOLAPI operator==(PCCharA pc, const StringA& str);
	friend TOLCPPDECL Bool TOLAPI operator==(const StringA& str, PCCharW pc);
	friend TOLCPPDECL Bool TOLAPI operator==(PCCharW pc, const StringA& str);

	friend TOLCPPDECL Bool TOLAPI operator!=(const StringA& str1, const StringA& str2);
	friend TOLCPPDECL Bool TOLAPI operator!=(const StringA& str, PCCharA pc);
	friend TOLCPPDECL Bool TOLAPI operator!=(PCCharA pc, const StringA& str);
	friend TOLCPPDECL Bool TOLAPI operator!=(const StringA& str, PCCharW pc);
	friend TOLCPPDECL Bool TOLAPI operator!=(PCCharW pc, const StringA& str);

	//friend TOLCPPDECL StringA TOLAPI operator+(const StringA& str1, const StringA& str2);
	//friend TOLCPPDECL StringA TOLAPI operator+(const StringA& str, PCCharA pc);
	//friend TOLCPPDECL StringA TOLAPI operator+(PCCharA pc, const StringA& str);
	//friend TOLCPPDECL StringA TOLAPI operator+(const StringA& str, PCCharW pc);
	//friend TOLCPPDECL StringA TOLAPI operator+(PCCharW pc, const StringA& str);
	//friend TOLCPPDECL StringA TOLAPI operator+(const StringA& str, CharA ch);
	//friend TOLCPPDECL StringA TOLAPI operator+(CharA ch, const StringA& str);
	//friend TOLCPPDECL StringA TOLAPI operator+(const StringA& str, CharW ch);
	//friend TOLCPPDECL StringA TOLAPI operator+(CharW ch, const StringA& str);

protected:
	PCharA m_pc;
	int m_nLen;

	void SetBufferLen(int nLen);
	Bool FromW(PCCharW pcSrc, int nCount=-1, Uint uCodePage=0);
};


TOLNS_END

#include <tolcpp_stringa.inl>

#endif // _INC_TOLCPP_STRINGA_H
