/*********************************************************************
Copyright (C),  
�ļ���KernelContext.h KernelContext.cpp

���ߣ���־��
�汾��1.0
���ڣ�2009-11-15
�������ں˵�ǰ�����Ķ�����.

�޸ļ�¼��
����					����				����
----------------------------------------------------------------------

*********************************************************************/

#pragma once 

#include <Com\Export.h>
#include <osg\ref_ptr>
#include <osg\Referenced>
#include <osg\Node>
#include <osg\Group>
#include <osgViewer\Viewer>
#include <osgViewer\CompositeViewer>
#include <osg\Camera>
#include <osgWidget\WindowManager>
#include <osgGA\GUIEventHandler>
#include <osgGA\MatrixManipulator>
#include <osgVec\VecLayerManagerNode>
//#include <Com\Ramble\RambleManager.h>
#include <Com\Decoration\DecoraManager.h>
#include <Com\Cartoon\CartoonManager.h>
#include <Com\LocateAgent\LocateAgent.h>
#include <Com\Weather\Weather.h>
#include <Com\LightingMan\LightingMan.h>
#include <COM\Event\Event.h>
#include <COM\Command\Cmd.h>
#include <COM\Ramble\RambleMgr.h>
#include <COM\Ramble\BaseRamble.h>

#include <osgShadow/ShadowedScene>

#include <afxmt.h>

namespace COM	{

//Kernel������������
class COM_EXPORT CKernelContext : public osg::Referenced 
{
public:
	CKernelContext();
	virtual ~CKernelContext();

	void SetStop();

	//���úͻ�ȡ�Ӿ�������Ⱦ����
	void SetRenderWnd(HWND hwnd);
	HWND GetRenderWnd() const;

	//���á���ȡ��ͼ������
// 	void SetCompsiteViewer(osgViewer::CompositeViewer* compositeViewer);
// 	osgViewer::CompositeViewer *GetCompositeViewer() const;

	//���úͻ�ȡ�Ӿ���
	void SetSceneViewer(osgViewer::Viewer *viewer);
	//����������Ⱦ
	void SetViewerStereo(osg::ref_ptr<osg::DisplaySettings> dis);
	osg::DisplaySettings* GetViewerStereo();
	osgViewer::Viewer *GetSceneViewer() const;

	//��ȡ���������
	osg::Group *GetSceneRoot() const;

	//��ȡ��������
	osg::Group *GetFieldsRoot() const;

	osg::Group *GetShpLineRoot()const; 

	osg::Group* GetParticleGroup()const;

	osg::Group* GetFieldShadowRoot()  const
	{
	   return 	m_sceneShadow.get();
	}

	//��Ӻ��Ƴ���㵽(��)���������
	bool AddToSceneRoot(osg::Node *child);
	bool RemoveFromSceneRoot(osg::Node *child);

	//���úͻ�ȡ������
	void SetEarthNode(osg::Node *earth);
	osg::Node *GetEarthNode() const;

	//��ȡ������ĸ����(��������õ��򡢵ر��ϵ�ģ��)
	osg::Group *GetEarthGroupRoot() const;

	//��ȡ�����ĸ����(�������ô����㡢�ǿա����)
	osg::Group *GetSkyGroupRoot() const;

	//��ȡ��ʱ��������
	osg::Group *GetTempGroupRoot() const;

	//��ȡ������������
	osg::Group *GetAnimationGroupRoot() const;

	//��ȡ�û�����ģ�Ͷ�������
	osg::Group *GetUserCreateModelRoot() const;

	//��ʼ��ֹͣ��Ⱦ
	void StartRender();
	void StopRender();

	//���úͻ�ȡHUD
	bool SetHudForScreenObjects(osg::Camera *hud);
	osg::Camera *GetHudForScreenObjects() const;
	bool SetHudForWidgets(osg::Camera *hud);
	osg::Camera *GetHudForWidgets() const;

	bool SetHudForScreenObjectsCenterAligned(osg::Camera *hud);
	osg::Camera *GetHudForScreenObjectsCenterAligned() const;

	////���õ���hud
	//bool setHudNavigation(osg::Camera* hud);
	//osg::Camera* getHudNavigation() const;
	//void AddToHudForNavigtion(osg::Node *child);
	//void RemoveFromHudForNavigtion(osg::Node *child);
	//bool setHudNavigation2(osg::Camera* hud);
	//osg::Camera* getHudNavigation2() const;
	//void AddToHudForNavigtion2(osg::Node *child);
	//void RemoveFromHudForNavigtion2(osg::Node *child);

