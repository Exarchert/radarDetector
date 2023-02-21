/*
Copyright (C),  
文件：COM_ItemXmlLabel.h  COM_ItemXmlLabel.cpp
作者：黄志鹏
版本：1.0
日期：2009-09-01
描述：空间标盘类--支持XML格式配置

修改记录：
作者						日期						描述
-----------------------------------------------------------------------

*/

#pragma once 
#include <Temp\Export.h>
#include <Temp\Cartoon\COM_CartoonItem.h>
#include <osg\ref_ptr>
#include <osg\MatrixTransform>
#include <osg\Vec4>
#include <osgAPEx\TableLabel>

namespace TEMP		{

class TEMP_EXPORT CCOM_ItemXmlLabel : public CCOM_CartoonItem
{
public:
	CCOM_ItemXmlLabel()
	{
		 m_Name = _T("未命名Xml标牌");
	}

	virtual ~CCOM_ItemXmlLabel();

	CCOM_ItemXmlLabel(const CCOM_ItemXmlLabel &label);

	virtual CCOM_CartoonItem * Clone();

	/*
	目的：动画单元执行行为
	参数：Time		(in)	相对于开始活动时刻的时刻
		  pos		(in)	动画单位此时的位置			
	返回：void
	*/
	virtual void DoAction(double Time, osg::Vec3d pos);

	/*
	目的：序列化操作
	*/
	virtual void Serialize(CArchive& ar);

	/*
	Purpose:	Parser XML	Data Format for Animation
	Author:		Liugh
	*/
	virtual bool ParserXml(MSXML2::IXMLDOMNodePtr pNode);

	/*
	目的：获取和设置Label信息
	*/
	BOOL GetInformation(CString &devName);
	BOOL SetInformation(const CString &devName);

protected:
	DECLARE_SERIAL(CCOM_ItemXmlLabel)

protected:
	CString		m_devName;				//设备名称
};
}