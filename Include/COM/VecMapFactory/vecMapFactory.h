/************************************************************
Copyright (C),  
�ļ���vecMapFactory.h vecMapFactory.cpp
���ߣ�zhangqiang
�汾��1.0
���ڣ�2011-2-23
������ʸ������.

�޸ļ�¼��
����					����				����
-------------------------------------------------------------*/

#pragma once
#include <osg/Object>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>
#include <osgVec/VecLayerManagerNode>
#include <tinyxml.h>

namespace COM	{

	class COM_EXPORT CVecMapFactory : public osg::Referenced 
	{
	public:
		CVecMapFactory();
		~CVecMapFactory();
		
		osgVec::VecLayerNode* loadvecLayer(std::string filename,osgVec::VecLayerManagerNode* vecManager);
		bool LoadMap(const std::string& filename,osgVec::VecLayerManagerNode* vecManager);

	private:
		//read 
		BOOL string2bool(const char* c_str);
		void configLayer(osgVec::VecLayerNode* layernode,const TiXmlElement* config,std::string& type );
		osg::Vec4 str2vec4(const char* str);
		void bulidTreeFromXml(const TiXmlElement* node,const TiXmlElement* configMap,osgVec::VecLayerManagerNode* vecManager);
	private:
		std::string m_xmlPath;
		CTreeCtrl*	m_tree;
		int			m_i;	
	};

}