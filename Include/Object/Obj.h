/*************************************************
file:			Object.h
Author:			��ƽ    
Date:			2009-03-32
Description:    �����࣬���ж���Ļ���

--------------------------------------------------
modify:      л��ҫ
Date:        2009-11-27
Description: ��ӻ�ȡRowShowObj�Ľӿ�GetRowShowObj

*************************************************/
#pragma once

//osg
#include <OSG/ref_ptr>
#include <OSG/MatrixTransform>
#include <OSG/Switch>
#include <OSG/Geode>
#include <OSG/Vec3d>
#include <OSG/Vec3f>
#include <osg\StateSet>
#include <osg\Material>
#include <osg\NodeVisitor>
#include <osgFX/Effect>

//C++
#include <vector>

//self
#include <Data\RowStruct\RowShowObj.h>
#include <com\Movement\Movement.h>
#include <Com\Cartoon\CartoonManager.h>
#include <Com\Lod\Lod.h>
#include <Com\XML\XMLNode.h>
#include <Object\EditPropSheet\EditPropSheet.h>
#include <Object/ModelView.h>
#include <Object/ShowObj/TechniqueNodeCallback.h>
#include <Object/Export.h>

#import   "msxml3.dll"
namespace EMObj
{
	class OBJECT_EXPORT CObj : public osg::Switch
	{
	private:
		class CNCObj : public osg::NodeCallback 
		{
		public:
			CNCObj() : _lastUpdate(0) {}
			virtual void operator()(osg::Node* node, osg::NodeVisitor* nv) 
			{
				CObj* pObj = dynamic_cast<CObj*>(node);
				if (!pObj)
					return;
				if (nv->getVisitorType() == osg::NodeVisitor::UPDATE_VISITOR && 
					nv->getFrameStamp() && 
					nv->getFrameStamp()->getFrameNumber() != _lastUpdate) 
				{
					pObj->Update(nv, nv->getFrameStamp()->getFrameNumber(), _lastUpdate);
					_lastUpdate = nv->getFrameStamp()->getFrameNumber();
				}
				traverse(node, nv);
			}
			int _lastUpdate;
		};


	public:
		CObj(void);
		virtual ~CObj(void);

		/////////////////////////////////////��ʼ����ط���//////////////////////////////////////
	public:
		//���ݿ��ʼ��
		bool Init(Data::CRowShowObj* pRowShowObj);
		//XML��ʼ��
		bool Init(MSXML2::IXMLDOMNodePtr pNode);

		//��ȡ�ͻ�ȡ�ӵ���Ϣ
		bool ReadEyePosAndPose(MSXML2::IXMLDOMNodePtr pNode);
		MSXML2::IXMLDOMNodePtr GetEyePosAndPose();

		//��ȡ�ͻ�ȡLod��Ϣ
		bool ReadLod(MSXML2::IXMLDOMNodePtr pNode,LONG64 id);
		MSXML2::IXMLDOMNodePtr GetLod();

		//��ȡ�ͻ�ȡ������Ϣ
		virtual bool ReadProperty(MSXML2::IXMLDOMNodePtr pNode);
		virtual MSXML2::IXMLDOMNodePtr GetProperty();

		/////////////////////////////////////������ط���//////////////////////////////////////
		//Ĭ�ϵĶ���������������ѡ�У�͸������˸����ʾ�༭�Ի���
		enum{
			EM_OATYPE_Visible = 1,
			EM_OATYPE_Select,            //ѡ��
			EM_OATYPE_Transparent,
			EM_OATYPE_Flicker,
			EM_OATYPE_Clear,
		};
		virtual bool DefaultAction(DWORD ActionType, DWORD Value = 0);
		//�˷�����ʱֻ����ѡ��״̬
		virtual bool DefaultAction(DWORD ActionType, std::vector<float> Value );

		//��λ
		enum{
			LocateStyle_Normal = 1,
			LocateStyle_Fly
		};
		virtual bool Locate(COM::CCartoonManager *CartoonManager = NULL, DWORD LocateStyle = LocateStyle_Fly);

