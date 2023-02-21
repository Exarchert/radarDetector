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
	�õ��ӿ�ʼ���ƶ���Ŀ���Ĺ켣��,��̬���򲻱�
	StartPos	(in)	��ǰλ��
	TargetPos	(in)	Ŀ���
	TimeLenght	(in)	ʱ�䳤��
	*/
	static CMoveTrack* CreateTrack_MoveTo(osg::Vec3d& StartPos, osg::Vec3d& TargetPos, double TimeLenght = 3);

	/*
	�õ��ӿ�ʼ���ƶ���Ŀ���Ĺ켣��,��̬���򲻱�
	StartPos	(in)	��ǰλ��
	TargetPos	(in)	Ŀ���
	TimeLenght	(in)	ʱ�䳤��
	*/
	static CMoveTrack* CreateTrack_ChangeAngle(osg::Vec3d& StartPos, osg::Vec3d& TargetPos, osg::Vec3d startPose, osg::Vec3d endPose, double TimeLenght = 3);

	/*
	�õ��ӿ�ʼ���ƶ���Ŀ���Ĺ켣��
	��̬��Ŀ������ĳ���㣻
	·���������˶���
	���ʣ��ȼ����˶���ĩ�ٶ�Ϊ0.
	*/
	static CMoveTrack* CreateTrack_LockEyeArcPath(const osg::Vec3d &startPos, const osg::Vec3d &endPos, const osg::Vec3d &targetPos, double timeLen = 3);

	/*
	�õ��ӿ�ʼ���ƶ���Ŀ���Ĺ켣��
	��̬��Ŀ������ĳ���㣻
	·����ֱ���˶���
	���ʣ��ȼ����˶���ĩ�ٶ�Ϊ0.
	*/
	static CMoveTrack* CreateTrack_LockEyeLinePath(const osg::Vec3d &startPos, const osg::Vec3d &endPos, const osg::Vec3d &targetPos, double timeLen = 3);

	/*
	�õ��ӿ�ʼ���ƶ���Ŀ���Ĺ켣��
	��̬��Ŀ������ĳ���㣻
	·����ֱ���˶���
	���ʣ������˶���ĩ�ٶ�Ϊ0.
	*/
	static CMoveTrack* CreateTrack_LockEyeConstantSpeedLinePath(const osg::Vec3d &startPos, const osg::Vec3d &endPos, 
		const osg::Vec3d &targetPos, double timeLen=3.0f);


	/*
	�õ��ӿ�ʼ���ƶ���Ŀ���Ĺ켣��
	��̬��Ŀ������ĳ���߶Σ�
	·����ֱ���˶���						
	���ʣ��ȼ����˶���ĩ�ٶ�Ϊ0.
	*/
	static CMoveTrack* CreateTrack_LockEyeSegmentLinePath(const osg::Vec3d &startPos, const osg::Vec3d &endPos, 
		const osg::Vec3d &targetStart, const osg::Vec3d &targetEnd, double timeLen = 3);

	/*
	�������
	*/
	static CMoveTracks* CreateTrack_CameraFly(
		const osg::Vec3d &startPos, 
		const osg::Vec3d &endPos, 
		const osg::Vec3d &startPose,	//��ʼ��̬
		const osg::Vec3d &endPose,		//ĩ��̬
		double &timeLen);
};

}