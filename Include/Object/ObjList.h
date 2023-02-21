/*************************************************
file:			COM_ObjectList.h
Author:			张平    
Date:			2009-03-31
Description:    物件列表
*************************************************/
#pragma once
#include <Object\Export.h>
#include <osg\MatrixTransform>
#include <osg\Switch>

#include <osgAPEx\CoordinateTool>
#include <Object\Obj.h>
#include <Data\Center.h>
#include <vector>
#include <map>
#include <osg/Array>


namespace EMObj
{
	//struct CollisionDetectionObj
	//{
	//public:
	//	
	//	osg::ref_ptr<CObj> m_obj;
	//	std::vector<osg::Vec3f> CollisionDetectionDot;
	//} ;

	class OBJECT_EXPORT CObjList : public osg::Switch
	{

	public:
		CObjList(void);
		~CObjList(void);

		void SetMT(double Lon, double Lat, double Altitude, float Yaw, float Pitch, float Roll);

		virtual void AddObj(CObj* Obj);
		virtual void DelObj(CObj* Obj);
		virtual void DelObj(LONG64 ID);
		virtual void Clear();

		CObj* GetObj(LONG64 ID);

		//点选
		//SelObjs	(out)	选中对象列表
		virtual int PtSelect(double &x, double &y, osg::Vec3d& eye, osg::Vec3d& vec, std::vector<osg::ref_ptr<CObj>>& SelObjs);

		//碰撞检测
		/************************************************************************/
		/* 碰撞检测
		startArray 检测起点集合，需要和终点一一对应
		endArray   检测终点集合，需要和起点一一对应
		collisionDectionArray  碰撞点集合
		返回 bool 有碰撞返回true 无返回false
		2013-03-04 xubaolong
		*/
		/************************************************************************/
		virtual int CollisionDetection(std::vector<osg::Vec3d> &startArray,std::vector<osg::Vec3d> &endArray,std::vector<osg::ref_ptr<CObj>>& objs, std::vector<std::vector<osg::Vec3d>>& collisionDectionArray);


		osg::MatrixTransform *GetMTNode() {return m_MT.get();}
		std::map<LONG64, osg::ref_ptr<CObj>> * GetObjs(){return &m_mapObjects;}

		virtual void traverse(osg::NodeVisitor& nv);

		void SetLodAble(bool Able);
		bool GetLodAble();
	protected:
		std::map<LONG64, osg::ref_ptr<CObj>> m_mapObjects;
		osg::ref_ptr<osg::MatrixTransform> m_MT;
		osg::ref_ptr<osg::MatrixTransform> m_CopyMT;
		bool m_bLodAble;

	};

}