#include "StdAfx.h"
#include <RadarCurve/DBTable.h>

#include "DBAccess/OtlRecordset.h"

CDBTable::CDBTable(void)
{
	_lpRecordSet = NULL;
}

CDBTable::~CDBTable(void)
{
	close();
}
void CDBTable::createRecordSet( CDBConnectHelp *lpHelp )
{
	_lpRecordSet = new DBAPI::COtlRecordset( lpHelp->GetConnect() );
}
bool CDBTable::open( CDBConnectHelp *lpHelp, std::string const& sql)
{
	if ( !_lpRecordSet )
	{
		createRecordSet( lpHelp );
	}

	_lpRecordSet->Close();

	if ( _lpRecordSet->Open( sql.c_str() ) )
	{
		return true;
	}

	return false;
}

void CDBTable::read( RowVector &rv )
{
	if ( !_lpRecordSet )
	{
		return ;
	}

	while( !_lpRecordSet->IsEOF() )
	{
		rv.push_back( readImpl( _lpRecordSet ) );
		_lpRecordSet->MoveNext();
	}
}

CDBTable::RowVector CDBTable::read( int readCount )
{
	RowVector rv;

	if ( !_lpRecordSet )
	{
		return rv;
	}

	for ( int i = 0; i < readCount; i ++ )
	{
		if ( !_lpRecordSet->IsEOF() )
		{
			rv.push_back( readImpl( _lpRecordSet ) );
			_lpRecordSet->MoveNext();
		}else
		{
			break;
		}
	}


	return rv;

}

bool CDBTable::close()
{
	if ( _lpRecordSet )
	{
		_lpRecordSet->Close();
		_lpRecordSet->CloseStream();
		delete _lpRecordSet;

		_lpRecordSet = NULL;
	}

	return true;
}

bool CDBTable::prepareStream(CDBConnectHelp *lpHelp, std::string const& sql, bool closeAutoCommit )
{
	if ( !_lpRecordSet)
	{
		createRecordSet( lpHelp );
	}

	if ( !_lpRecordSet->OpenStream( sql, closeAutoCommit ) )
	{
		lpHelp->Reconnect();
		if ( !_lpRecordSet->OpenStream( sql, closeAutoCommit ) )
		{
			return false;
		}

	}

	return true;
}

bool CDBTable::clearStream()
{
	if ( _lpRecordSet )
	{
		_lpRecordSet->CloseStream();
	}

	return true;
}