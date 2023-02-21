/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2006 Robert Osfield 
 *
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * OpenSceneGraph Public License for more details.
*/

#ifndef COM_NODETRACKERMANIPULATOR
#define COM_NODETRACKERMANIPULATOR 1

#include <COM/Export.h>
#include <osgGA/MatrixManipulator>

#include <osg/Quat>
#include <osg/observer_ptr>
#include <COM/Ramble/ObserverNodePath.h>

namespace COM{

class COM_EXPORT NodeTrackerManipulator : public osgGA::MatrixManipulator
{
    public:

        NodeTrackerManipulator(); 

        virtual const char* className() const { return "NodeTrackerManipulator"; }

       // typedef std::vector< osg::observer_ptr<osg::Node> >   ObserverNodePath;

        void setTrackNodePath(const osg::NodePath& nodePath)
        {
			_trackNodePath.setNodePath(nodePath);
           /*_trackNodePath.clear();
           _trackNodePath.reserve(nodePath.size());
           std::copy(nodePath.begin(), nodePath.end(), std::back_inserter(_trackNodePath));*/
        }

		void setTrackNodePath(const COM::ObserverNodePath& nodePath) { _trackNodePath = nodePath; }
		COM::ObserverNodePath& getTrackNodePath() { return _trackNodePath; }

        void setTrackNode(osg::Node* node);
		osg::Node* getTrackNode() 
		{
			osg::NodePath nodePath;
			return _trackNodePath.getNodePath(nodePath)&&!nodePath.empty()?nodePath.back():0;
		}//{ return _trackNodePath.empty() ? 0 : _trackNodePath.back().get(); }
        const osg::Node* getTrackNode() const 
		{
			osg::NodePath nodePath;
			return _trackNodePath.getNodePath(nodePath)&&!nodePath.empty()?nodePath.back():0;
		}
			//{ return _trackNodePath.empty() ? 0 : _trackNodePath.back().get(); }

        enum TrackerMode 
        {
            /** Track the center of the node's bounding sphere, but not rotations of the node.  
              * For databases which have a CoordinateSystemNode, the orientation is kept relative the coordinate frame if the center of the node.
              */
            NODE_CENTER,
            /** Track the center of the node's bounding sphere, and the azimuth rotation (about the z axis of the current coordinate frame).
              * For databases which have a CoordinateSystemNode, the orientation is kept relative the coordinate frame if the center of the node.
              */
            NODE_CENTER_AND_AZIM,
            /** Tack the center of the node's bounding sphere, and the all rotations of the node.
              */
            NODE_CENTER_AND_ROTATION
        };
        
        void setTrackerMode(TrackerMode mode);
        TrackerMode getTrackerMode() const { return _trackerMode; }


        enum RotationMode 
        {
            /** Use a trackball style manipulation of the view direction w.r.t the tracked orientation.
              */
            TRACKBALL,
            /** Allow the elevation and azimuth angles to be adjust w.r.t the tracked orientation.
              */
            ELEVATION_AZIM
        };
        
        void setRotationMode(RotationMode mode);
        RotationMode getRotationMode() const { return _rotationMode; }


        /** set the position of the matrix manipulator using a 4x4 Matrix.*/
        virtual void setByMatrix(const osg::Matrixd& matrix);

        /** set the position of the matrix manipulator using a 4x4 Matrix.*/
        virtual void setByInverseMatrix(const osg::Matrixd& matrix) { setByMatrix(osg::Matrixd::inverse(matrix)); }

        /** get the position of the manipulator as 4x4 Matrix.*/
        virtual osg::Matrixd getMatrix() const;

        /** get the position of the manipulator as a inverse matrix of the manipulator, typically used as a model view matrix.*/
        virtual osg::Matrixd getInverseMatrix() const;

        /** Get the FusionDistanceMode. Used by SceneView for setting up stereo convergence.*/
        virtual osgUtil::SceneView::FusionDistanceMode getFusionDistanceMode() const { return osgUtil::SceneView::USE_FUSION_DISTANCE_VALUE; }

        /** Get the FusionDistanceValue. Used by SceneView for setting up stereo convergence.*/
        virtual float getFusionDistanceValue() const { return _distance; }

        /** Attach a node to the manipulator. 
            Automatically detaches previously attached node.
            setNode(NULL) detaches previously nodes.
            Is ignored by manipulators which do not require a reference model.*/
        virtual void setNode(osg::Node*);

        /** Return node if attached.*/
        virtual const osg::Node* getNode() const;

        /** Return node if attached.*/
        virtual osg::Node* getNode();

        virtual void computeHomePosition();

        /** Move the camera to the default position. 
            May be ignored by manipulators if home functionality is not appropriate.*/
		virtual void home(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& us);
        
        /** Start/restart the manipulator.*/
        virtual void init(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& us);

        /** handle events, return true if handled, false otherwise.*/
        virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& us);

        /** Get the keyboard and mouse usage of this manipulator.*/
        virtual void getUsage(osg::ApplicationUsage& usage) const;

    protected:

        virtual ~NodeTrackerManipulator();
        
        osg::NodePath getNodePath() const;

        bool validateNodePath() const;

        /** Reset the internal GUIEvent stack.*/
        void flushMouseEventStack();
        /** Add the current mouse GUIEvent to internal stack.*/
		void addMouseEvent(const osgGA::GUIEventAdapter& ea);

        void computeNodeWorldToLocal(osg::Matrixd& worldToLocal) const;
        void computeNodeLocalToWorld(osg::Matrixd& localToWorld) const;

        void computeNodeCenterAndRotation(osg::Vec3d& center, osg::Quat& rotation) const;

        void computePosition(const osg::Vec3d& eye,const osg::Vec3d& lv,const osg::Vec3d& up);

        /** For the give mouse movement calculate the movement of the camera.
            Return true is camera has moved and a redraw is required.*/
        bool calcMovement();
        
        void trackball(osg::Vec3& axis,double& angle, double p1x, double p1y, double p2x, double p2y);
        double tb_project_to_sphere(double r, double x, double y);


        /** Check the speed at which the mouse is moving.
            If speed is below a threshold then return false, otherwise return true.*/
        bool isMouseMoving();


        void clampOrientation();

		


        // Internal event stack comprising last two mouse events.
        osg::ref_ptr<const  osgGA::GUIEventAdapter> _ga_t1;
        osg::ref_ptr<const  osgGA::GUIEventAdapter> _ga_t0;

        osg::ref_ptr<osg::Node> _node;
        
		COM::ObserverNodePath      _trackNodePath;

        TrackerMode             _trackerMode;
        RotationMode            _rotationMode;

        bool                    _thrown;
        
        osg::Quat               _nodeRotation;
        osg::Quat               _rotation;
        float                   _distance;
	
		mutable OpenThreads::Mutex _mutex;

};

}

#endif

