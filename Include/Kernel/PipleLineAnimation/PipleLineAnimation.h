#pragma once
/************************************************************************/
/* 功能 管道动画管理
   作者：徐宝龙
   时间：2013-03-14
   2013-08-23 添加shp管网信息加入*/
/************************************************************************/
#include "FlowSignMatrix.h"
#include <Object/ObjManager.h>
#include <Kernel/AnimationManager/AnimationType.h>
#include <Kernel/PipleLineAnimation/PipleAnimationDB.h>

namespace Kernel
{
	
	class PipleLineAnimation:public osg::Referenced
	{	
	public:
		
		PipleLineAnimation(void);
		PipleLineAnimation(AnimationPipleLineModeDB animationPipleLineModeDB);
		~PipleLineAnimation(void);

		void setAnimationState(AnimationMode state);

	private:
		void RecursionChildMode2(CString modeID,osg::Vec3 beginDot,float beginTime);
		void RecursionChildMode(CString modeID,osg::Vec3 beginDot,float beginTime);
		osg::ref_ptr<Kernel::FlowSignMatrix> CreateFlowSignMatrix(osg::Vec3d beginDot,osg::Vec3d endDot,float beginTime,float speed,float &endTime);
		osg::Matrix GetEnterpriseMatrix(EMObj::CObj* modeObj);
		void AnimationBegin();
		void AnimationEnd();
		void AnimationPause(bool pause);
	private:
		std::vector<AnimationDotNodeDB> m_animationDotNodeDBArray;
		std::map<CString,AnimationDotChildMode> m_animationPipleLine;
		osg::ref_ptr<EMObj::CObjManager> m_objManager;
		osg::ref_ptr<osg::Group> m_animationGroup;
		osg::ref_ptr<osg::MatrixTransform> m_animationGroupMT;
		float m_radius;
		float m_speed;
		CString m_beginID;
		osg::Matrix m_EnterpriseMatrix;
		std::vector<osg::ref_ptr<FlowSignMatrix>> m_flowSignMatrixVec; 

	};

}
