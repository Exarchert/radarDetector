/***************************************************************************
Copyright (C),  
�ļ���SOPipe.h
���ߣ���ƽ
�汾��1.0
���ڣ�2009-03-20
�����������������

�޸ļ�¼��
����						����					����
л��ҫ                    2009-12-22            ����ɽ��Ŀ��ֲ
***************************************************************************/
#pragma once
#include <Object/Export.h>
#include <Object/Obj.h>

#include <osgAPEx/LongPipe>
#include <osgAPEx/LandGroove>

namespace EMObj{

class OBJECT_EXPORT CSOLongPipe : public EMObj::CObj 
{
public:
	CSOLongPipe();
	~CSOLongPipe();

	enum
	{
		SOType = 3
	};

	DWORD GetType()
	{
		return SOType;
	}

	DWORD GetRenderType()
	{
		return Render_Type_Scene;
	}

	enum
	{
		ES,
		EE,
	};

	bool ReadProperty(MSXML2::IXMLDOMNodePtr pNode);
	MSXML2::IXMLDOMNodePtr GetProperty();

	void UpdateDraw(DWORD Value = 0);
	bool BeginEditUI();
	bool EndEdit3D();

public:
	//ʹ�õ�������滻���ߣ����ڱ༭���ߵ�
	void ReplacePipeWithPointLine();

	//��ӹܵ�����ʼS�ͽ�βE��־
	bool AddSEFlag();

	//�Ƴ�S��E��־
	bool RemoveSEFlag();

	/*
	Ŀ�ģ����¹ؼ����λ��
	����: index	(in)	���µ������
	      pos	(in)	�µ�λ��	
	*/
	void UpdatePointPos(unsigned int index,osg::Vec3 pos);
	
	/*
	Ŀ�ģ�ɾ��ָ�������Ŀ��Ƶ�
	����: index	(in)	ɾ���������
	���أ�ɾ���ɹ�����true�����򷵻�false
	*/
	bool DeletePoint(unsigned int index);

	/*
	Ŀ�ģ�ɾ��ָ��������������
	����: index	(in)	ɾ���ߵ�����
	���أ�ɾ���ɹ�����true�����򷵻�false
	*/
	bool DeleteLine(unsigned int index);

	/*
	Ŀ�ģ����¹ܵ���λ��
	����: moveVec	(in)	���µ�λ������
	*/
	void UpdatePipePos(osg::Vec3& moveVec);

	//�ڹ��ߵĿ�ʼ����Ӵ������
    void AddNewBreakPlane();

	/*
	Ŀ�ģ����´�������λ��
	����: preIndex	(in)	��ǰ�����λ�õ�ǰһ�����Ƶ�����
	      postIndex	(in)	��ǰ�����λ�õĺ�һ�����Ƶ�����
		  pos   	(in)	��ǰ������λ��
	���أ��ɹ�����true�����򷵻�false
	*/
	bool UpdateBreakPlane(int preIndex,int postIndex,osg::Vec3d pos);

	//�����¼�����������Ĵ�ϵ㣬��Ϲ���
	bool BreakPipe();

	/*
	Ŀ�ģ����ӹ���
	����: pPipe	        (in)	�����ӵĹ���
	      connectType	(in)	��������
	���أ��ɹ�����true�����򷵻�false
	*/
    bool ConnectPipe(CSOLongPipe* pPipe,int connectType);
	
	//ɾ�������߼�����
	void RemovePointsLines();

	//��ָ�����������һ�����ĵ�
	void AddNewPoint(int index);

	//��дʰȡ
	virtual bool PtSelect(double &x, double &y, osg::Vec3d& eye, osg::Vec3d& vec);

	//��ȡ���ߵĵ����飬����Ϊ��������ϵ�µ����꣬����ֵΪ���������
	DWORD GetPipePoints(osg::Vec3dArray &Points);
public:

	osg::ref_ptr<osgAPEx::LongPipe> createLongPipe(osg::Group* root, osgGA::MatrixManipulator* manip,const osg::Vec4& color,
		                                           osg::ref_ptr<osg::Vec3dArray> points,float r,std::string srtName);
private:

