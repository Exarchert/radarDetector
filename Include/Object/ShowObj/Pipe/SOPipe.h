/***************************************************************************
Copyright (C),  
文件：SOPipe.h
作者：张平
版本：1.0
日期：2009-03-20
描述：管线类

修改记录：
作者						日期					描述
谢荣耀                    2009-12-09            从龙山项目移植
***************************************************************************/
#pragma once
#include <Object/Export.h>
#include <Object/Obj.h>

#include <osgAPEx\SimplePipe>
#include <osgAPEx/LongPipe>
#include <osgAPEx/LandGroove>

namespace EMObj{

class OBJECT_EXPORT CSOPipe : public EMObj::CObj
{
public:
	CSOPipe(void);
	~CSOPipe(void);

	enum
	{
		SOType = 2
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

	//虚函数
	bool ReadProperty(MSXML2::IXMLDOMNodePtr pNode);
	MSXML2::IXMLDOMNodePtr GetProperty();

	void UpdateDraw(DWORD Value = 0);
	bool BeginEditUI();
	bool EndEdit3D();

	void ReplacePipeWithPointLine();

	bool DeletePoint(unsigned int index);

	void UpdatePointPos(unsigned int index,osg::Vec3 pos);

	void RemovePointsLines();

	void AddNewBreakPlane();

	void UpdatePipePos(osg::Vec3& moveVec);

	bool UpdateBreakPlane(int preIndex,int postIndex,osg::Vec3 pos);

	bool RemoveBreakPlane();

	bool BreakPipe();

	bool AddSEFlag();

	bool ConnectPipe(CSOPipe* pPipe,int connectType);

	bool RemoveConnectPipe(CSOPipe* pPipe);

    bool RemoveSEFlag();

 public:
	osg::Vec3d  m_PipeCrossPt; //选中点
	int m_nIndex;              //拾取点后的索引 

	osg::Vec3d pos1,pos2;

	void update();
	osg::ref_ptr<osg::Node> m_Dome;
	double m_DeHeight;
	osg::ref_ptr<osg::Vec4Array> m_Pts;

	float m_radius;
	CString                      m_imagePath;

	osg::Vec4                    m_pointColor;//控制点颜色

	osgAPEx::SimplePipe * GetGeodePipe(){return m_pPipe.get();}

	//获取管线的中心线点数组，坐标为绝对坐标系下的坐标，返回值为坐标点总数
	DWORD GetPipePoints(osg::Vec3dArray &Points);


protected:
	osg::ref_ptr<osgAPEx::SimplePipe> m_pPipe;

private:
	void ReadXmlVec4d(MSXML2::IXMLDOMNodePtr pNode);
    void ReadXmlNodeColor(MSXML2::IXMLDOMElementPtr node,osg::Vec4& color);
    osg::Geode*  CreatePointGeode(osg::Vec3& center,float radius) ;
	osg::Geode*  CreateLineGeode(osg::Vec3& startPoint,osg::Vec3& endPoint);

	bool DeleteLine(unsigned int index);

	bool ParseXml(MSXML2::IXMLDOMNodePtr pNode);

	osg::Geode* CreateBreakGeode(osg::Vec3 prePoint,osg::Vec3 postPoint,osg::Vec3 pos,float radius);

private:

	osg::Vec4 	                 m_pipeColor;
	osg::ref_ptr<osg::Image>     m_image;
	osg::ref_ptr<osg::Texture2D> m_texture;

	osg::Vec4                               m_lineColor;        //线条颜色


	osg::ref_ptr<osg::Geode>                m_axisNode;
	osg::ref_ptr<osg::MatrixTransform>      m_axisTransform;

	osg::ref_ptr<osg::Geode>                m_breakPlane;     //打断管线的几何体

	float                                   m_textSize;
	osg::Vec4                               m_textColor;
	osg::ref_ptr<osg::Geode>                m_connectSEbb;  //连接管线时创建的几何体

public:
	std::vector<osg::ref_ptr<osg::Geode> >  m_PointsNodeList;//N个圆球几何体
	std::vector<osg::ref_ptr<osg::Geode> >  m_LinesNodeList;//N-1条线段几何体

	osg::Vec4              m_ambient;
	osg::Vec4              m_diffuse;
	osg::Vec4              m_specular;
	float                  m_shininess;

	/*
	Add Author:	Liugh
	Purpose:	Xml given ID mapping pipe
	*/
	int						m_givenID;	
};

}
