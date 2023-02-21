/*************************************************
file:			ParticleManager.h
Author:			xubaolong    
Date:			2014-03-26
Description:    用户自创建模型管理
*************************************************/
#pragma once
#include <Object/Export.h>
#include <Object/UserCreateModel/UserCreateLine.h>
#include <Object/UserCreateModel/UserCreatePolygon.h>


namespace EMObj
{
class OBJECT_EXPORT UserCreateModelManager : public osg::Referenced
{
public:
	UserCreateModelManager(void);
	~UserCreateModelManager(void);

	void Init(osg::Group *userCreateModelRoot);

	void addLine(CString id,LineData lineData);
	void insertLine(CString id,int serialNumber,LineData lineData);
	void updateLine( CString id,int serialNumber,LineData lineData);
	void deleteLinePoint(CString id,int serialNumber);
	void addPolygon(CString id,PolygonData polygonData);
	void insertPolygon(CString id,int serialNumber,PolygonData polygonData);
	void deleteUserModel(CString id);

protected:
	typedef std::map<CString, osg::ref_ptr<osg::MatrixTransform>> MapUserCreateModel;
	MapUserCreateModel m_mapUserCreateModel;

	osg::ref_ptr<osg::Group> m_userCreateModelRoot;

	std::map<CString,LineData> m_mapLineData;

};

//XML读取和保存模块
OBJECT_EXPORT UserCreateModelManager *GetUserCreateModelManager();
}