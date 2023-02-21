#pragma once
#include <osg/StateSet>
#define PIPLEMANAGER_LIBRARY
#include <PipleManager/Export.h>

class PipleManager_EXPORT InitEffect
{
public:
	InitEffect(void);
	~InitEffect(void);
	void init();
private:
	//������������
	osg::ref_ptr<osg::StateSet> CreateEffect1(osg::Vec4 ambient,osg::Vec4 diffuse,
		osg::Vec4 specular,float shininess);
	//��ȡ����״̬
	osg::ref_ptr<osg::StateSet> CreateHightlightStateSet();

	osg::ref_ptr<osg::StateSet> CreateBumpnotan();
};
