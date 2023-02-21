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
		Ϊ�������һ�������λ����̬
		key			(in)	��λ����̬��Ӧ�Ĺؼ���
		pos			(in)	���λ��
		at/up		(in)	�����̬
		*/
		void setHomePosition(unsigned int key, const osg::Vec3d& pos, const osg::Vec3& at, const osg::Vec3& up);

		/*
		Ϊ�������һ�������λ����̬
		key				(in)	��λ����̬��Ӧ�Ĺؼ���
		lon/lat/height	(in)	���λ��,��γ��
		yaw/pitch/roll	(in)	����ڱ�������ϵ����̬��
		*/
		void setHomePosition(unsigned int key, double lon, double lat, double height, float yaw, float pitch, float roll);

		/*
		��ȡһ���ѱ����λ����̬
		key		(in)	��λ����̬��Ӧ�Ĺؼ���
		pos		(out)	���λ��
		at/up	(out)	�����̬
		return: �ɹ�����true
		*/
		bool getHomePosition(unsigned int key, osg::Vec3d& pos, osg::Vec3& at, osg::Vec3& up);

		/*
		��ȡ���������λ����̬
		*/
		const CameraPosPoseMap& getHomePositions() { return _homes; };

		/*
		���������λ����̬Ϊ�����ֵ
		key		(in)	Ԥ�ȱ����λ����̬��Ӧ�Ĺؼ���
		return:	�ɹ�����true
		*/
		bool home(unsigned int key);

		/////////////////////////////////////////////////////////////////////////////////
		//Ϊ��ʹ�÷��㣬������½ӿڣ�����ʵ����CBaseRamble��
		/////////////////////////////////////////////////////////////////////////////////
		/*
		���������λ����̬
		pos			(in)	���λ��
		at/up		(in)	�����̬
		*/
		void setPosPose(const osg::Vec3d& pos, const osg::Vec3& at, const osg::Vec3& up);

		/*
		�������λ��
		*/
		void setPos(const osg::Vec3d& pos);

		/*
		���������λ����̬
		lon/lat/height	(in)	���λ��,��γ��
		yaw/pitch/roll	(in)	����ڱ�������ϵ����̬��
		*/
		void setPosPose(double lon, double lat, double height, float yaw, float pitch, float roll);

		/*
		�����������̬,����λ�ò���
		yaw/pitch/roll	(in)	����ڱ�������ϵ����̬��
		*/
		void setPose(float yaw, float pitch, float roll);

		/*
		��ȡ�����λ��
		lon/lat/height	(out)	���λ��,��γ��
		*/
		void getPos(double& lon, double& lat, double& height);

		/*
		��ȡ�����λ��
		pos				(out)	�������������ϵ�е�λ��
		*/
		void getPos(osg::Vec3d& pos);

		/*
		��ȡ�����̬
		at/up			(out)	�������������ϵ�е���̬����
		*/
		void getPose(osg::Vec3& at, osg::Vec3& up);

		/*
		��ȡ�����̬
		yaw/pitch/roll	(out)	����ڱ�������ϵ�е���̬��
		*/
		void getPose(float& yaw, float& pitch, float& roll);
		////////////////////////////////////////////////////////////////////////
	private:

		CameraPosPoseMap	_homes;

		bool				_enable;

		CKernelContext*		m_KernelContext;


	public:
		//��ͬ���Ͳ���������л�������

		enum ManiType
		{
			TRRIN_MANI = 0,		//���������
			TRACK_MANI = 1,		//�����������
			FIRST_MANI,			//��һ�˳��ӽ�
		};

		ManiType m_ManiType;

		int  GetManiType()
		{
			return m_ManiType;
		}

		//�л�������
		void SwitchManip();

		//����������
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
	//	//���������Ƿ���
	//	bool EnableRamble(bool bEnable=true);
	//
	//	/*
	//		Ϊ�������һ�������λ����̬
	//		key			(in)	��λ����̬��Ӧ�Ĺؼ���
	//		pos			(in)	���λ��
	//		at/up		(in)	�����̬
	//	*/
	//	void setHomePosition(unsigned int key, const osg::Vec3d& pos, const osg::Vec3& at, const osg::Vec3& up);
	//
	//	/*
	//		Ϊ�������һ�������λ����̬
	//		key				(in)	��λ����̬��Ӧ�Ĺؼ���
	//		lon/lat/height	(in)	���λ��,��γ��
	//		yaw/pitch/roll	(in)	����ڱ�������ϵ����̬��
	//	*/
	//	void setHomePosition(unsigned int key, double lon, double lat, double height, float yaw, float pitch, float roll);
	//
	//	/*
	//		��ȡһ���ѱ����λ����̬
	//		key		(in)	��λ����̬��Ӧ�Ĺؼ���
	//		pos		(out)	���λ��
	//		at/up	(out)	�����̬
	//		return: �ɹ�����true
	//	*/
	//	bool getHomePosition(unsigned int key, osg::Vec3d& pos, osg::Vec3& at, osg::Vec3& up);
	//
	//	/*
	//		��ȡ���������λ����̬
	//	*/
	//	const CameraPosPoseMap& getHomePositions() { return _homes; };
	//
	//	/*
	//		���������λ����̬Ϊ�����ֵ
	//		key		(in)	Ԥ�ȱ����λ����̬��Ӧ�Ĺؼ���
	//		return:	�ɹ�����true
	//	*/
	//	bool home(unsigned int key);
	//
	//	/////////////////////////////////////////////////////////////////////////////////
	//	//Ϊ��ʹ�÷��㣬������½ӿڣ�����ʵ����CBaseRamble��
	//	/////////////////////////////////////////////////////////////////////////////////
	//	/*
	//		���������λ����̬
	//		pos			(in)	���λ��
	//		at/up		(in)	�����̬
	//	*/
	//	void setPosPose(const osg::Vec3d& pos, const osg::Vec3& at, const osg::Vec3& up);
	//
	//	/*
	//		�������λ��
	//	*/
	//	void setPos(const osg::Vec3d& pos);
	//
	//	/*
	//		���������λ����̬
	//		lon/lat/height	(in)	���λ��,��γ��
	//		yaw/pitch/roll	(in)	����ڱ�������ϵ����̬��
	//	*/
	//	void setPosPose(double lon, double lat, double height, float yaw, float pitch, float roll);
	//
	//	/*
	//		�����������̬,����λ�ò���
	//		yaw/pitch/roll	(in)	����ڱ�������ϵ����̬��
	//	*/
	//	void setPose(float yaw, float pitch, float roll);
	//
	//	/*
	//		��ȡ�����λ��
	//		lon/lat/height	(out)	���λ��,��γ��
	//	*/
	//	void getPos(double& lon, double& lat, double& height);
	//
	//	/*
	//		��ȡ�����λ��
	//		pos				(out)	�������������ϵ�е�λ��
	//	*/
	//	void getPos(osg::Vec3d& pos);
	//
	//	/*
	//		��ȡ�����̬
	//		at/up			(out)	�������������ϵ�е���̬����
	//	*/
	//	void getPose(osg::Vec3& at, osg::Vec3& up);
	//
	//	/*
	//		��ȡ�����̬
	//		yaw/pitch/roll	(out)	����ڱ�������ϵ�е���̬��
	//	*/
	//	void getPose(float& yaw, float& pitch, float& roll);
	//	////////////////////////////////////////////////////////////////////////
	//private:
	//
	//	CameraPosPoseMap	_homes;
	//};

}
