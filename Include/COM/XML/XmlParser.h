/************************************************************************
Copyright (C)  
�ļ���XmlParser.h XmlParser.cpp
���ߣ���־��
�汾��1.0
���ڣ�2009-11-24
������XML������,��װ��XML�Ľ�������,�ṩ�ԡ�·����Ϊ�����ķ��ʷ�ʽ.
	�����У�
	1. ���ݽ����ı�����\����;
	2. ���ݽ������Է���\����;
	3. ����XML�ַ���.

	���·����ʽΪ"\XXX\xxxx\aaaa"
�޸ļ�¼��
����					����				����
----------------------------------------------------------------------
��־��				2009-12-17			������Ա����: IsExist(...)

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
	Ŀ�ģ���ʼ��
	������xml	(in)	xml�ַ���
	���أ��ɹ�����true,ʧ�ܷ���false.
	*/
	bool Init(CString &xml);
	
	/*
	Ŀ�ģ����
	*/
	void Clear();

	/*
	Ŀ�ģ���ȡĳ����ĳ������
	������path		(in)	���ȫ·��
	      name		(in)	��������
	���أ�����ֵ
	*/
	CString GetNodeAttribute(const CString &path, const CString &name);

	/*
	Ŀ�ģ���ȡĳ�����ı�����
	������path		(in)	����ȫ·��
	���أ��ı�����
	*/
	CString GetNodeText(const CString &path);

	/*
	Ŀ�ģ��ж�ĳ����Ƿ����
	����: path		(in)	����ȫ·��
	���أ���ŷ���true, ���򷵻�false
	*/
	bool IsExist(const CString &path);

	/*
	Ŀ�ģ�����ĳ����ĳ����
	������path		(in)	����ȫ·��
	      name		(in)	��������
		  value		(in)	����ֵ
	���أ��ɹ�����true,���򷵻�false
	��ע����������ڸý�㣬���Զ���Ӹý��.
	*/
	bool SetNodeAttribute(const CString &path, const CString &name, const CString &value);

	/*
	Ŀ�ģ�����ĳ�����ı�����
	������path		(in)	����ȫ·��
	      text		(in)	�ı�����
	���أ��ɹ�����true,���򷵻�false
	��ע����������ڸý�㣬���Զ���Ӹý��.
	*/
	bool SetNodeText(const CString &path, const CString &text);

	/*
	Ŀ�ģ�����XML�ַ���
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

	//�ݹ����XML���ϵ�����
	osg::ref_ptr<CNodeInfo> Parser(CNodeInfo *parent, MSXML2::IXMLDOMNodePtr pNode);

	//���·��
	void SplitPath(const CString &path, std::vector<CString> &names);

	/*
	Ŀ�ģ�����·�����ҽ��
	���أ��ɹ����ؽ��ָ�룬ʧ�ܷ���NULL
	*/
	CNodeInfo *Find(std::vector<CString> &names);

	/*
	Ŀ�ģ�����·�����ҽ��
	������names		(in)		·��������
	      index		(out)		>=index�����ֽ�㲻����
	���أ������ҵ��Ľ��ָ�룬ʧ�ܷ���NULL
	*/
	CNodeInfo *Find(std::vector<CString> &names, size_t &index);

	/*
	Ŀ�ģ�����ĳ����㣬����������򲻴��������򴴽�.
	������path		(in)		·����
	      pNode		(out)		Ŀ����ָ��
	���أ��ɹ�����true(pNode��Ч)�����򷵻�false(pNode��Ч)
	*/
	bool Create(const CString &path, CNodeInfo *&pNode);

	/*
	Ŀ�ģ�����MsNode���
	������pParent		(in)		NodeInfo�����
	      pMsParent		(in)		MsNode�����
		  pDoc			(in)		MsDoc���
	����: ��
	*/
	void GenerateMsNode(CNodeInfo *pRoot, MSXML2::IXMLDOMNodePtr pMsParent, MSXML2::IXMLDOMDocumentPtr pDoc) const;

protected:
	osg::ref_ptr<CNodeInfo>	m_root;
};

class COM_EXPORT CXmlParserEx
{

public:

	//һ����·��Ϣ
	typedef struct STURoadInfo
	{
	public:
		//ROADID='223' WIDTH='1' PARTSNUM='3' SCHEME='1��������'
		std::map<CString, CString> m_mapAttr;////��·ID,//��·�뾶,//��·����,//��������
		CString m_strPoint;//һ����·���ж���---һ�ε����˺�ӷ��յȼ�37.84545 112.5764 1.824102,37.84544 112.576 1.623652,37.84544 112.5759 1.741274;1|37.84544 112.576 1.623652,37.84544 112.5759 1.741274;1

	}RoadInfo;
	CXmlParserEx();
	virtual ~CXmlParserEx();
	bool Init(CString &xml);
	bool InitEx(CString &xml);
	/*
	Ŀ�ģ���ȡĳ����ĳ������
	������
	      name		(in)	��������
	���أ�����ֵ
	*/
	CString GetNodeAttribute(const CString &name);
	//������е�·
	std::list<STURoadInfo> GetListStuRoadInfo();
private:
	std::map<CString, CString> m_mapAttr;//�����Ļ�������-��ɫ
	std::list<STURoadInfo> m_listStu;

};

}