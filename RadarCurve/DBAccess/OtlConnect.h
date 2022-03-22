#pragma once
#include <RadarCurve/DBConnectBase.h>

#include "RadarCurve/otldef.h"

namespace DBAPI
{


	class COtlRecordset;

	class COtlConnect :
		public CDBConnectBase
	{
	public:
		COtlConnect(void);
		virtual ~COtlConnect(void);
	public:
		virtual bool Open( const char *dbsrc, const char *dbname, const char *user, const char *pass);
		virtual bool Close();
		virtual bool GetStatus();
		virtual bool ExecSql( const char *sql );
		virtual bool Commit();
	friend class COtlRecordset;
	protected:
		otl_connect &GetConnect(){ return m_db; };
	public:
		static void PrintErr ( otl_exception &p );
	private:
		otl_connect m_db;
		static bool			m_init;
	};

}
