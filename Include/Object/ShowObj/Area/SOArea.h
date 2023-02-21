/*************************************************************************
FileName：CSOArea.h CSOArea.cpp
Author:	Liugh
Date：2009-12-10
Description：Area Create
**************************************************************************/

#pragma once
#include <osg/Geode>
#include <vector>
#include <OpenThreads/Mutex>

namespace EMObj
{
	class CSOArea :	public EMObj::CObj
	{
	public:
		CSOArea(void);
		~CSOArea(void);

		enum{
			SOType = 15
		};

		enum DrawType
		{
			FLOAT,
			COVERD
		};

		//获取区域半径和区域中心坐标
		bool GetAreaRadiusAndCenter(osg::Vec3d& center,float* radius = 0);

		//设置绘制颜色
		void SetAreaColor(COLORREF color);

		//设置透明度
		void SetAlpha(const float& alpha);

		//设置区域材质
		void SetMaterialToArea(float& w,float& h);

		//设置绘制区域的类型:	0-平面	/	1-曲面
		void SetAreaType(UINT type);

		//获取区域二维纹理坐标
		bool GetTexCoords(const float w,const float h,osg::Vec2Array* texArray);

		bool GetTexCoodsSingle(const float w,const float h,osg::Vec2Array* texArray);


		//根据绘制类型设置区域高度
		void ResetHeight();

		//高程设置
		void SetHeight(const float& h);

		 void GetHeight(float& h);

		//设置中心点坐标与姿态
		void SetCenterPosAndPose(const osg::Vec3d& cenPos, const osg::Vec3f& cenPose);


		void GetCenterPosAndPose(osg::Vec3d& cenPos,osg::Vec3f& cenPose);


		//得到点数据
		void getVertexArray(osg::ref_ptr<osg::Vec3Array> pArray);

		//设置区域矩阵
		void SetMatrixT(double Lon, double Lat, double Altitude, float Yaw, float Pitch, float Roll);

		void SetFillType(int type);

		//获取企业ID
		int GetEnterID();
	
	public:		//虚函数

		//读取和获取属性信息
		virtual bool ReadProperty(MSXML2::IXMLDOMNodePtr pNode);

		virtual MSXML2::IXMLDOMNodePtr GetProperty();

		void UpdateDraw(DWORD Value = 0);

		
		DWORD GetType()
		{
			return SOType;
		}

		DWORD GetRenderType()
		{
			return Render_Type_Scene;
		}
		//弹出编辑对话框
		bool BeginEditUI();
		bool EndEdit3D();

	protected:
		//闪烁控制，可改写该行为
		virtual void FlickPulse(bool bflag);
		virtual void FlickRestore();
		
	public:		//数据成员
		int		m_FillType;			//贴图方式
		bool	m_bCanceled;		//取消操作标志
		BOOL	m_bUseMaterial;
		UINT	m_repeat;			//贴图重复率
		float	m_height;			//平面绘制海拔高度
		float	m_AreaSlope;		//坡度
		DrawType	_AreaType;
		osg::Vec4f	_AreaColor;
		CString		_ImagePath;
		osg::Vec3d	m_CenPos;
		osg::Vec3	m_CenPose;
		osg::ref_ptr<osg::Vec3Array> _ptArray;
		osg::ref_ptr<osg::Geode>	_AreaGeode;
		//位置节点
		//osg::ref_ptr<osg::MatrixTransform> m_areaMT;
		OpenThreads::Mutex m_mutex;
	};
}