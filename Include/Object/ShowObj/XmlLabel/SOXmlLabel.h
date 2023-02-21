/************************************************************************
Copyright (C),  
�ļ���SOXmlLabel.h  SOXmlLabel.cpp
���ߣ���־��
�汾��1.0
���ڣ�2009-09-10
�����������࣬ͨ��XML��ʽ���ñ��Ʋ��֡���ɫ����ʾ����. XML�ĸ�ʽ���£�

			<���� name = "�ҵı���">
				<λ�� X="0" Y="0" Z="0"></λ��>
				<���� rows = "3" cols = "5" hmargin = "20" vmargin = "20" rowspace = "10" colspace = "10"></����>
				<������ R="0.8f" G="0.8f" B="0.8f" A="0.5f" handleWidth="0.5f" offsetX="30.f" offsetY="30.f"></������>
				<���� imageFolder="������Ȼ"></����>
				<����>
					<����x_y>
						<λ�� row = "x" col = "y"></λ��>
						<������ fontName = "SIMFANG.TTF"></������>
						<�����С fontSize = "20"></�����С>
						<��ɫ R = "1.f" G = "0.f" B = "0.f" A = "1.f"></��ɫ>
						<���뷽ʽ hor = "����\�����\�Ҷ���" ver = "����\������\�׶���"></���뷽ʽ>
						<�ı�>
							��ã�������������...
						</�ı�>
					</����x_y>

					�������� ....

				</����>
			</����>


�޸ļ�¼��
����					����					����
-------------------------------------------------------------------------
��־��				2009-09-28				��ӹ��ܣ���ѯĳ��ĳ�е�ֵ

************************************************************************/

#pragma once

#include <Object\Export.h>
#include <Object\Obj.h>
#include <osg\Vec4>
#include <osg\Vec2>
#include <osg\Vec3d>
#include <osg\ref_ptr>
#include <osg\Referenced>
#include <osg\Image>
#include <osgText\Font>
#include <osgDB\WriteFile>
#include <osgAPEx\TableLabel>
#include <osgAPEx\ImageManager>
#include <osgWidget\Widget>
#include <osgWidget\WindowManager>
#include <osgWidget\ViewerEventHandlers>
#include <osgViewer\ViewerEventHandlers>
#include <osg\GraphicsContext>
#include <osg\Camera>
#include <osgWidget\WindowManager>


namespace EMObj		{

class OBJECT_EXPORT CSOXmlLabel : public CObj
{
public:
	CSOXmlLabel(osgWidget::WindowManager *winMan, osgViewer::Viewer *viewer);
	virtual ~CSOXmlLabel();

	/*
	Ŀ�ģ���ʼ�����ƵĲ��ֺ���ʾ
	������params		(in)	XML�ַ���
	      bChangeLayOut	(in)	�����Ƿ�仯, ����ڶ��εĲ��ֲ��������Ϊ�񣬱�����˸
	���أ�void
	*/
	void Init(CString params, bool bChangeLayout = true);

	/*
	Ŀ�ģ���ʼ�����ƵĲ���
	*/
	void Init(const CString &name, unsigned int rows, unsigned int cols, float hmargin, float vmargin, float rowspace, float colspace,
		osg::Vec4 triaColor, float handleWidth, float offsetX, float offsetY);

	/*
	Ŀ�ģ�����ĳ�����ӵ��ı�
	������row			(in)	�к�
	      col			(in)	�к�
	      font			(in)	������
	      fontSize		(in)	�����С
	      fontColor		(in)	������ɫ
		  horAlignType	(in)	ˮƽ���뷽ʽ
	      verAlignType	(in)	��ֱ���뷽ʽ
	      text			(in)	�ı�
	���أ�void
	*/
	void SetText(unsigned int row, unsigned int col, const CString &fontName, float fontSize, osg::Vec4 color, unsigned int horAlignType,
				unsigned int verAlignType, const CString &text);

	/*
	Ŀ�ģ����¼�����ƵĴ�С�������ı��������.
	*/
	void Resize();

	//��ѡ
	virtual bool PtSelect(double &x, double &y, osg::Vec3d& eye, osg::Vec3d& vec);

