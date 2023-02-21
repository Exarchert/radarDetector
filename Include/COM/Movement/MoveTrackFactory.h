#pragma once
#include "MoveTracks.h"
#include <osg\Vec3d>

namespace COM{
class COM_EXPORT CMoveTrackFactory
{
public:
	CMoveTrackFactory(void);
	~CMoveTrackFactory(void);


	/*
	得到从开始点移动到目标点的轨迹集,姿态朝向不变
	StartPos	(in)	当前位置
	TargetPos	(in)	目标点
	TimeLenght	(in)	时间长度
	*/
	static CMoveTrack* CreateTrack_MoveTo(osg::Vec3d& StartPos, osg::Vec3d& TargetPos, double TimeLenght = 3);

	/*
	得到从开始点移动到目标点的轨迹集,姿态朝向不变
	StartPos	(in)	当前位置
	TargetPos	(in)	目标点
	TimeLenght	(in)	时间长度
	*/
	static CMoveTrack* CreateTrack_ChangeAngle(osg::Vec3d& StartPos, osg::Vec3d& TargetPos, osg::Vec3d startPose, osg::Vec3d endPose, double TimeLenght = 3);

	/*
	得到从开始点移动到目标点的轨迹：
	姿态：目标锁定某个点；
	路径：弧线运动；
	速率：匀减速运动，末速度为0.
	*/
	static CMoveTrack* CreateTrack_LockEyeArcPath(const osg::Vec3d &startPos, const osg::Vec3d &endPos, const osg::Vec3d &targetPos, double timeLen = 3);

	/*
	得到从开始点移动到目标点的轨迹：
	姿态：目标锁定某个点；
	路径：直线运动；
	速率：匀减速运动，末速度为0.
	*/
	static CMoveTrack* CreateTrack_LockEyeLinePath(const osg::Vec3d &startPos, const osg::Vec3d &endPos, const osg::Vec3d &targetPos, double timeLen = 3);

	/*
	得到从开始点移动到目标点的轨迹：
	姿态：目标锁定某个点；
	路径：直线运动；
	速率：匀速运动，末速度为0.
	*/
	static CMoveTrack* CreateTrack_LockEyeConstantSpeedLinePath(const osg::Vec3d &startPos, const osg::Vec3d &endPos, 
		const osg::Vec3d &targetPos, double timeLen=3.0f);


	/*
	得到从开始点移动到目标点的轨迹：
	姿态：目标锁定某个线段；
	路径：直线运动；						
	速率：匀减速运动，末速度为0.
	*/
	static CMoveTrack* CreateTrack_LockEyeSegmentLinePath(const osg::Vec3d &startPos, const osg::Vec3d &endPos, 
		const osg::Vec3d &targetStart, const osg::Vec3d &targetEnd, double timeLen = 3);

	/*
	相机飞行
	*/
	static CMoveTracks* CreateTrack_CameraFly(
		const osg::Vec3d &startPos, 
		const osg::Vec3d &endPos, 
		const osg::Vec3d &startPose,	//初始姿态
		const osg::Vec3d &endPose,		//末姿态
		double &timeLen);
};

}