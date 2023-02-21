#pragma once 
#include <Kernel/Export.h>
#include <osg/Vec3d>
#include <string>
#include <Kernel/AnimationManager/AnimationType.h>
namespace Kernel
{
	//************************************
	// Method:    AddCarAnimationPath
	// FullName:  Kernel::AddCarAnimationPath
	// Access:    public 
	// Returns:   int �������
	// Qualifier:  ���һ������·�������ڵ㡣
	// Parameter: std::vector<osg::Vec3> postions  ·���㼯��ÿһ�����ɾ��ȣ�γ�ȣ��߶ȱ�ʾ��
	// Parameter: std::string carModelFilePath	   ����ģ��·����
	// Parameter: float speed					   ����
	//************************************
	KERNEL_EXPORT int AddCarAnimationPath(int id,std::vector<osg::Vec3d> postions,float times);

	KERNEL_EXPORT int CreateCarAnimationPath(osg::Vec3d postions,std::string carModelFilePath,std::wstring describe,bool showline);

	KERNEL_EXPORT bool OperationAnimation(int animationPathID ,AnimationMode animationPathOperationStype); 

	KERNEL_EXPORT bool OperationAnimationNodetTrackerManipulator(int animationPathID ,bool nodeTrankerOperationStype = true); 

}