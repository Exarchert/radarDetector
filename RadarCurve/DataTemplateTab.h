#pragma once
#include <RadarCurve/DBTable.h>

#include <RadarCurve/RadarData.h>
#include <osg/Vec3d>
#include <osg/Vec3f>

typedef struct StuRadarIOS
{
	unsigned char ucBuf[4096];
	std::string strBuf;

}RadarIOS;
class DataTemplateTab : public CDBTable
{
public:
	DataTemplateTab(void);
	~DataTemplateTab(void);


	void openStream(CDBConnectHelp *lpHelp);
	void closeStream();
	
	void setProjectName( std::string const& projectName );
	std::string getProjectName()const;

	void insertEX( CDBConnectHelp *lpHelp, const DBRow *lpRow );
	virtual void insert( CDBConnectHelp *lpHelp, const DBRow *lpRow );
	virtual void update( CDBConnectHelp *lpHelp, const DBRow *lpRow );
	virtual void del( CDBConnectHelp *lpHelp, const DBRow *lpRow );
	virtual void drop(CDBConnectHelp *lpHelp);
	virtual std::string getSelectSql();
	std::string getSelectSqlAll();

	bool createTab(CDBConnectHelp *lpHelp);

	bool bFExport;


	bool exportSqlDataRadarIni(std::string strFileNamePath,int iCount,int sampleCount );
	bool importSqlDataRadarIni(CDBConnectHelp *lpHelp,std::string strFileNamePath,int sampleCount);


	bool GetAllPoint(CDBConnectHelp *lpHelp,const std::string strProjectName,const std::string strIndex,std::vector< osg::Vec3f>& _vec);
	//bool GetAllPoint(CDBConnectHelp *lpHelp,const std::string strProjectName,const std::string strIndex,std::vector< osg::Vec3d>& _vec);
protected:
	virtual DBRow *readImpl( DBAPI::CRecordSetBase *lpRecordSet );
	
	std::string _projectName;

	std::string _strDBUserName;
};
