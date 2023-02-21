/************************************************************************
Copyright (C)  
文件：XmlParser.h XmlParser.cpp
作者：黄志鹏
版本：1.0
日期：2009-11-24
描述：XML解析器,封装了XML的解析过程,提供以“路径”为索引的访问方式.
	功能有：
	1. 数据结点的文本访问\设置;
	2. 数据结点的属性访问\设置;
	3. 生成XML字符串.

	结点路径格式为"\XXX\xxxx\aaaa"
修改记录：
作者					日期				描述
----------------------------------------------------------------------
黄志鹏				2009-12-17			新增成员函数: IsExist(...)

************************************************************************/

#pragma once

#import <msxml3.dll>

#include <Com\Export.h>
#include <map>
#include <list>
#include <vector>
#include <osg\ref_ptr>
#include <osg\Referenced>


namespace COM	{

class COM_EXPORT CXmlParser
{
public:
	CXmlParser();
	virtual ~CXmlParser();

	/*
	目的：初始化
	参数：xml	(in)	xml字符串
	返回：成功返回true,失败返回false.
	*/
	bool Init(CString &xml);
	
	/*
	目的：清空
	*/
	void Clear();

	/*
	目的：获取某结点的某个属性
	参数：path		(in)	结点全路径
	      name		(in)	属性名称
	返回：属性值
	*/
	CString GetNodeAttribute(const CString &path, const CString &name);

	/*
	目的：获取某结点的文本内容
	参数：path		(in)	结点的全路径
	返回：文本内容
	*/
	CString GetNodeText(const CString &path);

	/*
	目的：判断某结点是否存在
	参数: path		(in)	结点的全路径
	返回：存放返回true, 否则返回false
	*/
	bool IsExist(const CString &path);

	/*
	目的：设置某结点的某属性
	参数：path		(in)	结点的全路径
	      name		(in)	属性名称
		  value		(in)	属性值
	返回：成功返回true,否则返回false
	备注：如果不存在该结点，会自动添加该结点.
	*/
	bool SetNodeAttribute(const CString &path, const CString &name, const CString &value);

	/*
	目的：设置某结点的文本内容
	参数：path		(in)	结点的全路径
	      text		(in)	文本内容
	返回：成功返回true,否则返回false
	备注：如果不存在该结点，会自动添加该结点.
	*/
	bool SetNodeText(const CString &path, const CString &text);

	/*
	目的：导出XML字符串
	*/
	CString GetXml();

protected:
	typedef std::map<CString, CString> StrToStrMap;

	class CNodeInfo : public osg::Referenced
	{
	public:
		CNodeInfo() : m_parent(NULL), m_firstChild(NULL), m_rSibling(NULL) {}

	public:
		StrToStrMap m_attris;
		CString		m_text;
		CString		m_name;
		CNodeInfo*	m_parent;
		osg::ref_ptr<CNodeInfo>	m_firstChild;
		osg::ref_ptr<CNodeInfo>	m_rSibling;
	};

	//递归解析XML树上的数据
	osg::ref_ptr<CNodeInfo> Parser(CNodeInfo *parent, MSXML2::IXMLDOMNodePtr pNode);

	//拆分路径
	void SplitPath(const CString &path, std::vector<CString> &names);

	/*
	目的：根据路径查找结点
	返回：成功返回结点指针，失败返回NULL
	*/
	CNodeInfo *Find(std::vector<CString> &names);

	/*
	目的：根据路径查找结点
	参数：names		(in)		路径名序列
	      index		(out)		>=index的名字结点不存在
	返回：返回找到的结点指针，失败返回NULL
	*/
	CNodeInfo *Find(std::vector<CString> &names, size_t &index);

	/*
	目的：创建某个结点，如果结点存在则不创建，否则创建.
	参数：path		(in)		路径名
	      pNode		(out)		目标结点指针
	返回：成功返回true(pNode有效)，否则返回false(pNode无效)
	*/
	bool Create(const CString &path, CNodeInfo *&pNode);

	/*
	目的：生成MsNode结点
	参数：pParent		(in)		NodeInfo父结点
	      pMsParent		(in)		MsNode父结点
		  pDoc			(in)		MsDoc结点
	返回: 无
	*/
	void GenerateMsNode(CNodeInfo *pRoot, MSXML2::IXMLDOMNodePtr pMsParent, MSXML2::IXMLDOMDocumentPtr pDoc) const;

protected:
	osg::ref_ptr<CNodeInfo>	m_root;
};

class COM_EXPORT CXmlParserEx
{

public:

	//一条道路信息
	typedef struct STURoadInfo
	{
	public:
		//ROADID='223' WIDTH='1' PARTSNUM='3' SCHEME='1方案名称'
		std::map<CString, CString> m_mapAttr;////道路ID,//道路半径,//道路段数,//方案名称
		CString m_strPoint;//一条道路所有段数---一段点完了后接风险等级37.84545 112.5764 1.824102,37.84544 112.576 1.623652,37.84544 112.5759 1.741274;1|37.84544 112.576 1.623652,37.84544 112.5759 1.741274;1

	}RoadInfo;
	CXmlParserEx();
	virtual ~CXmlParserEx();
	bool Init(CString &xml);
	bool InitEx(CString &xml);
	/*
	目的：获取某结点的某个属性
	参数：
	      name		(in)	属性名称
	返回：属性值
	*/
	CString GetNodeAttribute(const CString &name);
	//获得所有道路
	std::list<STURoadInfo> GetListStuRoadInfo();
private:
	std::map<CString, CString> m_mapAttr;//分析的基本属性-颜色
	std::list<STURoadInfo> m_listStu;

};

}