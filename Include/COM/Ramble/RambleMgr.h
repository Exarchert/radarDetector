#pragma once

#include <Com\Export.h>
#include <osgGA\KeySwitchMatrixManipulator>
#include <osgGA/MatrixManipulator>
#include <osgViewer/View>

namespace COM
{

	class CKernelContext;
	class COM_EXPORT CRambleMgr : public osgGA::MatrixManipulator
	{
	public:
		CRambleMgr(CKernelContext* kc);
		~CRambleMgr(void);
		virtual const char* className() const { return "CRambleMgr"; }

		struct CameraPosPose : public osg::Referenced
		{
			osg::Vec3d	_pos;
			osg::Vec3	_at;
			osg::Vec3	_up;
		};
		typedef std::map<unsigned int, osg::ref_ptr<CameraPosPose> >	CameraPosPoseMap;

		//virtual osg::Matrixd getMatrix() const { return _current->getMatrix(); }
		//virtual osg::Matrixd getInverseMatrix() const { return _current->getInverseMatrix(); }
		//virtual void setByMatrix(const osg::Matrixd& matrix) { _current->setByMatrix(matrix); }
		//virtual void setByInverseMatrix(const osg::Matrixd& matrix) { _current->setByInverseMatrix(matrix); }
		virtual osg::Matrixd getMatrix() const;
		virtual osg::Matrixd getInverseMatrix() const;
		virtual void setByMatrix(const osg::Matrixd& matrix);
		virtual void setByInverseMatrix(const osg::Matrixd& matrix);


		virtual void setMinimumDistance(float minimumDistance);
		virtual void setCoordinateFrameCallback(CoordinateFrameCallback* cb);
		virtual osgUtil::SceneView::FusionDistanceMode getFusionDistanceMode() const { return _current->getFusionDistanceMode(); }
		virtual float getFusionDistanceValue() const { return _current->getFusionDistanceValue(); }
		virtual void setNode(osg::Node* n);
		virtual const osg::Node* getNode() const        { return _current->getNode(); }
		virtual osg::Node* getNode()                    { return _current->getNode(); }

		virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);


		osgGA::MatrixManipulator* getCurrentMatrixManipulator()
		{
			return _current.get(); 
		}
		const osgGA::MatrixManipulator* getCurrentMatrixManipulator() const 
		{ 
			return _current.get(); 
		}

		void addMatrixManipulator(std::string name, osgGA::MatrixManipulator *cm);
		void removeMatrixManipulator(std::string name);
		void selectMatrixManipulator(std::string name);
		void toggleManipulator();
		std::string getCurrentManipulatorName();

	private:
		bool pick(osgViewer::View* view, osg::Node** node, osg::Vec3d& center);

	private:
		typedef std::vector<osg::ref_ptr<osgGA::MatrixManipulator>> ManipulatorList;

		ManipulatorList _manips;
		osg::ref_ptr<osgGA::MatrixManipulator> _current;

		osg::Matrix	_inverseMat;

		//////////////////////////////////////////////////////////////////////////
	public:
		void EnableRamble(bool bEnable=true) { _enable = bEnable; };

		/*
		为相机设置一个保存的位置姿态
		key			(in)	该位置姿态对应的关键字
		pos			(in)	相机位置
		at/up		(in)	相机姿态
		*/
		void setHomePosition(unsigned int key, const osg::Vec3d& pos, const osg::Vec3& at, const osg::Vec3& up);

		/*
		为相机设置一个保存的位置姿态
		key				(in)	该位置姿态对应的关键字
		lon/lat/height	(in)	相机位置,经纬高
		yaw/pitch/roll	(in)	相机在本地坐标系的姿态角
		*/
		void setHomePosition(unsigned int key, double lon, double lat, double height, float yaw, float pitch, float roll);

