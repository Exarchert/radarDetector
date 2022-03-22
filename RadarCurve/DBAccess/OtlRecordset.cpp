#include "StdAfx.h"
#include <algorithm>
#include <time.h>
#include "OtlRecordset.h"
#include "OtlConnect.h"

namespace DBAPI
{
	COtlRecordset::COtlRecordset( CDBConnectBase *lpDBConnect )
		: m_lpDBConnect ( ( COtlConnect * )(lpDBConnect ) )
	{

	}

	COtlRecordset::~COtlRecordset(void)
	{
		Close();
	}

	bool COtlRecordset::Open( const char *sql )
	{		
		m_lpDBConnect->GetConnect().set_max_long_size(65535);
		m_nameMap.erase( m_nameMap.begin(), m_nameMap.end() );
		try
		{
			if ( m_bAllToStr )
				m_rs.set_all_column_types( otl_all_num2str | otl_all_date2str );
			m_rs.open( 30, sql, m_lpDBConnect->GetConnect()/*,otl_implicit_select*/  );
			int column;
			otl_column_desc *desc = m_rs.describe_select ( column );
			for ( int i = 0; i < column; i ++ )
			{
				std::string name = desc[i].name;
				transform  ( name.begin(), name.end(), name.begin(), toupper );
				m_nameMap[name] = i;
			}
			m_iterator.attach( m_rs );
			m_end = m_iterator.next_row();
			return true;
		}
		catch (otl_exception &p)
		{
			COtlConnect::PrintErr( p );
			return false;
		}
		return true;
	}
	bool COtlRecordset::Close ()
	{
		try
		{
			m_iterator.detach();
			m_rs.close();
		}
		catch (otl_exception &p)
		{
			COtlConnect::PrintErr( p );
			return false;
		}
		return true;
	}
	// 所有ID从0开始
/*	bool COtlRecordset::GetFieldValue ( const long index, std::string &value  )
	{
		m_iterator.get ( int ( index + 1 ), value );
	}
*/

