/*************************************************
file:			UserCreateLine.h
Author:			xubaolong    
Date:			2014-03-26
Description:    用户创建线模型
*************************************************/
#pragma once
#include <Object/Export.h>
#include <osg/MatrixTransform>


namespace EMObj
{
	struct PolygonData
	{
	public:
		PolygonData()
		{
			transparency = 1.0;
		}
		osg::Vec4 color;
		std::vector<osg::Vec3d> positions;
		enum Charactor
		{
			 POLYGON = 0,
			 CIRCLE
		};
		Charactor charactor;
		float radius;
		float transparency;
		PolygonData operator =(PolygonData& pd)
		{
			color = pd.color;
			radius = pd.radius;
			charactor = pd.charactor;
			transparency = pd.transparency;
			for (unsigned int i = 0 ; i < pd.positions.size() ; i++)
			{
				positions.push_back(pd.positions[i]);
			}
	
			return *this;
		}
	};
	

	class OBJECT_EXPORT UserCreatePolygon : public osg::MatrixTransform
	{
	public:
		UserCreatePolygon(void);
		~UserCreatePolygon(void);

		void addPolygon(PolygonData polygonData);
	public:
		void updateDrawGeode();
		void updateDrawPolygon();
		void updateDrawCircle();
		void genPolygonModel(std::vector<osg::Vec3d>& triangles5D);
	protected:
		void test();
		PolygonData m_polygonData;
		osg::ref_ptr<osg::Geode> m_childGeode;
		osg::ref_ptr<osg::MatrixTransform> m_matrixTransform;

	};

}