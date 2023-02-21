#pragma once 
namespace Kernel	{
class StartAnimationContext : public osg::Referenced 
{
public:
	StartAnimationContext();
	virtual ~StartAnimationContext();

	//设置和获取视景器的渲染窗口
	void setRenderWnd(HWND hwnd);
	HWND getRenderWnd() const;


	//设置和获取视景器
	void setSceneViewer(osgViewer::Viewer *viewer);
	osgViewer::Viewer *getSceneViewer() const;

	//获取场景根结点
	osg::Group *getSceneRoot() const;

	void initCameraCfg();

	//开始和停止渲染
	void startRender();
	void stopRender();
protected:
	void reset();
	void setSceneShadow();
protected:
	HWND								m_hwnd;			//视景器的渲染窗口
	osg::ref_ptr<osgViewer::Viewer>		m_viewer;		//视景器
	osg::ref_ptr<osg::Group>			m_sceneRoot;	//场景图根结点,所有场景要加在该结点上


};

}