	class UpdateLongPipe : public osg::NodeCallback 
	{
	public:
		UpdateLongPipe() : _lastUpdate(0) {}
		virtual void operator()(osg::Node* node, osg::NodeVisitor* nv) 
		{
			CSOLongPipe* pipe = dynamic_cast<CSOLongPipe*>(node);
			if (!pipe)
				return;
			if (nv->getVisitorType() == osg::NodeVisitor::UPDATE_VISITOR && 
				nv->getFrameStamp() && 
				nv->getFrameStamp()->getFrameNumber() != _lastUpdate) 
			{
				_lastUpdate = nv->getFrameStamp()->getFrameNumber();
				pipe->update();
			}
		}
		int _lastUpdate;
	};


public:
	void update();

private:
    //�ӽڵ�����ȡ��������
	bool ParseXml(MSXML2::IXMLDOMNodePtr pNode);
	//�ӽڵ�����ȡ������������
    bool ParsePipeAttributeXml(MSXML2::IXMLDOMNodePtr pNode);
	//�ӽڵ�����ȡ����������
    bool ParseLineAttributeXml(MSXML2::IXMLDOMNodePtr pNode);
	//�ӽڵ��ж�ȡ��ɫ��Ϣ
    void ReadXmlNodeColor(MSXML2::IXMLDOMElementPtr node,osg::Vec4& color);

	//�����㼸����
	osg::Geode*  CreatePointGeode(osg::Vec3& center,float radius) ;
	//�����߼�����
	osg::Geode*  CreateLineGeode(osg::Vec3& startPoint,osg::Vec3& endPoint);
	//�������漸����
	osg::Geode*  CreateBreakGeode(osg::Vec3d prePoint,osg::Vec3d postPoint,osg::Vec3d pos,float radius);
    //�Ƴ����漸����
    bool RemoveBreakPlane();

public:
	float GetTotalLength();

public: 
	float                                 m_radius;   //���Ӱ뾶
	float                                 m_segLength;//�ֶγ���
	float                                 m_pipeVisiableDis;//�ܵ��߶ȷ�ֵ
	osg::Vec4                             m_pipeColor;//�ܵ���ɫ

	osg::Vec4                             m_pipeAmbient;//�ܵ�����
	osg::Vec4                             m_pipeDiffuse;
	osg::Vec4                             m_pipeSpecular;
	float                                 m_pipeShininess;
	CString                               m_imageFilePath;//��ͼ·��

	float                                 m_lineVisiableDis;//�߸߶ȷ�ֵ
	float                                 m_initLineWidth;//�߳�ʼ���
	osg::Vec4                             m_lodLineColor;//����ɫ
	std::map<float,float>                 m_disLineWidthMap;//����������߿����ֵ��
	std::map<float,float>                 m_disLineOffsetMap;//�����������ƫ��������ֵ��


	osg::Vec3d pos1,pos2;

	osg::ref_ptr<osgAPEx::LongPipe>       m_longPipe;

	osg::ref_ptr<osg::Vec3dArray>         m_pipePoints;//������
	osg::ref_ptr<osg::MatrixTransform>    m_longPipeMT;
	osg::ref_ptr<osg::MatrixTransform>    m_moveMT;

	osg::ref_ptr<osg::Vec2dArray>         m_left, m_right;
	osg::ref_ptr<osg::Node>               m_Dome;

public:
	osg::Vec4                               m_lineColor;        //������ɫ
	osg::Vec4                               m_pointColor;       //���Ƶ���ɫ
	std::vector<osg::ref_ptr<osg::Geode> >  m_PointsNodeList;//N��Բ�򼸺���
	std::vector<osg::ref_ptr<osg::Geode> >  m_LinesNodeList;//N-1���߶μ�����
	osg::ref_ptr<osg::Geode>                m_axisNode;
	osg::ref_ptr<osg::MatrixTransform>      m_axisTransform;

	osg::ref_ptr<osg::Geode>                m_breakPlane;     //��Ϲ��ߵļ�����

	float                                   m_textSize;
	osg::Vec4                               m_textColor;
	osg::ref_ptr<osg::Geode>                m_connectSEbb;  //���ӹ���ʱ�����ļ�����

	osg::Vec3d                              m_newPointPosLLA;

private:
	std::map<float, float>	                m_HeightThreshold;


};
}
