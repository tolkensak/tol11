
#include "stdafx.h"
#include <tolxml_util.h>
#include <tolxml_doc.h>

TOLNS_XML_BEGIN


void TOLAPI TESTHR(HRESULT hr)
{
	if FAILED(hr)
		_com_issue_error(hr);
}

void TOLAPI ComError(_com_error &e)
{
	CString str(_T("Error:\n"));
	str.AppendFormat(_T("Code = %08lx\n"), e.Error());
	str.AppendFormat(_T("Code meaning = %s\n"), (LPTSTR)e.ErrorMessage());
	str.AppendFormat(_T("Source = %s\n"), (LPTSTR)e.Source());
	str.AppendFormat(_T("Error Description = %s\n"), (LPTSTR)e.Description());
	AfxMessageBox(str);
}

TOLXMLDECL MSXML2::IXMLDOMDocument2Ptr /*TOLAPI*/ XML_Open(LPCTSTR pcFile, BOOL bValidateOnParse)
{
	try
	{
		MSXML2::IXMLDOMDocument2Ptr docPtr=NULL;
		TESTHR(docPtr.CreateInstance("Msxml2.DOMDocument.4.0"));

		_variant_t varXml(pcFile);
		_variant_t varOut((bool)TRUE);

		docPtr->validateOnParse=(VARIANT_BOOL)bValidateOnParse;
		docPtr->async=VARIANT_FALSE;

		varOut=docPtr->load(varXml);

		if((bool)varOut==TRUE)
		{
			//docPtr->setProperty("SelectionLanguage", "XPath");
			//docPtr->setProperty("SelectionNamespaces", "xmlns=\"urn:map.tolken.kz/Layer\"");
			return docPtr;
		}

		MSXML2::IXMLDOMParseErrorPtr errPtr=docPtr->parseError;
		_bstr_t bstrErr(errPtr->reason);

		CString str(_T("Error:\n"));
		str.AppendFormat(_T("Code = 0x%x\n"), errPtr->errorCode);
		str.AppendFormat(_T("Source = Line : %ld; Char : %ld\n"), errPtr->line, errPtr->linepos);
		str.AppendFormat(_T("Error Description = %s\n"), (LPTSTR)bstrErr);
		AfxMessageBox(str);
	}
	catch(_com_error &e)
	{
		ComError(e);
	}
	catch(...)
	{
		CString str;
		str.Format(_T("Can not open file \"%s\""), pcFile);
		AfxMessageBox(str);
	}

	return NULL;
}

//BOOL PrepareXslToTransform(XmlDoc& xslDoc, LPCTSTR pcCssDir)
//{
//	MSXML2::IXMLDOMDocument2Ptr docPtr=xslDoc.GetDocPtr();
//	if(!docPtr)
//		return FALSE;
//
//	docPtr->setProperty((bstr_t)"SelectionLanguage", (_variant_t)"XPath");
//	docPtr->setProperty((bstr_t)"SelectionNamespaces", (_variant_t)"xmlns:htm=\"http://www.w3.org/1999/xhtml\"");
//
//	MSXML2::IXMLDOMNodePtr head=docPtr->selectSingleNode((_bstr_t)"//htm:html/htm:head");
//	if(!head)
//		return TRUE;
//
//	//_variant_t var;
//	//var.vt=VT_DISPATCH;
//
//	//MSXML2::IXMLDOMElementPtr metaAuth=docPtr->createElement((_bstr_t)"meta");
//	//metaAuth->setAttribute((_bstr_t)"Application", (_bstr_t)"Transformer");
//	//var.pdispVal=head->GetchildNodes()->Getitem(1);
//	//head->insertBefore(metaAuth, var);
//
//	//MSXML2::IXMLDOMElementPtr metaApp=docPtr->createElement((_bstr_t)"meta");
//	//metaApp->setAttribute((_bstr_t)"Author", (_bstr_t)"Tolken Ahmetolla");
//	//var.pdispVal=head->firstChild;
//	//head->insertBefore(metaApp, var);
//
//	MSXML2::IXMLDOMElementPtr link;
//	MSXML2::IXMLDOMElementPtr style;
//
//	TCHAR pchCssFile[MAX_PATH];
//
//	CStdioFile file;
//	CString str;
//	StringA stra;
//
//	while(1)
//	{
//		link=head->selectSingleNode((_bstr_t)"htm:link[@rel=\"stylesheet\"]");
//		if(!link)
//			break;
//
//		style=docPtr->createNode(NODE_ELEMENT, (_bstr_t)"style", (_bstr_t)"http://www.w3.org/1999/xhtml");
//		if(!style)
//			continue;
//
//		style->setAttribute((_bstr_t)"type", link->getAttribute((_bstr_t)"type"));
//		style->setAttribute((_bstr_t)"media", link->getAttribute((_bstr_t)"media"));
//
//		PathCombine(pchCssFile, pcCssDir, (LPCTSTR)link->getAttribute((_bstr_t)"href").pcVal);
//		if(file.Open(pchCssFile, CFile::modeRead))
//		{
//			stra.Empty();
//
//			while(file.ReadString(str))
//			{
//				stra+="\n";
//				stra.Cat(str, str.GetLength(), CP_UTF8);
//			}
//
//			stra+="\n";
//
//			style->text=(_bstr_t)stra;
//			head->replaceChild(style, link);
//			file.Close();
//		}
//	}
//
//	return TRUE;
//}

