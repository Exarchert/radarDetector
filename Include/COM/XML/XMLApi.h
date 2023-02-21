/*************************************************************************
File:			XMLApi.h
Author:			��ƽ    
Date:			2009-11-4
Description:    XML�Ļ������ݶ���

Alter History:
				09/12/14	LiuGh	��ӷ���Vec3Array��Xml��������

*************************************************************************/
#pragma once
#include <COM\Export.h>
#include <com\xml\XMLNode.h>
#include <Osg/Vec2d>
#include <osg/Vec3f>
#include <osg/Vec3d>
#include <Osg/Vec4f>
#include <osg/Array>
#include <osg/matrixd>

namespace COM{

COM_EXPORT bool CXMLGetVec2d(osg::Vec2d &Vec, MSXML2::IXMLDOMNodePtr pNode);
COM_EXPORT MSXML2::IXMLDOMNodePtr CXMLGetNode(osg::Vec2d &Vec, MSXML2::IXMLDOMDocumentPtr pDoc);

COM_EXPORT bool CXMLGetVec3d(osg::Vec3d &Vec, MSXML2::IXMLDOMNodePtr pNode);
COM_EXPORT MSXML2::IXMLDOMNodePtr CXMLGetNode(osg::Vec3d &Vec, MSXML2::IXMLDOMDocumentPtr pDoc);

COM_EXPORT bool CXMLGetVec3f(osg::Vec3f &Vec, MSXML2::IXMLDOMNodePtr pNode);
COM_EXPORT MSXML2::IXMLDOMNodePtr CXMLGetNode(osg::Vec3f &Vec, MSXML2::IXMLDOMDocumentPtr pDoc);

COM_EXPORT bool CXMLGetVec4f(osg::Vec4f &Vec, MSXML2::IXMLDOMNodePtr pNode);
COM_EXPORT MSXML2::IXMLDOMNodePtr CXMLGetNode(osg::Vec4f &Vec, MSXML2::IXMLDOMDocumentPtr pDoc);

COM_EXPORT bool CXMLGetMinMaxVec2(osg::Vec2 &Vec, MSXML2::IXMLDOMNodePtr pNode);

COM_EXPORT bool CXMLGetVec3dArray(osg::Vec3dArray &Vec3dArray, MSXML2::IXMLDOMNodePtr pNode);
COM_EXPORT bool CXMLGetVec3Array(osg::Vec3Array& Vec3A, MSXML2::IXMLDOMNodePtr pNode);

COM_EXPORT CString CXMLGetString(MSXML2::IXMLDOMNodePtr pNode);

COM_EXPORT MSXML2::IXMLDOMNodePtr CXMLGetNode(CString &strXML);

COM_EXPORT bool CXMLGetMat(osg::Matrixd& matrix,MSXML2::IXMLDOMNodePtr pNode);

}