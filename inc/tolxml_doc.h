
#ifndef _INC_TOLXML_DOC_H
#define _INC_TOLXML_DOC_H

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif

#include <tolxml.h>
#include <tolcpp_object.h>


TOLNS_XML_BEGIN


class TOLXMLDECL XmlDoc : public Object
{
public:
	XmlDoc();
	virtual ~XmlDoc();

	BOOL IsValid() const;
	MSXML2::IXMLDOMDocument2Ptr GetDocPtr();

	BOOL Open(LPCTSTR pcFile, BOOL bValidateOnParse=FALSE);

	void Close();

protected:
	MSXML2::IXMLDOMDocument2Ptr m_docPtr;
};


TOLNS_XML_END

#include <tolxml_doc.inl>

#endif /* _INC_TOLXML_DOC_H */