void IncludeCss(MSXML2::IXMLDOMDocument2Ptr docPtr, LPCTSTR pcCssDir)
{
	if(!docPtr)
		return;

	docPtr->setProperty((bstr_t)"SelectionLanguage", (_variant_t)"XPath");
	docPtr->setProperty((bstr_t)"SelectionNamespaces", (_variant_t)"xmlns:htm=\"http://www.w3.org/1999/xhtml\"");

	MSXML2::IXMLDOMNodePtr nodeHead=docPtr->selectSingleNode((_bstr_t)"//htm:html/htm:head");
	if(!nodeHead)
		return;

	MSXML2::IXMLDOMNodeListPtr listLink=nodeHead->selectNodes((_bstr_t)"htm:link[@rel=\"stylesheet\"]");
	if(!listLink)
		return;

	if(!listLink->length)
		return;

	//_variant_t var;
	//var.vt=VT_DISPATCH;

	//MSXML2::IXMLDOMElementPtr metaAuth=docPtr->createElement((_bstr_t)"meta");
	//metaAuth->setAttribute((_bstr_t)"Application", (_bstr_t)"Transformer");
	//var.pdispVal=head->GetchildNodes()->Getitem(1);
	//head->insertBefore(metaAuth, var);

	//MSXML2::IXMLDOMElementPtr metaApp=docPtr->createElement((_bstr_t)"meta");
	//metaApp->setAttribute((_bstr_t)"Author", (_bstr_t)"Tolken Ahmetolla");
	//var.pdispVal=head->firstChild;
	//head->insertBefore(metaApp, var);

	StringA stra;
	CString cstr;
	CStdioFile file;
	TCHAR pchCssFile[MAX_PATH];

	MSXML2::IXMLDOMElementPtr elmLink;
	MSXML2::IXMLDOMElementPtr elmStyle;
	MSXML2::IXMLDOMCommentPtr script;
	//MSXML2::IXMLDOMCDATASectionPtr script;

	for(int i=0; i<listLink->length; i++)
	{
		elmLink=listLink->item[i];
		if(!elmLink)
			continue;

		_variant_t var;

		var=elmLink->getAttribute((_bstr_t)"href");
		if(var.vt!=VT_BSTR)
			continue;

		cstr=(LPCTSTR)var.bstrVal;
		if(cstr.Find(_T(':'))==-1)
		{
			cstr.Replace(_T('/'), _T('\\'));
			PathCombine(pchCssFile, pcCssDir, cstr);
		}
		else if(cstr.Find(_T("file:"))==0)
		{
			cstr.Replace(_T("file:"), _T(""));
			cstr.TrimLeft(_T('/'));
			cstr.Replace(_T('/'), _T('\\'));
			lstrcpy(pchCssFile, cstr);
		}
		else
			continue;

		elmStyle=docPtr->createNode(NODE_ELEMENT, (_bstr_t)"style", (_bstr_t)"http://www.w3.org/1999/xhtml");
		if(!elmStyle)
			continue;

		var=elmLink->getAttribute((_bstr_t)"type");
		if(var.vt==VT_BSTR)
			elmStyle->setAttribute((_bstr_t)"type", var);

		var=elmLink->getAttribute((_bstr_t)"media");
		if(var.vt==VT_BSTR)
			elmStyle->setAttribute((_bstr_t)"media", var);

		if(!file.Open(pchCssFile, CFile::modeRead))
			continue;

		stra.Empty();

		while(file.ReadString(cstr))
		{
			if(cstr.IsEmpty())
				continue;

			//cstr.Replace(_T("&"), _T("$#38;"));
			//cstr.Replace(_T("<"), _T("$#60;"));
			//cstr.Replace(_T(">"), _T("$#62;"));
			//cstr.Replace(_T("'"), _T("$#39;"));
			//cstr.Replace(_T("\""), _T("$#34;"));

			stra+="\n";
			stra.Cat(cstr, cstr.GetLength(), CP_UTF8);
		}

		stra+="\n";

		script=docPtr->createComment((_bstr_t)stra);
		//script=docPtr->createCDATASection((_bstr_t)stra);
		elmStyle->appendChild(script);

		nodeHead->replaceChild(elmStyle, elmLink);
		file.Close();
	}
}

