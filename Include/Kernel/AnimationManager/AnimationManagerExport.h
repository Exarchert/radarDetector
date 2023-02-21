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
	// Returns:   int 动画编号
	// Qualifier:  添加一个车辆路径动画节点。
	// Parameter: std::vector<osg::Vec3> postions  路径点集，每一个点由经度，纬度，高度表示。
	// Parameter: std::string carModelFilePath	   车辆模型路径。
	// Parameter: float speed					   车速
	//************************************
	KERNEL_EXPORT int AddCarAnimationPath(int id,std::vector<osg::Vec3d> postions,float times);

	KERNEL_EXPORT int CreateCarAnimationPath(osg::Vec3d postions,std::string carModelFilePath,std::wstring describe,bool showline);

	KERNEL_EXPORT bool OperationAnimation(int animationPathID ,AnimationMode animationPathOperationStype); 

	KERNEL_EXPORT bool OperationAnimationNodetTrackerManipulator(int animationPathID ,bool nodeTrankerOperationStype = true); 

}