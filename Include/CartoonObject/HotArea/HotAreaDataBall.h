#pragma once

#include <CartoonObject\Export.h>
#include <osg\Matrixd>
#include <osg\Switch>
#include <osg\MatrixTransform>
#include <osgGA\GUIEventHandler>
#include <Com\DataBall\DataBall.h>
#include <tinyxml.h>


namespace CartoonObject{

	class CARTOONOBJECT_EXPORT HotAreaDataBall : public osgGA::GUIEventHandler
	{
	public:
		struct HotEvent
		{
			std::string name;
			std::string script;
		};
		struct HotArea
		{
			int id;
			float r;
			std::string  modelName;
			osg::Vec3d pos;
			osg::Vec3d pose;
			std::vector<HotEvent> enent;
		};

		//static HotAreaDataBall* GetInstance(void) // 获得类的实例
		//{
		//	if( !m_HotArea )
		//		m_HotArea = new HotAreaDataBall();
		//	return m_HotArea;
		//}
		//static osg::ref_ptr<HotAreaDataBall> m_HotArea;

	public:
		HotAreaDataBall(void);
		virtual ~HotAreaDataBall(void);

		//处理OSG输入
		virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);

		//初始化
		void Init(CString string);

		/*
		目的：设置提示球(或其它模型)，表示当前的位置和姿态
		参数：szHint		(in)		资源名称
		*/
		void SetHintBall(LPCTSTR szHint);

		//获取提示球的结点
		osg::Node * GetHintBall() const;

		//更新提示球位置和姿态
		void UpdateHintBall(const osg::Matrixd &mat);

		//添加提示球
		void AddHintBall();

		//移除提示球
		void RemoveHintBall();

		//显隐提示球
		void ShowHintBall(bool bShow);
		void end();
	protected:
		osg::Node* addHotAreaMark(osg::Matrixd& mat);
		void addHotMark(osg::Vec3d pos,osg::Vec3d pose,int id,std::string& name);
		void writeToDisk();
		void readHotArea();
	private:
		std::string double2string(double f);
		double getDoubleFromXML(std::string name,const TiXmlElement* node);
	public:
		COM::CDataBall	m_dataBall;						//根据用户输入计算位置

		static CString	m_CaseFolder;
	protected:
		osg::ref_ptr<osg::MatrixTransform>	m_trans;	//变换矩阵
		osg::ref_ptr<osg::Switch>			m_switch;	//显隐控制
		bool								m_bWork;						//是否启用
		osg::Vec3d							_pos;
		std::vector<HotArea>				_hotAreas;
		int									_id;
		bool								_beEditing;
	private:
		bool								_beSaveToDisk;
		CString								m_path;
	};

}