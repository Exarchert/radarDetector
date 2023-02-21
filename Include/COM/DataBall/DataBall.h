/****************************************************************************
Copyright (C),  
文件：DataBall.h  DataBall.cpp
作者：黄志鹏
版本：1.0
日期：2009-08-14
描述：数据采集类，采集三维场景中的显示对象的位置和姿态.

修改记录：
作者						日期						描述
-----------------------------------------------------------------------------
黄志鹏						2009-11-26					改进为对用户操作的响应封装，
														向用户提供出最终的结果.
黄志鹏						2010-01-25					增加设置步长的接口SetStep().
黄志鹏						2010-02-03					增加绑定相机功能，供相机数据采集
黄志鹏						2010-03-04					增加相机切换三视图功能

****************************************************************************/

#pragma once 

#include <Com\Export.h>
#include <osg\Vec3>
#include <osg\Vec3d>
#include <osg\Matrixd>
#include <osg\Node>
#include <osgViewer\Viewer>
#include <osgGA\GUIEventAdapter>
#include <osgGA\GUIActionAdapter>
//#include <Com\Ramble\RambleManager.h>
#include <COM\Ramble\RambleMgr.h>


namespace COM	{

//CDataBall类
class COM_EXPORT CDataBall : public osg::Referenced
{
public:
	CDataBall();
	virtual ~CDataBall();

	/*
	目的：初始化
	参数：pNode			(in)		传入显示物体(用于鼠标命中判断, 可为NULL)
	      pos			(in)		坐标位置x,y,z
	      at			(in)		姿态向量
		  up			(in)		姿态向量
		  pField		(in)		厂区组结点或地球结点, 用于鼠标拖动时的定位计算	
		  localToWorld	(in)		厂区到世界的变换矩阵, NULL表示使用地球坐标系(pos为经纬高)
	返回：无
	*/
	void Init(osg::Node *pNode, const osg::Vec3d &pos, const osg::Vec3f &at, const osg::Vec3f &up, osg::Node *pField,
			osg::Matrixd *localToWorld=NULL);

	/*
	目的：初始化
	参数：pNode			(in)		传入显示物体(用于鼠标命中判断，可为NULL)
	      pos			(in)		坐标位置x,y,z
	      yaw			(in)		姿态yaw
		  pitch			(in)		姿态pitch
	      roll			(in)		姿态roll
		  pField		(in)		厂区组结点或地球结点, 用于鼠标拖动时的定位计算
		  localToWorld	(in)		厂区到世界的变换矩阵, NULL表示使用地球坐标系(pos为经纬高)
	返回：无
	*/
	void Init(osg::Node *pNode, const osg::Vec3d &pos, float yaw, float pitch, float roll, osg::Node *pField, 
		osg::Matrixd *localToWorld=NULL);

	//设置漫游管理器,用于拖拽时操作器不工作.
	void SetRambleManager(COM::CRambleMgr *rambleMan);

	//处理用户输入
	bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);

	/*
	目的：设置步长
	备注：步长供调节位置和姿态使用
	*/
	void SetStep(float len);

	/*
	目的: 设置三视图距离
	*/
	void SetDistance(float dist) { m_distance = dist; }

	/*
	目的：判断坐标或姿态是否已改变
	返回：改变返回true,否则返回false
	*/
	bool IsChanged() const;

	/*
	目的：获取结果
	参数：pos		(out)		坐标位置x,y,z
	      at		(out)		姿态向量
		  up		(out)		姿态向量
	返回：无	
	*/
	void GetPosAndPose(osg::Vec3d &pos, osg::Vec3f &at, osg::Vec3f &up) const;

	/*
	目的：获取结果
	参数：pos		(out)		坐标位置x,y,z
	      yaw		(out)		姿态yaw
	      pitch		(out)		姿态pitch
	      roll		(out)		姿态roll
	返回：无
	*/
	void GetPosAndPose(osg::Vec3d &pos, float &yaw, float &pitch, float &roll);

	/*
	目的：获取物体到世界坐标系的变换矩阵
	参数：localToWorld	(out)	变换矩阵
	*/
	void GetLocalToWorld(osg::Matrixd &localToWorld) const;

	//获取采集球应该放大的倍数(为伴随相机时的自动放大使用)
	void GetScale(float &scale) const { scale = m_zoom; }

	void SetAllowDrag(bool bAllow = true){m_bAllowDrag = bAllow;}

protected:
	//计算位置和姿态并写入内存映射文件
	void CalculateAndWrite2MMBuffer();

	/*
	目的：写入位置和姿态数据到内存映射文件
	参数：lon, lat, height	(in)	经纬高
		  yaw, pitch, roll	(in)	姿态角
	*/
	void Write2MMBuffer(double lon, double lat, double height, float yaw, float pitch, float roll);

protected:

	enum DIRECTION_MODE{
		USER_VIEW,			//用户视角
		FIELD_SYSTEM		//厂区坐标系
	};

protected:
	HANDLE	m_hMMFile;
	LPVOID	m_pMMBuffer;

	//显示对象
	osg::Node *m_pNode;
	
	//拖动求交结点
	osg::Node *m_pField;

	//漫游管理者
	//osg::ref_ptr<COM::CRambleManager> m_rambleMan;
	osg::ref_ptr<COM::CRambleMgr> m_rambleMan;

	//鼠标的按下
	bool  m_bPicked;

	//移动操作模式
	DIRECTION_MODE				m_moveMode;

	//数据
	osg::Vec3d					m_posLocal;			//在参考坐标系的中(X, Y, Z)
	osg::Vec3f					m_poseLocal;		//在参考坐标系的中(yaw, pitch, roll)
	float						m_scale;			//调节精度倍率
	bool						m_bChanged;			//数据改变标识
	bool						m_bEarth;			//使用地球坐标系
	osg::Matrixd				m_localToWorld;		//厂区坐标系到世界坐标系的变换

	//绑定相机
	bool						m_bBindWithCamera;
	bool						m_bUseXYZDirection;
	bool						m_bAllowDrag;	//允许拖动
	float						m_zoom;

	//三视图观察距离
	float						m_distance;
};

}