/**************************************************************************
File:			CLodItem.h
Author:			张平    
Date:			2009-10-28
Description:    lod配置基类
---------------------------------------------------------------------------
Modify
Author:			谢荣耀   
Date:			2009-11-30
Description:    增加LOD配置数据的更新接口：UpdateData

Date:           2009-12-2
Description:    增加LOD配置中的中心点配置字段
                增加接口ReadLodCondition

Date:			2010-05-31
Author:			LiuGH
Description:	修改Lod类型获取函数，主要用于距离模式时计算显示对象坐标
***************************************************************************/

#pragma once
#include <COM\Lod\LodItem.h>
#include <vector>
#include <osg/ref_ptr>
#import   "msxml3.dll"

namespace COM{

class COM_EXPORT CLod : public osg::Referenced
{
public:
	CLod(void);
	~CLod(void);
	/*
	初始化
	LLA				(in)	经纬高
	PortfolioType	(in)	组合方式
	Interval		(in)	时间间隔，如果为0表示没有时间间隔
	*/
	bool Init(osg::Vec3d &LLA, bool PortfolioType, double Interval = 0);

	/*
	通过XML数据进行数据初始化
	*/
	bool Init(CString strXML);
	bool Init(MSXML2::IXMLDOMNodePtr pParaNode,MSXML2::IXMLDOMNodePtr pLodConNode);


	/*
	更新函数
	CameraLLA		(in)	相机经纬高
	CameraPos		(in)	相机位置，
	返回值:0表示未进行条件判断，执行的是空操作；1表示进行条件判断条件为真；2表示进行条件判断条件为假
	*/
	DWORD Update(double &Time, double &LastTime, osg::Vec3d &CameraLLA, osg::Vec3d &CameraPos);

	void AddItem(CLodItem* Item);
	void DelItem(CLodItem* Item);
	void Clear();

	/*

	*/
	void SetLLA(osg::Vec3d &LLA);
	osg::Vec3d GetLLA();

	//绝对坐标
	void SetPos(osg::Vec3d &Pos);
	osg::Vec3d GetPos();
	int	GetPriority(){return m_Priority;}
	void SetPriority(int Priority){m_Priority = Priority;}

	/*
    获取中心点模式
	return:中心点模式
	*/
	int GetCenterMode();

	/*
	获取条件组合方式
	return:条件组合方式的字符串
	*/
	CString GetConditionJudgeType();

	/*
	获取刷新时间间隔
	return : 时间间隔
	*/
	double GetUpdateInterval();

    /*
    更新LOD配置的数据
	strXML         (in)     LOD配置的字符串
	*/
	void UpdateData(CString strParaXML,CString strContentXML);

	/*
	读取LOD配置中的条件组合信息
	pNode          (in)     LOD条件组合的节点
	*/
	void ReadLodCondition(MSXML2::IXMLDOMNodePtr pNode);

	/*
	读取LOD配置中的中心点控制信息
	pNode          (in)     LOD中心点的节点
	*/
	void ReadCenterMode(MSXML2::IXMLDOMNodePtr pNode);

	/*
	获取当前Lod类型，如果有Distance控制，则返回Lod_Distance，否则返回Lod_None
	Modify History:
	Add Function Author: LiuGH
	Date:				 2010-06-01
	*/
	DWORD GetLodTypeContainDist();



protected:
	int		    m_centerPointMode;   //中心点模式 1 表示自身中心点 2 表示制定中心点
	int			m_Priority;			//优先级

	bool		m_PortfolioType;		//条件组合方式，true表示与方式组合，false表示或条件组合
	double		m_Interval;			//时间间隔，秒

	osg::Vec3d	m_LLA;
	osg::Vec3d	m_Pos;

	std::vector<osg::ref_ptr<CLodItem>> m_Items;

private:
	double m_DeTime;
	float m_CamraHeight;
	float m_Dist;
};

}