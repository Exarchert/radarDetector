#include "StdAfx.h"
#include "DataCountTab.h"
#include "RadarCurve\ConfigureSet.h"
#include "RadarCurve\RadarManager.h"

DataCountTab::DataCountTab(void)
{
	ConfigureSet* pConfigureSet = RadarManager::Instance()->getConfigureSet();
	std::string strDBUserName = pConfigureSet->get("db", "user").c_str();
	_strDBUserName = strDBUserName + ".";
}

DataCountTab::~DataCountTab(void)
{
}

void DataCountTab::setTabName( std::string const& tabName )
{
	_projectName = tabName;
}
std::string DataCountTab::getSelectSql()
{
	return "select count(*) from " + _strDBUserName + _projectName;
}

int DataCountTab::getTabDataCount(CDBConnectHelp *lpHelp, std::string const& selectSql)
{
	if ( !open( lpHelp, selectSql ) )
	{
		return 0;
	}

	RowVector rv = read( 1 );
	if ( rv.size() == 0 )
	{
		close();
		return 0;
	}

	close();

	CountRow *lpCR = dynamic_cast<CountRow*>(rv[0].get());
	if ( !lpCR )
	{
		return 0;
	}

	return lpCR->_count;
}

DBRow *DataCountTab::readImpl( DBAPI::CRecordSetBase *lpRecordSet )
{
	CountRow *lpRD = new CountRow;
	int value = 0;
	
	lpRecordSet->GetFieldValue( long(0), value);
	lpRD->_count = value;

	return lpRD;
}


//////////////////////////////////////////////////////////////////////////
TabMaxIDTab::TabMaxIDTab(void)
{
	ConfigureSet* pConfigureSet = RadarManager::Instance()->getConfigureSet();
	std::string strDBUserName = pConfigureSet->get("db", "user").c_str();
	_strDBUserName = strDBUserName + ".";
}

TabMaxIDTab::~TabMaxIDTab(void)
{
}

void TabMaxIDTab::setTabName( std::string const& tabName )
{
	_projectName = tabName;
}
std::string TabMaxIDTab::getSelectSql()
{
	
	return "select max(id) from " + _strDBUserName + _projectName;
}

long TabMaxIDTab::getMaxID(CDBConnectHelp *lpHelp, std::string const& selectSql)
{
	if ( !open( lpHelp, selectSql ) )
	{
		return 0;
	}

	RowVector rv = read( 1 );
	if ( rv.size() == 0 )
	{
		close();
		return 0;
	}

	close();

	MaxIDRow *lpCR = dynamic_cast<MaxIDRow*>(rv[0].get());
	if ( !lpCR )
	{
		return 0;
	}

	return lpCR->_maxID;
}

DBRow *TabMaxIDTab::readImpl( DBAPI::CRecordSetBase *lpRecordSet )
{
	MaxIDRow *lpRD = new MaxIDRow;
	long value = 0;

	lpRecordSet->GetFieldValue( long(0), value);
	lpRD->_maxID = value;

	return lpRD;
}