		//����״̬
		virtual void SaveState();

		//�ָ����һ�α����״̬
		virtual void ResumeState();

		//�������ݵ����ݿ�
		virtual bool SaveDataToDB(DWORD Value = 0);

	protected:
		//��˸���ƣ��ɸ�д����Ϊ
		virtual void FlickPulse(bool bflag);
		virtual void FlickRestore();

		/////////////////////////////////////������ط���//////////////////////////////////////
	public:
		//ID
		LONG64	ID;

		//����
		virtual DWORD GetType();

		//��Ⱦ����
		enum{
			Render_Type_UnDefault,
			Render_Type_Screen,
			Render_Type_Widget,
			Render_Type_Scene
		};
		virtual DWORD GetRenderType();

		//����
		CString GetName();
		void SetName(CString Name);

		//���ݿ��¼
		Data::CRowShowObj* GetRowShowObj();
		//XML�ڵ���Ϣ
		MSXML2::IXMLDOMNodePtr GetXMLNode();

		/////////////////////////////////////ˢ����ط���//////////////////////////////////////
	public:
		//ˢ��
		void Refresh(DWORD Para1 = 0, DWORD Para2 = 0);

		//ˢ�»�����Ϣ
		virtual bool UpdateBaseData();

		//ˢ��Lod��Ϣ
		virtual bool UpdateLod();

		//����������Ϣ���»���
		virtual void UpdateDraw(DWORD Value = 0);

		//��Ⱦ�������������ݸ���
		virtual bool UpdateToProData();

		//LiuGH��ӣ�ɾ����ǰ��ά��ʾ�����ӽڵ�
		virtual bool RemoveChildNode();


		//����ˢ�¿���
		void SetUpdateSwitch(bool Value);

		//��������
		virtual void Show(BOOL bShow);

	protected:
		virtual void Update(osg::NodeVisitor* nv, double Time, double LastTime);

		osg::ref_ptr<CNCObj> m_NCObj;
		osg::ref_ptr<CTechniqueNodeCallback> m_ctechniqueNodeCallback;

		/////////////////////////////////////λ�õ�����ط���//////////////////////////////////////
	public:
		//���úͻ�ȡλ��
		void SetPos(osg::Vec3d &pos);
		//����ɸ�д,�����Լ���position
		virtual void GetPos(osg::Vec3d &pos);

		//���úͻ�ȡ��̬
		void SetPose(osg::Vec3f &axisAt, osg::Vec3f &axisUp);
		void GetPose(osg::Vec3f &axisAt, osg::Vec3f &axisUp);

		void SetPosAndPose(osg::Vec3d &pos, osg::Vec3f &axisAt, osg::Vec3f &axisUp);

		//�������ű���
		void SetScale(osg::Vec3f Scale);
		osg::Vec3f GetScale();

		//����λ�ýڵ�
		osg::MatrixTransform *GetMT(){return m_MT.get();}

		//����LOD��λ��
		void SetLodPos(osg::Vec3d &pos);

	protected:
		void ApplyMatrix();
		//λ�ýڵ�
		osg::ref_ptr<osg::MatrixTransform> m_MT;
		osg::Vec3d m_Pos;
		osg::Vec3f m_Scale, m_AxisAt, m_AxisUp;
		//λ�ýڵ��Ƿ���Ч
		bool m_IsPosAndPoseAble;

		/////////////////////////////////////�༭��ط���//////////////////////////////////////
	public:
		void	ShowEditDlg();

		//��ʼ�༭������UI�̵߳���
		virtual bool BeginEditUI();
		//��ʼ�༭��������Ⱦ�̵߳���
		virtual bool BeginEdit3D();

		//��ʼ�༭������UI�̵߳���
		virtual bool EndEditUI();
		//�����༭��������Ⱦ�̵߳���
		virtual bool EndEdit3D();
		/////////////////////////////////////�۲����ط���//////////////////////////////////////
	public:
		//ģ��Ԥ��
		void ShowModelViewDlg();
	public:
		void SetEyePosAndPose(osg::Vec3d EyeLLA, osg::Vec3 EyeAngle);
		void GetEyePosAndPose(osg::Vec3d &EyeLLA, osg::Vec3 &EyeAngle);

