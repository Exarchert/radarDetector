////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

// Copyright (C), 中盈高科. 研发

// 文件：DBTable.h

// 作者：耿蓓蕾

// 版本：1.0

// 日期：2012-3-17

// 描述：数据库连接帮助

// 修改记录：

// 作者						日期					描述
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

#pragma once
#include "RecordSetBase.h"
namespace DBAPI
{
	class CDBConnectBase
	{
	public:
		CDBConnectBase(void);
		virtual ~CDBConnectBase(void);
	public:
		virtual bool Open( const char *dbsrc, const char *dbname, const char *user, const char *pass) = 0;
		virtual bool Close() = 0;
		virtual bool GetStatus() = 0;
		virtual bool ExecSql( const char *sql ) = 0;
		virtual bool Commit() = 0;



	};
}
// enum DB_CONNECT_TYPE
// {
// 	USE_ADO = 1,		// 使用ADObe连接数据库
// 	USE_OTL = 2,			// 使用OTL连接数据库
// };

//void SetDBType ( DB_CONNECT_TYPE type );

class CDBConnectHelp
{

public:
	CDBConnectHelp( );
	~CDBConnectHelp()
	{
		if (NULL != m_lpConnectBase )
		{
			m_lpConnectBase->Close();
			delete m_lpConnectBase;
			m_lpConnectBase = NULL;
		}
	}

	void Init ();

public:
	DBAPI::CDBConnectBase *GetConnect ()
	{
		if ( !m_lpConnectBase )
		{
			Init();
		}
		return m_lpConnectBase;
	};

	bool Open( const char *dbsrc, const char *dbname, const char *user, const char *pass);
	bool Close()
	{
		if ( m_lpConnectBase )
			return m_lpConnectBase->Close();
		return true;
	};
	bool GetStatus()
	{
		if ( m_lpConnectBase)
			return m_lpConnectBase->GetStatus();
		return false;
	};
	bool ExecSql( const char *sql )
	{
		if ( !m_lpConnectBase )
		{
			Init();
		}
		return m_lpConnectBase->ExecSql( sql );
	};

	bool Commit()
	{
		if ( !m_lpConnectBase )
		{
			return m_lpConnectBase->Commit();
		}

		return false;
	}

	 bool Reconnect()
	 {
		 if ( !m_lpConnectBase )
		 {
			 return false;
		 }
		 Close();
		 if ( Open( m_dbSrc.c_str(), m_dbName.c_str(), m_dbUser.c_str(), m_dbPass.c_str() ) )
		 {
			 return true;
		 }

		 return false;
	 }

private:
	DBAPI::CDBConnectBase *m_lpConnectBase;

protected:
	std::string  m_dbSrc;
	std::string m_dbName;
	std::string m_dbUser;
	std::string m_dbPass;
};

class CRecordsetHelp
{
public:
	CRecordsetHelp( CDBConnectHelp *dbhelp );
	~CRecordsetHelp ()
	{
		Close();
	}
public:
	DBAPI::CRecordSetBase *GetRecordSet () { _ASSERT ( m_lpRecordset ); return m_lpRecordset; };

public:
//	bool GetFiledValue ( long index, std::string &value  ){ _ASSERT ( m_lpRecordset ); return m_lpRecordset->GetFiledValue (index, value ); };
	bool GetFiledValue ( long index, time_t &value  ){ _ASSERT ( m_lpRecordset ); return m_lpRecordset->GetFieldValue (index, value ); };
/*	bool GetFiledValue ( long index, unsigned int &value  ){ _ASSERT ( m_lpRecordset ); return m_lpRecordset->GetFiledValue (index, value ); };*/
	bool GetFiledValue ( long index, int &value  ){ _ASSERT ( m_lpRecordset ); return m_lpRecordset->GetFieldValue (index, value ); };
	bool GetFiledValue ( long index, char *value, int *len  ){ _ASSERT ( m_lpRecordset ); return m_lpRecordset->GetFieldValue (index, value, len ); };
	bool GetFiledValue ( long index, unsigned char &value  ){ _ASSERT ( m_lpRecordset ); return m_lpRecordset->GetFieldValue (index, value ); };
	bool GetFiledValue ( long index, long &value  ){ _ASSERT ( m_lpRecordset ); return m_lpRecordset->GetFieldValue (index, value ); };
	bool GetFiledValue ( long index, unsigned long &value  ){ _ASSERT ( m_lpRecordset ); return m_lpRecordset->GetFieldValue (index, value ); };
	bool GetFiledValue ( long index, short &value  ){ _ASSERT ( m_lpRecordset ); return m_lpRecordset->GetFieldValue (index, value ); };
/*	bool GetFiledValue ( long index, unsigned short &value  ){ _ASSERT ( m_lpRecordset ); return m_lpRecordset->GetFiledValue (index, value ); };*/
	bool GetFiledValue ( long index, float &value  ){ _ASSERT ( m_lpRecordset ); return m_lpRecordset->GetFieldValue (index, value ); };
	bool GetFiledValue ( long index, double &value  ){	_ASSERT ( m_lpRecordset );return m_lpRecordset->GetFieldValue (index, value );};
	bool GetFiledValue ( long index, std::string &value  ) { _ASSERT ( m_lpRecordset );	return m_lpRecordset->GetFieldValue (index, value );};

//	bool GetFiledValue ( const char *filedName, std::string &value  ){ _ASSERT ( m_lpRecordset ); return m_lpRecordset->GetFiledValue (filedName, value ); };
	bool GetFiledValue ( const char *filedName, time_t &value  ){ _ASSERT ( m_lpRecordset ); return m_lpRecordset->GetFieldValue (filedName, value ); };
	/*bool GetFiledValue ( const char *filedName, unsigned int &value  ){ _ASSERT ( m_lpRecordset ); return m_lpRecordset->GetFiledValue (filedName, value ); };*/
	bool GetFiledValue ( const char *filedName, int &value  ){ _ASSERT ( m_lpRecordset ); return m_lpRecordset->GetFieldValue (filedName, value ); };
	bool GetFiledValue ( const char *filedName, char *value, int *len  ){ _ASSERT ( m_lpRecordset ); return m_lpRecordset->GetFieldValue (filedName, value, len ); };
	bool GetFiledValue ( const char *filedName, unsigned char &value  ){ _ASSERT ( m_lpRecordset ); return m_lpRecordset->GetFieldValue (filedName, value ); };
	bool GetFiledValue ( const char *filedName, long &value  ){ _ASSERT ( m_lpRecordset ); return m_lpRecordset->GetFieldValue (filedName, value ); };
	bool GetFiledValue ( const char *filedName, unsigned long &value  ){ _ASSERT ( m_lpRecordset ); return m_lpRecordset->GetFieldValue (filedName, value ); };
	bool GetFiledValue ( const char *filedName, short &value  ){ _ASSERT ( m_lpRecordset ); return m_lpRecordset->GetFieldValue (filedName, value ); };
/*	bool GetFiledValue ( const char *filedName, unsigned short &value  ){ _ASSERT ( m_lpRecordset ); return m_lpRecordset->GetFiledValue (filedName, value ); };*/
	bool GetFiledValue ( const char *filedName, float &value  ){ _ASSERT ( m_lpRecordset ); return m_lpRecordset->GetFieldValue (filedName, value ); };
	bool GetFiledValue ( const char *filedName, double &value  ){ _ASSERT ( m_lpRecordset ); return m_lpRecordset->GetFieldValue (filedName, value ); };
	bool GetFiledValue ( const char *filedName, std::string &value  ){ _ASSERT ( m_lpRecordset ); return m_lpRecordset->GetFieldValue (filedName, value );};


