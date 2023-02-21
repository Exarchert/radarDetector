#pragma once

#include <string>
#include <osgViewer/Viewer>
#include <Object/Export.h>

namespace EMObj
{

//������ÿ��ģ�͵�����
class OBJECT_EXPORT ElementInfo:public osg::Referenced
{
public:
	ElementInfo(void);
	ElementInfo(std::string filename, osg::Vec3 center,osg::Vec3 angle,osg::Vec3 scale);

public:
	~ElementInfo(void);

public:
	osg::Vec3 getCenter() { return m_vCenter; };
	osg::Vec3 getAngle(){return m_vAngle;}
	osg::Vec3 getScale(){return m_vScale;}
	std::string getFilename() { return m_filename; };

private:
	//ģ�͵��ļ���
	std::string m_filename;
	//ģ�͵�����
	osg::Vec3 m_vCenter;
	//��̬
	osg::Vec3 m_vAngle;
	//����
	osg::Vec3 m_vScale;
};

}
