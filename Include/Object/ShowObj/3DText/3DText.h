/************************************************************************
Copyright (C)  
�ļ���3DText.h 3DText.cpp
���ߣ���־��
�汾��1.0
���ڣ�2009-11-23
��������ʾ����-�ռ�������.
						XML��ʽ����ʾ��
	<�ռ�����>
		<λ�� X="0" Y="0" Z="0">				</λ��>
		<��̬ Y="0" P="0" R="0">				</��̬>
		<������ɫ R="255" G="0" B="0" A="255">	</������ɫ>
		<���ִ�С S="1">						</���ִ�С>
		<�ֱ��� WIDTH="128" HEIGHT="128">		</�ֱ���>
		<�������� T="NONE">						</��������>
		<������ɫ R="255" G="0" B="0" A="255">	</������ɫ>
		<ԭ����뷽ʽ ALIGN="LEFT_TOP">			</ԭ����뷽ʽ>
		<����뷽ʽ AXIS="SCREEN">				</����뷽ʽ>
		<���� FONT="SIMSUN.TTC">				</����>
		<�ı�����>
			���, ���Ǹ��ռ����ֵ�XML����ʾ��.
			Hello World!
		</�ı�����>
	</�ռ�����>

�޸ļ�¼��
����					����				����
----------------------------------------------------------------------
��־��					2010-01-22			���ӻ�ȡλ�ú���̬�Ľӿ�(�̰߳�ȫ)

************************************************************************/

#pragma once

#include <Object\Export.h>
#include <Object\Obj.h>
#include <map>
#include <osg\Vec3d>
#include <osg\ref_ptr>
#include <osgText\Text>
#include <OpenThreads\Mutex>


namespace EMObj		{

class OBJECT_EXPORT CSO3DText : virtual public CObj
{
public:
	CSO3DText();
	virtual ~CSO3DText();
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

	//���úͻ�ȡ����뷽ʽ
	void SetAxisAlignment(unsigned int axisAlign);
	unsigned int GetAxisAlignment() const;

	//���úͻ�ȡ����λ��
	void SetPosition(const osg::Vec3d &pos, const osg::Vec3f &pose);
	void GetPosition(osg::Vec3d &pos, osg::Vec3f &pose);

	//���úͻ�ȡ��Ӱƫ��
	void SetDropOffset(float offsetX, float offsetY);
	void GetDropOffset(float &offsetX, float &offsetY);

	enum {
		SOType = 5
	};

	//����
	virtual DWORD GetType() {return SOType; }

	//��Ⱦ����
	virtual DWORD GetRenderType(){return Render_Type_Scene; }

	//�����Ծ�̬��Ա����
	//�������Ͳ���
	static unsigned int RemapBkType(const CString &name);
	static CString RemapBkType(unsigned int type);
	static std::map<CString, unsigned int> &GetBkTypesMap();

	//ԭ����뷽ʽ����
	static unsigned int RemapOriginType(const CString &name);
	static CString RemapOriginType(unsigned int type);
	static std::map<CString, unsigned int> &GetOriginTypesMap();

	//����뷽ʽ����
	static unsigned int RemapAxisType(const CString &name);
	static CString RemapAxisType(unsigned int type);
	static std::map<CString, unsigned int> &GetAxisTypesMap();

protected:
	osg::Vec3d		m_position;		//λ��
	osg::Vec3f		m_pose;			//��̬
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
	unsigned int	m_axisAlign;	//����뷽ʽ
	float			m_dropOffsetX;	//��Ӱƫ��X
	float			m_dropOffsetY;	//��Ӱƫ��Y

	osg::ref_ptr<osgText::Text>	m_textNode;	//���ֽ��

	//�����Եľ�̬��Ա����
	static std::map<CString, unsigned int> s_bkType;		//��������
	static std::map<CString, unsigned int> s_originType;	//ԭ����뷽ʽ
	static std::map<CString, unsigned int> s_axisType;		//����뷽ʽ

	//������
	OpenThreads::Mutex	m_mutex;
};

}