CString GetAttributeValue(const CString& cstr, const CString& cstrAttr)
{
	int i=cstr.Find(cstrAttr);
	if(i==-1)
		return _T("");

	int n=cstr.GetLength();
	for(i+=cstrAttr.GetLength(); i<n; i++)
		if(cstr[i]!=_T(' '))
			break;

	if(cstr[i]!=_T('='))
		return _T("");

	for(i++; i<n; i++)
		if(cstr[i]!=_T(' '))
			break;

	if(cstr[i]!=_T('\"') || cstr[i]==_T('\''))
		return _T("");

	TCHAR chQuote=cstr[i];
	TCHAR chPrev=_T('\0');
	int iBegin=i+1;
	int iEnd=0;

	for(i++; i<n; i++)
	{
		if(cstr[i]==chQuote && chPrev!=_T('\\'))
		{
			iEnd=i;
			break;
		}

		chPrev=cstr[i];
	}

	if(iEnd==0)
		return _T("");

	return cstr.Mid(iBegin, iEnd-iBegin);
}

CString GetOnBoardXsl(MSXML2::IXMLDOMDocument2Ptr docPtr)
{
	if(!docPtr)
		return _T("");

	MSXML2::IXMLDOMNodePtr node;

	for(int i=0; i<docPtr->childNodes->length; i++)
	{
		node=docPtr->childNodes->item[i];
		if(!node)
			continue;

		if(node->nodeType!=NODE_PROCESSING_INSTRUCTION)
			continue;

		if(lstrcmp(node->nodeName, _T("xml-stylesheet")))
			continue;

		if(GetAttributeValue(node->nodeValue, _T("type"))!=_T("text/xsl"))
			continue;

		return GetAttributeValue(node->nodeValue, _T("href"));
	}

	return _T("");
}

#define TRANSFORM_METHOD 2

