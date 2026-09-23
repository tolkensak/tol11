
#include "stdafx.h"
#include <tolxml_doc.h>
#include <tolxml_util.h>


TOLNS_XML_BEGIN


XmlDoc::XmlDoc()
	: m_docPtr(NULL)
{
}

XmlDoc::~XmlDoc()
{
	Close();
}

BOOL XmlDoc::Open(LPCTSTR pcFile, BOOL bValidateOnParse)
{
	m_docPtr=XML_Open(pcFile, bValidateOnParse);
	return m_docPtr!=NULL;
}

void XmlDoc::Close()
{
	if(m_docPtr)
	{
		m_docPtr.Release();
		m_docPtr=NULL;
	}
}


TOLNS_XML_END