		/*
		获取一个已保存的位置姿态
		key		(in)	该位置姿态对应的关键字
		pos		(out)	相机位置
		at/up	(out)	相机姿态
		return: 成功返回true
		*/
		bool getHomePosition(unsigned int key, osg::Vec3d& pos, osg::Vec3& at, osg::Vec3& up);

		/*
		获取保存的所有位置姿态
		*/
		const CameraPosPoseMap& getHomePositions() { return _homes; };

		/*
		设置相机的位置姿态为保存的值
		key		(in)	预先保存的位置姿态对应的关键字
		return:	成功返回true
		*/
		bool home(unsigned int key);

		/////////////////////////////////////////////////////////////////////////////////
		//为了使用方便，添加以下接口，具体实现在CBaseRamble中
		/////////////////////////////////////////////////////////////////////////////////
		/*
		设置相机的位置姿态
		pos			(in)	相机位置
		at/up		(in)	相机姿态
		*/
		void setPosPose(const osg::Vec3d& pos, const osg::Vec3& at, const osg::Vec3& up);

		/*
		设置相机位置
		*/
		void setPos(const osg::Vec3d& pos);

		/*
		设置相机的位置姿态
		lon/lat/height	(in)	相机位置,经纬高
		yaw/pitch/roll	(in)	相机在本地坐标系的姿态角
		*/
		void setPosPose(double lon, double lat, double height, float yaw, float pitch, float roll);

		/*
		设置相机的姿态,保持位置不变
		yaw/pitch/roll	(in)	相机在本地坐标系的姿态角
		*/
		void setPose(float yaw, float pitch, float roll);

		/*
		获取相机的位置
		lon/lat/height	(out)	相机位置,经纬高
		*/
		void getPos(double& lon, double& lat, double& height);

		/*
		获取相机的位置
		pos				(out)	相机在世界作标系中的位置
		*/
		void getPos(osg::Vec3d& pos);

		/*
		获取相机姿态
		at/up			(out)	相机在世界坐标系中的姿态向量
		*/
		void getPose(osg::Vec3& at, osg::Vec3& up);

		/*
		获取相机姿态
		yaw/pitch/roll	(out)	相机在本地坐标系中的姿态角
		*/
		void getPose(float& yaw, float& pitch, float& roll);
		////////////////////////////////////////////////////////////////////////
	private:

		CameraPosPoseMap	_homes;

		bool				_enable;

		CKernelContext*		m_KernelContext;


	public:
		//不同类型操作器相关切换与设置

		enum ManiType
		{
			TRRIN_MANI = 0,		//地球操作器
			TRACK_MANI = 1,		//跟踪球操作器
			FIRST_MANI,			//第一人称视角
		};

		ManiType m_ManiType;

		int  GetManiType()
		{
			return m_ManiType;
		}

		//切换操作器
		void SwitchManip();

