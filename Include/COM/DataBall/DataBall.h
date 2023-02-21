/****************************************************************************
Copyright (C),  
�ļ���DataBall.h  DataBall.cpp
���ߣ���־��
�汾��1.0
���ڣ�2009-08-14
���������ݲɼ��࣬�ɼ���ά�����е���ʾ�����λ�ú���̬.

�޸ļ�¼��
����						����						����
-----------------------------------------------------------------------------
��־��						2009-11-26					�Ľ�Ϊ���û���������Ӧ��װ��
														���û��ṩ�����յĽ��.
��־��						2010-01-25					�������ò����Ľӿ�SetStep().
��־��						2010-02-03					���Ӱ�������ܣ���������ݲɼ�
��־��						2010-03-04					��������л�����ͼ����

****************************************************************************/

#pragma once 

#include <Com\Export.h>
#include <osg\Vec3>
#include <osg\Vec3d>
#include <osg\Matrixd>
#include <osg\Node>
#include <osgViewer\Viewer>
#include <osgGA\GUIEventAdapter>
#include <osgGA\GUIActionAdapter>
//#include <Com\Ramble\RambleManager.h>
#include <COM\Ramble\RambleMgr.h>


namespace COM	{

//CDataBall��
class COM_EXPORT CDataBall : public osg::Referenced
{
public:
	CDataBall();
	virtual ~CDataBall();

	/*
	Ŀ�ģ���ʼ��
	������pNode			(in)		������ʾ����(������������ж�, ��ΪNULL)
	      pos			(in)		����λ��x,y,z
	      at			(in)		��̬����
		  up			(in)		��̬����
		  pField		(in)		��������������, ��������϶�ʱ�Ķ�λ����	
		  localToWorld	(in)		����������ı任����, NULL��ʾʹ�õ�������ϵ(posΪ��γ��)
	���أ���
	*/
	void Init(osg::Node *pNode, const osg::Vec3d &pos, const osg::Vec3f &at, const osg::Vec3f &up, osg::Node *pField,
			osg::Matrixd *localToWorld=NULL);

	/*
	Ŀ�ģ���ʼ��
	������pNode			(in)		������ʾ����(������������жϣ���ΪNULL)
	      pos			(in)		����λ��x,y,z
	      yaw			(in)		��̬yaw
		  pitch			(in)		��̬pitch
	      roll			(in)		��̬roll
		  pField		(in)		��������������, ��������϶�ʱ�Ķ�λ����
		  localToWorld	(in)		����������ı任����, NULL��ʾʹ�õ�������ϵ(posΪ��γ��)
	���أ���
	*/
	void Init(osg::Node *pNode, const osg::Vec3d &pos, float yaw, float pitch, float roll, osg::Node *pField, 
		osg::Matrixd *localToWorld=NULL);

	//�������ι�����,������קʱ������������.
	void SetRambleManager(COM::CRambleMgr *rambleMan);

	//�����û�����
	bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);

	/*
	Ŀ�ģ����ò���
	��ע������������λ�ú���̬ʹ��
	*/
	void SetStep(float len);

	/*
	Ŀ��: ��������ͼ����
	*/
	void SetDistance(float dist) { m_distance = dist; }

	/*
	Ŀ�ģ��ж��������̬�Ƿ��Ѹı�
	���أ��ı䷵��true,���򷵻�false
	*/
	bool IsChanged() const;

	/*
	Ŀ�ģ���ȡ���
	������pos		(out)		����λ��x,y,z
	      at		(out)		��̬����
		  up		(out)		��̬����
	���أ���	
	*/
	void GetPosAndPose(osg::Vec3d &pos, osg::Vec3f &at, osg::Vec3f &up) const;

	/*
	Ŀ�ģ���ȡ���
	������pos		(out)		����λ��x,y,z
	      yaw		(out)		��̬yaw
	      pitch		(out)		��̬pitch
	      roll		(out)		��̬roll
	���أ���
	*/
	void GetPosAndPose(osg::Vec3d &pos, float &yaw, float &pitch, float &roll);

	/*
	Ŀ�ģ���ȡ���嵽��������ϵ�ı任����
	������localToWorld	(out)	�任����
	*/
	void GetLocalToWorld(osg::Matrixd &localToWorld) const;

	//��ȡ�ɼ���Ӧ�÷Ŵ�ı���(Ϊ�������ʱ���Զ��Ŵ�ʹ��)
	void GetScale(float &scale) const { scale = m_zoom; }

	void SetAllowDrag(bool bAllow = true){m_bAllowDrag = bAllow;}

protected:
	//����λ�ú���̬��д���ڴ�ӳ���ļ�
	void CalculateAndWrite2MMBuffer();

	/*
	Ŀ�ģ�д��λ�ú���̬���ݵ��ڴ�ӳ���ļ�
	������lon, lat, height	(in)	��γ��
		  yaw, pitch, roll	(in)	��̬��
	*/
	void Write2MMBuffer(double lon, double lat, double height, float yaw, float pitch, float roll);

protected:

	enum DIRECTION_MODE{
		USER_VIEW,			//�û��ӽ�
		FIELD_SYSTEM		//��������ϵ
	};

protected:
	HANDLE	m_hMMFile;
	LPVOID	m_pMMBuffer;

	//��ʾ����
	osg::Node *m_pNode;
	
	//�϶��󽻽��
	osg::Node *m_pField;

	//���ι�����
	//osg::ref_ptr<COM::CRambleManager> m_rambleMan;
	osg::ref_ptr<COM::CRambleMgr> m_rambleMan;

	//���İ���
	bool  m_bPicked;

	//�ƶ�����ģʽ
	DIRECTION_MODE				m_moveMode;

	//����
	osg::Vec3d					m_posLocal;			//�ڲο�����ϵ����(X, Y, Z)
	osg::Vec3f					m_poseLocal;		//�ڲο�����ϵ����(yaw, pitch, roll)
	float						m_scale;			//���ھ��ȱ���
	bool						m_bChanged;			//���ݸı��ʶ
	bool						m_bEarth;			//ʹ�õ�������ϵ
	osg::Matrixd				m_localToWorld;		//��������ϵ����������ϵ�ı任

	//�����
	bool						m_bBindWithCamera;
	bool						m_bUseXYZDirection;
	bool						m_bAllowDrag;	//�����϶�
	float						m_zoom;

	//����ͼ�۲����
	float						m_distance;
};

}