	bool COtlRecordset::GetFieldValue ( const long index, time_t &value )
	{
		try
		{
			otl_datetime t;
			m_iterator.get ( int ( index + 1 ), t );
			if ( t.year < 2000 )
			{
				value = 0;
				return false;
			}
			struct tm atm;
			atm.tm_sec = t.second;
			atm.tm_min = t.minute;
			atm.tm_hour = t.hour;
			atm.tm_mday = t.day;
			atm.tm_mon = t.month - 1;        // tm_mon is 0 based
			atm.tm_year = t.year - 1900;     // tm_year is 1900 based
			atm.tm_isdst = t.fraction;

			value = _mktime64(&atm);
		}
		catch (otl_exception &e)
		{
			COtlConnect::PrintErr ( e );
			return false;
		}
		return true;
	}
// 	bool COtlRecordset::GetFieldValue ( const long index, unsigned int &value  )
// 	{
// 		try
// 		{
// 			m_iterator.get ( int ( index + 1 ), value );
// 		}
// 		catch (otl_exception &e)
// 		{
// 			value = 0;
// 			COtlConnect::PrintErr ( e );
// 			return false;
// 		}
// 		return true;
// 	}
	bool COtlRecordset::GetFieldValue ( const long index, int &value  )
	{
		try
		{
			m_iterator.get ( int ( index + 1 ), value );
		}
		catch (otl_exception &e)
		{
			value = 0;
			COtlConnect::PrintErr ( e );
			return false;
		}
		return true;
	}
	bool COtlRecordset::GetFieldValue ( const long index, char *value, int *len  )
	{
		try
		{
			otl_long_string ls;
			m_iterator.get ( int ( index + 1 ), ls );

			if ( value && ls.len() > 0 )
			{
				memcpy ( value, ls.v, ls.len() );
			}
			if ( len )
			{
				*len = 0;
			}
			if ( len && ls.len() > 0 )
			{
				*len = ls.len();
			}

		}
		catch (otl_exception &e)
		{
			COtlConnect::PrintErr ( e );
			return false;
		}
		return true;
	}
	bool COtlRecordset::GetFieldValue ( const long index, unsigned char &value  )
	{
		try
		{
			m_iterator.get ( int ( index + 1 ), value );
		}
		catch (otl_exception &e)
		{
			value = 0;
			COtlConnect::PrintErr ( e );
			return false;
		}
		return true;
	}
	bool COtlRecordset::GetFieldValue ( const long index, long &value  )
	{
		try
		{
			m_iterator.get ( int ( index + 1 ), value );
		}
		catch (otl_exception &e)
		{
			value = 0;
			COtlConnect::PrintErr ( e );
			return false;
		}
		return true;
	}
	bool COtlRecordset::GetFieldValue ( const long index, unsigned long &value  )
	{
		try
		{
			long tmp = 0;
			m_iterator.get ( int ( index + 1 ), tmp );
			value = tmp;
		}
		catch (otl_exception &e)
		{
			value = 0;
			COtlConnect::PrintErr ( e );
			return false;
		}
		return true;
	}
	bool COtlRecordset::GetFieldValue ( const long index, short &value  )
	{
		try
		{
			m_iterator.get ( int ( index + 1 ), value );
		}
		catch (otl_exception &e)
		{
			value = 0;
			COtlConnect::PrintErr ( e );
			return false;
		}
		return true;
	}
// 	bool COtlRecordset::GetFieldValue ( const long index, unsigned short &value  )
// 	{
// 		try
// 		{
// 			m_iterator.get ( int ( index + 1 ), value );
// 		}
// 		catch (otl_exception &e)
// 		{
// 			value = 0;
// 			COtlConnect::PrintErr ( e );
// 			return false;
// 		}
// 		return true;
// 	}
	bool COtlRecordset::GetFieldValue ( const long index, float &value  )
	{
		try
		{
			m_iterator.get ( int ( index + 1 ), value );
		}
		catch (otl_exception &e)
		{
			value = 0;
			COtlConnect::PrintErr ( e );
			return false;
		}
		return true;
	}
	bool COtlRecordset::GetFieldValue ( const long index, double &value  )
	{
		try
		{
			m_iterator.get ( int ( index + 1 ), value );
		}
		catch (otl_exception &e)
		{
			value = 0;
			COtlConnect::PrintErr ( e );
			return false;
		}
		return true;
	}
	bool COtlRecordset::GetFieldValue ( const long index, std::string &value  )
	{
		try
		{
			int len = 0;

			if ( !GetFieldValue ( index, NULL, &len ) )
			{
				char buff[2048] = {0};
				m_iterator.get ( int ( index + 1 ), buff );
				value = buff;
				return true;
			}

			char *buf = new char[ len + 1];
			memset( buf, 0, len + 1 );

			GetFieldValue( index, buf, NULL );

			value = buf;
			delete[] buf;

		}
		catch (otl_exception &e)
		{
			value = "";
			COtlConnect::PrintErr ( e );
			return false;
		}
		return true;
	}

//////////////////////////////////////////////////////////////////////////
/*	bool COtlRecordset::GetFieldValue ( const char *filedName, std::string &value  )
	{
		ColumnNameMap::iterator pos = m_nameMap.find( GetUpperStr( filedName) );
		if ( pos != m_nameMap.end() )
		{
			return GetFieldValue( (*pos).second, value );
		}
		return false;
	}
	*/
	bool COtlRecordset::GetFieldValue ( const char *filedName, time_t &value  )
	{
		ColumnNameMap::iterator pos = m_nameMap.find( GetUpperStr( filedName) );
		if ( pos != m_nameMap.end() )
		{
			return GetFieldValue( (*pos).second, value );
		}
		return false;
	}
// 	bool COtlRecordset::GetFieldValue ( const char *filedName, unsigned int &value  )
// 	{
// 		ColumnNameMap::iterator pos = m_nameMap.find( GetUpperStr( filedName) );
// 		if ( pos != m_nameMap.end() )
// 		{
// 			return GetFieldValue( (*pos).second, value );
// 		}
// 		return false;
// 	}
	bool COtlRecordset::GetFieldValue ( const char *filedName, int &value  )
	{
		ColumnNameMap::iterator pos = m_nameMap.find( GetUpperStr( filedName) );
		if ( pos != m_nameMap.end() )
		{
			return GetFieldValue( (*pos).second, value );
		}
		return false;
	}
	bool COtlRecordset::GetFieldValue ( const char *filedName, char *value, int *len  )
	{
		ColumnNameMap::iterator pos = m_nameMap.find( GetUpperStr( filedName) );
		if ( pos != m_nameMap.end() )
		{
			return GetFieldValue( (*pos).second, value, len );
		}
		return false;
	}
	bool COtlRecordset::GetFieldValue ( const char *filedName, unsigned char &value  )
	{
		ColumnNameMap::iterator pos = m_nameMap.find( GetUpperStr( filedName) );
		if ( pos != m_nameMap.end() )
		{
			return GetFieldValue( (*pos).second, value );
		}
		return false;
	}
	bool COtlRecordset::GetFieldValue ( const char *filedName, long &value  )
	{
		ColumnNameMap::iterator pos = m_nameMap.find( GetUpperStr( filedName) );
		if ( pos != m_nameMap.end() )
		{
			return GetFieldValue( (*pos).second, value );
		}
		return false;
	}
	bool COtlRecordset::GetFieldValue ( const char *filedName, unsigned long &value  )
	{
		ColumnNameMap::iterator pos = m_nameMap.find( GetUpperStr( filedName) );
		if ( pos != m_nameMap.end() )
		{
			return GetFieldValue( (*pos).second, value );
		}
		return false;
	}
	bool COtlRecordset::GetFieldValue ( const char *filedName, short &value  )
	{
		ColumnNameMap::iterator pos = m_nameMap.find( GetUpperStr( filedName) );
		if ( pos != m_nameMap.end() )
		{
			return GetFieldValue( (*pos).second, value );
		}
		return false;
	}
// 	bool COtlRecordset::GetFieldValue ( const char *filedName, unsigned short &value  )
// 	{
// 		ColumnNameMap::iterator pos = m_nameMap.find( GetUpperStr( filedName) );
// 		if ( pos != m_nameMap.end() )
// 		{
// 			return GetFieldValue( (*pos).second, value );
// 		}
// 		return false;
// 	}
	bool COtlRecordset::GetFieldValue ( const char *filedName, float &value  )
	{
		ColumnNameMap::iterator pos = m_nameMap.find( GetUpperStr( filedName) );
		if ( pos != m_nameMap.end() )
		{
			return GetFieldValue( (*pos).second, value );
		}
		return false;
	}
	bool COtlRecordset::GetFieldValue ( const char *filedName, double &value  )
	{
		ColumnNameMap::iterator pos = m_nameMap.find( GetUpperStr( filedName) );
		if ( pos != m_nameMap.end() )
		{
			return GetFieldValue( (*pos).second, value );
		}
		return false;
	}
	bool COtlRecordset::GetFieldValue ( const char *filedName, std::string &value  )
	{
		ColumnNameMap::iterator pos = m_nameMap.find( GetUpperStr( filedName) );
		if ( pos != m_nameMap.end() )
		{
			return GetFieldValue( (*pos).second, value );
		}
		return false;
	}

