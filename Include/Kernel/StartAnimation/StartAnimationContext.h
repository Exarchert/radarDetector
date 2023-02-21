#pragma once 
namespace Kernel	{
class StartAnimationContext : public osg::Referenced 
{
public:
	StartAnimationContext();
	virtual ~StartAnimationContext();

	//���úͻ�ȡ�Ӿ�������Ⱦ����
	void setRenderWnd(HWND hwnd);
	HWND getRenderWnd() const;


	//���úͻ�ȡ�Ӿ���
	void setSceneViewer(osgViewer::Viewer *viewer);
	osgViewer::Viewer *getSceneViewer() const;

	//��ȡ���������
	osg::Group *getSceneRoot() const;

	void initCameraCfg();

	//��ʼ��ֹͣ��Ⱦ
	void startRender();
	void stopRender();
protected:
	void reset();
	void setSceneShadow();
protected:
	HWND								m_hwnd;			//�Ӿ�������Ⱦ����
	osg::ref_ptr<osgViewer::Viewer>		m_viewer;		//�Ӿ���
	osg::ref_ptr<osg::Group>			m_sceneRoot;	//����ͼ�����,���г���Ҫ���ڸý����


};

}