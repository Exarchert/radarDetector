/************************************************************************
Copyright (C),  
文件：SOXmlLabel.h  SOXmlLabel.cpp
作者：黄志鹏
版本：1.0
日期：2009-09-10
描述：标牌类，通过XML格式设置标牌布局、颜色、显示内容. XML的格式如下：

			<标牌 name = "我的标牌">
				<位置 X="0" Y="0" Z="0"></位置>
				<布局 rows = "3" cols = "5" hmargin = "20" vmargin = "20" rowspace = "10" colspace = "10"></布局>
				<三角面 R="0.8f" G="0.8f" B="0.8f" A="0.5f" handleWidth="0.5f" offsetX="30.f" offsetY="30.f"></三角面>
				<背景 imageFolder="春意怏然"></背景>
				<内容>
					<格子x_y>
						<位置 row = "x" col = "y"></位置>
						<字体名 fontName = "SIMFANG.TTF"></字体名>
						<字体大小 fontSize = "20"></字体大小>
						<颜色 R = "1.f" G = "0.f" B = "0.f" A = "1.f"></颜色>
						<对齐方式 hor = "居中\左对齐\右对齐" ver = "居中\顶对齐\底对齐"></对齐方式>
						<文本>
							你好，美丽的新世界...
						</文本>
					</格子x_y>

					其它格子 ....

				</内容>
			</标牌>


修改记录：
作者					日期					描述
-------------------------------------------------------------------------
黄志鹏				2009-09-28				添加功能：查询某行某列的值

************************************************************************/

#pragma once

#include <Object\Export.h>
#include <Object\Obj.h>
#include <osg\Vec4>
#include <osg\Vec2>
#include <osg\Vec3d>
#include <osg\ref_ptr>
#include <osg\Referenced>
#include <osg\Image>
#include <osgText\Font>
#include <osgDB\WriteFile>
#include <osgAPEx\TableLabel>
#include <osgAPEx\ImageManager>
#include <osgWidget\Widget>
#include <osgWidget\WindowManager>
#include <osgWidget\ViewerEventHandlers>
#include <osgViewer\ViewerEventHandlers>
#include <osg\GraphicsContext>
#include <osg\Camera>
#include <osgWidget\WindowManager>


namespace EMObj		{

class OBJECT_EXPORT CSOXmlLabel : public CObj
{
public:
	CSOXmlLabel(osgWidget::WindowManager *winMan, osgViewer::Viewer *viewer);
	virtual ~CSOXmlLabel();

	/*
	目的：初始化标牌的布局和显示
	参数：params		(in)	XML字符串
	      bChangeLayOut	(in)	布局是否变化, 如果第二次的布局不变就设置为否，避免闪烁
	返回：void
	*/
	void Init(CString params, bool bChangeLayout = true);

	/*
	目的：初始化标牌的布局
	*/
	void Init(const CString &name, unsigned int rows, unsigned int cols, float hmargin, float vmargin, float rowspace, float colspace,
		osg::Vec4 triaColor, float handleWidth, float offsetX, float offsetY);

	/*
	目的：设置某个格子的文本
	参数：row			(in)	行号
	      col			(in)	列号
	      font			(in)	字体名
	      fontSize		(in)	字体大小
	      fontColor		(in)	字体颜色
		  horAlignType	(in)	水平对齐方式
	      verAlignType	(in)	竖直对齐方式
	      text			(in)	文本
	返回：void
	*/
	void SetText(unsigned int row, unsigned int col, const CString &fontName, float fontSize, osg::Vec4 color, unsigned int horAlignType,
				unsigned int verAlignType, const CString &text);

	/*
	目的：重新计算标牌的大小，设置文本后需调用.
	*/
	void Resize();

	//点选
	virtual bool PtSelect(double &x, double &y, osg::Vec3d& eye, osg::Vec3d& vec);

