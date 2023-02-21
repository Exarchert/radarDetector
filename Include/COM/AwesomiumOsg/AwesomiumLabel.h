//#pragma once
//#include <osg/Texture2D>
//#include <osg/MatrixTransform>
//#include <osgDB/ReadFile>
//#include <osgGA/EventVisitor>
//#include <osgViewer/ViewerEventHandlers>
//#include <osgViewer/Viewer>
//#include <Awesomium/WebCore.h>
//#include <Awesomium/WebView.h>
//#include <Awesomium/JSObject.h>
//#include <COM/Export.h>
//
//using namespace Awesomium;
//
// namespace COM
// {
//
//class AwesomiumImage : public osg::Image
//{
//public: 
//	AwesomiumImage();
//	AwesomiumImage(const AwesomiumImage& copy,const osg::CopyOp& op = osg::CopyOp::SHALLOW_COPY);
//
//	META_Object(osg,AwesomiumImage);
//
//	void loadURL(const std::string& url,int w=0,int h=0);
//	virtual bool requiresUpdateCall()const {return true;}
//	virtual void update(osg::NodeVisitor* nv);
//	virtual bool sendPointerEvent(int , int , int );
//	virtual bool sendKeyEvent(int , bool );
//protected:
//	virtual ~AwesomiumImage();
//	void injectAwesomiumKey(WebKeyboardEvent::Type type,int key);
//private:
//	WebView* _webView;
//	 int _lastButtonMask;
//	 WebCore* _webCore;
//};
//
//class COM_EXPORT AwesomiumLabel
//{
//public:
//	AwesomiumLabel(void);
//	~AwesomiumLabel(void);
//private:
//	void init();
//};
//
//COM_EXPORT WebCore* getWebCore();
// }
//
//
//