	int GetFiledType( long index ){ _ASSERT ( m_lpRecordset ); return m_lpRecordset->GetFiledType( index );};
	int GetFiledType ( const char *filedName ){ _ASSERT ( m_lpRecordset); return m_lpRecordset->GetFiledType( filedName ); };

	int	 GetFieldsCount(){_ASSERT ( m_lpRecordset ); return m_lpRecordset->GetFieldsCount(); };


	std::string GetFieldName( long index ){ _ASSERT ( m_lpRecordset); return m_lpRecordset->GetFieldName ( index ); };


	// 以Stream方式保存数据
	virtual bool OpenStream( const std::string &sql, bool closeAutoCommit ){ _ASSERT  ( m_lpRecordset ); return m_lpRecordset->OpenStream( sql, closeAutoCommit ); };

	virtual bool CloseStream(){ _ASSERT ( m_lpRecordset ); return m_lpRecordset->CloseStream(); };

	virtual bool StreamIn( time_t &value ) { _ASSERT ( m_lpRecordset ); return m_lpRecordset->StreamIn( value ); };
	virtual bool StreamIn( int &value ) { _ASSERT ( m_lpRecordset ); return m_lpRecordset->StreamIn( value ); };
	virtual bool StreamIn( char &value ) { _ASSERT ( m_lpRecordset ); return m_lpRecordset->StreamIn( value ); };
	virtual bool StreamIn( char *value, int len ) { _ASSERT ( m_lpRecordset ); return m_lpRecordset->StreamIn( value, len ); };
	virtual bool StreamIn( unsigned char &value ) { _ASSERT ( m_lpRecordset ); return m_lpRecordset->StreamIn( value ); };
	virtual bool StreamIn( long &value ) { _ASSERT ( m_lpRecordset ); return m_lpRecordset->StreamIn( value ); };
	virtual bool StreamIn( unsigned long &value ) { _ASSERT ( m_lpRecordset ); return m_lpRecordset->StreamIn( value ); };
	virtual bool StreamIn( short &value ) { _ASSERT ( m_lpRecordset ); return m_lpRecordset->StreamIn( value ); };
	virtual bool StreamIn( float &value ) { _ASSERT ( m_lpRecordset ); return m_lpRecordset->StreamIn( value ); };
	virtual bool StreamIn( double &value ) { _ASSERT ( m_lpRecordset ); return m_lpRecordset->StreamIn( value ); };
	virtual bool StreamIn( std::string &value ) { _ASSERT ( m_lpRecordset ); return m_lpRecordset->StreamIn( value ); };


public:
	void SetAllToStr ( bool allToStr ){ _ASSERT ( m_lpRecordset ); m_lpRecordset->SetAllToStr( allToStr ); };
public:
	bool Open( const char *sql ){ _ASSERT ( m_lpRecordset );  return m_lpRecordset->Open( sql ); };
	bool Close ()
	{
		if ( !m_lpRecordset )
			return true;
		bool retvalue = m_lpRecordset->Close();
		if ( retvalue )
		{
			delete m_lpRecordset;
			m_lpRecordset = NULL;
		}
		return retvalue;
	};

	bool MoveNext () { _ASSERT ( m_lpRecordset ); return m_lpRecordset->MoveNext(); };
	bool MoveFirst(){ _ASSERT ( m_lpRecordset ); return m_lpRecordset->MoveFirst(); };
	bool IsEOF (){ _ASSERT ( m_lpRecordset ); return m_lpRecordset->IsEOF(); };

	static std::string GetValueStr( const char *value, const int type );
private:
	DBAPI::CRecordSetBase *m_lpRecordset;
};

