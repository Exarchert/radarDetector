/*************************************************
file:			CmdToRadar.h
Author:			yg    
Date:			2016年7月4日
Description:    雷达模块的ocx接口-不合理
*************************************************/

#pragma once
#include <RadarCurve/export.h>
//#include <Kernel/CmdHandleCenter/CmdHandle.h>
#include <Kernel/CmdHandleCenter/CmdManager.h>

#include <osgAPEx/CoordinateTool>
#include <osg/MatrixTransform>
#include <osg/Geode>
#include <osg/Geometry>
#include <osgDB/WriteFile>
#include <osgDB/ReadFile>
#include <osg/LineWidth>
#include <osgUtil/Tessellator>
class DetectRoadModel : public osg::MatrixTransform
{
public:
	DetectRoadModel(std::vector<osg::Vec3f> points, osg::Vec3f offset, float leftDis, float rightDis, osg::Vec4f color)
	{
		//计算道路的局部矩阵
		osg::Matrixd toOriginMat;
		osg::Vec3 LLA = points[0];
		osg::MatrixTransform* MT = new osg::MatrixTransform;

		osg::Matrixd localToWorld;
		osg::Matrixd worldToLocal;
		osgAPEx::ComputeLocalToWorldTransformFromLongLatHeight(LLA.x(),LLA.y(),LLA.z(), localToWorld );
		MT->setMatrix(localToWorld);//ok
		//bool aaa = MT->addChild(osgDB::readNodeFile("D:\\GDJW_Data\\osgData\\cow.osg"));
		worldToLocal = osg::Matrixd::inverse(localToWorld);
		
		std::vector<osg::Vec3f> linePoints;
		//以该条道路的第一个点为原点（0，0，0）计算其他点相对该点的局部坐标
		for(int i = 0; i != points.size(); ++i)
		{
			osg::Vec3d xyz;
			osgAPEx::ConvertLongLatHeightToXYZ( points[i].x(), points[i].y(), points[i].z(), xyz );
			osg::Vec3d localPos = xyz * worldToLocal;

			if (i == 0 || i == (points.size()-1))
			{
				linePoints.push_back(localPos);
				continue;
			}
			if (linePoints.size()!=0 && ((linePoints[linePoints.size()-1]-localPos).length())>=5)
			{
				linePoints.push_back(localPos);
			}
		}
		osg::Vec3Array* coor = CalcRoadPoints(linePoints, leftDis, rightDis);

		osg::Geode* pGeode = new osg::Geode;
		osg::Geometry* geom = new osg::Geometry;
		geom->setVertexArray(coor);
		osg::Vec4Array* pColor = new osg::Vec4Array;
		pColor->push_back(color);
		geom->setColorArray(pColor);
		geom->setColorBinding(osg::Geometry::BIND_OVERALL);
		geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POLYGON,0,coor->size()));
		//创建分割类型为几何体
		osgUtil::Tessellator* pTessel = new osgUtil::Tessellator;
		pTessel->setTessellationType(osgUtil::Tessellator::TESS_TYPE_GEOMETRY);
		//设置轮廓线
		pTessel->setBoundaryOnly(false);
		pTessel->setWindingType(osgUtil::Tessellator::TESS_WINDING_ODD);
		pTessel->retessellatePolygons(*geom);

		geom->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
		pGeode->addDrawable(geom);
		

