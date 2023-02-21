/************************************************************************
Copyright (C)  
�ļ���3DImage.h 3DImage.cpp
���ߣ���־��
�汾��1.0
���ڣ�2009-11-25
��������ʾ����-�ռ�ͼƬ��.
				XML��ʽ����ʾ��
	<�ռ�ͼƬ>
			<λ�� X="0.f" Y="0.f" Z="0.f">						</λ��>
			<���� T="�����">									</����>
			<ͼƬ��С Type="�Զ���" Width="200" Height="300">	</ͼƬ��С>
			<���뷽ʽ Type="CENTER">							</���뷽ʽ>
			<ͼƬ����>����վ.png								</ͼƬ����>
	</�ռ�ͼƬ>

�޸ļ�¼��
����					����				����
----------------------------------------------------------------------

************************************************************************/

#pragma once

#include <Object\Export.h>
#include <Object\Obj.h>
#include <map>
#include <osg\Vec3>
#include <osg\Vec3d>
#include <osg\Billboard>
#include <osg\ImageStream>
#include <osgAPEx\BaseGeo>
#include <OpenThreads\Mutex>


namespace EMObj		{

class OBJECT_EXPORT CSO3DImage : virtual public CObj
{
public:
	CSO3DImage();
	virtual ~CSO3DImage();

	//��ʼ��
	virtual bool Init(CString &xml);

	//��ȡ������Ϣ
	virtual bool ReadProperty(MSXML2::IXMLDOMNodePtr pNode);
	virtual MSXML2::IXMLDOMNodePtr GetProperty();

	//�����༭�Ի���
	virtual bool BeginEditUI();

	//�����༭
	virtual bool EndEdit3D();

	//ˢ�»���
	virtual void UpdateDraw(DWORD Value = 0);

	//��ȡXML��������
	virtual CString GetXml();

	//��������
	virtual void Reset();

	//��ȡ��ҵID
	int GetEnterID() const;

	enum SizeType {
		EQUAL_PICTURE,
		USER_DEFINED
	};

	/*
	Ŀ�ģ����úͻ�ȡͼƬ��
	������szName	(in)	ͼƬ��
	���أ���
	*/
	void SetPicture(const CString &szName);
	CString GetPicture() const;

	/*
	Ŀ�ģ�����ͼƬ��С
	������type		(in)	����(�Զ��塢����ͼƬ)
	      width		(in)	��(�Զ�����Ч)
	      height	(in)	��(�Զ�����Ч)
	���أ���
	*/
	void SetSize(unsigned int type, float width, float height);
	void GetSize(unsigned int &type, float &width, float &height) const;

	/*
	Ŀ�ģ����ö���(ԭ��)��ʽ
	������type		(in)	���뷽ʽ
	���أ���
	*/
	void SetAlignType(unsigned int type);
	unsigned int GetAlignType() const;
	
	//���úͻ�ȡλ��&��̬
	void SetPosition(const osg::Vec3d &pos, const osg::Vec3f &pose);
	void GetPosition(osg::Vec3d &pos, osg::Vec3f &pose);

	//���úͻ�ȡͼƬ����̬����
	void SetPoseType(const CString &szType);
	CString GetPoseType() const;

	enum {
		SOType = 6
	};

	//����
	virtual DWORD GetType() {return SOType; }

	//��Ⱦ����
	virtual DWORD GetRenderType(){return Render_Type_Scene; }

	//��̬������Ա����
	static unsigned int RemapAlignType(const CString &name);
	static CString RemapAlignType(unsigned int type);
	static osg::Geometry* CreateImageRectangle(const osg::Image* image, const osg::Vec2* size, unsigned int mode, bool bRawSize);

	//ԭ����뷽ʽ
	enum ALIGNTYPE {
		CENTER,
		BOTTOM_LEFT,
		BOTTOM_RIGHT,
		TOP_LEFT,
		TOP_RIGHT,
		LEFT_CENTER,
		RIGHT_CENTER,
		TOP_CENTER,
		BOTTOM_CENTER
	};

protected:
	osg::Vec3d		m_position;		//λ��
	osg::Vec3f		m_pose;			//��̬
	CString			m_szPicture;	//ͼƬ����
	unsigned int	m_sizeType;		//��С����
	float			m_width;		//���
	float			m_height;		//�߶�
	unsigned int	m_alignType;	//���뷽ʽ
	CString			m_szPoseType;	//��̬����

	osg::ref_ptr<osg::Billboard>	m_billboard;
	osg::ref_ptr<osg::Geode>		m_geode;
	osg::ref_ptr<osg::ImageStream>	m_imageStream;
	//��̬������Ա����
	static std::map<CString, unsigned int> s_alignTypes;

	//������
	OpenThreads::Mutex	m_mutex;
};

}