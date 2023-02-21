/***************************************************************************
Copyright (C),  
�ļ���CartoonEventHandler.h
���ߣ�л��ҫ
�汾��1.0
���ڣ�2009-11-19
�����������û������룬�༭��λ�ú���̬��

�޸ļ�¼��
����						����					����
***************************************************************************/
#pragma once
#include <COM\thread\UIThreadUpdate.h>
#include <osgGA/GUIEventHandler>
#include <osgViewer/Viewer>

namespace CartoonObject
{

class  CCartoonEventHandler: public osgGA::GUIEventHandler
{
public:
	/*
	���캯��
	root          (in)     �������ڵ�
    terrain       (in)     �������νڵ㣬�����ǳ�����Ҳ�����ǵ���
    dlgInterface  (in)     ����Ļص��ӿ�
	*/
	CCartoonEventHandler(osg::Group* root,osg::Node* terrain,COM::CUIThreadUpdate* dlgInterface);
	~CCartoonEventHandler(void);

	/*
	�����¼�
	ea            (in)     ��ǰ���¼�����
	aa            (in)     ��ǰ�ĳ���
	return: �������true�� ���򷵻�false
	*/
	virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);

	/*
	��ȡ��ǰ�Ŀ��Ƶ��б�
	pointsList    (in/out)  ���Ƶ��б�
	*/
	void getControlPointsList(std::vector<osg::Vec3d>& pointsList);

	/*
	���¿��Ƶ��б�
    updateMap     (in)      ��ǰ���µĵ�ļ���
	*/
	void setUpdateControlPoints(std::map<unsigned int,osg::Vec3d> updateMap);

	/*
	���ÿ��Ƶ�λ�õ�UI�ӿ�
	intefacePtr   (in)      UI�ӿ�ָ��
	*/
	void setPosInterface(COM::CUIThreadUpdate* intefacePtr);

	/*
	���ÿ��Ƶ�Ƭ�ε�UI�ӿ�
	intefacePtr   (in)      UI�ӿ�ָ��
	*/
	void setPoseInterface(COM::CUIThreadUpdate* intefacePtr);

	/*
	ɾ����������
	*/
	void deleteGeometry();

private:
	/*
	���ݵ�ǰ��ʰȡ�㣬ȡ���ཻ�õ���λ��
	viewer	       (in)	     ����
	ea             (in)      ��ǰ����λ��
	pos            (out)     �����ʰȡ��
	return: �ɹ�����true��ʧ�ܷ���false
	*/
	bool pick(osgViewer::Viewer* viewer, const osgGA::GUIEventAdapter& ea, osg::Vec3& pos);

	/*
	�õ�����߶�
	camera	        (in)	 ��ǰ�������
	subgraph        (in)     ִ���ཻ���������ڵ�
    x               (in)     ���λ��
	y               (in)     ���λ��
	pos             (in)     �볡���ཻ��λ��
	return: �ɹ�����true��ʧ�ܷ���false
	*/
	bool getTerrainIntersection(osg::Camera* camera, osg::Node* subgraph,float x,float y, osg::Vec3& pos);

	/*
	�������¼�
	ea              (in)      ��ǰʰȡ��λ��
	*/
	void handlePush(const osgGA::GUIEventAdapter& ea,osgViewer::View* view);

	/*
	��������ƶ��¼�
	ea              (in)      ��ǰʰȡ��λ��
    view            (in)      ������ͼ
	return: ����ɹ�����true�����򷵻�false
	*/
	bool handleMove(const osgGA::GUIEventAdapter& ea,osgViewer::View* view);

	/*
	�õ�����߶�
	pCamera	(in)	���
	return: ����߶�
	*/
	double getCameraHeight(osg::Camera*pCamera);

	/*
	���㱾�ؾ��󣬼�x,y,z����������ϵ�ķ���
	pos     (in)	��λ��
	mat     (out)   ���ؾ���
	rt      (out)   x����
	at      (out)   y����
	up      (out)   z����
	*/
	void computerMatrixAndXYZ(const osg::Vec3d& pos,osg::Matrix &mat,osg::Vec3 &rt,osg::Vec3 &at,osg::Vec3 &up);

	/*
	���һ���µĿ��Ƶ�
	*/
	void AddPoint();

