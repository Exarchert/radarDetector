/********************************************************
File:			ScriptGenerate.h	ScriptGenerate.cpp
Description:	�ű����ɹ���
Author:			Liugh
Date:			2011.1.25
*********************************************************/
#pragma once
#include <CartoonObject/Export.h>
#include <osg/Referenced>
#include <CartoonObject/ScriptCreate/Cartoon/COM_Cartoon.h>

namespace CartoonObject
{
	////////////////////////////////////////////////////////////////////////////////////
	//���ж���Ԫ������
	enum CartType
	{
		Model = 0,		//ģ��
		Camera,			//���
		Voice,			//��Ч
		RunLine,		//�˶���
		Table,			//����
		Text,			//����(2D��3D)
		ScrollText,		//��������
		Image,			//ͼƬ(��ĻͼƬ��avi��gif��swf)
		Water,			//��ˮ
		Spray,			//ˮĻ
		Fire,			//��
		Flow,			//����
		Cmd				//����(��ʾ�����������ű�����)
	};

/////////////////////////////////////////////////////////////////////////////////////
	//�ű�����ģʽ,����ģʽ:	��������Ԫ˳�򲥷�;	����ģʽ:	��ʱ���Ქ��
	enum PlayMode
	{
		Serial,		//����
		Parallel	//����
	};
/////////////////////////////////////////////////////////////////////////////////////
	//�洢һ���������̬
	struct PosAndPose
	{
		osg::Vec3d _pos;
		osg::Vec3 _pose;
	};

	//�洢һ�����������̬�б���·��ȫ��
	struct PosAndPoseArray
	{
		std::vector<PosAndPose> _posList;
		float	_dist;
	};

	//�洢һ��������б���ȫ��
	struct PtArray
	{
		osg::ref_ptr<osg::Vec3Array> _ptList;
		float	_dist;
	};

///////////////////////////////////////////////////////////////////////////////////////////////////
/************************************************************************
	�����ű�������
	ʹ�÷�����	1��SetGroupName(...)	�ɲ��õ���
				2��SetPlayMode(...)		�ɲ��õ��ã����ɽű�Ĭ��Ϊ����
				3��������Ӻ�����Add...(...)
				4��GenerateScript(...)
************************************************************************/
	class CARTOONOBJECT_EXPORT CScriptGenerate : public osg::Referenced
	{
	public:
		CScriptGenerate();
		~CScriptGenerate();

	public:
		//����
		void SetGroupName(const CString gpName);

		//0:����,1:����
		void SetPlayMode(UINT type = Serial);

		/*
		Ŀ�ģ�	�ű����ɽӿ�
		������	fileName		(in)		�洢��Ӳ���ϵ��ļ�����(����·��)
		*/
		bool GenerateScript(CString fileName);

	public:
		/*************************************************************************************************
		˵��:	1��	��Դ�ļ���Ŀ¼Ϊ��Ŀ·���µ�scriptĿ¼,�ű������õ�����Դ�ļ����������·��,
					��ά�����Ϊ��������
				2��	����·���˶�Ŀǰֻ֧��ȫ�������˶�,��������˶��ɺ��ڽ�������
		**************************************************************************************************/

		/*
		Ŀ�ģ�	��ű������һ��ģ�Ͷ���
		������	FilePath			(in)		ģ���ļ�·��(���·��)
				life				(in)		ʱ��
				posAndPoseArray		(in)		ģ���˶��ؼ�������(��������)��ÿ���Ӧ����̬�б�
				btm					(in)		��ʼʱ��
		*/
		bool AddModel(CString FilePath, const float life, PosAndPoseArray posAndPoseArray,const float btm = 0.0f);
		//����ǰ��
		bool AddModel(CString FilePath, const float life, PtArray ptArray,const float btm = 0.0f);

		/*
		Ŀ�ģ�	��ű������һ���������
		������	life				(in)		ʱ��
				posAndPoseArray		(in)		����˶��ؼ�������(��������)��ÿ���Ӧ����̬�б�
				btm					(in)		��ʼʱ��
		*/
		bool AddCamera(const float life, PosAndPoseArray posAndPoseArray,const float btm = 0.0f);

