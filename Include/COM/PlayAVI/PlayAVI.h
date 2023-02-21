/************************************************************************
CopyRight (C),  
文件：PlayAVI.h PlayAVI.cpp
作者：黄志鹏
版本：1.0
日期：2010-03-05
描述：自定义播放AVI的显示对象，仅在Win32下使用.
				osg::Drawable
					  |
				   CPlayAVI

修改记录：
作者					日期					描述
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

	//具体的渲染
	virtual void drawImplementation (osg::RenderInfo& renderInfo) const;

	//设置四边形
	void SetQuads(const osg::Vec3f &lt, const osg::Vec3f &lb, const osg::Vec3f &rt, const osg::Vec3f &rb);

	//打开AVI文件
	bool OpenAVI(LPCTSTR szFile);

	int Width() const { return m_width; }
	int Height() const { return m_height; }

	//播放模式
	enum PLAY_MODE {
		NO_LOOPING,
		LOOPING
	};

	//播放状态
	enum PLAY_STATE {
		PLAYING,
		PAUSED,
		STOPED
	};

	//播放控制
	void Reset();
	void Play(PLAY_MODE mode);
	void Pause(bool bPause);
	void Stop();

protected:
	//初始化(在回调时使用)
	void Init();

	//抓取一帧画面
	void GrabAVIFrame(int frame);

	//关闭AVI
	void CloseAVI();

	//翻转Red字节和Blue字节
	inline static void flipIt(void *buffer, int pixels);

	struct CUpdateCallback : public EventCallback
	{
		CUpdateCallback() {}

		CUpdateCallback(const CUpdateCallback&, const osg::CopyOp&) {}
		
		virtual void event(osg::NodeVisitor *nv, osg::Drawable *drawable);
	};

protected:
	osg::Vec3f			m_lt;			//左上角
	osg::Vec3f			m_lb;			//左下角
	osg::Vec3f			m_rt;			//右上角
	osg::Vec3f			m_rb;			//右下角
	bool				m_quadChanged;	//矩形更改标识
	bool				m_inited;		//已初始化标识

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
	osg::Texture2D		*m_tex;			// 纹理
	
	PLAY_STATE			m_state;		//播放状态
	PLAY_MODE			m_mode;			//播放模式
	long				m_curframe;		//当前播放帧
	unsigned long		m_lasttime;		//第一次时刻(milliseconds)
	unsigned long		m_playtime;		//已播放时间(milliseconds)
};

}