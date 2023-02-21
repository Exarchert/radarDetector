/***************************************************************************
Copyright (C),  
�ļ���SOMovingLine.h
���ߣ�л��ҫ
�汾��1.0
���ڣ�2009-12-08
�������˶��߿��Ƶ����ʾ������

�޸ļ�¼��
����						����					����
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
		float      controlPointLen;//�����ߵĳ���
		float      leftPointLen;
		float      rightPointLen;
	};
public:

	//XML��ȡ�ͱ���ģ��
	bool Init(MSXML2::IXMLDOMNodePtr pNode);
	MSXML2::IXMLDOMNodePtr GetXMLNode();

	virtual void update();
	void ReadXmlNodeColor(MSXML2::IXMLDOMElementPtr node,osg::Vec4& color);
private:

	/*
	��������
	*/
	void DrawStrip();

	/*
	������
	*/
	void DrawLine();

	bool ComputeAllControlPoints();

	/*
	����ĳ�����right��up����
	front          (in)    ǰһ����
	point          (in)    ��ǰ��
	back           (in)    ��һ����
	upDir          (out)   ���ϵ�����
	rightDir       (out)   ���ҵ�����
	*/
	void ComputeSideDir(osg::Vec3d front,osg::Vec3d point,osg::Vec3d back,osg::Vec3d& rightDir);/*osg::Vec3d& upDir,*/

	/*?
	�������߷��̣��������Ϊtʱ��ֵ
	t              (in)    ָ���ķָ�λ��
	p1             (in)    �����ĸ���ĵ�һ����
	p2             (in)    �����ĸ���ĵڶ�����
	p3             (in)    �����ĸ���ĵ�������
	p4             (in)    �����ĸ���ĵ��ĸ���
	return: �õ������
	*/
	osg::Vec3d Eq(float t, const osg::Vec3d& p1, const osg::Vec3d& p2, const osg::Vec3d& p3, const osg::Vec3d& p4);

	/*
	��ȡ����Ϊtʱ�Ĳ�ֵ��
	t              (in)    ָ���ķָ�λ��
	return: �õ������
	*/
	osg::Vec3d GetInterpolatedSplinePoint(float t,float delta,std::vector<osg::Vec3d>& controlPointsList);

    osg::Vec3d GetInterpolatedPoint(float t);
	/*
	��ֹ����Խ��
	*/
	void Clamp(int& index);

	/*
	�����������ĵ㼯
	*/
	void ComputeOutLine();

	bool ReadXmlNode(MSXML2::IXMLDOMNodePtr pNode);

protected:
	/************************************
	Add Author: LiuGH
	Purpose:	SHow or Hide Show Object
	Date:		2010-3-11
	*************************************/
	//��˸���ƣ��ɸ�д����Ϊ
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

	std::vector<PathShapePoint> _pathShapePointsList;//·������Ƽ��������ݼ�

	float                         _tile;
	float                         _pathWidth;
    float                         _textureSpeed;
	double                        _eplison;//�����ֵ�����ĵ�ļ��С�ڸ�ֵ����ɾ��

	bool                          _isTextureMove;
	CString		                  _imageFilePath;
	CString                       _releativeFilePath;

	osg::Vec3d                    _up;
	osg::Vec3d                    _firstRightPoint;
	osg::Vec3d                    _lastPathSideDir;

	std::vector<osg::Vec3d>       _controlPointsList;//ԭʼ���꼯
	std::vector<osg::Vec3d>       _splinePointsList;//����Ҫ���л��Ƶ����꼯

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