	/*
	Ŀ�ģ����ñ���λ��
	*/
	void SetPosition(osg::Vec3d pos);
	//ֱ�����ñ�����������ϵλ��(�������λ��)
	void SetWorldPosition(const osg::Vec3d &posWorld);

	osg::Vec3d GetPosition() const;

	//��ȡ�������ƫ��
	osg::Vec2 GetOffset();

	/*
	Ŀ�ģ���ȡ���Ƶ���&��.
	������rows		(out)	����
	      cols		(out)	����
	���أ��ɹ�����true�����򷵻�false
	*/
	bool GetRowsCols(unsigned int &rows, unsigned int &cols);

	/*
	Ŀ�ģ���ȡĳ��ĳ�е�����
	������row		(in)	������
	      col		(in)	������
	      value		(out)	�����ַ���
	���أ��ɹ�����true�����򷵻�false
	*/
	bool GetValue(unsigned int row, unsigned int col, CString &value);

	//���뷽ʽ���ֺͱ�ʶ��ӳ��
	//bhor	(in)	true��ʾˮƽ����false��ʾ��ֱ����	
	static unsigned int RemapAlignName2Value(const CString &name, bool bhor=true);
	static CString RemapValue2AlignName(unsigned int value, bool bhor=true);

	//////////////////////////////////////////////////////////////////////////
	//��дCObj��Ϊ
	//��ȡ������Ϣ
	virtual bool ReadProperty(MSXML2::IXMLDOMNodePtr pNode);
	virtual MSXML2::IXMLDOMNodePtr GetProperty();

	//�����༭�Ի���
	virtual bool BeginEditUI();

	//�����༭
	virtual bool EndEdit3D();

	//ˢ�»���
	virtual void UpdateDraw(DWORD Value = 0);

	//��������
	virtual void Show(BOOL bShow);

	//��ȡXML��������
	virtual CString GetXml();
	virtual void SetXml(const CString &xml);

	//��������
	virtual void Reset();

	enum {
		SOType = 17
	};

	//����
	virtual DWORD GetType() {return SOType; }

	//��Ⱦ����
	virtual DWORD GetRenderType(){return Render_Type_Screen; }

	//ˢ�»�����Ϣ
	virtual bool UpdateBaseData();

protected:
	/*
	Ŀ�ģ����ر���ͼƬ
	����: x		(in)	���
	      y		(in)	�߶�
	      list	(out)	ͼ���б�	
	���أ��ɹ�����true�����򷵻�false
	*/
	bool LoadImages(float x, float y,osgAPEx::ImageManager::SubImageList* list);

	/*
	Ŀ�ģ����ñ���ͼƬ
	������pImage	(in)	ͼ�����ָ��
	����: void
	*/
	void SetBackImage(void *, osg::Referenced *pImage);

	/*
	Ŀ�ģ����ɼ�ֵ
	������row	(in)	������
	      col	(in)	������
	���أ���ֵ
	*/
	ULONG64 MakeKey(unsigned int row, unsigned int col)
	{
		return ((((ULONG64)(row)) << 32) | ((ULONG64)(col)));
	}

	void SetTableLabelHyLine(unsigned int row,unsigned int col);

protected:
	osg::ref_ptr<osgWidget::WindowManager>	m_pwinMan;
	osg::GraphicsContext *m_pgc;
	osg::Camera *m_pcamera;
	osg::ref_ptr<osgAPEx::TableLabel> m_tableLabel; 
	osg::ref_ptr<osgAPEx::ImageManager::SubImageList> m_imagelist;
	unsigned int m_rows, m_cols;
	float m_hmargin;			//ˮƽҳ�߾�
	float m_vmargin;			//��ֱҳ�߾�
	float m_rowspace;			//�м��
	float m_colspace;			//�м��
	CString m_backGround;		//����ͼƬ����
	std::map<ULONG64, CString> m_content;	//��������
	//�����������
	osg::Vec4	m_triaColor;
	float		m_handleWidth;
	float		m_offsetX;
	float		m_offsetY;

	//�༭ʱʹ��
	CString		m_xml;			//xml����
	osg::Vec3d	m_pos;			//λ��(����������Ϊ��γ�ߣ�����ΪXYZ)

	//��ʾʹ��
	bool		m_bInScene;		//�Ƿ����ڳ�����
};

}