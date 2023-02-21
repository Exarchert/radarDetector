#pragma once

#include <vector>
#include <osg/Vec2d>
#include <COM/Export.h>

namespace COM
{

enum WindingOrder
{
	CCW,
	CW
};

//��ά�����
class COM_EXPORT Polygon2D
{
public:
	//���캯��
	Polygon2D(void);
	//���ݱ߽�������������
	Polygon2D(std::vector<osg::Vec2d> lstPoint);
	~Polygon2D(void);

public:
	//��ȡ���бպϵĵ����Ŀ
	int count();

	//��ȡ����open�����Ŀ
	int countOpen();	

	//��ȡָ���������ĵ�
	osg::Vec2d getPoint(int index);

public:
	//��ȡ�պϵ������
	std::vector<osg::Vec2d> getPointList();
	
	//��ȡopen�������
	std::vector<osg::Vec2d> getOpenPointList();

private:
	//������������
	void cleanUpPolygon(std::vector<osg::Vec2d> lstPoint);

	//ɾ�������ظ��ĵ�
	void removeDuplicatePoints();
	
	//��������ʱ��ʱ�뻹��˳ʱ��
	void checkWindingOrder();
	
	//�ѵ���������
	void reversePointList();
	
	//����������ʾ�Ķ���ε����
	double computeArea(std::vector<osg::Vec2d> lstPoint);
	

private:
	//���бպϵĶ���εĵ������
	std::vector<osg::Vec2d>  m_lstPoint;
	
	//��ȡ����open�Ķ���εĵ������
	std::vector<osg::Vec2d>  m_lstOpenPoint;
	
	//��ȡ��ʼ�������
	std::vector<osg::Vec2d> m_lstOriginPoint;

};

}