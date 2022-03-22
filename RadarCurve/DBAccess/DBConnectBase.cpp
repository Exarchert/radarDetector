#include "StdAfx.h"
#include <RadarCurve/DBConnectBase.h>
#include "OtlConnect.h"
#include "OtlRecordset.h"



// DB_CONNECT_TYPE		G_USE_TYPE = USE_OTL;
// void SetDBType( DB_CONNECT_TYPE type )
// {
// 	G_USE_TYPE = type;
// }

namespace DBAPI
{
	CDBConnectBase::CDBConnectBase(void)
	{

	}

	CDBConnectBase::~CDBConnectBase(void)
	{
	}

}
//////////////////////////////////////////////////////////////////////////
CDBConnectHelp::CDBConnectHelp()
:m_lpConnectBase ( NULL )
{


}
void CDBConnectHelp::Init()
{

	m_lpConnectBase = new DBAPI::COtlConnect;

}

bool CDBConnectHelp::Open( const char *dbsrc, const char *dbname, const char *user, const char *pass)
{
	if ( m_lpConnectBase )
	{
		delete m_lpConnectBase;
		
	}
	Init();
	m_dbSrc = dbsrc;
	m_dbName = dbname;
	m_dbUser = user;
	m_dbPass = pass;
	return m_lpConnectBase->Open( dbsrc, dbname, user, pass );
}
//	switch ( G_USE_TYPE )
//	{
//	case USE_OTL:
		/*m_lpConnectBase = new DBAPI::COtlConnect;*/
//		break;
//	case USE_ADO:
		//m_lpConnectBase = new DBAPI::CAdoConnectionEx;
//		break;
//	default:
//		m_lpConnectBase = NULL;
//	}
CRecordsetHelp::CRecordsetHelp ( CDBConnectHelp *dbhelp )
{
	//switch ( G_USE_TYPE )
	{
	//case USE_OTL:
		m_lpRecordset = new DBAPI::COtlRecordset(dbhelp->GetConnect() );
	//	break;
	//case USE_ADO:
	//	m_lpRecordset = new DBAPI::CAdoRecordSetEx ( dbhelp->GetConnect() );
	//	break;
	//default:
	//	m_lpRecordset = NULL;
	}
}


std::string CRecordsetHelp::GetValueStr( const char *value, const int type )
{
	//switch ( G_USE_TYPE )
	{
	//case USE_OTL:
		return DBAPI::COtlRecordset::GetValueStr( value, type );
// 		break;
// 	case USE_ADO:
// 		return DBAPI::CAdoRecordSetEx::GetValueStr( value, type );
// 		break;
// 	default:
// 		return "";
	}
}