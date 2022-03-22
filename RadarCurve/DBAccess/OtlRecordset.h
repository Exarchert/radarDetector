#pragma once
#include <RadarCurve/RecordSetBase.h>
#include <string>

#include "RadarCurve/otldef.h"

#include <string>
#include <map>
namespace DBAPI
{

	struct ColInfo
	{
		long	_index;
		int		_type;
		ColInfo()
		{
			memset ( this, 0, sizeof ( ColInfo ) );
		}
	};

	typedef std::map< std::string, ColInfo >	ColCollection;
	typedef std::map < std::string/* col name */, long/* col index */ >		ColumnNameMap;


	typedef otl_stream_read_iterator<otl_stream,otl_exception,otl_lob_stream> rs_iterator;

	class CDBConnectBase;
	class COtlConnect;
	class COtlRecordset :
		public CRecordSetBase
	{
	public:
		COtlRecordset(CDBConnectBase *lpDBConnect );
		virtual ~COtlRecordset(void);
	public:
	public:
		virtual bool Open( const char *sql );
		virtual bool Close ();
	public:

	//	virtual bool GetFieldValue ( const long index, std::string &value  );
		virtual bool GetFieldValue ( const long index, time_t &value  );
/*		virtual bool GetFieldValue ( const long index, unsigned int &value  );*/
		virtual bool GetFieldValue ( const long index, int &value  );
		virtual bool GetFieldValue ( const long index, char *value, int *len  );
		virtual bool GetFieldValue ( const long index, unsigned char &value  );
		virtual bool GetFieldValue ( const long index, long &value  );
		virtual bool GetFieldValue ( const long index, unsigned long &value  );
		virtual bool GetFieldValue ( const long index, short &value  );
/*		virtual bool GetFieldValue ( const long index, unsigned short &value  );*/
		virtual bool GetFieldValue ( const long index, float &value  );
		virtual bool GetFieldValue ( const long index, double &value  );
		virtual bool GetFieldValue ( const long index, std::string &value  );

	//	virtual bool GetFieldValue ( const char *filedName, std::string &value  );
		virtual bool GetFieldValue ( const char *filedName, time_t &value  );
/*		virtual bool GetFieldValue ( const char *filedName, unsigned int &value  );*/
		virtual bool GetFieldValue ( const char *filedName, int &value  );
		virtual bool GetFieldValue ( const char *filedName, char *value, int *len  );
		virtual bool GetFieldValue ( const char *filedName, unsigned char &value  );
		virtual bool GetFieldValue ( const char *filedName, long &value  );
		virtual bool GetFieldValue ( const char *filedName, unsigned long &value  );
		virtual bool GetFieldValue ( const char *filedName, short &value  );
/*		virtual bool GetFieldValue ( const char *filedName, unsigned short &value  );*/
		virtual bool GetFieldValue ( const char *filedName, float &value  );
		virtual bool GetFieldValue ( const char *filedName, double &value  );
		virtual bool GetFieldValue ( const char *filedName, std::string &value  );

		virtual int	 GetFiledType( long index );
		virtual int  GetFiledType ( const char*filedName );

		virtual int	 GetFieldsCount(){ return m_nameMap.size(); };

		virtual std::string GetFieldName( long index );

		virtual bool OpenStream( const std::string &sql, bool closeAutoCommit );

		virtual bool CloseStream();

		virtual bool StreamIn( time_t &value );
		virtual bool StreamIn( int &value );
		virtual bool StreamIn( char &value );
		virtual bool StreamIn( char *value, int len );
		virtual bool StreamIn( unsigned char &value );
		virtual bool StreamIn( long &value );
		virtual bool StreamIn( unsigned long &value );
		virtual bool StreamIn( short &value );
		virtual bool StreamIn( float &value );
		virtual bool StreamIn( double &value );
		virtual bool StreamIn( std::string &value );
	public:
		virtual bool MoveNext ();
		virtual bool MoveFirst();
		virtual bool IsEOF ();
	private:
		otl_stream m_rs;
		otl_stream m_saveStream;

		rs_iterator	m_iterator;
		COtlConnect		*m_lpDBConnect;
		ColumnNameMap m_nameMap;
		bool		m_end;
	protected:
		std::string GetUpperStr( const char *filedName );
		static void PrintErr ( otl_exception &p );
	public:
		static std::string GetValueStr( const char *value, const int type );
	};

}
