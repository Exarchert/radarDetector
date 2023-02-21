/*************************************************
file:			ShpModelManager.h
Author:			xubaolong    
Date:			2014-04-01
Description		Shp���ݹ���
*************************************************/
#pragma once
#include <Object/Export.h>
#include <tinyxml.h>
#include <vector>
#include <map>
#include <osg/MatrixTransform>
#include <Object/Shp/ShpNodeCallback.h>


namespace EMObj
{
	struct LayerConfig
	{  
	public:
		float	_minHeight ;
		float	_maxHeight ;

		float   _minDis ;
		float	_maxDis ;

		float  _heightOffset ;
		osg::Vec4 _color;
		osg::Vec3 _center;

		double _devation ;

		bool   _addScene ;
	};
	struct PicLayerConfig
	{  
	public:
		PicLayerConfig()
		{
			_maxHeight = FLT_MAX;
			_minHeight = 0.0;
		}
		float	_minHeight ;
		float	_maxHeight ;

		osg::Vec3 _center;

		std::string _picFilePath;
	};
	struct ShpNodeConfig
	{
	public:
		double _minlon;
		double _minlat;
		double _maxlon;
		double _maxlat;
		std::string _fileName;
		std::string _id;
	};
	struct ShpPicNodeConfig
	{
	public:
		double _longitude;
		double _latiude;
		double _heightitude;
		std::string _picFilePath;
		std::string _id;
	};
	class OBJECT_EXPORT ShpModelManager : public osg::Referenced
	{
	public:
		ShpModelManager(void);
		~ShpModelManager(void);

		void Init(osg::Group *sceneRoot,osg::Group* hudRoot);

		bool readShpXmlFile(std::string xmlFilePath);
		void readNodeToScene();
		void computerModelToEarth();

		enum Em_Oatype{
			EM_OATYPE_Visible = 1,
			EM_OATYPE_Select,            //ѡ��
			EM_OATYPE_Transparent,
			EM_OATYPE_Flicker,
			EM_OATYPE_Clear,
		};

		//����ͼ�㿪�ؿ��ƣ�Ĭ������ʾtrue��false ������
		void setLayerSwitch(std::string layerId,bool isSwitch = true);


		bool eyeToCenter(osg::Vec3d eye,std::map<std::string,double>& lengths,std::map<std::string,osg::Vec3d>& centersd);

		void deleteShpNode(CString id);
		void PtSelect(double &x, double &y,osg::Vec3d& startPos, osg::Vec3d& endPos, std::map<std::string,int>& selectShpNode);

		void setTransparent(bool transparent);

		void setOatype(std::string shpNodeName,Em_Oatype type,float values=0);

		bool getOatype(std::string shpNodeName,Em_Oatype type,bool& isOatype);

		osg::ref_ptr<osg::MatrixTransform> getShpNodeForWord(std::string shpNodeName);
	private:
		void bulidTreeFromXml(const TiXmlElement *node,const TiXmlElement* configMap);
		bool string2bool(const char* c_str);
		bool loadvecLayer(std::string filename);
		void configLayer(const TiXmlElement *config, std::string &type,std::string& layerName);
		bool psNode(osg::Vec3d& pos,osg::Vec3d& posEnd,osg::ref_ptr<osg::Node> node);
		void setStateSet(osg::Node* node,osg::Vec4d emission,osg::Vec4d diffuse,float transparent = 0.4f);

	protected:
		typedef std::map<std::string, osg::ref_ptr<osg::Node>> MapShpNodeModel;//�ڵ���,�ڵ����
		typedef std::map<std::string, MapShpNodeModel> MapLayerChildNode;//ͼ�������ڵ����
		typedef std::map<std::string, osg::ref_ptr<osg::MatrixTransform>> MapLayerNodeModel;//ͼ������ͼ�����
		typedef std::map<std::string, std::vector<ShpNodeConfig>> MapShpNodeConfig;	//ͼ�������ڵ���Ϣ��
		typedef std::map<std::string, LayerConfig> MapLayerConfig;//ͼ������ͼ����Ϣ��

		MapLayerChildNode     m_mapLayerChildNode;
		MapShpNodeConfig      m_mapShpNodeConfig;
		MapLayerConfig        m_mapLayerConfig; 
		MapLayerNodeModel     m_mapLayerNodeModel;

		//typedef std::map<std::string,osg::ref_ptr<osg::LOD>> MapLayerLod;
		//MapLayerLod m_selectLod;

		osg::ref_ptr<osg::Switch> m_shpRoot;
		osg::ref_ptr<osg::Group> m_hudRoot;

		std::string m_xmlPath;

		std::map<std::string,osg::ref_ptr<ShpNodeCallback>> m_shpNodeCallbacks;
		std::map<std::string,Em_Oatype>   m_shpNodeOatype;

	public:
		//���ͼ����Ϣ
		bool addPicLayerConfig(std::string layerId, PicLayerConfig layerConfig);
		//���һ��ͼƬ�ڵ�
		bool addShpPicConfig(std::string layerId,std::vector<ShpPicNodeConfig> shpPickNodeConfig);
		std::map<std::string,std::vector<ShpPicNodeConfig>>& getShpPicNodeConfigMap()
		{
			return m_mapShpPicNodeConfig;
		}

	private:
		typedef std::map<std::string,std::vector<ShpPicNodeConfig>> MapShpPicNodeConfig;  //ͼ���� �ڵ���Ϣ
		typedef std::map<std::string,PicLayerConfig>                 MapPicLayerConfig;    //ͼ���� ͼ����Ϣ   

		MapShpPicNodeConfig                                          m_mapShpPicNodeConfig;
		MapPicLayerConfig                                            m_mapPicLayerConfig;
	};

	//XML��ȡ�ͱ���ģ��
	OBJECT_EXPORT ShpModelManager *GetShpModelManager();
}
