/***************************************************************************
Copyright (C),  
文件：SOLine.h
作者：谢荣耀
版本：1.0
日期：2009-01-22
描述：线控制点的显示对象类

修改记录：
作者						日期					描述
谢荣耀                   2010-03-15             添加距离--线宽控制模块
***************************************************************************/

#pragma once
#include <Object/Export.h>
#include <Object/Obj.h>
#include <osg/TexMat>
#include <osgAPEx/CoordinateTool>
#include <osgDB/ReadFile>
#include <Object/ObjManager.h>

#pragma once

namespace EMObj{

//线的显示对象
class OBJECT_EXPORT CSOLine : public EMObj::CObj
{
public:
	CSOLine(void);
	~CSOLine(void);

	enum{
		SOType = 18
	};

	DWORD GetType()
	{
		return SOType;
	}

	DWORD GetRenderType()
	{
		return Render_Type_Scene;
	}

	//使用数据集中的数据初始化线模块
	bool Init(Data::CRowShowObj* pRowShowObj);

	//XML读取和保存模块
	bool Init(MSXML2::IXMLDOMNodePtr pNode);
	MSXML2::IXMLDOMNodePtr GetXMLNode();

	//改写CObj行为，获取属性信息
	bool ReadProperty(MSXML2::IXMLDOMNodePtr pNode);
	MSXML2::IXMLDOMNodePtr GetProperty();
	
	//重写编辑对话框
	bool BeginEditUI();

	//结束编辑对话框
	bool EndEdit3D();

	//刷新绘制
	void UpdateDraw(DWORD Value = 0);

    //得到控制点列表
	std::vector<osg::Vec3d> GetPointsList();

	//删除所有点和线几何体
	void RemoveAllChildren();

	/*
	目的：设置距离线宽列表
	参数：linewidth_thresholds		(in)	距离线宽列表
	*/
	void setLineWidthPairs(const std::map<float, float>& linewidth_thresholds=std::map<float, float>());

	/*
	目的：设置相机操作器
	参数：manip		(in)	相机操作器指针
	*/
	void setMatrixManipulator(osgGA::MatrixManipulator* manip);

protected:
	/************************************
	Add Author: LiuGH
	Purpose:	SHow or Hide Show Object
	Date:		2010-3-11
	************************************/
	//闪烁控制，可改写该行为
	virtual void FlickPulse(bool bflag);
	virtual void FlickRestore();

private:
    /*
	目的：获取当前XML节点里包含的距离线宽列表
	参数：pairs		(out)	距离线宽列表
	      pNode		(in)	要解析的XML节点
	*/
    void GetDistanceWidthPairs(std::map<float, float>& pairs,MSXML2::IXMLDOMNodePtr pNode);

	//执行绘制
	void Draw();

	//解析节点的XML数据
	bool ReadXmlNode(MSXML2::IXMLDOMNodePtr pNode);

	//重写点选，因为对线的点选和其他的不一样
	virtual bool PtSelect(double &x, double &y, osg::Vec3d& eye, osg::Vec3d& vec);

	//更新线属性
	virtual void update();

	//根据距离线宽列表和相机高度，更新线属性
	class UpdateLine : public osg::NodeCallback
	{
		virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
		{
			CSOLine* line = dynamic_cast<CSOLine*>(node);
			if (!line || !nv || !nv->getFrameStamp())
			{
				return;
			}

			line->update();
		}
	};
	friend class UpdateLine;

private:
	std::vector<osg::Vec3d>         m_controlPointsList;
	osg::ref_ptr<osg::Geode>        m_pathGeode;
	osg::ref_ptr<osg::Vec3dArray>	m_Points;  
	osg::Vec4 m_color;
  
	float	m_defaultLineWidth;
	float	m_curLineWidth;
	osgGA::MatrixManipulator*	m_manipulator;

public:
	std::map<float, float>	m_linewidthThresholds;

private:
	std::map<float, float>	                m_HeightThreshold;
};
}
