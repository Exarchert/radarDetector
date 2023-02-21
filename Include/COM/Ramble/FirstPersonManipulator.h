#pragma once
#include <osgGA/MatrixManipulator>


namespace COM
{
class COM_EXPORT FirstPersonManipulator : public osgGA::MatrixManipulator
{
public:

	FirstPersonManipulator();

	virtual const char* className() const { return "FirstPerson"; }

	virtual void setByMatrix(const osg::Matrixd& matrix);
	virtual void setByInverseMatrix(const osg::Matrixd& matrix) { setByMatrix(osg::Matrixd::inverse(matrix)); }
	virtual osg::Matrixd getMatrix() const;
	virtual osg::Matrixd getInverseMatrix() const;

	virtual void setNode(osg::Node*);
	virtual const osg::Node* getNode() const;
	virtual osg::Node* getNode();

	void init();

	virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& us);

protected:

	virtual ~FirstPersonManipulator();

	bool intersect(const osg::Vec3d& start, const osg::Vec3d& end, osg::Vec3d& intersection, osg::Vec3d& normal) const;

	void computePosition(const osg::Vec3d& eye,const osg::Vec3d& lv,const osg::Vec3d& up);

	void moveAhead();
	void moveBack();
	void moveLeft();
	void moveRight();
	void rotateYaw(float delta);
	void rotatePitch(float delta);

	osg::ref_ptr<osg::Node>       _node;

	float	_moveStep;
	float	_rotateStep;
	
	double _height;
	double _buffer;


	osg::Vec3d   _eye;
	osg::Quat    _rotation;
	double       _pitch;
	double       _distance;

	float	_lastX;
	float	_lastY;
};
}