/***************************************************************************
Copyright (C),  
文件：SOPipe.h
作者：张平
版本：1.0
日期：2009-03-20
描述：长距离管线类

修改记录：
作者						日期					描述
谢荣耀                    2009-12-22            从龙山项目移植
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
	//使用点球和线替换管线，用于编辑管线点
	void ReplacePipeWithPointLine();

	//添加管道的起始S和结尾E标志
	bool AddSEFlag();

	//移除S、E标志
	bool RemoveSEFlag();

	/*
	目的：更新关键点的位置
	参数: index	(in)	更新点的索引
	      pos	(in)	新的位置	
	*/
	void UpdatePointPos(unsigned int index,osg::Vec3 pos);
	
	/*
	目的：删除指定索引的控制点
	参数: index	(in)	删除点的索引
	返回：删除成功返回true，否则返回false
	*/
	bool DeletePoint(unsigned int index);

	/*
	目的：删除指定索引的中心线
	参数: index	(in)	删除线的索引
	返回：删除成功返回true，否则返回false
	*/
	bool DeleteLine(unsigned int index);

	/*
	目的：更新管道的位置
	参数: moveVec	(in)	更新的位移向量
	*/
	void UpdatePipePos(osg::Vec3& moveVec);

	//在管线的开始处添加打断切面
    void AddNewBreakPlane();

	/*
	目的：更新打断切面的位置
	参数: preIndex	(in)	当前打断面位置的前一个控制点索引
	      postIndex	(in)	当前打断面位置的后一个控制点索引
		  pos   	(in)	当前打断面的位置
	返回：成功返回true，否则返回false
	*/
	bool UpdateBreakPlane(int preIndex,int postIndex,osg::Vec3d pos);

	//根据事件处理器保存的打断点，打断管线
	bool BreakPipe();

	/*
	目的：连接管线
	参数: pPipe	        (in)	被连接的管线
	      connectType	(in)	连接类型
	返回：成功返回true，否则返回false
	*/
    bool ConnectPipe(CSOLongPipe* pPipe,int connectType);
	
	//删除中心线几何体
	void RemovePointsLines();

	//在指定索引处添加一个中心点
	void AddNewPoint(int index);

	//重写拾取
	virtual bool PtSelect(double &x, double &y, osg::Vec3d& eye, osg::Vec3d& vec);

	//获取管线的点数组，坐标为绝对坐标系下的坐标，返回值为坐标点总数
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
    //从节点中提取所有数据
	bool ParseXml(MSXML2::IXMLDOMNodePtr pNode);
	//从节点中提取管线属性数据
    bool ParsePipeAttributeXml(MSXML2::IXMLDOMNodePtr pNode);
	//从节点中提取线属性数据
    bool ParseLineAttributeXml(MSXML2::IXMLDOMNodePtr pNode);
	//从节点中读取颜色信息
    void ReadXmlNodeColor(MSXML2::IXMLDOMElementPtr node,osg::Vec4& color);

	//创建点几何体
	osg::Geode*  CreatePointGeode(osg::Vec3& center,float radius) ;
	//创建线几何体
	osg::Geode*  CreateLineGeode(osg::Vec3& startPoint,osg::Vec3& endPoint);
	//创建切面几何体
	osg::Geode*  CreateBreakGeode(osg::Vec3d prePoint,osg::Vec3d postPoint,osg::Vec3d pos,float radius);
    //移除切面几何体
    bool RemoveBreakPlane();

public:
	float GetTotalLength();

public: 
	float                                 m_radius;   //管子半径
	float                                 m_segLength;//分段长度
	float                                 m_pipeVisiableDis;//管道高度阀值
	osg::Vec4                             m_pipeColor;//管道颜色

	osg::Vec4                             m_pipeAmbient;//管道材质
	osg::Vec4                             m_pipeDiffuse;
	osg::Vec4                             m_pipeSpecular;
	float                                 m_pipeShininess;
	CString                               m_imageFilePath;//贴图路径

	float                                 m_lineVisiableDis;//线高度阀值
	float                                 m_initLineWidth;//线初始宽度
	osg::Vec4                             m_lodLineColor;//线颜色
	std::map<float,float>                 m_disLineWidthMap;//相机距离与线宽的数值对
	std::map<float,float>                 m_disLineOffsetMap;//相机距离与线偏移量的数值对


	osg::Vec3d pos1,pos2;

	osg::ref_ptr<osgAPEx::LongPipe>       m_longPipe;

	osg::ref_ptr<osg::Vec3dArray>         m_pipePoints;//点数据
	osg::ref_ptr<osg::MatrixTransform>    m_longPipeMT;
	osg::ref_ptr<osg::MatrixTransform>    m_moveMT;

	osg::ref_ptr<osg::Vec2dArray>         m_left, m_right;
	osg::ref_ptr<osg::Node>               m_Dome;

public:
	osg::Vec4                               m_lineColor;        //线条颜色
	osg::Vec4                               m_pointColor;       //控制点颜色
	std::vector<osg::ref_ptr<osg::Geode> >  m_PointsNodeList;//N个圆球几何体
	std::vector<osg::ref_ptr<osg::Geode> >  m_LinesNodeList;//N-1条线段几何体
	osg::ref_ptr<osg::Geode>                m_axisNode;
	osg::ref_ptr<osg::MatrixTransform>      m_axisTransform;

	osg::ref_ptr<osg::Geode>                m_breakPlane;     //打断管线的几何体

	float                                   m_textSize;
	osg::Vec4                               m_textColor;
	osg::ref_ptr<osg::Geode>                m_connectSEbb;  //连接管线时创建的几何体

	osg::Vec3d                              m_newPointPosLLA;

private:
	std::map<float, float>	                m_HeightThreshold;


};
}
