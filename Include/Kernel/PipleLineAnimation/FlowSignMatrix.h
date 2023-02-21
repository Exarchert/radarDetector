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
	/* ����˵��
	   dotArray    ��ʾ�����Ĺؼ��㣬�����
	   tims        �洢�����ڵ���ʼʱ�� ��λ����
	   radius      �뾶	
	/************************************************************************/
	FlowSignMatrix(osg::Vec3dArray* dotArray,osg::Vec2 tims,float radius);
	~FlowSignMatrix(void);

	void setPause(bool pause);

private:
	//����һ������ģ��
	osg::Node* CreateFlowSign();
	osg::StateSet* GetHightlightStateSet(osg::StateSet* s_state);
	//����ڵ�
	void ComputerDotArray();

	osg::ref_ptr<osg::Vec3dArray> m_dotArray;
	osg::Vec2 m_tims;
	float m_radius;

	osg::ref_ptr<PipleAnimationPathCallback> nc;
};
}