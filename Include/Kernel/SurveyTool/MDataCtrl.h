#pragma once
/********************************************************************
created:	2009/07/01
file base:	GUIEventHandler
file ext:	h
author:		����
purpose:	������ƹ������ṩ����֧��
*********************************************************************/
/*
History:
Data: 2010.3.17
Description: liugh Move Here
*/
#include <osg/Node>
#include <vector>
#include <osg/Geode>
class CrenderTool;

class MLineData//��������
{
public:
	osg::Vec3 m_begin;
	osg::Vec3 m_end;
	//
	int m_id;
};
class MCircleData//Բ������
{
public:
	double m_r;
	osg::Vec3 m_Center;
	//
	int m_id;
};
class MPathData//·������
{
public:
	MPathData(){_vertex = new osg::Vec3Array;}
	osg::ref_ptr<osg::Vec3Array> _vertex;
	MPathData&operator=(const MPathData&rh)
	{
		*_vertex = *rh._vertex;
		m_id = rh.m_id;
		return (*this);
	}
	//
	int m_id;
};
class MPolyData//���������
{
public:
	MPolyData(){_vertex = new osg::Vec3Array;}
	osg::ref_ptr<osg::Vec3Array> _vertex;
	MPolyData&operator=(const MPolyData&rh)
	{
		*_vertex = *rh._vertex;
		m_id = rh.m_id;
		return (*this);
	}
	//
	int m_id;
};
class MNodeData
{
public:
	osg::ref_ptr<osg::Node> node;
	int m_id;
};
enum TOOL_TYPE{//��������
	LINE_TOOL = 1,//��
	CIRCLE_TOOL,//Բ
	POLY_TOOL,//�����
	PATH_TOOL,//·��
	UNUSE_TOOL,//δʹ��
};
class CMDataCtrl
{
public:
	CMDataCtrl(void);
	~CMDataCtrl(void);
	friend class CRenderTool;
private:
	//���������
	void addLineData(const MLineData&data);
	//ɾ��������
	void delLineData(const MLineData&data);
	//��ѯ������
	bool findLineData(MLineData&data);
	//���Բ����
	void addCircleData(const MCircleData&data);
	//ɾ��Բ����
	void delCircleData(const MCircleData&data);
	//����Բ����
	bool findCircleData(MCircleData&data);
	//���·������
	void addPathData(const MPathData&data);
	//ɾ��·������
	void delPathData(const MPathData&data);
	//���Ҳ�����·������
	bool findPathData(MPathData&data);
	//��Ӷ��������
	void addPolyData(const MPolyData&data);
	//ɾ�����������
	void delPolyData(const MPolyData&data);
	//���Ҳ�����·������
	bool findPolyData(MPolyData&data);
	//�½�Ψһ��ʾ
	int createNewId();
	//���osg����
	void addOsgData(int id,osg::ref_ptr<osg::Node>gnode);
	//ɾ��osg����
	void delOsgData(int id);
	//���Ҳ�����osg����
	osg::ref_ptr<osg::Node> findOsgData(int id);
	//ɾ��ָ��ID������
	void delOneIdData(int id);
	//ɾ��ָ�����͵�����
	void delOneToolTypeData(TOOL_TYPE type);
	//ɾ����������
	void delAllData();
	//�õ�ָ��ID������
	TOOL_TYPE getObjectType(int id);
	////�õ�ID���б�
	std::vector<int> getAllId();
	////�õ�ָ�����͵�ID�б�
	std::vector<int> getTypeIdLst(TOOL_TYPE type);
private:
	std::vector<MLineData>		m_Line;//������
	std::vector<MCircleData>	m_Circle;//Բ����
	std::vector<MPathData>		m_Path;//·������
	std::vector<MPolyData>		m_Poly;	//����Σ�����������
	//�����ڵ�洢
	std::vector<MNodeData>		m_osgData;//�����ڵ�洢
};
