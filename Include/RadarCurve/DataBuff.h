#ifndef __DATA_BUFF_H_FILE__
#define __DATA_BUFF_H_FILE__
template<class T>
struct DataBuff
{
protected:
	T *_databuff;
	unsigned int _use_len;
	unsigned int _buff_len;
	unsigned int _clientID;
public:
	DataBuff(unsigned int len = 1024 )
	{ 
		_databuff = new T [len];
		_use_len = 0 ;
		_buff_len = len;
	};

	DataBuff ( const DataBuff &item )
	{
		if ( this == &item )
		{
			return;
		}
		_databuff = NULL;
		_use_len = 0;
		_buff_len = 0;
		copyData( item.getData(), item.getDataLen() );
	}

	DataBuff & operator = ( const DataBuff &item )
	{
		if ( this == &item )
		{
			return *this;
		}
		if ( _databuff )
		{
			delete[] _databuff;
		}
		_databuff = NULL;
		copyData( item.getData(), item.getDataLen() );
		return *this;
	}
	int buffLen()
	{
		return _buff_len;
	}

	void setBuffLen( unsigned int len )
	{
		if ( len > _buff_len )
		{
			T *anydata =new T[len];
			if ( anydata )
			{
				delete[] _databuff;
				_databuff = anydata;
				_buff_len = len;
				_use_len = 0;
			}
		}
	}

	void copyData( const T *databuff, unsigned int len )
	{
		if ( len <= 0 )
			return;

		if ( !_databuff )
		{
			_databuff = new T [ len ];
			_buff_len = len;
			_use_len = 0;
		}

		if ( len > _buff_len )
		{

			delete []_databuff;

			_databuff = new T[len];
			_buff_len = len;
			_use_len = _buff_len;

			memcpy ( _databuff, databuff, _buff_len * sizeof ( T ) );
		}else
		{
			_use_len = len;
			memcpy ( _databuff, databuff, len * sizeof ( T ) );
		}
	};

	void addData( const T* databuff, unsigned int len )
	{
		if ( _use_len + len > _buff_len )
		{
			T *anyData = new T [ _use_len + len ];
			_buff_len = _use_len + len;

			memcpy ( anyData, _databuff, sizeof ( T ) * _use_len );
			memcpy ( anyData + _use_len, databuff, len );

			delete []_databuff;
			_databuff = anyData;
			_use_len += len;
		}
		else
		{
			memcpy ( _databuff + _use_len, databuff, len );
			_use_len += len;
		}
	}


	//注意此时没有判断ＢＵＦ的大小哦
	void AddDataToEnd(const T* databuff,unsigned int len)
	{
		for (unsigned int i = 0 ; i < len ; ++i)
		{
			_databuff[_use_len + i] = *databuff++;
		}
		_use_len += len;

	}
	unsigned int getData( T* dataBuff, unsigned int startPos, unsigned int len )const
	{
		if ( startPos + len > getDataLen() )
		{
			len = getDataLen() - startPos;
			//return 0;
		}

		memcpy( dataBuff, getData() + startPos, len );
		return len;
	}

	void remove(unsigned int len)
	{
		if ( _use_len > len )
		{
			memmove( _databuff, &_databuff[len], _use_len - len );
			_use_len -= len;
		}else
		{
			_use_len = 0;
		}
	}

	T *getData() const{ return _databuff; };
	unsigned int getDataLen() const { return _use_len; };
	void clear() { _use_len = 0; };

	~DataBuff()
	{
		if ( _buff_len > 0 )
			delete []_databuff;
		_use_len = 0;
		_buff_len = 0;
	};
};

typedef DataBuff<unsigned char> CharDataBuf;

#endif // __DATA_BUFF_H_FILE__