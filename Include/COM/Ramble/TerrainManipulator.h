#pragma once
#include <osgGA/MatrixManipulator>
#include <osg/Quat>
#include <osgUtil/LineSegmentIntersector>
#include <osgUtil/IntersectionVisitor>
#include <osgViewer/Viewer>


namespace COM
{

class COM_EXPORT TerrainManipulator : public osgGA::MatrixManipulator
{
public:

	TerrainManipulator(/*osgViewer::Viewer* viewer*/);

	virtual const char* className() const { return "Terrain"; }


	enum RotationMode 
	{
		ELEVATION_AZIM_ROLL,
		ELEVATION_AZIM
	};

	void setRotationMode(RotationMode mode);
	RotationMode getRotationMode() const { return _rotationMode; }

	virtual void setByMatrix(const osg::Matrixd& matrix);
	virtual void setByInverseMatrix(const osg::Matrixd& matrix) { setByMatrix(osg::Matrixd::inverse(matrix)); }
	virtual osg::Matrixd getMatrix() const;
	virtual osg::Matrixd getInverseMatrix() const;

	virtual osgUtil::SceneView::FusionDistanceMode getFusionDistanceMode() const { return osgUtil::SceneView::USE_FUSION_DISTANCE_VALUE; }
	virtual float getFusionDistanceValue() const { return _distance; }

	virtual void setNode(osg::Node*);
	virtual const osg::Node* getNode() const;
	virtual osg::Node* getNode();

	virtual void home(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& us);

	virtual void init(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& us);

	virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& us);

	virtual void getUsage(osg::ApplicationUsage& usage) const;

protected:

	virtual ~TerrainManipulator();

	bool intersect(const osg::Vec3d& start, const osg::Vec3d& end, osg::Vec3d& intersection, bool nearest=true) const;
	void flushMouseEventStack();
	void addMouseEvent(const osgGA::GUIEventAdapter& ea);
	void computePosition(const osg::Vec3d& eye,const osg::Vec3d& lv,const osg::Vec3d& up);
	bool calcMovement();
	void trackball(osg::Vec3& axis,double& angle, double p1x, double p1y, double p2x, double p2y);
	double tb_project_to_sphere(double r, double x, double y);
	bool isMouseMoving();
	void clampOrientation();
	bool CollisionEarth(osg::Vec3 center,double distance);
	bool examineIntoGround(double distance,osg::Quat rotation,osg::Vec3d center);

	void updateFrustum();


	osg::ref_ptr<const osgGA::GUIEventAdapter> _ga_t1;
	osg::ref_ptr<const osgGA::GUIEventAdapter> _ga_t0;

	osg::ref_ptr<osg::Node> _node;

	RotationMode            _rotationMode;

	bool                    _thrown;

	osg::Vec3d              _center;
	osg::Vec3d              _eartchCenter; 
	osg::Quat               _rotation;
	double                  _distance;
	osg::Vec3d              _previousUp;
	//osgViewer::Viewer*       _viewer;
	osg::Vec3d              _oldCenter;
	osg::Matrixd            _oldMatrix;

	osg::ref_ptr<osgUtil::LineSegmentIntersector>	_intersector;
	osg::ref_ptr<osgUtil::IntersectionVisitor>		_intersectionVisitor;
	
};

}