	//��Ӻ�ɾ����Ļ����(��)HUD
	void AddToHudForScreenObjects(osg::Node *child);
	void RemoveFromHudForScreenObjects(osg::Node *child);
	void AddToHudForWidgets(osg::Node *child);
	void RemoveFromHudForWidgets(osg::Node *child);

	//���úͻ�ȡ���ڹ���
	bool SetWindowManager(osgWidget::WindowManager *wm);
	osgWidget::WindowManager *GetWindowManager() const;

	//����¼��ص�����
	bool AddEventHandler(osgGA::GUIEventHandler *handler);
	bool RemoveEventHandler(osgGA::GUIEventHandler *handler);

	//
	bool InsertEventHandlerFront(osgGA::GUIEventHandler *handler);
	//���û��ȡ���������
	//bool SetCameraManipulator(osgGA::MatrixManipulator *cameraMan);
	//osgGA::MatrixManipulator *GetCameraManipulator() const;

	//���úͻ�ȡ���ι�����
	bool SetRambleManager(COM::CRambleMgr *rambleMan);
	COM::CRambleMgr *GetRambleManager() const;
	COM::CBaseRamble* GetRamble() const;

	//���úͻ�ȡװ�ζ��������
	bool SetDecoraManager(COM::CDecoraManager *decoraMan);
	COM::CDecoraManager *GetDecoraManager() const;

	//���úͻ�ȡ�����������
	bool SetWeatherManager(COM::CWeather *weaMan);
	COM::CWeather *GetWeatherManager() const;

	//���úͻ�ȡ���չ������
	bool SetLightingManager(COM::CLightingMan *lightMan);
	COM::CLightingMan *GetLightingManager() const;

	//���úͻ�ȡ���������
	bool SetCameraPlayer(COM::CCartoonManager *player);
	COM::CCartoonManager *GetCameraPlayer() const;

	//���úͻ�ȡ��λ�������
	bool SetLocateAgent(COM::CLocateAgent *agent);
	COM::CLocateAgent *GetLocateAgent() const;

	//����Ϣ����Ͷ����Ϣ
	void SendMsg(COM::CMessage &Msg);

	//����Ӧ�ó����¼�����Ϣ���к���Ϣֵ
	void SetAppEvent(COM::CMessageQueue *pMsgQueue, UINT uAppMsgValue);

	COM::CEventMsgCenter &GetEventMsgCenter(){return m_EventMsgCenter;}

	//	��������ü�����CRambleManagerʹ�ã���ÿ���л����ʱ���á�
	bool SetNearFarCompute(osgGA::MatrixManipulator* manip);

	osgVec::VecLayerManagerNode* getVecNode(){return m_vecLayerMamagerNode;}
	//CTreeCtrl* getVecTree(){return m_tree;}
	
	//�����ź���ͣ��������ά�߳�
	void SetPauseRender(bool bPause);

	//���õ���͸���� transparency͸�������÷�ΧΪ0-100 �챦��2012-12-25
	void setEarthTransparent(int transparency);

	////��ȡ��ѡ�������
	//Kernel::Region* GetRegion();

	/************************************************************************/
	/* �޳���ײ���ֹ������                                                 */
	/************************************************************************/
	void addRejectNode(LONG64 nodeID)
	{
		bool b=true;
		std::vector<LONG64>::iterator nodeIter =m_rejectNode.begin();
		for (;nodeIter!=m_rejectNode.end();nodeIter++)
		{
			if (nodeID == *nodeIter)
			{
				b=false;
			}
		}
		if (b)
		{
			m_rejectNode.push_back(nodeID);
		}
		
	}

	void deleteRejectNode(LONG64 nodeID)
	{
		std::vector<LONG64>::iterator nodeIter =m_rejectNode.begin();
		for (;nodeIter!=m_rejectNode.end();nodeIter++)
		{
			if (nodeID == *nodeIter)
			{
			   m_rejectNode.erase(nodeIter);
			   break;;
			}
		}
		
	}

	std::vector<LONG64> getRejectNode()
	{
		return m_rejectNode;
	}


	/************************************************************************/
	/* ����״̬�����ִ���                                                 */
	/************************************************************************/
	void addActionObjs(LONG64 nodeID)
	{
		bool b=true;
		std::vector<LONG64>::iterator nodeIter =m_ActionObjs.begin();
		for (;nodeIter!=m_ActionObjs.end();nodeIter++)
		{
			if (nodeID == *nodeIter)
			{
				b=false;
			}
		}
		if (b)
		{
			m_ActionObjs.push_back(nodeID);
		}

	}

