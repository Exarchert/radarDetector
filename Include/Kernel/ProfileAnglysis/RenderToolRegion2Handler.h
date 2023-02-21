#pragma once
/********************************************************************
created:	2013-03-6
file base:	GUIEventHandler
file ext:	h
author:		�챦��
purpose:	��ѡ
*********************************************************************/

#include <osgViewer/ViewerEventHandlers>
#include <Kernel/Embankment/MDataCtrlEmbankment.h>
#include "ProfileAnalysisDlg.h"


namespace Kernel
{


class RenderToolRegion2Handler : public osgGA::GUIEventHandler
{
public:
	RenderToolRegion2Handler(osg::Group* root);
	~RenderToolRegion2Handler(void);

public:
	/** ���ش�����������ͷš��϶�*/
	bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa, osg::Object*, osg::NodeVisitor*);
	//����Ƿ��ڻ���״̬
	bool isMeasure();
	//���ò�濪��
	void setMeasureEnable(bool bEnable);
	//���ȫ�����
	void clearAll();
	//���ָ������
	void delObject(int id);
	//���Ҳ�����osg����
	osg::ref_ptr<osg::Node> getOsgData(int id);
	//���ָ��ָ�����Ͷ���
	void delObject(Embankment_TOOL_TYPE type);
	//�õ���ǰ�������
	Embankment_TOOL_TYPE getToolType();
	//���õ�ǰ�������
	void setToolType(Embankment_TOOL_TYPE type);
	//�½����� �������toolType ���ر�ʾ
	void createNewMeasure(Embankment_TOOL_TYPE toolType);
	//������ǰ����
	void closeCurrentMeasure();
	//�õ��������Ϣ
	void getPolyInfo(int id,MPolyDataEmbankment&);
	//�õ���ǰ����ID -1��ʾ�޲���
	int getCurrentObjId();
	//�õ������ɵĶ���ID -1��ʾ�޲���
	int getRecentObjId();
	//ѡ��һ������ Ψһ��ʾid 
	//void selectOneObject(int id,bool bSelected = true);
	//�õ������ʾ�б�
	std::vector<int> getObjectIdLst();
	//�õ�ָ���������
	Embankment_TOOL_TYPE getObjectType(int id);
	//���ݵ�������ָ��
	void setEarthNode(osg::ref_ptr<osg::Node>earth);
private:
	/*
	Ŀ�ģ���ȡ��Ļ���������Ľ���
	������camera      (in)       ���ָ��
	subGraph    (in)       ����ĳ���ͼ
	x, y        (in)       �����ӿ�����
	pos         (out)      �������������
	���أ��ɹ�����true,���򷵻�false
	*/
	bool getTerrainIntersection(osg::Camera *camera, osg::Node *subGraph,float x, float y, osg::Vec3 &pos);
	
	/*
	Ŀ�ģ���ӻ��Ƶ�������
	������	pg	(in)	������Ϣ
			id  ��in��  ���ݱ�ʾ
	���أ���	long
	*/
	void addGeometryToOsg(osg::Node* pN,int id);
	/*
	Ŀ�ģ�������갴���¼�
	������	ea	(in)	����¼�
	���أ���
	*/
	void mousePush(const osgGA::GUIEventAdapter& ea);
	/*
	Ŀ�ģ���������ͷ��¼�
	������	ea	(in)	����¼�
			view(in)    ������Ϣ
	���أ���
	*/
	void mouseRelease(const osgGA::GUIEventAdapter& ea,osgViewer::View *view);
	/*
	Ŀ�ģ������������ͷ�
	������	ea	(in)	����¼�
	view(in)    ������Ϣ
	���أ���
	*/
	void mouseLeftButtonRelease(const osgGA::GUIEventAdapter& ea,osgViewer::View *view);

	/*
	Ŀ�ģ���������϶��¼�
	������	ea	(in)	����¼�
			view(in)    ������Ϣ
	���أ���
	*/
	void mouseDrag(const osgGA::GUIEventAdapter& ea,osgViewer::View *view);
	/*
	Ŀ�ģ���������ƶ��¼�
	������	ea	(in)	����¼�
			view(in)    ������Ϣ
	���أ���
	*/
	void mouseMove(const osgGA::GUIEventAdapter& ea,osgViewer::View *view);
	/*
	��ռ�һ����ָ���ռ�����ϵ�е�����
	pos			(in/out)	����ռ������������ϵ�е�����,����õ���ָ���ռ�����ϵ�е�����
	origin		(in)		ָ���ռ�����ϵԭ������������ϵ�е�����
	x/y/z		(in)		ָ������ϵ��X/Y/Z������������ϵ�ķ���
	*/
	void GetPos(osg::Vec3& pos, const osg::Vec3& origin, const osg::Vec3& x, const osg::Vec3& y, const osg::Vec3& z);
	/*
	Ŀ�ģ��õ�����߶�
	������	pCamera	(in)	���ָ��
	���أ�����߶�
	*/
	double getCameraHeight(osg::Camera*pCamera);
	/*
	Ŀ�ģ����ƶ����
	������	mpld	(in)	MPolyDataEmbankment
	cameraHeight(in) ����߶�
	color(in) ��ɫ
	���أ���			 long
	*/
	void renderPoly(MPolyDataEmbankment&mpld);
	void renderPolyEnd(MPolyDataEmbankment&mpld);
	void renderPolyMove(MPolyDataEmbankment&mpld);
	void readerPolyLineMove(MPolyDataEmbankment&mpld,osg::Vec3 newPoint);
	
	/*
	Ŀ�ģ����㱾�ؾ��󣬼�x,y,z����������ϵ�ķ���
	������	pos(in)	��λ��
			mat(out) ���ؾ���
			rt(out) x����
			at(out) y����
			up(out) z����
	���أ���
	*/
	void computerMatrixAndXYZ(const osg::Vec3d& pos,osg::Matrix &mat,osg::Vec3 &rt,osg::Vec3 &at,osg::Vec3 &up);

	
	/*
	Ŀ�ģ�����һ��͸������
	������	
			
	���أ�����ָ��
	*/
	osg::StateSet* createTransparentBaffleStateSet(osg::Vec4 color);

	bool pointToFace(std::vector<osg::Vec3>& face4,osg::Vec3& point);


	inline osg::Vec3 project2Vector(osg::Vec3& vec2 , osg::Vec3 ori_vec2 ,osg::Vec3 p)
	{
		osg::Vec3f dir = p - ori_vec2;
		double t = (dir*vec2)/(vec2*vec2);
		return ori_vec2 + vec2 *t ;
	}
private://����
	Embankment_TOOL_TYPE m_toolType;
	CMDataCtrlEmbankment m_data;
	osg::ref_ptr<osg::Node> m_earth;
private://����
	float _mx;
	float _my;
	//���ƿ���
	bool m_bMeasured;
	//ѡ�����
	int m_iMeasurCount;
	//��ǰ���ƿ��Ʊ�ʾ
	int m_id;
	//��¼����༭��ID
	int m_ReadlyEdit;
	//����߶ȣ������л����Ʒ�ʽ
	double m_CameraHeight;
	//�����ǰ�߶�
	double m_currentCameraHeight;
	//��ӵĸ߶�
	double m_addHeight;
	//
	osg::Vec3 m_pathVertexMove;
	//������ɫ
	osg::Vec4 m_renderColor;
	//���������ɫ
	osg::Vec4 m_renderdColor;
	//�����߿�
	int m_lineWith;
	//�������ڵ�
	osg::Group* m_Root;

};
}