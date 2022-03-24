#include "StdAfx.h"
#include <RadarCurve/MeasureProject.h>
#include <OpenThreads/ScopedLock>
#include "RadarCurve/RadarManager.h"
#include <RadarCurve/DBConnectBase.h>
#include "DBAccess/OtlRecordset.h"
#include <OpenThreads/Block>
#include <vector>
#include "DataTemplateTab.h"
#include "VideoTemplateTab.h"
#include <Object/CmdCenter/CmdCenter.h>
#include <Object/ShowObj/ScreenMark/ScreenMark.h>
#include <Object/ObjManager.h>
#include "ProjectNameTab.h"
#include "DataCountTab.h"
#include "SGYWriter.h"
#include <ctime>
#include <direct.h>//用来创建文件夹
#include <io.h>
#include <windows.h>//用来创建文件夹
#include <afxstr.h>//CString头文件
//ftp
#include <wininet.h>//网络
#pragma comment(lib,"wininet.lib")//网络
//socket
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
//#include <arpa/inet.h>
#include <windows.h>
//#include <sys/socket.h>
//#include <sys/types.h>
//#include <sys/time.h>
//#include <netinet/in.h>
//#include <netdb.h>
//#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <process.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "md5.h"

#define POST_FORMAT "POST %s HTTP/1.1\r\n\
Host: %s:%d\r\n\
Content-Length: %s\r\n\
Content-Type: application/json;\r\n\
\r\n\
%s"
#define INIT_JSON_FORMAT "\
{\n\
    \"taskcode\": \"%s\",\n\
    \"gprcode\": \"%s\",\n\
    \"timestamp\": \"%s\",\n\
    \"latlng\": \"%f,%f\"\n\
}"
#define GPS_JSON_FORMAT_FOR_EACH "\
{\n\
    \"oprcode\": \"%s\",\n\
    \"lang\": %f,\n\
    \"lont\": %f,\n\
    \"taskcode\": \"%s\",\n\
    \"gprcode\": \"%s\",\n\
    \"timestamp\": \"%s\"\n\
}"//那lang lont是接口规定的别改
#define CHECK_OPR_JSON_FORMAT "\
{\n\
    \"oprcode\": \"%s\"\n\
}"
#define UPDATE_OPR_JSON_FORMAT "\
{\n\
    \"oprcode\": \"%s\",\n\
    \"rtstatus\": %d\n\
}"
#define RADAR_JSON_FORMAT "\
{\n\
    \"AccountId\": \"%s\",\n\
    \"AccessKey\": \"%s\",\n\
    \"GPRUUID\": \"%s\",\n\
    \"Timestamp\": \"%s\",\n\
    \"Random\": \"%d\",\n\
    \"Content\": \"%s\"\n\
}"

#define COMPLETE_JSON_FORMAT "\
{\n\
    \"oprcode\": \"%s\",\n\
    \"foldername\": \"%s\",\n\
    \"taskcode\": \"%s\",\n\
    \"gprcode\": \"%s\",\n\
    \"timestamp\": \"%s\"\n\
}"

//=========================================sha256加密算法=========================================//
//=========================================sha256加密算法=========================================//
#define SHA256_ROTL(a,b) (((a>>(32-b))&(0x7fffffff>>(31-b)))|(a<<b))
#define SHA256_SR(a,b) ((a>>b)&(0x7fffffff>>(b-1)))
#define SHA256_Ch(x,y,z) ((x&y)^((~x)&z))
#define SHA256_Maj(x,y,z) ((x&y)^(x&z)^(y&z))
#define SHA256_E0(x) (SHA256_ROTL(x,30)^SHA256_ROTL(x,19)^SHA256_ROTL(x,10))
#define SHA256_E1(x) (SHA256_ROTL(x,26)^SHA256_ROTL(x,21)^SHA256_ROTL(x,7))
#define SHA256_O0(x) (SHA256_ROTL(x,25)^SHA256_ROTL(x,14)^SHA256_SR(x,3))
#define SHA256_O1(x) (SHA256_ROTL(x,15)^SHA256_ROTL(x,13)^SHA256_SR(x,10))
char* StrSHA256(const char* str, long long length, char* sha256){
    char *pp, *ppend;
    long l, i, W[64], T1, T2, A, B, C, D, E, F, G, H, H0, H1, H2, H3, H4, H5, H6, H7;
    H0 = 0x6a09e667, H1 = 0xbb67ae85, H2 = 0x3c6ef372, H3 = 0xa54ff53a;
    H4 = 0x510e527f, H5 = 0x9b05688c, H6 = 0x1f83d9ab, H7 = 0x5be0cd19;
    long K[64] = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
        0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
    };
    l = length + ((length % 64 > 56) ? (128 - length % 64) : (64 - length % 64));
    if (!(pp = (char*)malloc((unsigned long)l))) return 0;
    for (i = 0; i < length; pp[i + 3 - 2 * (i % 4)] = str[i], i++);
    for (pp[i + 3 - 2 * (i % 4)] = 128, i++; i < l; pp[i + 3 - 2 * (i % 4)] = 0, i++);
    *((long*)(pp + l - 4)) = length << 3;
    *((long*)(pp + l - 8)) = length >> 29;
    for (ppend = pp + l; pp < ppend; pp += 64){
        for (i = 0; i < 16; W[i] = ((long*)pp)[i], i++);
        for (i = 16; i < 64; W[i] = (SHA256_O1(W[i - 2]) + W[i - 7] + SHA256_O0(W[i - 15]) + W[i - 16]), i++);
        A = H0, B = H1, C = H2, D = H3, E = H4, F = H5, G = H6, H = H7;
        for (i = 0; i < 64; i++){
            T1 = H + SHA256_E1(E) + SHA256_Ch(E, F, G) + K[i] + W[i];
            T2 = SHA256_E0(A) + SHA256_Maj(A, B, C);
            H = G, G = F, F = E, E = D + T1, D = C, C = B, B = A, A = T1 + T2;
        }
        H0 += A, H1 += B, H2 += C, H3 += D, H4 += E, H5 += F, H6 += G, H7 += H;
    }
    free(pp - l);
    sprintf(sha256, "%08x%08x%08x%08x%08x%08x%08x%08x", H0, H1, H2, H3, H4, H5, H6, H7);
    return sha256;
}
//=========================================sha256加密算法=========================================//
//=========================================sha256加密算法=========================================//

#define THREAD_MAX_DATA 5000

//===================================================================class SaveFileDataThread 数据本地保存线程================================================================//
//===================================================================class SaveFileDataThread 数据本地保存线程================================================================//
class SaveRadarFileDataCmd : public SaveDataCmd{
	public:
		osg::ref_ptr<RadarData> _lpData;
		SaveRadarFileDataCmd(){};
		virtual void exec( std::string const& projectTab ){};
		virtual void exec(SGYWriter* _SGYWriter){
			if (_SGYWriter){
				/*
				if(m_nUpload==0){
					_SGYWriter->writeRadarData(_lpData.get());
				}else if(m_nUpload==1){
					_SGYWriter->writeRadarDataForAutomation(_lpData.get());
				}*/
				_SGYWriter->writeRadarData(_lpData.get());
				if ( RadarManager::Instance()->getExportGpsPos() ){
					if(_SGYWriter->writeGpsFlag){
						_SGYWriter->writeGPSData(_lpData.get());
					}
				}
				if ( RadarManager::Instance()->getExportKml() ){
					if(_SGYWriter->writeKmlFlag){
						//_SGYWriter->writeKml(_lpData.get());
					}
				}
			}
			_lpData = NULL;
		}
};

class SaveFileDataThread : public OpenThreads::Thread{
	public:
		SaveFileDataThread(){};
		~SaveFileDataThread(){};

		void run(){
			while( !testCancel() ){
				CmdQueue tmpQueue;
				{
					OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
					tmpQueue.swap( _dataQueue );//swap操作实现交换两个容器内所有元素的功能
				}
				if ( tmpQueue.size() > 0 ){
					//unsigned long startTime = GetTickCount();
					for ( CmdQueue::iterator it = tmpQueue.begin(); it != tmpQueue.end(); it++ ){
						SaveDataCmd *lpCmd = (*it).get();
						if ( lpCmd ){
							lpCmd->exec( _SGYWriter );
						}
					}
				}
				if ( _dataQueue.size() == 0 ){
					_block.reset();
					_block.block();
				}
				if ( !testCancel() ){
					continue;
				}
			}
		};

		int cancel(){
			OpenThreads::Thread::cancel();
			//清空队列
			CmdQueue tmpQueue;
			{
				OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
				tmpQueue.swap( _dataQueue );//swap操作实现交换两个容器内所有元素的功能
			}
			if ( isRunning() ){
				_block.reset();
				_block.release();
				join();
			}
			return 0;
		};

		void addData( SaveDataCmd *data ){
			OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
			_dataQueue.push_back( data );

			_block.reset();
			_block.release();
		};

		//20200306hjl 用于测量轮回滚
		void deleteData(){
			OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
			_dataQueue.pop_back();
			//_dataQueue.erase(_dataQueue.end());
			_block.reset();
			_block.release();
		};

		bool init(const char *dbsrc, const char *dbname, const char *user, const char *pass, std::string &projectName);
	public:
		OpenThreads::Mutex _mutex;
		OpenThreads::Block _block;
		std::string _projectTab;
		typedef std::vector<osg::ref_ptr<SaveDataCmd> > CmdQueue;
		CmdQueue _dataQueue;
		SGYWriter* _SGYWriter;
};
//===================================================================class SaveFileDataThread 数据本地保存线程================================================================//
//===================================================================class SaveFileDataThread 数据本地保存线程================================================================//


//===================================================================class UploadFileToFtpThread 上传线程================================================================//
//===================================================================class UploadFileToFtpThread 上传线程================================================================//
class UploadFileToFtpThread : public OpenThreads::Thread{
	public:
		UploadFileToFtpThread(){};
		~UploadFileToFtpThread(){};

		void run(){
			while( !testCancel() ){
				std::vector<std::string> tmpLocalPathQueue;
				std::vector<std::string> tmpOnlinePathQueue;
				std::vector<int> tmpPartQueue;
				{
					OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
					tmpLocalPathQueue.swap( _localPathQueue );
					tmpOnlinePathQueue.swap( _OnlinePathQueue );
					tmpPartQueue.swap( _PartQueue );
					//_partQueue.clear();
				}
				if ( tmpLocalPathQueue.size() > 0 ){
					//unsigned long startTime = GetTickCount();
					std::vector<std::string>::iterator itOnline = tmpOnlinePathQueue.begin();
					std::vector<int>::iterator itPart = tmpPartQueue.begin();
					for ( std::vector<std::string>::iterator itLocal = tmpLocalPathQueue.begin(); itLocal != tmpLocalPathQueue.end(); itLocal++ ){
						_MeasureProject->uploadFileToFtp(*itLocal,*itOnline,*itPart);
						itOnline++;
						itPart++;
					}
				}
				if ( _localPathQueue.size() == 0 ){
					_block.reset();
					_block.block();
				}
				if ( !testCancel() ){
					continue;
				}
			}
		};

		int cancel(){
			OpenThreads::Thread::cancel();
			/*
			//清空队列
			CmdQueue tmpQueue;
			{
				OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
				tmpQueue.swap( _dataQueue );//swap操作实现交换两个容器内所有元素的功能
			}*/

			std::vector<std::string> tmpLocalPathQueue;
			std::vector<std::string> tmpOnlinePathQueue;
			std::vector<int> tmpPartQueue;
			{
				OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
				tmpLocalPathQueue.swap( _localPathQueue );
				tmpOnlinePathQueue.swap( _OnlinePathQueue );
				tmpPartQueue.swap( _PartQueue );
				//_partQueue.clear();
			}

			if ( isRunning() ){
				_block.reset();
				_block.release();
				join();
			}
			return 0;
		};

		//void addData( SaveDataCmd *data ){
		void addPart( std::string localPath, std::string onlinePath, int part ){
			OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
			//_dataQueue.push_back( data );
			_localPathQueue.push_back( localPath );
			_OnlinePathQueue.push_back( onlinePath );
			_PartQueue.push_back(part);
			_block.reset();
			_block.release();
		};

		//bool init(const char *dbsrc, const char *dbname, const char *user, const char *pass, std::string &projectName);
	public:
		OpenThreads::Mutex _mutex;
		OpenThreads::Block _block;
		//std::string _projectTab;
		//typedef std::vector<osg::ref_ptr<SaveDataCmd> > CmdQueue;
		//CmdQueue _dataQueue;
		//SGYWriter* _SGYWriter;
		MeasureProject* _MeasureProject;
		std::vector<std::string> _localPathQueue;
		std::vector<std::string> _OnlinePathQueue;
		std::vector<int> _PartQueue;
};
//===================================================================class UploadFileToFtpThread 上传线程================================================================//
//===================================================================class UploadFileToFtpThread 上传线程================================================================//

//===================================================================class SaveOnlineThread 上传线程================================================================//
//===================================================================class SaveOnlineThread 上传线程================================================================//
class SaveOnlineThread : public OpenThreads::Thread{
	public:
		SaveOnlineThread(){};
		~SaveOnlineThread(){};

		void run(){
			while( !testCancel() ){
				std::vector<int> tmpQueue;
				{
					OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
					tmpQueue.swap( _partQueue );
					//_partQueue.clear();
				}
				if ( tmpQueue.size() > 0 ){
					//unsigned long startTime = GetTickCount();
					for ( std::vector<int>::iterator it = tmpQueue.begin(); it != tmpQueue.end(); it++ ){
						//int *dPart = *it;
						_MeasureProject->changeTimeTab();
						//if ( *it!=1 ){
							Sleep(1000);
							_MeasureProject->changeFtpFolderPath();
							_MeasureProject->writeRadFileForPart( *it );
							//_MeasureProject->uploadTxtFileForGroup( *it );
							_MeasureProject->uploadDataForPart( *it );
							_MeasureProject->postEndMessage( *it );
							
						//}
						_MeasureProject->m_nFinishedPart = *it;
					}
				}
				if ( _partQueue.size() == 0 ){
					_block.reset();
					_block.block();
				}
				if ( !testCancel() ){
					continue;
				}
			}
		};

		int cancel(){
			OpenThreads::Thread::cancel();
			/*
			//清空队列
			CmdQueue tmpQueue;
			{
				OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
				tmpQueue.swap( _dataQueue );//swap操作实现交换两个容器内所有元素的功能
			}*/

			std::vector<int> tmpQueue;
			{
				OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
				tmpQueue.swap( _partQueue );
				//_partQueue.clear();
			}

			if ( isRunning() ){
				_block.reset();
				_block.release();
				join();
			}
			return 0;
		};

		//void addData( SaveDataCmd *data ){
		void addPart( int part ){
			OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
			//_dataQueue.push_back( data );
			_partQueue.push_back( part );

			_block.reset();
			_block.release();
		};

		//bool init(const char *dbsrc, const char *dbname, const char *user, const char *pass, std::string &projectName);
	public:
		OpenThreads::Mutex _mutex;
		OpenThreads::Block _block;
		//std::string _projectTab;
		//typedef std::vector<osg::ref_ptr<SaveDataCmd> > CmdQueue;
		//CmdQueue _dataQueue;
		//SGYWriter* _SGYWriter;
		MeasureProject* _MeasureProject;
		std::vector<int> _partQueue;
};
//===================================================================class SaveOnlineThread 上传线程================================================================//
//===================================================================class SaveOnlineThread 上传线程================================================================//

//===================================================================class SaveOnlineForRepairmentThread 上传线程================================================================//
//===================================================================class SaveOnlineForRepairmentThread 上传线程================================================================//
class SaveOnlineForRepairmentThread : public OpenThreads::Thread{
	public:
		SaveOnlineForRepairmentThread(){};
		~SaveOnlineForRepairmentThread(){};

		void run(){
			while( !testCancel() ){
				std::vector<std::string> tmpLocalPathQueue;
				std::vector<std::string> tmpOnlinePathQueue;
				std::vector<int> tmpPartQueue;
				{
					OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
					tmpLocalPathQueue.swap( _localPathQueue );
					tmpOnlinePathQueue.swap( _OnlinePathQueue );
					tmpPartQueue.swap( _PartQueue );
					//_partQueue.clear();
				}
				if ( tmpLocalPathQueue.size() > 0 ){
					//unsigned long startTime = GetTickCount();
					std::vector<std::string>::iterator itOnline = tmpOnlinePathQueue.begin();
					std::vector<int>::iterator itPart = tmpPartQueue.begin();
					for ( std::vector<std::string>::iterator itLocal = tmpLocalPathQueue.begin(); itLocal != tmpLocalPathQueue.end(); itLocal++ ){
						_MeasureProject->uploadDataForPartForRepairment(*itLocal,*itOnline,*itPart);
						itOnline++;
						itPart++;
					}
				}
				if ( _localPathQueue.size() == 0 ){
					_block.reset();
					_block.block();
				}
				if ( !testCancel() ){
					continue;
				}
			}
		};

		int cancel(){
			OpenThreads::Thread::cancel();
			/*
			//清空队列
			CmdQueue tmpQueue;
			{
				OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
				tmpQueue.swap( _dataQueue );//swap操作实现交换两个容器内所有元素的功能
			}*/

			std::vector<std::string> tmpLocalPathQueue;
			std::vector<std::string> tmpOnlinePathQueue;
			std::vector<int> tmpPartQueue;
			{
				OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
				tmpLocalPathQueue.swap( _localPathQueue );
				tmpOnlinePathQueue.swap( _OnlinePathQueue );
				tmpPartQueue.swap( _PartQueue );
				//_partQueue.clear();
			}

			if ( isRunning() ){
				_block.reset();
				_block.release();
				join();
			}
			return 0;
		};

		//void addData( SaveDataCmd *data ){
		void addPart( std::string localPath, std::string onlinePath, int part ){
			OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
			//_dataQueue.push_back( data );
			_localPathQueue.push_back( localPath );
			_OnlinePathQueue.push_back( onlinePath );
			_PartQueue.push_back(part);
			_block.reset();
			_block.release();
		};

		//bool init(const char *dbsrc, const char *dbname, const char *user, const char *pass, std::string &projectName);
	public:
		OpenThreads::Mutex _mutex;
		OpenThreads::Block _block;
		//std::string _projectTab;
		//typedef std::vector<osg::ref_ptr<SaveDataCmd> > CmdQueue;
		//CmdQueue _dataQueue;
		//SGYWriter* _SGYWriter;
		MeasureProject* _MeasureProject;
		std::vector<std::string> _localPathQueue;
		std::vector<std::string> _OnlinePathQueue;
		std::vector<int> _PartQueue;
};
//===================================================================class SaveOnlineForRepairmentThread 上传线程================================================================//
//===================================================================class SaveOnlineForRepairmentThread 上传线程================================================================//



//===================================================================class UploadGpsOnlineThread 上传线程================================================================//
//===================================================================class UploadGpsOnlineThread 上传线程================================================================//
class UploadGpsOnlineThread : public OpenThreads::Thread{
	public:
		UploadGpsOnlineThread(){};
		~UploadGpsOnlineThread(){};

		void run(){
			while( !testCancel() ){
				std::vector<osg::Vec3d> tmpQueue;
				{
					OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
					tmpQueue.swap( _gpsDataQueue );
					//_partQueue.clear();
				}
				if ( tmpQueue.size() > 0 ){
					//unsigned long startTime = GetTickCount();
					for ( std::vector<osg::Vec3d>::iterator it = tmpQueue.begin(); it != tmpQueue.end(); it++ ){
						//Sleep(1000);
						_MeasureProject->uploadGpsDataByPost(*it);
					}
				}
				if ( _gpsDataQueue.size() == 0 ){
					_block.reset();
					_block.block();
				}
				if ( !testCancel() ){
					continue;
				}
			}
		};

		int cancel(){
			OpenThreads::Thread::cancel();
			/*
			//清空队列
			CmdQueue tmpQueue;
			{
				OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
				tmpQueue.swap( _dataQueue );//swap操作实现交换两个容器内所有元素的功能
			}*/

			std::vector<osg::Vec3d> tmpQueue;
			{
				OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
				tmpQueue.swap( _gpsDataQueue );
				//_partQueue.clear();
			}

			if ( isRunning() ){
				_block.reset();
				_block.release();
				join();
			}
			return 0;
		};

		//void addData( SaveDataCmd *data ){
		void addData( osg::Vec3d pos ){
			OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
			//_dataQueue.push_back( data );
			_gpsDataQueue.push_back( pos );

			_block.reset();
			_block.release();
		};

		//bool init(const char *dbsrc, const char *dbname, const char *user, const char *pass, std::string &projectName);
	public:
		OpenThreads::Mutex _mutex;
		OpenThreads::Block _block;
		//std::string _projectTab;
		//typedef std::vector<osg::ref_ptr<SaveDataCmd> > CmdQueue;
		//CmdQueue _dataQueue;
		//SGYWriter* _SGYWriter;
		MeasureProject* _MeasureProject;
		std::vector<osg::Vec3d> _gpsDataQueue;
};
//===================================================================class UploadGpsOnlineThread 上传线程================================================================//
//===================================================================class UploadGpsOnlineThread 上传线程================================================================//


//===================================================================class SaveDataThread 数据库用的================================================================//
//===================================================================class SaveDataThread 数据库用的================================================================//
class SaveDataThread : public OpenThreads::Thread{
	public:
		SaveDataThread();
		void run();
		int cancel();
		void addData( SaveDataCmd *data );
		bool isFull();
		bool init(const char *dbsrc, const char *dbname, const char *user, const char *pass, std::string &projectName);
	public:
		OpenThreads::Mutex _mutex;
		//typedef std::vector<osg::ref_ptr<RadarData>> ReceiveQueue;
		OpenThreads::Block _block;
		//ReceiveQueue _dataQueue;
		std::string _projectTab;
	// 	DataTemplateTab _tab;
	// 	VideoTemplateTab _videoTab;
		typedef std::vector<osg::ref_ptr<SaveDataCmd> > CmdQueue;
		CmdQueue _dataQueue;
		CDBConnectHelp _dbHelp;
		DataTemplateTab _dataTab;
};
SaveDataThread::SaveDataThread()
{

}
bool SaveDataThread::init(const char *dbsrc, const char *dbname, const char *user, const char *pass, std::string &projectName){
	if ( !_dbHelp.Open(dbsrc, dbname, user, pass ) ){
		return false;
	}
	if ( !_dbHelp.ExecSql( "alter session set sort_area_size=100000000" ) )
	{

		TRACE("set session sort_area_size failed!\n");
	}

	_dataTab.setProjectName( projectName );

	std::string nologginsql = "alter table " + _dataTab.getProjectName() + " nologging";
	if ( !_dbHelp.ExecSql( nologginsql.c_str() ) ){
		printf("set session sort_area_size failed!\n");
	}

	_dataTab.openStream( &_dbHelp );
	return true;
}
bool SaveDataThread::isFull(){
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
	return _dataQueue.size() >= THREAD_MAX_DATA;
}
void SaveDataThread::run(){
	while( !testCancel() ){
		CmdQueue tmpQueue;
		{
			OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
			tmpQueue.swap( _dataQueue );
		}

		try{
			if ( tmpQueue.size() > 0 ){
				//unsigned long startTime = GetTickCount();
				for ( CmdQueue::iterator it = tmpQueue.begin(); it != tmpQueue.end(); it ++ ){
					SaveDataCmd *lpCmd = (*it).get();
					if ( lpCmd ){
						//lpCmd->exec( _projectTab );//hjl backORnot
					}
				}
				// 		unsigned long endTime = GetTickCount();
				// 		if ( endTime != startTime )
				// 		{
				// 			TRACE("use Time : %f, count = %d, pre second save count : %f\n", ( endTime - startTime ) / 1000.0f, tmpQueue.size(), 1000 / ( (endTime - startTime) * 1.0f / tmpQueue.size()) )  ;
				// 		}
				{
					_dbHelp.GetConnect()->Commit();
				}
			}
		
		}
		catch (...){
			
		}

		if ( _dataQueue.size() == 0 ){
			_block.reset();
			_block.block();
		}
		if ( !testCancel() ){
			continue;
		}

	}
}
void SaveDataThread::addData( SaveDataCmd *data ){
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
	_dataQueue.push_back( data );
	_block.reset();
	_block.release();
}
int SaveDataThread::cancel(){
	OpenThreads::Thread::cancel();
	if ( isRunning() ){
		_block.reset();
		_block.release();
		join();
	}
	_dataTab.closeStream();
	return 0;
}
//===================================================================class SaveDataThread 数据库用的================================================================//
//===================================================================class SaveDataThread 数据库用的================================================================//



class SaveRadarDataCmd : public SaveDataCmd{
	public:

		SaveRadarDataCmd()
		{
			_dbHelp = NULL;
			_lpTab = NULL;
		}
		virtual void exec( std::string const& projectTab )
		{
			if ( !_lpData )
			{
				return;
			}
			CDBConnectHelp *dbHelp = RadarManager::Instance()->getDBHelp();

			if ( !dbHelp )
			{
				return;
			}
			_lpTab->insertEX( _dbHelp, _lpData.get() );

			_lpData = NULL;
		}
		
		osg::ref_ptr<RadarData> _lpData;

		CDBConnectHelp *_dbHelp;
		DataTemplateTab *_lpTab;
};


class UpdateProjectInfoCmd : public SaveDataCmd
{
	public:
		UpdateProjectInfoCmd( ProjectNameTab::ProjectNameRow *lpRow ){
			_lpRow = lpRow;
		}

		virtual void exec( std::string const &projectTab ){
			ProjectNameTab::ProjectNameRow *lpRow = _lpRow.get();
			if ( lpRow ){
				ProjectNameTab tab;
				CDBConnectHelp *dbHelp = RadarManager::Instance()->getDBHelp();

				if ( !dbHelp ){
					return;
				}
				tab.update( dbHelp, lpRow );
			}
		}
		osg::ref_ptr<ProjectNameTab::ProjectNameRow> _lpRow;
};


//////////////////////////////////////////////////////////////////////////
MeasureProject::MeasureProject(void)
{

	//_lpThread = NULL;
	for(int i=0;i<CHANNELCOUNT;i++){
		_setMark[i] = false;
		m_arrnRecordTotalWheelCount[i]=0;
		m_arrnRealTotalWheelCount[i]=0;
	}

	m_nGpsTimeStamp=0;
	m_fLastGpsLen = 0.0f;
	m_fLastPostedGpsLen=0.0f;
	_lastLen = 0.0f;
	//_recordTotalWheelCount = 0;
	//_realTotalWheelCount = 0;
	m_vecUploadWheelCount.clear();
	m_vecUploadWheelCount.push_back(0);

	m_vecVecUploadSuccessFlag.clear();
	
	m_nInitOpr = 0;
	m_nRoadPart = 1;
	m_nFinishedPart = 1;
	ConfigureSet *cfg = RadarManager::Instance()->getConfigureSet();
	m_nChannelCount = atoi( cfg->get("radar", "channelCount").c_str() );
	m_nChannelCountForUpload = atoi( cfg->get("radar", "channelCountForUpload").c_str() );
	m_nTimeThreshold = atoi( cfg->get("radar", "threshold").c_str() );
	m_nUpload = atoi( cfg->get("radar", "uploadFlag").c_str() );

	m_strTaskCode = cfg->get("projectname", "projectName");
	m_strEquipmentName = cfg->get("projectname", "equipmentName");
	m_strPostServerIp = cfg->get("post", "serverIP");
	m_nPostPort = atoi( cfg->get("post", "port").c_str() );


	//_lpThread = new SaveDataThread;
	//_lpThread->start();

	_maxID = 0;
	m_bIsBegin = false;

	m_bIsAdd = false;

	m_iRadarWorkType = 8;//
	m_iSaveOracle = 1;//
	m_strSavePath = L"c:\\";


	for(int i=0;i<CHANNELCOUNT;i++){
		_lpThreadSaveFile[i] = NULL;
		m_SGYWriter[i] = NULL;
	}
	_lpThreadSaveOnline=NULL;
	_lpThreadSaveOnlineForRepairment=NULL;
	_lpThreadUploadGps=NULL;
}

//用以配合gps单个文件输出 需通过更改gpswriter进行优化
int useIt(int num){
	//int tempArr[MAX_CHANNEL]={RadarManager::Instance()->int_Check_Channel1,RadarManager::Instance()->int_Check_Channel2,RadarManager::Instance()->int_Check_Channel3,RadarManager::Instance()->int_Check_Channel4,RadarManager::Instance()->int_Check_Channel5,RadarManager::Instance()->int_Check_Channel6,RadarManager::Instance()->int_Check_Channel7,RadarManager::Instance()->int_Check_Channel8};//!!!!!!!
	/*
	int tempArr[MAX_CHANNEL];
	for (int i=0;i<MAX_CHANNEL;i++){
		tempArr[i]=RadarManager::Instance()->int_Check_Channel[i];
	}*/
	int flag=1;
	for(int i=0;i<num;i++){
		if(RadarManager::Instance()->int_Check_Channel[i]==1){
			flag=0;
			break;
		}
	}
	return flag;
}

string intToString(int n){
	std::stringstream ss;
	ss<<n;
	return ss.str();
}

CString StringToCString( std::string str ){
	USES_CONVERSION;
	CString cstr = A2T(str.c_str());
	return cstr;
}

MeasureProject::~MeasureProject(void){
	for ( ThreadVector::iterator it = _threadVector.begin(); it != _threadVector.end(); it ++ ){
		OpenThreads::Thread *lpThread = (*it);
		if ( !lpThread ){
			continue;
		}
		if ( lpThread->isRunning() ){
			lpThread->cancel();
		}
		delete lpThread;
	}
	_threadVector.clear();

	for(int i =0;i<CHANNELCOUNT;i++){
		if (_lpThreadSaveFile[i]){
			_lpThreadSaveFile[i]->cancel();
			delete _lpThreadSaveFile[i];
			_lpThreadSaveFile[i] = NULL;
		}
	}

	if (true == m_bIsBegin){
		for(int i =0;i<CHANNELCOUNT;i++){
			if (m_SGYWriter[i]){
				m_SGYWriter[i]->closeSGY();
				if ( RadarManager::Instance()->getExportGpsPos() ){
					if(useIt(i)){
						m_SGYWriter[i]->closeGPS();}
				}
				if ( RadarManager::Instance()->getExportKml() ){
					if(useIt(i)){
						m_SGYWriter[i]->writeKmlEnding();
						m_SGYWriter[i]->closeKml();
					}
				}
			}
		}

		for(int i=0; i<CHANNELCOUNT; i++){
			if (m_SGYWriter[i]){
				delete m_SGYWriter[i];
				m_SGYWriter[i] = NULL;
			}
		}
		
		writeRadFile();
		writeNameCsvFile();
		copyRadarData();

		AfxMessageBox(L"保存数据完成。");
		m_bIsBegin = false;
	}

	if(m_nUpload==1){
		Sleep( 2000 );
		if(_lpThreadUploadGps){
			_lpThreadUploadGps->cancel();
			delete _lpThreadUploadGps;
			_lpThreadUploadGps = NULL;
		}
		/*
		if((_realTotalWheelCount-9)%m_nThreshold!=0){
			//uploadDataForPart(m_nRoadPart);
			((SaveOnlineThread*)_lpThreadSaveOnline)->addPart(m_nRoadPart);
			
			//m_nRoadPart=m_nRoadPart+1;
		}*/
		m_vecUploadWheelCount.push_back(m_arrnRealTotalWheelCount[0]);
		if(m_nUpload==1){
			m_vecUploadWheelCount.push_back(m_arrnRealTotalWheelCount[0]);
			std::vector<bool> vecTemp;//用来判断是否上传成功的
			for(int i=0;i<15;i++){//一共15项 txt cor csv rad*6 rd3*6
				vecTemp.push_back(true);
			}
			m_vecVecUploadSuccessFlag.push_back(vecTemp);
			((SaveOnlineThread*)_lpThreadSaveOnline)->addPart(m_nRoadPart);
			while(m_nRoadPart!=m_nFinishedPart){//保证上传完成
				Sleep(5000);
			}
		}else{
			writeRadFileForPart(m_nRoadPart);
		}

		if(_lpThreadSaveOnline){
			_lpThreadSaveOnline->cancel();
			delete _lpThreadSaveOnline;
			_lpThreadSaveOnline = NULL;
		}

		if(_lpThreadSaveOnlineForRepairment){
			_lpThreadSaveOnlineForRepairment->cancel();
			delete _lpThreadSaveOnlineForRepairment;
			_lpThreadSaveOnlineForRepairment = NULL;
		}
		
		AfxMessageBox(L"上传数据完成。");
		m_vecUploadWheelCount.clear();
	}

	//_recordTotalWheelCount = 0;
	//_realTotalWheelCount = 0;
	for(int i=0;i<CHANNELCOUNT;i++){
		m_arrnRecordTotalWheelCount[i]=0;
		m_arrnRealTotalWheelCount[i]=0;
	}

	if(m_nUpload==1){
		if(m_fpLog){
			fclose( m_fpLog );
			m_fpLog = 0;
		}

		if(m_fpLogForRepairment){
			fclose( m_fpLogForRepairment );
			m_fpLogForRepairment = 0;
		}
	}
}

void MeasureProject::addSaveCmd(SaveDataCmd *lpCMD )
{
// 	if ( !_lpThread )
// 	{
// 		return;
// 	}
	SaveDataThread *lpSaveThread = dynamic_cast<SaveDataThread*>(getAvaliadThread());
	if ( lpSaveThread ){
		lpSaveThread->addData( lpCMD );
	}
}
/*
void test(){
	string ftpSvrIp="39.106.6.179";
	int port=21;
	string userName="administrator";
	string password="Pass@word2021";
	
	HINTERNET hint;
	HINTERNET hftp;
	//创建ftp连接
	hint = InternetOpen(0, INTERNET_OPEN_TYPE_PRECONFIG, 0, 0, 0);
	if (hint==NULL)//连上为止  
	{   
		hint = InternetOpen(0, INTERNET_OPEN_TYPE_PRECONFIG, 0, 0, 0);
	} 
	//创建ftp连接
	hftp = InternetConnectA(hint, ftpSvrIp.c_str(),port,userName.c_str(),password.c_str(), INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
	if (hftp==NULL)//连上为止
	{
		hftp = InternetConnectA(hint, ftpSvrIp.c_str(),port,userName.c_str(),password.c_str(), INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
	}
	//bool flag1=FtpPutFile(hftp,"F:\\test\\GD20210913180634\\GD20210913180634_1\\GD20210913180634_1_1.rad","GD20210913180634_1_1.rad", FTP_TRANSFER_TYPE_ASCII, 0);	
	bool flag1=FtpPutFileA(hftp,"C:\\Users\\Administrator\\Desktop\\20210926\\20210926130000\\20210926130000_3.rd3",".\\test\\20210926130000_3.rd3", FTP_TRANSFER_TYPE_BINARY, 0);
	if(flag1==0){
		cout << "Failed to send file." << endl;
        cout << "Error: " << GetLastError() << endl;
        cout << "Response Info: " << InternetGetLastResponseInfo << endl;
	}
	InternetCloseHandle(hftp);
	InternetCloseHandle(hint);
	AfxMessageBox(L"test");
}*/

void MeasureProject::addData( RadarData *rd, int index ){
	//非网口模式且以文件保存
	if ((m_iRadarWorkType == RADAR_WORK_TYPE_ONE_USB || m_iRadarWorkType == RADAR_WORK_TYPE_DOUBLE_USB || m_iRadarWorkType == RADAR_WORK_TYPE_DOUBLE_USB_OLD || m_iRadarWorkType == RADAR_WORK_TYPE_FOUR_USB) && m_iSaveOracle == 0){
		if ( index >= 8 ){
			return;
		}
		if(m_arrnRecordTotalWheelCount[index] == rd->getRealWheelCount()){
			return;
		}
		osg::ref_ptr<SaveRadarFileDataCmd> lpCmd = new SaveRadarFileDataCmd;//osg库的线程
		RadarData *tmpRD = rd;
		lpCmd->_lpData = tmpRD;

		osg::Vec3d pos = RadarManager::Instance()->getGpsReader()->getCurPos();//获取当前gps数据
		float curLen = RadarManager::Instance()->getGpsReader()->getCurLen() + _lastLen;//获取当前总数据gps距离长度

		//补全雷达数据
		tmpRD->setPosition( pos );
		tmpRD->setTime( time(NULL)/*GetTickCount()*/ );
		tmpRD->setLen( curLen );

		//判断该数据是否被注释
		if ( _setMark[index] ){
			tmpRD->setMark( true );
			_setMark[index] = false;
		}else{
			tmpRD->setMark( false );
		}
		/*if(index == 0){
			_recordTotalWheelCount = rd->getRealWheelCount();
			_realTotalWheelCount = _realTotalWheelCount+1;
		}*/
		m_arrnRecordTotalWheelCount[index] = rd->getRealWheelCount();
		m_arrnRealTotalWheelCount[index] = m_arrnRealTotalWheelCount[index]+1;

		((SaveFileDataThread*)_lpThreadSaveFile[index])->addData(lpCmd.get());
	}else if (m_iRadarWorkType == RADAR_WORK_TYPE_EIGHT && m_iSaveOracle == 0){//网口模式且以文件保存
		//int tempArr[MAX_CHANNEL]={RadarManager::Instance()->int_Check_Channel1,RadarManager::Instance()->int_Check_Channel2,RadarManager::Instance()->int_Check_Channel3,RadarManager::Instance()->int_Check_Channel4,RadarManager::Instance()->int_Check_Channel5,RadarManager::Instance()->int_Check_Channel6,RadarManager::Instance()->int_Check_Channel7,RadarManager::Instance()->int_Check_Channel8};
		/*
		int tempArr[MAX_CHANNEL];
		for (int i=0;i<MAX_CHANNEL;i++){
			tempArr[i]=RadarManager::Instance()->int_Check_Channel[i];
		}*/
		//if (tempArr[index]==1){
		if (RadarManager::Instance()->int_Check_Channel[index]==1){
			if ( index >= CHANNELCOUNT ){
				return;
			}
			osg::ref_ptr<SaveRadarFileDataCmd> lpCmd = new SaveRadarFileDataCmd;//线程指令的指针
			RadarData *tmpRD = rd;
			lpCmd->_lpData = tmpRD;
			if(m_arrnRecordTotalWheelCount[index] == rd->getRealWheelCount()){
				return;
			}
			/*if(index == 0){
				_recordTotalWheelCount = rd->getRealWheelCount();
				_realTotalWheelCount = _realTotalWheelCount+1;
			}*/
			m_arrnRecordTotalWheelCount[index] = rd->getRealWheelCount();
			m_arrnRealTotalWheelCount[index] = m_arrnRealTotalWheelCount[index]+1;
			if(m_arrnRealTotalWheelCount[index]==1){//不要第一个数据，第一个数据总是上一次采集的，原因不明
				return;
			}
			//处理上传时的分段判断
			if(m_nUpload==1){
				//if(index==0&&_realTotalWheelCount>m_nThreshold&&(_realTotalWheelCount-9)%m_nThreshold==0){
				int currentSecond = clock();
				if(index==0&&(currentSecond-m_nLastSecond)>m_nTimeThreshold&&(m_arrnRealTotalWheelCount[0]-m_vecUploadWheelCount[m_vecUploadWheelCount.size()-1])>500){
					for(int i=0;i<m_nChannelCountForUpload;i++){
						m_SGYWriter[i]->setDataCountThreshold(m_arrnRealTotalWheelCount[0]);//后面的索引是0，因为进这里的判断是index=0，所以其他几个没变完
						m_SGYWriter[i]->setIsChange(true);
					}
					/*
					if(_tempWheelCount!=0){
						_lastWheelCount=_tempWheelCount;
					}
					_tempWheelCount=_realTotalWheelCount;*/
					m_vecUploadWheelCount.push_back(m_arrnRealTotalWheelCount[0]);
					std::vector<bool> vecTemp;//用来判断是否上传成功的
					for(int i=0;i<15;i++){//一共15项 txt cor csv rad*6 rd3*6
						vecTemp.push_back(true);
					}
					m_vecVecUploadSuccessFlag.push_back(vecTemp);
					((SaveOnlineThread*)_lpThreadSaveOnline)->addPart(m_nRoadPart);
					/*
					if(m_nUpload==1){
						((SaveOnlineThread*)_lpThreadSaveOnline)->addPart(m_nRoadPart);
					}
					*/
					//m_bIsAdd=true;
					//m_nLastSecond=currentSecond;
					m_nLastSecond=m_nLastSecond+m_nTimeThreshold;
					m_nRoadPart=m_nRoadPart+1;
				}
				/*
				if(index==0&&m_bIsAdd==true&&(currentSecond-m_nLastSecond)>1){//延时2秒 避免sgywriter那边没换完文件写导致不可读
					((SaveOnlineThread*)_lpThreadSaveOnline)->addPart(m_nRoadPart-1);//之前已经更新了 所以要用回之前的
					m_bIsAdd=false;
				}*/
			}

			/*
			if(_realTotalWheelCount==2&&tmpRD->getPrec()==_currentWheelCount){
				//_realTotalWheelCount = _realTotalWheelCount-1;
				return;
			}
			if(index == 0){
				_currentWheelCount = tmpRD->getPrec();//获取该当前道数数据的测量轮道数
			}*/

			//osg::Vec3d pos = RadarManager::Instance()->getGpsReader()->getCurPos();//获取当前gps数据
			osg::Vec3d pos;
			std::vector< std::string > vecStrGPGGA;
			RadarManager::Instance()->getGpsReader()->getCurrentGpsImformation(pos,vecStrGPGGA);
			float curLen = RadarManager::Instance()->getGpsReader()->getCurLen() + _lastLen;//获取当前总数据距离长度

			//上传模式下实时上传gpsData
			if(m_nUpload==1){
				if(index == 0){
					if(curLen>m_fLastGpsLen||m_fLastGpsLen==0){
						/*std::vector< std::string > strGpsData(RadarManager::Instance()->getGpsReader()->getCurrentGpsDataBuff());
						if (!( _currentGps.size()!=15 || _currentGps[14].size()<3 || _currentGps[2]=="" || _currentGps[4]=="" )){//第二个判断是确保最后一项有换行符 三四判断经纬度不为零
							std::string strUploadGpsData=
							((UploadGpsOnlineThread*)_lpThreadUploadGps)->addData(pos);
						}*/
						if(m_fLastPostedGpsLen!=m_fLastGpsLen){
							time_t mytime = time(NULL);
							int ntemp=mytime;
							if(ntemp>m_nGpsTimeStamp||m_fLastGpsLen==0){
								m_nGpsTimeStamp=ntemp;
								((UploadGpsOnlineThread*)_lpThreadUploadGps)->addData(m_pLastPos);
							}
						}
						m_fLastPostedGpsLen=m_fLastGpsLen;
						m_pLastPos=pos;
						m_fLastGpsLen=curLen;
					}
				}
			}
			
			
			//补全雷达数据
			tmpRD->setPosition( pos );
			tmpRD->setGPGGA( vecStrGPGGA );
			tmpRD->setTime( time(NULL)/*GetTickCount()*/ );
			tmpRD->setLen( curLen );
			
			
			//判断该数据是否被注释
			if ( _setMark[index] ){
				tmpRD->setMark( true );
				_setMark[index] = false;
			}else{
				tmpRD->setMark( false );
			}

			//按index分配到对应的雷达数据储存线程中
			((SaveFileDataThread*)_lpThreadSaveFile[index])->addData(lpCmd.get());
		}
	}else if(m_iSaveOracle == 1){//数据库模式
		if ( !RadarManager::Instance()->dbOpen() ){
			return;
		}
		if ( index >= CHANNELCOUNT ){
			return;
		}
		_maxID ++;
		rd->setID(_maxID );
		osg::ref_ptr<SaveRadarDataCmd> lpCmd = new SaveRadarDataCmd;
		RadarData *tmpRD = rd;

		lpCmd->_lpData = tmpRD;

		osg::Vec3d pos = RadarManager::Instance()->getGpsReader()->getCurPos();

		float curLen = RadarManager::Instance()->getGpsReader()->getCurLen() + _lastLen;

		tmpRD->setPosition( pos );
		tmpRD->setTime( time(NULL)/*GetTickCount()*/ );
		tmpRD->setLen( curLen );

		if ( _setMark[index] ){
			tmpRD->setMark( true );
			_setMark[index] = false;
		}else{
			tmpRD->setMark( false );
		}

		SaveDataThread *lpSaveThread = dynamic_cast<SaveDataThread*>(getAvaliadThread());
		if ( lpSaveThread ){
			lpCmd->_dbHelp = &lpSaveThread->_dbHelp;
			lpCmd->_lpTab = &lpSaveThread->_dataTab;
			lpSaveThread->addData( lpCmd.get() );
		}
	}
}

//20200306 hjl 用于测量轮回滚删除数据
void MeasureProject::deleteData(int index )
{
	if ((m_iRadarWorkType == RADAR_WORK_TYPE_ONE_USB || m_iRadarWorkType == RADAR_WORK_TYPE_DOUBLE_USB  || m_iRadarWorkType == RADAR_WORK_TYPE_DOUBLE_USB_OLD 
		|| m_iRadarWorkType == RADAR_WORK_TYPE_FOUR_USB) && m_iSaveOracle == 0)//非网口模式且以文件保存
	{
		((SaveFileDataThread*)_lpThreadSaveFile[index])->deleteData();
	}
}

MeasureProject::ReceiveQueue MeasureProject::getDataQueue()
{
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_dataMutex);
	return _dataQueue;
}

void MeasureProject::setProjectName( std::string const& name )
{
	_projectName = name;

}
std::string MeasureProject::getProjectName()const
{
	return _projectName;
}
void MeasureProject::setProjectTab( std::string const& tabName )
{
	_projectTab = tabName;
}
std::string MeasureProject::getProjectTab()const
{
	return _projectTab;
}

void MeasureProject::setProjectRow( DBRow *lpRow )
{
	_projectRow = lpRow;
	ProjectNameTab::ProjectNameRow *lpProjectRow = dynamic_cast<ProjectNameTab::ProjectNameRow *>(_projectRow.get());
	if ( lpProjectRow ){
		//_lastLen = lpProjectRow->_curLen;
		_lastLen = 0;
	}else{
		_lastLen = 0;
	}
	for(int i=0;i<CHANNELCOUNT;i++){
		m_arrnRecordTotalWheelCount[i]=0;
		m_arrnRealTotalWheelCount[i]=0;
	}
	m_vecUploadWheelCount.clear();
	m_vecUploadWheelCount.push_back(0);
	m_vecVecUploadSuccessFlag.clear();
}

void MeasureProject::updateProjectInfo( float curLen )
{
	ProjectNameTab::ProjectNameRow *lpRow = dynamic_cast<ProjectNameTab::ProjectNameRow *>(_projectRow.get());
	if ( lpRow ){
		lpRow->_curLen = curLen;
		osg::ref_ptr<UpdateProjectInfoCmd> lpCmd = new UpdateProjectInfoCmd( lpRow );
		addSaveCmd( lpCmd.get() );	
	}
}

void MeasureProject::setMark()
{
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_dataMutex);
	for ( int i = 0; i < CHANNELCOUNT; i ++ ){
		_setMark[i] = true;
	}
}

OpenThreads::Thread *MeasureProject::getAvaliadThread()
{
	//OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_threadMutex );

	for ( ThreadVector::iterator it = _threadVector.begin(); it != _threadVector.end(); it++ )
	{
		SaveDataThread *lpThread = dynamic_cast<SaveDataThread*>((*it));
		if ( !lpThread )
		{
			continue;
		}
		if ( !lpThread->isFull() )
		{
			return lpThread;
		}
	}

	SaveDataThread *lpThread = new SaveDataThread;
	lpThread->_projectTab = _projectTab;
	_threadVector.push_back( lpThread );

	if (m_iSaveOracle==1)
	{
		ConfigureSet *lpCS = RadarManager::Instance()->getConfigureSet();
		if ( !lpThread->init(lpCS->get("db","dbsource").c_str()
			, lpCS->get("db", "dbname").c_str()
			, lpCS->get("db", "user").c_str()
			, lpCS->get("db", "pwd").c_str()
			, _projectTab
			) )
		{
			return NULL;
		}

	}
	
	if ( !lpThread->isRunning() )
	{
		lpThread->start();
	}

	return lpThread;
}



void MeasureProject::SetSaveOracleOrFile( int iRadarWorkType,int isaveOracle,CString strPath )
{
	m_iRadarWorkType = iRadarWorkType;
	m_iSaveOracle = isaveOracle;
	m_strSavePath = strPath;

	//存文件
	if (m_iSaveOracle == 0 ){
		switch (m_iRadarWorkType){
			case RADAR_WORK_TYPE_FOUR_USB:
				{
					m_bIsBegin = true;
					_maxID = 0;

					CString cstrCreateTime;
					USES_CONVERSION;

					std::string strPathOrigin = W2A( strPath );
					strPathOrigin += _projectName;
					strPathOrigin += _projectTab;
					
					std::string strPathSGY[FOUR_CHANNEL];
					for(int i=0;i<FOUR_CHANNEL;i++){
						strPathSGY[i]+=strPathOrigin;
						strPathSGY[i]+="_";
						strPathSGY[i]+=+i;
						strPathSGY[i]+=".SGY";
					}
					std::string strPathGPS;
					strPathGPS+=strPathOrigin;
					strPathGPS+=".txt";

					ProjectNameTab::ProjectNameRow *lpProjectRow = dynamic_cast<ProjectNameTab::ProjectNameRow *>(_projectRow.get());
					if (NULL == lpProjectRow){
						::AfxMessageBox(L"表头信息不正确，不能保存ＳＧＹ文件！");
						return;
					}
					
					for(int i=0;i<FOUR_CHANNEL;i++){
						if (_lpThreadSaveFile[i]){
							delete _lpThreadSaveFile[i];
							_lpThreadSaveFile[i] = NULL;
						}
						_lpThreadSaveFile[i] = new SaveFileDataThread;
						if (m_SGYWriter[i]){
							delete m_SGYWriter[i];
							m_SGYWriter[i] = NULL;
						}
						m_SGYWriter[i] = new SGYWriter;
						m_SGYWriter[i]->openSGY(strPathSGY[i],lpProjectRow->_paramXML ,i);
						if ( RadarManager::Instance()->getExportGpsPos() ){
							if(useIt(i)){
								//m_SGYWriter[i]->openGPS(strPathGPS);
							}
						}
						m_SGYWriter[i]->writeHead(1,_projectTab);
						((SaveFileDataThread*)_lpThreadSaveFile[i])->_SGYWriter = m_SGYWriter[i];
						_lpThreadSaveFile[i]->start();
					}
				}
				break;
			case RADAR_WORK_TYPE_EIGHT:
				if(m_nUpload==1){
					m_bIsAdd = false;
					m_bIsBegin = true;
					_maxID = 0;

					//getOnlinePath();
					writeTxtFile();
					//writeEndFile();
	
					while(m_nInitOpr==0){
						initOprTab();
					}
					//setStartTimeTab();


					CString cstrCreateTime;
					USES_CONVERSION;

					std::string strPathOrigin = W2A( strPath );
					_documentPathLength = strPathOrigin.length();
					strPathOrigin += _projectName;
					strPathOrigin += _projectTab;
					_namePathLength = strPathOrigin.length()-_documentPathLength;
					if (0 != access(strPathOrigin.c_str(), 0)){
						// if this folder not exist, create a new one.
						mkdir(strPathOrigin.c_str());   // 返回 0 表示创建成功，-1 表示失败
					}

					m_fpLog = fopen( (strPathOrigin+"\\log.txt").c_str(), "a" );
					m_fpLogForRepairment = fopen( (strPathOrigin+"\\logForRepairment.txt").c_str(), "a" );
					strPathOrigin+="\\";
					strPathOrigin += _projectName;
					strPathOrigin += _projectTab;
					strPathOrigin+="_1";
					if (0 != access(strPathOrigin.c_str(), 0)){
						// if this folder not exist, create a new one.
						mkdir(strPathOrigin.c_str());   // 返回 0 表示创建成功，-1 表示失败
					}
					strPathOrigin+="\\";
					strPathOrigin += _projectName;
					strPathOrigin += _projectTab;
					strPathOrigin+="_1";

					std::string strPathOriginForHide = W2A( strPath );
					strPathOriginForHide += _projectName;
					strPathOriginForHide += _projectTab;
					strPathOriginForHide+="\\";
					strPathOriginForHide += _projectName;
					strPathOriginForHide += _projectTab;
					strPathOriginForHide+="_1";
					strPathOriginForHide += "\\moreChannels";
					if (0 != access(strPathOriginForHide.c_str(), 0)){
						// if this folder not exist, create a new one.
						mkdir(strPathOriginForHide.c_str());   // 返回 0 表示创建成功，-1 表示失败
					}
					strPathOriginForHide += "\\";
					strPathOriginForHide += _projectName;
					strPathOriginForHide += _projectTab;
					strPathOriginForHide += "_1";
					
					
					/*
					//char strPathSGY[CHANNELCOUNT][300];//文件路径字节量
					char **strPathSGY = new char*[m_nChannelCountForUpload];
					for(int i=0;i<m_nChannelCountForUpload;i++){
						strPathSGY[i] = new char[300];
					}
					for(int i=0;i<m_nChannelCountForUpload;i++){//使后面的strcat生效
						strPathSGY[i][0]='\0';
					}*/
					for(int i=0;i<m_nChannelCountForUpload;i++){//使后面的strcat生效
						_strLocalRadarDataPath[i][0]='\0';
					}
					
					for(int i=0;i<m_nChannelCountForUpload;i++){
						//strcat(strPathSGY[i],strPathOrigin.c_str());
						strcat(_strLocalRadarDataPath[i],strPathOrigin.c_str());
						/*
						if(i<12){
							strcat(_strLocalRadarDataPath[i],strPathOrigin.c_str());
						}else{
							strcat(_strLocalRadarDataPath[i],strPathOriginForHide.c_str());
						}*/
						strcat(_strLocalRadarDataPath[i],"_");
					
						//int转char*
						stringstream ssTemp;
						//ssTemp << (RadarManager::Instance()->originalIndexToRecordIndex(i)+1);
						ssTemp << (i+1);
						string strTemp = ssTemp.str();
						char* cIndex = (char*) strTemp.c_str();
						
						strcat(_strLocalRadarDataPath[i],cIndex);//(char*)(48+ i) int i 转成char*
						strcat(_strLocalRadarDataPath[i],".rd3");
						//strcat(_strLocalRadarDataPath[i],".gdd");
					}

					for(int i=0;i<m_nChannelCountForUpload;i++){//字符串结尾补个终止符
						_strLocalRadarDataPath[i][strPathOrigin.length()+6]='\0';
					}

					_strLocalGPSCorPath ="";
					_strLocalGPSCorPath += strPathOrigin;
					_strLocalGPSCorPath+="gps.cor";

					_strLocalGPSCorForUploadPath ="";
					_strLocalGPSCorForUploadPath += strPathOrigin;
					_strLocalGPSCorForUploadPath+="gpsForUpload.cor";
					
					_strLocalGPSCsvPath ="";
					_strLocalGPSCsvPath += strPathOrigin;
					_strLocalGPSCsvPath+=".csv";

					std::string strPathKml = W2A( strPath );
					strPathKml+="kml";
					if (0 != access(strPathKml.c_str(), 0)){
						// if this folder not exist, create a new one.
						mkdir(strPathKml.c_str());   // 返回 0 表示创建成功，-1 表示失败
					}
					strPathKml+="\\";
					strPathKml += _projectName;
					strPathKml += _projectTab;
					strPathKml+=".kml";
						
					ProjectNameTab::ProjectNameRow *lpProjectRow = dynamic_cast<ProjectNameTab::ProjectNameRow *>(_projectRow.get());
					if (NULL == lpProjectRow){
						::AfxMessageBox(L"表头信息不正确，不能保存ＳＧＹ文件！");
						return;
					}

					for(int i=0;i<m_nChannelCountForUpload;i++){
						if(RadarManager::Instance()->int_Check_Channel[i] == 1 ){
							if (_lpThreadSaveFile[i]){
								delete _lpThreadSaveFile[i];
								_lpThreadSaveFile[i] = NULL;
							}
							_lpThreadSaveFile[i] = new SaveFileDataThread;
							if (m_SGYWriter[i])
							{
								delete m_SGYWriter[i];
								m_SGYWriter[i] = NULL;
							}
							m_SGYWriter[i] = new SGYWriter;
							m_SGYWriter[i]->setUploadFlag(m_nUpload);
							m_SGYWriter[i]->openSGYForAutomation(_strLocalRadarDataPath[i],lpProjectRow->_paramXML,i,_documentPathLength,_namePathLength);
							if ( RadarManager::Instance()->getExportGpsPos() ){
								if(useIt(i)){
									m_SGYWriter[i]->openGPS(_strLocalGPSCorPath, _strLocalGPSCorForUploadPath, _strLocalGPSCsvPath);
								}
							}
							if ( RadarManager::Instance()->getExportKml() ){
								if(useIt(i)){
									m_SGYWriter[i]->openKml(strPathKml);
									m_SGYWriter[i]->writeKmlHead();
								}
							}
							//m_SGYWriter[i]->writeHead(1,_projectTab);//20210618hjl
							((SaveFileDataThread*)_lpThreadSaveFile[i])->_SGYWriter = m_SGYWriter[i];
							_lpThreadSaveFile[i]->start();
						}
					}

					m_nLastSecond = clock();//记录开始时间 

					if (_lpThreadSaveOnline){
						delete _lpThreadSaveOnline;
						_lpThreadSaveOnline = NULL;
					}
					_lpThreadSaveOnline = new SaveOnlineThread;
					((SaveOnlineThread*)_lpThreadSaveOnline)->_MeasureProject = this;
					_lpThreadSaveOnline->start();

					if (_lpThreadSaveOnlineForRepairment){
						delete _lpThreadSaveOnlineForRepairment;
						_lpThreadSaveOnlineForRepairment = NULL;
					}
					_lpThreadSaveOnlineForRepairment = new SaveOnlineForRepairmentThread;
					((SaveOnlineForRepairmentThread*)_lpThreadSaveOnlineForRepairment)->_MeasureProject = this;
					_lpThreadSaveOnlineForRepairment->start();

					if (_lpThreadUploadGps){
						delete _lpThreadUploadGps;
						_lpThreadUploadGps = NULL;
					}
					_lpThreadUploadGps = new UploadGpsOnlineThread;
					((UploadGpsOnlineThread*)_lpThreadUploadGps)->_MeasureProject = this;
					_lpThreadUploadGps->start();
				}else if(m_nUpload==0){
					m_bIsBegin = true;
					_maxID = 0;

					writeTxtFile();

					CString cstrCreateTime;
					USES_CONVERSION;
					/*
					std::string strPathOrigin = W2A( strPath );
					strPathOrigin += _projectName;
					strPathOrigin += _projectTab;
					if (0 != access(strPathOrigin.c_str(), 0)){
						// if this folder not exist, create a new one.
						mkdir(strPathOrigin.c_str());   // 返回 0 表示创建成功，-1 表示失败
					}
					strPathOrigin+="\\";
					strPathOrigin += _projectName;
					strPathOrigin += _projectTab;
					*/
					std::string strPathOrigin = W2A( m_strSavePath );
					strPathOrigin += _projectName;
					strPathOrigin += _projectTab;
					if (0 != access(strPathOrigin.c_str(), 0)){
						// if this folder not exist, create a new one.
						mkdir(strPathOrigin.c_str());   // 返回 0 表示创建成功，-1 表示失败
					}
					strPathOrigin += "\\";
					strPathOrigin += _projectName;
					strPathOrigin += _projectTab;

					//m_fpLog=fopen( (strPathOrigin+"\\log.txt").c_str(), "a" );

					std::string strPathOriginForHide = W2A( m_strSavePath );
					strPathOriginForHide += _projectName;
					strPathOriginForHide += _projectTab;
					strPathOriginForHide+="\\moreChannels";
					if (0 != access(strPathOriginForHide.c_str(), 0)){
						// if this folder not exist, create a new one.
						mkdir(strPathOriginForHide.c_str());   // 返回 0 表示创建成功，-1 表示失败
					}
					strPathOriginForHide += "\\";
					strPathOriginForHide += _projectName;
					strPathOriginForHide += _projectTab;
					

					char strPathSGY[CHANNELCOUNT][300];//文件路径字节量
					for(int i=0;i<CHANNELCOUNT;i++){
						strPathSGY[i][0]='\0';
					}

					for(int i=0;i<CHANNELCOUNT;i++){
						if(i<12){
							strcat(strPathSGY[i],strPathOrigin.c_str());
						}else{
							strcat(strPathSGY[i],strPathOriginForHide.c_str());
						}
						strcat(strPathSGY[i],"_");
					
						//int转char*
						stringstream ssTemp;
						//ssTemp << (RadarManager::Instance()->originalIndexToRecordIndex(i)+1);
						ssTemp << (i+1);
						string strTemp = ssTemp.str();
						char* cIndex = (char*) strTemp.c_str();
						
						strcat(strPathSGY[i],cIndex);//(char*)(48+ i) int i 转成char*
						strcat(strPathSGY[i],".rd3");
					}

					_strLocalGPSCorPath ="";
					_strLocalGPSCorPath += strPathOrigin;
					_strLocalGPSCorPath+=".cor";

					_strLocalGPSCsvPath ="";
					_strLocalGPSCsvPath += strPathOrigin;
					_strLocalGPSCsvPath+=".csv";

					std::string strPathKml = W2A( strPath );
					strPathKml+="kml";
					if (0 != access(strPathKml.c_str(), 0)){
						// if this folder not exist, create a new one.
						mkdir(strPathKml.c_str());   // 返回 0 表示创建成功，-1 表示失败
					}
					strPathKml+="\\";
					strPathKml += _projectName;
					strPathKml += _projectTab;
					strPathKml+=".kml";
						
					ProjectNameTab::ProjectNameRow *lpProjectRow = dynamic_cast<ProjectNameTab::ProjectNameRow *>(_projectRow.get());
					if (NULL == lpProjectRow){
						::AfxMessageBox(L"表头信息不正确，不能保存ＳＧＹ文件！");
						return;
					}

					for(int i=0;i<CHANNELCOUNT;i++){
						if(RadarManager::Instance()->int_Check_Channel[i] == 1 ){
							if (_lpThreadSaveFile[i]){
								delete _lpThreadSaveFile[i];
								_lpThreadSaveFile[i] = NULL;
							}
							_lpThreadSaveFile[i] = new SaveFileDataThread;
							if (m_SGYWriter[i])
							{
								delete m_SGYWriter[i];
								m_SGYWriter[i] = NULL;
							}
							m_SGYWriter[i] = new SGYWriter;
							m_SGYWriter[i]->openSGY(strPathSGY[i],lpProjectRow->_paramXML ,i);
							if ( RadarManager::Instance()->getExportGpsPos() ){
								if(useIt(i)){
									//m_SGYWriter[i]->openGPS(strPathGPS);
									m_SGYWriter[i]->openGPS(_strLocalGPSCorPath, "", _strLocalGPSCsvPath);
								}
							}
							if ( RadarManager::Instance()->getExportKml() ){
								if(useIt(i)){
									m_SGYWriter[i]->openKml(strPathKml);
									m_SGYWriter[i]->writeKmlHead();
								}
							}
							//m_SGYWriter[i]->writeHead(1,_projectTab);//20210618hjl
							((SaveFileDataThread*)_lpThreadSaveFile[i])->_SGYWriter = m_SGYWriter[i];
							_lpThreadSaveFile[i]->start();
						}
					}
				}
				break;
			default:
				{
					m_bIsBegin = true;
					_maxID = 0;

					CString cstrCreateTime;
					USES_CONVERSION;

					std::string strPathOrigin = W2A( strPath );
					strPathOrigin += _projectName;
					strPathOrigin += _projectTab;
					
					std::string strPathSGY[TWO_CHANNEL];
					for(int i=0;i<TWO_CHANNEL;i++){
						strPathSGY[i]+=strPathOrigin;
						strPathSGY[i]+="_";
						strPathSGY[i]+=+i;
						strPathSGY[i]+=".SGY";
					}
					std::string strPathGPS;
					strPathGPS+=strPathOrigin;
					strPathGPS+=".txt";

					ProjectNameTab::ProjectNameRow *lpProjectRow = dynamic_cast<ProjectNameTab::ProjectNameRow *>(_projectRow.get());
					/*if (NULL == lpProjectRow)
					{
						::AfxMessageBox(L"表头信息不正确，不能保存ＳＧＹ文件！");
						return;
					}*/
					
					for(int i=0;i<TWO_CHANNEL;i++){
						if (_lpThreadSaveFile[i]){
							delete _lpThreadSaveFile[i];
							_lpThreadSaveFile[i] = NULL;
						}
						_lpThreadSaveFile[i] = new SaveFileDataThread;
						
						if (m_SGYWriter[i]){
							delete m_SGYWriter[i];
							m_SGYWriter[i] = NULL;
						}
						m_SGYWriter[i] = new SGYWriter;
						m_SGYWriter[i]->openSGY(strPathSGY[i],lpProjectRow->_paramXML ,i);
						if ( RadarManager::Instance()->getExportGpsPos() ){//生成gps信息文件
							if(useIt(i)){
								//m_SGYWriter[i]->openGPS(strPathGPS);
							}
						}
						m_SGYWriter[i]->writeHead(1,_projectTab);
						((SaveFileDataThread*)_lpThreadSaveFile[i])->_SGYWriter = m_SGYWriter[i];
						_lpThreadSaveFile[i]->start();
					}
				}
				break;
		}
	}else if (m_iSaveOracle == 1){//存数据库

		TabMaxIDTab tab;

		DataTemplateTab dataTab;
		dataTab.setProjectName( _projectTab );

		tab.setTabName( dataTab.getProjectName() );
		std::stringstream ss;
		ss << tab.getSelectSql();

		_maxID  = tab.getMaxID( RadarManager::Instance()->getDBHelp(), ss.str());
	}
}

void MeasureProject::writeRadFile(){
	USES_CONVERSION;

	std::string strPathOrigin = W2A( m_strSavePath );
	strPathOrigin += _projectName;
	strPathOrigin += _projectTab;
	strPathOrigin += "\\";
	strPathOrigin += _projectName;
	strPathOrigin += _projectTab;

	std::string strPathOriginForHide = W2A( m_strSavePath );
	strPathOriginForHide += _projectName;
	strPathOriginForHide += _projectTab;
	strPathOriginForHide+="\\moreChannels";
	if (0 != access(strPathOriginForHide.c_str(), 0)){
		// if this folder not exist, create a new one.
		mkdir(strPathOriginForHide.c_str());   // 返回 0 表示创建成功，-1 表示失败
	}
	strPathOriginForHide+="\\";
	strPathOriginForHide += _projectName;
	strPathOriginForHide += _projectTab;
	
	char strPathRad[CHANNELCOUNT][300];
	for(int i=0;i<CHANNELCOUNT;i++){
		strPathRad[i][0]='\0';
	}

	for(int i=0;i<CHANNELCOUNT;i++){
		if(i<12){
			strcat(strPathRad[i],strPathOrigin.c_str());
		}else{
			strcat(strPathRad[i],strPathOriginForHide.c_str());
		}
		strcat(strPathRad[i],"_");
	
		//int转char*
		stringstream ssTemp;
		ssTemp << (i+1);
		string strTemp = ssTemp.str();
		char* cIndex = (char*) strTemp.c_str();
		
		strcat(strPathRad[i],cIndex);//(char*)(48+ i) int i 转成char*
		strcat(strPathRad[i],".rad");
	}

	ProjectNameTab::ProjectNameRow *lpProjectRow = dynamic_cast<ProjectNameTab::ProjectNameRow *>(_projectRow.get());
	ConfigureSet set;
	set.fromGroupXML( lpProjectRow->_paramXML );

	int sampleNum = RadarManager::Instance()->getSampCount( atoi ( set.get("radar", "sample").c_str()) );
	float sampleRate = RadarManager::Instance()->getSampRatio( atoi ( set.get("radar", "sampleratio" ).c_str() ), 0 );
	float distanceInterval = RadarManager::Instance()->GetTrueAccuracyFromPreclenAndPrecindex(atoi ( set.get("radar", "preclen").c_str() ),(float)atoi( set.get("radar", "precision").c_str() )) / 100.0;
	
	for(int i=0;i<CHANNELCOUNT;i++){
		ofstream ofile; 
		ofile.open(strPathRad[i]);  

		ofile<<"SAMPLES:"<<sampleNum<<endl;
		ofile<<"FREQUENCY:"<<sampleRate*1000<<endl;
		ofile<<"FREQUENCY STEPS:"<<1<<endl;
		ofile<<"SIGNAL POSITION:"<<0.000000<<endl;
		ofile<<"RAW SIGNAL POSITION:"<<1<<endl;
		ofile<<"DISTANCE FLAG:"<<0<<endl;
		ofile<<"TIME FLAG:"<<1<<endl;
		ofile<<"PROGRAM FLAG:"<<0<<endl;
		ofile<<"EXTERNAL FLAG:"<<0<<endl;
		ofile<<"TIME INTERVAL:"<<100.000000<<endl;
		ofile<<"DISTANCE INTERVAL:"<<distanceInterval<<endl;
		ofile<<"OPERATOR:"<<0<<endl;
		ofile<<"CUSTOMER:"<<0<<endl;
		ofile<<"SITE:"<<0<<endl;
		ofile<<"ANTENNAS:"<<"unknow"<<endl;
		ofile<<"ANTENNA ORIENTATION:"<<"NOT VALID FIELD"<<endl;
		ofile<<"ANTENNA SEPARATION:"<<0.012000<<endl;
		ofile<<"COMMENT:"<<"Saved from matlab"<<endl;
		ofile<<"TIMEWINDOW:"<<1.0 / sampleRate * sampleNum<<endl;
		ofile<<"STACKS:"<<1<<endl;
		ofile<<"STACK EXPONENT:"<<0<<endl;
		ofile<<"STACKING TIME:"<<0.015000<<endl;
		ofile<<"LAST TRACE:"<<m_arrnRealTotalWheelCount[0]<<endl;
		ofile<<"STOP POSITION:"<<distanceInterval*m_arrnRealTotalWheelCount[0]<<endl;
		ofile<<"SYSTEM CALIBRATION:"<<0.001000<<endl;
		ofile<<"START POSITION:"<<0.000000<<endl;
		ofile<<"SHORT FLAG:"<<1<<endl;
		ofile<<"INTERMEDIATE FLAG:"<<0<<endl;
		ofile<<"LONG FLAG:"<<0<<endl;
		ofile<<"PREPROCESSING:"<<0<<endl;
		ofile<<"HIGH:"<<5<<endl;
		ofile<<"LOW:"<<15<<endl;
		ofile<<"FIXED INCREMENT:"<<0.300000<<endl;
		ofile<<"FIXED MOVES UP:"<<0<<endl;
		ofile<<"FIXED MOVES DOWN:"<<1<<endl;
		ofile<<"FIXED POSITION:"<<0.000000<<endl;
		ofile<<"ARRAY:"<<83<<endl;
		ofile<<"ANTENNA FREQUENCY:"<<500<<endl;

		ofile.close();//关闭文件
	}
}

void MeasureProject::writeRadFileForPart( int roadPart ){
	USES_CONVERSION;
	
	stringstream ssRoadPart;
	ssRoadPart << roadPart;
	string strRoadPart = ssRoadPart.str();
	
	std::string strPathOrigin = W2A( m_strSavePath );
	strPathOrigin += _projectName;
	strPathOrigin += _projectTab;
	strPathOrigin += "\\";
	strPathOrigin += _projectName;
	strPathOrigin += _projectTab;
	strPathOrigin += "_";
	strPathOrigin += strRoadPart;
	strPathOrigin += "\\";
	strPathOrigin += _projectName;
	strPathOrigin += _projectTab;
	strPathOrigin += "_";
	strPathOrigin += strRoadPart;
	
	//char strPathRad[m_nChannelCountForUpload][300];
	char **strPathRad = new char* [m_nChannelCountForUpload];
	for(int i=0;i<m_nChannelCountForUpload;i++){
		strPathRad[i]=new char [300];
	}
	for(int i=0;i<m_nChannelCountForUpload;i++){
		strPathRad[i][0]='\0';
	}

	for(int i=0;i<m_nChannelCountForUpload;i++){
		strcat(strPathRad[i],strPathOrigin.c_str());
		strcat(strPathRad[i],"_");
	
		//int转char*
		stringstream ssTemp;
		ssTemp << (i+1);
		string strTemp = ssTemp.str();
		char* cIndex = (char*) strTemp.c_str();
		
		strcat(strPathRad[i],cIndex);//(char*)(48+ i) int i 转成char*
		strcat(strPathRad[i],".rad");
	}

	ProjectNameTab::ProjectNameRow *lpProjectRow = dynamic_cast<ProjectNameTab::ProjectNameRow *>(_projectRow.get());
	ConfigureSet set;
	set.fromGroupXML( lpProjectRow->_paramXML );
	int sampleNum = RadarManager::Instance()->getSampCount( atoi ( set.get("radar", "sample").c_str()) );
	float sampleRate = RadarManager::Instance()->getSampRatio( atoi ( set.get("radar", "sampleratio" ).c_str() ), 0 );
	float distanceInterval = RadarManager::Instance()->GetTrueAccuracyFromPreclenAndPrecindex(atoi ( set.get("radar", "preclen").c_str() ),(float)atoi( set.get("radar", "precision").c_str() )) / 100.0;
	int wheelCount;
	/*
	if(_realTotalWheelCount>=(roadPart*m_nThreshold)){
		wheelCount=m_nThreshold;
	}else{
		wheelCount=_realTotalWheelCount+m_nThreshold-roadPart*m_nThreshold;
	}*/
	/*
	if(roadPart>m_vecUploadWheelCount.size()-1){//最后一次的情况	
		wheelCount=_realTotalWheelCount-m_vecUploadWheelCount[roadPart];
	}else{
		wheelCount=m_vecUploadWheelCount[roadPart]-m_vecUploadWheelCount[roadPart-1];
	}*/
	wheelCount=m_vecUploadWheelCount[roadPart]-m_vecUploadWheelCount[roadPart-1];

	for(int i=0;i<m_nChannelCountForUpload;i++){
		ofstream ofile; 
		ofile.open(strPathRad[i]);  

		ofile<<"SAMPLES:"<<sampleNum<<endl;
		ofile<<"FREQUENCY:"<<sampleRate*1000<<endl;
		ofile<<"FREQUENCY STEPS:"<<1<<endl;
		ofile<<"SIGNAL POSITION:"<<0.000000<<endl;
		ofile<<"RAW SIGNAL POSITION:"<<1<<endl;
		ofile<<"DISTANCE FLAG:"<<0<<endl;
		ofile<<"TIME FLAG:"<<1<<endl;
		ofile<<"PROGRAM FLAG:"<<0<<endl;
		ofile<<"EXTERNAL FLAG:"<<0<<endl;
		ofile<<"TIME INTERVAL:"<<100.000000<<endl;
		ofile<<"DISTANCE INTERVAL:"<<distanceInterval<<endl;
		ofile<<"OPERATOR:"<<0<<endl;
		ofile<<"CUSTOMER:"<<0<<endl;
		ofile<<"SITE:"<<0<<endl;
		ofile<<"ANTENNAS:"<<"unknow"<<endl;
		ofile<<"ANTENNA ORIENTATION:"<<"NOT VALID FIELD"<<endl;
		ofile<<"ANTENNA SEPARATION:"<<0.012000<<endl;
		ofile<<"COMMENT:"<<"Saved from matlab"<<endl;
		ofile<<"TIMEWINDOW:"<<1.0 / sampleRate * sampleNum<<endl;
		ofile<<"STACKS:"<<1<<endl;
		ofile<<"STACK EXPONENT:"<<0<<endl;
		ofile<<"STACKING TIME:"<<0.015000<<endl;
		ofile<<"LAST TRACE:"<<wheelCount<<endl;
		ofile<<"STOP POSITION:"<<distanceInterval*wheelCount<<endl;
		ofile<<"SYSTEM CALIBRATION:"<<0.001000<<endl;
		ofile<<"START POSITION:"<<0.000000<<endl;
		ofile<<"SHORT FLAG:"<<1<<endl;
		ofile<<"INTERMEDIATE FLAG:"<<0<<endl;
		ofile<<"LONG FLAG:"<<0<<endl;
		ofile<<"PREPROCESSING:"<<0<<endl;
		ofile<<"HIGH:"<<5<<endl;
		ofile<<"LOW:"<<15<<endl;
		ofile<<"FIXED INCREMENT:"<<0.300000<<endl;
		ofile<<"FIXED MOVES UP:"<<0<<endl;
		ofile<<"FIXED MOVES DOWN:"<<1<<endl;
		ofile<<"FIXED POSITION:"<<0.000000<<endl;
		ofile<<"ARRAY:"<<83<<endl;
		ofile<<"ANTENNA FREQUENCY:"<<500<<endl;

		ofile.close();//关闭文件
	}
	for(int i=0;i<m_nChannelCountForUpload;i++){
		delete []strPathRad[i];
	}
	delete []strPathRad;
}


////void MeasureProject::uploadRadFileForGroup( int roadPart ){
////	USES_CONVERSION;
////	
////	stringstream ssRoadPart;
////	ssRoadPart << roadPart;
////	string strRoadPart = ssRoadPart.str();
////	
////	std::string strPathOrigin = W2A( m_strSavePath );
////	strPathOrigin += _projectName;
////	strPathOrigin += _projectTab;
////	strPathOrigin += "\\";
////	strPathOrigin += _projectName;
////	strPathOrigin += _projectTab;
////	strPathOrigin += "_";
////	strPathOrigin += strRoadPart;
////	strPathOrigin += "\\";
////	strPathOrigin += _projectName;
////	strPathOrigin += _projectTab;
////	strPathOrigin += "_";
////	strPathOrigin += strRoadPart;
////	
////	//char strPathRad[m_nChannelCount][300];
////	char **strPathRad = new char* [m_nChannelCount];
////	for(int i=0;i<m_nChannelCountForUpload;i++){
////		strPathRad[i]=new char [300];
////	}
////	for(int i=0;i<m_nChannelCountForUpload;i++){
////		strPathRad[i][0]='\0';
////	}
////
////	for(int i=0;i<m_nChannelCountForUpload;i++){
////		strcat(strPathRad[i],strPathOrigin.c_str());
////		strcat(strPathRad[i],"_");
////	
////		//int转char*
////		stringstream ssTemp;
////		ssTemp << (i+1);
////		string strTemp = ssTemp.str();
////		char* cIndex = (char*) strTemp.c_str();
////		
////		strcat(strPathRad[i],cIndex);//(char*)(48+ i) int i 转成char*
////		strcat(strPathRad[i],".rad");
////	}
////
////	ProjectNameTab::ProjectNameRow *lpProjectRow = dynamic_cast<ProjectNameTab::ProjectNameRow *>(_projectRow.get());
////	ConfigureSet set;
////	set.fromGroupXML( lpProjectRow->_paramXML );
////	int sampleNum = RadarManager::Instance()->getSampCount( atoi ( set.get("radar", "sample").c_str()) );
////	float sampleRate = RadarManager::Instance()->getSampRatio( atoi ( set.get("radar", "sampleratio" ).c_str() ), 0 );
////	float distanceInterval = RadarManager::Instance()->GetTrueAccuracyFromPreclenAndPrecindex(atoi ( set.get("radar", "preclen").c_str() ),(float)atoi( cfg->get("radar", "precision").c_str() )) / 100.0;
////	int wheelCount;
////	/*
////	if(_realTotalWheelCount>=(roadPart*m_nThreshold)){
////		wheelCount=m_nThreshold;
////	}else{
////		wheelCount=_realTotalWheelCount+m_nThreshold-roadPart*m_nThreshold;
////	}*/
////	/*
////	if(roadPart>m_vecUploadWheelCount.size()-1){//最后一次的情况	
////		wheelCount=_realTotalWheelCount-m_vecUploadWheelCount[roadPart];
////	}else{
////		wheelCount=m_vecUploadWheelCount[roadPart]-m_vecUploadWheelCount[roadPart-1];
////	}*/
////	wheelCount=m_vecUploadWheelCount[roadPart]-m_vecUploadWheelCount[roadPart-1];
////
////	for(int i=0;i<m_nChannelCountForUpload;i++){
////		ofstream ofile; 
////		ofile.open(strPathRad[i]);  
////
////		ofile<<"SAMPLES:"<<sampleNum<<endl;
////		ofile<<"FREQUENCY:"<<sampleRate*1000<<endl;
////		ofile<<"FREQUENCY STEPS:"<<1<<endl;
////		ofile<<"SIGNAL POSITION:"<<0.000000<<endl;
////		ofile<<"RAW SIGNAL POSITION:"<<1<<endl;
////		ofile<<"DISTANCE FLAG:"<<0<<endl;
////		ofile<<"TIME FLAG:"<<1<<endl;
////		ofile<<"PROGRAM FLAG:"<<0<<endl;
////		ofile<<"EXTERNAL FLAG:"<<0<<endl;
////		ofile<<"TIME INTERVAL:"<<100.000000<<endl;
////		ofile<<"DISTANCE INTERVAL:"<<distanceInterval<<endl;
////		ofile<<"OPERATOR:"<<0<<endl;
////		ofile<<"CUSTOMER:"<<0<<endl;
////		ofile<<"SITE:"<<0<<endl;
////		ofile<<"ANTENNAS:"<<"unknow"<<endl;
////		ofile<<"ANTENNA ORIENTATION:"<<"NOT VALID FIELD"<<endl;
////		ofile<<"ANTENNA SEPARATION:"<<0.012000<<endl;
////		ofile<<"COMMENT:"<<"Saved from matlab"<<endl;
////		ofile<<"TIMEWINDOW:"<<1.0 / sampleRate * sampleNum<<endl;
////		ofile<<"STACKS:"<<1<<endl;
////		ofile<<"STACK EXPONENT:"<<0<<endl;
////		ofile<<"STACKING TIME:"<<0.015000<<endl;
////		ofile<<"LAST TRACE:"<<wheelCount<<endl;
////		ofile<<"STOP POSITION:"<<distanceInterval*wheelCount<<endl;
////		ofile<<"SYSTEM CALIBRATION:"<<0.001000<<endl;
////		ofile<<"START POSITION:"<<0.000000<<endl;
////		ofile<<"SHORT FLAG:"<<1<<endl;
////		ofile<<"INTERMEDIATE FLAG:"<<0<<endl;
////		ofile<<"LONG FLAG:"<<0<<endl;
////		ofile<<"PREPROCESSING:"<<0<<endl;
////		ofile<<"HIGH:"<<5<<endl;
////		ofile<<"LOW:"<<15<<endl;
////		ofile<<"FIXED INCREMENT:"<<0.300000<<endl;
////		ofile<<"FIXED MOVES UP:"<<0<<endl;
////		ofile<<"FIXED MOVES DOWN:"<<1<<endl;
////		ofile<<"FIXED POSITION:"<<0.000000<<endl;
////		ofile<<"ARRAY:"<<83<<endl;
////		ofile<<"ANTENNA FREQUENCY:"<<500<<endl;
////
////		ofile.close();//关闭文件
////	}
////
////	ConfigureSet *cfg = RadarManager::Instance()->getConfigureSet();
////	string ftpServerIp = cfg->get("ftp", "serverIP") ;
////	int port = atoi( cfg->get("ftp", "port").c_str());
////	string account = cfg->get("ftp", "account") ;
////	string password = cfg->get("ftp", "password") ;
////	//创建ftp连接所需句柄
////	HINTERNET hint;
////	HINTERNET hftp;
////	hint = InternetOpen(0, INTERNET_OPEN_TYPE_PRECONFIG, 0, 0, 0);
////	if (hint==NULL)  {   
////		AfxMessageBox(L"没网。");
////		return;  
////	} 
////	hftp = InternetConnectA(hint, ftpServerIp.c_str(), port, account.c_str(), password.c_str(), INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
////	//hftp = InternetConnectA(hint, "192.168.1.108",21,"ftpTest","gdjw", INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
////	if (hftp==NULL) {
////		AfxMessageBox(L"连接ftp失败。");
////		return;
////	}
////	while(hftp==NULL){
////		hftp = InternetConnectA(hint, ftpServerIp.c_str(), port, account.c_str(), password.c_str(), INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
////		//hftp = InternetConnectA(hint, "192.168.1.108",21,"ftpTest","gdjw", INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
////	}
////	/*
////	std::string strOnlinePathOrigin = ".\\radarData\\";
////	//std::string strOnlinePathOrigin = "C:\\Users\\Administrator\\Desktop\\test\\";
////	strOnlinePathOrigin += _projectName;
////	strOnlinePathOrigin += _projectTab;
////	strOnlinePathOrigin += "\\";
////	if(_access(strOnlinePathOrigin.c_str(),0)==-1){
////		FtpCreateDirectoryA(hftp,strOnlinePathOrigin.c_str());
////	}
////	strOnlinePathOrigin += _projectName;
////	strOnlinePathOrigin += _projectTab;
////	strOnlinePathOrigin += "_";
////	strOnlinePathOrigin += strRoadPart;
////	strOnlinePathOrigin += "\\";
////	if(_access(strOnlinePathOrigin.c_str(),0)==-1){
////		FtpCreateDirectoryA(hftp,strOnlinePathOrigin.c_str());
////	}
////	strOnlinePathOrigin += _projectName;
////	strOnlinePathOrigin += _projectTab;
////	strOnlinePathOrigin += "_";
////	strOnlinePathOrigin += strRoadPart;
////
////	std::string strOnlinePathOriginForHide = ".\\radarData\\";
////	//std::string strOnlinePathOriginForHide = "C:\\Users\\Administrator\\Desktop\\test\\";
////	strOnlinePathOriginForHide += _projectName;
////	strOnlinePathOriginForHide += _projectTab;
////	strOnlinePathOriginForHide += "\\";
////	strOnlinePathOriginForHide += _projectName;
////	strOnlinePathOriginForHide += _projectTab;
////	strOnlinePathOriginForHide += "_";
////	strOnlinePathOriginForHide += strRoadPart;
////	strOnlinePathOriginForHide += "\\";
////	if(_access(strOnlinePathOriginForHide.c_str(),0)==-1){
////		FtpCreateDirectoryA(hftp,strOnlinePathOriginForHide.c_str());
////	}
////	strOnlinePathOriginForHide+="\\moreChannels\\";
////	if(_access(strOnlinePathOriginForHide.c_str(),0)==-1){
////		FtpCreateDirectoryA(hftp,strOnlinePathOriginForHide.c_str());
////	}
////	strOnlinePathOriginForHide += _projectName;
////	strOnlinePathOriginForHide += _projectTab;
////	strOnlinePathOriginForHide += "_";
////	strOnlinePathOriginForHide += strRoadPart;
////
////	char strOnlinePathRad[CHANNELCOUNT][300];
////	for(int i=0;i<CHANNELCOUNT;i++){
////		strOnlinePathRad[i][0]='\0';
////	}
////
////	for(int i=0;i<CHANNELCOUNT;i++){
////		if(i<12){
////			strcat(strOnlinePathRad[i],strOnlinePathOrigin.c_str());
////		}else{
////			strcat(strOnlinePathRad[i],strOnlinePathOriginForHide.c_str());
////		}
////		strcat(strOnlinePathRad[i],"_");
////	
////		//int转char*
////		stringstream ssTemp;
////		ssTemp << (RadarManager::Instance()->originalIndexToRecordIndex(i)+1);
////		string strTemp = ssTemp.str();
////		char* cIndex = (char*) strTemp.c_str();
////		
////		strcat(strOnlinePathRad[i],cIndex);//(char*)(48+ i) int i 转成char*
////		strcat(strOnlinePathRad[i],".rad");
////	}*/
////	
////	//ftp端路径	
////	std::string strOnlinePathOrigin = m_strFtpFolderPath;
////	/*
////	std::string strOnlinePathOrigin = ".\\";
////	strOnlinePathOrigin += cfg->get("projectname", "projectName");
////	strOnlinePathOrigin += "\\";
////	if(_access(strOnlinePathOrigin.c_str(),0)==-1){
////		FtpCreateDirectoryA(hftp,strOnlinePathOrigin.c_str());
////	}
////	strOnlinePathOrigin += cfg->get("projectname", "equipmentName");
////	strOnlinePathOrigin += "\\";
////	if(_access(strOnlinePathOrigin.c_str(),0)==-1){
////		FtpCreateDirectoryA(hftp,strOnlinePathOrigin.c_str());
////	}
////	strOnlinePathOrigin += m_strTimeTab;
////	strOnlinePathOrigin += "\\";
////	if(_access(strOnlinePathOrigin.c_str(),0)==-1){
////		FtpCreateDirectoryA(hftp,strOnlinePathOrigin.c_str());
////	}
////	strOnlinePathOrigin += m_strTimeTab;
////	*/
////	//char strOnlinePathRad[CHANNELCOUNT][300];
////	char **strOnlinePathRad = new char*[m_nChannelCountForUpload];
////	for(int i=0;i<m_nChannelCountForUpload;i++){
////		strOnlinePathRad[i] = new char[300];
////	}
////	for(int i=0;i<m_nChannelCountForUpload;i++){//使后面的strcat生效
////		strOnlinePathRad[i][0]='\0';
////	}
////	for(int i=0; i<m_nChannelCountForUpload; i++){
////		strcat(strOnlinePathRad[i],strOnlinePathOrigin.c_str());
////		strcat(strOnlinePathRad[i],"_");
////		strcat(strOnlinePathRad[i],intToString(i+1).c_str());
////		strcat(strOnlinePathRad[i],".rad");
////	}
////
////	for(int i=0;i<m_nChannelCountForUpload;i++){
////		if(!(fopen(strPathRad[i],"r")==NULL)){
////			int failCount = 0;
////			while(!FtpPutFileA(hftp,strPathRad[i],strOnlinePathRad[i], FTP_TRANSFER_TYPE_ASCII, 0)){
////				failCount=failCount+1;
////				Sleep(1000);
////				if(failCount>10){
////					stringstream ssPart,ssIndex;
////					ssPart<<roadPart;
////					ssIndex<<(i+1);
////					string strTemp="上传第"+ssPart.str()+"roadPart"+"第"+ssIndex.str()+"通道rad失败。目标路径:";
////					strTemp+=strOnlinePathRad[i];
////					strTemp+="\n";
////					fprintf(m_fpLog,strTemp.c_str() );
////					//AfxMessageBox(StringToCString(strTemp)); 
////					//AfxMessageBox(L"rad上传失败。");
////					break;
////				}
////			}
////		}else{
////			string strTemp="读取";
////			strTemp+=strPathRad[i];
////			strTemp+="失败";
////			strTemp+="\n";
////			fprintf(m_fpLog,strTemp.c_str() );
////			//AfxMessageBox(StringToCString(strTemp)); 
////		}
////	}
////
////	InternetCloseHandle(hftp);
////	InternetCloseHandle(hint);
////
////	for(int i=0;i<m_nChannelCountForUpload;i++){
////		delete []strOnlinePathRad[i];
////		delete []strPathRad[i];
////	}
////	delete []strOnlinePathRad;
////	delete []strPathRad;
////}

/*
//编写本地中每part的rad文件
void MeasureProject::writeRadFileForPart( int roadPart ){
	USES_CONVERSION;
	
	stringstream ssRoadPart;
	ssRoadPart << roadPart;
	string strRoadPart = ssRoadPart.str();
	
	std::string strPathOrigin = W2A( m_strSavePath );
	strPathOrigin += _projectName;
	strPathOrigin += _projectTab;
	strPathOrigin += "\\";
	strPathOrigin += _projectName;
	strPathOrigin += _projectTab;
	strPathOrigin += "_";
	strPathOrigin += strRoadPart;
	strPathOrigin += "\\";
	strPathOrigin += _projectName;
	strPathOrigin += _projectTab;
	strPathOrigin += "_";
	strPathOrigin += strRoadPart;
	
	//char strPathRad[m_nChannelCount][300];
	char **strPathRad = new char* [m_nChannelCount];
	for(int i=0;i<m_nChannelCountForUpload;i++){
		strPathRad[i]=new char [300];
	}
	for(int i=0;i<m_nChannelCountForUpload;i++){
		strPathRad[i][0]='\0';
	}

	for(int i=0;i<m_nChannelCountForUpload;i++){
		strcat(strPathRad[i],strPathOrigin.c_str());
		strcat(strPathRad[i],"_");
	
		//int转char*
		stringstream ssTemp;
		ssTemp << (i+1);
		string strTemp = ssTemp.str();
		char* cIndex = (char*) strTemp.c_str();
		
		strcat(strPathRad[i],cIndex);//(char*)(48+ i) int i 转成char*
		strcat(strPathRad[i],".rad");
	}

	ProjectNameTab::ProjectNameRow *lpProjectRow = dynamic_cast<ProjectNameTab::ProjectNameRow *>(_projectRow.get());
	ConfigureSet set;
	set.fromGroupXML( lpProjectRow->_paramXML );
	int sampleNum = RadarManager::Instance()->getSampCount( atoi ( set.get("radar", "sample").c_str()) );
	float sampleRate = RadarManager::Instance()->getSampRatio( atoi ( set.get("radar", "sampleratio" ).c_str() ), 0 );
	float distanceInterval = RadarManager::Instance()->GetTrueAccuracyFromPreclenAndPrecindex(atoi ( set.get("radar", "preclen").c_str() ),(float)atoi( cfg->get("radar", "precision").c_str() )) / 100.0;
	int wheelCount;
	wheelCount=m_vecUploadWheelCount[roadPart]-m_vecUploadWheelCount[roadPart-1];

	for(int i=0;i<m_nChannelCountForUpload;i++){
		ofstream ofile; 
		ofile.open(strPathRad[i]);  

		ofile<<"SAMPLES:"<<sampleNum<<endl;
		ofile<<"FREQUENCY:"<<sampleRate*1000<<endl;
		ofile<<"FREQUENCY STEPS:"<<1<<endl;
		ofile<<"SIGNAL POSITION:"<<0.000000<<endl;
		ofile<<"RAW SIGNAL POSITION:"<<1<<endl;
		ofile<<"DISTANCE FLAG:"<<0<<endl;
		ofile<<"TIME FLAG:"<<1<<endl;
		ofile<<"PROGRAM FLAG:"<<0<<endl;
		ofile<<"EXTERNAL FLAG:"<<0<<endl;
		ofile<<"TIME INTERVAL:"<<100.000000<<endl;
		ofile<<"DISTANCE INTERVAL:"<<distanceInterval<<endl;
		ofile<<"OPERATOR:"<<0<<endl;
		ofile<<"CUSTOMER:"<<0<<endl;
		ofile<<"SITE:"<<0<<endl;
		ofile<<"ANTENNAS:"<<"unknow"<<endl;
		ofile<<"ANTENNA ORIENTATION:"<<"NOT VALID FIELD"<<endl;
		ofile<<"ANTENNA SEPARATION:"<<0.012000<<endl;
		ofile<<"COMMENT:"<<"Saved from matlab"<<endl;
		ofile<<"TIMEWINDOW:"<<1.0 / sampleRate * sampleNum<<endl;
		ofile<<"STACKS:"<<1<<endl;
		ofile<<"STACK EXPONENT:"<<0<<endl;
		ofile<<"STACKING TIME:"<<0.015000<<endl;
		ofile<<"LAST TRACE:"<<wheelCount<<endl;
		ofile<<"STOP POSITION:"<<distanceInterval*wheelCount<<endl;
		ofile<<"SYSTEM CALIBRATION:"<<0.001000<<endl;
		ofile<<"START POSITION:"<<0.000000<<endl;
		ofile<<"SHORT FLAG:"<<1<<endl;
		ofile<<"INTERMEDIATE FLAG:"<<0<<endl;
		ofile<<"LONG FLAG:"<<0<<endl;
		ofile<<"PREPROCESSING:"<<0<<endl;
		ofile<<"HIGH:"<<5<<endl;
		ofile<<"LOW:"<<15<<endl;
		ofile<<"FIXED INCREMENT:"<<0.300000<<endl;
		ofile<<"FIXED MOVES UP:"<<0<<endl;
		ofile<<"FIXED MOVES DOWN:"<<1<<endl;
		ofile<<"FIXED POSITION:"<<0.000000<<endl;
		ofile<<"ARRAY:"<<83<<endl;
		ofile<<"ANTENNA FREQUENCY:"<<500<<endl;

		ofile.close();//关闭文件
	}
}*/

//编写本地Araayset.txt文件
void MeasureProject::writeTxtFile(){
	USES_CONVERSION;
	//下面10行构成本地文件路径 格式：path\Nametab\NameTabArrayset.txt
	std::string strPathTxt = W2A( m_strSavePath );
	strPathTxt += _projectName;
	strPathTxt += _projectTab;
	strPathTxt += "\\";
	if (0 != access(strPathTxt.c_str(), 0)){
		mkdir(strPathTxt.c_str());   // 返回 0 表示创建成功，-1 表示失败
	}
	strPathTxt += _projectName;
	strPathTxt += _projectTab;
	strPathTxt+="Arrayset.txt";

	//获取当前工程的各项参数
	ProjectNameTab::ProjectNameRow *lpProjectRow = dynamic_cast<ProjectNameTab::ProjectNameRow *>(_projectRow.get());
	ConfigureSet set;
	set.fromGroupXML( lpProjectRow->_paramXML );
	int sampleNum = RadarManager::Instance()->getSampCount( atoi ( set.get("radar", "sample").c_str()) );
	float sampleRate = RadarManager::Instance()->getSampRatio( atoi ( set.get("radar", "sampleratio" ).c_str() ), 0 );
	float distanceInterval = RadarManager::Instance()->GetTrueAccuracyFromPreclenAndPrecindex(atoi ( set.get("radar", "preclen").c_str() ),(float)atoi( set.get("radar", "precision").c_str() )) / 100.0;
	float tempArr[25]={1,1.9,2,2.5,2.8,2.8,3.2,4,4,4.1,5,5,6,6.4,7,7,7,8,8,12,15,16,30,30,81};
	float dieletric = tempArr [atoi ( set.get("radar", "dielectric").c_str() ) ];
	int startingPoint = atoi(set.get("radar", "directWave").c_str());
	
	//输出当前参数到本地文件中
	ofstream ofile; 
	ofile.open(strPathTxt.c_str()); 
	ofile<<"天线频率(MHz):"<<"200"<<endl;
	ofile<<"采样点数:"<<sampleNum<<endl;
	ofile<<"采样时窗(微秒):"<<1.0 / sampleRate * sampleNum<<endl;
	ofile<<"介电常数:"<<dieletric<<endl;
	ofile<<"X向距离间隔(厘米):"<<distanceInterval*100<<endl;
	if(m_nUpload==0){
		ofile<<"Y向数据通道数:"<<12<<endl;
		ofile<<"Y向数据距离间隔(厘米):"<<"0,14,28,42,56,70,84,98,112,126,140,154"<<endl;
		ofile<<"Y向数据距离间隔(厘米):"<<"0,0,0,0,0,0,0,0,0,0,0,0"<<endl;
	}else{
		ofile<<"Y向数据通道数:"<<6<<endl;
		ofile<<"Y向数据距离间隔(厘米):"<<"0,12,24,36,48,60"<<endl;
		ofile<<"直达波起点:"<<startingPoint<<endl;
	}
	ofile.close();//关闭文件
}



void MeasureProject::uploadTxtFileForGroup( int part ){
	USES_CONVERSION;
	//下面10行构成本地文件路径 格式：path\Nametab\NameTabArrayset.txt
	std::string strPathTxt = W2A( m_strSavePath );
	strPathTxt += _projectName;
	strPathTxt += _projectTab;
	strPathTxt += "\\";
	strPathTxt += _projectName;
	strPathTxt += _projectTab;
	strPathTxt+="Arrayset.txt";

	//获取ftp参数 ip 端口 用户 密码
	ConfigureSet *cfg = RadarManager::Instance()->getConfigureSet();
	string ftpServerIp = cfg->get("ftp", "serverIP");
	int port = atoi( cfg->get("ftp", "port").c_str());
	string account = cfg->get("ftp", "account");
	string password = cfg->get("ftp", "password");

	//创建ftp连接所需句柄
	HINTERNET hint;
	HINTERNET hftp;
	hint = InternetOpen(0, INTERNET_OPEN_TYPE_PRECONFIG, 0, 0, 0);
	if (hint==NULL)  {   
		AfxMessageBox(L"没网。");
		return;  
	} 
	hftp = InternetConnectA(hint, ftpServerIp.c_str(), port, account.c_str(), password.c_str(), INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
	if (hftp==NULL) {
		AfxMessageBox(L"连接ftp失败,正在尝试重连");
		return;
	}
	while(hftp==NULL){
		hftp = InternetConnectA(hint, ftpServerIp.c_str(), port, account.c_str(), password.c_str(), INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
	}

	//构成ftp上传路径
	std::string strOnlinePathTxt = m_strFtpFolderPath;
	strOnlinePathTxt += "Arrayset.txt";

	//将本地Arrayset.txt上传至ftp端
	//if(!(fopen(strPathTxt.c_str(),"rb")==NULL)){
	FILE *fp;
	if((fp=fopen(strPathTxt.c_str(),"rb"))!=NULL){
		fclose(fp);
		fp = NULL;
		int failCount=0;
		while(!FtpPutFileA(hftp,strPathTxt.c_str(),strOnlinePathTxt.c_str(), FTP_TRANSFER_TYPE_ASCII, 0)){
			failCount=failCount+1;
			Sleep(1000);
			if(failCount>10){
				stringstream ssPart;
				ssPart<<part;
				string strTemp="上传第"+ssPart.str()+"part Arrayset.txt失败。目标路径:";
				strTemp+=strOnlinePathTxt.c_str();
				strTemp+="\n";
				fprintf(m_fpLog,strTemp.c_str() );
				//AfxMessageBox(StringToCString(strTemp)); 
				//AfxMessageBox(L"Arrayset.txt上传失败。");
				break;
			}
		}
	}else{
		string strTemp="读取";
		strTemp+=strPathTxt.c_str();
		strTemp+="失败";
		strTemp+="\n";
		fprintf(m_fpLog,strTemp.c_str() );
		//AfxMessageBox(StringToCString(strTemp)); 
	}
	if (fp != NULL){
		fclose(fp);
		fp = NULL;
	}
	InternetCloseHandle(hftp);
	InternetCloseHandle(hint);
}

void MeasureProject::uploadGpsDataByPost(osg::Vec3d pos){
	//搭socket
	int sta_socket;
	struct sockaddr_in remote_ip;
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	sta_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (-1 == sta_socket) {
		closesocket(sta_socket);
		return;
	}
	memset(&remote_ip, 0, sizeof(struct sockaddr_in));
	remote_ip.sin_family = AF_INET;
	remote_ip.sin_addr.s_addr = inet_addr(m_strPostServerIp.c_str());
	remote_ip.sin_port = htons(m_nPostPort);
	if (0 != connect(sta_socket, (struct sockaddr*) (&remote_ip), sizeof(struct sockaddr))){
		closesocket(sta_socket);
		return;
	}
	//发送内容
	char *gpsJsonBuff = new char[1024];
	char *gpsSendBuff = new char[1024];
	time_t mytime = time(NULL);
	std::string strTimeStamp = intToString(mytime-10);
	//nGpsDataBufferLen+=sprintf(gpsDataBuffer+nGpsDataBufferLen, "{\"wheelCount\":%s, \"latitude\":\"%s\", \"longitude\":\"%s\", \"time\":\"%s %s\"},", dataList[1].c_str(), dataList[2].c_str(), dataList[3].c_str(), dataList[5].c_str(), dataList[6].c_str());//后面的按照“，内容”来填
	//int nGpsJsonLen=sprintf(gpsJsonBuff, GPS_JSON_FORMAT_FOR_EACH, m_strOprTab.c_str(), latitudeTransForm(dataList[2].c_str()), longitudeTransForm(dataList[3].c_str()), projectName.c_str(), equipmentName.c_str(), strTimeStamp.c_str());
	int nGpsJsonLen=sprintf(gpsJsonBuff, GPS_JSON_FORMAT_FOR_EACH, m_strOprTab.c_str(), pos.x(), pos.y(), m_strTaskCode.c_str(), m_strEquipmentName.c_str(), strTimeStamp.c_str());
	sprintf(gpsSendBuff, POST_FORMAT, "/externalservice/collectGPSData.ht", m_strPostServerIp.c_str(), m_nPostPort, intToString(nGpsJsonLen).c_str(), gpsJsonBuff);
	if (send(sta_socket, gpsSendBuff, (unsigned int)strlen(gpsSendBuff)+1, 0) < 0) {
		delete []gpsJsonBuff;
		delete []gpsSendBuff;
		return;
	}

	char* gpsRecieveBuff = new char[1024];
	int nGpsRecieveBuffLen = recv(sta_socket, gpsRecieveBuff, 1024, 0);
	if(nGpsRecieveBuffLen>0){
		//根据回复参数进行操作
		gpsRecieveBuff[nGpsRecieveBuffLen]='\0';
		std::string strRecieve=string(gpsRecieveBuff);
	}else{
		//没有收到回复参数
	}
	delete []gpsRecieveBuff;
	
	delete []gpsJsonBuff;
	delete []gpsSendBuff;
	closesocket(sta_socket);
}


//用于分割字符的方法函数
std::vector< std::string > SplitStr( std::string const& str, char splitChar ){//按字符splitChar分割目标字符串str，返回vector
	std::vector< std::string > result;
	std::string tmpStr = str;
	size_t findP;
	while ((findP = tmpStr.find(splitChar)) != std::string::npos){//从头遍历到最后 找出每个splitChar的位置并分割
		std::string subStr = tmpStr.substr(0, findP);
		tmpStr = tmpStr.substr(findP + 1, tmpStr.length() - findP - 1);
		result.push_back( subStr );
	}
	if (tmpStr.length() > 0){
		result.push_back(tmpStr);
	}
	return result;
}

//编写便于写电子文档的csv文件
void MeasureProject::writeNameCsvFile(){
	std::vector< std::string > namePart = SplitStr( _projectName, '_');//将工程名称分割得到各项信息
	if ( namePart.size() < 2 ){
		return;
	}

	USES_CONVERSION;
	//组成文件路径 格式path\SurveyLineName.csv
	std::string strPathCsv = W2A( m_strSavePath );
	strPathCsv+="SurveyLineName.csv";

	//输入工程名字中的信息
	FILE *_lpFile;
	_lpFile = fopen( strPathCsv.c_str(), "a" );
	fprintf(_lpFile, namePart[0].c_str());
	fprintf(_lpFile, ",");
	fprintf(_lpFile, namePart[1].c_str());
	fprintf(_lpFile, ",");
	fprintf(_lpFile, ",");
	fprintf(_lpFile, ",");
	fprintf(_lpFile, namePart[2].c_str());
	fprintf(_lpFile, ",");
	fprintf(_lpFile, namePart[3].c_str());
	fprintf(_lpFile, ",");

	//输入距离信息
	ProjectNameTab::ProjectNameRow *lpProjectRow = dynamic_cast<ProjectNameTab::ProjectNameRow *>(_projectRow.get());
	ConfigureSet set;
	set.fromGroupXML( lpProjectRow->_paramXML );
	float distanceInterval = RadarManager::Instance()->GetTrueAccuracyFromPreclenAndPrecindex(atoi ( set.get("radar", "preclen").c_str() ),(float)atoi( set.get("radar", "precision").c_str() )) / 100.0;
	float totalDistance=distanceInterval*(float)m_arrnRecordTotalWheelCount[0];
	fprintf(_lpFile, "%d", (int)totalDistance);

	//结尾换行
	fprintf(_lpFile, "\n");

	//关闭文件
	fclose( _lpFile );
	_lpFile = 0;

}

//16通道变28通道
void MeasureProject::copyRadarData(){
	USES_CONVERSION;
	std::string strPathOrigin = W2A( m_strSavePath );
	strPathOrigin += _projectName;
	strPathOrigin += _projectTab;
	strPathOrigin += "\\";
	strPathOrigin += _projectName;
	strPathOrigin += _projectTab;

	std::string strPathOriginForHide = W2A( m_strSavePath );
	strPathOriginForHide += _projectName;
	strPathOriginForHide += _projectTab;
	strPathOriginForHide+="\\moreChannels\\";
	strPathOriginForHide += _projectName;
	strPathOriginForHide += _projectTab;
	
	char strPathIn[12][300];
	for(int i=0;i<12;i++){
		strPathIn[i][0]='\0';
	}
	//int inIndex[12]={0,1,2,3,4,5,6,7,8,9,10,11};
	for(int i=0;i<12;i++){
		if(i<12){
			strcat(strPathIn[i],strPathOrigin.c_str());
		}else{
			strcat(strPathIn[i],strPathOriginForHide.c_str());
		}
		strcat(strPathIn[i],"_");
		//int转char*
		stringstream ssTemp;
		ssTemp << (i+1);
		string strTemp = ssTemp.str();
		char* cIndex = (char*) strTemp.c_str();
		strcat(strPathIn[i],cIndex);//(char*)(48+ i) int i 转成char*
		strcat(strPathIn[i],".rd3");
		//strcat(strPathIn[i],".gdd");
	}

	char strPathOut[12][300];
	for(int i=0;i<12;i++){
		strPathOut[i][0]='\0';
	}
	//int outIndex[13]={12,13,14,15,16,17,18,19,20,21,22,23,24};
	for(int i=0;i<12;i++){
		strcat(strPathOut[i],strPathOriginForHide.c_str());
		strcat(strPathOut[i],"_");
		//int转char*
		stringstream ssTemp;
		ssTemp << (i+12+1);
		string strTemp = ssTemp.str();
		char* cIndex = (char*) strTemp.c_str();
		strcat(strPathOut[i],cIndex);//(char*)(48+ i) int i 转成char*
		strcat(strPathOut[i],".rd3");
		//strcat(strPathOut[i],".gdd");
	}

	//ifstream in(strPathIn[0],ios::binary);
	//ofstream out(strPathOut[0],ios::binary);
	ofstream ofile; 
	ifstream ifile;
	for(int i=0;i<12;i++){
		ofile.open(strPathOut[i],ios::binary); 
		ifile.open(strPathIn[i],ios::binary); 
		if(ofile!=NULL && ifile!=NULL){
			ofile<<ifile.rdbuf();
		}
		ifile.close();
		ofile.close();
	}
}
/*
//组成ftp端文件路径
void MeasureProject::getOnlinePath(){
	USES_CONVERSION;

	//获取ftp端口信息 ip 端口 用户 密码
	ConfigureSet *cfg = RadarManager::Instance()->getConfigureSet();
	string ftpServerIp = cfg->get("ftp", "serverIP");
	int port = atoi( cfg->get("ftp", "port").c_str());
	string account = cfg->get("ftp", "account");
	string password = cfg->get("ftp", "password");

	//获取ftp连接句柄
	HINTERNET hint;
	HINTERNET hftp;
	hint = InternetOpen(0, INTERNET_OPEN_TYPE_PRECONFIG, 0, 0, 0);
	if (hint==NULL)  {   
		AfxMessageBox(L"没网。");
		return;  
	} 
	hftp = InternetConnectA(hint, ftpServerIp.c_str(), port, account.c_str(), password.c_str(), INTERNET_SERVICE_FTP, 0, 0);
	if (hftp==NULL) {
		AfxMessageBox(L"连接ftp失败。");
		return;
	}
	while(hftp==NULL){
		hftp = InternetConnectA(hint, ftpServerIp.c_str(), port, account.c_str(), password.c_str(), INTERNET_SERVICE_FTP, 0, 0);
	}
	
	//组成ftp端文件路径 格式.\radarData\NameTab\NameTab_1\NameTab_1_i.rd3及.\radarData\NameTab\NameTab_1\moreChannels\NameTab_1_i.rd3
	std::string strPathOrigin = ".\\radarData\\";
	_onlineDocumentPathLength = strPathOrigin.length();
	strPathOrigin += _projectName;
	strPathOrigin += _projectTab;
	_onlineNamePathLength = strPathOrigin.length()-_onlineDocumentPathLength;
	strPathOrigin += "\\";
	if(_access(strPathOrigin.c_str(),0)==-1){
		FtpCreateDirectoryA(hftp,strPathOrigin.c_str());
	}
	strPathOrigin += _projectName;
	strPathOrigin += _projectTab;
	strPathOrigin+="_1";
	if(_access(strPathOrigin.c_str(),0)==-1){
		FtpCreateDirectoryA(hftp,strPathOrigin.c_str());
	}
	strPathOrigin+="\\";
	strPathOrigin += _projectName;
	strPathOrigin += _projectTab;
	strPathOrigin+="_1";

	std::string strPathOriginForHide = ".\\radarData\\";
	//std::string strPathOriginForHide = "C:\\Users\\Administrator\\Desktop\\test\\";
	strPathOriginForHide += _projectName;
	strPathOriginForHide += _projectTab;
	strPathOriginForHide+="\\";
	strPathOriginForHide += _projectName;
	strPathOriginForHide += _projectTab;
	strPathOriginForHide+="_1";
	strPathOriginForHide += "\\moreChannels";
	if(_access(strPathOriginForHide.c_str(),0)==-1){
		FtpCreateDirectoryA(hftp,strPathOriginForHide.c_str());
	}
	strPathOriginForHide+="\\";
	strPathOriginForHide += _projectName;
	strPathOriginForHide += _projectTab;
	strPathOriginForHide += "_1";
	
	for(int i=0;i<CHANNELCOUNT;i++){
		_strOnlineRadarDataPath[i][0]='\0';
		//strcat(strPathSGY[i],strPathOrigin.c_str());
		if(i<12){
			strcat(_strOnlineRadarDataPath[i],strPathOrigin.c_str());
		}else{
			strcat(_strOnlineRadarDataPath[i],strPathOriginForHide.c_str());
		}
		strcat(_strOnlineRadarDataPath[i],"_");
	
		//int转char*
		stringstream ssTemp;
		//ssTemp << (RadarManager::Instance()->originalIndexToRecordIndex(i)+1);
		ssTemp << (i+1);
		string strTemp = ssTemp.str();
		char* cIndex = (char*) strTemp.c_str();
		
		strcat(_strOnlineRadarDataPath[i],cIndex);
		strcat(_strOnlineRadarDataPath[i],".rd3");
	}

	for(int i=0;i<CHANNELCOUNT;i++){//字符串结尾补个终止符
		if(i<9){
			_strOnlineRadarDataPath[i][strPathOrigin.length()+6]='\0';
		}else if(i<12){
			_strOnlineRadarDataPath[i][strPathOrigin.length()+7]='\0';
		}else{
			_strOnlineRadarDataPath[i][strPathOriginForHide.length()+7]='\0';
		}
	}
}*/

/*
//组成ftp端文件路径
void MeasureProject::getOnlinePath(){
	USES_CONVERSION;
	
	////std::string strPathOrigin = ".\\radarData\\";
	//std::string strPathOrigin = "C:\\Users\\Administrator\\Desktop\\test\\";
	//_onlineDocumentPathLength = strPathOrigin.length();
	//strPathOrigin += _projectName;
	//strPathOrigin += _projectTab;
	//_onlineNamePathLength = strPathOrigin.length()-_onlineDocumentPathLength;
	//if (0 != access(strPathOrigin.c_str(), 0)){
	//	// if this folder not exist, create a new one.
	//	mkdir(strPathOrigin.c_str());   // 返回 0 表示创建成功，-1 表示失败
	//}
	//strPathOrigin+="\\";
	//strPathOrigin += _projectName;
	//strPathOrigin += _projectTab;
	//strPathOrigin+="_1";
	//if (0 != access(strPathOrigin.c_str(), 0)){
	//	// if this folder not exist, create a new one.
	//	mkdir(strPathOrigin.c_str());   // 返回 0 表示创建成功，-1 表示失败
	//}
	//strPathOrigin+="\\";
	//strPathOrigin += _projectName;
	//strPathOrigin += _projectTab;
	//strPathOrigin+="_1";

	////std::string strPathOriginForHide = ".\\radarData\\";
	//std::string strPathOriginForHide = "C:\\Users\\Administrator\\Desktop\\test\\";
	//strPathOriginForHide += _projectName;
	//strPathOriginForHide += _projectTab;
	//strPathOriginForHide+="\\";
	//strPathOriginForHide += _projectName;
	//strPathOriginForHide += _projectTab;
	//strPathOriginForHide+="_1";
	//strPathOriginForHide += "\\moreChannels";
	//if (0 != access(strPathOriginForHide.c_str(), 0)){
	//	// if this folder not exist, create a new one.
	//	mkdir(strPathOriginForHide.c_str());   // 返回 0 表示创建成功，-1 表示失败
	//}
	//strPathOriginForHide += "\\";
	//strPathOriginForHide += _projectName;
	//strPathOriginForHide += _projectTab;
	//strPathOriginForHide += "_1";
	//
	//for(int i=0;i<CHANNELCOUNT;i++){//使后面的strcat生效
	//	_strLocalRadarDataPath[i][0]='\0';
	//}
	//
	//for(int i=0;i<CHANNELCOUNT;i++){
	//	//strcat(strPathSGY[i],strPathOrigin.c_str());
	//	if(i<12){
	//		strcat(_strLocalRadarDataPath[i],strPathOrigin.c_str());
	//	}else{
	//		strcat(_strLocalRadarDataPath[i],strPathOriginForHide.c_str());
	//	}
	//	strcat(_strLocalRadarDataPath[i],"_");
	//
	//	//int转char*
	//	stringstream ssTemp;
	//	//ssTemp << (RadarManager::Instance()->originalIndexToRecordIndex(i)+1);
	//	ssTemp << (i+1);
	//	string strTemp = ssTemp.str();
	//	char* cIndex = (char*) strTemp.c_str();
	//	
	//	strcat(_strOnlineRadarDataPath[i],cIndex);//(char*)(48+ i) int i 转成char*
	//	strcat(_strOnlineRadarDataPath[i],".rd3");
	//}

	//for(int i=0;i<CHANNELCOUNT;i++){//字符串结尾补个终止符
	//	if(i<9){
	//		_strOnlineRadarDataPath[i][strPathOrigin.length()+6]='\0';
	//	}else if(i<12){
	//		_strOnlineRadarDataPath[i][strPathOrigin.length()+7]='\0';
	//	}else{
	//		_strOnlineRadarDataPath[i][strPathOriginForHide.length()+7]='\0';
	//	}
	//}

	//获取ftp端口信息 ip 端口 用户 密码
	ConfigureSet *cfg = RadarManager::Instance()->getConfigureSet();
	string ftpServerIp = cfg->get("ftp", "serverIP");
	int port = atoi( cfg->get("ftp", "port").c_str());
	string account = cfg->get("ftp", "account");
	string password = cfg->get("ftp", "password");

	//获取ftp连接句柄
	HINTERNET hint;
	HINTERNET hftp;
	hint = InternetOpen(0, INTERNET_OPEN_TYPE_PRECONFIG, 0, 0, 0);
	if (hint==NULL)  {   
		AfxMessageBox(L"没网。");
		return;  
	} 
	hftp = InternetConnectA(hint, ftpServerIp.c_str(), port, account.c_str(), password.c_str(), INTERNET_SERVICE_FTP, 0, 0);
	if (hftp==NULL) {
		AfxMessageBox(L"连接ftp失败。");
	}
	while(hftp==NULL){
		hftp = InternetConnectA(hint, ftpServerIp.c_str(), port, account.c_str(), password.c_str(), INTERNET_SERVICE_FTP, 0, 0);
	}
	//作业名/道路名/设备名/时间戳/
	//组成ftp端文件路径 格式.\radarData\NameTab\NameTab_1\NameTab_1_i.rd3及.\radarData\NameTab\NameTab_1\moreChannels\NameTab_1_i.rd3
	//组成ftp端文件路径 格式.\radarData\ProjectName\roadName\equipmentName\Tab\NameTab_1\NameTab_1_i.rd3及.\radarData\ProjectName\roadName\equipmentName\Tab\NameTab_1\moreChannels\NameTab_1_i.rd3
	std::string strPathOrigin = ".\\radarData\\";
	strPathOrigin += cfg->get("projectname", "projectName");
	strPathOrigin += "\\";
	if(_access(strPathOrigin.c_str(),0)==-1){
		FtpCreateDirectoryA(hftp,strPathOrigin.c_str());
	}
	
	//strPathOrigin += cfg->get("projectname", "roadname");
	//strPathOrigin += "\\";
	//if(_access(strPathOrigin.c_str(),0)==-1){
	//	FtpCreateDirectoryA(hftp,strPathOrigin.c_str());
	//}
	
	strPathOrigin += cfg->get("projectname", "equipmentName");
	strPathOrigin += "\\";
	if(_access(strPathOrigin.c_str(),0)==-1){
		FtpCreateDirectoryA(hftp,strPathOrigin.c_str());
	}
	strPathOrigin += _projectTab;
	strPathOrigin += "\\";
	_onlineDocumentPathLength = strPathOrigin.length();
	_onlineNamePathLength = _projectName.length()+_projectTab.length();
	
	if(_access(strPathOrigin.c_str(),0)==-1){
		FtpCreateDirectoryA(hftp,strPathOrigin.c_str());
	}
	strPathOrigin += _projectName;
	strPathOrigin += _projectTab;
	strPathOrigin+="_1";
	if(_access(strPathOrigin.c_str(),0)==-1){
		FtpCreateDirectoryA(hftp,strPathOrigin.c_str());
	}
	strPathOrigin+="\\";
	strPathOrigin += _projectName;
	strPathOrigin += _projectTab;
	strPathOrigin+="_1";

	std::string strPathOriginForHide = ".\\radarData\\";
	//std::string strPathOriginForHide = "C:\\Users\\Administrator\\Desktop\\test\\";
	strPathOriginForHide += cfg->get("projectname", "projectName");
	strPathOriginForHide += "\\";
	//strPathOriginForHide += cfg->get("projectname", "roadname");
	//strPathOriginForHide += "\\";
	strPathOriginForHide += cfg->get("projectname", "equipmentName");
	strPathOriginForHide += "\\";
	strPathOriginForHide += _projectTab;
	strPathOriginForHide+="\\";
	strPathOriginForHide += _projectName;
	strPathOriginForHide += _projectTab;
	strPathOriginForHide+="_1";
	strPathOriginForHide += "\\moreChannels";
	if(_access(strPathOriginForHide.c_str(),0)==-1){
		FtpCreateDirectoryA(hftp,strPathOriginForHide.c_str());
	}
	strPathOriginForHide+="\\";
	strPathOriginForHide += _projectName;
	strPathOriginForHide += _projectTab;
	strPathOriginForHide += "_1";
	
	for(int i=0;i<CHANNELCOUNT;i++){
		_strOnlineRadarDataPath[i][0]='\0';
		//strcat(strPathSGY[i],strPathOrigin.c_str());
		if(i<12){
			strcat(_strOnlineRadarDataPath[i],strPathOrigin.c_str());
		}else{
			strcat(_strOnlineRadarDataPath[i],strPathOriginForHide.c_str());
		}
		strcat(_strOnlineRadarDataPath[i],"_");
	
		//int转char*
		stringstream ssTemp;
		//ssTemp << (RadarManager::Instance()->originalIndexToRecordIndex(i)+1);
		ssTemp << (i+1);
		string strTemp = ssTemp.str();
		char* cIndex = (char*) strTemp.c_str();
		
		strcat(_strOnlineRadarDataPath[i],cIndex);
		strcat(_strOnlineRadarDataPath[i],".rd3");
	}

	for(int i=0;i<CHANNELCOUNT;i++){//字符串结尾补个终止符
		if(i<9){
			_strOnlineRadarDataPath[i][strPathOrigin.length()+6]='\0';
		}else if(i<12){
			_strOnlineRadarDataPath[i][strPathOrigin.length()+7]='\0';
		}else{
			_strOnlineRadarDataPath[i][strPathOriginForHide.length()+7]='\0';
		}
	}
}*/


//////将某part的数据上传至ftp端
////void MeasureProject::copyRadarDataOnline(int part){
////	
////	ConfigureSet *cfg = RadarManager::Instance()->getConfigureSet();
////	//获取ftp端信息 ip地址 端口 用户 密码
////	string ftpServerIp = cfg->get("ftp", "serverIP");
////	int port = atoi( cfg->get("ftp", "port").c_str());
////	string account = cfg->get("ftp", "account");
////	string password = cfg->get("ftp", "password");
////
////	//获取ftp连接句柄
////	HINTERNET hint;
////	HINTERNET hftp;
////	hint = InternetOpen(0, INTERNET_OPEN_TYPE_PRECONFIG, 0, 0, 0);
////	if (hint==NULL)  {   
////		AfxMessageBox(L"没网。");
////		return;  
////	} 
////	hftp = InternetConnectA(hint, ftpServerIp.c_str(), port, account.c_str(), password.c_str(), INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
////	if (hftp==NULL) {
////		AfxMessageBox(L"连接ftp失败。");
////		return;
////	}
////	while(hftp==NULL){
////		hftp = InternetConnectA(hint, ftpServerIp.c_str(), port, account.c_str(), password.c_str(), INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
////	}
////
////	//创建cor及rd3的本地与线上路径
////	std::string localCorFilePath;
////	std::string onlineCorFilePath;
////	/*
////	char strOnlinePath[CHANNELCOUNT][300];
////	char strLocalPath[CHANNELCOUNT][300];
////	for(int i=0;i<CHANNELCOUNT;i++){
////		strOnlinePath[i][0]='\0';
////		strLocalPath[i][0]='\0';
////	}*/
////
////	char **vecStrRadarDataOnlinePath = new char* [m_nChannelCount];
////	char **vecStrRadarDataLocalPath = new char* [m_nChannelCount];
////	for(int i=0;i<m_nChannelCountForUpload;i++){
////		vecStrRadarDataOnlinePath[i]=new char[300];
////		vecStrRadarDataLocalPath[i]=new char[300];
////	}
////	for(int i=0;i<m_nChannelCountForUpload;i++){
////		vecStrRadarDataOnlinePath[i][0]='\0';
////		vecStrRadarDataLocalPath[i][0]='\0';
////	}
////	
////	//本地上传路径
////	for(int i=0; i<m_nChannelCountForUpload; i++){
////		//==============================按part得到当前所需本地路径及线上路径==============================//
////		//前12道.\radarData\NameTab\NameTab_1\NameTab_1_i.rd3及.\radarData\NameTab\NameTab_1\moreChannels\NameTab_1_i.rd3变成
////		//前12道.\radarData\ProjectName\roadName\equipmentName\Tab\NameTab_1\NameTab_1_i.rd3及.\radarData\ProjectName\roadName\equipmentName\Tab\NameTab_1\moreChannels\NameTab_1_i.rd3
////		//前12道.\radarData\NameTab\NameTab_part\NameTab_part_i.rd3及.\radarData\NameTab\NameTab_part\moreChannels\NameTab_part_i.rd3
////		//前12道localPath\NameTab\NameTab_1\NameTab_1_i.rd3及localPath\NameTab\NameTab_1\moreChannels\NameTab_1_i.rd3变成
////		//前12道localPath\NameTab\NameTab_part\NameTab_part_i.rd3及localPath\NameTab\NameTab_part\moreChannels\NameTab_part_i.rd3
////		
////		std::stringstream ss;
////		ss << part;
////
////		std::string localFilePath,onlineFilePath;
////		localFilePath = _strLocalRadarDataPath[i];
////		//将NameTab_1文件夹修改成NameTab_part
////		localFilePath.replace(_documentPathLength+_namePathLength*2+2, 1, ss.str() );//string.replace(a,b,c)第a位开始的b个字符替换成c
////		//将NameTab_1_i文件名修改成NameTab_part_i
////		if(i<12){//前12道
////			if(part<10){
////				localFilePath.replace(_documentPathLength+_namePathLength*3+5, 1, ss.str() );
////			}else if(part<100){
////				localFilePath.replace(_documentPathLength+_namePathLength*3+6, 1, ss.str() );
////			}else{
////				localFilePath.replace(_documentPathLength+_namePathLength+7*3, 1, ss.str() );
////			}
////		}else{//后12道的文件路径格式不同
////			if(part<10){
////				localFilePath.replace(_documentPathLength+_namePathLength*3+18, 1, ss.str() );
////			}else if(part<100){
////				localFilePath.replace(_documentPathLength+_namePathLength*3+19, 1, ss.str() );
////			}else{
////				localFilePath.replace(_documentPathLength+_namePathLength*3+20, 1, ss.str() );
////			}
////		}
////		strcat(vecStrRadarDataLocalPath[i],localFilePath.c_str());
////		if(i==0){//组成cor的local和online路径
////			localCorFilePath = localFilePath.substr(0,localFilePath.length() - 6);
////			localCorFilePath+="gpsForUpload.cor";
////		}
////	}
////
////	//ftp端路径	
////	for(int i=0; i<m_nChannelCountForUpload; i++){
////		strcat(vecStrRadarDataOnlinePath[i],m_strFtpFolderPath.c_str());
////		strcat(vecStrRadarDataOnlinePath[i],"_");
////		strcat(vecStrRadarDataOnlinePath[i],intToString(i+1).c_str());
////		strcat(vecStrRadarDataOnlinePath[i],".rd3");
////	}
////
////	onlineCorFilePath=m_strFtpFolderPath;
////	onlineCorFilePath+="gps.cor";
////
////	//按local路径及online路径进行上传
////	//上传cor数据
////	if(!(fopen(localCorFilePath.c_str(),"rb")==NULL)){
////		int failCount = 0;
////		while(!(FtpPutFileA(hftp,localCorFilePath.c_str(),onlineCorFilePath.c_str(), FTP_TRANSFER_TYPE_BINARY, 0))){
////			failCount=failCount+1;
////			Sleep(1000);
////			if(failCount>10){
////				stringstream ssPart;
////				ssPart<<part;
////				string strTemp="上传第"+ssPart.str()+"part"+"cor失败。目标路径:";
////				strTemp+=onlineCorFilePath.c_str();
////				strTemp+="\n";
////				fprintf(m_fpLog,strTemp.c_str() );
////				//AfxMessageBox(StringToCString(strTemp)); 
////				break;
////			}
////		}
////	}else{
////		string strTemp="读取";
////		strTemp+=localCorFilePath.c_str();
////		strTemp+="失败";
////		strTemp+="\n";
////		fprintf(m_fpLog,strTemp.c_str() );
////		//AfxMessageBox(StringToCString(strTemp)); 
////	}
////	//上传rd3数据
////	for(int i=0; i<m_nChannelCountForUpload; i++){
////		if(!(fopen(vecStrRadarDataLocalPath[i],"rb")==NULL)){
////			int failCount = 0;
////			while(!(FtpPutFileA(hftp,vecStrRadarDataLocalPath[i],vecStrRadarDataOnlinePath[i], FTP_TRANSFER_TYPE_BINARY, 0))){
////				failCount=failCount+1;
////				Sleep(1000);
////				if(failCount>10){
////					stringstream ssPart,ssIndex;
////					ssPart<<part;
////					ssIndex<<(i+1);
////					string strTemp="上传第"+ssPart.str()+"part"+"第"+ssIndex.str()+"通道rd3失败。目标路径:";
////					strTemp+=vecStrRadarDataOnlinePath[i];
////					strTemp+="\n";
////					fprintf(m_fpLog,strTemp.c_str() );
////					//AfxMessageBox(StringToCString(strTemp)); 
////					break;
////				}
////			}
////		}else{
////			string strTemp="读取";
////			strTemp+=vecStrRadarDataLocalPath[i];
////			strTemp+="失败";
////			strTemp+="\n";
////			fprintf(m_fpLog,strTemp.c_str() );
////			//AfxMessageBox(StringToCString(strTemp)); 
////		}
////		//FtpPutFileA(hftp,vecStrRadarDataLocalPath[i],vecStrRadarDataOnlinePath[i], FTP_TRANSFER_TYPE_BINARY, 0);
////		//FtpPutFileA(hftp,vecStrRadarDataLocalPath[i],vecStrRadarDataOnlinePath[i+12], FTP_TRANSFER_TYPE_BINARY, 0);
////	}
////	//断开ftp句柄
////	InternetCloseHandle(hftp);
////	InternetCloseHandle(hint);
////	
////	for(int i=0;i<m_nChannelCountForUpload;i++){
////		delete []vecStrRadarDataOnlinePath[i];
////		delete []vecStrRadarDataLocalPath[i];
////	}
////	delete []vecStrRadarDataOnlinePath;
////	delete []vecStrRadarDataLocalPath;
////
////	
////	/*
////	//创建cor及rd3的本地路径
////	//本地上传路径	
////	std::string localCorFilePath;
////	char strLocalPath[m_nChannelCount][300];
////	for(int i=0;i<m_nChannelCount;i++){
////		strLocalPath[i][0]='\0';
////	}
////	for(int i=0; i<m_nChannelCount; i++){
////		//==============================按part得到当前所需本地路径及线上路径==============================//
////		//前12道.\radarData\NameTab\NameTab_1\NameTab_1_i.rd3及.\radarData\NameTab\NameTab_1\moreChannels\NameTab_1_i.rd3变成
////		//前12道.\radarData\ProjectName\roadName\equipmentName\Tab\NameTab_1\NameTab_1_i.rd3及.\radarData\ProjectName\roadName\equipmentName\Tab\NameTab_1\moreChannels\NameTab_1_i.rd3
////		//前12道.\radarData\NameTab\NameTab_part\NameTab_part_i.rd3及.\radarData\NameTab\NameTab_part\moreChannels\NameTab_part_i.rd3
////		//前12道localPath\NameTab\NameTab_1\NameTab_1_i.rd3及localPath\NameTab\NameTab_1\moreChannels\NameTab_1_i.rd3变成
////		//前12道localPath\NameTab\NameTab_part\NameTab_part_i.rd3及localPath\NameTab\NameTab_part\moreChannels\NameTab_part_i.rd3
////		
////		std::stringstream ss;
////		ss << part;
////
////		std::string localFilePath;
////		localFilePath = _strLocalRadarDataPath[i];
////		//将NameTab_1文件夹修改成NameTab_part
////		localFilePath.replace(_documentPathLength+_namePathLength*2+2, 1, ss.str() );//string.replace(a,b,c)第a位开始的b个字符替换成c
////		//将NameTab_1_i文件名修改成NameTab_part_i
////		if(i<12){//前12道
////			if(part<10){
////				localFilePath.replace(_documentPathLength+_namePathLength*3+5, 1, ss.str() );
////			}else if(part<100){
////				localFilePath.replace(_documentPathLength+_namePathLength*3+6, 1, ss.str() );
////			}else{
////				localFilePath.replace(_documentPathLength+_namePathLength+7*3, 1, ss.str() );
////			}
////		}else{//后12道的文件路径格式不同
////			if(part<10){
////				localFilePath.replace(_documentPathLength+_namePathLength*3+18, 1, ss.str() );
////			}else if(part<100){
////				localFilePath.replace(_documentPathLength+_namePathLength*3+19, 1, ss.str() );
////			}else{
////				localFilePath.replace(_documentPathLength+_namePathLength*3+20, 1, ss.str() );
////			}
////		}
////		strcat(strLocalPath[i],localFilePath.c_str());
////		if(i==0){//组成cor的local和online路径
////			localCorFilePath = localFilePath.substr(0,localFilePath.length() - 6);
////			localCorFilePath+="gpsForUpload.cor";
////		}
////	}
////
////	ConfigureSet *cfg = RadarManager::Instance()->getConfigureSet();
////	//获取socket端信息 ip地址 端口 用户 密码 以及 项目名称 设备编号 opr
////	string serverIp = cfg->get("ftp", "serverIP");
////	int port = atoi( cfg->get("ftp", "port").c_str());
////	string account = cfg->get("ftp", "account");
////	string password = cfg->get("ftp", "password");
////	string projectName = cfg->get("projectname", "projectName");
////	string equipmentName = cfg->get("projectname", "equipmentName");
////	string opr = m_strOprTab;
////	
////	//socket
////	int sta_socket;
////	struct sockaddr_in remote_ip;
////	//char* send_buf = NULL;
////	char* recv_buf = NULL;
////	char* radarData = NULL;
////	char* data = NULL;
////	int recbytes;
////	char* ptr = NULL;
////
////	int len_int = 0;
////
////	WSADATA wsaData;
////	WSAStartup(MAKEWORD(2, 2), &wsaData);
////
////	sta_socket = socket(PF_INET, SOCK_STREAM, 0);
////
////	if (-1 == sta_socket) {
////		closesocket(sta_socket);
////		return -1;
////	}
////	//bzero(&remote_ip, sizeof(struct sockaddr_in));
////	memset(&remote_ip, 0, sizeof(struct sockaddr_in));
////	remote_ip.sin_family = AF_INET;
////	remote_ip.sin_addr.s_addr = inet_addr(serverIp.c_str());
////	remote_ip.sin_port = htons(port);
////	if (0 != connect(sta_socket, (struct sockaddr*) (&remote_ip), sizeof(struct sockaddr))) {
////		closesocket(sta_socket);
////		return -1;
////	}
////
////	{
////		FILE *fp = fopen(localCorFilePath.c_str(),"r");
////		fseek(fp,0,SEEK_END);
////		int radarDataSize = ftell(fp);
////		fclose(fp);
////		char *radarDataBuffer = new char[radarDataSize];
////		//读取雷达数据
////		fp = fopen(localCorFilePath.c_str(),"r");
////		fread(radarDataBuffer, 1, radarDataSize, fp);
////		fclose(fp);
////		//json
////		char *content = new char[strlen(jsonFormat)+strlen(projectName.c_str())+strlen(equipmentName.c_str())+strlen(opr.c_str())+strlen(intToString(i).c_str())+strlen(intToString(part).c_str())+radarDataSize];
////		sprintf(content, jsonFormat, projectName, equipmentName, opr, m_strTimeTab, i, part, radarDataBuffer);//拼成json
////		//post_form_data("xxx", "xxx", data, 1);//参数拼接index使用1
////		//len = strlen(data);
////		//post_form_data("xxx", "xxx", data + len, 0);//参数最后index使用0
////		//len = strlen(data);
////		int len = strlen(content);
////		char* send_buf = new char[strlen(Query_version_url)+strlen(serverIp.c_str())+strlen(intToString(port).c_str())+strlen(intToString(len).c_str())+strlen(Boundary)+len];
////		sprintf(send_buf, Query_version_url, serverIp, port, len, Boundary, content);
////		
////		if (send(sta_socket, send_buf, (unsigned int)strlen(send_buf), 0) < 0) {
////			free(send_buf);
////			free(content);
////			free(radarDataBuffer);
////			closesocket(sta_socket);
////			return -1;
////		}
////		memset(recv_buf, 0, 1024);
////		recbytes = recv(sta_socket, recv_buf, 1024, 0);
////		//recbytes = recieveStream(sta_socket, recv_buf, 1024, 0);
////		if(recbytes>0){
////			recv_buf[recbytes]=0x00;
////			//根据回复参数进行操作
////		}else{
////			//没有收到回复参数
////		}
////		free(send_buf);
////		free(content);
////		free(radarDataBuffer);
////	}
////
////	//传输雷达数据
////	for(int i=0; i<6;i++){
////		//获取radarData大小
////		FILE *fp = fopen(strLocalPath[i],"rb");
////		fseek(fp,0,SEEK_END);
////		int radarDataSize = ftell(fp);
////		fclose(fp);
////		char *radarDataBuffer = new char[radarDataSize];
////		//读取雷达数据
////		fp = fopen(strLocalPath[i],"rb");
////		fread(radarDataBuffer, 1, radarDataSize, fp);
////		fclose(fp);
////
////		len=strlen(jsonFormat)+strlen(projectName.c_str())+strlen(equipmentName.c_str())+strlen(opr.c_str())+strlen(intToString(i).c_str())+strlen(intToString(part).c_str())+radarDataSize;
////		sprintf(send_buf, Query_version_json, serverIp, port, len, Boundary, projectName, equipmentName, opr, m_strTimeTab, i, part, radarDataBuffer);
////
////		if (send(sta_socket, send_buf, (unsigned int)strlen(send_buf), 0) < 0) {
////			free(send_buf);
////			//free(content);
////			free(radarDataBuffer);
////			closesocket(sta_socket);
////			return -1;
////		}
////
////		memset(recv_buf, 0, 1024);
////		recbytes = recv(sta_socket, recv_buf, 1024, 0);
////		//recbytes = recieveStream(sta_socket, recv_buf, 1024, 0);
////		if(recbytes>0){
////			recv_buf[recbytes]=0x00;
////			//根据回复参数进行操作
////		}else{
////			//没有收到回复参数
////		}
////
////		free(send_buf);
////		//free(content);
////		free(radarDataBuffer);
////	}
////	closesocket(sta_socket);
////	*/
////}


//将某part的数据上传至ftp端
void MeasureProject::uploadDataForPart(int part){
	
	ConfigureSet *cfg = RadarManager::Instance()->getConfigureSet();
	//获取ftp端信息 ip地址 端口 用户 密码
	string ftpServerIp = cfg->get("ftp", "serverIP");
	int port = atoi( cfg->get("ftp", "port").c_str());
	string account = cfg->get("ftp", "account");
	string password = cfg->get("ftp", "password");

	//获取ftp连接句柄
	HINTERNET hint;
	HINTERNET hftp;
	hint = InternetOpen(0, INTERNET_OPEN_TYPE_PRECONFIG, 0, 0, 0);
	if (hint==NULL)  {   
		AfxMessageBox(L"没网。");
		return;  
	} 
	hftp = InternetConnectA(hint, ftpServerIp.c_str(), port, account.c_str(), password.c_str(), INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
	if (hftp==NULL) {
		AfxMessageBox(L"连接ftp失败。");
		return;
	}
	while(hftp==NULL){
		hftp = InternetConnectA(hint, ftpServerIp.c_str(), port, account.c_str(), password.c_str(), INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
	}

	//创建txt、cor、rad及rd3的本地与线上路径
	std::string localCsvFilePath;
	std::string onlineCsvFilePath;
	std::string localCorFilePath;
	std::string onlineCorFilePath;
	std::string localTxtFilePath;
	std::string onlineTxtFilePath;
	char **vecStrRadarDataHeaderOnlinePath = new char* [m_nChannelCountForUpload];
	char **vecStrRadarDataHeaderLocalPath = new char* [m_nChannelCountForUpload];
	char **vecStrRadarDataOnlinePath = new char* [m_nChannelCountForUpload];
	char **vecStrRadarDataLocalPath = new char* [m_nChannelCountForUpload];
	for(int i=0;i<m_nChannelCountForUpload;i++){
		vecStrRadarDataHeaderOnlinePath[i]=new char[300];
		vecStrRadarDataHeaderLocalPath[i]=new char[300];
		vecStrRadarDataOnlinePath[i]=new char[300];
		vecStrRadarDataLocalPath[i]=new char[300];
	}
	for(int i=0;i<m_nChannelCountForUpload;i++){
		vecStrRadarDataHeaderOnlinePath[i][0]='\0';
		vecStrRadarDataHeaderLocalPath[i][0]='\0';
		vecStrRadarDataOnlinePath[i][0]='\0';
		vecStrRadarDataLocalPath[i][0]='\0';
	}

	USES_CONVERSION;
	stringstream ssRoadPart;
	ssRoadPart << part;
	string strRoadPart = ssRoadPart.str();

	std::string strPathOrigin = W2A( m_strSavePath );
	strPathOrigin += _projectName;
	strPathOrigin += _projectTab;
	strPathOrigin += "\\";
	strPathOrigin += _projectName;
	strPathOrigin += _projectTab;

	localTxtFilePath=strPathOrigin+"Arrayset.txt";

	strPathOrigin += "_";
	strPathOrigin += strRoadPart;
	strPathOrigin += "\\";
	strPathOrigin += _projectName;
	strPathOrigin += _projectTab;
	strPathOrigin += "_";
	strPathOrigin += strRoadPart;

	localCorFilePath=strPathOrigin+"gpsForUpload.cor";
	localCsvFilePath=strPathOrigin+".csv";

	for(int i=0;i<m_nChannelCountForUpload;i++){
		strcat(vecStrRadarDataHeaderLocalPath[i],strPathOrigin.c_str());
		strcat(vecStrRadarDataHeaderLocalPath[i],"_");
		strcat(vecStrRadarDataHeaderLocalPath[i],intToString(i+1).c_str());
		strcat(vecStrRadarDataHeaderLocalPath[i],".rad");
		strcat(vecStrRadarDataLocalPath[i],strPathOrigin.c_str());
		strcat(vecStrRadarDataLocalPath[i],"_");
		strcat(vecStrRadarDataLocalPath[i],intToString(i+1).c_str());
		strcat(vecStrRadarDataLocalPath[i],".rd3");
	}

	//ftp端路径	
	onlineTxtFilePath=m_strFtpFolderPath;
	onlineTxtFilePath+="Arrayset.txt";
	onlineCorFilePath=m_strFtpFolderPath;
	onlineCorFilePath+="gps.cor";
	onlineCsvFilePath=m_strFtpFolderPath;
	onlineCsvFilePath+=".csv";
	for(int i=0; i<m_nChannelCountForUpload; i++){
		strcat(vecStrRadarDataHeaderOnlinePath[i],m_strFtpFolderPath.c_str());
		strcat(vecStrRadarDataHeaderOnlinePath[i],"_");
		strcat(vecStrRadarDataHeaderOnlinePath[i],intToString(i+1).c_str());
		strcat(vecStrRadarDataHeaderOnlinePath[i],".rad");
		strcat(vecStrRadarDataOnlinePath[i],m_strFtpFolderPath.c_str());
		strcat(vecStrRadarDataOnlinePath[i],"_");
		strcat(vecStrRadarDataOnlinePath[i],intToString(i+1).c_str());
		strcat(vecStrRadarDataOnlinePath[i],".rd3");
	}
	
	//前置到thread->addData那了
	/*
	std::vector<bool> vecTemp;
	for(int i=0;i<15;i++){//一共15项 txt cor csv rad*6 rd3*6
		vecTemp.push_back(true);
	}
	m_vecVecUploadSuccessFlag.push_back(vecTemp);
	*/

	FILE *fp;
	//按local路径及online路径进行上传
	//上传txt数据
	//if(!(fopen(localTxtFilePath.c_str(),"rb")==NULL)){
	if((fp=fopen(localTxtFilePath.c_str(),"rb"))!=NULL){
		fclose(fp);
		fp=NULL;
		int failCount = 0;
		while(!(FtpPutFileA(hftp,localTxtFilePath.c_str(),onlineTxtFilePath.c_str(), FTP_TRANSFER_TYPE_ASCII, 0))){
			failCount=failCount+1;
			Sleep(1000);
			if(failCount>10){
				stringstream ssPart;
				ssPart<<part;
				string strTemp="上传第"+ssPart.str()+"part"+"txt失败。目标路径:";
				strTemp+=onlineTxtFilePath.c_str();
				strTemp+="\n";
				fprintf(m_fpLog,strTemp.c_str() );
				//AfxMessageBox(StringToCString(strTemp)); 
				((SaveOnlineForRepairmentThread*)_lpThreadSaveOnlineForRepairment)->addPart(localTxtFilePath,onlineTxtFilePath,part);
				m_vecVecUploadSuccessFlag[part-1][0]=false;
				break;
			}
		}
		//m_vecVecUploadSuccessFlag[part-1][0]=true;//得先弄个上传成功的判断
	}else{
		string strTemp="读取";
		strTemp+=localTxtFilePath.c_str();
		strTemp+="失败";
		strTemp+="\n";
		fprintf(m_fpLog,strTemp.c_str() );
		((SaveOnlineForRepairmentThread*)_lpThreadSaveOnlineForRepairment)->addPart(localTxtFilePath,onlineTxtFilePath,part);
		m_vecVecUploadSuccessFlag[part-1][0]=false;
		//AfxMessageBox(StringToCString(strTemp)); 
	}
	if(fp!=NULL){
		fclose(fp);
		fp=NULL;
	}

	//上传cor数据
	//if(!(fopen(localCorFilePath.c_str(),"rb")==NULL)){
	if((fp=fopen(localCorFilePath.c_str(),"rb"))!=NULL){
		fclose(fp);
		fp=NULL;
		int failCount = 0;
		while(!(FtpPutFileA(hftp,localCorFilePath.c_str(),onlineCorFilePath.c_str(), FTP_TRANSFER_TYPE_ASCII, 0))){
			failCount=failCount+1;
			Sleep(1000);
			if(failCount>10){
				stringstream ssPart;
				ssPart<<part;
				string strTemp="上传第"+ssPart.str()+"part"+"cor失败。目标路径:";
				strTemp+=onlineCorFilePath.c_str();
				strTemp+="\n";
				fprintf(m_fpLog,strTemp.c_str() );
				//AfxMessageBox(StringToCString(strTemp)); 
				((SaveOnlineForRepairmentThread*)_lpThreadSaveOnlineForRepairment)->addPart(localCorFilePath,onlineCorFilePath,part);
				m_vecVecUploadSuccessFlag[part-1][1]=false;
				break;
			}
		}
		//m_vecVecUploadSuccessFlag[part-1][1]=true;
	}else{
		string strTemp="读取";
		strTemp+=localCorFilePath.c_str();
		strTemp+="失败";
		strTemp+="\n";
		fprintf(m_fpLog,strTemp.c_str() );
		((SaveOnlineForRepairmentThread*)_lpThreadSaveOnlineForRepairment)->addPart(localCorFilePath,onlineCorFilePath,part);
		m_vecVecUploadSuccessFlag[part-1][1]=false;
		//AfxMessageBox(StringToCString(strTemp)); 
	}
	if(fp!=NULL){
		fclose(fp);
		fp=NULL;
	}

	//上传csv数据
	if((fp=fopen(localCsvFilePath.c_str(),"rb"))!=NULL){
		fclose(fp);
		fp=NULL;
		int failCount = 0;
		while(!(FtpPutFileA(hftp,localCsvFilePath.c_str(),onlineCsvFilePath.c_str(), FTP_TRANSFER_TYPE_ASCII, 0))){
			failCount=failCount+1;
			Sleep(1000);
			if(failCount>10){
				stringstream ssPart;
				ssPart<<part;
				string strTemp="上传第"+ssPart.str()+"part"+"csv失败。目标路径:";
				strTemp+=onlineCsvFilePath.c_str();
				strTemp+="\n";
				fprintf(m_fpLog,strTemp.c_str() );
				//AfxMessageBox(StringToCString(strTemp)); 
				((SaveOnlineForRepairmentThread*)_lpThreadSaveOnlineForRepairment)->addPart(localCsvFilePath,onlineCsvFilePath,part);
				m_vecVecUploadSuccessFlag[part-1][2]=false;
				break;
			}
		}
	}else{
		string strTemp="读取";
		strTemp+=localCsvFilePath.c_str();
		strTemp+="失败";
		strTemp+="\n";
		fprintf(m_fpLog,strTemp.c_str() );
		((SaveOnlineForRepairmentThread*)_lpThreadSaveOnlineForRepairment)->addPart(localCsvFilePath,onlineCsvFilePath,part);
		m_vecVecUploadSuccessFlag[part-1][2]=false;
		//AfxMessageBox(StringToCString(strTemp)); 
	}
	if(fp!=NULL){
		fclose(fp);
		fp=NULL;
	}


	//上传rad数据
	for(int i=0; i<m_nChannelCountForUpload; i++){
		//if(!(fopen(vecStrRadarDataHeaderLocalPath[i],"rb")==NULL)){
		if((fp=fopen(vecStrRadarDataHeaderLocalPath[i],"rb"))!=NULL){
			fclose(fp);
			fp=NULL;
			int failCount = 0;
			while(!(FtpPutFileA(hftp,vecStrRadarDataHeaderLocalPath[i],vecStrRadarDataHeaderOnlinePath[i], FTP_TRANSFER_TYPE_ASCII, 0))){
				failCount=failCount+1;
				Sleep(1000);
				if(failCount>10){
					stringstream ssPart,ssIndex;
					ssPart<<part;
					ssIndex<<(i+1);
					string strTemp="上传第"+ssPart.str()+"part"+"第"+ssIndex.str()+"通道rad失败。目标路径:";
					strTemp+=vecStrRadarDataHeaderOnlinePath[i];
					strTemp+="\n";
					fprintf(m_fpLog,strTemp.c_str() );
					//AfxMessageBox(StringToCString(strTemp)); 
					string localTemp=vecStrRadarDataHeaderLocalPath[i];
					string onlineTemp=vecStrRadarDataHeaderOnlinePath[i];
					((SaveOnlineForRepairmentThread*)_lpThreadSaveOnlineForRepairment)->addPart(localTemp,onlineTemp,part);
					m_vecVecUploadSuccessFlag[part-1][3+i]=false;
					break;
				}
			}
		}else{
			string strTemp="读取";
			strTemp+=vecStrRadarDataHeaderLocalPath[i];
			strTemp+="失败";
			strTemp+="\n";
			fprintf(m_fpLog,strTemp.c_str() );
			string localTemp=vecStrRadarDataHeaderLocalPath[i];
			string onlineTemp=vecStrRadarDataHeaderOnlinePath[i];
			((SaveOnlineForRepairmentThread*)_lpThreadSaveOnlineForRepairment)->addPart(localTemp,onlineTemp,part);
			m_vecVecUploadSuccessFlag[part-1][3+i]=false;
			//AfxMessageBox(StringToCString(strTemp)); 
		}
		if(fp!=NULL){
			fclose(fp);
			fp=NULL;
		}
	}
	//上传rd3数据
	for(int i=0; i<m_nChannelCountForUpload; i++){
		//if(!(fopen(vecStrRadarDataLocalPath[i],"rb")==NULL)){
		if((fp=fopen(vecStrRadarDataLocalPath[i],"rb"))!=NULL){
			fclose(fp);
			fp=NULL;
			int failCount = 0;
			while(!(FtpPutFileA(hftp,vecStrRadarDataLocalPath[i],vecStrRadarDataOnlinePath[i], FTP_TRANSFER_TYPE_BINARY, 0))){
				failCount=failCount+1;
				Sleep(1000);
				if(failCount>10){
					stringstream ssPart,ssIndex;
					ssPart<<part;
					ssIndex<<(i+1);
					string strTemp="上传第"+ssPart.str()+"part"+"第"+ssIndex.str()+"通道rd3失败。目标路径:";
					strTemp+=vecStrRadarDataOnlinePath[i];
					strTemp+="\n";
					fprintf(m_fpLog,strTemp.c_str() );
					string localTemp=vecStrRadarDataLocalPath[i];
					string onlineTemp=vecStrRadarDataOnlinePath[i];
					((SaveOnlineForRepairmentThread*)_lpThreadSaveOnlineForRepairment)->addPart(localTemp,onlineTemp,part);
					m_vecVecUploadSuccessFlag[part-1][9+i]=false;
					//AfxMessageBox(StringToCString(strTemp)); 
					break;
				}
			}
		}else{
			string strTemp="读取";
			strTemp+=vecStrRadarDataLocalPath[i];
			strTemp+="失败";
			strTemp+="\n";
			fprintf(m_fpLog,strTemp.c_str() );
			string localTemp=vecStrRadarDataLocalPath[i];
			string onlineTemp=vecStrRadarDataOnlinePath[i];
			((SaveOnlineForRepairmentThread*)_lpThreadSaveOnlineForRepairment)->addPart(localTemp,onlineTemp,part);
			m_vecVecUploadSuccessFlag[part-1][9+i]=false;
			//AfxMessageBox(StringToCString(strTemp)); 
		}
		if(fp!=NULL){
			fclose(fp);
			fp=NULL;
		}
	}

	//断开ftp句柄
	InternetCloseHandle(hftp);
	InternetCloseHandle(hint);
	
	for(int i=0;i<m_nChannelCountForUpload;i++){
		delete []vecStrRadarDataHeaderOnlinePath[i];
		delete []vecStrRadarDataHeaderLocalPath[i];
		delete []vecStrRadarDataOnlinePath[i];
		delete []vecStrRadarDataLocalPath[i];
	}
	delete []vecStrRadarDataHeaderOnlinePath;
	delete []vecStrRadarDataHeaderLocalPath;
	delete []vecStrRadarDataOnlinePath;
	delete []vecStrRadarDataLocalPath;
}

void MeasureProject::uploadDataForPartForRepairment(std::string strLocalPath, std::string strOnlinePath, int part){
	ConfigureSet *cfg = RadarManager::Instance()->getConfigureSet();
	//获取ftp端信息 ip地址 端口 用户 密码
	string ftpServerIp = cfg->get("ftp", "serverIP");
	int port = atoi( cfg->get("ftp", "port").c_str());
	string account = cfg->get("ftp", "account");
	string password = cfg->get("ftp", "password");

	//获取ftp连接句柄
	HINTERNET hint;
	HINTERNET hftp;
	hint = InternetOpen(0, INTERNET_OPEN_TYPE_PRECONFIG, 0, 0, 0);
	if (hint==NULL)  {   
		AfxMessageBox(L"没网。");
		return;  
	} 
	hftp = InternetConnectA(hint, ftpServerIp.c_str(), port, account.c_str(), password.c_str(), INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
	if (hftp==NULL) {
		AfxMessageBox(L"连接ftp失败。");
		return;
	}
	while(hftp==NULL){
		hftp = InternetConnectA(hint, ftpServerIp.c_str(), port, account.c_str(), password.c_str(), INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
	}

	bool uploadSuccessFlag=true;
	FILE *fp;
	if((fp=fopen(strLocalPath.c_str(),"rb"))!=NULL){
		fclose(fp);
		fp=NULL;
		int failCount = 0;
		if(strLocalPath.substr(strLocalPath.length()-3)=="rd3"){//rd3传2进制的
			while(!(FtpPutFileA(hftp,strLocalPath.c_str(),strOnlinePath.c_str(), FTP_TRANSFER_TYPE_BINARY, 0))){
				failCount=failCount+1;
				Sleep(1000);
				if(failCount>100){
					string strTemp="上传失败。目标路径:";
					strTemp+=strOnlinePath.c_str();
					strTemp+="\n";
					fprintf(m_fpLogForRepairment,strTemp.c_str() );
					uploadSuccessFlag=false;
					//AfxMessageBox(StringToCString(strTemp)); 
					break;
				}
			}
		}else{//其他传2进制
			while(!(FtpPutFileA(hftp,strLocalPath.c_str(),strOnlinePath.c_str(), FTP_TRANSFER_TYPE_ASCII, 0))){
				failCount=failCount+1;
				Sleep(1000);
				if(failCount>100){
					string strTemp="上传失败。目标路径:";
					strTemp+=strOnlinePath.c_str();
					strTemp+="\n";
					fprintf(m_fpLogForRepairment,strTemp.c_str() );
					uploadSuccessFlag=false;
					//AfxMessageBox(StringToCString(strTemp)); 
					break;
				}
			}
		}
	}else{
		string strTemp="读取";
		strTemp+=strLocalPath.c_str();
		strTemp+="失败";
		strTemp+="\n";
		fprintf(m_fpLogForRepairment,strTemp.c_str() );
		uploadSuccessFlag=false;
		//AfxMessageBox(StringToCString(strTemp)); 
	}

	if(fp!=NULL){
		fclose(fp);
		fp=NULL;
	}
	
	if(uploadSuccessFlag){
		int RepairmentIndex=0;
		string fileType=strLocalPath.substr(strLocalPath.length()-3);
		if(fileType=="txt"){
			RepairmentIndex=0;
		}else if(fileType=="cor"){
			RepairmentIndex=1;
		}else if(fileType=="csv"){
			RepairmentIndex=2;
		}else if(fileType=="rad"){
			RepairmentIndex=2+atoi(strLocalPath.substr(strLocalPath.length()-5,1).c_str());
		}else if(fileType=="rd3"){
			RepairmentIndex=8+atoi(strLocalPath.substr(strLocalPath.length()-5,1).c_str());
		}
		m_vecVecUploadSuccessFlag[part-1][RepairmentIndex]=true;
		postEndMessage(part);
	}
}

void MeasureProject::uploadFileToFtp(std::string strLocalPath, std::string strOnlinePath, int part){
	ConfigureSet *cfg = RadarManager::Instance()->getConfigureSet();
	//获取ftp端信息 ip地址 端口 用户 密码
	string ftpServerIp = cfg->get("ftp", "serverIP");
	int port = atoi( cfg->get("ftp", "port").c_str());
	string account = cfg->get("ftp", "account");
	string password = cfg->get("ftp", "password");

	//获取ftp连接句柄
	HINTERNET hint;
	HINTERNET hftp;
	hint = InternetOpen(0, INTERNET_OPEN_TYPE_PRECONFIG, 0, 0, 0);
	if (hint==NULL)  {   
		AfxMessageBox(L"没网。");
		return;  
	} 
	hftp = InternetConnectA(hint, ftpServerIp.c_str(), port, account.c_str(), password.c_str(), INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
	if (hftp==NULL) {
		AfxMessageBox(L"连接ftp失败。");
		return;
	}
	while(hftp==NULL){
		hftp = InternetConnectA(hint, ftpServerIp.c_str(), port, account.c_str(), password.c_str(), INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
	}

	bool uploadSuccessFlag=true;
	FILE *fp;
	if((fp=fopen(strLocalPath.c_str(),"rb"))!=NULL){
		fclose(fp);
		fp=NULL;
		int failCount = 0;
		if(strLocalPath.substr(strLocalPath.length()-3)=="rd3"){//rd3传2进制的
			while(!(FtpPutFileA(hftp,strLocalPath.c_str(),strOnlinePath.c_str(), FTP_TRANSFER_TYPE_BINARY, 0))){
				failCount=failCount+1;
				Sleep(1000);
				if(failCount>100){
					string strTemp="上传失败。目标路径:";
					strTemp+=strOnlinePath.c_str();
					strTemp+="\n";
					fprintf(m_fpLogForRepairment,strTemp.c_str() );
					uploadSuccessFlag=false;
					//AfxMessageBox(StringToCString(strTemp)); 
					break;
				}
			}
		}else{//其他传2进制
			while(!(FtpPutFileA(hftp,strLocalPath.c_str(),strOnlinePath.c_str(), FTP_TRANSFER_TYPE_ASCII, 0))){
				failCount=failCount+1;
				Sleep(1000);
				if(failCount>100){
					string strTemp="上传失败。目标路径:";
					strTemp+=strOnlinePath.c_str();
					strTemp+="\n";
					fprintf(m_fpLogForRepairment,strTemp.c_str() );
					uploadSuccessFlag=false;
					//AfxMessageBox(StringToCString(strTemp)); 
					break;
				}
			}
		}
	}else{
		string strTemp="读取";
		strTemp+=strLocalPath.c_str();
		strTemp+="失败";
		strTemp+="\n";
		fprintf(m_fpLogForRepairment,strTemp.c_str() );
		uploadSuccessFlag=false;
		//AfxMessageBox(StringToCString(strTemp)); 
	}

	if(fp!=NULL){
		fclose(fp);
		fp=NULL;
	}
	
	//你还没弄那个上传完成数组的验证!!!!!!//得上个互斥锁
	if(uploadSuccessFlag){
		postEndMessage(part);
	}
}


void MeasureProject::postEndMessage(int part){
	bool postFlag=true;
	for(int i=0;i<15;i++){
		if(m_vecVecUploadSuccessFlag[part-1][i]==false){
			postFlag=false;
			break;
		}
	}
	if(postFlag==false){
		return;
	}

	int sta_socket;
	struct sockaddr_in remote_ip;
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	sta_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (-1 == sta_socket) {
		closesocket(sta_socket);
	}
	memset(&remote_ip, 0, sizeof(struct sockaddr_in));
	remote_ip.sin_family = AF_INET;
	remote_ip.sin_addr.s_addr = inet_addr(m_strPostServerIp.c_str());
	remote_ip.sin_port = htons(m_nPostPort);
	if (0 != connect(sta_socket, (struct sockaddr*) (&remote_ip), sizeof(struct sockaddr))){
		closesocket(sta_socket);
	}

	time_t mytime = time(NULL);
	string strTimeStamp = intToString(mytime-10);
	char* completeJsonBuff = new char[1024];
	char* completeSendBuff = new char[1024];
	//int nCompleteJsonBuffLen=sprintf(completeJsonBuff, COMPLETE_JSON_FORMAT, m_strOprTab.c_str(), m_strTimeTab.c_str(),  m_strTaskCode.c_str(), m_strEquipmentName.c_str(), strTimeStamp.c_str());
	int nCompleteJsonBuffLen=sprintf(completeJsonBuff, COMPLETE_JSON_FORMAT, m_strOprTab.c_str(), m_vecStrTimeTab[part-1].c_str(),  m_strTaskCode.c_str(), m_strEquipmentName.c_str(), strTimeStamp.c_str());
	sprintf(completeSendBuff, POST_FORMAT, "/externalservice/completeUploadFile.ht", m_strPostServerIp.c_str(), m_nPostPort, intToString(nCompleteJsonBuffLen).c_str(), completeJsonBuff);
	//发送
	int sendLen=-1;
	while(sendLen<0){
		sendLen=send(sta_socket, completeSendBuff, (unsigned int)strlen(completeSendBuff)+1, 0);
	}
	//接收反馈
	char* completeRecieveBuff = new char[1024];
	int nCompleteRecieveBuffLen = recv(sta_socket, completeRecieveBuff, 1024, 0);
	if(nCompleteRecieveBuffLen>0){
		//根据回复参数进行操作
		completeRecieveBuff[nCompleteRecieveBuffLen]='\0';
		string strRecieve=string(completeRecieveBuff);
		string strTemp="上传"+ m_vecStrTimeTab[part-1]+"完成指令发送成功。\n回复内容为：";
		string strTemp2=completeRecieveBuff;
		strTemp=strTemp+strTemp2;
		fprintf(m_fpLog,strTemp.c_str() );
	}else{
		//没有收到回复参数
		string strTemp="上传"+ m_vecStrTimeTab[part-1]+"完成指令发送成功。\n回复内容为空";
		fprintf(m_fpLog,strTemp.c_str() );
	}	
	delete []completeRecieveBuff;
	delete []completeSendBuff;
	delete []completeJsonBuff;
	closesocket(sta_socket);

}

////void MeasureProject::writeEndFile(){
////	USES_CONVERSION;
////	//下面10行构成本地文件路径 格式：path\Nametab\NameTabArrayset.txt
////	std::string strPathTxt = W2A( m_strSavePath );
////	strPathTxt += _projectName;
////	strPathTxt += _projectTab;
////	strPathTxt += "\\";
////	if (0 != access(strPathTxt.c_str(), 0)){
////		mkdir(strPathTxt.c_str());   // 返回 0 表示创建成功，-1 表示失败
////	}
////	strPathTxt += _projectName;
////	strPathTxt += _projectTab;
////	strPathTxt+="complete.txt";
////	
////	//输出当前参数到本地文件中
////	ofstream ofile; 
////	ofile.open(strPathTxt.c_str()); 
////	ofile<<"complete"<<endl;
////	ofile.close();//关闭文件
////}

////void MeasureProject::uploadEndFile(){
////	USES_CONVERSION;
////	//下面10行构成本地文件路径 格式：path\Nametab\complete.txt
////	std::string strPathTxt = W2A( m_strSavePath );
////	strPathTxt += _projectName;
////	strPathTxt += _projectTab;
////	strPathTxt += "\\";
////	if (0 != access(strPathTxt.c_str(), 0)){
////		mkdir(strPathTxt.c_str());   // 返回 0 表示创建成功，-1 表示失败
////	}
////	strPathTxt += _projectName;
////	strPathTxt += _projectTab;
////	strPathTxt+="complete.txt";
////
////	//获取ftp参数 ip 端口 用户 密码
////	ConfigureSet *cfg = RadarManager::Instance()->getConfigureSet();
////	string ftpServerIp = cfg->get("ftp", "serverIP");
////	int port = atoi( cfg->get("ftp", "port").c_str());
////	string account = cfg->get("ftp", "account");
////	string password = cfg->get("ftp", "password");
////
////	//创建ftp连接所需句柄
////	HINTERNET hint;
////	HINTERNET hftp;
////	hint = InternetOpen(0, INTERNET_OPEN_TYPE_PRECONFIG, 0, 0, 0);
////	if (hint==NULL)  {   
////		AfxMessageBox(L"没网。");
////		return;  
////	} 
////	hftp = InternetConnectA(hint, ftpServerIp.c_str(), port, account.c_str(), password.c_str(), INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
////	if (hftp==NULL) {
////		AfxMessageBox(L"连接ftp失败,正在尝试重连");
////		return;
////	}
////	while(hftp==NULL){
////		hftp = InternetConnectA(hint, ftpServerIp.c_str(), port, account.c_str(), password.c_str(), INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
////	}
////
////	std::string strPathOrigin = ".\\radarData\\";
////	strPathOrigin += cfg->get("projectname", "projectName");
////	strPathOrigin += "\\";
////	if(_access(strPathOrigin.c_str(),0)==-1){
////		FtpCreateDirectoryA(hftp,strPathOrigin.c_str());
////	}
////	strPathOrigin += cfg->get("projectname", "equipmentName");
////	strPathOrigin += "\\";
////	if(_access(strPathOrigin.c_str(),0)==-1){
////		FtpCreateDirectoryA(hftp,strPathOrigin.c_str());
////	}
////	//strPathOrigin += m_strStartTimeTab.substr(0,8);
////	strPathOrigin += m_strOprTab;
////	strPathOrigin += "\\";
////	if(_access(strPathOrigin.c_str(),0)==-1){
////		FtpCreateDirectoryA(hftp,strPathOrigin.c_str());
////	}
////	strPathOrigin += m_strTimeTab;
////	strPathOrigin += "\\complete.txt";
////
////	//将本地complete.txt上传至ftp端
////	if(!(fopen(strPathTxt.c_str(),"rb")==NULL)){
////		int failCount=0;
////		while(!FtpPutFileA(hftp,strPathTxt.c_str(),strPathOrigin.c_str(), FTP_TRANSFER_TYPE_ASCII, 0)){
////			failCount=failCount+1;
////			Sleep(1000);
////			if(failCount>10){
////				stringstream ssPart;
////				ssPart<<part;
////				string strTemp="上传第"+ssPart.str()+"part complete.txt失败。目标路径:";
////				strTemp+=strPathOrigin.c_str();
////				AfxMessageBox(StringToCString(strTemp)); 
////				break;
////			}
////		}
////	}else{
////		string strTemp="读取";
////		strTemp+=strPathTxt.c_str();
////		strTemp+="失败";
////		AfxMessageBox(StringToCString(strTemp)); 
////	}
////	InternetCloseHandle(hftp);
////	InternetCloseHandle(hint);
////}


void MeasureProject::initOprTab(){
	/*
	ConfigureSet *cfg = RadarManager::Instance()->getConfigureSet();
	m_strOprTab = cfg->get("ftp", "opr");
	*/
	int sta_socket;
	struct sockaddr_in remote_ip;
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	sta_socket = socket(PF_INET, SOCK_STREAM, 0);
	if (-1 == sta_socket) {
		closesocket(sta_socket);
		Sleep(5000);
		return;
	}
	memset(&remote_ip, 0, sizeof(struct sockaddr_in));
	remote_ip.sin_family = AF_INET;
	remote_ip.sin_addr.s_addr = inet_addr(m_strPostServerIp.c_str());
	remote_ip.sin_port = htons(m_nPostPort);
	if (0 != connect(sta_socket, (struct sockaddr*) (&remote_ip), sizeof(struct sockaddr))){
		closesocket(sta_socket);
		Sleep(5000);
		return;
	}

	Sleep(1000);
	time_t mytime = time(NULL);
	string strTimeStamp = intToString(mytime-10);//-1防止timeout

	osg::Vec3d pos = RadarManager::Instance()->getGpsReader()->getCurPos();
	//std::vector< std::string > vecStrGPGGA;
	//RadarManager::Instance()->getGpsReader()->getCurrentGpsImformation(pos,vecStrGPGGA);
	//RadarManager::Instance()->getGpsReader()->getCurrentGpsImformation(pos,NULL);

	char* initJsonBuff = new char[1024];
	char* initSendBuff = new char[1024];
	//int nInitJsonBuffLen=sprintf(initJsonBuff, INIT_JSON_FORMAT, m_strTaskCode.c_str(), m_strEquipmentName.c_str(), strTimeStamp.c_str());
	int nInitJsonBuffLen=sprintf(initJsonBuff, INIT_JSON_FORMAT, m_strTaskCode.c_str(), m_strEquipmentName.c_str(), strTimeStamp.c_str(), pos.x(), pos.y());
	sprintf(initSendBuff, POST_FORMAT, "/externalservice/initOprCode.ht", m_strPostServerIp.c_str(), m_nPostPort, intToString(nInitJsonBuffLen).c_str(), initJsonBuff);
	//发送
	int sendLen=-1;
	while(sendLen<0){
		sendLen=send(sta_socket, initSendBuff, (unsigned int)strlen(initSendBuff)+1, 0);
	}
	//接收反馈
	char* initRecieveBuff = new char[1024];
	int nInitRecieveBuffLen = recv(sta_socket, initRecieveBuff, 1024, 0);
	if(nInitRecieveBuffLen>0){
		initRecieveBuff[nInitRecieveBuffLen]='\0';
		string strRecieve=string(initRecieveBuff);
		int postion3=strRecieve.find("result");
		string result=strRecieve.substr(postion3+9,4);
		if(result=="succ"){
			m_nInitOpr=1;
			int postion1=strRecieve.find("oprcode");
			int postion2=strRecieve.find("\"",postion1+12);
			m_strOprTab = strRecieve.substr(postion1+10,postion2-postion1-10);
		}else{
			m_nInitOpr=0;
		}
		
		//根据回复参数进行操作
	}else{
		//没有收到回复参数
	}
	delete []initRecieveBuff;
	delete []initSendBuff;
	delete []initJsonBuff;
	closesocket(sta_socket);
	Sleep(5000);
}
/*
void MeasureProject::setStartTimeTab(){
	time_t nowtime;
	struct tm* p;
	time(&nowtime);
	p = localtime(&nowtime);
	std::string timeTab = intToString(p->tm_year+1900);
	if(p->tm_mon+1<10){
		timeTab=timeTab+"0"+intToString(p->tm_mon+1);
	}else{
		timeTab=timeTab+intToString(p->tm_mon+1);
	}
	if(p->tm_mday<10){
		timeTab=timeTab+"0"+intToString(p->tm_mday);
	}else{
		timeTab=timeTab+intToString(p->tm_mday);
	}
	if(p->tm_hour==0){
		timeTab=timeTab+"00";
	}else if(p->tm_hour<10){
		timeTab=timeTab+"0"+intToString(p->tm_hour);
	}else{
		timeTab=timeTab+intToString(p->tm_hour);
	}
	if(p->tm_min==0){
		timeTab=timeTab+"00";
	}else if(p->tm_min<10){
		timeTab=timeTab+"0"+intToString(p->tm_min);
	}else{
		timeTab=timeTab+intToString(p->tm_min);
	}
	if(p->tm_sec==0){
		timeTab=timeTab+"00";
	}else if(p->tm_sec<10){
		timeTab=timeTab+"0"+intToString(p->tm_sec);
	}else{
		timeTab=timeTab+intToString(p->tm_sec);
	}
	m_strStartTimeTab=timeTab;
}*/

void MeasureProject::changeTimeTab(){
	time_t nowtime;
	struct tm* p;
	time(&nowtime);
	p = localtime(&nowtime);
	std::string timeTab = intToString(p->tm_year+1900);
	if(p->tm_mon+1<10){
		timeTab=timeTab+"0"+intToString(p->tm_mon+1);
	}else{
		timeTab=timeTab+intToString(p->tm_mon+1);
	}
	if(p->tm_mday<10){
		timeTab=timeTab+"0"+intToString(p->tm_mday);
	}else{
		timeTab=timeTab+intToString(p->tm_mday);
	}
	if(p->tm_hour==0){
		timeTab=timeTab+"00";
	}else if(p->tm_hour<10){
		timeTab=timeTab+"0"+intToString(p->tm_hour);
	}else{
		timeTab=timeTab+intToString(p->tm_hour);
	}
	if(p->tm_min==0){
		timeTab=timeTab+"00";
	}else if(p->tm_min<10){
		timeTab=timeTab+"0"+intToString(p->tm_min);
	}else{
		timeTab=timeTab+intToString(p->tm_min);
	}
	if(p->tm_sec==0){
		timeTab=timeTab+"00";
	}else if(p->tm_sec<10){
		timeTab=timeTab+"0"+intToString(p->tm_sec);
	}else{
		timeTab=timeTab+intToString(p->tm_sec);
	}
	m_strTimeTab=timeTab;
	m_vecStrTimeTab.push_back(timeTab);
}

//根据刚更新的timeTab重新合成新的ftp
void MeasureProject::changeFtpFolderPath(){
	ConfigureSet *cfg = RadarManager::Instance()->getConfigureSet();
	//获取ftp端信息 ip地址 端口 用户 密码
	string ftpServerIp = cfg->get("ftp", "serverIP");
	int port = atoi( cfg->get("ftp", "port").c_str());
	string account = cfg->get("ftp", "account");
	string password = cfg->get("ftp", "password");
	//获取ftp连接句柄
	HINTERNET hint;
	HINTERNET hftp;
	hint = InternetOpen(0, INTERNET_OPEN_TYPE_PRECONFIG, 0, 0, 0);
	if (hint==NULL)  {   
		AfxMessageBox(L"没网。");
		return;  
	}
	hftp = InternetConnectA(hint, ftpServerIp.c_str(), port, account.c_str(), password.c_str(), INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
	if (hftp==NULL) {
		AfxMessageBox(L"连接ftp失败。");
		return;
	}
	while(hftp==NULL){
		hftp = InternetConnectA(hint, ftpServerIp.c_str(), port, account.c_str(), password.c_str(), INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
	}

	m_strFtpFolderPath="";
	//std::string strPathOrigin = ".\\";
	std::string strPathOrigin = ".\\radarData\\";
	strPathOrigin += cfg->get("projectname", "projectName");
	strPathOrigin += "\\";
	if(_access(strPathOrigin.c_str(),0)==-1){
		FtpCreateDirectoryA(hftp,strPathOrigin.c_str());
	}
	/*
	strPathOrigin += cfg->get("projectname", "roadname");
	strPathOrigin += "\\";
	if(_access(strPathOrigin.c_str(),0)==-1){
		FtpCreateDirectoryA(hftp,strPathOrigin.c_str());
	}
	*/
	strPathOrigin += cfg->get("projectname", "equipmentName");
	strPathOrigin += "\\";
	if(_access(strPathOrigin.c_str(),0)==-1){
		FtpCreateDirectoryA(hftp,strPathOrigin.c_str());
	}
	//strPathOrigin += m_strTimeTab.substr(0,8);
	//strPathOrigin += m_strStartTimeTab.substr(0,8);
	strPathOrigin += m_strOprTab;
	strPathOrigin += "\\";
	if(_access(strPathOrigin.c_str(),0)==-1){
		FtpCreateDirectoryA(hftp,strPathOrigin.c_str());
	}
	strPathOrigin += m_strTimeTab;
	strPathOrigin += "\\";
	if(_access(strPathOrigin.c_str(),0)==-1){
		FtpCreateDirectoryA(hftp,strPathOrigin.c_str());
	}
	strPathOrigin += m_strTimeTab;
	m_strFtpFolderPath=strPathOrigin;

	InternetCloseHandle(hftp);
	InternetCloseHandle(hint);
}
	