	/*
	目的：设置标牌位置
	*/
	void SetPosition(osg::Vec3d pos);
	//直接设置标牌世界坐标系位置(不保存该位置)
	void SetWorldPosition(const osg::Vec3d &posWorld);

	osg::Vec3d GetPosition() const;

	//获取三角面的偏移
	osg::Vec2 GetOffset();

	/*
	目的：获取标牌的行&列.
	参数：rows		(out)	行数
	      cols		(out)	列数
	返回：成功返回true，否则返回false
	*/
	bool GetRowsCols(unsigned int &rows, unsigned int &cols);

	/*
	目的：获取某行某列的内容
	参数：row		(in)	行索引
	      col		(in)	列索引
	      value		(out)	内容字符串
	返回：成功返回true，否则返回false
	*/
	bool GetValue(unsigned int row, unsigned int col, CString &value);

	//对齐方式名字和标识的映射
	//bhor	(in)	true表示水平方向，false表示竖直方向	
	static unsigned int RemapAlignName2Value(const CString &name, bool bhor=true);
	static CString RemapValue2AlignName(unsigned int value, bool bhor=true);

	//////////////////////////////////////////////////////////////////////////
	//改写CObj行为
	//获取属性信息
	virtual bool ReadProperty(MSXML2::IXMLDOMNodePtr pNode);
	virtual MSXML2::IXMLDOMNodePtr GetProperty();

	//弹出编辑对话框
	virtual bool BeginEditUI();

	//结束编辑
	virtual bool EndEdit3D();

	//刷新绘制
	virtual void UpdateDraw(DWORD Value = 0);

	//显隐控制
	virtual void Show(BOOL bShow);

	//获取XML描述属性
	virtual CString GetXml();
	virtual void SetXml(const CString &xml);

	//重置设置
	virtual void Reset();

	enum {
		SOType = 17
	};

	//类型
	virtual DWORD GetType() {return SOType; }

	//渲染类型
	virtual DWORD GetRenderType(){return Render_Type_Screen; }

	//刷新基本信息
	virtual bool UpdateBaseData();

protected:
	/*
	目的：加载背景图片
	参数: x		(in)	宽度
	      y		(in)	高度
	      list	(out)	图像列表	
	返回：成功返回true，否则返回false
	*/
	bool LoadImages(float x, float y,osgAPEx::ImageManager::SubImageList* list);

	/*
	目的：设置背景图片
	参数：pImage	(in)	图像对象指针
	返回: void
	*/
	void SetBackImage(void *, osg::Referenced *pImage);

	/*
	目的：生成键值
	参数：row	(in)	行索引
	      col	(in)	列索引
	返回：键值
	*/
	ULONG64 MakeKey(unsigned int row, unsigned int col)
	{
		return ((((ULONG64)(row)) << 32) | ((ULONG64)(col)));
	}

	void SetTableLabelHyLine(unsigned int row,unsigned int col);

protected:
	osg::ref_ptr<osgWidget::WindowManager>	m_pwinMan;
	osg::GraphicsContext *m_pgc;
	osg::Camera *m_pcamera;
	osg::ref_ptr<osgAPEx::TableLabel> m_tableLabel; 
	osg::ref_ptr<osgAPEx::ImageManager::SubImageList> m_imagelist;
	unsigned int m_rows, m_cols;
	float m_hmargin;			//水平页边距
	float m_vmargin;			//竖直页边距
	float m_rowspace;			//行间距
	float m_colspace;			//列间距
	CString m_backGround;		//背景图片方案
	std::map<ULONG64, CString> m_content;	//标牌内容
	//三角面的设置
	osg::Vec4	m_triaColor;
	float		m_handleWidth;
	float		m_offsetX;
	float		m_offsetY;

	//编辑时使用
	CString		m_xml;			//xml配置
	osg::Vec3d	m_pos;			//位置(非属厂区则为经纬高，否则为XYZ)

	//显示使用
	bool		m_bInScene;		//是否已在场景中
};

}