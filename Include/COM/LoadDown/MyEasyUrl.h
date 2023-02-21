#pragma once
#include <Com\Export.h>
#include <OSG\Referenced>
#include <OSG\ref_ptr>
#include <osgDB/ReaderWriter>
//#include <osgDB/FileNameUtils>
//#include <osgDB/FileUtils>
//#include <osgDB/ReadFile>
//#include <osgDB/WriteFile>
#include <osgDB/Registry>

#include <iostream>
#include <sstream>
#include <fstream>

#include <curl/curl.h>
#include <curl/types.h>
#include <map>
namespace COM	{

enum ObjectType
{
	OBJECT,
	ARCHIVE,
	IMAGE,
	HEIGHTFIELD,
	NODE
};

class COM_EXPORT CMyEasyUrl : public osg::Referenced
{
public:

	struct StreamObject
	{
		StreamObject(std::ostream* stream1, const std::string& cacheFileName);

		void write(const char* ptr, size_t realsize);

		std::ostream*   _stream1;

		bool            _foutOpened;
		std::string     _cacheFileName;
		std::ofstream   _fout;
		std::string     _resultMimeType;
	};

	static size_t StreamMemoryCallback(void *ptr, size_t size, size_t nmemb, void *data);

	CMyEasyUrl();

	// Added this function to set the desired connection timeout if needed (in case someone needs to try to connect
	// to offline servers without freezing the process for a very long time) [even if, as stated on curl website,
	// some normal transfer may be timed out this way].
	inline void setConnectionTimeout(long val) { _connectTimeout = val; }

	// the timeout variable is used to limit the whole transfer duration instead of the connection phase only.
	inline void setTimeout(long val) { _timeout = val; }

	osgDB::ReaderWriter::ReadResult read(const std::string& proxyAddress, const std::string& fileName, StreamObject& sp, const osgDB::ReaderWriter::Options *options);

	/** Returns the mime type of the data retrieved with the provided stream object on a
	* previous call to CMyEasyUrl::read(). */
	std::string getResultMimeType(const StreamObject& sp) const;

protected:

	virtual ~CMyEasyUrl();

	// disallow copying
	CMyEasyUrl(const CMyEasyUrl& rhs):_curl(rhs._curl) {}
	CMyEasyUrl& operator = (const CMyEasyUrl&) { return *this; }


	CURL* _curl;

	std::string     _previousPassword;
	long            _previousHttpAuthentication;
	long            _connectTimeout;
	long            _timeout;
};

}