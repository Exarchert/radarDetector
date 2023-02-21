#pragma once
#include <osg/MatrixTransform>
#include <osg/AnimationPath>
#include <Kernel/PipleLineAnimation/PipleLineFlow.h>
namespace Kernel
{
class FlowSignMatrix:public osg::MatrixTransform
{
public:
	
	/************************************************************************/
	/* 参数说明
	   dotArray    标示流量的关键点，拐弯点
	   tims        存储两个节点起始时间 单位是秒
	   radius      半径	
	/************************************************************************/
	FlowSignMatrix(osg::Vec3dArray* dotArray,osg::Vec2 tims,float radius);
	~FlowSignMatrix(void);

	void setPause(bool pause);

private:
	//创建一个动画模型
	osg::Node* CreateFlowSign();
	osg::StateSet* GetHightlightStateSet(osg::StateSet* s_state);
	//计算节点
	void ComputerDotArray();

	osg::ref_ptr<osg::Vec3dArray> m_dotArray;
	osg::Vec2 m_tims;
	float m_radius;

	osg::ref_ptr<PipleAnimationPathCallback> nc;
};
}