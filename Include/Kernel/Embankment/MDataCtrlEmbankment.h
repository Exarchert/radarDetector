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
class CRenderToolEmbankment;

class MLineDataEmbankment//��������
{
public:
	osg::Vec3 m_begin;
	osg::Vec3 m_end;
	//
	int m_id;
};
class MCircleDataEmbankment//Բ������
{
public:
	double m_r;
	osg::Vec3 m_Center;
	//
	int m_id;
};
class MPathDataEmbankment//·������
{
public:
	MPathDataEmbankment(){_vertex = new osg::Vec3Array;}
	osg::ref_ptr<osg::Vec3Array> _vertex;
	MPathDataEmbankment&operator=(const MPathDataEmbankment&rh)
	{
		*_vertex = *rh._vertex;
		m_id = rh.m_id;
		return (*this);
	}
	//
	int m_id;
};
class MPolyDataEmbankment//���������
{
public:
	MPolyDataEmbankment(){_vertex = new osg::Vec3Array;}
	osg::ref_ptr<osg::Vec3Array> _vertex;
	MPolyDataEmbankment&operator=(const MPolyDataEmbankment&rh)
	{
		*_vertex = *rh._vertex;
		m_id = rh.m_id;
		return (*this);
	}
	//
	int m_id;
};
class MNodeDataEmbankment
{
public:
	osg::ref_ptr<osg::Node> node;
	int m_id;
};
enum Embankment_TOOL_TYPE{//��������
	LINE_TOOL2 = 1,//��
	CIRCLE_TOOL2,//Բ
	POLY_TOOL2,//�����
	PATH_TOOL2,//·��
	UNUSE_TOOL2,//δʹ��
};
class CMDataCtrlEmbankment
{
public:
	CMDataCtrlEmbankment(void);
	~CMDataCtrlEmbankment(void);
	friend class CRenderToolEmbankment;

	//���������
	void addLineData(const MLineDataEmbankment&data);
	//ɾ��������
	void delLineData(const MLineDataEmbankment&data);
	//��ѯ������
	bool findLineData(MLineDataEmbankment&data);
	//���Բ����
	void addCircleData(const MCircleDataEmbankment&data);
	//ɾ��Բ����
	void delCircleData(const MCircleDataEmbankment&data);
	//����Բ����
	bool findCircleData(MCircleDataEmbankment&data);
	//���·������
	void addPathData(const MPathDataEmbankment&data);
	//ɾ��·������
	void delPathData(const MPathDataEmbankment&data);
	//���Ҳ�����·������
	bool findPathData(MPathDataEmbankment&data);
	//��Ӷ��������
	void addPolyData(const MPolyDataEmbankment&data);
	//ɾ�����������
	void delPolyData(const MPolyDataEmbankment&data);
	//���Ҳ�����·������
	bool findPolyData(MPolyDataEmbankment&data);
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
	void delOneToolTypeData(Embankment_TOOL_TYPE type);
	//ɾ����������
	void delAllData();
	//�õ�ָ��ID������
	Embankment_TOOL_TYPE getObjectType(int id);
	////�õ�ID���б�
	std::vector<int> getAllId();
	////�õ�ָ�����͵�ID�б�
	std::vector<int> getTypeIdLst(Embankment_TOOL_TYPE type);
private:
	std::vector<MLineDataEmbankment>		m_Line;//������
	std::vector<MCircleDataEmbankment>	m_Circle;//Բ����
	std::vector<MPathDataEmbankment>		m_Path;//·������
	std::vector<MPolyDataEmbankment>		m_Poly;	//����Σ�����������
	//�����ڵ�洢
	std::vector<MNodeDataEmbankment>		m_osgData;//�����ڵ�洢
};
