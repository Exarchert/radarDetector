/*************************************************
file:			AnimationSceneManager.h
Author:			xubaolong    
Date:			2014-03-14
Description:    动画节点管理
*************************************************/
#pragma once
#include <Object/Export.h>
#include <osg/Referenced>

namespace EMObj
{

	class OBJECT_EXPORT AnimationSceneManager : public osg::Referenced
	{
	public:
		AnimationSceneManager(void);
		~AnimationSceneManager(void);

		static osg::ref_ptr<AnimationSceneManager> getSingleton();

		void Init(osg::Group *animationSceneGroup);

		void addAnimationNode(int id,osg::ref_ptr<osg::Node> animationNode,osg::ref_ptr<osg::Node> animationLine);
		void deleteAnimationNode(int id);
		void updateAnimationLine(int ,osg::Vec3d pos);

		void addNodeTrackerManipulater(int id);
		void removeNodeTrackerManipulater(int id);
	private:
		void computerPosition(osg::ref_ptr<osg::Geode> geode, osg::Vec3 pos);
	protected:
		typedef std::map<int, osg::ref_ptr<osg::Node>> MapAnimationNode;
		MapAnimationNode m_mapAnimationNode;
		MapAnimationNode m_mapAnimationLine;

		osg::ref_ptr<osg::Group> m_panimationSceneGroup;
		//

		static osg::ref_ptr<AnimationSceneManager> m_animationSceneManager;
	};

}