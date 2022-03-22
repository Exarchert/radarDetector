#pragma once
#include <radarcurve\dbtable.h>

class DataCountTab : public CDBTable
{
public:
	DataCountTab(void);
	~DataCountTab(void);

	void setTabName( std::string const& tabName );
	int getTabDataCount(CDBConnectHelp *lpHelp, std::string const& selectSql );
	virtual void insert( CDBConnectHelp *lpHelp, const DBRow *lpRow ){};
	virtual void update( CDBConnectHelp *lpHelp, const DBRow *lpRow ){};
	virtual void del( CDBConnectHelp *lpHelp, const DBRow *lpRow ){};
	virtual std::string getSelectSql();


	class CountRow : public DBRow
	{
	public:
		int _count;
	};

protected:
	virtual DBRow *readImpl( DBAPI::CRecordSetBase *lpRecordSet );

	std::string _projectName;
	std::string _strDBUserName;

};

class TabMaxIDTab : public CDBTable
{
public:
	TabMaxIDTab(void);
	~TabMaxIDTab(void);

	void setTabName( std::string const& tabName );
	long getMaxID(CDBConnectHelp *lpHelp, std::string const& selectSql );
	virtual void insert( CDBConnectHelp *lpHelp, const DBRow *lpRow ){};
	virtual void update( CDBConnectHelp *lpHelp, const DBRow *lpRow ){};
	virtual void del( CDBConnectHelp *lpHelp, const DBRow *lpRow ){};
	virtual std::string getSelectSql();


	class MaxIDRow : public DBRow
	{
	public:
		long _maxID;
	};

protected:
	virtual DBRow *readImpl( DBAPI::CRecordSetBase *lpRecordSet );

	std::string _projectName;
	std::string _strDBUserName;

	
};