	void deleteActionObjs(LONG64 nodeID)
	{
		std::vector<LONG64>::iterator nodeIter =m_ActionObjs.begin();
		for (;nodeIter!=m_ActionObjs.end();nodeIter++)
		{
			if (nodeID == *nodeIter)
			{
				m_ActionObjs.erase(nodeIter);
				break;;
			}
		}

	}

	std::vector<LONG64> getActionObjs()
	{
		return m_ActionObjs;
	}
	std::vector<LONG64> m_ActionObjs;

	/************************************************************************/
	/* ѡ�ж�������ִ���                                                 */
	/************************************************************************/
	void addSelectObjs(LONG64 nodeID)
	{
		bool b=true;
		std::vector<LONG64>::iterator nodeIter =m_SelectObjs.begin();
		for (;nodeIter!=m_SelectObjs.end();nodeIter++)
		{
			if (nodeID == *nodeIter)
			{
				b=false;
			}
		}
		if (b)
		{
			m_SelectObjs.push_back(nodeID);
		}
	}

	void deleteSelectObjs(LONG64 nodeID)
	{
		std::vector<LONG64>::iterator nodeIter =m_SelectObjs.begin();
		for (;nodeIter!=m_SelectObjs.end();nodeIter++)
		{
			if (nodeID == *nodeIter)
			{
				m_SelectObjs.erase(nodeIter);
				break;;
			}
		}

	}

	void deleteAllSelect()
	{
		m_SelectObjs.clear();
	}

	std::vector<LONG64> getSelectObjs()
	{
		return m_SelectObjs;
	}
	std::vector<LONG64> m_SelectObjs;
	//
	/************************************************************************/
	/* ���ƹ����ִ���                                                 */
	/************************************************************************/
	void addScutheonObjs(LONG64 nodeID)
	{
		bool b=true;
		std::vector<LONG64>::iterator nodeIter =m_ScutheonObjs.begin();
		for (;nodeIter!=m_ScutheonObjs.end();nodeIter++)
		{
			if (nodeID == *nodeIter)
			{
				b=false;
			}
		}
		if (b)
		{
			m_ScutheonObjs.push_back(nodeID);
		}

	}

	void deleteScutheonObjs(LONG64 nodeID)
	{
		std::vector<LONG64>::iterator nodeIter =m_ScutheonObjs.begin();
		for (;nodeIter!=m_ScutheonObjs.end();nodeIter++)
		{
			if (nodeID == *nodeIter)
			{
				m_ScutheonObjs.erase(nodeIter);
				break;;
			}
		}

	}

	void SetShowSetModelInfoDlg(bool flag);
	bool GetShowSetModelInfoDlg();

	std::vector<LONG64> getScutheonObjs()
	{
		return m_ScutheonObjs;
	}
	std::vector<LONG64> m_ScutheonObjs;
	//
protected:
	void Reset();
	void SetSceneShadow();
protected:
	HWND								m_hwnd;			//�Ӿ�������Ⱦ����
//	osg::ref_ptr<osgViewer::CompositeViewer> m_CompositeViewer;	//��ͼ������
	osg::ref_ptr<osgViewer::Viewer>		m_viewer;		//�Ӿ���
	static osg::DisplaySettings* m_dis;           //������Ⱦģʽ  
	osg::ref_ptr<osg::Group>			m_sceneRoot;	//����ͼ�����,���г���Ҫ���ڸý����
	osg::ref_ptr<osg::Group>			m_sceneModels;	//����ģ�͵ķ��ý��

	osg::ref_ptr<osgShadow::ShadowedScene> m_sceneShadow;//

	osg::ref_ptr<osg::Group>             m_shpRootGroup;
	osg::ref_ptr<osg::Group>            m_ParticleGroup; //����ϵͳ
	osg::ref_ptr<osg::Group>            m_AnimationGroup; //�����ڵ�
	osg::ref_ptr<osg::Group>            m_userModelGroup; //�û�ģ�ͽڵ�

	osg::ref_ptr<osg::Node>				m_earth;		//������
	osg::ref_ptr<osg::Group>			m_earthGroup;	//����������---����� m_earth
	osg::ref_ptr<osg::Group>			m_skyGroup;		//���������
	osg::ref_ptr<osg::Group>			m_tempGroup;	//��ʱ��������

