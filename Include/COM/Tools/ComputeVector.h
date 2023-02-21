#include <COM\Export.h>
#include <vector>
#include <osg/Vec3>
namespace COM
{
	/*
	功能描述：	计算出垂直与AB，CD向量的向量
	参数：		AB	(in)	向量AB
				CD  (in)	向量CD
	返回值:		法向量
	Author:		xubaolong
	Data:		2013.2.27
	*/
	COM_EXPORT osg::Vec3 computeNormalVector(const osg::Vec3& AB,const osg::Vec3& CD);

	
	/*
	功能描述：	计算两点之间的距离
	参数：		vertex1	(in)	点1
				vertex2  (in)	点2
	返回值:		距离
	Author:		xubaolong
	Data:		2013.3.6
	*/
	COM_EXPORT double computeTwoVectorLength(const osg::Vec3 vertex1,const osg::Vec3 vertex2);
	
}