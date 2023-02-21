/*************************************************************************
File:			COM_MoveTracks.h
Author:			张平    
Date:			2009-03-16
Description:    运动控制类中的运动序列类，描述一组连续的运动.

ChangeLog:
作者					日期					描述
--------------------------------------------------------------------------
黄志鹏				2009-07-21			修改CCOM_MoveTracks类从MFC CObject
										继承，使之具有Serialize功能.

*************************************************************************/

#pragma once

#include <temp/Export.h>
#include <temp/Movement/COM_MoveTrack.h>


namespace TEMP		{

//运动序列类
class TEMP_EXPORT CCOM_MoveTracks : public osg::Referenced, public CObject
{
public:
	CCOM_MoveTracks(void)
	{
		m_timeLength = 0.f;
	}

	virtual ~CCOM_MoveTracks(void)
	{
	}

	CCOM_MoveTracks(const CCOM_MoveTracks &tracks);

	//克隆自身
	virtual CCOM_MoveTracks * Clone();

	/*
	目的：获取CCOM_MoveTrack对象的个数
	*/
	int GetTrackCount()
	{
		return m_Tracks.size();
	}

	/*
	目的：获取指定序号的CCOM_MoveTrack对象
	参数：index		(in)	索引值
	返回：成功返回CCOM_MoveTrack对象指针，否则返回NULL
	*/
	CCOM_MoveTrack *GetTrackByIndex(unsigned int index)
	{
		if (index >= 0 && index < m_Tracks.size())
		{
			return m_Tracks[index].get();
		}
		return NULL;
	}

	//添加和删除某个CCOM_MoveTrack对象
	void AddTrack(CCOM_MoveTrack* Track);
	void DelTrack(CCOM_MoveTrack* Track);
	void Clear();

	/*
	目的：在指定对象前插入某个CCOM_MoveTrack对象
	参数：where		(in)	插入位置
		  track		(in)	插入的对象
	返回：成功返回TRUE,否则返回FALSE
	备注：如果where为NULL，则在队列最后添加
	*/
	bool InsertTrack(CCOM_MoveTrack* where, CCOM_MoveTrack* track);

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

	bool GetSpeed(double time, double &speed);

	/*
	目的：重新计算消耗时间
	*/
	void RecalTimeLength();

	/*
	目的：序列化操作
	*/
	virtual void Serialize(CArchive& ar);

	bool ParserXml(MSXML2::IXMLDOMNodePtr pNode);

protected:
	/*
	目的：获取时刻time所落在的CCOM_MoveTrack对象
	参数：Time		(in)		时刻
		  OutTime	(out)		返回相对于Track对象内的时刻
	返回：NULL表示失败，OutTime有效，否则OutTime无效
	*/
	CCOM_MoveTrack* GetTrack(double Time, double& OutTime);

	DECLARE_SERIAL(CCOM_MoveTracks)

protected:
	double m_timeLength;	//消耗时间
	std::vector<osg::ref_ptr<CCOM_MoveTrack>> m_Tracks;
};

typedef	std::vector<osg::ref_ptr<CCOM_MoveTrack>>::iterator	VecTrackItr;
typedef	std::vector<osg::ref_ptr<CCOM_MoveTrack>>::const_iterator VecTrackConstItr;

}