/*
 *	������
 *	��Ϊ��Ⱦ����������ͼ�Ľӿ�
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

// �����Ǵ� ssCore.dll ������
class ssCore 
{
public:
	// TODO: �ڴ�������ķ�����
	ssCore(HWND hWnd/*,osg::Node* node */);
	~ssCore();


public:
	// �ؼ��ӿ�
	bool init();
	bool update();
	bool updateFrm();
	bool size(int cx, int cy);
	bool close();

public:
	// ����
	int getRunState(){ return m_RunState; }
	osg::Group* getRoot(){return m_Root;}
	osgViewer::Viewer* getViewer(){return m_Viewer;}

public:
	// �ص�

protected:
	void initSG();			// ��ʼ������
	void initCameraCfg();	// ��ʼ������豸�����ġ���ʼ��Viewer
	bool renderFrame();		// ��Ⱦһ֡
private:
	// ˽����
	osgViewer::Viewer* m_Viewer;
	osg::Group* m_Root;				// �������ڵ�
	int m_RunState;
	//osg::Node* m_node;

	HWND m_hWnd;
};
