#include <COM\Export.h>
#include <vector>
#include <osg/Vec3>
namespace COM
{
	/*
	����������	�������ֱ��AB��CD����������
	������		AB	(in)	����AB
				CD  (in)	����CD
	����ֵ:		������
	Author:		xubaolong
	Data:		2013.2.27
	*/
	COM_EXPORT osg::Vec3 computeNormalVector(const osg::Vec3& AB,const osg::Vec3& CD);

	
	/*
	����������	��������֮��ľ���
	������		vertex1	(in)	��1
				vertex2  (in)	��2
	����ֵ:		����
	Author:		xubaolong
	Data:		2013.3.6
	*/
	COM_EXPORT double computeTwoVectorLength(const osg::Vec3 vertex1,const osg::Vec3 vertex2);
	
}