TOLXMLDECL CString /*TOLAPI*/ XML_Transform(LPCTSTR pcXmlFile, LPCTSTR pcXslFile, LPCTSTR pcHtmlFile, BOOL bCssDirRelateiveToXsl)
{
	if(!pcXmlFile || !PathFileExists(pcXmlFile))
		return NULL;

	TCHAR pchHtml[MAX_PATH];
	if(pcHtmlFile==NULL || pcHtmlFile[0]==_T('\0'))
	{
		lstrcpy(pchHtml, pcXmlFile);
		PathRenameExtension(pchHtml, _T(".htm"));
		pcHtmlFile=pchHtml;
	}
	else if(PathIsRelative(pcHtmlFile))
	{
		TCHAR pchTmp[MAX_PATH];
		lstrcpy(pchTmp, pcXmlFile);
		PathRemoveFileSpec(pchTmp);
		PathCombine(pchHtml, pchTmp, pcHtmlFile);

		if(PathIsDirectory(pchHtml))
		{
			PathAppend(pchHtml, PathFindFileName(pcXmlFile));
			PathRenameExtension(pchHtml, _T(".htm"));
		}
		else
		{
			LPTSTR pcExt=PathFindExtension(pchHtml);
			if(*pcExt!=_T('\0')) // if a file
			{
				PathRemoveFileSpec(pchHtml);
				Folder_EnsureExists(pchHtml);
				PathAppend(pchHtml, pcExt);
			}
			else if(*(pcExt-1)==_T('\\'))
			{
				PathRemoveBackslash(pchHtml);
				Folder_EnsureExists(pchHtml);
				PathAppend(pchHtml, PathFindFileName(pcXmlFile));
				PathRenameExtension(pchHtml, _T(".htm"));
			}
			else
			{
				lstrcpy(pchTmp, PathFindFileName(pchHtml));
				PathAddExtension(pchTmp, _T(".htm"));
				PathRemoveFileSpec(pchHtml);
				Folder_EnsureExists(pchHtml);
				PathAppend(pchHtml, pchTmp);
			}
		}

		pcHtmlFile=pchHtml;
	}
	else
	{
		lstrcpy(pchHtml, pcHtmlFile);

		if(PathIsDirectory(pchHtml))
		{
			PathAppend(pchHtml, PathFindFileName(pcXmlFile));
			PathRenameExtension(pchHtml, _T(".htm"));
			pcHtmlFile=pchHtml;
		}
		else
		{
			LPTSTR pcExt=PathFindExtension(pchHtml);
			if(*pcExt!=_T('\0')) // if a file
			{
				PathRemoveFileSpec(pchHtml);
				Folder_EnsureExists(pchHtml);
			}
			else if(*(pcExt-1)==_T('\\'))
			{
				PathRemoveBackslash(pchHtml);
				Folder_EnsureExists(pchHtml);
				PathAppend(pchHtml, PathFindFileName(pcXmlFile));
				PathRenameExtension(pchHtml, _T(".htm"));
				pcHtmlFile=pchHtml;
			}
			else
			{
				TCHAR pchTmp[MAX_PATH];
				lstrcpy(pchTmp, PathFindFileName(pchHtml));
				PathAddExtension(pchTmp, _T(".htm"));
				PathRemoveFileSpec(pchHtml);
				Folder_EnsureExists(pchHtml);
				PathAppend(pchHtml, pchTmp);
				pcHtmlFile=pchHtml;
			}
		}
	}


	XmlDoc xmlDoc;
	if(!xmlDoc.Open(pcXmlFile))
		return NULL;

	TCHAR pchXsl[MAX_PATH];
	if(!pcXslFile || !PathFileExists(pcXslFile))
	{
		CString cstr=GetOnBoardXsl(xmlDoc.GetDocPtr());
		if(cstr.IsEmpty())
			return NULL;

		if(cstr.Find(_T(':'))==-1)
		{
			TCHAR pchTmp[MAX_PATH];
			lstrcpy(pchTmp, pcXmlFile);
			PathRemoveFileSpec(pchTmp);
			cstr.Replace(_T('/'), _T('\\'));
			PathCombine(pchXsl, pchTmp, cstr);
			pcXslFile=pchXsl;
		}
		else if(cstr.Find(_T("file:"))==0)
		{
			cstr.Replace(_T("file:"), _T(""));
			cstr.TrimLeft(_T('/'));
			cstr.Replace(_T('/'), _T('\\'));
			lstrcpy(pchXsl, cstr);
			pcXslFile=pchXsl;
		}
		else
			return NULL;
	}

	XmlDoc xslDoc;
	if(!xslDoc.Open(pcXslFile))
		return NULL;

#if TRANSFORM_METHOD == 1

	CFile file;
	if(!file.Open(pcHtmlFile, CFile::modeCreate|CFile::modeWrite))
		return NULL;

	StringA stra;
	stra.Copy((LPCTSTR)xmlDoc.GetDocPtr()->transformNode(xslDoc.GetDocPtr()), -1, CP_UTF8);
	file.Write(stra, stra.Len());
	file.Close();

#elif TRANSFORM_METHOD == 2

	MSXML2::IXMLDOMDocument2Ptr docPtr=NULL;
	TESTHR(docPtr.CreateInstance("Msxml2.DOMDocument.4.0"));
	docPtr->validateOnParse=VARIANT_FALSE;
	docPtr->async=VARIANT_FALSE;

	VARIANT vObject;
	vObject.vt=VT_DISPATCH;
	docPtr.QueryInterface(IID_IDispatch, (void**)&vObject.pdispVal);
	xmlDoc.GetDocPtr()->transformNodeToObject(xslDoc.GetDocPtr(), vObject);

	TCHAR pchCssDir[MAX_PATH];
	lstrcpy(pchCssDir, bCssDirRelateiveToXsl?pcXslFile:pcXmlFile);
	PathRemoveFileSpec(pchCssDir);
	IncludeCss(docPtr, pchCssDir);

	docPtr->save((bstr_t)pcHtmlFile);
	docPtr.Release();

#endif

	return pcHtmlFile;
}


TOLNS_XML_END
