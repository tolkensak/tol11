
#ifndef _INC_TOLCPP_STRINGW_H
#define _INC_TOLCPP_STRINGW_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <tolcpp_object.h>

TOLNS_BEGIN


class TOLCPPDECL StringW : public Object
{
public:
	explicit StringW(int nBufferLen=0);
	StringW(const StringW& str, int nCount=-1);
	StringW(PCCharW pc, int nCount=-1);
	StringW(PCCharA pc, int nCount=-1, Uint uCodePage=0);
	StringW(CharW ch, int nCount=1);
	StringW(CharA ch, int nCount=1, Uint uCodePage=0);
	virtual ~StringW();

	int Len() const;
	Bool IsEmpty() const;
	void Empty();

	PCharW Buffer(int nBufferLen=-1);
	void ReleaseBuffer(int nLen=-1);
	void Shrink();

	operator PCCharW() const;
	CharW& operator[](int nIndex);
	CharW operator[](int nIndex) const;

	void Copy(const StringW& str, int nCount=-1);
	void Copy(PCCharW pc, int nCount=-1);
	void Copy(PCCharA pc, int nCount=-1, Uint uCodePage=0);
	void Copy(CharW ch, int nCount=1);
	void Copy(CharA ch, int nCount=1, Uint uCodePage=0);

	StringW& operator=(const StringW& str);
	StringW& operator=(PCCharW pc);
	StringW& operator=(PCCharA pc);
	StringW& operator=(CharW ch);
	StringW& operator=(CharA ch);

	void Cat(const StringW& str, int nCount=-1);
	void Cat(PCCharW pc, int nCount=-1);
	void Cat(PCCharA pc, int nCount=-1, Uint uCodePage=0);
	void Cat(CharW ch, int nCount=1);
	void Cat(CharA ch, int nCount=1, Uint uCodePage=0);

	StringW& operator+=(const StringW& str);
	StringW& operator+=(PCCharW pc);
	StringW& operator+=(PCCharA pc);
	StringW& operator+=(CharW ch);
	StringW& operator+=(CharA ch);

	void Format(PCCharW pcFmt, ...);
	void FormatCat(PCCharW pcFmt, ...);

	void Trim(PCCharW pcTrim=L" ");
	void TrimLeft(PCCharW pcTrim=L" ");
	void TrimRight(PCCharW pcTrim=L" ");

	int Find(PCCharW pcFind, int nCount=-1, int nStart=0);
	int Find(CharW chFind, int nStart=0);

	int Comapre(const StringW& str, int nCount=-1) const;
	int Comapre(PCCharW pc, int nCount=-1) const;

	StringW Token(PCCharW pcToken, int& nStart);
	BOOL EnvVar(PCCharW pcVarName);

	void ChangeEndian();

	StringW Left(int nCount);
	StringW Right(int nCount);
	StringW Middle(int nIndex, int nCount);
	
	friend TOLCPPDECL Bool TOLAPI operator==(const StringW& str1, const StringW& str2);
	friend TOLCPPDECL Bool TOLAPI operator==(const StringW& str, PCCharW pc);
	friend TOLCPPDECL Bool TOLAPI operator==(PCCharW pc, const StringW& str);
	friend TOLCPPDECL Bool TOLAPI operator==(const StringW& str, PCCharA pc);
	friend TOLCPPDECL Bool TOLAPI operator==(PCCharA pc, const StringW& str);

	friend TOLCPPDECL Bool TOLAPI operator!=(const StringW& str1, const StringW& str2);
	friend TOLCPPDECL Bool TOLAPI operator!=(const StringW& str, PCCharW pc);
	friend TOLCPPDECL Bool TOLAPI operator!=(PCCharW pc, const StringW& str);
	friend TOLCPPDECL Bool TOLAPI operator!=(const StringW& str, PCCharA pc);
	friend TOLCPPDECL Bool TOLAPI operator!=(PCCharA pc, const StringW& str);

	//friend TOLCPPDECL StringW TOLAPI operator+(const StringW& str1, const StringW& str2);
	//friend TOLCPPDECL StringW TOLAPI operator+(const StringW& str, PCCharW pc);
	//friend TOLCPPDECL StringW TOLAPI operator+(PCCharW pc, const StringW& str);
	//friend TOLCPPDECL StringW TOLAPI operator+(const StringW& str, PCCharA pc);
	//friend TOLCPPDECL StringW TOLAPI operator+(PCCharA pc, const StringW& str);
	//friend TOLCPPDECL StringW TOLAPI operator+(const StringW& str, CharW ch);
	//friend TOLCPPDECL StringW TOLAPI operator+(CharW ch, const StringW& str);
	//friend TOLCPPDECL StringW TOLAPI operator+(const StringW& str, CharA ch);
	//friend TOLCPPDECL StringW TOLAPI operator+(CharA ch, const StringW& str);

protected:
	PCharW m_pc;
	int m_nLen;

	void SetBufferLen(int nLen);
	Bool FromA(PCCharA pcSrc, int nCount=-1, Uint uCodePage=0);
};


TOLNS_END

#include <tolcpp_stringw.inl>

#endif // _INC_TOLCPP_STRINGW_H
