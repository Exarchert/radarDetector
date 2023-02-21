/*************************************************
file:			AssembleObj.h
Author:			xubaolong    
Date:			2014-05-30
Description:    组装模型对象管理
*************************************************/
#pragma once

#include <Object/Export.h>
#include <Object/AssembleObj/AssembleObj.h>
#include <string>
#include "osgGA/GUIActionAdapter"

namespace EMObj
{
class OBJECT_EXPORT AssembleObjManager : public osg::Referenced
{
public:
	AssembleObjManager(void);
	~AssembleObjManager(void);

	void Init(osg::Group *Root);

	bool addAssembleGroup(AssembleGroupDB assembleGroupDB);
	bool addMemble(MemberDB memberDb);
	bool GetObjectXYZ(CString& cstrID,osg::Vec3d& pos);//获得基准世界坐标
	bool GetObjectXYZ(CString& cstrBaseID,CString& cstrChildID,osg::Vec3d& pos);//获得部件世界坐标
	bool GetPartXYZ(CString& cstrBaseID,CString& cstrObjectID,CString& cstrPartID,osg::Vec3d& pos);//获得对象部件世界坐标

	bool GetSelectPartID(CString& cstrBaseID,CString& cstrObjectID,CString& cstrPartID);//获得对象部件ID
	

	
	//点选  第一个合理的名称就返回，没有为FALSE
	bool PtSelect(double &x, double &y, osg::Vec3d& eye, osg::Vec3d& vec,osgGA::GUIActionAdapter& aa);
	bool IsRatoinalNode(const char* cName,CString& cstr);//判断节点是否合理
	void SetIsSelect(bool flag){m_bIsSelect = flag;}
	bool GetIsSelect(){return m_bIsSelect;}
	typedef std::map<std::string,osg::ref_ptr<osg::Node>> MapNodeCopy;
	MapNodeCopy m_mapNodeCopy;
protected:
	typedef std::map<CString, osg::ref_ptr<AssembleObj>> MapAssembleObjMx;
	MapAssembleObjMx m_mapAssembleObjGroup;

	osg::ref_ptr<osg::Group> m_assembleObjRoot;

	bool m_isPtSelect;//是否开启点选
	std::list<MemberDB*> m_listSelectMember;//存放选择的部件

	double m_x;
	double m_y;
	osg::Vec3d m_eye;
	osg::Vec3d m_vec;
	osgGA::GUIActionAdapter * m_paa;
	bool m_bIsSelect;

	//CString m_cstrCelectBaseID;
	//CString m_cstrCelectObjectID;
	//CString m_cstrCelectPartID;
	//bool m_bIsSelect;


};

//XML读取和保存模块
OBJECT_EXPORT AssembleObjManager *GetAssembleObjManager();
}