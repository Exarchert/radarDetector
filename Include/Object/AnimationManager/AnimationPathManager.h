#pragma once
#include <osg/Referenced>
#include <osg/PositionAttitudeTransform>
#include <Object/Export.h>
#include <Kernel/AnimationManager/AnimationType.h>

namespace EMObj
{
	class OBJECT_EXPORT AnimationPathManager :public osg::Referenced
	{
	public:
		AnimationPathManager(void);
		~AnimationPathManager(void);
		int createAnimation(int id, std::string modelPathName,osg::Vec3d pathPoints,std::wstring describe,bool showLine);
		int addAnimationPath(int id,std::vector<osg::Vec3d> pathPoints,float speed);
		int addAnimationPath(int id,std::string modelPathName,std::vector<osg::Vec3d> pathPoints,float speed);
		void operationAnimation(int id,Kernel::AnimationMode animationMode);
		void operationAnimationNodeTracker(int id,bool animationNodeTrackerStype);
		int getMaxID();

	private:
		bool createAnimationPath(std::vector<osg::Vec3d> pathPoions,float cirumscribeLength,float speed,osg::ref_ptr<osg::AnimationPath> animationPath);
		bool createAnimationControlPoint(osg::Vec3d& startPoition,osg::Vec3d& endPosition,osg::AnimationPath::ControlPoint& startCP,osg::AnimationPath::ControlPoint& endCP);
		void computerSecondPointRotate(osg::Vec3d& pos1,osg::Vec3d& pos2,osg::Quat& rotate1,osg::Quat& rotate2);
		osg::ref_ptr<osg::Node> CreatePolyline(std::vector<osg::Vec3d>& pathPointsXYZ,osg::Vec4 color);
		osg::ref_ptr<osg::Node> CreatePolyline(osg::Vec3d pathPointsXYZ,osg::Vec4 color);
	private:
		std::map<int,osg::ref_ptr<osg::AnimationPathCallback>>     m_animationPathCallbacks;
		std::map<int,osg::ref_ptr<osg::AnimationPath>>             m_animationPaths;
		struct AnimationOriginalDB
		{
		public:
			float time;
			osg::Vec3d position;
		};
		std::map<int,std::vector<AnimationOriginalDB>>                      m_animationPathPositions; 
		int maxID;
	
	};
}
