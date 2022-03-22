#ifndef __VIDEO_TEMPLATE_TAB_H_FILE__
#define __VIDEO_TEMPLATE_TAB_H_FILE__

#include <RadarCurve/DBTable.h>
#include <RadarCurve/DBRow.h>
#include <RadarCurve/DataBuff.h>

class VideoTemplateTab : public CDBTable
{
public:
	VideoTemplateTab();
	class VideoRow : public DBRow
	{
	public:
		INT64 _recvTime;
		std::string _filePath;
		int _cameraNum;
	};
	void setProjectName( std::string const& projectName );
	std::string& getProjectName();

	virtual void insert( CDBConnectHelp *lpHelp, const DBRow *lpRow );
	virtual void update( CDBConnectHelp *lpHelp, const DBRow *lpRow );
	virtual void del( CDBConnectHelp *lpHelp, const DBRow *lpRow );
	virtual void drop(CDBConnectHelp *lpHelp);
	std::string getSelectSql();
	bool createTab(CDBConnectHelp *lpHelp);

	bool exportSqlDataVideoIni(std::string strFileExportPath);

	bool getPicPath(CDBConnectHelp *lpHelp, std::string strReceiveTime,std::string& strPicPathA,std::string& strPicPathB);
protected:
	virtual DBRow *readImpl( DBAPI::CRecordSetBase *lpRecordSet );
protected:
	std::string _projectName;
	std::string _strDBUserName;
};


#endif