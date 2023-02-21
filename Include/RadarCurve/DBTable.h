#pragma once
#include <osg/Referenced>
#include <osg/ref_ptr>
#include <RadarCurve/DBConnectBase.h>
#include <RadarCurve/RecordSetBase.h>
#include <RadarCurve/DBRow.h>
#include <vector>

class CDBTable
{
public:
	CDBTable(void);
	~CDBTable(void);

	typedef std::vector< osg::ref_ptr<DBRow>> RowVector;

	virtual bool open( CDBConnectHelp *lpHelp, std::string const& sql);
	RowVector CDBTable::read( int readCount );

	void CDBTable::read( RowVector &rv );
	bool close();
	void createRecordSet( CDBConnectHelp *lpHelp );



	virtual void insert( CDBConnectHelp *lpHelp, const DBRow *lpRow ) = 0;
	virtual void update( CDBConnectHelp *lpHelp, const DBRow *lpRow ) = 0;
	virtual void del( CDBConnectHelp *lpHelp, const DBRow *lpRow ) = 0;
	virtual void drop(CDBConnectHelp *lpHelp){};
	virtual std::string getSelectSql() = 0;


protected:
	virtual DBRow *readImpl( DBAPI::CRecordSetBase *lpRecordSet ) = 0;
	bool prepareStream(CDBConnectHelp *lpHelp, std::string const& sql, bool closeAutoCommit);
	bool clearStream();
protected:

	DBAPI::CRecordSetBase *_lpRecordSet;
};
