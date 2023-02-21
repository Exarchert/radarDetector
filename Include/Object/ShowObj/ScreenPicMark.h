#pragma once
#include <Object/Export.h>
#include <osg/MatrixTransform>

class OBJECT_EXPORT ScreenPicMark	:public osg::MatrixTransform
{
public:
	ScreenPicMark(void);
	~ScreenPicMark(void);
	//创建一个屏幕图片
	static osg::Geometry* CreateImageRectangle(const osg::Image* image, const osg::Vec2* size, float offsetX, float offsetY);

	bool Init(osg::Image* Img, osg::Vec3d Pos, osgViewer::Viewer* Viewer,osg::Vec2 lod);

protected:
	osg::ref_ptr<osg::Geode>	m_Pic;		//图片结点
	osg::Vec3d					m_Pos;
};