		/*
		Ŀ�ģ�	��ű������һ����Ч
		������	FilePath			(in)		��Ƶ�ļ�·��
				life				(in)		ʱ��
				voice				(in)		����
				pos					(in)		��Դ����
				bBind				(in)		true:	��������˶�
				btm					(in)		��ʼʱ��
		*/
		bool AddVoice(const CString FilePath, const float life, float voice = 0.5, const osg::Vec3 pos = osg::Vec3(0.0,0.0,0.0), bool bBind = false,
			const float btm = 0.0f);

		/*
		Ŀ�ģ�	��ű������һ���˶���
		������	life				(in)		ʱ��
				ptArray				(in)		�˶��߹ؼ�������
				color				(in)		�˶�����ɫ
				lineWidth			(in)		�߿�
				btm					(in)		��ʼʱ��
		*/
		bool AddRunLine(const float life, PtArray ptArray, osg::Vec4 color= osg::Vec4(0.0f,1.0f,0.0f,1.0f), 
			float lineWidth = 2.0, const float btm = 0.0f);
		/*
		Ŀ�ģ�	��ű������һ������
		������	content				(in)		��ʾ����
				life				(in)		ʱ��
				posAndPoseArray		(in)		���ƿռ��˶��ؼ�������
				size				(in)		���ִ�С
				font				(in)		����
				offset				(in)		��������β��ͬ��ע���ƫ����(��)
				color				(in)		������ɫ
				btm					(in)		��ʼʱ��
		*/
		bool AddTable(const CString content, const float life, PtArray ptArray, const float size = 30, 
			CString font = _T("SIMSUN.TTC"), const osg::Vec4 color= osg::Vec4(0.0f,1.0f,0.0f,1.0f), const osg::Vec2 offset= osg::Vec2(0.0,0.0), 
			const float btm = 0.0f);
		/*
		Ŀ�ģ�	��ű������һ����Ļ����
		������	content				(in)		��ʾ����
				life				(in)		ʱ��
				screenPos			(in)		�������½���Ļ����
				color				(in)		������ɫ
				size				(in)		���ִ�С
				font				(in)		����
				bHasBKRect			(in)		�Ƿ���������(trueΪ����)
				bkColor				(in)		��������ɫ
				btm					(in)		��ʼʱ��
		*/
		bool Add2DText(const CString content, const float life, osg::Vec2 screenPos,const osg::Vec4 color= osg::Vec4(0.0f,1.0f,0.0f,1.0f), 
			const float size = 30, const CString font = _T("SIMSUN.TTC"), const bool bHasBKRect = true, 
			osg::Vec4 bkColor= osg::Vec4(0.5,0.5,0.5,0.5), const float btm = 0.0f);

		/*
		Ŀ�ģ�	��ű������һ���ռ�����
		������	content				(in)		��ʾ����
				life				(in)		ʱ��
				posAndPoseArray		(in)		���ֿռ��˶��ؼ�������
				color				(in)		������ɫ
				size				(in)		���ִ�С
				font				(in)		����
				bHasBKRect			(in)		�Ƿ���������(trueΪ����)
				bkColor				(in)		��������ɫ
				btm					(in)		��ʼʱ��
		*/
		bool Add3DText(const CString content, const float life, PtArray ptArray, const osg::Vec4 color= osg::Vec4(0.0f,1.0f,0.0f,1.0f), 
			const float size = 30, const CString font = _T("SIMSUN.TTC"), const bool bHasBKRect = true, 
			osg::Vec4 bkColor= osg::Vec4(0.5,0.5,0.5,0.5), const float btm = 0.0f);