// 		osg::Geode* lineGeode = new osg::Geode;
// 		{
// 			osg::Vec3Array* LineCoor = new osg::Vec3Array;
// 			for (int i = 0; i != linePoints.size();++i)
// 			{
// 				LineCoor->push_back(linePoints[i]);
// 			}
// 			osg::Geometry* lineGeom = new osg::Geometry;
// 			lineGeom->setVertexArray(LineCoor);
// 			osg::Vec4Array* pLineColor = new osg::Vec4Array;
// 			pColor->push_back(osg::Vec4f(1.0,0.0,0.0,0.8));
// 			lineGeom->setColorArray(pLineColor);
// 			lineGeom->setColorBinding(osg::Geometry::BIND_OVERALL);
// 			lineGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP,0,LineCoor->size()));
// 			lineGeode->addDrawable(lineGeom);
// 		}

		MT->addChild(pGeode);
		//MT->addChild(lineGeode);
		//bool bbb = osgDB::writeNodeFile(*MT, "F:\\1.ive");
		addChild(MT);
	}

	//求向量的法向量的单位向量，默认z = A;
	osg::Vec3f ComputeNormal(osg::Vec3f pNormal)
	{
		osg::Vec3f normal;
		normal = osg::Vec3f(0.0,0.0,1.0)^pNormal;
// 		if(0 == pNormal.y())
// 		{
// 			normal.y() = 1;
// 			normal.x() = 0;
// 			normal.z() = pNormal.z();
// 		}
// 		else
// 		{
// 			normal.x() = 1;
// 			normal.y() = -(pNormal.x()/pNormal.y())*normal.x();
// 			normal.z() = pNormal.z();
// 		}
 		normal.normalize();
		return normal;
	}



	//计算点
	osg::Vec3Array* CalcRoadPoints(std::vector<osg::Vec3f> linePoints,float leftDis, float rightDis)
	{
		osg::Vec3f pDirNormal = linePoints[linePoints.size()-1]-linePoints[0];
		osg::Vec3f pVerticalNormal = ComputeNormal(pDirNormal);
		pVerticalNormal.normalize();

		osg::Vec3Array* coor = new osg::Vec3Array;
		for (int i = 0; i != linePoints.size(); ++i)
		{
			//左侧的点
			coor->push_back(linePoints[i]+pVerticalNormal*leftDis);
		}

		for (int i = linePoints.size()-1; i != -1; --i)
		{
			//左侧的点
			coor->push_back(linePoints[i]-pVerticalNormal*rightDis);
		}

		return coor;
// 		osg::Vec3Array* coor = new osg::Vec3Array;
// 		std::vector<osg::Vec3f> leftPoints,rightPoints;
// 
// 		//计算第一个点的左右点
// 		osg::Vec3f tempLeftPoint;
// 		osg::Vec3f tempRightPoint;
// 		osg::Vec3f dirNormal = linePoints[linePoints.size()-1]-linePoints[0];
// 		dirNormal.normalize();
// 		//第一个点的法向量
// 		osg::Vec3f verticalNormal = CalcVerticalNormal(linePoints[0], dirNormal);
// 		CalcLeftRightPoint(tempLeftPoint, tempRightPoint, linePoints[0], leftDis, rightDis,
// 			 dirNormal, verticalNormal);
// 		leftPoints.push_back(tempLeftPoint);
// 		rightPoints.push_back(tempRightPoint);
// 	
//    		for (int i = 1; i != linePoints.size()-1; ++i)//
//    		{
//    			//到达该点的向量、从改点出发的向量
//    			osg::Vec3f pArriveNormal = linePoints[i]-linePoints[i-1];pArriveNormal.normalize();
//    			osg::Vec3f pGoNoemal = linePoints[i+1]-linePoints[i];pGoNoemal.normalize();
//    			AddOnePoint(linePoints[i],pArriveNormal,pGoNoemal,leftDis,rightDis,leftPoints,rightPoints);
//    		}
// 
// 		//最后一个点
//    		osg::Vec3f lastLeftPoint;
//    		osg::Vec3f lastRightPoint;
//    		dirNormal = linePoints[linePoints.size()-1]-linePoints[linePoints.size()-2];
//    		verticalNormal = CalcVerticalNormal(linePoints[linePoints.size()-1], dirNormal);
//    		CalcLeftRightPoint(lastLeftPoint, lastRightPoint, linePoints[linePoints.size()-1], leftDis, rightDis,
//    			dirNormal, verticalNormal);
//    		//leftPoints.push_back(lastLeftPoint);
//    		rightPoints.push_back(lastRightPoint);
// 
// // 		for (int i = 0; i != leftPoints.size(); ++i)
// // 		{
// // 			coor->push_back(leftPoints[i]);
// // 		}
// 		for (int i = rightPoints.size()-1; i != -1; --i)
// 		{
// 			coor->push_back(rightPoints[i]);
// 		}
// 		return coor;
	}


	//old=====
	void AddOnePoint(osg::Vec3f point, osg::Vec3f pArriveNormal, osg::Vec3f pGoNoemal,float leftDis, float rightDis,
		std::vector<osg::Vec3f>& leftPoints,std::vector<osg::Vec3f>& rightPoints)
	{
		//计算垂直向量
		osg::Vec3f pArriveVerticalNormal = CalcVerticalNormal(point, pArriveNormal);
		osg::Vec3f pGoVerticalNormal = CalcVerticalNormal(point, pGoNoemal);
		
		osg::Vec3f pArriveLeftPoint, pArriveRightPoint, pGoLeftPoint, pGorightPoint;
		CalcLeftRightPoint(pArriveLeftPoint, pArriveRightPoint, point, leftDis, rightDis,pArriveNormal,pArriveVerticalNormal);
		CalcLeftRightPoint(pGoLeftPoint, pGorightPoint, point, leftDis, rightDis,pGoNoemal,pGoVerticalNormal);
		leftPoints.push_back(pArriveLeftPoint);
		//leftPoints.push_back(pGoLeftPoint);
		//rightPoints.push_back(pArriveRightPoint);
		//rightPoints.push_back(pGorightPoint);
// 		osg::Vec3f OA = pArriveLeftPoint-point,OB=pGoLeftPoint-point;
// 		if (((OB-OA)*pArriveNormal>0))
// 		{
// 			leftPoints.push_back(pArriveLeftPoint);
// 			leftPoints.push_back(pGoLeftPoint);
// 
// 			osg::Vec3f pCrossPoint;
// 			osg::Vec3f tempNormal = (OA+OB);
// 			tempNormal.normalize();
// 			float tempDis = leftDis/(tempNormal*pArriveVerticalNormal);
// 			pCrossPoint = point + tempNormal*tempDis;
// 			rightPoints.push_back(pCrossPoint);
// 
// 		}
// 		else//求交点
// 		{
// 			osg::Vec3f pCrossPoint;
// 			osg::Vec3f tempNormal = (OA+OB);
// 			tempNormal.normalize();
// 			float tempDis = leftDis/(tempNormal*pArriveVerticalNormal);
// 			pCrossPoint = point + tempNormal*tempDis;
// 			leftPoints.push_back(pCrossPoint);
// 
// 			rightPoints.push_back(pArriveRightPoint);
// 			rightPoints.push_back(pGorightPoint);
// 		}
	}

	osg::Vec3f CalcVerticalNormal(osg::Vec3f pPoint, osg::Vec3f pNormal)
	{
		osg::Vec3f normal;
		if(0 == pNormal.y())
		{
			normal.y() = 1;
			normal.x() = 0;
			normal.z() = pNormal.z();
		}
		else
		{
			normal.x() = 1;
			normal.y() = -(pNormal.x()/pNormal.y())*normal.x();
			normal.z() = pNormal.z();
		}
		normal.normalize();
		return normal;

// 		osg::Vec3f pVerticalNormal;
// 		float x,y;
// 		if (pNormal[0]==0)
// 		{
// 			x = 1;
// 			y = 0;
// 		}
// 		else if (pNormal[1]==0)
// 		{
// 			x = 0;
// 			y = 1;
// 		}
// 		else
// 		{
// 			y = 1;
// 			x = -(pNormal[1]/pNormal[0])*y;
// 		}
// 		pVerticalNormal.x()=x;
// 		pVerticalNormal.y()=y;
// 		pVerticalNormal.z()=pPoint.z();
// 		pVerticalNormal.normalize();
// 		return pVerticalNormal;
	}

	//计算左右点
	void CalcLeftRightPoint(osg::Vec3f& pLeftPoint, osg::Vec3f& pRightPoint, 
		osg::Vec3f pPoint,float leftDis, float rightDis,
		osg::Vec3f pNormal, osg::Vec3f pVerticalNormal)
	{
// 		if (((pNormal^pVerticalNormal)*osg::Vec3f(0.0,0.0,1.0)) > 0)//左侧
// 		{
// 			pLeftPoint = pPoint+pVerticalNormal*leftDis;
// 			pRightPoint = pPoint-pVerticalNormal*rightDis;
// 		}
// 		else//右侧
// 		{
// 			pLeftPoint = pPoint-pVerticalNormal*leftDis;
// 			pRightPoint = pPoint+pVerticalNormal*rightDis;
// 		}
		pLeftPoint = pPoint+pVerticalNormal*leftDis;
		pRightPoint = pPoint-pVerticalNormal*rightDis;
	}
};

class RADAR_EXPORT CmdRadarDetection : public CCmdHandle
{
public:
	CmdRadarDetection();
	~CmdRadarDetection();

	//获取解析器的名字
	virtual CString GetName();

	//执行命令
	virtual CString ExecuteCmd(CString Cmd);
private:
	CString DrawRadarArea(CString Cmd);
	CString ClearRadarArea(CString Cmd);

private:
	std::map<int, osg::ref_ptr<DetectRoadModel>> m_pDetectRoadModel;
};



//#pragma once
////#include <RadarCurve/export.h>
//
//#include <Kernel/CmdHandleCenter/CmdManager.h>
//
//#include "RadarCurve/RadarManager.h"
//
//class CmdRadarDetection : public CCmdHandle
//{
//public:
//	CmdRadarDetection()
//	{
//		RadarManager::Instance()->init();
//	}
//	~CmdRadarDetection()
//	{
//
//	}
//
//	//获取解析器的名字
//	virtual CString GetName()
//	{
//
//		return L"";
//	}
//
//	//执行命令
//	virtual CString ExecuteCmd(CString Cmd)
//	{
//		return L"";
//	}
//protected:
//private:
//};