	/*
	���һ����Ƭ��
	*/
	void addSegment();

	/*
	����Ƭ��
	*/
	void updateSegment(unsigned int index,osg::Vec3d pos);

	/*
	ʹһ�����Ƶ��ý���
	*/
	void setActivePoint();

	/*
	���ݿ��Ƶ��λ�ô���һ�����Ƶ㼸����
	point          (in)   ���Ƶ��λ��
	*/
	osg::Geometry* createPointGeom(osg::Vec3d point,osg::Vec4 color);

	/*
	���������˵㴴��һ���߶μ�����
	startPoint     (in)   ��ʼ��
	endPoint       (in)   �յ�
	*/
	osg::Geometry* createSegmentGeom(osg::Vec3d startPoint,osg::Vec3d endPoint);

	/*
	��������ҵ��ڵ���
	geode          (in)   �ڵ�ָ��
	geom           (in)   ������ָ��
    index          (in)   ����������
	*/
	void addOrSetGeometry(osg::Geode* geode,osg::Geometry* geom,unsigned int index);

	/*
	����Ƿ�ʰȡ�����߶�
	ea              (in)      ��ǰʰȡ��λ��
	view            (in)      ������ͼ
	return: ���ʰȡ�����߶Σ��򷵻ش���0��ֵ�����򷵻�-1
	*/
	int selectSegment(const osgGA::GUIEventAdapter& ea,osgViewer::View* view);

	/*
	��⵱ǰ�������꿪ʼ�������Ƿ���ָ�����߶��ཻ
    eye             (in)       �ӵ�λ�ã����������������ϵ�е�����
	ray			    (in)	   ���߷����ѵ�λ��
	segment         (in)       �ཻ����Ŀ���߶�
    dis             (out)      �߶���Ŀ���߶εľ���
	rayCross        (out)      ���߶��ϵĴ���
	segCross        (out)      Ŀ���߶��ϵĴ���
	return :
	0����ʾ�������߶ε��м��������������
	1��ƽ��
	2����ʾ�������߶ε��м�����߷�������
	3����ʾ�������߶ε��ӳ����Ϻ�������������
	4����ʾ�������߶ε��ӳ����Ϻ����߷�������
	*/
	int RaySegmentDis(osg::Vec3d eye, osg::Vec3d ray, osgUtil::LineSegmentIntersector segment, double &dis, osg::Vec3d& rayCross, osg::Vec3d& segCross);

	/*
	ֱ����ֱ�ߵľ���
	line0		(in)	ֱ��0
	line1		(in)	ֱ��1
	dis			(out)	����
	pQ0			(out)	ֱ��0�ϵĴ���
	pQ1			(out)	ֱ��1�ϵĴ���
	����ֵ��0����ʾ��ƽ�У�1����ʾƽ�У�ƽ��ʱ������Ч
	*/
	int LineLineDis(osgUtil::LineSegmentIntersector& line0, osgUtil::LineSegmentIntersector& line1, double &dis, osg::Vec3d *pQ0 = NULL, osg::Vec3d *pQ1 = NULL);


private:
	osg::ref_ptr<osg::Node>						_terrain;
	osg::ref_ptr<osg::Geode>                    _lineGeode;
	osg::ref_ptr<osg::Geode>                    _pointGeode;

	osg::Vec3                                   _prePoint;
	osg::Vec3                                   _mousePoint;
	osg::Geometry*                              _activePointGeom;
	std::vector<osg::Vec3d>                     _controlPointsList;
	std::vector<osg::ref_ptr<osg::Geometry> >   _segmentGeometryList;

	std::map<int,osg::Vec3d>                    _updateMap;//�����б�

	osg::Vec4                                   _pointColor;       //����ɫ
	osg::Vec4                                   _activePointColor; //��ý������ɫ
	osg::Vec4                                   _lineColor;        //������ɫ
	unsigned int                                _lineWidth;        //�����߿�

	osg::ref_ptr<osg::Group>                    _group;

	bool                                        _isStop;
	bool                                        _isUpdate;
	unsigned int                                _updatePointIndex;

	bool                                        _isDirty;
	bool                                        _delete;

	double                                      _pickSegmentEplison;
	COM::CUIThreadUpdate*                       _trackPosInterface;
	COM::CUIThreadUpdate*                       _trackPoseInterface;
};

}
