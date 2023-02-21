/***************************************************************************
Copyright (C),  
文件：CatmullRomInterpolator.h
作者：谢荣耀
版本：1.0
日期：2009-12-08
描述：实现了一种过控制点的插值算法（catmullrom算法）

修改记录：
作者						日期					描述
***************************************************************************/
#pragma once
#include <vector>

template<typename T>
class TemplateCatmullRomInterpolator
{
public:
	TemplateCatmullRomInterpolator(){};
	~TemplateCatmullRomInterpolator(){};

	TemplateCatmullRomInterpolator(const std::vector<T>& pointsList){ m_controlPointsList = pointsList; }

	void setPointsList(const std::vector<T>& pointLists){ m_controlPointsList = pointLists;}
	const std::vector<T>& getPointsList() { return m_controlPointsList; }

	T computerSplinePoints(float t)
	{
		m_delta = (float)1 / (float)m_controlPointsList.size();
		int p = (int)(t/m_delta);
		int i0 = p - 1;     Clamp(i0);
		int i1 = p;         Clamp(i1);
		int i2 = p + 1;     Clamp(i2);
		int i3 = p + 2;     Clamp(i3);

		float lt = (t - m_delta*(float)p) / m_delta;

		T p0 = m_controlPointsList[i0];
		T p1 = m_controlPointsList[i1];
		T p2 = m_controlPointsList[i2];
		T p3 = m_controlPointsList[i3];

		float t2 = lt * lt;
		float t3 = t2 * lt;
		osg::Vec4 parametric(0.5,0.5*lt,0.5*t2,0.5*t3);//参数列表

		osg::Matrix  matrix;//系数矩阵
		matrix.set(0,2,0,0,
					-1,0,1,0,
					2,-5,4,-1,
					-1,3,-3,1);

		osg::Vec4 vec= parametric*matrix;

		return T(p0*vec[0] + p1*vec[1] + p2*vec[2] + p3*vec[3]);
	}

private:
	void Clamp(int& index)
	{
		if (index < 0) 
		{
			index = 0;
		} 
		else 
		{
			if (index >= (int)m_controlPointsList.size()-1)
			{
				index = m_controlPointsList.size() - 1; 
			}
		}
	}

private:
    float          m_delta;

	std::vector<T> m_controlPointsList;
	std::vector<T> m_splinePointsList;
};

typedef TemplateCatmullRomInterpolator<osg::Vec3>   CatmullRomInterpolatorVec3;
typedef TemplateCatmullRomInterpolator<osg::Vec3d>  CatmullRomInterpolatorVec3d;
