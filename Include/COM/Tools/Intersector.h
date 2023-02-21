/***************************************************
Copyright:       (C), 
File name:       OSGA_ToolsIntersector.h
Author:          �¿���    Version:   1.0     Date: 2009-03-25
Description:     �ཻ�����
Others:          
Function List:   
History:        
*************************************************/
#pragma once
#include <COM\Export.h>
#include <osg/Vec3d>
#include <osgGA/GUIActionAdapter>
#include <osgUtil/LineSegmentIntersector>
#include <osgUtil/IntersectVisitor>
#include <osgViewer/Viewer>

namespace COM	{

class COM_EXPORT CIntersector
{
public:
	CIntersector(void);
	~CIntersector(void);

	/*
	������  ��Ļ������
	������
	pt		(out)	�����볡���Ľ���
	x		(in)	��Ļ������λ�ã�-1��1֮�䣩
	y		(in)	��Ļ������λ�ã�-1��1֮�䣩
	aa		(in)	osg�¼���������GUIActionAdapter   
	����ֵ�� �����Ƿ��ཻ
	*/
	static bool GetPointByScreenInters(osg::Vec3d& pt,const float& x,const float& y,osgGA::GUIActionAdapter& aa);
	/*
	������  ��Ļ������
	������
	nodePath		(out)	�������ཻ��nodepath����¼���������ཻ����ײ�Ҷ�ӽڵ㵽�������ڵ��·����node*���飩
	x				(in)	��Ļ������λ�ã�-1��1֮�䣩
	y				(in)	��Ļ������λ�ã�-1��1֮�䣩
	aa				(in)	osg�¼���������GUIActionAdapter   
	����ֵ�� �����Ƿ��ཻ
	*/
	static bool GetNodePathByScreenInters(osg::NodePath& nodePath,const float& x,const float& y,osgGA::GUIActionAdapter& aa);
	/*
	������  ��Ļ������
	������
	hits		    (out)	�ཻ����ԭʼ����������osgUtil::LineSegmentIntersector::Intersections˵��
	x				(in)	��Ļ������λ�ã�-1��1֮�䣩
	y				(in)	��Ļ������λ�ã�-1��1֮�䣩
	aa				(in)	osg�¼���������GUIActionAdapter   
	����ֵ�� �����Ƿ��ཻ
	*/
	static bool GetIntersectionsByScreenInters(osgUtil::LineSegmentIntersector::Intersections& hits,const float& x,const float& y,osgGA::GUIActionAdapter& aa);



	/*
	������  �ռ��߶���ָ��ڵ���ཻ���
	������
	pt				(out)	�ཻ����
	startPt			(in)	�߶����
	endPt			(in)	�߶��յ�
	intersectedNode	(in)	�����Ľڵ�   
	����ֵ�� �����Ƿ��ཻ
	*/
	static bool GetPointByLineInters(osg::Vec3d& pt,const osg::Vec3d& startPt,const osg::Vec3d& endPt,osg::Node* intersectedNode);
	/*
	������  �ռ��߶���ָ��ڵ���ཻ���
	������
	nodePath		(out)	�ཻ�ڵ㵽�������ڵ��·��
	startPt			(in)	�߶����
	endPt			(in)	�߶��յ�
	intersectedNode	(in)	�����Ľڵ�   
	����ֵ�� �����Ƿ��ཻ
	*/
	static bool GetNodePathByLineInters(osg::NodePath& nodePath,const osg::Vec3d& startPt,const osg::Vec3d& endPt,osg::Node* intersectedNode);
	/*
	������  �ռ��߶���ָ��ڵ���ཻ���
	������
	hitList 		(out)	�ཻ����ԭʼ����������osgUtil::IntersectVisitor::HitList˵��
	startPt			(in)	�߶����
	endPt			(in)	�߶��յ�
	intersectedNode	(in)	�����Ľڵ�   
	����ֵ�� �����Ƿ��ཻ
	*/
	static bool GetHitListByLineInters(osgUtil::IntersectVisitor::HitList& hitList,const osg::Vec3d& startPt,const osg::Vec3d& endPt,osg::Node* intersectedNode);
};
}