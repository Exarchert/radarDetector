/*
 *	核心类
 *	作为渲染引擎与主视图的接口
 *	
 *	write by StackSnow
 */

#include <string>
#include <osg/Node>
 

namespace osg
{
	class Group;
}
namespace osgViewer
{
	class Viewer;
}
namespace osgGA
{
	class CameraManipulator;
	class GUIEventHandler;
	class KeySwitchMatrixManipulator;
}

// 此类是从 ssCore.dll 导出的
class ssCore 
{
public:
	// TODO: 在此添加您的方法。
	ssCore(HWND hWnd/*,osg::Node* node */);
	~ssCore();


public:
	// 关键接口
	bool init();
	bool update();
	bool updateFrm();
	bool size(int cx, int cy);
	bool close();

public:
	// 功能
	int getRunState(){ return m_RunState; }
	osg::Group* getRoot(){return m_Root;}
	osgViewer::Viewer* getViewer(){return m_Viewer;}

public:
	// 回调

protected:
	void initSG();			// 初始化场景
	void initCameraCfg();	// 初始化相机设备上下文、初始化Viewer
	bool renderFrame();		// 渲染一帧
private:
	// 私有量
	osgViewer::Viewer* m_Viewer;
	osg::Group* m_Root;				// 场景根节点
	int m_RunState;
	//osg::Node* m_node;

	HWND m_hWnd;
};