		//锁定操作器
		void LockManip(bool bLock);
		void SwitchManipulator(ManiType type = TRRIN_MANI);
	private:
		bool m_bSwitchManip;
		bool m_bLockManip;
	};
	//
	//class COM_EXPORT CRambleMgr
	//	: public osgGA::KeySwitchMatrixManipulator
	//{
	//public:
	//	CRambleMgr(CKernelContext* kContext);
	//	~CRambleMgr(void);
	//
	//	struct CameraPosPose : public osg::Referenced
	//	{
	//		osg::Vec3d	_pos;
	//		osg::Vec3	_at;
	//		osg::Vec3	_up;
	//	};
	//	typedef std::map<unsigned int, osg::ref_ptr<CameraPosPose> >	CameraPosPoseMap;
	//
	//	//osg::ref_ptr< CKernelContext > m_KC;
	//
	//	virtual const char* className() const { return "CRambleMgr"; }
	//
	//	void selectMatrixManipulator(unsigned int num);
	//
	//	osg::Matrixd getMatrix() const;
	//
	//	void setByInverseMatrix(const osg::Matrixd& matrix);
	//	osg::Matrixd getInverseMatrix() const;
	//
	//	//void SetRamble(RAMBLE_TYPE type, osg::ref_ptr<osgAPEx::EarthManipulator> rpEM);
	//
	//	//设置漫游是否工作
	//	bool EnableRamble(bool bEnable=true);
	//
	//	/*
	//		为相机设置一个保存的位置姿态
	//		key			(in)	该位置姿态对应的关键字
	//		pos			(in)	相机位置
	//		at/up		(in)	相机姿态
	//	*/
	//	void setHomePosition(unsigned int key, const osg::Vec3d& pos, const osg::Vec3& at, const osg::Vec3& up);
	//
	//	/*
	//		为相机设置一个保存的位置姿态
	//		key				(in)	该位置姿态对应的关键字
	//		lon/lat/height	(in)	相机位置,经纬高
	//		yaw/pitch/roll	(in)	相机在本地坐标系的姿态角
	//	*/
	//	void setHomePosition(unsigned int key, double lon, double lat, double height, float yaw, float pitch, float roll);
	//
	//	/*
	//		获取一个已保存的位置姿态
	//		key		(in)	该位置姿态对应的关键字
	//		pos		(out)	相机位置
	//		at/up	(out)	相机姿态
	//		return: 成功返回true
	//	*/
	//	bool getHomePosition(unsigned int key, osg::Vec3d& pos, osg::Vec3& at, osg::Vec3& up);
	//
	//	/*
	//		获取保存的所有位置姿态
	//	*/
	//	const CameraPosPoseMap& getHomePositions() { return _homes; };
	//
	//	/*
	//		设置相机的位置姿态为保存的值
	//		key		(in)	预先保存的位置姿态对应的关键字
	//		return:	成功返回true
	//	*/
	//	bool home(unsigned int key);
	//
	//	/////////////////////////////////////////////////////////////////////////////////
	//	//为了使用方便，添加以下接口，具体实现在CBaseRamble中
	//	/////////////////////////////////////////////////////////////////////////////////
	//	/*
	//		设置相机的位置姿态
	//		pos			(in)	相机位置
	//		at/up		(in)	相机姿态
	//	*/
	//	void setPosPose(const osg::Vec3d& pos, const osg::Vec3& at, const osg::Vec3& up);
	//
	//	/*
	//		设置相机位置
	//	*/
	//	void setPos(const osg::Vec3d& pos);
	//
	//	/*
	//		设置相机的位置姿态
	//		lon/lat/height	(in)	相机位置,经纬高
	//		yaw/pitch/roll	(in)	相机在本地坐标系的姿态角
	//	*/
	//	void setPosPose(double lon, double lat, double height, float yaw, float pitch, float roll);
	//
	//	/*
	//		设置相机的姿态,保持位置不变
	//		yaw/pitch/roll	(in)	相机在本地坐标系的姿态角
	//	*/
	//	void setPose(float yaw, float pitch, float roll);
	//
	//	/*
	//		获取相机的位置
	//		lon/lat/height	(out)	相机位置,经纬高
	//	*/
	//	void getPos(double& lon, double& lat, double& height);
	//
	//	/*
	//		获取相机的位置
	//		pos				(out)	相机在世界作标系中的位置
	//	*/
	//	void getPos(osg::Vec3d& pos);
	//
	//	/*
	//		获取相机姿态
	//		at/up			(out)	相机在世界坐标系中的姿态向量
	//	*/
	//	void getPose(osg::Vec3& at, osg::Vec3& up);
	//
	//	/*
	//		获取相机姿态
	//		yaw/pitch/roll	(out)	相机在本地坐标系中的姿态角
	//	*/
	//	void getPose(float& yaw, float& pitch, float& roll);
	//	////////////////////////////////////////////////////////////////////////
	//private:
	//
	//	CameraPosPoseMap	_homes;
	//};

}
