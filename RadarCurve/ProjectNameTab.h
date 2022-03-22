#pragma once
#include <RadarCurve/DBTable.h>
#include <RadarCurve/DBRow.h>


class ProjectNameTab : public CDBTable
{
public:
	ProjectNameTab(void);
	~ProjectNameTab(void);

	class ProjectNameRow : public DBRow
	{
	public:
		std::string _projectName;
		INT64 _createTime;
		float _startLen;
		float _curLen;
		std::string _paramXML;

	};
	virtual void insert( CDBConnectHelp *lpHelp, const DBRow *lpRow );
	virtual void update( CDBConnectHelp *lpHelp, const DBRow *lpRow );
	virtual void del( CDBConnectHelp *lpHelp, const DBRow *lpRow );
	virtual std::string getSelectSql();

	bool createTab(CDBConnectHelp *lpHelp);

	std::string getProjectName()const;

	//添加一行，如果时间相同就更新
	//bool AddOrUpdateALine( CDBConnectHelp *lpHelp, const DBRow *lpRow );

protected:
	virtual DBRow *readImpl( DBAPI::CRecordSetBase *lpRecordSet );
	std::string _projectName;
	std::string _strDBUserName;
};
