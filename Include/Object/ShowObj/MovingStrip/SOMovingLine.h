/***************************************************************************
Copyright (C),  
文件：SOMovingLine.h
作者：谢荣耀
版本：1.0
日期：2009-12-08
描述：运动线控制点的显示对象类

修改记录：
作者						日期					描述
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

class OBJECT_EXPORT CSOMovingLine : public EMObj::CObj
{
public:
	CSOMovingLine(void);
	~CSOMovingLine(void);

	enum{
		SOType = 11
	};

	bool Init(Data::CRowShowObj* pRowShowObj);

	DWORD GetType()
	{
		return SOType;
	}

	DWORD GetRenderType()
	{
		return Render_Type_Scene;
	}
	
	bool ReadProperty(MSXML2::IXMLDOMNodePtr pNode);
	MSXML2::IXMLDOMNodePtr GetProperty();

	void UpdateDraw(DWORD Value = 0);
	bool BeginEditUI();
	bool EndEdit3D();

	std::vector<osg::Vec3d> GetPointsList();

	void RemoveAllChildren();

	struct PathShapePoint
	{
		osg::Vec3d leftPoint;
		osg::Vec3d centerPoint;
		osg::Vec3d rightPoint;
		float      controlPointLen;//中心线的长度
		float      leftPointLen;
		float      rightPointLen;
	};
public:

	//XML读取和保存模块
	bool Init(MSXML2::IXMLDOMNodePtr pNode);
	MSXML2::IXMLDOMNodePtr GetXMLNode();

	virtual void update();
	void ReadXmlNodeColor(MSXML2::IXMLDOMElementPtr node,osg::Vec4& color);
private:

	/*
	绘制条带
	*/
	void DrawStrip();

	/*
	绘制线
	*/
	void DrawLine();

	bool ComputeAllControlPoints();

	/*
	计算某个点的right和up向量
	front          (in)    前一个点
	point          (in)    当前点
	back           (in)    后一个点
	upDir          (out)   向上的向量
	rightDir       (out)   向右的向量
	*/
	void ComputeSideDir(osg::Vec3d front,osg::Vec3d point,osg::Vec3d back,osg::Vec3d& rightDir);/*osg::Vec3d& upDir,*/

	/*?
	根据曲线方程，计算参数为t时的值
	t              (in)    指定的分割位置
	p1             (in)    曲线四个点的第一个点
	p2             (in)    曲线四个点的第二个点
	p3             (in)    曲线四个点的第三个点
	p4             (in)    曲线四个点的第四个点
	return: 该点的坐标
	*/
	osg::Vec3d Eq(float t, const osg::Vec3d& p1, const osg::Vec3d& p2, const osg::Vec3d& p3, const osg::Vec3d& p4);

	/*
	获取参数为t时的插值点
	t              (in)    指定的分割位置
	return: 该点的坐标
	*/
	osg::Vec3d GetInterpolatedSplinePoint(float t,float delta,std::vector<osg::Vec3d>& controlPointsList);

    osg::Vec3d GetInterpolatedPoint(float t);
	/*
	防止索引越界
	*/
	void Clamp(int& index);

	/*
	计算出几何面的点集
	*/
	void ComputeOutLine();

	bool ReadXmlNode(MSXML2::IXMLDOMNodePtr pNode);

protected:
	/************************************
	Add Author: LiuGH
	Purpose:	SHow or Hide Show Object
	Date:		2010-3-11
	*************************************/
	//闪烁控制，可改写该行为
	virtual void FlickPulse(bool bflag);
	virtual void FlickRestore();


private:
	class UpdatePathCallback: public osg::NodeCallback 
	{
	public:
		UpdatePathCallback(osg::TexMat* texmat,float speed):
		  _texmat(texmat),
			  _phaseT(0.0f),
			  _speed(speed)
		  {}

		  virtual void operator()(osg::Node* node, osg::NodeVisitor* nv) 
		  {
			  CSOMovingLine* pStrip = dynamic_cast<CSOMovingLine*>(node);
			  if(!pStrip)
			  {
              	  return;
			  }
			  pStrip->update();

			  if (!_texmat)
			  {
				 return;
			  }
			
			  _phaseT = _phaseT - _speed;
			  _texmat->setMatrix(osg::Matrix::translate(0.0,_phaseT,0.0));

		  }
	public:
		osg::ref_ptr<osg::TexMat> _texmat;
		float _phaseT;
		float _speed;

	};
	UpdatePathCallback* _updateCallback;

	std::vector<PathShapePoint> _pathShapePointsList;//路径面绘制几何体数据集

	float                         _tile;
	float                         _pathWidth;
    float                         _textureSpeed;
	double                        _eplison;//如果插值出来的点的间距小于该值，则删除

	bool                          _isTextureMove;
	CString		                  _imageFilePath;
	CString                       _releativeFilePath;

	osg::Vec3d                    _up;
	osg::Vec3d                    _firstRightPoint;
	osg::Vec3d                    _lastPathSideDir;

	std::vector<osg::Vec3d>       _controlPointsList;//原始坐标集
	std::vector<osg::Vec3d>       _splinePointsList;//最终要进行绘制的坐标集

	osg::ref_ptr<osg::Geode>      _pathGeode;

	osg::ref_ptr<osg::TexMat>     _texmat;
	osg::ref_ptr<osg::Texture2D>  _texture2D;

	osg::ref_ptr<osg::Vec3dArray>	m_Points;  

	float _delta;

	unsigned int                  _smoothness;
	unsigned int                  _repeatRatio;

	float	m_defaultLineWidth;
	float	m_curLineWidth;
	//osgGA::MatrixManipulator*	m_manipulator;

public:
	std::map<float, float>	m_linewidthThresholds;
	osg::Vec4               m_lineColor;
	float                   m_lineVisiableDis;
	float                   m_stripVisiableDis;

	osg::ref_ptr<osg::Geode>   m_lineGeode;
	osg::ref_ptr<osg::Switch>  m_switchNode;
};
}
