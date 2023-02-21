/*************************************************
file:			AddModelEvent.h
Author:			徐宝龙    
Date:			2009-3-32
Description:    添加模型事件
*************************************************/
#pragma once
#include <Kernel\Export.h>
#include <osgGA\GUIEventHandler>

namespace Kernel
{

class KERNEL_EXPORT AddModelEvent : public osgGA::GUIEventHandler
{
	enum OperatorStype
	{
		ADDMODEL =0,
		DRAWPIPLE = 1,
		DELETEMODE =2
	};
public:
	AddModelEvent(void);
	~AddModelEvent(void);
	bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);
	void setDrawPiple();
	void setDelete();
	void setModelFile(std::string modelFile);

	void setAddModelEvent(bool isEvent){_isAddModelevent = isEvent;}
	bool getAddModelEvent(){return _isAddModelevent;}

private:

	bool GetRay(float x, float y, osg::Vec3d& eye, osg::Vec3d& ray);
	void AddModelEnter();
	void drawPiple();
	void AddMarkSphere(osg::Vec3 pos);
	//北
	void translateNorth();
	//南
	void translateSouth();
	//东
	void translateEast();
	//西
	void translateWest();
	//上
	void translateUp();
	//下
	void translateDown();

	void updateTranslate();

private:
	osg::ref_ptr<osg::MatrixTransform> m_mark;
	osg::ref_ptr<osg::Node> m_model;
	osg::Matrix rotateMT;
	osg::Vec3d m_pos;
	bool isPos;
	bool _isCtrl;
	float _xRotate,_yRotate,_zRotate;
	OperatorStype _operatStype;
	
	bool _isAddModelevent;

	int _nodeID;
	std::map<int,osg::ref_ptr<osg::MatrixTransform>> m_mapNode;
	std::map<int,osg::ref_ptr<osg::StateSet>> m_mapOldStateSet;
	std::vector<int> m_selectNode;

};
	osg::StateSet *GetHightlightStateSet();
	osg::StateSet* CreatePipleStateSet();
}
