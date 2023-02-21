/**************************************************************************
File:			PathEdit.h
Author:			��ƽ    
Date:			2011-1-17
Description:    ·���༭ģ��
***************************************************************************/

#pragma once
#include <COM\Export.h>
#include <osgGA\GUIEventHandler>
#include <vector>
#include <osg/Group>
#include <osgDB/ReadFile>
#include <osg/Image>
#include <osgViewer/Viewer>
#include <Com/DataBall/DataBall.h>
#include <Com/KernelContext/KernelContext.h>

namespace COM{

	class COM_EXPORT CPathEdit : public osgGA::GUIEventHandler
	{
	public:
		CPathEdit(void);
		//�ؼ�����԰ڷ�һ��ģ�ͣ�strModelΪ����·��
		CPathEdit(CKernelContext* pContext,CString strModel = _T(""));
		~CPathEdit(void);
		bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);

		/*
		��ʼ��
		StartIco	(in)		����־ͼ�꣬�ռ�󶨵�Ĭ��Ϊͼ��ĵױ��е�
		EndIco		(in)		�յ��־ͼ�꣬�ռ�󶨵�Ĭ��Ϊͼ��ĵױ��е�
		FlagIve		(in)		�ڵ��־ģ��
		LineColor	(in)		������ɫ
		LineWidth	(in)		�������
		Viewer		(in)		
		EarthGroup	(in)		�󽻽ڵ�
		SceneGroup	(in)		��������ϵ�µĳ����ڵ㣬������ʾ�ڵ�����ģ��
		ScreenGroup	(in)		��Ļ�ڵ�
		*/
		bool Init(CString StartIco, CString EndIco, CString FlagIve, osg::Vec4 LineColor, int LineWidth, 
			osgViewer::Viewer* Viewer, osg::Group* EarthGroup, osg::Group* SceneGroup, osg::Group* ScreenGroup);

		/*
		����·����ʼ��
		pos		(in)		�����������
		*/
		void SetStartPos(osg::Vec3d pos);

		/*
		��ȡ·��ֵ
		Path		(out)		���·��������
		ValueType	(in)		��ֵ���ͣ�1��ʾ�������Ϊ�������꣬2��ʾ�������Ϊ��γ��
		*/
		bool GetPath(osg::Vec3dArray& Path, int ValueType = 1);

		bool GetPath(osg::Vec3dArray& Path, osg::Vec3dArray& poseVecs, int ValueType = 1);

		void RemovePath();

		float GetPathDist();
	protected:
		
		osg::Vec4	m_LineColor;
		int			m_LineWidth;
		osgViewer::Viewer*			m_Viewer;
		osg::ref_ptr<osg::Group>	m_EarthGroup;
		osg::ref_ptr<osg::Group>	m_SceneGroup;
		osg::ref_ptr<osg::Group>	m_ScreenGroup;

		osg::ref_ptr<osg::Node>		m_StartNode;		//ͼƬ���
		osg::ref_ptr<osg::Image>	m_StartImg;
		osg::ref_ptr<osg::Node>		m_EndNode;			//ͼƬ���
		osg::ref_ptr<osg::Image>	m_EndImg;
		osg::ref_ptr<osg::Node>		m_FlagNode;

		osg::ref_ptr<osg::Geode>	m_PathGeode;
		osg::ref_ptr<osg::MatrixTransform>	m_PathTrans;

		//·���ڵ�����
		std::vector<osg::Vec3d> m_PathPoints;
		std::vector<osg::Vec3> m_PathPoseS;
		int						m_PathPointsCount;
		float					m_PathDist;

	public:
		//������ʾ��(������ģ��)����ʾ��ǰ��λ�ú���̬	
		void SetHintBall(LPCTSTR szHint);

		//��ȡ��ʾ��Ľ��
		osg::Node * GetHintBall() const;

		//������ʾ��λ�ú���̬
		void UpdateHintBall(const osg::Matrixd &mat);


		//�Ƴ���ʾ��
		void RemoveHintBall();


		//�����ʾ��
		void AddHintBall();

		void SetPathEditActive(bool bActive){m_bActive = bActive;}
		bool GetPathEditActive(){return m_bActive;}

		void GetCurPosAndPose(osg::Vec3d& pos,osg::Vec3& pose);

		float GetDataBallRadius(){return m_dataBallRadius;}
	public:
		//��ʾ��(������ģ��)
		COM::CDataBall	m_dataBall;		//�����û��������λ��
	private:
		//�ؼ��㴦�ڷ�ģ��
		float	m_dataBallRadius;
		CString	m_ModelName;
		osg::ref_ptr<CKernelContext>	m_pContext;
		osg::ref_ptr<osg::MatrixTransform>	m_DBTrans;	//�任����
		osg::ref_ptr<osg::MatrixTransform>	m_MTrans;	//�任����

		osg::Vec3d	m_curPos;
		osg::Vec3	m_curPose;
		bool		m_bDataBallShowed;
		bool		m_bActive;
	};

}