		/*
		Ŀ�ģ�	��ű������һ����������
		������	content				(in)		��ʾ����
				life				(in)		ʱ��
				screenPos			(in)		������������ĵ�ƫ����
				color				(in)		������ɫ
				size				(in)		���ִ�С				
				font				(in)		����
				fieldHeight			(in)		��������߶�
				speedf				(in)		�����ٶ�(����/s)
				lineSpace			(in)		�м��
				maxLines			(in)		�������ɵ��������
				align				(in)		���뷽ʽ,�������Ļ��ʾ����
												osgAPEx::ScrollText::Alignment alignTypes[] = 
												{
													osgAPEx::ScrollText::Left,
													osgAPEx::ScrollText::Right,
													osgAPEx::ScrollText::Center
												};
				bHasBKRect			(in)		�Ƿ�����Ļ������(trueΪ����)
				bkColor				(in)		��������ɫ
				btm					(in)		��ʼʱ��
		*/
		bool AddScrollText(CString content, const float life, osg::Vec2 offSet,osg::Vec4 color= osg::Vec4(0.0f,1.0f,0.0f,1.0f), 
			const float size = 30, CString font = _T("SIMSUN.TTC"),const float fieldHeight = 400, const float speedf = 0.05,
			const float lineSpace = 0,const float maxLines = 50, DWORD align = 0, bool bHasBKRect = TRUE,
			osg::Vec4 bkColor= osg::Vec4(0.5,0.5,0.5,0.5), const float btm = 0.0f);

		/*
		Ŀ�ģ�	��ű������һ����ĻͼƬ
		������	FilePat				(in)		ͼƬ��Դ(bmp��jpg��png�ȳ��þ�̬ͼƬ;avi��gif��swf��̬ͼƬ)
				life				(in)		ʱ��
				width				(in)		ͼƬ���
				height				(in)		ͼƬ�߶�
				centerMode			(in)		ͼƬ���ĵ�
												osgAPEx::CenterMode centerModes[] = 
												{
													osgAPEx::CENTER,
													osgAPEx::BOTTOM_LEFT,
													osgAPEx::BOTTOM_RIGHT,
													osgAPEx::TOP_LEFT,
													osgAPEx::TOP_RIGHT
												};
				screenPos			(in)		��Ļ����
				bEqualImage			(in)		��С��ԭͼƬ��ͬ(trueΪ��ͬ��falseΪwidth*height)
				bLoop				(in)		����ͼƬ�����Ƿ�ѭ������(trueΪѭ������)
				btm					(in)		��ʼʱ��
		*/
		bool AddImage(CString FilePath, const float life, const float width = 100.0f, const float height = 100.0f, 
			DWORD centerMode = 1, osg::Vec2 screenPos = osg::Vec2(100.0f,100.0f), bool bEqualImage = false, 
			bool bLoop = false,const float btm = 0.0f);

		/*
		Ŀ�ģ�	��ű������һ����ˮ
		������	ȫ��Ϊ�������
				CString				FilePath			��ͼ
				float				life				ʱ��
				double				particleLife 		������������
				PosAndPoseArray	posAndPoseArray		�ռ������˶��������б�
				float				minNum;				��С������ / ����ڰ뾶
				float				maxNum;				��������� / ��ͼ��
				float				minSpeed;			��С���ٶ� / ��ʼ�ٶ�
				float				maxSpeed;			�����ٶ� / �뾶������
				float				phi;				ָ��ˮƽ��Χ / ��ͼ����(����Ϊ����,����Ϊ����)
				float				theta;				ָ����ֱ��Χ / ��������
				int					type;				0������ˮ;1��Բ׶������
				float				dist;				ˮƽ����
				flost				btm					��ʼʱ��
		��ע��	���������غ��壬����ˮΪ��һ�㶨�塢Բ׶����ˮΪ�ڶ��㺬��
		*/
		bool AddWater(CString FilePath, const float life, const float particleLife, PosAndPoseArray posAndPoseArray, const float minNum = 80.0f, const float maxNum = 81.0f, 
			const float minSpeed = 80.0f, const float maxSpeed = 81.0f, const float phi = 2.0f, const float theta = 0.01f,
			const int type = 0, const float dist = 10.0f, const float btm = 0.0f);

