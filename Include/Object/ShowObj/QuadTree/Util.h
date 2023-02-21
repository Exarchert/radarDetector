#pragma once

#include <osgViewer/Viewer>

//ʵ���࣬�����࣬����ʵ��һЩʵ�ù���
class Util
{
private:
	Util(void);
	~Util(void);

public:
	static Util* instance();
	
public:
	//����һ�������
	static int randNum(float min, float max);
	
	//����һ���������
	static osg::Vec3 randVec3(osg::Vec3 leftTop, osg::Vec3 rightBottom); 

	//��ȡ���Ϻ����½ڵ�����Ľڵ�
	static osg::Vec3 getCenter(osg::Vec3 leftTop, osg::Vec3 rightBottom);

	//���Ժ��������һ������
	static void outputVec3(osg::Vec3 v); 


	static bool isInRange(float min,float max, float value);
	
private:
	static Util* _instance;
};
