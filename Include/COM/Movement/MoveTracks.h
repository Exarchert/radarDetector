/*************************************************************************
File:			MoveTracks.h
Author:			张平    
Date:			2009-03-16
Description:    运动控制类中的运动序列类，描述一组连续的运动.

ChangeLog:
作者					日期					描述
--------------------------------------------------------------------------

*************************************************************************/

#pragma once
#include <COM\Export.h>
#include <COM\Movement\MoveTrack.h>

#include <COM\XML\XMLNode.h>
#include <COM\XML\XMLApi.h>

namespace COM{

//运动序列类
class COM_EXPORT CMoveTracks : public osg::Referenced, public CXMLNode
{
public:
	CMoveTracks(void)
	{
		m_timeLength = 0.f;
	}

	virtual ~CMoveTracks(void)
	{
	}

	CMoveTracks(const CMoveTracks &tracks);

	//克隆自身
	virtual CMoveTracks * Clone();

	/*
	目的：获取CMoveTrack对象的个数
	*/
	int GetTrackCount()
	{
		return m_Tracks.size();
	}

	/*
	目的：获取指定序号的CMoveTrack对象
	参数：index		(in)	索引值
	返回：成功返回CMoveTrack对象指针，否则返回NULL
	*/
	CMoveTrack *GetTrackByIndex(unsigned int index)
	{
		if (index >= 0 && index < m_Tracks.size())
		{
			return m_Tracks[index].get();
		}
		return NULL;
	}

	//添加和删除某个CMoveTrack对象
	void AddTrack(CMoveTrack* Track);
	void DelTrack(CMoveTrack* Track);
	void Clear();

	/*
	目的：在指定对象前插入某个CMoveTrack对象
	参数：where		(in)	插入位置
		  track		(in)	插入的对象
	返回：成功返回TRUE,否则返回FALSE
	备注：如果where为NULL，则在队列最后添加
	*/
	bool InsertTrack(CMoveTrack* where, CMoveTrack* track);

	/*
	目的：获取此运动序列的耗时
	*/
	double GetTimeLength()
	{
		return m_timeLength;
	}

	bool GetPosition(double time, osg::Vec3d &position);
	bool GetStartPosition(osg::Vec3d &position);
	bool GetEndPosition(osg::Vec3d &position);

	bool GetPose(double time, osg::Vec3d& pose);
	bool GetStartPose(osg::Vec3d& pose);
	bool GetEndPose(osg::Vec3d& pose);

	/*
	目的：重新计算消耗时间
	*/
	void RecalTimeLength();

	//XML读取和保存模块
	bool Init(MSXML2::IXMLDOMNodePtr pNode);
	MSXML2::IXMLDOMNodePtr GetXMLNode();

protected:
	/*
	目的：获取时刻time所落在的CMoveTrack对象
	参数：Time		(in)		时刻
		  OutTime	(out)		返回相对于Track对象内的时刻
	返回：NULL表示失败，OutTime有效，否则OutTime无效
	*/
	CMoveTrack* GetTrack(double Time, double& OutTime);

protected:
	double m_timeLength;	//消耗时间
	std::vector<osg::ref_ptr<CMoveTrack>> m_Tracks;
};

typedef	std::vector<osg::ref_ptr<CMoveTrack>>::iterator	VecTrackItr;
typedef	std::vector<osg::ref_ptr<CMoveTrack>>::const_iterator VecTrackConstItr;

}