	osg::ref_ptr<osg::Camera>			m_hudWidget;	//��Widgetʹ�õ�HUD
	osg::ref_ptr<osg::Camera>			m_hudScrObj;	//����Ļ��ʾ�������֡�ͼƬ��ʹ�õ�HUD
	osg::ref_ptr<osg::Camera>			m_hudScrObjCenterAligned;
	//osg::ref_ptr<osg::Camera>           m_hudNavigtion;  //������ʹ��
	//osg::ref_ptr<osg::Camera>           m_hudNavigtion2;  //���ɵ�������

	osg::ref_ptr<osgWidget::WindowManager>	m_wm;		//Widgets���ڹ���
	
	osg::ref_ptr<osgVec::VecLayerManagerNode>	m_vecLayerMamagerNode;
	//CTreeCtrl*									m_tree;
	//osg::ref_ptr<osgGA::MatrixManipulator>	m_cameraMan;//���������
	//osg::ref_ptr<COM::CRambleManager>	m_rambleMan;	//���ι�����
	osg::ref_ptr<COM::CRambleMgr>		m_rambleMan;	//���ι�����

	osg::ref_ptr<COM::CDecoraManager>	m_decoraMan;	//װ�ζ��������
	osg::ref_ptr<COM::CWeather>			m_weatherMan;	//�����������
	osg::ref_ptr<COM::CLightingMan>		m_lightMan;		//���չ������

	osg::ref_ptr<COM::CCartoonManager>	m_cameraPlayer;	//���������
	osg::ref_ptr<COM::CLocateAgent>		m_locateAgent;	//��λ�������

	//Kernel::Region*        m_region;        //���ÿ�ѡ��

	//�¼���Ϣ����
	COM::CEventMsgCenter				m_EventMsgCenter;

	//�������߳�ʹ�õ���Ϣ����
	COM::CMessageQueue*					m_pAppMsgQueue;
	//Ӧ�ó�����Ӧ��Ϣ
	UINT								m_uAppMsgValue;

	//�ź���
	CEvent	m_PauseRenderEvent;
	bool	m_bPause;
	bool    m_bShowSetModelInfoDlg;//����ģ����Ϣ�ĶԻ����Ƿ���ʾ--EM_WorkStyle_OCX_LocalResDD �����Ψһʹ��

	std::vector<LONG64>                 m_rejectNode;
public:
	//����ģʽ
	enum{
		EM_WorkStyle_UnDef,		
		EM_WorkStyle_ObjEdid,				//��ʾԪ��--�༭״̬
		EM_WorkStyle_CartoonEdit,			//�����༭״̬
		EM_WorkStyle_TestOCXViewer,			//�����ӿ�
		EM_WorkStyle_OCXViewer,				//�ͻ����ӿ�--������Դ
		EM_WorkStyle_OCXViewer_LocalRes,	//�ͻ����ӿ�--������Դ
		EM_WorkStyle_OCX_LocalResDD,		//�ͻ����ӿ�--������Դ
	};
	DWORD m_WorkStyle;

	//��������
	COM::CCmdCenter					m_CmdCenter;
	
	//��ִ���ļ�·��
	CString m_BinPath;

	//�����ļ�
	CString m_SysConfigFile;

	//��Ŀ����
	CString m_ProName;

	//��Դ���ͣ�1 ��ʾ������أ�2 ��ʾ���ؼ���
	DWORD m_ProResType;

	//��Ŀ��Դ��ַ
	CString m_ProUrl;

	//��Ŀ��Դ��ַ
	CString m_ProResUrl;

	//CacheĿ¼
	CString m_CachePath;

	//��Ŀ·��
	CString m_ProPath;

	//��Ŀ��Դ·�� = m_ProPath + "\\Res"
	CString m_ProResPath;

	//�����ļ�
	CString m_EarthFile;

	//�汾
	CString m_Version;

	//�Կ���Ϣ
	// 	printf("OpenGLʵ�ֳ��̵����֣�%s\n", name);
	// 	printf("��Ⱦ����ʶ����%s\n", biaoshifu);
	// 	printf("OOpenGLʵ�ֵİ汾�ţ�%s\n",OpenGLVersion );
	// 	printf("OGLU���߿�汾��%s\n", gluVersion);
	std::string m_glSupporName;
	std::string m_glRender;
	std::string m_glVersion;
	std::string m_gluVersion;

public:
	//�Ĳ�����������
	int MAX_DEPTH ;

	//ÿ���Ĳ������������ģ����Ŀ
	int MAX_CHILDREN_NUMBER ;

	int lodLength ; //��Χ��뾶�ı���
};

}