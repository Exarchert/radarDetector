/*************************************************************************
File:			CXMLNode.h
Author:			��ƽ    
Date:			2009-11-4
Description:    CXMLNode����.�ṩ�й���XML�����Ľӿ�

*************************************************************************/
#pragma once
#include <COM\Export.h>
#include <Osg\Referenced>

#import   "msxml3.dll"

namespace COM{

class COM_EXPORT CXMLNode
{
public:
	CXMLNode(void);
	virtual ~CXMLNode(void);

	/*
	ͨ��XML���ݽ������ݳ�ʼ��
	*/
	bool InitByString(CString strXML);
	virtual bool Init(MSXML2::IXMLDOMNodePtr pNode){return false;}

	/*
	���XML����
	*/
	CString GetXMLString();
	virtual MSXML2::IXMLDOMNodePtr GetXMLNode(){return NULL;}

	/*
	���XML����
	*/
	bool Load(CString strFile);

	/*
	���XML����
	*/
	bool SaveAs(CString strFile);

};

}