		/*
		Ŀ�ģ�	��ű������һ��ˮĻ
		������	life				(in)		ʱ��
				posAndPoseArray		(in)		���ֿռ��˶��ؼ�������
				FilePath			(in)		��ͼ�ļ���
				repeatRation		(in)		��ͼ�ظ���
				speed				(in)		��ͼ����
				radius				(in)		Բ����Բ��뾶
				cylinderHeight		(in)		Բ���߶�
				sphereHeight		(in)		��߶�
				btm					(in)		��ʼʱ��
		��ע��	ˮĻģ���ɰ����Բ�������ۼӹ���,����߶Ȳ��ܳ���Բ��ֱ��
		*/
		bool AddSpray(const float life, PosAndPoseArray posAndPoseArray, CString FilePath = _T("shui.tga"),
			const float repeatRation = 5.0f, const float speedSpray = 0.01f, const float radius = 2.0f, 
			const float cylinderHeight = 2.0f, const float sphereHeight = 2.0f, const float btm = 0.0f);
		/*
		Ŀ�ģ�	��ű������һ����
		������	life					(in)		ʱ��
				posAndPoseArray			(in)		�ռ��˶��ؼ�������
				fireModel				(in)		��ģ���ļ�
				flamePic				(in)		��ͼ�ļ���
				particleSystemHeight	(in)		���Ӹ߶�
				particleSpeed			(in)		�����ٶ�
				particleSpread			(in)		���ӿ��
				particleSystemShape		(in)		�����С
				particleShape			(in)		����Ƭ��С
				particleSize			(in)		����Ƭ���
				btm						(in)		��ʼʱ��
		��ע��	ˮĻģ���ɰ����Բ�������ۼӹ���,����߶Ȳ��ܳ���Բ��ֱ��
		*/
 		bool AddFire(const float life, PosAndPoseArray posAndPoseArray,const CString &fireModel = _T("QuadArray.3ds"),
 			const CString &flamePic  = _T("Flame.tga"), float particleSystemHeight = 25.0f, float particleSpeed = 1.3f,
 			float particleSpread = 10.5f, float particleSystemShape = 0.2f, float particleShape = 1.5f, 
			float particleSize = 3.9f, const float btm = 0.0f);

		/*
		Ŀ�ģ�	��ű������һ������
		������	life				(in)		ʱ��
				PipeID				(in)		����ID
				FlowSpeed			(in)		����
				bClear				(in)		������Ƿ������������(TRUE:���)
				Color				(in)		����������ɫ
				bDirection			(in)		�Ƿ���������(TRUE:��������)
				btm					(in)		��ʼʱ��
		*/
  		bool AddFlow(const float life, LONG64 PipeID, float FlowSpeed, float radius, BOOL bClear = TRUE,
 			osg::Vec4 color = osg::Vec4(0.0,0.0,1.0,0.6), BOOL bDirection = FALSE,const float btm = 0.0f);

		/*
		Ŀ�ģ�	��ű������һ������
		������	srcCmd				(in)		��ʼʱ����������
				life				(in)		ʱ��
				srcClr				(in)		����ʱִ����������
				btm					(in)		��ʼʱ��
				strInfo				(in)		��ע��Ϣ(��Ҫ�����������ݽ���)
		��ע��	Ŀǰ����֧������
				1��	��ʾ�����������:
					1>	����������
						<���� ������="��ʾ�����������"  ID="" ��������="����/ѡ��/͸��/��˸/�������Ч��"  ����=""></����>
					2>	����˵����
						"����"��	���� / ��ʾ				0  /  ��������
									����ѡ�С����Ч��		����ֵ����Ϊ�ջ�ȥ���ýڵ�����
									��˸					��������˸������
									͸��		0~100��������ֵ��С��͸���ȳ����ȣ�0Ϊ��͸��100Ϊȫ͸��
				2��	�ű��������
					1>	����
						XML������
						<���� ������="����" ����="����" �ű���="���·��"></����>

		*/
		bool AddCmd(const CString srcCmd, const float life, const CString srcClr= _T(""),const float btm = 0.0f,const CString strInfo = _T(""));

	private:
		osg::ref_ptr<CCOM_CartoonUnit>	m_pCartUnit;
		CString		m_GroupName;
		UINT		m_PlayMode;
	};
}