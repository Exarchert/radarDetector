#pragma once

#include <osgViewer/Viewer>

//实用类，单件类，用于实现一些实用功能
class Util
{
private:
	Util(void);
	~Util(void);

public:
	static Util* instance();
	
public:
	//生成一个随机数
	static int randNum(float min, float max);
	
	//生成一个随机向量
	static osg::Vec3 randVec3(osg::Vec3 leftTop, osg::Vec3 rightBottom); 

	//获取左上和右下节点的中心节点
	static osg::Vec3 getCenter(osg::Vec3 leftTop, osg::Vec3 rightBottom);

	//测试函数，输出一个向量
	static void outputVec3(osg::Vec3 v); 


	static bool isInRange(float min,float max, float value);
	
private:
	static Util* _instance;
};
