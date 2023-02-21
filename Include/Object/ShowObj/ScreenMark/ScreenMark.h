/************************************************************************
Copyright (C)  
�ļ���ScreenMark.h ScreenMark.cpp
���ߣ���־��
�汾��1.0
���ڣ�2009-12-01
��������ʾ����-��Ļ��ע��.
					XML��ʽ����ʾ��
	<��Ļ��ע>
		<�ռ�λ�� X="0" Y="0" Z="0">			</�ռ�λ��>
		<������ɫ R="255" G="0" B="0" A="255">	</������ɫ>
		<���ִ�С S="1">						</���ִ�С>
		<�ֱ��� WIDTH="128" HEIGHT="128">		</�ֱ���>
		<�������� T="NONE">						</��������>
		<������ɫ R="255" G="0" B="0" A="255">	</������ɫ>
		<ԭ����뷽ʽ ALIGN="LEFT_TOP">			</ԭ����뷽ʽ>
		<���� FONT="SIMSUN.TTC">				</����>
		<��ʽģ�� NAME="">						</��ʽģ��>
		<�ı�����>
				����վ
		</�ı�����>

		<ͼƬ��С Type="�Զ���" Width="200" Height="300">	</ͼƬ��С>
		<���뷽ʽ Type="CENTER">							</���뷽ʽ>
		<ͼƬ����>����վ.png								</ͼƬ����>
	</��Ļ��ע>

	<��ʽģ��>
		<������ɫ R="255" G="0" B="0" A="255">	</������ɫ>
		<���ִ�С S="1">						</���ִ�С>
		<�ֱ��� WIDTH="128" HEIGHT="128">		</�ֱ���>
		<�������� T="NONE">						</��������>
		<������ɫ R="255" G="0" B="0" A="255">	</������ɫ>
		<ԭ����뷽ʽ ALIGN="LEFT_TOP">			</ԭ����뷽ʽ>
		<���� FONT="SIMSUN.TTC">				</����>
	</��ʽģ��>

�޸ļ�¼��
����					����				����
----------------------------------------------------------------------

************************************************************************/

#pragma once

#include <map>
#include <Object\Export.h>
#include <Object\Obj.h>
#include <osg\Vec3d>
#include <osg\ref_ptr>
#include <osg\Switch>
#include <osgText\Text>
#include <osg\Geode>
#include <OpenThreads\Mutex>


namespace EMObj		{

class OBJECT_EXPORT CSOScreenMark : public CObj
{
public:
	CSOScreenMark();
	virtual ~CSOScreenMark();
	//��ʼ��
	virtual bool Init(CString &xml);

	//��ȡ��ʽģ��
	void ReadFromTemplate(const CString &szTempName);

	//��ȡ������Ϣ
	virtual bool ReadProperty(MSXML2::IXMLDOMNodePtr pNode);
	virtual MSXML2::IXMLDOMNodePtr GetProperty();

	//��ȡ��ҵID
	virtual int GetEnterID() const;

	//�����༭�Ի���
	virtual bool BeginEditUI();

	//�����༭
	virtual bool EndEdit3D();

	//ˢ�»���
	virtual void UpdateDraw(DWORD Value = 0);

	//��ѡ
	virtual bool PtSelect(double &x, double &y, osg::Vec3d& eye, osg::Vec3d& vec);

	//��ȡXML��������
	virtual CString GetXml();

	//��������
	virtual void Reset();

	//���úͻ�ȡ��ʽģ������
	void SetTemplateName(const CString &szTempName);
	CString GetTemplateName() const;

	//���úͻ�ȡ��������
	void SetContent(const CString &szContent);
	CString GetContent() const;

	//���úͻ�ȡ������
	void SetFont(const CString &szFont);
	CString GetFont() const;

	//���úͻ�ȡ��ɫ
	void SetColor(const COLORREF &rgb, BYTE alpha);
	void GetColor(COLORREF &rgb, BYTE &alpha) const;

	//���úͻ�ȡ���ִ�С
	void SetSize(float size);
	float GetSize() const;

	//���úͻ�ȡ���ֱַ���
	void SetResolution(unsigned int width, unsigned int height);
	void GetResolution(unsigned int &width, unsigned int &height) const;

	//���úͻ�ȡ��������
	void SetBackDropType(unsigned int bkDropType);
	unsigned int GetBackDropType() const;

