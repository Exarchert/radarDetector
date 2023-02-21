#pragma once
#include <map>
#include <osg/Referenced>
#include <string>
#include <osg/ref_ptr>
#include <PipleManager/Obj.h>
#include <osg/NodeCallback>
#include <time.h>
#include <osgCg/Effect.h>
#include <PipleManager/Context.h>
namespace PipleManager
{
	enum ObjStateEnum;
	class NodeCallbackObj : public osg::NodeCallback 
	{
	public:
		NodeCallbackObj() : _lastUpdate(0),m_flickLastTime(0.f),m_flickCount(0.f) {
			osgCg::EffectManipulator* em = getPipleEffectManipulator();
			osgCg::EffectBase* ef= em->getEffect();
			m_fickState = ef->getPass("caizhi1","hightlight");//
		}

		virtual void operator()(osg::Node* node, osg::NodeVisitor* nv) 
		{
			Obj* pObj = dynamic_cast<Obj*>(node);
			if (!pObj)
				return;
			if (nv->getVisitorType() == osg::NodeVisitor::UPDATE_VISITOR && 
				nv->getFrameStamp() && 
				nv->getFrameStamp()->getFrameNumber() != _lastUpdate) 
			{
				//pObj->UpdateFick();
				if (m_flickLastTime == 0.f) {
					m_flickLastTime = clock();
				}
				double curTime = clock();
				if ((curTime-m_flickLastTime) >= 500.f)
				{
					m_flickLastTime = curTime;
					m_flickCount++;
					if (m_flickCount & 0x01)
					{
						pObj->setStateSet(m_fickState);
					}
					else
					{
						pObj->clearStateSet();
					}


				}
				_lastUpdate = nv->getFrameStamp()->getFrameNumber();
			}
			traverse(node, nv);
		}
	private:
		int     _lastUpdate;
		unsigned long m_flickLastTime;
		unsigned long   m_flickCount;
		osg::ref_ptr<osg::StateSet> m_fickState;

	};
	class ObjStateManager :public osg::Referenced
	{
	public:
		ObjStateManager(void);
		~ObjStateManager(void);
		typedef	std::map<std::string,int>  StatesMap;
		typedef std::map<std::string,osg::ref_ptr<NodeCallbackObj>> ObjsNodeCallback;
		bool setObjState(std::string obj,ObjStateEnum state);
		bool setObjState(std::string objID,osg::ref_ptr<Obj> objNode,ObjStateEnum state);

		std::vector<ObjStateEnum> getObjState(std::string objID);
		bool getObjState(std::string objID,ObjStateEnum state);
	private:
		
		StatesMap _objStates;
		ObjsNodeCallback _objsNodeCallback;
	};


}
								