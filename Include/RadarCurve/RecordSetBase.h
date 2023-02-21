#pragma once

#include <string>
namespace DBAPI
{
	class CRecordSetBase
	{
	public:
		CRecordSetBase(void);
		virtual ~CRecordSetBase(void);
	public:
		void SetAllToStr ( bool allToStr ){ m_bAllToStr = allToStr; };
	public:
		virtual bool Open( const char *sql ) = 0;
		virtual bool Close () = 0;
	public:

//		virtual bool GetFieldValue ( const long index, std::string &value  ) = 0;
		virtual bool GetFieldValue ( const long index, time_t &value  ) = 0;
/*		virtual bool GetFieldValue ( const long index, unsigned int &value  ) = 0;*/
		virtual bool GetFieldValue ( const long index, int &value  ) = 0;
		virtual bool GetFieldValue ( const long index, char *value, int *len  ) = 0;
		virtual bool GetFieldValue ( const long index, unsigned char &value  ) = 0;
		virtual bool GetFieldValue ( const long index, long &value  ) = 0;
		virtual bool GetFieldValue ( const long index, unsigned long &value  ) = 0;
		virtual bool GetFieldValue ( const long index, short &value  ) = 0;
/*		virtual bool GetFieldValue ( const long index, unsigned short &value  ) = 0;*/
		virtual bool GetFieldValue ( const long index, float &value  ) = 0;
		virtual bool GetFieldValue ( const long index, double &value  ) = 0;
		virtual bool GetFieldValue ( const long index, std::string &value  ) = 0;

	//	virtual bool GetFieldValue ( const char *filedName, std::string &value  ) = 0;
		virtual bool GetFieldValue ( const char *filedName, time_t &value  ) = 0;
/*		virtual bool GetFieldValue ( const char *filedName, unsigned int &value  ) = 0;*/
		virtual bool GetFieldValue ( const char *filedName, int &value  ) = 0;
		virtual bool GetFieldValue ( const char *filedName, char *value, int *len  ) = 0;
		virtual bool GetFieldValue ( const char *filedName, unsigned char &value  ) = 0;
		virtual bool GetFieldValue ( const char *filedName, long &value  ) = 0;
		virtual bool GetFieldValue ( const char *filedName, unsigned long &value  ) = 0;
		virtual bool GetFieldValue ( const char *filedName, short &value  ) = 0;
/*		virtual bool GetFieldValue ( const char *filedName, unsigned short &value  ) = 0;*/
		virtual bool GetFieldValue ( const char *filedName, float &value  ) = 0;
		virtual bool GetFieldValue ( const char *filedName, double &value  ) = 0;
		virtual bool GetFieldValue ( const char *filedName, std::string &value  ) = 0;

		virtual int	 GetFiledType( long index ) = 0;
		virtual int  GetFiledType ( const char*filedName ) = 0;

		virtual int	 GetFieldsCount() = 0;

		virtual std::string GetFieldName( long index ) = 0;


		virtual bool OpenStream( const std::string &sql, bool closeAutoCommit ) = 0;

		virtual bool CloseStream() = 0;

		virtual bool StreamIn( time_t &value ) = 0;
		virtual bool StreamIn( int &value ) = 0;
		virtual bool StreamIn( char &value ) = 0;
		virtual bool StreamIn( char *value, int len ) = 0;
		virtual bool StreamIn( unsigned char &value ) = 0;
		virtual bool StreamIn( long &value ) = 0;
		virtual bool StreamIn( unsigned long &value ) = 0;
		virtual bool StreamIn( short &value ) = 0;
		virtual bool StreamIn( float &value ) = 0;
		virtual bool StreamIn( double &value ) = 0;
		virtual bool StreamIn( std::string &value ) = 0;


	public:
		virtual bool MoveNext () = 0;
		virtual bool MoveFirst() = 0;
		virtual bool IsEOF () = 0;
	protected:
		bool m_bAllToStr;
	};
}
