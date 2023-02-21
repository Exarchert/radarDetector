#pragma once
#include <osgGA/MatrixManipulator>
#include <osg/Quat>
#include <osgViewer/View>


namespace COM
{

class COM_EXPORT TrackballManipulator : public osgGA::MatrixManipulator
{
public:
	TrackballManipulator();

	virtual const char* className() const { return "Trackball"; }
	virtual void setByMatrix(const osg::Matrixd& matrix);
	virtual void setByInverseMatrix(const osg::Matrixd& matrix) { setByMatrix(osg::Matrixd::inverse(matrix)); }
	virtual osg::Matrixd getMatrix() const;
	virtual osg::Matrixd getInverseMatrix() const;

	virtual osgUtil::SceneView::FusionDistanceMode getFusionDistanceMode() const { return osgUtil::SceneView::USE_FUSION_DISTANCE_VALUE; }
	virtual float getFusionDistanceValue() const { return _distance; }

	virtual void setNode(osg::Node*);
	virtual const osg::Node* getNode() const;
	virtual osg::Node* getNode();

	void setTrackCenter(const osg::Vec3d& center);

	enum RotationMode 
	{
		ELEVATION_AZIM_ROLL,
		ELEVATION_AZIM
	};

	void setRotationMode(RotationMode mode) { _rotationMode = mode; };
	RotationMode getRotationMode() const { return _rotationMode; }

	virtual void home(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& us);
	virtual void home(double);

	virtual void init(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& us);

	virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& us);

	virtual void getUsage(osg::ApplicationUsage& usage) const;

	void setMinimumZoomScale(double minimumZoomScale) { _minimumZoomScale=minimumZoomScale; }
	double getMinimumZoomScale() const { return _minimumZoomScale; }

	void setCenter(const osg::Vec3d& center) { _center = center; }
	const osg::Vec3d& getCenter() const { return _center; }

	void setRotation(const osg::Quat& rotation) { _rotation = rotation; }
	const osg::Quat& getRotation() const { return _rotation; }

	void setDistance(double distance) { _distance = distance; }
	double getDistance() const { return _distance; }

	void setTrackballSize(float size);
	float getTrackballSize() const { return _trackballSize; }

protected:
	virtual ~TrackballManipulator();

	void flushMouseEventStack();
	void addMouseEvent(const osgGA::GUIEventAdapter& ea);
	void computePosition(const osg::Vec3& eye,const osg::Vec3& lv,const osg::Vec3& up);
	bool calcMovement();
	void trackball(osg::Vec3& axis,float& angle, float p1x, float p1y, float p2x, float p2y);
	float tb_project_to_sphere(float r, float x, float y);
	bool isMouseMoving();

	bool pick(osgViewer::View* view, const osgGA::GUIEventAdapter& ea, osg::Node** node, osg::Vec3d& center);

	osg::ref_ptr<const osgGA::GUIEventAdapter> _ga_t1;
	osg::ref_ptr<const osgGA::GUIEventAdapter> _ga_t0;


	osg::ref_ptr<osg::Node>       _node;

	RotationMode            _rotationMode;

	double _modelScale;
	double _minimumZoomScale;

	bool _thrown;

	osg::Vec3d   _center;
	osg::Quat    _rotation;
	double       _distance;
	float        _trackballSize;

	bool _isCenter;

};

}