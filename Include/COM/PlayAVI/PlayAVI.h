/************************************************************************
CopyRight (C),  
�ļ���PlayAVI.h PlayAVI.cpp
���ߣ���־��
�汾��1.0
���ڣ�2010-03-05
�������Զ��岥��AVI����ʾ���󣬽���Win32��ʹ��.
				osg::Drawable
					  |
				   CPlayAVI

�޸ļ�¼��
����					����					����
-------------------------------------------------------------------------

************************************************************************/

#pragma once

#include <Com\Export.h>
#include <Windows.h>
#include <vfw.h>
#include <osg\Vec3f>
#include <osg\Drawable>
#include <osg\Geometry>
#include <osg\Texture2D>


namespace COM	{

//Class CPlayAVI
	class COM_EXPORT CPlayAVI : public osg::Geometry//osg::Drawable
{
public:

	CPlayAVI();
	virtual ~CPlayAVI();

	CPlayAVI (const CPlayAVI& pg, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY) 
	{ /* TODO: Finish it */ }

	virtual osg::Object* cloneType() const { return new CPlayAVI(); }

	virtual osg::Object* clone(const osg::CopyOp& copyop) const { return new CPlayAVI (*this, copyop); }

	//�������Ⱦ
	virtual void drawImplementation (osg::RenderInfo& renderInfo) const;

	//�����ı���
	void SetQuads(const osg::Vec3f &lt, const osg::Vec3f &lb, const osg::Vec3f &rt, const osg::Vec3f &rb);

	//��AVI�ļ�
	bool OpenAVI(LPCTSTR szFile);

	int Width() const { return m_width; }
	int Height() const { return m_height; }

	//����ģʽ
	enum PLAY_MODE {
		NO_LOOPING,
		LOOPING
	};

	//����״̬
	enum PLAY_STATE {
		PLAYING,
		PAUSED,
		STOPED
	};

	//���ſ���
	void Reset();
	void Play(PLAY_MODE mode);
	void Pause(bool bPause);
	void Stop();

protected:
	//��ʼ��(�ڻص�ʱʹ��)
	void Init();

	//ץȡһ֡����
	void GrabAVIFrame(int frame);

	//�ر�AVI
	void CloseAVI();

	//��תRed�ֽں�Blue�ֽ�
	inline static void flipIt(void *buffer, int pixels);

	struct CUpdateCallback : public EventCallback
	{
		CUpdateCallback() {}

		CUpdateCallback(const CUpdateCallback&, const osg::CopyOp&) {}
		
		virtual void event(osg::NodeVisitor *nv, osg::Drawable *drawable);
	};

protected:
	osg::Vec3f			m_lt;			//���Ͻ�
	osg::Vec3f			m_lb;			//���½�
	osg::Vec3f			m_rt;			//���Ͻ�
	osg::Vec3f			m_rb;			//���½�
	bool				m_quadChanged;	//���θ��ı�ʶ
	bool				m_inited;		//�ѳ�ʼ����ʶ

	HDRAWDIB			m_hdd;
	HBITMAP				m_hBitmap;
	HDC					m_hdc;
	PAVISTREAM			m_pavi;			// Handle To An Open Stream
	PGETFRAME			m_pgf;			// Pointer To A GetFrame Object
	long				m_lastframe;	// Last Frame Of The Stream
	int					m_width;		// Video Width
	int					m_height;		// Video Height
	char				*m_data;		// Pointer To Texture Data(image pixels)
	int					m_mpf;			// Will Hold Rough Milliseconds Per Frame
	osg::Texture2D		*m_tex;			// ����
	
	PLAY_STATE			m_state;		//����״̬
	PLAY_MODE			m_mode;			//����ģʽ
	long				m_curframe;		//��ǰ����֡
	unsigned long		m_lasttime;		//��һ��ʱ��(milliseconds)
	unsigned long		m_playtime;		//�Ѳ���ʱ��(milliseconds)
};

}