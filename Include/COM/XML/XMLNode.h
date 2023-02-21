/*************************************************************************
File:			CXMLNode.h
Author:			张平    
Date:			2009-11-4
Description:    CXMLNode基类.提供有关于XML操作的接口

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
	通过XML数据进行数据初始化
	*/
	bool InitByString(CString strXML);
	virtual bool Init(MSXML2::IXMLDOMNodePtr pNode){return false;}

	/*
	获得XML数据
	*/
	CString GetXMLString();
	virtual MSXML2::IXMLDOMNodePtr GetXMLNode(){return NULL;}

	/*
	获得XML数据
	*/
	bool Load(CString strFile);

	/*
	获得XML数据
	*/
	bool SaveAs(CString strFile);

};

}