	int COtlRecordset::GetFiledType( long index )
	{
		int column = 0;
		otl_column_desc *desc = m_rs.describe_select ( column );
		if ( index < column )
		{
			return desc[index].dbtype;
		}
		return 0;
	}
	int COtlRecordset::GetFiledType( const char*filedName )
	{
		ColumnNameMap::iterator pos = m_nameMap.find( GetUpperStr( filedName) );
		if ( pos != m_nameMap.end() )
		{
			return GetFiledType( (*pos).second );
		}
		return 0;
	}

	std::string COtlRecordset::GetFieldName( long index )
	{
		_ASSERT ( index >= 0 );
		int column = 0;
		otl_column_desc *desc = m_rs.describe_select ( column );
		if ( index < column )
		{
			return std::string ( desc[index].name );
		}
		return "";
	}

	//////////////////////////////////////////////////////////////////////////
	bool COtlRecordset::MoveNext ()
	{
		try
		{
			m_end = m_iterator.next_row();
			return m_end;
			
		}
		catch ( otl_exception &p )
		{
			COtlConnect::PrintErr( p );
			return false;
		}
		return true;
	}
	bool COtlRecordset::MoveFirst()
	{
		try
		{
			m_iterator.detach();
			m_rs.rewind();
			m_iterator.attach( m_rs );
			m_end = m_iterator.next_row();
			return m_end;
		}
		catch ( otl_exception &p )
		{
			COtlConnect::PrintErr( p );
			return false;
		}
		return true;
	}
	bool COtlRecordset::IsEOF()
	{
		try
		{
			return !m_end;
		}
		catch ( otl_exception &p )
		{
			COtlConnect::PrintErr( p );
			return false;
		}

		return false;
	}

	std::string COtlRecordset::GetUpperStr( const char *filedName )
	{
		std::string upName = filedName;
		transform( upName.begin(), upName.end(), upName.begin(), toupper );
		return upName;
	}

