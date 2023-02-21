#ifndef PIPLELINEFLOW
#define PIPLELINEFLOW 1

#include <map>
#include <istream>
#include <float.h>

#include <osg/AnimationPath>
#include <osg/Matrixf>
#include <osg/Matrixd>
#include <osg/Quat>
#include <osg/NodeCallback>

namespace Kernel {

class  PipleAnimationPathCallback : public osg::NodeCallback
{
    public:

        PipleAnimationPathCallback():
            _pivotPoint(0.0,0.0,0.0),
            _useInverseMatrix(false),
            _timeOffset(0.0),
            _timeMultiplier(1.0),
            _firstTime(DBL_MAX),
            _latestTime(0.0),
            _pause(false),
            _pauseTime(0.0) {}

        PipleAnimationPathCallback(const PipleAnimationPathCallback& apc,const osg::CopyOp& copyop):
            NodeCallback(apc,copyop),
            _animationPath(apc._animationPath),
            _pivotPoint(apc._pivotPoint),
            _useInverseMatrix(apc._useInverseMatrix),
            _timeOffset(apc._timeOffset),
            _timeMultiplier(apc._timeMultiplier),
            _firstTime(apc._firstTime),
            _latestTime(apc._latestTime),
            _pause(apc._pause),
            _pauseTime(apc._pauseTime) {}

        
        //META_Object(osg,AnimationPathCallback);

        /** Construct an AnimationPathCallback with a specified animation path.*/
        PipleAnimationPathCallback(osg::AnimationPath* ap,double timeOffset=0.0,double timeMultiplier=1.0):
            _animationPath(ap),
            _pivotPoint(0.0,0.0,0.0),
            _useInverseMatrix(false),
            _timeOffset(timeOffset),
            _timeMultiplier(timeMultiplier),
            _firstTime(DBL_MAX),
            _latestTime(0.0),
            _pause(false),
            _pauseTime(0.0) {}

        /** Construct an AnimationPathCallback and automatical create an animation path to produce a rotation about a point.*/
        PipleAnimationPathCallback(const osg::Vec3d& pivot,const osg::Vec3d& axis,float angularVelocity);
 
            
        void setAnimationPath(osg::AnimationPath* path) { _animationPath = path; }
        osg::AnimationPath* getAnimationPath() { return _animationPath.get(); }
        const osg::AnimationPath* getAnimationPath() const { return _animationPath.get(); }

        inline void setPivotPoint(const osg::Vec3d& pivot) { _pivotPoint = pivot; }
        inline const osg::Vec3d& getPivotPoint() const { return _pivotPoint; }

        void setUseInverseMatrix(bool useInverseMatrix) { _useInverseMatrix = useInverseMatrix; }
        bool getUseInverseMatrix() const { return _useInverseMatrix; }

        void setTimeOffset(double offset) { _timeOffset = offset; }
        double getTimeOffset() const { return _timeOffset; }
        
        void setTimeMultiplier(double multiplier) { _timeMultiplier = multiplier; }
        double getTimeMultiplier() const { return _timeMultiplier; }


        virtual void reset();

        void setPause(bool pause);
        bool getPause() const { return _pause; }

        /** Get the animation time that is used to specify the position along
          * the AnimationPath. Animation time is computed from the formula:
          *   ((_latestTime-_firstTime)-_timeOffset)*_timeMultiplier.*/
        virtual double getAnimationTime() const;

        /** Implements the callback. */
        virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);
        
        void update(osg::Node& node);

    public:

		osg::ref_ptr<osg::AnimationPath>  _animationPath;
        osg::Vec3d              _pivotPoint;
        bool                    _useInverseMatrix;
        double                  _timeOffset;
        double                  _timeMultiplier;
        double                  _firstTime;
        double                  _latestTime;
        bool                    _pause;
        double                  _pauseTime;

    protected:
    
        ~PipleAnimationPathCallback(){}

};

}

#endif
