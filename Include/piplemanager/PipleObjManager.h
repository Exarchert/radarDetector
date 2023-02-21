/*************************************************


*************************************************/
#pragma once
#include <osg/Referenced>
#include <osg/NodeCallback>
#include <osg/Group>
#include <vector>
#include <PipleManager/Obj.h>
#include <PipleManager/ObjStateManager.h>
#include <PipleManager/Export.h>

//#define OUTPUTMODELS

namespace PipleManager
{
	enum ObjStateEnum
	{
		INIT      =1,	//初始
		HIGHLIGHT =2,	//高亮
		FLICKER   =4,   //闪烁    //4,8,16,32 
		HIDE      =8,	//隐藏
		MAXSTATE  =16

	};

class PipleManager_EXPORT PipleObjManager : public osg::Referenced
{

public:
	PipleObjManager(void);
	~PipleObjManager(void);

	void Init(osg::Group *SceneNode);
	typedef	 std::map<std::string,osg::ref_ptr<Obj>> ObjsMap;

	bool setObjState(std::string objID,ObjStateEnum state);
	std::vector<ObjStateEnum> getObjState(std::string objID);
	bool getObjState(std::string objID,ObjStateEnum state);
	osg::ref_ptr<Obj> getObj(std::string objID);
	bool getObjPos(std::string objID,osg::Vec3d& pos);

	bool AddObj(std::string objID ,osg::ref_ptr<Obj> objNode);

	bool ClearGroup();

	//点选
	int PtSelect(double &x, double &y, osg::Vec3d& eye, osg::Vec3d& vec, std::string& objName);
	int CollisionDetection(osg::BoundingBox& collisionBox,std::vector<osg::Vec3d> &startArray,std::vector<osg::Vec3d> &endArray,std::vector<std::string>& objIDstr, std::vector<std::vector<osg::Vec3d>> &collisionDectionArray);

protected:
	ObjsMap m_mapPipleObjList;

	osg::ref_ptr<osg::Group> m_SceneNode;
	osg::ref_ptr<ObjStateManager> _objStateManager;

#if defined OUTPUTMODELS
	std::map<__int64, osg::Matrix>	_enterMatrixList;
#endif
	//
};

//XML读取和保存模块
PipleManager_EXPORT PipleObjManager *GetObjManager();
}