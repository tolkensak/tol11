
#ifndef _INC_TOLCPP_TEXT_H
#define _INC_TOLCPP_TEXT_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <tolcpp_string.h>

TOLNS_BEGIN


////////////////////////////////////////////////////////////////////////
// Text

class TOLCPPDECL Text : public Object
{
//private:
//	Text(const Text&);
//	void operator=(const Text&);

public:
	enum Encoding
	{
		ENC_UTF8,
		ENC_UTF8SIG,
		ENC_UTF16LE,
		ENC_UTF16BE,
#ifdef _UTF32_SUPPORT
		ENC_UTF32LE,
		ENC_UTF32BE,
#endif
		EncodingCount
	};

	typedef struct CodePage
	{
		Uint uCP;
		int cbBOM;
		Byte BOM[4];
	} *PCodePage;

	virtual ~Text();
	virtual Ulong Open(PCChar pcFile)=0;
	virtual void Close();

	PVoid GetFile();

	Encoding GetEncoding() const;
	Bool SetEncoding(Encoding e);

	static const CodePage m_cp[EncodingCount];

protected:
	Text(Encoding enc=ENC_UTF8);

	PVoid m_pFile;
	Encoding m_enc;
};


////////////////////////////////////////////////////////////////////////
// TextReader

class TOLCPPDECL TextReader : public Text
{
//private:
//	TextReader(const TextReader&);
//	void operator=(const TextReader&);

public:
	class TOLCPPDECL View : public Object
	{
	public:
		View(PCChar pcName=NULL);
		virtual ~View();

		Bool Open(PVoid pFile);
		virtual void Close();

		PByte GetBuffer();
		PByte GetBuffer(Encoding enc);

		Ulong GetOffset() const;
		void SetOffset(Ulong ulOffset);
		void AddOffset(Ulong ulOffset);

	protected:
		PVoid m_pMap;
		String m_strName;
		PVoid m_pBuffer;
		Ulong m_ulOffset;
	};

	TextReader(Encoding enc=ENC_UTF8);
	virtual ~TextReader();

	virtual Ulong Open(PCChar pcFile);
	virtual void Close();

	Bool ReadLine(StringA& str);
	Bool ReadLine(StringW& str);

	PByte GetBuffer();

protected:
	void ReadEnd(PCharA pc, PByte pby);
	void ReadEnd(PCharW pc, PByte pby);
	void ReadBOM();

	View m_view;
};


////////////////////////////////////////////////////////////////////////
// TextWriter

class TOLCPPDECL TextWriter : public Text
{
//private:
//	TextWriter(const TextWriter&);
//	void operator=(const TextWriter&);

public:
	enum EndStyle
	{
		ES_WINDOWS, // \r\n
		ES_UNIX,    // \n
		ES_MAC,     // \r
		EndStyleNum
	};

	TextWriter(Encoding enc=ENC_UTF8, EndStyle es=ES_WINDOWS);
	virtual ~TextWriter();

	virtual Ulong Open(PCChar pcFile);
	virtual void Close();

	 // cchWrite not include terminating null
	Bool Write(PCCharA pcWrite, int cchWrite=-1);
	Bool Write(PCCharW pcWrite, int cchWrite=-1);

	Bool WriteLine(PCCharA pcWrite, int cchWrite=-1);
	Bool WriteLine(PCCharW pcWrite, int cchWrite=-1);

	EndStyle GetEndStyle() const;
	void SetEndStyle(EndStyle es);

protected:
	void WriteBOM();

	EndStyle m_es;
};


TOLNS_END

#include "tolcpp_text.inl"

#endif // _INC_TOLCPP_TEXT_H