	std::string COtlRecordset::GetValueStr( const char *value, const int type )
	{
		switch ( abs ( type ) )
		{
		case otl_var_none:
		case otl_var_char:
		case otl_var_varchar_long:
		case otl_var_long_string:
		case otl_var_db2time:
		case otl_var_db2date:
		case otl_var_blob:
		case otl_var_clob:
		case 92:
			return std::string ( "'" ) + value + "'";
			break;
		case otl_var_double:
		case otl_var_float:
		case otl_var_int:
		case otl_var_unsigned_int:
		case otl_var_short:
		case otl_var_long_int:
		case otl_var_timestamp:
		case otl_var_raw_long:
		case otl_var_refcur:
		case otl_var_bigint:		
			return value;
			break;
		case otl_var_tz_timestamp:
		case otl_var_ltz_timestamp:
			break;
		default:
			return value;
			break;
		}
		return "0";
	}
	void COtlRecordset::PrintErr(otl_exception &p)
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
	bool COtlRecordset::OpenStream( const std::string &sql, bool closeAutoCommit )
	{
		try
		{

			m_saveStream.open( 1, sql.c_str(), m_lpDBConnect->GetConnect() );
			if ( closeAutoCommit )
			{
				m_saveStream.set_commit( 0 );
			}

			return true;
		}
		catch ( otl_exception &e )
		{
			PrintErr( e );
		}
		catch (...)
		{

		}

		return false;

	}

	bool COtlRecordset::CloseStream()
	{
		try
		{
			m_saveStream.clean();
			m_saveStream.close();
			if ( m_lpDBConnect )
			{
				m_lpDBConnect->Commit();
			}
			return true;
		}
		catch (otl_exception &e)
		{
			PrintErr( e );
		}
		catch ( ... )
		{
		}

		return false;

	}

	bool COtlRecordset::StreamIn( time_t &value )
	{
		return false;
		try
		{
			m_saveStream << INT32 (value);
			return true;
		}
		catch(otl_exception &e)
		{
			PrintErr( e );
		}
		catch ( ... )
		{

		}

		return false;
	}
	bool COtlRecordset::StreamIn( int &value )
	{
		try
		{
			m_saveStream << value;
			return true;
		}
		catch(otl_exception &e)
		{
			PrintErr( e );
		}
		catch ( ... )
		{

		}
		return false;
	}
	bool COtlRecordset::StreamIn( char &value )
	{
		try
		{
			m_saveStream << value;
			return true;
		}
		catch(otl_exception &e)
		{
			PrintErr( e );
		}
		catch ( ... )
		{

		}
		return false;
	}

	bool COtlRecordset::StreamIn( char *value, int len )
	{
		try
		{
			otl_long_string l( value, 1, len );
			otl_lob_stream lob;



			m_saveStream << lob;
			lob.set_len( len );
			lob << l;

			lob.close();

			return true;
		}
		catch(otl_exception &e)
		{
			PrintErr( e );
		}
		catch ( ... )
		{

		}
		return false;
	}

	bool COtlRecordset::StreamIn( unsigned char &value )
	{
		try
		{
			m_saveStream << value;
			return true;
		}
		catch(otl_exception &e)
		{
			PrintErr( e );

		}
		catch ( ... )
		{

		}
		return false;
	}
	bool COtlRecordset::StreamIn( long &value )
	{
		try
		{
			m_saveStream << value;
			return true;
		}
		catch(otl_exception &e)
		{
			PrintErr( e );
		}
		catch ( ... )
		{

		}
		return false;
	}
	bool COtlRecordset::StreamIn( unsigned long &value )
	{
		try
		{
			m_saveStream << long(value);
			return true;
		}
		catch(otl_exception &e)
		{
			PrintErr( e );
		}
		catch ( ... )
		{

		}
		return false;
	}
	bool COtlRecordset::StreamIn( short &value )
	{
		try
		{
			m_saveStream << value;
			return true;
		}
		catch(otl_exception &e)
		{
			PrintErr( e );
		}
		catch ( ... )
		{

		}
		return false;
	}
	bool COtlRecordset::StreamIn( float &value )
	{
		try
		{
			m_saveStream << value;
			return true;
		}
		catch(otl_exception &e)
		{
			PrintErr( e );
		}catch(...)
		{

		}

		return false;
	}
	bool COtlRecordset::StreamIn( double &value )
	{
		try
		{
			m_saveStream << value;
			return true;
		}
		catch(otl_exception &e)
		{
			PrintErr( e );
		}
		catch ( ... )
		{

		}
		return false;
	}
	bool COtlRecordset::StreamIn( std::string &value )
	{
		try
		{
			m_saveStream << value.c_str();
			return true;
		}
		catch(otl_exception &e)
		{
			PrintErr( e );
		}
		catch ( ... )
		{

		}
		return false;
	}

}