		/*
		Ŀ�ģ����ɼ���Ĺ۲���λ�ú���̬(����ɸ�д֮)
		������EyeLLA	(out)	��γ��
		EyeAngle	(out)	��̬
		���أ��ɹ�����true�����򷵻�false
		*/
		virtual bool GenEyePosAndPose(osg::Vec3d &EyeLLA, osg::Vec3 &EyeAngle);

	protected:
		//�۲��λ�ú���̬
		osg::Vec3d m_EyeLLA, m_EyePos;
		osg::Vec3 m_EyeAngle, m_EyeAxisAt, m_EyeAxisUp;
		//�۲���Ƿ���Ч
		bool m_IsEyeAble;	
		/////////////////////////////////////������ط���//////////////////////////////////////
	public:
		//��ѡ
		virtual bool PtSelect(double &x, double &y, osg::Vec3d& eye, osg::Vec3d& vec);
		/************************************************************************/
		/* ��ײ���
		startArray �����㼯�ϣ���Ҫ���յ�һһ��Ӧ
		endArray   ����յ㼯�ϣ���Ҫ�����һһ��Ӧ
		collisionDectionArray  ��ײ�㼯��
		���� bool ����ײ����true �޷���false
		2013-03-04 xubaolong
		*/
		/************************************************************************/
		virtual bool CollisionDetection(std::vector<osg::Vec3d> &startArray,std::vector<osg::Vec3d> &endArray,std::vector<osg::Vec3d> &collisionDectionArray);
		//���Ӻ�ɾ���ڵ�
		bool addChild(Node *child);
		bool removeChild( Node *child );

		//�����ӽڵ�
		bool AddNodeFromFile(CString strFile);

		//ѡ�е�
		osg::Vec3d m_SelCrossPt;

		//�������ö���
		osg::ref_ptr<COM::CLod> m_Lod;
		osg::ref_ptr<COM::CLod> old_LOD;
		float old_LODPrentID;

		bool m_IsLodAble;	
		bool m_bHide;

		BOOL GetObjVisible();
		void SetObjVisible(BOOL bVisible);

		bool getHilightStype(){return m_bhilight;}

		//�ж϶�����Դ�ļ��Ƿ����
		virtual bool IsExistRes(){return true;}

		//ɾ��������Դ
		virtual void DelRes(){}

		bool getFlicCount(){
			if (m_flickCount>0)
			{
				return false;
			}
			else
			{
				return true;
			}
		}


	protected:
		osg::ref_ptr<Data::CRowShowObj> m_RowShowObj;

		//״̬����
		std::vector<osg::ref_ptr<osg::StateSet>> m_stateStack;	//״̬ջ
		//��˸����
		double	m_flickLastTime;	//�ϴ��л���ʱ��
		DWORD	m_flickCount;		//��˸����
		bool	m_bResExist;		//�ļ���Դ���ڱ�ʶ
		osg::Vec4 stateSetColor1;
		osg::Vec4 stateSetColor2;

		bool    m_bhilight;         //�Ƿ��ڸ���״̬ 

		//UI�༭
	protected:
		static bool m_IsEdit;
		static EMObj::CEditPropSheet * m_pEditPropSheet;

		/////////////////////////////////////��Ϣ������ط���//////////////////////////////////////
	public:

		void SendMsg(DWORD EventType, DWORD Message, DWORD Para1 = 0, DWORD Para2 = 0, CString strXML = _T(""));
	};

	//��������
	//��ȡ����״̬
	osg::StateSet *GetHightlightStateSet();
	osg::StateSet *GetHightlightStateSet(osg::Vec4d emission,osg::Vec4d diffuse);
	//��ȡ��͸״̬
	osg::StateSet *GetTransparentStateSet();

}
