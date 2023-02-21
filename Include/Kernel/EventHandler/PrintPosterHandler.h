#pragma once
#include <osgGA/GUIEventHandler>
#include <osg/Image>
#include <COM/Command/Cmd.h>

namespace Kernel
{
class PrintPosterHandler;
class PrintPosterHandlerCmd :public COM::CCmdObj
{
public:
	PrintPosterHandlerCmd(PrintPosterHandler* printPosterHander,bool bOK);
	void DoAction();

private:
	PrintPosterHandler* _printPosterHander;
	bool _bOK;
};

class PrintPosterHandler :public osgGA::GUIEventHandler
{
public:
	typedef std::pair<unsigned int,unsigned int>			 TilePosition;
	typedef std::map<TilePosition,osg::ref_ptr<osg::Image> > TileTmages;
	
	PrintPosterHandler(void);
	~PrintPosterHandler(void);

	bool handle(const osgGA::GUIEventAdapter&,osgGA::GUIActionAdapter&);
	void setCameraRoot(osg::ref_ptr<osg::Group> cameraRoot);
	inline void setTileSize(int w,int h){_tileSize.set(w,h);}
	
	void setPosterSize(int w,int h);

	inline void setImagePathName(const std::string imagePathName){_imagePathName = imagePathName;}
private:
	void recordImages();
	void bindCameraToImage(osg::Camera* camera,int row,int col);

	osg::Camera* getAvailableCamera();
private:
	TileTmages _images;
	osg::ref_ptr<osg::Image> _findPoster;
	osg::Vec2 _tileSize;
	osg::Vec2 _posterSize;

	int _tileRows;
	int _tileColumns;

	int _currentRow;
	int _currentColumn;

	int _cameraIndex;

	osg::Matrixd _currentViewMatrx;
	osg::Matrixd _currentProjectMatrix;

	std::string _imagePathName;

	osg::ref_ptr<osg::Group> _cameraRoot;

	bool _isRunning;
	bool _isFinished;
};
}