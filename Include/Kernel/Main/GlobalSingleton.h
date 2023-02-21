#pragma once
#include <osg/Referenced>
#include <Kernel/PipleLineAnimation/PipleLineAnimation.h>
#include <Kernel/Embankment/RenderToolManager.h>
#include <Kernel/SurveyTool/Region.h>
#include <Kernel/ClickMessage/ClickMessage.h>
#include <Kernel\SurveyTool\AddModelCmd.h>
#include <Kernel\ProfileAnglysis\ProfileManager.h>
#include <Object\AnimationManager\AnimationPathManager.h>
namespace Kernel
{
	class GlobalSingleton :
		public osg::Referenced
	{
	public:
		GlobalSingleton(void);
		~GlobalSingleton(void);
		//************************************
		// Method:    getSingletion
		// FullName:  BRC::GlobalSingleton::getSingletion
		// Access:    public 
		// Returns:   GlobalSingleton*
		// Qualifier:  获取全局单例
		//************************************
		static GlobalSingleton* getSingletion();
		PipleLineAnimation* getPipleLineAnimation()	;
		void setPipleLineAnimation(PipleLineAnimation* pPipline);
	
		RenderToolManager* getReaderToolManager();
		void setReaderToolManager(RenderToolManager* rtm);

		Region* getRegion();

		osg::ref_ptr<ClickMessage> getClickMessage();

		AddModelCmd* getAddModelCmd();

		ProfileManager* getProfileManager();
		void setProfileManager(ProfileManager* pfm);

		osg::ref_ptr<EMObj::AnimationPathManager> getAnimationPathManager();
		
	private:
		osg::ref_ptr<PipleLineAnimation> m_pipleLineAnimation;
		RenderToolManager* m_embankmentManager;
		Region* m_region;
		osg::ref_ptr<ClickMessage> m_clickMessage;
		AddModelCmd* m_addModelCmd;
		ProfileManager* m_profileManager;
		osg::ref_ptr<EMObj::AnimationPathManager> m_animationPathManager;

		static GlobalSingleton* m_globalSingletion ;
	};
}
