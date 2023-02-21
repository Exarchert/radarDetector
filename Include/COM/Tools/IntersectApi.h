/***************************************************************************
Copyright (C),  
�ļ���Intersect.h
���ߣ�л��ҫ
�汾��1.0
���ڣ�2009-12-4
����������������󽻵Ľӿ�

�޸ļ�¼��
����						����					����
***************************************************************************/
#pragma once
#include <COM\Export.h>
#include <osg\Camera>
namespace COM
{
	/*
	�õ�������󽻵������
	camera	        (in)	  ��ǰ�������
	subgraph        (in)      ִ���ཻ���������ڵ�
	x/y             (in)      ���λ��  
	pos             (in/out)  �볡���ཻ��λ��
	return: �ɹ�����true��ʧ�ܷ���false
	*/
	COM_EXPORT bool GetTerrainIntersection(osg::Camera *camera, osg::Node *subGraph,float x, float y, osg::Vec3 &pos);


}
