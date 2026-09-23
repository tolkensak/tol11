
#ifndef __INC_TOLXML_DOC_INL
#define __INC_TOLXML_DOC_INL

#if defined(_MSC_VER) && (_MSC_VER>=1020)
#pragma once
#endif


TOLNS_XML_BEGIN


TOLINL
BOOL XmlDoc::IsValid() const
{
	return m_docPtr!=NULL;
}

TOLINL
MSXML2::IXMLDOMDocument2Ptr XmlDoc::GetDocPtr()
{
	return m_docPtr;
}


TOLNS_XML_END

#endif // __INC_TOLXML_DOC_INL
