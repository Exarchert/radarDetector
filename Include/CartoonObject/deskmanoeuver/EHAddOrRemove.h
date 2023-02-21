/*************************************************************************
FileName��EHAddOrRemove.h
Author:	Liugh
Date��2010-01-22
Description��Add Or Remove EvenetHandler
**************************************************************************/
#pragma once
namespace CartoonObject
{
	class CAddOrRemoveEH : public COM::CCmdObj
	{
	public:
		CAddOrRemoveEH(int mode,osg::ref_ptr<osgGA::GUIEventHandler> pHandler)
		{
			_mode = mode;
			_pHandler = pHandler;
		}
		void DoAction()	
		{
			if(_mode == 1)		
			{
				g_KernelContext->AddEventHandler(_pHandler);
			}
			else
			{
				g_KernelContext->RemoveEventHandler(_pHandler);
			}
		}

		/*
		���/ɾ��EventHandler
		_modeΪ1���,�����Ƴ�
		*/
		int _mode;
		osg::ref_ptr<osgGA::GUIEventHandler> _pHandler;
	};
}

/***********************************************/
//�϶�ͼ���뿪���ں��͸���Ϣ