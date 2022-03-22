#include "StdAfx.h"
#include "OtlConnect.h"
#include <iostream>
#include <string>

#ifndef USE_ODBC
	#define USE_ORCALE 1
#endif

//#define USE_MYSQL

namespace DBAPI
{
	bool COtlConnect::m_init = false;
	COtlConnect::COtlConnect(void)
	{
		if ( !m_init )
		{
			otl_conn::initialize();
		}
	}

	COtlConnect::~COtlConnect(void)
	{
		Close();
	}


	bool COtlConnect::Open(const char *dbsrc, const char *dbname, const char *user, const char *pass)
	{
		_ASSERT ( dbsrc );
		_ASSERT ( dbname );
		_ASSERT ( user );
		_ASSERT ( pass );
#ifdef USE_ORCALE
#ifdef OTL_ODBC
		std::string conn = std::string ( user ) + "/" + pass + "@" + dbsrc;
#else
		std::string conn = std::string ( user ) + "/" + pass + "@" + dbsrc + "/" + dbname;
#endif

#else
		std::string conn = std::string( ( "DSN=" ) ) + dbsrc +
//			 ( "; Initial Catalog=" ) + dbname +
			 ( "; UID=" ) + user +
			 ( "; PWD=" ) + pass;
#endif

		try
		{
			m_db.rlogon( conn.c_str() ); // connect to ODBC
#ifdef USE_MYSQL
			std::string changedb = std::string ( "USE " ) + " " + dbname + ";";
			m_db.direct_exec( changedb.c_str() );
#endif
		}
		catch(otl_exception& p)
		{
			PrintErr( p );
			return false;
		}
#ifdef USE_MYSQL
		try
		{
			std::string changecodepage = "set names gb2312";
			m_db.direct_exec( changecodepage.c_str() );
		}
		catch (otl_exception &e)
		{
			PrintErr( e );
		}
#endif

		return true;
	}
	bool COtlConnect::Close()
	{
		try 
		{
			m_db.logoff();
		}
		catch ( otl_exception &p )
		{
			PrintErr( p );
			return false;
		}
		return true;
	}
	bool COtlConnect::GetStatus()
	{
		otl_stream i;
		try 
		{
			i.open( 1, "select 1", m_db );
			i.close();
			
		}
		catch ( otl_exception &p )
		{
			PrintErr( p );
			return false;
		}
		return true;
	}
	bool COtlConnect::ExecSql( const char *sql )
	{
		try
		{
			m_db.direct_exec( sql );
			m_db.commit();
		}
		catch (otl_exception &p )
		{
			PrintErr( p );
			return false;
		}
		return true;
	}

	void COtlConnect::PrintErr(otl_exception &p)
	{
		// intercept OTL exceptions  
		TRACE ( "%s\n", p.msg );
		TRACE ( "%s\n", p.stm_text );
		TRACE ( "%s\n", p.sqlstate );
		TRACE ( "%s\n", p.var_info );
	//	std::cerr<<p.msg<<std::endl; // print out error message  
	//	std::cerr<<p.stm_text<<std::endl; // print out SQL that caused the error  
	//	std::cerr<<p.sqlstate<<std::endl; // print out SQLSTATE message  
	//	std::cerr<<p.var_info<<std::endl; // print out the variable that caused the error
	}

	bool COtlConnect::Commit()
	{
		try
		{
			m_db.commit();
		}
		catch (otl_exception &p )
		{
			PrintErr( p );
			return false;
		}
		return true;
	}
}
