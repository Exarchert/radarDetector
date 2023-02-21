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

		//static HotAreaDataBall* GetInstance(void) // ������ʵ��
		//{
		//	if( !m_HotArea )
		//		m_HotArea = new HotAreaDataBall();
		//	return m_HotArea;
		//}
		//static osg::ref_ptr<HotAreaDataBall> m_HotArea;

	public:
		HotAreaDataBall(void);
		virtual ~HotAreaDataBall(void);

		//����OSG����
		virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);

		//��ʼ��
		void Init(CString string);

		/*
		Ŀ�ģ�������ʾ��(������ģ��)����ʾ��ǰ��λ�ú���̬
		������szHint		(in)		��Դ����
		*/
		void SetHintBall(LPCTSTR szHint);

		//��ȡ��ʾ��Ľ��
		osg::Node * GetHintBall() const;

		//������ʾ��λ�ú���̬
		void UpdateHintBall(const osg::Matrixd &mat);

		//�����ʾ��
		void AddHintBall();

		//�Ƴ���ʾ��
		void RemoveHintBall();

		//������ʾ��
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
		COM::CDataBall	m_dataBall;						//�����û��������λ��

		static CString	m_CaseFolder;
	protected:
		osg::ref_ptr<osg::MatrixTransform>	m_trans;	//�任����
		osg::ref_ptr<osg::Switch>			m_switch;	//��������
		bool								m_bWork;						//�Ƿ�����
		osg::Vec3d							_pos;
		std::vector<HotArea>				_hotAreas;
		int									_id;
		bool								_beEditing;
	private:
		bool								_beSaveToDisk;
		CString								m_path;
	};

}