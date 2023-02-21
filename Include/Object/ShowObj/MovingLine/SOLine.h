/***************************************************************************
Copyright (C),  
�ļ���SOLine.h
���ߣ�л��ҫ
�汾��1.0
���ڣ�2009-01-22
�������߿��Ƶ����ʾ������

�޸ļ�¼��
����						����					����
л��ҫ                   2010-03-15             ��Ӿ���--�߿����ģ��
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

//�ߵ���ʾ����
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

	//ʹ�����ݼ��е����ݳ�ʼ����ģ��
	bool Init(Data::CRowShowObj* pRowShowObj);

	//XML��ȡ�ͱ���ģ��
	bool Init(MSXML2::IXMLDOMNodePtr pNode);
	MSXML2::IXMLDOMNodePtr GetXMLNode();

	//��дCObj��Ϊ����ȡ������Ϣ
	bool ReadProperty(MSXML2::IXMLDOMNodePtr pNode);
	MSXML2::IXMLDOMNodePtr GetProperty();
	
	//��д�༭�Ի���
	bool BeginEditUI();

	//�����༭�Ի���
	bool EndEdit3D();

	//ˢ�»���
	void UpdateDraw(DWORD Value = 0);

    //�õ����Ƶ��б�
	std::vector<osg::Vec3d> GetPointsList();

	//ɾ�����е���߼�����
	void RemoveAllChildren();

	/*
	Ŀ�ģ����þ����߿��б�
	������linewidth_thresholds		(in)	�����߿��б�
	*/
	void setLineWidthPairs(const std::map<float, float>& linewidth_thresholds=std::map<float, float>());

	/*
	Ŀ�ģ��������������
	������manip		(in)	���������ָ��
	*/
	void setMatrixManipulator(osgGA::MatrixManipulator* manip);

protected:
	/************************************
	Add Author: LiuGH
	Purpose:	SHow or Hide Show Object
	Date:		2010-3-11
	************************************/
	//��˸���ƣ��ɸ�д����Ϊ
	virtual void FlickPulse(bool bflag);
	virtual void FlickRestore();

private:
    /*
	Ŀ�ģ���ȡ��ǰXML�ڵ�������ľ����߿��б�
	������pairs		(out)	�����߿��б�
	      pNode		(in)	Ҫ������XML�ڵ�
	*/
    void GetDistanceWidthPairs(std::map<float, float>& pairs,MSXML2::IXMLDOMNodePtr pNode);

	//ִ�л���
	void Draw();

	//�����ڵ��XML����
	bool ReadXmlNode(MSXML2::IXMLDOMNodePtr pNode);

	//��д��ѡ����Ϊ���ߵĵ�ѡ�������Ĳ�һ��
	virtual bool PtSelect(double &x, double &y, osg::Vec3d& eye, osg::Vec3d& vec);

	//����������
	virtual void update();

	//���ݾ����߿��б������߶ȣ�����������
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