	//���úͻ�ȡ������ɫ
	void SetDropColor(const COLORREF &rgb, BYTE alpha);
	void GetDropColor(COLORREF &rgb, BYTE &alpha) const;

	//���úͻ�ȡ���ֵ�ԭ����뷽ʽ
	void SetAlignment(unsigned int align);
	unsigned int GetAlignment() const;

	//���úͻ�ȡ�ռ�λ��
	void SetPosition(const osg::Vec3d &pos);
	osg::Vec3d GetPosition();

	void GetPos(osg::Vec3d &pos);

	//���úͻ�ȡ�ı���ƫ��
	void SetTextOffset(float offsetX, float offsetY);
	void GetTextOffset(float &offsetX, float &offsetY);

	//���úͻ�ȡ�ı�����Ӱƫ��
	void SetDropOffset(float offsetX, float offsetY);
	void GetDropOffset(float &offsetX, float &offsetY);

	enum {
		SOType = 12
	};

	//����
	virtual DWORD GetType() {return SOType; }

	//��Ⱦ����
	virtual DWORD GetRenderType(){return Render_Type_Screen; }

	//�����Ծ�̬��Ա����
	//�������Ͳ���
	static unsigned int RemapBkType(const CString &name);
	static CString RemapBkType(unsigned int type);
	static std::map<CString, unsigned int> &GetBkTypesMap();

	//ԭ����뷽ʽ����
	static unsigned int RemapOriginType(const CString &name);
	static CString RemapOriginType(unsigned int type);
	static std::map<CString, unsigned int> &GetOriginTypesMap();

///////////////////////////ͼƬ����//////////////////////////////////////

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

	void SetPicOffset(float offsetX, float offsetY);
	void GetPicOffset(float &offsetX, float &offsetY);

	//��̬������Ա����
	static unsigned int RemapAlignType(const CString &name);
	static CString RemapAlignType(unsigned int type);
	static osg::Geometry* CreateImageRectangle(const osg::Image* image, const osg::Vec2* size, unsigned int mode,
							float offsetX, float offsetY);

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

	/*
	Ŀ�ģ����ɼ���Ĺ۲���λ�ú���̬(����ɸ�д֮)
	������EyeLLA	(out)	��γ��
	      EyeAngle	(out)	��̬
	���أ��ɹ�����true�����򷵻�false
	*/
	virtual bool GenEyePosAndPose(osg::Vec3d &EyeLLA, osg::Vec3 &EyeAngle);

protected:
	//��˸���ƣ��ɸ�д����Ϊ
	virtual void FlickPulse(bool bflag);
	virtual void FlickRestore();

protected:
	osg::Vec3d		m_position;		//λ��
	//����
	CString			m_templateName;	//��ʽģ��
	CString			m_szContent;	//��������
	CString			m_szFont;		//������
	COLORREF		m_rgb;			//��ɫ
	BYTE			m_alpha;		//͸����
	float			m_size;			//���ִ�С
	unsigned int	m_width;		//����ֱ���
	unsigned int	m_height;		//����ֱ���
	unsigned int	m_bkDropType;	//��������
	COLORREF		m_bkDropColor;	//������ɫ
	BYTE			m_bkDropAlpha;	//����͸����
	unsigned int	m_align;		//ԭ����뷽ʽ
	float			m_textOffsetX;	//��Ļƫ��X
	float			m_textOffsetY;	//��Ļƫ��Y
	float			m_dropOffsetX;	//��Ӱƫ��X
	float			m_dropOffsetY;	//��Ӱƫ��Y
	
	osg::ref_ptr<osgText::Text>	m_textNode;	//���ֽ��
	osg::ref_ptr<osg::Switch>	m_switch;

	//ͼƬ
	CString			m_szPicture;	//ͼƬ����
	unsigned int	m_sizeType;		//��С����
	float			m_picWidth;		//���
	float			m_picHeight;	//�߶�
	unsigned int	m_alignType;	//���뷽ʽ
	float			m_picOffsetX;	//��Ļƫ��X
	float			m_picOffsetY;	//��Ļƫ��Y

	osg::ref_ptr<osg::Geode>	m_pic;		//ͼƬ���

	//�����Եľ�̬��Ա����
	static std::map<CString, unsigned int> s_bkType;		//��������
	static std::map<CString, unsigned int> s_originType;	//ԭ����뷽ʽ
	static std::map<CString, unsigned int> s_alignTypes;	//ͼƬ���뷽ʽ

	//������
	OpenThreads::Mutex	m_mutex;
};

}