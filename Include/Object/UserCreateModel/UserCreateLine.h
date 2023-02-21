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
	struct LineData
	{
	public:
		float with ;
		osg::Vec4 color;
		std::vector<osg::Vec3d> positions;
		LineData operator =(LineData ld)
		{
			with = ld.with;
			color = ld.color;
			for (unsigned int i = 0 ; i < ld.positions.size() ; i++)
			{
				positions.push_back(ld.positions[i]);
			}

			return *this;
		}
		bool insert(int serialNumber,LineData ld)
		{
			with = ld.with;
			color = ld.color;
			if (serialNumber > (positions.size()-1))
			{
				for (unsigned int i = 0 ; i < ld.positions.size() ; i++)
				{
					positions.push_back(ld.positions[i]);
				}
			}
			else if(serialNumber >-1)
			{
				positions.insert(positions.begin()+serialNumber,ld.positions.begin(),ld.positions.end());
			}
			return true;
		}
		bool update(int serialNumber,LineData ld)
		{
			with = ld.with;
			color = ld.color;
			if (serialNumber < positions.size() && serialNumber >-1 &&ld.positions.size()>0)
			{
				std::vector<osg::Vec3d>::iterator itorErase = positions.erase(positions.begin()+serialNumber);
				positions.insert(itorErase,ld.positions[0])	;
			}			
			return true;
		}
		bool deletePoint(int serialNumber)
		{
			if (serialNumber < positions.size())
			{
				positions.erase(positions.begin()+serialNumber);
			}
			return true;
		}

	};

	class OBJECT_EXPORT UserCreateLine : public osg::MatrixTransform
	{
	public:
		UserCreateLine(void);
		~UserCreateLine(void);

		void addLine(LineData lineData);
		void insertLine(int serialNumber,LineData lineData);
		void updateLine(int serialNumber,LineData lineData);
		void deleteLinePoint(int serialNumber);
	public:
		void updateDrawGeode();
	protected:
		LineData m_LineData;
		osg::ref_ptr<osg::Geode> m_childGeode;

	};

}