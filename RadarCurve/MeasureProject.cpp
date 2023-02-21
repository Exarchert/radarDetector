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
#include "TERDataWriter.h"
#include "SGYWriter.h"
#include <ctime>
#include <direct.h>//���������ļ���
#include <io.h>
#include <windows.h>//���������ļ���
#include <afxstr.h>//CStringͷ�ļ�
//ftp
#include <wininet.h>//����
#pragma comment(lib,"wininet.lib")//����
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
}"//��lang lont�ǽӿڹ涨�ı��
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

//=========================================sha256�����㷨=========================================//
//=========================================sha256�����㷨=========================================//
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
//=========================================sha256�����㷨=========================================//
//=========================================sha256�����㷨=========================================//

#define THREAD_MAX_DATA 5000


class SaveRadarFileDataCmd : public SaveDataCmd{
	public:
		SaveRadarFileDataCmd(){};
		osg::ref_ptr<RadarData> _lpData;
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
						_SGYWriter->writeKml(_lpData.get());
					}
				}
			}
			_lpData = NULL;
		}
};

class SaveTERFileDataCmd : public SaveDataCmd{
	public:
		SaveTERFileDataCmd(){};
		osg::ref_ptr<TERData> _lpTERData;
		virtual void exec( std::string const& projectTab ){};
		virtual void exec(TERDataWriter* _TERDataWriter){
			if (_TERDataWriter){
				_TERDataWriter->writeUepData(_lpTERData.get());
			}
			_lpTERData = NULL;
		}
};

//===================================================================class SaveTERDataThread ���ݱ��ر����߳�================================================================//
//===================================================================class SaveTERDataThread ���ݱ��ر����߳�================================================================//
class SaveTERDataThread : public OpenThreads::Thread{
	/*public:
		SaveTERDataThread(){};
		~SaveTERDataThread(){};

		void run(){
			while( !testCancel() ){
				std::vector<osg::ref_ptr<TERData> > tmpQueue;
				{
					OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
					tmpQueue.swap( _TERDataQueue );//swap����ʵ�ֽ�����������������Ԫ�صĹ���
				}
				if ( tmpQueue.size() > 0 ){
					//unsigned long startTime = GetTickCount();
					for ( std::vector<osg::ref_ptr<TERData> >::iterator it = tmpQueue.begin(); it != tmpQueue.end(); it++ ){
						if (_TERDataWriter){
							_TERDataWriter->writeUepData((*it).get());
						}
						*it = NULL;						
					}
				}
				if ( _TERDataQueue.size() == 0 ){
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
			//��ն���
			std::vector<osg::ref_ptr<TERData> > tmpQueue;
			{
				OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
				tmpQueue.swap( _TERDataQueue );//swap����ʵ�ֽ�����������������Ԫ�صĹ���
			}
			if ( isRunning() ){
				_block.reset();
				_block.release();
				join();
			}
			return 0;
		};

		void addData( TERData *data ){
			OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
			_TERDataQueue.push_back( data );

			_block.reset();
			_block.release();
		};

	public:
		OpenThreads::Mutex _mutex;
		OpenThreads::Block _block;
		std::vector<osg::ref_ptr<TERData>> _TERDataQueue;
		TERDataWriter* _TERDataWriter;*/
	public:
		SaveTERDataThread(){};
		~SaveTERDataThread(){};

		void run(){
			while( !testCancel() ){
				CmdQueue tmpQueue;
				{
					OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
					tmpQueue.swap( _TERDataQueue );//swap����ʵ�ֽ�����������������Ԫ�صĹ���
				}
				if ( tmpQueue.size() > 0 ){
					//unsigned long startTime = GetTickCount();
					for ( CmdQueue::iterator it = tmpQueue.begin(); it != tmpQueue.end(); it++ ){
						SaveDataCmd *lpCmd = (*it).get();
						if ( lpCmd ){
							lpCmd->exec( _TERDataWriter );
						}
					}
				}
				if ( _TERDataQueue.size() == 0 ){
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
			//��ն���
			CmdQueue tmpQueue;
			{
				OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
				tmpQueue.swap( _TERDataQueue );//swap����ʵ�ֽ�����������������Ԫ�صĹ���
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
			_TERDataQueue.push_back( data );

			_block.reset();
			_block.release();
		};

	public:
		OpenThreads::Mutex _mutex;
		OpenThreads::Block _block;
		typedef std::vector<osg::ref_ptr<SaveDataCmd> > CmdQueue;
		CmdQueue _TERDataQueue;
		TERDataWriter* _TERDataWriter;

};
//===================================================================class SaveTERDataThread ���ݱ��ر����߳�================================================================//
//===================================================================class SaveTERDataThread ���ݱ��ر����߳�================================================================//

//===================================================================class SaveFileDataThread ���ݱ��ر����߳�================================================================//
//===================================================================class SaveFileDataThread ���ݱ��ر����߳�================================================================//
class SaveFileDataThread : public OpenThreads::Thread{
	public:
		SaveFileDataThread(){};
		~SaveFileDataThread(){};

		void run(){
			while( !testCancel() ){
				CmdQueue tmpQueue;
				{
					OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
					tmpQueue.swap( _dataQueue );//swap����ʵ�ֽ�����������������Ԫ�صĹ���
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
			//��ն���
			CmdQueue tmpQueue;
			{
				OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
				tmpQueue.swap( _dataQueue );//swap����ʵ�ֽ�����������������Ԫ�صĹ���
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

		//20200306hjl ���ڲ����ֻع�
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
//===================================================================class SaveFileDataThread ���ݱ��ر����߳�================================================================//
//===================================================================class SaveFileDataThread ���ݱ��ر����߳�================================================================//


//===================================================================class AutoCorrectionCmd ���ݼ���߳�================================================================//
//===================================================================class AutoCorrectionCmd ���ݼ���߳�================================================================//
class AutoCorrectionCmd : public OpenThreads::Thread{
	public:
		AutoCorrectionCmd(){};
		~AutoCorrectionCmd(){};

		void run(){
			while( !testCancel() ){
				std::vector<osg::ref_ptr<RadarData>> tempQueue;
				{
					OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
					tempQueue.swap( _radarDataQueue );//swap����ʵ�ֽ�����������������Ԫ�صĹ���
				}
				if ( tempQueue.size() > 0 ){
					//unsigned long startTime = GetTickCount();
					//std::vector<std::vector<osg::ref_ptr<RadarData>>>::reverse_iterator it;
					//std::vector<std::vector<osg::ref_ptr<RadarData>>> m_vecRadarGroupData
					for ( std::vector<osg::ref_ptr<RadarData>>::iterator it = tempQueue.begin(); it != tempQueue.end(); it++ ){
						RadarData *pRadarData = (*it).get();
						if ( pRadarData ){
							if (_MeasureProject){
								_MeasureProject->AutoCorrection(pRadarData);
							}
							pRadarData = NULL;
						}
					}
				}
				if ( _radarDataQueue.size() == 0 ){
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
			//��ն���
			std::vector<osg::ref_ptr<RadarData>> tempQueue;
			{
				OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
				tempQueue.swap( _radarDataQueue );//swap����ʵ�ֽ�����������������Ԫ�صĹ���
			}
			if ( isRunning() ){
				_block.reset();
				_block.release();
				join();
			}
			return 0;
		};

		void addData( RadarData *data ){
			OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
			_radarDataQueue.push_back( data );

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
		std::vector<osg::ref_ptr<RadarData>> _radarDataQueue;
};
//===================================================================class AutoCorrectionCmd ���ݼ���߳�================================================================//
//===================================================================class AutoCorrectionCmd ���ݼ���߳�================================================================//



//===================================================================class CheckDataThread ���ݼ���߳�================================================================//
//===================================================================class CheckDataThread ���ݼ���߳�================================================================//
class CheckDataThread : public OpenThreads::Thread{
	public:
		CheckDataThread(){};
		~CheckDataThread(){};

		void run(){
			while( !testCancel() ){
				std::vector<osg::ref_ptr<RadarData>> tempQueue;
				{
					OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
					tempQueue.swap( _radarDataQueue );//swap����ʵ�ֽ�����������������Ԫ�صĹ���
				}
				if ( tempQueue.size() > 0 ){
					//unsigned long startTime = GetTickCount();
					//std::vector<std::vector<osg::ref_ptr<RadarData>>>::reverse_iterator it;
					//std::vector<std::vector<osg::ref_ptr<RadarData>>> m_vecRadarGroupData
					for ( std::vector<osg::ref_ptr<RadarData>>::iterator it = tempQueue.begin(); it != tempQueue.end(); it++ ){
						RadarData *pRadarData = (*it).get();
						if ( pRadarData ){
							if (_MeasureProject){
								_MeasureProject->checkRadarData(pRadarData);
							}
							pRadarData = NULL;
						}
					}
				}
				if ( _radarDataQueue.size() == 0 ){
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
			//��ն���
			std::vector<osg::ref_ptr<RadarData>> tempQueue;
			{
				OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
				tempQueue.swap( _radarDataQueue );//swap����ʵ�ֽ�����������������Ԫ�صĹ���
			}
			if ( isRunning() ){
				_block.reset();
				_block.release();
				join();
			}
			return 0;
		};

		void addData( RadarData *data ){
			OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
			_radarDataQueue.push_back( data );

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
		std::vector<osg::ref_ptr<RadarData>> _radarDataQueue;
};
//===================================================================class CheckDataThread ���ݼ���߳�================================================================//
//===================================================================class CheckDataThread ���ݼ���߳�================================================================//



//===================================================================class UploadFileToFtpThread �ϴ��߳�================================================================//
//===================================================================class UploadFileToFtpThread �ϴ��߳�================================================================//
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
			//��ն���
			CmdQueue tmpQueue;
			{
				OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
				tmpQueue.swap( _dataQueue );//swap����ʵ�ֽ�����������������Ԫ�صĹ���
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
			_localPathQueue.push_back( localPath );//����·��
			_OnlinePathQueue.push_back( onlinePath );//�ϴ�·��
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
//===================================================================class UploadFileToFtpThread �ϴ��߳�================================================================//
//===================================================================class UploadFileToFtpThread �ϴ��߳�================================================================//

//===================================================================class SaveOnlineThread �ϴ��߳�================================================================//
//===================================================================class SaveOnlineThread �ϴ��߳�================================================================//
class SaveOnlineThread : public OpenThreads::Thread{
	public:
		SaveOnlineThread(){};
		~SaveOnlineThread(){};

		void run(){
			while( !testCancel() ){
				std::vector<int> tmpQueue;//¼�뱾�����е�����Ԫ
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
						Sleep(1000);
						_MeasureProject->changeFtpFolderPath();
						_MeasureProject->writeRadFileForPart( *it );
						//_MeasureProject->uploadTxtFileForGroup( *it );
						_MeasureProject->uploadDataForPart( *it );
						//_MeasureProject->postEndMessage( *it );
						//_MeasureProject->m_nFinishedPart = *it;
						/*
						bool postFlag=true;
						for(int i=0;i<15;i++){
							if(_MeasureProject->m_vecVecUploadSuccessFlag[part-1][i]==false){
								postFlag=false;
								break;
							}
						}
						if(postFlag==true){
							m_vecbUploadConditionForEachPart[part-1]=true;
							_MeasureProject->addDataToPostFinishedMessageThread(*it);
						}*/
						if(_MeasureProject->isDataInPartUploaded(*it)){
							_MeasureProject->addDataToPostFinishedMessageThread(*it);
						}
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
			//��ն���
			CmdQueue tmpQueue;
			{
				OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
				tmpQueue.swap( _dataQueue );//swap����ʵ�ֽ�����������������Ԫ�صĹ���
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
//===================================================================class SaveOnlineThread �ϴ��߳�================================================================//
//===================================================================class SaveOnlineThread �ϴ��߳�================================================================//

//��һ���ϴ�ʧ��ʱ���������ϴ���
//===================================================================class SaveOnlineForRepairmentThread �ϴ��߳�================================================================//
//===================================================================class SaveOnlineForRepairmentThread �ϴ��߳�================================================================//
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
			//��ն���
			CmdQueue tmpQueue;
			{
				OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
				tmpQueue.swap( _dataQueue );//swap����ʵ�ֽ�����������������Ԫ�صĹ���
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

		//void agddData( SaveDataCmd *data ){
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
//===================================================================class SaveOnlineForRepairmentThread �ϴ��߳�================================================================//
//===================================================================class SaveOnlineForRepairmentThread �ϴ��߳�================================================================//



//===================================================================class UploadGpsOnlineThread �ϴ��߳�================================================================//
//===================================================================class UploadGpsOnlineThread �ϴ��߳�================================================================//
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
			//��ն���
			CmdQueue tmpQueue;
			{
				OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
				tmpQueue.swap( _dataQueue );//swap����ʵ�ֽ�����������������Ԫ�صĹ���
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
//===================================================================class UploadGpsOnlineThread �ϴ��߳�================================================================//
//===================================================================class UploadGpsOnlineThread �ϴ��߳�================================================================//

//===================================================================class PostFinishedMessageThread �ϴ��߳�================================================================//
//===================================================================class PostFinishedMessageThread �ϴ��߳�================================================================//
class PostFinishedMessageThread : public OpenThreads::Thread{
	public:
		PostFinishedMessageThread(){};
		~PostFinishedMessageThread(){};

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
						_MeasureProject->postEndMessage( *it );
						//_MeasureProject->m_nFinishedPart = *it;
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

		void addPart( int part ){
			OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
			_partQueue.push_back( part );

			_block.reset();
			_block.release();
		};

		//bool init(const char *dbsrc, const char *dbname, const char *user, const char *pass, std::string &projectName);
	public:
		OpenThreads::Mutex _mutex;
		OpenThreads::Block _block;

		MeasureProject* _MeasureProject;
		std::vector<int> _partQueue;
};
//===================================================================class PostFinishedMessageThread �ϴ��߳�================================================================//
//===================================================================class PostFinishedMessageThread �ϴ��߳�================================================================//


//===================================================================class SaveDataThread ���ݿ��õ�================================================================//
//===================================================================class SaveDataThread ���ݿ��õ�================================================================//
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
//===================================================================class SaveDataThread ���ݿ��õ�================================================================//
//===================================================================class SaveDataThread ���ݿ��õ�================================================================//

//===================================================================class CopyLocalThread ���ظ����߳�================================================================//
//===================================================================class CopyLocalThread ���ظ����߳�================================================================//
class CopyLocalThread : public OpenThreads::Thread{
	public:
		CopyLocalThread(){};
		~CopyLocalThread(){};

		void run(){
			while( !testCancel() ){
				std::vector<int> tmpQueue;//¼�뱾�����е�����Ԫ
				{
					OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
					tmpQueue.swap( _partQueue );
					//_partQueue.clear();
				}
				if ( tmpQueue.size() > 0 ){
					//unsigned long startTime = GetTickCount();
					for ( std::vector<int>::iterator it = tmpQueue.begin(); it != tmpQueue.end(); it++ ){
						Sleep(1000);
						_MeasureProject->copyRadarDataForPart( *it );
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
			//��ն���
			CmdQueue tmpQueue;
			{
				OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
				tmpQueue.swap( _dataQueue );//swap����ʵ�ֽ�����������������Ԫ�صĹ���
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
//===================================================================class CopyLocalThread �ϴ��߳�================================================================//
//===================================================================class CopyLocalThread �ϴ��߳�================================================================//


//===========================================================================��ȡ����ʱ��=====================================================================//
//===========================================================================��ȡ����ʱ��=====================================================================//
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

SYSTEMTIME FormatServerTime(DWORD serverTime){
	FILETIME      ftNew;
	SYSTEMTIME    stNew;

	stNew.wYear = 1900;
	stNew.wMonth = 1;
	stNew.wDay = 1;
	stNew.wHour = 0;
	stNew.wMinute = 0;
	stNew.wSecond = 0;
	stNew.wMilliseconds = 0;
	::SystemTimeToFileTime(&stNew, &ftNew);

	LARGE_INTEGER li;			//64λ������
	li = *(LARGE_INTEGER *)&ftNew;
	li.QuadPart += (LONGLONG)10000000 * serverTime;
	ftNew = *(FILETIME *)&li;
	::FileTimeToSystemTime(&ftNew, &stNew);
	// ����ʱ�䣨ע�⣺���ﷵ�ص��Ǹ�������ʱ�䣬�뱱��ʱ�����8Сʱ��
	return stNew;
}

struct NTPPacket
{
	union
	{
		struct _ControlWord
		{
			unsigned int uLI : 2;       // 00 = no leap, clock ok   
			unsigned int uVersion : 3;  // version 3 or version 4
			unsigned int uMode : 3;     // 3 for client, 4 for server, etc.
			unsigned int uStratum : 8;  // 0 is unspecified, 1 for primary reference system,
										// 2 for next level, etc.
			int nPoll : 8;              // seconds as the nearest power of 2
			int nPrecision : 8;         // seconds to the nearest power of 2
		};

		int nControlWord;             // 4
	};

	int nRootDelay;                   // 4
	int nRootDispersion;              // 4
	int nReferenceIdentifier;         // 4

	__int64 n64ReferenceTimestamp;    // 8
	__int64 n64OriginateTimestamp;    // 8
	__int64 n64ReceiveTimestamp;      // 8

	int nTransmitTimestampSeconds;    // 4
	int nTransmitTimestampFractions;  // 4
};

int Get_time_t(time_t & ttime)
{
	ttime = 0;
	WSADATA wsaData;
	// Initialize Winsock
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0){
		return 0;
	}
	int result, count;
	int sockfd = 0, rc;
	sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sockfd < 0){
		return 0;
	}
	fd_set pending_data;
	timeval block_time;
	NTPPacket ntpSend = { 0 };
	ntpSend.nControlWord = 0x1B;
	NTPPacket ntpRecv;
	SOCKADDR_IN addr_server;
	addr_server.sin_family = AF_INET;
	addr_server.sin_port = htons(123);//NTP����Ĭ��Ϊ123�˿ں�
	addr_server.sin_addr.S_un.S_addr = inet_addr("120.25.115.20"); //�õ�ַΪ������NTP�������Ĺ�����ַ������NTP��������ַ�����аٶ�������
	SOCKADDR_IN sock;
	int len = sizeof(sock);
	if ((result = sendto(sockfd, (const char*)&ntpSend, sizeof(NTPPacket), 0, (SOCKADDR *)&addr_server, sizeof(SOCKADDR))) < 0){
		int err = WSAGetLastError();
		return 0;
	}
	FD_ZERO(&pending_data);
	FD_SET(sockfd, &pending_data);
	//timeout 10 sec
	block_time.tv_sec = 10;
	block_time.tv_usec = 0;
	if (select(sockfd + 1, &pending_data, NULL, NULL, &block_time) > 0)
	{
		//��ȡ��ʱ��Ϊ1900��1��1�յ����ڵ�����
		if ((count = recvfrom(sockfd, (char*)&ntpRecv, sizeof(NTPPacket), 0, (SOCKADDR*)&sock, &len)) > 0)
			ttime = ntohl(ntpRecv.nTransmitTimestampSeconds);
	}
	closesocket(sockfd);
	WSACleanup();
	return ttime;
}


int getOnlineTime(){
	int dLocalTime = time(NULL);
	time_t  t;
	time_t tOnlineTime = -1;
	while (tOnlineTime==-1){
		Get_time_t(t);
		SYSTEMTIME d = FormatServerTime(t);
		
		tm tmTemp;
		tmTemp.tm_year = d.wYear-1900;
		tmTemp.tm_mon = d.wMonth-1;
		tmTemp.tm_mday = d.wDay;
		tmTemp.tm_hour = d.wHour;
		/*if(d.wHour<16){
			tmTemp.tm_mday = d.wDay;
			tmTemp.tm_hour = d.wHour+8;
		}else{
			tmTemp.tm_mday = d.wDay+1;
			tmTemp.tm_hour = d.wHour-16;
		}*/
		tmTemp.tm_min = d.wMinute;
		tmTemp.tm_sec = d.wSecond;
		tmTemp.tm_isdst = 0;
		tOnlineTime = mktime(&tmTemp);
	}
	
	int dOnlineTime=tOnlineTime+8*60*60;
	return dOnlineTime;
}
//===========================================================================��ȡ����ʱ��=====================================================================//
//===========================================================================��ȡ����ʱ��=====================================================================//

/*void test(){
	int a=0,b=3,c=3;
	a=b+c;
}

void test2(){
	int a=0,b=3,c=3;
	a=b+c;
}*/


//////////////////////////////////////////////////////////////////////////
MeasureProject::MeasureProject(void)
{
	//_lpThread = NULL;
	

	m_dGpsTimeStamp=0;
	m_fLastGpsLen = 0.0f;
	m_fLastPostedGpsLen=0.0f;
	_lastLen = 0.0f;
	//_recordTotalWheelCount = 0;
	//_realTotalWheelCount = 0;
	m_nVecOriginWheelCountForPart.clear();
	m_nVecOriginWheelCountForPart.push_back(0);

	m_vecVecUploadSuccessFlag.clear();
	m_vecVecProcessFinishedFlag.clear();
	
	m_nInitOpr = 0;
	m_dRoadPart = 1;
	//m_nFinishedPart = 1;
	ConfigureSet *cfg = RadarManager::Instance()->getConfigureSet();
	m_nChannelCount = RadarManager::Instance()->GetTrueChannelCount();
	m_nChannelCountForUpload = atoi( cfg->get("radar", "channelCountForUpload").c_str() );
	m_nTimeInterval = atoi( cfg->get("radar", "timeInterval").c_str() );
	m_nWheelCountInterval = atoi( cfg->get("radar", "wheelCountInterval").c_str() );
	m_nUpload = atoi( cfg->get("radar", "uploadFlag").c_str() );
	//m_dTwelveFlag=atoi( cfg->get("radar", "twelveFlag").c_str() );
	m_dSeperateType = atoi( cfg->get("radar", "autoSeperateType").c_str() );
	m_nSaveFileType = atoi( cfg->get("radar", "saveFileType").c_str() );
	for(int i=0;i<8;i++){
		std::stringstream ss;
		ss << i;
		m_nArr2DChannel[i]=atoi(cfg->get("2DchannelCheckBox", ss.str()).c_str());
	}
	
	m_dWaveWaringInterval =  atoi( cfg->get("waveWarning", "warningInterval").c_str() );
	m_dWaveWaringThreshold =  atoi( cfg->get("waveWarning", "warningThreshold").c_str() );

	m_strTaskCode = cfg->get("projectname", "projectName");
	m_strEquipmentName = cfg->get("projectname", "equipmentName");
	m_strPostServerIp = cfg->get("post", "serverIP");
	m_nPostPort = atoi( cfg->get("post", "port").c_str() );

	m_nCopyFinishedFlag=0;


	m_nVecVecAutoCorrectionPointIndex.clear();
	//m_nVecAutoCorrectionPointIndexCount.clear();
	m_nVecAutoCorrectionResult.clear();
	for(int i=0;i<m_nChannelCount;i++){
		std::vector<int> vecTemp;
		/*for(int j=0;j<100;j++){
			vecTemp.push_back(-1);
		}*/
		m_nVecVecAutoCorrectionPointIndex.push_back(vecTemp);
		//m_nVecAutoCorrectionPointIndexCount.push_back(0);
		m_nVecAutoCorrectionResult.push_back(-1);
	}
	


	for(int i=0;i<m_nChannelCount;i++){
		_setMark[i] = false;
		m_arrnRecordTotalWheelCount[i]=0;
		m_arrnRealTotalWheelCount[i]=0;
	}

	_maxID = 0;
	m_bIsBegin = false;

	//m_bIsAdd = false;

	m_iRadarWorkType = 8;//
	m_iSaveOracle = 1;//
	m_strSavePath = L"c:\\";

	for(int i=0;i<m_nChannelCount;i++){
		_lpThreadSaveFile[i] = NULL;
		
		m_SGYWriter[i] = NULL;
	}

	_lpThreadAutoCorrection = NULL;
	_lpThreadCopyData=NULL;
	_lpThreadCheckData=NULL;
	_lpThreadSaveOnline=NULL;
	_lpThreadPostFinishedMessage=NULL;
	_lpThreadSaveOnlineForRepairment=NULL;
	_lpThreadUploadGps=NULL;

	m_bTERIsBegin = NULL;
	m_TERDataWriter = NULL;
	m_ThreadTERSaveFile = NULL;

}

//�������gps�����ļ���� ��ͨ������gpswriter�����Ż�
int useIt(int num){
	int flag=1;
	ConfigureSet *cfg = RadarManager::Instance()->getConfigureSet();
	int nTemp[8];
	for(int i=0;i<8;i++){
		std::stringstream ss;
		ss << i;
		nTemp[i]=atoi(cfg->get("2DchannelCheckBox", ss.str()).c_str());
	}
	for(int i=0;i<num;i++){
		if(nTemp[i]==1){
			flag=0;
			break;
		}
	}
	return flag;
}

//int ת string
string intToString(int n){
	std::stringstream ss;
	ss<<n;
	return ss.str();
}

//StringתCString
CString StringToCString( std::string str ){
	USES_CONVERSION;
	CString cstr = A2T(str.c_str());
	return cstr;
}

//ɾ�߳� �ͷ�ָ�� ����ϴ�����ȷ���ϴ������һ������
MeasureProject::~MeasureProject(void){
	
	if (m_bTERIsBegin==true){
		if (m_ThreadTERSaveFile){
			m_ThreadTERSaveFile->cancel();
			delete m_ThreadTERSaveFile;
			m_ThreadTERSaveFile = NULL;
		}
		if(m_TERDataWriter){
			m_TERDataWriter->writeUepHead();
			m_TERDataWriter->copyUepData();
			m_TERDataWriter->writeUepTail();
			m_TERDataWriter->closeUep();
			delete m_TERDataWriter;
			m_TERDataWriter = NULL;
		}
		AfxMessageBox(L"����TER������ɡ�");
		m_bTERIsBegin = false;
	}

	if(m_nSaveFileType==0){//sgy
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

		for(int i =0;i<m_nChannelCount;i++){
			if (_lpThreadSaveFile[i]){
				_lpThreadSaveFile[i]->cancel();
				delete _lpThreadSaveFile[i];
				_lpThreadSaveFile[i] = NULL;
			}
		}
		
		
		/*
		if (_lpThreadSaveFileA){
			_lpThreadSaveFileA->cancel();
			delete _lpThreadSaveFileA;
			_lpThreadSaveFileA = NULL;
		}
		if (_lpThreadSaveFileB){
			_lpThreadSaveFileB->cancel();
			delete _lpThreadSaveFileB;
			_lpThreadSaveFileB = NULL;
		}*/
		

		if (true == m_bIsBegin){
			for(int i =0;i<m_nChannelCount;i++){
				if (m_SGYWriter[i]){
					m_SGYWriter[i]->closeSGY();
					if ( RadarManager::Instance()->getExportGpsPos() ){
						if(useIt(i)){
							m_SGYWriter[i]->closeGPS();
						}
					}
					if ( RadarManager::Instance()->getExportKml() ){
						if(useIt(i)){
							m_SGYWriter[i]->writeKmlEnding();
							m_SGYWriter[i]->closeKml();
						}
					}
				}
			}
			
			for(int i=0; i<m_nChannelCount; i++){
				delete m_SGYWriter[i];
				m_SGYWriter[i] = NULL;
			}
			
			//writeDocumentFile();
			writeNameCsvFile();
			AfxMessageBox(L"����������ɡ�");
			m_bIsBegin = false;
		}
	}else if(m_nSaveFileType==1){//rd3
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

		for(int i =0;i<m_nChannelCount;i++){
			if (_lpThreadSaveFile[i]){
				_lpThreadSaveFile[i]->cancel();
				delete _lpThreadSaveFile[i];
				_lpThreadSaveFile[i] = NULL;
			}
		}

		if (_lpThreadAutoCorrection){
			_lpThreadAutoCorrection->cancel();
			delete _lpThreadAutoCorrection;
			_lpThreadAutoCorrection = NULL;
		}

		if (true == m_bIsBegin){
			for(int i =0;i<m_nChannelCount;i++){
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

			for(int i=0; i<m_nChannelCount; i++){
				if (m_SGYWriter[i]){
					delete m_SGYWriter[i];
					m_SGYWriter[i] = NULL;
				}
			}

			if(_lpThreadCheckData){
				_lpThreadCheckData->cancel();
				delete _lpThreadCheckData;
				_lpThreadCheckData = NULL;
			}

			if(m_dSeperateType==0){
				writeRadFile();
			}
			writeNameCsvFile();

			/*if(m_dSeperateType==0&&m_nChannelCount==16){
				copyRadarData();
			}else if(m_dSeperateType==0&&m_nChannelCount==7){
				copyRadarDataForSevenChannel();
			}*/
			/*if(m_dSeperateType==0&&(m_nChannelCount==16||m_nChannelCount==8||m_nChannelCount==7||m_nChannelCount==4)){
				copyRadarData();
			}*/
			if(m_dSeperateType==0&&m_nChannelCount==16){
				copyRadarData();
			}

			if(m_nUpload==0&&m_dSeperateType!=0){
				if(m_dSeperateType==1||(m_dSeperateType==2&&m_arrnRealTotalWheelCount[0]%m_nWheelCountInterval!=0)){//�ж��Ƿ���Ҫ������µ�һ��
					Sleep(2000);
					m_nVecOriginWheelCountForPart.push_back(m_arrnRealTotalWheelCount[0]);
					writeRadFileForPart(m_dRoadPart);
					copyTxtFileForPart(m_dRoadPart);
					if(m_nChannelCount==16){
						((CopyLocalThread*)_lpThreadCopyData)->addPart(m_dRoadPart);
					}
				}
				//copyRadarDataForPart();
				while(m_nCopyFinishedFlag!=m_dRoadPart){//ȷ���������
					Sleep(2000);
				}
			}



			if(_lpThreadCopyData){
				_lpThreadCopyData->cancel();
				delete _lpThreadCopyData;
				_lpThreadCopyData = NULL;
			}

			AfxMessageBox(L"����������ɡ�");
			m_bIsBegin = false;

			if(m_nUpload==1){
				Sleep( 2000 );
				if(_lpThreadUploadGps){
					_lpThreadUploadGps->cancel();
					delete _lpThreadUploadGps;
					_lpThreadUploadGps = NULL;
				}
				/*if((_realTotalWheelCount-9)%m_nThreshold!=0){
					//uploadDataForPart(m_dRoadPart);
					((SaveOnlineThread*)_lpThreadSaveOnline)->addPart(m_dRoadPart);
					//m_dRoadPart=m_dRoadPart+1;
				}*/
				//�������µ�һ��
				m_nVecOriginWheelCountForPart.push_back(m_arrnRealTotalWheelCount[0]);//���µ���
				std::vector<bool> vecTempForSuccess;//�����ж��Ƿ��ϴ��ɹ���
				for(int i=0;i<15;i++){//һ��15�� txt cor csv rad*6 rd3*6
					vecTempForSuccess.push_back(true);
				}
				m_vecVecUploadSuccessFlag.push_back(vecTempForSuccess);
				std::vector<bool> vecTempForProcess;//������������Ƿ����
				for(int i=0;i<15;i++){//һ��15�� txt cor csv rad*6 rd3*6
					vecTempForProcess.push_back(false);
				}
				m_vecVecProcessFinishedFlag.push_back(vecTempForProcess);
				m_vecbUploadConditionForEachPart.push_back(false);
				((SaveOnlineThread*)_lpThreadSaveOnline)->addPart(m_dRoadPart);
				
				bool bAllUploadSuccessFlag=false;
				while(bAllUploadSuccessFlag==false){//��֤�ϴ����
					Sleep(5000);
					bool bCheck=true;
					for(int i=0;i<m_vecVecProcessFinishedFlag.size();i++){
						for(int j=0;j<m_vecVecProcessFinishedFlag[i].size();j++){
							if(m_vecVecProcessFinishedFlag[i][j]==false){
								bCheck=false;
								break;
							}
						}
						if(bCheck==false){
							break;
						}
					}
					bAllUploadSuccessFlag=bCheck;
				}
				

				if(_lpThreadSaveOnline){
					_lpThreadSaveOnline->cancel();
					delete _lpThreadSaveOnline;
					_lpThreadSaveOnline = NULL;
				}

				if(_lpThreadPostFinishedMessage){
					_lpThreadPostFinishedMessage->cancel();
					delete _lpThreadPostFinishedMessage;
					_lpThreadPostFinishedMessage = NULL;
				}

				if(_lpThreadSaveOnlineForRepairment){
					_lpThreadSaveOnlineForRepairment->cancel();
					delete _lpThreadSaveOnlineForRepairment;
					_lpThreadSaveOnlineForRepairment = NULL;
				}

				if(_lpThreadUploadGps){
					_lpThreadUploadGps->cancel();
					delete _lpThreadUploadGps;
					_lpThreadUploadGps = NULL;
				}
				
				AfxMessageBox(L"�ϴ�������ɡ�");
				m_nVecOriginWheelCountForPart.clear();
			}/*else if(m_dSeperateType!=0){
				writeRadFileForPart(m_dRoadPart);
			}*/

			//_recordTotalWheelCount = 0;
			//_realTotalWheelCount = 0;
			for(int i=0;i<m_nChannelCount;i++){
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
	//����ftp����
	hint = InternetOpen(0, INTERNET_OPEN_TYPE_PRECONFIG, 0, 0, 0);
	if (hint==NULL)//����Ϊֹ  
	{   
		hint = InternetOpen(0, INTERNET_OPEN_TYPE_PRECONFIG, 0, 0, 0);
	} 
	//����ftp����
	hftp = InternetConnectA(hint, ftpSvrIp.c_str(),port,userName.c_str(),password.c_str(), INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
	if (hftp==NULL)//����Ϊֹ
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


//��ͨ��������״������������
void MeasureProject::addData( RadarData *rd, int channelIndex ){
	//������ģʽ�����ļ�����
	if ((m_iRadarWorkType == RADAR_WORK_TYPE_ONE_USB || m_iRadarWorkType == RADAR_WORK_TYPE_DOUBLE_USB || m_iRadarWorkType == RADAR_WORK_TYPE_DOUBLE_USB_OLD || m_iRadarWorkType == RADAR_WORK_TYPE_FOUR_USB) && m_iSaveOracle == 0){
		if ( channelIndex >= 8 ){
			channelIndex;
		}
		if(m_arrnRecordTotalWheelCount[channelIndex] == rd->getRealWheelCount()){
			return;
		}
		osg::ref_ptr<SaveRadarFileDataCmd> lpCmd = new SaveRadarFileDataCmd;//osg����߳�
		RadarData *tmpRD = rd;
		lpCmd->_lpData = tmpRD;

		osg::Vec3d pos = RadarManager::Instance()->getGpsReader()->getCurPos();//��ȡ��ǰgps����
		float curLen = RadarManager::Instance()->getGpsReader()->getCurLen() + _lastLen;//��ȡ��ǰ������gps���볤��

		//��ȫ�״�����
		tmpRD->setPosition( pos );
		tmpRD->setTime( time(NULL)/*GetTickCount()*/ );
		tmpRD->setLen( curLen );

		//�жϸ������Ƿ�ע��
		if ( _setMark[channelIndex] ){
			tmpRD->setMark( true );
			_setMark[channelIndex] = false;
		}else{
			tmpRD->setMark( false );
		}
		/*if(index == 0){
			_recordTotalWheelCount = rd->getRealWheelCount();
			_realTotalWheelCount = _realTotalWheelCount+1;
		}*/
		m_arrnRecordTotalWheelCount[channelIndex] = rd->getRealWheelCount();
		m_arrnRealTotalWheelCount[channelIndex] = m_arrnRealTotalWheelCount[channelIndex]+1;

		((SaveFileDataThread*)_lpThreadSaveFile[channelIndex])->addData(lpCmd.get());
	}else if (m_iRadarWorkType == RADAR_WORK_TYPE_EIGHT && m_iSaveOracle == 0){//����ģʽ�����ļ�����
		if(channelIndex>=m_nChannelCount){
			return;
		}
		if(m_nUpload==1){
			if(channelIndex>=6){
				return;
			}
		}
		if(m_nSaveFileType==0){//2ά�״�ɸѡͨ��
			if(m_nArr2DChannel[channelIndex]==0){
				return;
			}
		}

		if(m_nUpload==0){
			if((rd->getPrec()+1)%m_dWaveWaringInterval==0){
				((CheckDataThread*)_lpThreadCheckData)->addData(rd);
			}
		}
		
		osg::ref_ptr<SaveRadarFileDataCmd> lpCmd = new SaveRadarFileDataCmd;//�߳�ָ���ָ��
		RadarData *tmpRD = rd;
		lpCmd->_lpData = tmpRD;
		if(m_arrnRecordTotalWheelCount[channelIndex] == rd->getRealWheelCount()){
			return;
		}
		/*if(index == 0){
			_recordTotalWheelCount = rd->getRealWheelCount();
			_realTotalWheelCount = _realTotalWheelCount+1;
		}*/
		m_arrnRecordTotalWheelCount[channelIndex] = rd->getRealWheelCount();
		m_arrnRealTotalWheelCount[channelIndex] = m_arrnRealTotalWheelCount[channelIndex]+1;
		if(m_arrnRealTotalWheelCount[channelIndex]==1){//��Ҫ��һ�����ݣ���һ������������һ�βɼ��ģ�ԭ����
			return;
		}
		//�����ϴ�ʱ�ķֶ��ж�
		if(m_nUpload==1){
			//if(index==0&&_realTotalWheelCount>m_nThreshold&&(_realTotalWheelCount-9)%m_nThreshold==0){
			int currentSecond = clock();
			if(channelIndex==0&&(currentSecond-m_nLastSecond)>m_nTimeInterval&&(m_arrnRealTotalWheelCount[0]-m_nVecOriginWheelCountForPart[m_nVecOriginWheelCountForPart.size()-1])>500){
				for(int i=0;i<m_nChannelCountForUpload;i++){
					m_SGYWriter[i]->setDataCountThreshold(m_arrnRealTotalWheelCount[0]);//�����������0����Ϊ��������ж���channelIndex=0��������������û����
					m_SGYWriter[i]->setNeedToChange(true);//���Ϸֶ��жϾ͸Ĵ���·��
				}
				m_nVecOriginWheelCountForPart.push_back(m_arrnRealTotalWheelCount[0]);
				std::vector<bool> vecTempForSuccess;//�����ж��Ƿ��ϴ��ɹ���
				for(int i=0;i<15;i++){//һ��15�� txt cor csv rad*6 rd3*6
					vecTempForSuccess.push_back(true);
				}
				m_vecVecUploadSuccessFlag.push_back(vecTempForSuccess);
				std::vector<bool> vecTempForProcess;//������������Ƿ����
				for(int i=0;i<15;i++){//һ��15�� txt cor csv rad*6 rd3*6
					vecTempForProcess.push_back(false);
				}
				m_vecVecProcessFinishedFlag.push_back(vecTempForProcess);
				m_vecbUploadConditionForEachPart.push_back(false);
				((SaveOnlineThread*)_lpThreadSaveOnline)->addPart(m_dRoadPart);//�Ѹո���ɵ���һ�η����ϴ��߳� ��ʼ�ϴ�
				m_nLastSecond=m_nLastSecond+m_nTimeInterval;
				m_dRoadPart=m_dRoadPart+1;//����·�κ�
			}
		}else if(m_dSeperateType==1){
			int currentSecond = clock();
			if(channelIndex==0&&(currentSecond-m_nLastSecond)>m_nTimeInterval&&(m_arrnRealTotalWheelCount[0]-m_nVecOriginWheelCountForPart[m_nVecOriginWheelCountForPart.size()-1])>500){
				for(int i=0;i<m_nChannelCount;i++){
					m_SGYWriter[i]->setDataCountThreshold(m_arrnRealTotalWheelCount[0]);//�����������0����Ϊ��������ж���channelIndex=0��������������û����
					m_SGYWriter[i]->setNeedToChange(true);//���Ϸֶ��жϾ͸Ĵ���·��
				}
				m_nVecOriginWheelCountForPart.push_back(m_arrnRealTotalWheelCount[0]);
				if(m_nSaveFileType==1){
					writeRadFileForPart(m_dRoadPart);
					copyTxtFileForPart(m_dRoadPart);
					if(m_nChannelCount==16){
						((CopyLocalThread*)_lpThreadCopyData)->addPart(m_dRoadPart);
					}
				}
				m_nLastSecond=m_nLastSecond+m_nTimeInterval;
				m_dRoadPart=m_dRoadPart+1;//����·�κ�
			}
		}else if(m_dSeperateType==2){
			//if(index==0&&_realTotalWheelCount>m_nThreshold&&(_realTotalWheelCount-9)%m_nThreshold==0){
			if(channelIndex==0&&m_arrnRealTotalWheelCount[0]>=m_nWheelCountInterval&&m_arrnRealTotalWheelCount[0]%m_nWheelCountInterval==0){
				//for(int i=0;i<CHANNELCOUNT;i++){//����������жϣ�����Ҫ�������
					//m_SGYWriter[i]->setDataCountThreshold(m_arrnRealTotalWheelCount[0]);//�����������0����Ϊ��������ж���channelIndex=0��������������û����
					//m_SGYWriter[i]->setNeedToChange(true);
				//}
				m_nVecOriginWheelCountForPart.push_back(m_arrnRealTotalWheelCount[0]);
				if(m_nSaveFileType==1){
					writeRadFileForPart(m_dRoadPart);
					copyTxtFileForPart(m_dRoadPart);
					if(m_nChannelCount==16){
						((CopyLocalThread*)_lpThreadCopyData)->addPart(m_dRoadPart);
					}
				}
				//m_nLastSecond=m_nLastSecond+m_nTimeInterval;
				m_dRoadPart=m_dRoadPart+1;//����·�κ�
			}
		}

		/*
		if(_realTotalWheelCount==2&&tmpRD->getPrec()==_currentWheelCount){
			//_realTotalWheelCount = _realTotalWheelCount-1;
			return;
		}
		if(index == 0){
			_currentWheelCount = tmpRD->getPrec();//��ȡ�õ�ǰ�������ݵĲ����ֵ���
		}*/

		//osg::Vec3d pos = RadarManager::Instance()->getGpsReader()->getCurPos();//��ȡ��ǰgps����
		osg::Vec3d pos;
		std::vector< std::string > vecStrGPGGA;
		RadarManager::Instance()->getGpsReader()->getCurrentGpsImformation(pos,vecStrGPGGA);
		float curLen = RadarManager::Instance()->getGpsReader()->getCurLen() + _lastLen;//��ȡ��ǰ�����ݾ��볤��

		//�ϴ�ģʽ��ʵʱ�ϴ�gpsData
		if(m_nUpload==1){
			if(channelIndex == 0){
				if(curLen>m_fLastGpsLen||m_fLastGpsLen==0){
					/*std::vector< std::string > strGpsData(RadarManager::Instance()->getGpsReader()->getCurrentGpsDataBuff());
					if (!( _currentGps.size()!=15 || _currentGps[14].size()<3 || _currentGps[2]=="" || _currentGps[4]=="" )){//�ڶ����ж���ȷ�����һ���л��з� �����жϾ�γ�Ȳ�Ϊ��
						std::string strUploadGpsData=
						((UploadGpsOnlineThread*)_lpThreadUploadGps)->addData(pos);
					}*/
					if(m_fLastPostedGpsLen!=m_fLastGpsLen){
						time_t mytime = time(NULL);
						int dTemp=mytime;
						if(dTemp>m_dGpsTimeStamp||m_fLastGpsLen==0){//ÿһ�봫һ��
							m_dGpsTimeStamp=dTemp;
							((UploadGpsOnlineThread*)_lpThreadUploadGps)->addData(m_pLastPos);//�̼߳�����
						}
					}
					m_fLastPostedGpsLen=m_fLastGpsLen;
					m_pLastPos=pos;
					m_fLastGpsLen=curLen;
				}
			}
		}
		
		
		//��ȫ�״�����
		tmpRD->setPosition( pos );
		tmpRD->setGPGGA( vecStrGPGGA );
		tmpRD->setTime( time(NULL)/*GetTickCount()*/ );
		tmpRD->setLen( curLen );
		
		
		//�жϸ������Ƿ�ע��
		if ( _setMark[channelIndex] ){
			tmpRD->setMark( true );
			_setMark[channelIndex] = false;
		}else{
			tmpRD->setMark( false );
		}

		//��index���䵽��Ӧ���״����ݴ����߳���
		((SaveFileDataThread*)_lpThreadSaveFile[channelIndex])->addData(lpCmd.get());
		
	}else if(m_iSaveOracle == 1){//���ݿ�ģʽ
		if ( !RadarManager::Instance()->dbOpen() ){
			return;
		}
		if ( channelIndex >= m_nChannelCount ){
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

		if ( _setMark[channelIndex] ){
			tmpRD->setMark( true );
			_setMark[channelIndex] = false;
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

void MeasureProject::addDataToDataLossCheck( RadarData *rd, int channelIndex ){
	/*if(index>=m_nTrueChannelCount){
		return;
	}
	
	m_vecRadarDataGroup.push_back(d);
		
	if(m_nTrueChannelCount!=15&&m_nTrueChannelCount!=14&&index==m_nTrueChannelCount-1){
		if(m_vecRadarDataGroup.size()==m_nTrueChannelCount){
			g_ThreeViewDlg->addGroupRadarData( m_vecRadarDataGroup );	
		}
		m_vecRadarDataGroup.clear();
	}else if(m_nTrueChannelCount==15&&index==13){//15ͨ���Ĵ���������������1324 5768 9111012 13151416���䣬���Ե�14��ͣ
		if(m_vecRadarDataGroup.size()==m_nTrueChannelCount){
			std::vector<osg::ref_ptr<RadarData>> temp;
			for(int i=0;i<3;i++){
				temp.push_back(m_vecRadarDataGroup[i*4+0]);
				temp.push_back(m_vecRadarDataGroup[i*4+2]);
				temp.push_back(m_vecRadarDataGroup[i*4+1]);
				temp.push_back(m_vecRadarDataGroup[i*4+3]);
			}
			temp.push_back(m_vecRadarDataGroup[12]);
			temp.push_back(m_vecRadarDataGroup[14]);
			temp.push_back(m_vecRadarDataGroup[13]);
			g_ThreeViewDlg->addGroupRadarData( temp );
		}
		m_vecRadarDataGroup.clear();
	}else if(m_nTrueChannelCount==14&&index==13){//14ͨ���Ĵ���Ҳ������������1324 5768 9111012 13151416���䣬���Ե�14��ͣ
		if(m_vecRadarDataGroup.size()==m_nTrueChannelCount){
			std::vector<osg::ref_ptr<RadarData>> temp;
			for(int i=0;i<3;i++){
				temp.push_back(m_vecRadarDataGroup[i*4+0]);
				temp.push_back(m_vecRadarDataGroup[i*4+2]);
				temp.push_back(m_vecRadarDataGroup[i*4+1]);
				temp.push_back(m_vecRadarDataGroup[i*4+3]);
			}
			temp.push_back(m_vecRadarDataGroup[12]);
			temp.push_back(m_vecRadarDataGroup[14]);
			//temp.push_back(m_vecRadarDataGroup[13]);
			g_ThreeViewDlg->addGroupRadarData( temp );
		}
		m_vecRadarDataGroup.clear();
	}*/
}

//20200306 hjl ���ڲ����ֻع�ɾ������
void MeasureProject::deleteData(int index )
{
	if ((m_iRadarWorkType == RADAR_WORK_TYPE_ONE_USB || m_iRadarWorkType == RADAR_WORK_TYPE_DOUBLE_USB  || m_iRadarWorkType == RADAR_WORK_TYPE_DOUBLE_USB_OLD 
		|| m_iRadarWorkType == RADAR_WORK_TYPE_FOUR_USB) && m_iSaveOracle == 0)//������ģʽ�����ļ�����
	{
		((SaveFileDataThread*)_lpThreadSaveFile[index])->deleteData();
	}
}

void MeasureProject::checkRadarData( RadarData *rd ){
	int len = 0;
	short *lpBuff = ( short *)(rd->getData( len ));//ת��short
	if ( !lpBuff ){
		return;
	}
	bool bWarning=true;
	int dSampleCount=rd->getDataCount();
	for(int i=4;i<dSampleCount;i++){
		if(lpBuff[i]>m_dWaveWaringThreshold){
			bWarning=false;
			break;
		}
	}
	if(bWarning){
		unsigned char *tempBuff = ( unsigned char *)(rd->getData( len ));
		int index = tempBuff[1] - 114; //TrTr��r��ascii����114
		string strIndex=intToString(index+1);
		string strMessage="��"+strIndex+"ͨ�����ֲ��ι�С״��";
		CString cstrMessage;
		cstrMessage=strMessage.c_str();
		::AfxMessageBox(cstrMessage);
		//::AfxMessageBox(L"���ֲ��ι�С״��");
	}
	
}

//��ȡ�����
MeasureProject::ReceiveQueue MeasureProject::getDataQueue()
{
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_dataMutex);
	return _dataQueue;
}

//������Ŀ����
void MeasureProject::setProjectName( std::string const& name ){
	_projectName = name;
}

//��ȡ��Ŀ����
std::string MeasureProject::getProjectName()const{
	return _projectName;
}

//������Ŀʱ���
void MeasureProject::setProjectTab( std::string const& tabName ){
	_projectTab = tabName;
}

//��ȡ��Ŀʱ���
std::string MeasureProject::getProjectTab()const{
	return _projectTab;
}

//������Ŀ�б�
void MeasureProject::setProjectRow( DBRow *lpRow ){
	_projectRow = lpRow;
	ProjectNameTab::ProjectNameRow *lpProjectRow = dynamic_cast<ProjectNameTab::ProjectNameRow *>(_projectRow.get());
	if ( lpProjectRow ){
		//_lastLen = lpProjectRow->_curLen;
		_lastLen = 0;
	}else{
		_lastLen = 0;
	}
	for(int i=0;i<m_nChannelCount;i++){
		m_arrnRecordTotalWheelCount[i]=0;
		m_arrnRealTotalWheelCount[i]=0;
	}
	m_nVecOriginWheelCountForPart.clear();
	m_nVecOriginWheelCountForPart.push_back(0);
	m_vecVecUploadSuccessFlag.clear();
	m_vecVecProcessFinishedFlag.clear();
}

//������Ŀ��Ϣ
void MeasureProject::updateProjectInfo( float curLen ){
	ProjectNameTab::ProjectNameRow *lpRow = dynamic_cast<ProjectNameTab::ProjectNameRow *>(_projectRow.get());
	if ( lpRow ){
		lpRow->_curLen = curLen;
		osg::ref_ptr<UpdateProjectInfoCmd> lpCmd = new UpdateProjectInfoCmd( lpRow );
		addSaveCmd( lpCmd.get() );	
	}
}

//��һ��������Ϊ���
void MeasureProject::setMark(){
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_dataMutex);
	for ( int i = 0; i < m_nChannelCount; i ++ ){
		_setMark[i] = true;
	}
}

//���ݿ�ģʽ�µ� ��ȡ���õ��߳�
OpenThreads::Thread *MeasureProject::getAvaliadThread(){
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

//���ô���ģʽ
void MeasureProject::SetSaveOracleOrFile( int iRadarWorkType,int isaveOracle,CString strPath ){
	m_iRadarWorkType = iRadarWorkType;
	m_iSaveOracle = isaveOracle;
	m_strSavePath = strPath;

	//���ļ�
	if (m_iSaveOracle == 0 ){//�������ݿ�
		switch (m_iRadarWorkType){//�õ��Ǽ�ͨ������ģʽ
			case RADAR_WORK_TYPE_FOUR_USB://��ͨ�� ��sgy
				{
					m_bIsBegin = true;
					_maxID = 0;

					CString cstrCreateTime;
					USES_CONVERSION;

					//��������·��
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
						::AfxMessageBox(L"��ͷ��Ϣ����ȷ�����ܱ���ӣǣ��ļ���");
						return;
					}
					
					//���߳�
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
						m_SGYWriter[i]->openSGY(strPathSGY[i],lpProjectRow->_paramXML ,i);//��·�����ļ�
						if ( RadarManager::Instance()->getExportGpsPos() ){
							if(useIt(i)){
								m_SGYWriter[i]->openGPS(strPathGPS);
							}
						}
						/*if ( RadarManager::Instance()->getExportKml() ){
							if(useIt(i)){
								m_SGYWriter[i]->openKml(strPathKml);
								m_SGYWriter[i]->writeKmlHead();
							}
						}*/
						m_SGYWriter[i]->writeHead(i,_projectTab);//��
						((SaveFileDataThread*)_lpThreadSaveFile[i])->_SGYWriter = m_SGYWriter[i];
						_lpThreadSaveFile[i]->start();
					}
					if (_lpThreadCheckData){
						delete _lpThreadCheckData;
						_lpThreadCheckData = NULL;
					}
					_lpThreadCheckData = new CheckDataThread;
					((CheckDataThread*)_lpThreadCheckData)->_MeasureProject = this;
					_lpThreadCheckData->start();
				}
				break;
			case RADAR_WORK_TYPE_EIGHT://����ģʽ 
				if(m_nSaveFileType==0){//��sgy
					if(m_dSeperateType==0){//���ֶ�
						m_bIsBegin = true;
						_maxID = 0;

						CString cstrCreateTime;
						USES_CONVERSION;

						std::string strPathOrigin = W2A( strPath );
						strPathOrigin += _projectName;
						strPathOrigin += _projectTab;
						
						std::vector<std::string> vecStrSGYPath;
						for(int i=0;i<m_nChannelCount;i++){
							std::string strPath;
							strPath=strPathOrigin;
							strPath+="_";
							//intתchar*
							stringstream ssTemp;
							ssTemp << (i+1);
							string strTemp = ssTemp.str();
							strPath+=strTemp;
							strPath+=".SGY";
							vecStrSGYPath.push_back(strPath);
						}

						std::string strPathGPS = W2A( strPath );
						strPathGPS+="GPStxt";
						if (0 != access(strPathGPS.c_str(), 0)){
							// if this folder not exist, create a new one.
							mkdir(strPathGPS.c_str());   // ���� 0 ��ʾ�����ɹ���-1 ��ʾʧ��
						}
						strPathGPS+="\\";
						strPathGPS += _projectName;
						strPathGPS += _projectTab;
						strPathGPS+=".csv";

						
						std::string strPathKml = W2A( strPath );
						strPathKml+="kml";
						if (0 != access(strPathKml.c_str(), 0)){
							// if this folder not exist, create a new one.
							mkdir(strPathKml.c_str());   // ���� 0 ��ʾ�����ɹ���-1 ��ʾʧ��
						}
						strPathKml+="\\";
						strPathKml += _projectName;
						strPathKml += _projectTab;
						strPathKml+=".kml";
						
						ProjectNameTab::ProjectNameRow *lpProjectRow = dynamic_cast<ProjectNameTab::ProjectNameRow *>(_projectRow.get());
						if (NULL == lpProjectRow)
						{
							::AfxMessageBox(L"��ͷ��Ϣ����ȷ�����ܱ���ӣǣ��ļ���");
							return;
						}

						for(int i=0;i<8;i++){
							if(m_nArr2DChannel[i] == 1 ){
								if (_lpThreadSaveFile[i])
								{
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
								m_SGYWriter[i]->openSGY(vecStrSGYPath[i].c_str(),lpProjectRow->_paramXML ,0);
								if ( RadarManager::Instance()->getExportGpsPos() ){
									if(useIt(i)){
										m_SGYWriter[i]->openGPS(strPathGPS);
									}
								}
								if ( RadarManager::Instance()->getExportKml() ){
									if(useIt(i)){
										m_SGYWriter[i]->openKml(strPathKml);
										m_SGYWriter[i]->writeKmlHead();
									}
								}
								m_SGYWriter[i]->writeHead(1,_projectTab);
								((SaveFileDataThread*)_lpThreadSaveFile[i])->_SGYWriter = m_SGYWriter[i];
								_lpThreadSaveFile[i]->start();
							}
						}
						if (_lpThreadCheckData){
							delete _lpThreadCheckData;
							_lpThreadCheckData = NULL;
						}
						_lpThreadCheckData = new CheckDataThread;
						((CheckDataThread*)_lpThreadCheckData)->_MeasureProject = this;
						_lpThreadCheckData->start();
					}else if(m_dSeperateType!=0){//��Ҫ�ֶ�
						m_bIsBegin = true;
						_maxID = 0;

						CString cstrCreateTime;
						USES_CONVERSION;

						std::string strPathOrigin = W2A( strPath );
						strPathOrigin += _projectName;
						strPathOrigin += _projectTab;
						if (0 != access(strPathOrigin.c_str(), 0)){
							// if this folder not exist, create a new one.
							mkdir(strPathOrigin.c_str());   // ���� 0 ��ʾ�����ɹ���-1 ��ʾʧ��
						}
						strPathOrigin += "\\";
						//strPathOrigin += _projectName;
						//strPathOrigin += _projectTab;
						
						std::vector<std::string> vecStrSGYPath;
						for(int i=0;i<m_nChannelCount;i++){
							std::string strPath;
							strPath=strPathOrigin;
							strPath+=_projectName;
							strPath+=_projectTab;
							strPath+="_1";
							strPath+="_";
							//intתchar*
							stringstream ssTemp;
							ssTemp << (i+1);
							string strTemp = ssTemp.str();
							strPath+=strTemp;
							strPath+=".SGY";
							vecStrSGYPath.push_back(strPath);
						}

						std::string strPathGPSCsv = strPathOrigin;
						strPathGPSCsv+="GPScsv";
						if (0 != access(strPathGPSCsv.c_str(), 0)){
							// if this folder not exist, create a new one.
							mkdir(strPathGPSCsv.c_str());   // ���� 0 ��ʾ�����ɹ���-1 ��ʾʧ��
						}
						strPathGPSCsv+="\\";
						strPathGPSCsv += _projectName;
						strPathGPSCsv += _projectTab;
						strPathGPSCsv += "_1";
						strPathGPSCsv+=".csv";

						std::string strPathKml = strPathOrigin;
						strPathKml+="kml";
						if (0 != access(strPathKml.c_str(), 0)){
							// if this folder not exist, create a new one.
							mkdir(strPathKml.c_str());   // ���� 0 ��ʾ�����ɹ���-1 ��ʾʧ��
						}
						strPathKml+="\\";
						strPathKml += _projectName;
						strPathKml += _projectTab;
						strPathKml += "_1";
						strPathKml+=".kml";
						
						ProjectNameTab::ProjectNameRow *lpProjectRow = dynamic_cast<ProjectNameTab::ProjectNameRow *>(_projectRow.get());
						if (NULL == lpProjectRow)
						{
							::AfxMessageBox(L"��ͷ��Ϣ����ȷ�����ܱ���ӣǣ��ļ���");
							return;
						}

						for(int i=0;i<8;i++){
							if(m_nArr2DChannel[i] == 1 ){
								if (_lpThreadSaveFile[i])
								{
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
								m_SGYWriter[i]->setUploadFlag(m_nUpload);//�����Ƿ��ϴ�
								m_SGYWriter[i]->setSaveFileType(m_nSaveFileType);//���ô����ļ�����
								m_SGYWriter[i]->setSeperateType(m_dSeperateType);//���÷ֶ�����
								m_SGYWriter[i]->setPathElement(W2A( strPath ),_projectName,_projectTab);//����·������Ԫ��
								m_SGYWriter[i]->openSGYForAutomation(vecStrSGYPath[i].c_str(),lpProjectRow->_paramXML,i,_documentPathLength,_namePathLength);
								//m_SGYWriter[i]->openSGY(vecStrSGYPath[i].c_str(),lpProjectRow->_paramXML ,0);
								if ( RadarManager::Instance()->getExportGpsPos() ){
									if(useIt(i)){
										m_SGYWriter[i]->openGPS(strPathGPSCsv);
									}
								}
								if ( RadarManager::Instance()->getExportKml() ){
									if(useIt(i)){
										m_SGYWriter[i]->openKml(strPathKml);
										m_SGYWriter[i]->writeKmlHead();
									}
								}
								m_SGYWriter[i]->writeHead(1,_projectTab);
								((SaveFileDataThread*)_lpThreadSaveFile[i])->_SGYWriter = m_SGYWriter[i];
								_lpThreadSaveFile[i]->start();
							}
						}
						//�����������
						if (_lpThreadCheckData){
							delete _lpThreadCheckData;
							_lpThreadCheckData = NULL;
						}
						_lpThreadCheckData = new CheckDataThread;
						((CheckDataThread*)_lpThreadCheckData)->_MeasureProject = this;
						_lpThreadCheckData->start();
						/*//���������߳�
						if (_lpThreadCopyData){
							delete _lpThreadCopyData;
							_lpThreadCopyData = NULL;
						}
						_lpThreadCopyData = new CopyLocalThread;
						((CopyLocalThread*)_lpThreadCopyData)->_MeasureProject = this;
						_lpThreadCopyData->start();*/
					}
				}else if(m_nSaveFileType==1){//��rd3
					if(m_nUpload==1){//�ϴ�ģʽ
						//m_bIsAdd = false;
						m_bIsBegin = true;
						_maxID = 0;

						//getOnlinePath();
						writeTxtFile();
						//writeEndFile();
		
						while(m_nInitOpr==0){//ȷ������opr�ɹ�
							initOprTab();
						}
						//setStartTimeTab();

						CString cstrCreateTime;
						USES_CONVERSION;

						//���ɱ��ش���·��
						std::string strPathOrigin = W2A( strPath );
						_documentPathLength = strPathOrigin.length();
						strPathOrigin += _projectName;
						strPathOrigin += _projectTab;
						_namePathLength = strPathOrigin.length()-_documentPathLength;
						if (0 != access(strPathOrigin.c_str(), 0)){
							// if this folder not exist, create a new one.
							mkdir(strPathOrigin.c_str());   // ���� 0 ��ʾ�����ɹ���-1 ��ʾʧ��
						}

						m_fpLog = fopen( (strPathOrigin+"\\log.txt").c_str(), "a" );//�������̵�log�ļ�
						m_fpLogForRepairment = fopen( (strPathOrigin+"\\logForRepairment.txt").c_str(), "a" );//�����ϴ���log�ļ�
						strPathOrigin+="\\";
						strPathOrigin += _projectName;
						strPathOrigin += _projectTab;
						strPathOrigin+="_1";
						if (0 != access(strPathOrigin.c_str(), 0)){
							// if this folder not exist, create a new one.
							mkdir(strPathOrigin.c_str());   // ���� 0 ��ʾ�����ɹ���-1 ��ʾʧ��
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
							mkdir(strPathOriginForHide.c_str());   // ���� 0 ��ʾ�����ɹ���-1 ��ʾʧ��
						}
						strPathOriginForHide += "\\";
						strPathOriginForHide += _projectName;
						strPathOriginForHide += _projectTab;
						strPathOriginForHide += "_1";
						
						
						/*
						//char strPathSGY[CHANNELCOUNT][300];//�ļ�·���ֽ���
						char **strPathSGY = new char*[m_nChannelCountForUpload];
						for(int i=0;i<m_nChannelCountForUpload;i++){
							strPathSGY[i] = new char[300];
						}
						for(int i=0;i<m_nChannelCountForUpload;i++){//ʹ�����strcat��Ч
							strPathSGY[i][0]='\0';
						}*/
						for(int i=0;i<m_nChannelCountForUpload;i++){//ʹ�����strcat��Ч
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
						
							//intתchar*
							stringstream ssTemp;
							//ssTemp << (RadarManager::Instance()->originalIndexToRecordIndex(i)+1);
							ssTemp << (i+1);
							string strTemp = ssTemp.str();
							char* cIndex = (char*) strTemp.c_str();
							
							strcat(_strLocalRadarDataPath[i],cIndex);//(char*)(48+ i) int i ת��char*
							strcat(_strLocalRadarDataPath[i],".rd3");
							//strcat(_strLocalRadarDataPath[i],".gdd");
						}

						for(int i=0;i<m_nChannelCountForUpload;i++){//�ַ�����β������ֹ��
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

						_strLocalGPSKmlPath="";
						_strLocalGPSKmlPath += strPathOrigin;
						_strLocalGPSKmlPath+=".kml";
						/*std::string strPathKml = W2A( strPath );
						strPathKml+="kml";
						if (0 != access(strPathKml.c_str(), 0)){
							// if this folder not exist, create a new one.
							mkdir(strPathKml.c_str());   // ���� 0 ��ʾ�����ɹ���-1 ��ʾʧ��
						}
						strPathKml+="\\";
						strPathKml += _projectName;
						strPathKml += _projectTab;
						strPathKml+=".kml";*/
							
						ProjectNameTab::ProjectNameRow *lpProjectRow = dynamic_cast<ProjectNameTab::ProjectNameRow *>(_projectRow.get());
						if (NULL == lpProjectRow){
							::AfxMessageBox(L"��ͷ��Ϣ����ȷ�����ܱ���ӣǣ��ļ���");
							return;
						}

						for(int i=0;i<m_nChannelCountForUpload;i++){
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
							m_SGYWriter[i]->setUploadFlag(m_nUpload);//�����Ƿ��ϴ�
							m_SGYWriter[i]->setSaveFileType(m_nSaveFileType);//���ô����ļ�����
							m_SGYWriter[i]->setSeperateType(m_dSeperateType);//���÷ֶ�����
							m_SGYWriter[i]->setPathElement(W2A( strPath ),_projectName,_projectTab);//����·������Ԫ��
							m_SGYWriter[i]->openSGYForAutomation(_strLocalRadarDataPath[i],lpProjectRow->_paramXML,i,_documentPathLength,_namePathLength);
							if ( RadarManager::Instance()->getExportGpsPos() ){//gps
								if(useIt(i)){
									m_SGYWriter[i]->openGPS(_strLocalGPSCorPath, _strLocalGPSCorForUploadPath, _strLocalGPSCsvPath);
								}
							}
							if ( RadarManager::Instance()->getExportKml() ){//kml
								if(useIt(i)){
									m_SGYWriter[i]->openKml(_strLocalGPSKmlPath);
									m_SGYWriter[i]->writeKmlHead();
								}
							}
							//m_SGYWriter[i]->writeHead(1,_projectTab);//20210618hjl
							((SaveFileDataThread*)_lpThreadSaveFile[i])->_SGYWriter = m_SGYWriter[i];
							_lpThreadSaveFile[i]->start();
						}

						m_nLastSecond = clock();//��¼��ʼʱ�� 

						//�����ϴ��߳�
						if (_lpThreadSaveOnline){
							delete _lpThreadSaveOnline;
							_lpThreadSaveOnline = NULL;
						}
						_lpThreadSaveOnline = new SaveOnlineThread;
						((SaveOnlineThread*)_lpThreadSaveOnline)->_MeasureProject = this;
						_lpThreadSaveOnline->start();

						//�����Ϣpost�߳�
						if (_lpThreadPostFinishedMessage){
							delete _lpThreadPostFinishedMessage;
							_lpThreadPostFinishedMessage = NULL;
						}
						_lpThreadPostFinishedMessage = new PostFinishedMessageThread;
						((PostFinishedMessageThread*)_lpThreadPostFinishedMessage)->_MeasureProject = this;
						_lpThreadPostFinishedMessage->start();

						//�����ϴ��߳�
						if (_lpThreadSaveOnlineForRepairment){
							delete _lpThreadSaveOnlineForRepairment;
							_lpThreadSaveOnlineForRepairment = NULL;
						}
						_lpThreadSaveOnlineForRepairment = new SaveOnlineForRepairmentThread;
						((SaveOnlineForRepairmentThread*)_lpThreadSaveOnlineForRepairment)->_MeasureProject = this;
						_lpThreadSaveOnlineForRepairment->start();

						//gps��ʱ�ϴ��߳�
						if (_lpThreadUploadGps){
							delete _lpThreadUploadGps;
							_lpThreadUploadGps = NULL;
						}
						_lpThreadUploadGps = new UploadGpsOnlineThread;
						((UploadGpsOnlineThread*)_lpThreadUploadGps)->_MeasureProject = this;
						_lpThreadUploadGps->start();
					}else if(m_nUpload==0){
						if(m_dSeperateType==0){//���ֶ�
							m_bIsBegin = true;
							_maxID = 0;

							writeTxtFile();//arraySet.txt

							CString cstrCreateTime;
							USES_CONVERSION;

							//�����洢·����rd3 rad cor kml
							std::string strPathOrigin = W2A( m_strSavePath );
							strPathOrigin += _projectName;
							strPathOrigin += _projectTab;
							if (0 != access(strPathOrigin.c_str(), 0)){//��·�������ļ���
								// if this folder not exist, create a new one.
								mkdir(strPathOrigin.c_str());   // ���� 0 ��ʾ�����ɹ���-1 ��ʾʧ��
							}
							strPathOrigin += "\\";
							strPathOrigin += _projectName;
							strPathOrigin += _projectTab;

							//m_fpLog=fopen( (strPathOrigin+"\\log.txt").c_str(), "a" );

							/*
							std::string strPathOriginForHide = W2A( m_strSavePath );
							strPathOriginForHide += _projectName;
							strPathOriginForHide += _projectTab;
							strPathOriginForHide+="\\moreChannels";
							if (0 != access(strPathOriginForHide.c_str(), 0)){
								// if this folder not exist, create a new one.
								mkdir(strPathOriginForHide.c_str());   // ���� 0 ��ʾ�����ɹ���-1 ��ʾʧ��
							}
							strPathOriginForHide += "\\";
							strPathOriginForHide += _projectName;
							strPathOriginForHide += _projectTab;
							*/

							//char strPathSGY[CHANNELCOUNT][300];//�ļ�·���ֽ���
							char **strPathSGY = new char*[m_nChannelCount];
							for(int i=0;i<m_nChannelCount;i++){
								strPathSGY[i] = new char[300];
							}
							for(int i=0;i<m_nChannelCount;i++){
								strPathSGY[i][0]='\0';
							}

							for(int i=0;i<m_nChannelCount;i++){
								/*if(i<16){
									strcat(strPathSGY[i],strPathOrigin.c_str());
								}else{
									strcat(strPathSGY[i],strPathOriginForHide.c_str());
								}*/
								strcat(strPathSGY[i],strPathOrigin.c_str());
								strcat(strPathSGY[i],"_");
							
								//intתchar*
								stringstream ssTemp;
								//ssTemp << (RadarManager::Instance()->originalIndexToRecordIndex(i)+1);
								ssTemp << (i+1);
								string strTemp = ssTemp.str();
								char* cIndex = (char*) strTemp.c_str();
								
								strcat(strPathSGY[i],cIndex);//(char*)(48+ i) int i ת��char*
								strcat(strPathSGY[i],".rd3");
							}

							_strLocalGPSCorPath ="";
							_strLocalGPSCorPath += strPathOrigin;
							_strLocalGPSCorPath+="gps.cor";

							_strLocalGPSCsvPath ="";
							_strLocalGPSCsvPath += strPathOrigin;
							_strLocalGPSCsvPath+=".csv";

							_strLocalGPSKmlPath="";
							_strLocalGPSKmlPath += strPathOrigin;
							_strLocalGPSKmlPath+=".kml";
							/*std::string strPathKml = W2A( strPath );
							strPathKml+="kml";
							if (0 != access(strPathKml.c_str(), 0)){
								// if this folder not exist, create a new one.
								mkdir(strPathKml.c_str());   // ���� 0 ��ʾ�����ɹ���-1 ��ʾʧ��
							}
							strPathKml+="\\";
							strPathKml += _projectName;
							strPathKml += _projectTab;
							strPathKml+=".kml";*/
								
							ProjectNameTab::ProjectNameRow *lpProjectRow = dynamic_cast<ProjectNameTab::ProjectNameRow *>(_projectRow.get());
							if (NULL == lpProjectRow){
								::AfxMessageBox(L"��ͷ��Ϣ����ȷ�����ܱ���ӣǣ��ļ���");
								return;
							}

							for(int i=0;i<m_nChannelCount;i++){
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
								m_SGYWriter[i]->setUploadFlag(m_nUpload);//�����Ƿ��ϴ�
								m_SGYWriter[i]->setSaveFileType(m_nSaveFileType);//���ô����ļ�����
								m_SGYWriter[i]->openSGY(strPathSGY[i],lpProjectRow->_paramXML ,i);
								if ( RadarManager::Instance()->getExportGpsPos() ){//gps
									if(useIt(i)){
										//m_SGYWriter[i]->openGPS(strPathGPS);
										m_SGYWriter[i]->openGPS(_strLocalGPSCorPath, "", _strLocalGPSCsvPath);
									}
								}
								if ( RadarManager::Instance()->getExportKml() ){//kml
									if(useIt(i)){
										m_SGYWriter[i]->openKml(_strLocalGPSKmlPath);
										m_SGYWriter[i]->writeKmlHead();
									}
								}
								//m_SGYWriter[i]->writeHead(1,_projectTab);//20210618hjl
								//д���ݵ��߳�
								((SaveFileDataThread*)_lpThreadSaveFile[i])->_SGYWriter = m_SGYWriter[i];
								_lpThreadSaveFile[i]->start();
							}
							//���ݼ���߳�
							if (_lpThreadCheckData){
								delete _lpThreadCheckData;
								_lpThreadCheckData = NULL;
							}
							_lpThreadCheckData = new CheckDataThread;
							((CheckDataThread*)_lpThreadCheckData)->_MeasureProject = this;
							_lpThreadCheckData->start();
						}else if(m_dSeperateType!=0){//�ֶ�
							m_bIsBegin = true;
							_maxID = 0;

							writeTxtFile();

							CString cstrCreateTime;
							USES_CONVERSION;

							//���ɱ��ش���·��
							std::string strPathOrigin = W2A( strPath );
							_documentPathLength = strPathOrigin.length();
							strPathOrigin += _projectName;
							strPathOrigin += _projectTab;
							_namePathLength = strPathOrigin.length()-_documentPathLength;
							if (0 != access(strPathOrigin.c_str(), 0)){
								// if this folder not exist, create a new one.
								mkdir(strPathOrigin.c_str());   // ���� 0 ��ʾ�����ɹ���-1 ��ʾʧ��
							}

							m_fpLog = fopen( (strPathOrigin+"\\log.txt").c_str(), "a" );//�������̵�log�ļ�
							m_fpLogForRepairment = fopen( (strPathOrigin+"\\logForRepairment.txt").c_str(), "a" );//�����ϴ���log�ļ�
							strPathOrigin+="\\";
							strPathOrigin += _projectName;
							strPathOrigin += _projectTab;
							strPathOrigin+="_1";
							if (0 != access(strPathOrigin.c_str(), 0)){
								// if this folder not exist, create a new one.
								mkdir(strPathOrigin.c_str());   // ���� 0 ��ʾ�����ɹ���-1 ��ʾʧ��
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
								mkdir(strPathOriginForHide.c_str());   // ���� 0 ��ʾ�����ɹ���-1 ��ʾʧ��
							}
							strPathOriginForHide += "\\";
							strPathOriginForHide += _projectName;
							strPathOriginForHide += _projectTab;
							strPathOriginForHide += "_1";
							
							
						
							for(int i=0;i<m_nChannelCount;i++){//ʹ�����strcat��Ч
								_strLocalRadarDataPath[i][0]='\0';
							}
							for(int i=0;i<m_nChannelCount;i++){
								strcat(_strLocalRadarDataPath[i],strPathOrigin.c_str());
								/*if(i<16){
									strcat(_strLocalRadarDataPath[i],strPathOrigin.c_str());
								}else{
									strcat(_strLocalRadarDataPath[i],strPathOriginForHide.c_str());
								}*/
								strcat(_strLocalRadarDataPath[i],"_");
							
								//intתchar*
								stringstream ssTemp;
								ssTemp << (i+1);
								string strTemp = ssTemp.str();
								char* cIndex = (char*) strTemp.c_str();
								
								strcat(_strLocalRadarDataPath[i],cIndex);//(char*)(48+ i) int i ת��char*
								strcat(_strLocalRadarDataPath[i],".rd3");
								//strcat(_strLocalRadarDataPath[i],".gdd");
							}
							for(int i=0;i<m_nChannelCount;i++){//�ַ�����β������ֹ��
								if(i<9){
									_strLocalRadarDataPath[i][strPathOrigin.length()+6]='\0';
								}else/* if(i<16)*/{
									_strLocalRadarDataPath[i][strPathOrigin.length()+7]='\0';
								}/*else{
									_strLocalRadarDataPath[i][strPathOriginForHide.length()+7]='\0';
								}*/
							}

							_strLocalGPSCorPath ="";
							_strLocalGPSCorPath += strPathOrigin;
							_strLocalGPSCorPath+="gps.cor";
							/*
							_strLocalGPSCorForUploadPath ="";
							_strLocalGPSCorForUploadPath += strPathOrigin;
							_strLocalGPSCorForUploadPath+="gpsForUpload.cor";
							*/
							_strLocalGPSCsvPath ="";
							_strLocalGPSCsvPath += strPathOrigin;
							_strLocalGPSCsvPath+=".csv";

							_strLocalGPSKmlPath="";
							_strLocalGPSKmlPath += strPathOrigin;
							_strLocalGPSKmlPath+=".kml";
								
							ProjectNameTab::ProjectNameRow *lpProjectRow = dynamic_cast<ProjectNameTab::ProjectNameRow *>(_projectRow.get());
							if (NULL == lpProjectRow){
								::AfxMessageBox(L"��ͷ��Ϣ����ȷ�����ܱ���ӣǣ��ļ���");
								return;
							}

							for(int i=0;i<m_nChannelCount;i++){
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
								m_SGYWriter[i]->setUploadFlag(m_nUpload);//�����Ƿ��ϴ�
								m_SGYWriter[i]->setSeperateType(m_dSeperateType);//���÷ֶ�����
								m_SGYWriter[i]->setSaveFileType(m_nSaveFileType);//���ô����ļ�����
								m_SGYWriter[i]->setPathElement(W2A( strPath ),_projectName,_projectTab);//����·������Ԫ��
								m_SGYWriter[i]->openSGYForAutomation(_strLocalRadarDataPath[i],lpProjectRow->_paramXML,i,_documentPathLength,_namePathLength);
								if ( RadarManager::Instance()->getExportGpsPos() ){//gps
									if(useIt(i)){
										m_SGYWriter[i]->openGPS(_strLocalGPSCorPath, _strLocalGPSCsvPath);
									}
								}
								if ( RadarManager::Instance()->getExportKml() ){//kml
									if(useIt(i)){
										m_SGYWriter[i]->openKml(_strLocalGPSKmlPath);
										m_SGYWriter[i]->writeKmlHead();
									}
								}
								//m_SGYWriter[i]->writeHead(1,_projectTab);//20210618hjl
								((SaveFileDataThread*)_lpThreadSaveFile[i])->_SGYWriter = m_SGYWriter[i];
								_lpThreadSaveFile[i]->start();
							}
							//���ݼ���߳�
							if (_lpThreadCheckData){
								delete _lpThreadCheckData;
								_lpThreadCheckData = NULL;
							}
							_lpThreadCheckData = new CheckDataThread;
							((CheckDataThread*)_lpThreadCheckData)->_MeasureProject = this;
							_lpThreadCheckData->start();

							//���������߳�
							if (_lpThreadCopyData){
								delete _lpThreadCopyData;
								_lpThreadCopyData = NULL;
							}
							_lpThreadCopyData = new CopyLocalThread;
							((CopyLocalThread*)_lpThreadCopyData)->_MeasureProject = this;
							_lpThreadCopyData->start();
						}
					}
				}
				break;
			default://Ĭ�϶�ͨ�� ��sgy
				{
					m_bIsBegin = true;
					_maxID = 0;

					CString cstrCreateTime;
					USES_CONVERSION;
					
					//��������·��
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
						::AfxMessageBox(L"��ͷ��Ϣ����ȷ�����ܱ���ӣǣ��ļ���");
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
						if ( RadarManager::Instance()->getExportGpsPos() ){//����gps��Ϣ�ļ�
							if(useIt(i)){
								m_SGYWriter[i]->openGPS(strPathGPS);
							}
						}
						/*if ( RadarManager::Instance()->getExportKml() ){
							if(useIt(i)){
								m_SGYWriter[i]->openKml(strPathKml);
								m_SGYWriter[i]->writeKmlHead();
							}
						}*/
						m_SGYWriter[i]->writeHead(i,_projectTab);//����ʱ���д��ͷ��Ϣ
						((SaveFileDataThread*)_lpThreadSaveFile[i])->_SGYWriter = m_SGYWriter[i];
						_lpThreadSaveFile[i]->start();
					}
				}
				break;
		}
	}else if (m_iSaveOracle == 1){//�����ݿ�

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
	if(m_nChannelCount==16){
		if (0 != access(strPathOriginForHide.c_str(), 0)){
			// if this folder not exist, create a new one.
			mkdir(strPathOriginForHide.c_str());   // ���� 0 ��ʾ�����ɹ���-1 ��ʾʧ��
		}
	}
	strPathOriginForHide+="\\";
	strPathOriginForHide += _projectName;
	strPathOriginForHide += _projectTab;
	
	int maxChannelCount;
	if(m_nChannelCount==16){
		maxChannelCount=m_nChannelCount+12;
	}/*else if(m_nChannelCount==7){
		maxChannelCount=m_nChannelCount+7;
	}else if(m_nChannelCount==8){
		maxChannelCount=m_nChannelCount+8;
	}else if(m_nChannelCount==4){
		maxChannelCount=m_nChannelCount+4;
	}*/else{
		maxChannelCount=m_nChannelCount;
	}

	//char strPathRad[CHANNELCOUNT][300];
	char **strPathRad = new char*[maxChannelCount];
	for(int i=0;i<maxChannelCount;i++){
		strPathRad[i] = new char[300];
	}
	for(int i=0;i<maxChannelCount;i++){
		strPathRad[i][0]='\0';
	}

	for(int i=0;i<maxChannelCount;i++){
		if(i<m_nChannelCount){
			strcat(strPathRad[i],strPathOrigin.c_str());
		}else{
			strcat(strPathRad[i],strPathOriginForHide.c_str());
		}
		strcat(strPathRad[i],"_");
	
		//intתchar*
		stringstream ssTemp;
		ssTemp << (i+1);
		string strTemp = ssTemp.str();
		char* cIndex = (char*) strTemp.c_str();
		
		strcat(strPathRad[i],cIndex);//(char*)(48+ i) int i ת��char*
		strcat(strPathRad[i],".rad");
	}

	ProjectNameTab::ProjectNameRow *lpProjectRow = dynamic_cast<ProjectNameTab::ProjectNameRow *>(_projectRow.get());
	ConfigureSet set;
	set.fromGroupXML( lpProjectRow->_paramXML );

	int sampleNum = RadarManager::Instance()->getSampCount( atoi ( set.get("radar", "sample").c_str()) );
	float sampleRate = RadarManager::Instance()->getSampRatio( atoi ( set.get("radar", "sampleratio" ).c_str() ), 0 );
	float distanceInterval = RadarManager::Instance()->GetTrueAccuracyFromPreclenAndPrecindex(atoi ( set.get("radar", "preclen").c_str() ),(float)atoi( set.get("radar", "precision").c_str() )) / 100.0;
	
	for(int i=0;i<maxChannelCount;i++){
		/*if(i>11&&i<16&&m_dTwelveFlag==1){
			continue;
		}*/

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
		ofile<<"LAST TRACE:"<<m_arrnRealTotalWheelCount[i]-1<<endl;
		ofile<<"STOP POSITION:"<<distanceInterval*(m_arrnRealTotalWheelCount[i]-1)<<endl;
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

		ofile.close();//�ر��ļ�
	}
}

void MeasureProject::writeRadFileForPart( int roadPart ){
	USES_CONVERSION;
	
	if(m_nUpload==1){
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
		
			//intתchar*
			stringstream ssTemp;
			ssTemp << (i+1);
			string strTemp = ssTemp.str();
			char* cIndex = (char*) strTemp.c_str();
			
			strcat(strPathRad[i],cIndex);//(char*)(48+ i) int i ת��char*
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
		if(roadPart>m_nVecOriginWheelCountForPart.size()-1){//���һ�ε����	
			wheelCount=_realTotalWheelCount-m_nVecOriginWheelCountForPart[roadPart];
		}else{
			wheelCount=m_nVecOriginWheelCountForPart[roadPart]-m_nVecOriginWheelCountForPart[roadPart-1];
		}*/
		wheelCount=m_nVecOriginWheelCountForPart[roadPart]-m_nVecOriginWheelCountForPart[roadPart-1];

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

			ofile.close();//�ر��ļ�
		}
		for(int i=0;i<m_nChannelCountForUpload;i++){
			delete []strPathRad[i];
		}
		delete []strPathRad;

	}else{//���ϴ�ģʽ������ѡ����������rad
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

		/*std::string strPathOriginForHide = W2A( m_strSavePath );
		strPathOriginForHide += _projectName;
		strPathOriginForHide += _projectTab;
		strPathOriginForHide += "\\";
		strPathOriginForHide += _projectName;
		strPathOriginForHide += _projectTab;
		strPathOriginForHide += "_";
		strPathOriginForHide += strRoadPart;
		strPathOriginForHide+="\\moreChannels";
		if (0 != access(strPathOriginForHide.c_str(), 0)){
			// if this folder not exist, create a new one.
			mkdir(strPathOriginForHide.c_str());   // ���� 0 ��ʾ�����ɹ���-1 ��ʾʧ��
		}
		strPathOriginForHide+="\\";
		strPathOriginForHide += _projectName;
		strPathOriginForHide += _projectTab;
		strPathOriginForHide += "_";
		strPathOriginForHide += strRoadPart;*/
		
		//char strPathRad[m_nChannelCountForUpload][300];
		/*int maxChannelCount;
		if(m_nChannelCount==16){
			maxChannelCount=28;
		}else{
			maxChannelCount=m_nChannelCount;
		}*/
		char **strPathRad = new char* [m_nChannelCount];
		for(int i=0;i<m_nChannelCount;i++){
			strPathRad[i]=new char [300];
		}
		for(int i=0;i<m_nChannelCount;i++){
			strPathRad[i][0]='\0';
		}

		for(int i=0;i<m_nChannelCount;i++){
			/*if(i<16){
				strcat(strPathRad[i],strPathOrigin.c_str());
			}else{
				strcat(strPathRad[i],strPathOriginForHide.c_str());
			}*/
			strcat(strPathRad[i],strPathOrigin.c_str());
			strcat(strPathRad[i],"_");
			//intתchar*
			stringstream ssTemp;
			ssTemp << (i+1);
			string strTemp = ssTemp.str();
			char* cIndex = (char*) strTemp.c_str();
			strcat(strPathRad[i],cIndex);//(char*)(48+ i) int i ת��char*
			strcat(strPathRad[i],".rad");
		}

		ProjectNameTab::ProjectNameRow *lpProjectRow = dynamic_cast<ProjectNameTab::ProjectNameRow *>(_projectRow.get());
		ConfigureSet set;
		set.fromGroupXML( lpProjectRow->_paramXML );
		int sampleNum = RadarManager::Instance()->getSampCount( atoi ( set.get("radar", "sample").c_str()) );
		float sampleRate = RadarManager::Instance()->getSampRatio( atoi ( set.get("radar", "sampleratio" ).c_str() ), 0 );
		float distanceInterval = RadarManager::Instance()->GetTrueAccuracyFromPreclenAndPrecindex(atoi ( set.get("radar", "preclen").c_str() ),(float)atoi( set.get("radar", "precision").c_str() )) / 100.0;
		int wheelCount;
		
		wheelCount=m_nVecOriginWheelCountForPart[roadPart]-m_nVecOriginWheelCountForPart[roadPart-1];

		for(int i=0;i<m_nChannelCount;i++){
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
			ofile.close();//�ر��ļ�
		}
		for(int i=0;i<m_nChannelCount;i++){
			delete []strPathRad[i];
		}
		delete []strPathRad;
	}
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
////		//intתchar*
////		stringstream ssTemp;
////		ssTemp << (i+1);
////		string strTemp = ssTemp.str();
////		char* cIndex = (char*) strTemp.c_str();
////		
////		strcat(strPathRad[i],cIndex);//(char*)(48+ i) int i ת��char*
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
////	if(roadPart>m_nVecOriginWheelCountForPart.size()-1){//���һ�ε����	
////		wheelCount=_realTotalWheelCount-m_nVecOriginWheelCountForPart[roadPart];
////	}else{
////		wheelCount=m_nVecOriginWheelCountForPart[roadPart]-m_nVecOriginWheelCountForPart[roadPart-1];
////	}*/
////	wheelCount=m_nVecOriginWheelCountForPart[roadPart]-m_nVecOriginWheelCountForPart[roadPart-1];
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
////		ofile.close();//�ر��ļ�
////	}
////
////	ConfigureSet *cfg = RadarManager::Instance()->getConfigureSet();
////	string ftpServerIp = cfg->get("ftp", "serverIP") ;
////	int port = atoi( cfg->get("ftp", "port").c_str());
////	string account = cfg->get("ftp", "account") ;
////	string password = cfg->get("ftp", "password") ;
////	//����ftp����������
////	HINTERNET hint;
////	HINTERNET hftp;
////	hint = InternetOpen(0, INTERNET_OPEN_TYPE_PRECONFIG, 0, 0, 0);
////	if (hint==NULL)  {   
////		AfxMessageBox(L"û����");
////		return;  
////	} 
////	hftp = InternetConnectA(hint, ftpServerIp.c_str(), port, account.c_str(), password.c_str(), INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
////	//hftp = InternetConnectA(hint, "192.168.1.108",21,"ftpTest","gdjw", INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
////	if (hftp==NULL) {
////		AfxMessageBox(L"����ftpʧ�ܡ�");
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
////		//intתchar*
////		stringstream ssTemp;
////		ssTemp << (RadarManager::Instance()->originalIndexToRecordIndex(i)+1);
////		string strTemp = ssTemp.str();
////		char* cIndex = (char*) strTemp.c_str();
////		
////		strcat(strOnlinePathRad[i],cIndex);//(char*)(48+ i) int i ת��char*
////		strcat(strOnlinePathRad[i],".rad");
////	}*/
////	
////	//ftp��·��	
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
////	for(int i=0;i<m_nChannelCountForUpload;i++){//ʹ�����strcat��Ч
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
////					string strTemp="�ϴ���"+ssPart.str()+"roadPart"+"��"+ssIndex.str()+"ͨ��radʧ�ܡ�Ŀ��·��:";
////					strTemp+=strOnlinePathRad[i];
////					strTemp+="\n";
////					fprintf(m_fpLog,strTemp.c_str() );
////					//AfxMessageBox(StringToCString(strTemp)); 
////					//AfxMessageBox(L"rad�ϴ�ʧ�ܡ�");
////					break;
////				}
////			}
////		}else{
////			string strTemp="��ȡ";
////			strTemp+=strPathRad[i];
////			strTemp+="ʧ��";
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
//��д������ÿpart��rad�ļ�
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
	
		//intתchar*
		stringstream ssTemp;
		ssTemp << (i+1);
		string strTemp = ssTemp.str();
		char* cIndex = (char*) strTemp.c_str();
		
		strcat(strPathRad[i],cIndex);//(char*)(48+ i) int i ת��char*
		strcat(strPathRad[i],".rad");
	}

	ProjectNameTab::ProjectNameRow *lpProjectRow = dynamic_cast<ProjectNameTab::ProjectNameRow *>(_projectRow.get());
	ConfigureSet set;
	set.fromGroupXML( lpProjectRow->_paramXML );
	int sampleNum = RadarManager::Instance()->getSampCount( atoi ( set.get("radar", "sample").c_str()) );
	float sampleRate = RadarManager::Instance()->getSampRatio( atoi ( set.get("radar", "sampleratio" ).c_str() ), 0 );
	float distanceInterval = RadarManager::Instance()->GetTrueAccuracyFromPreclenAndPrecindex(atoi ( set.get("radar", "preclen").c_str() ),(float)atoi( cfg->get("radar", "precision").c_str() )) / 100.0;
	int wheelCount;
	wheelCount=m_nVecOriginWheelCountForPart[roadPart]-m_nVecOriginWheelCountForPart[roadPart-1];

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

		ofile.close();//�ر��ļ�
	}
}*/

//��д����Araayset.txt�ļ�
void MeasureProject::writeTxtFile(){
	USES_CONVERSION;
	//����10�й��ɱ����ļ�·�� ��ʽ��path\Nametab\NameTabArrayset.txt
	std::string strPathTxt = W2A( m_strSavePath );
	strPathTxt += _projectName;
	strPathTxt += _projectTab;
	strPathTxt += "\\";
	if (0 != access(strPathTxt.c_str(), 0)){
		mkdir(strPathTxt.c_str());   // ���� 0 ��ʾ�����ɹ���-1 ��ʾʧ��
	}
	strPathTxt += _projectName;
	strPathTxt += _projectTab;
	strPathTxt+="Arrayset.txt";

	//��ȡ��ǰ���̵ĸ������
	ProjectNameTab::ProjectNameRow *lpProjectRow = dynamic_cast<ProjectNameTab::ProjectNameRow *>(_projectRow.get());
	ConfigureSet set;
	set.fromGroupXML( lpProjectRow->_paramXML );
	int sampleNum = RadarManager::Instance()->getSampCount( atoi ( set.get("radar", "sample").c_str()) );
	float sampleRate = RadarManager::Instance()->getSampRatio( atoi ( set.get("radar", "sampleratio" ).c_str() ), 0 );
	float distanceInterval = RadarManager::Instance()->GetTrueAccuracyFromPreclenAndPrecindex(atoi ( set.get("radar", "preclen").c_str() ),(float)atoi( set.get("radar", "precision").c_str() )) / 100.0;
	float tempArr[25]={1,1.9,2,2.5,2.8,2.8,3.2,4,4,4.1,5,5,6,6.4,7,7,7,8,8,12,15,16,30,30,81};
	float dieletric = tempArr [atoi ( set.get("radar", "dielectric").c_str() ) ];
	int startingPoint = atoi(set.get("radar", "directWave").c_str());
	
	//�����ǰ�����������ļ���
	ofstream ofile; 
	ofile.open(strPathTxt.c_str()); 
	ofile<<"����Ƶ��(MHz):"<<"200"<<endl;
	ofile<<"��������:"<<sampleNum<<endl;
	ofile<<"����ʱ��(΢��):"<<1.0 / sampleRate * sampleNum<<endl;
	ofile<<"��糣��:"<<dieletric<<endl;
	ofile<<"X�������(����):"<<distanceInterval*100<<endl;
	/*if(m_dTwelveFlag==1){
		ofile<<"Y������ͨ����:"<<12<<endl;
		ofile<<"Y�����ݾ�����(����):"<<"0,14,28,42,56,70,84,98,112,126,140,154"<<endl;
		ofile<<"ֱ�ﲨ���:"<<"0,0,0,0,0,0,0,0,0,0,0,0"<<endl;
	}else*/ 

	if(m_nChannelCount==4){
		ofile<<"Y������ͨ����:"<<4<<endl;
		ofile<<"Y�����ݾ�����(����):"<<"0,24,48,72"<<endl;
		ofile<<"ֱ�ﲨ���:"<<"0,0,0,0"<<endl;
		//ofile<<"ֱ�ﲨ���:"<<startingPoint<<endl;
	}else if(m_nChannelCount==6){
		ofile<<"Y������ͨ����:"<<6<<endl;
		ofile<<"Y�����ݾ�����(����):"<<"0,24,48,72,96,120"<<endl;
		//ofile<<"ֱ�ﲨ���:"<<"0,0,0,0,0,0"<<endl;
		ofile<<"ֱ�ﲨ���:"<<startingPoint<<endl;
	}else if(m_nChannelCount==7){
		ofile<<"Y������ͨ����:"<<7<<endl;
		ofile<<"Y�����ݾ�����(����):"<<"0,24,48,72,96,120,144"<<endl;
		//ofile<<"ֱ�ﲨ���:"<<"0,0,0,0,0,0"<<endl;
		ofile<<"ֱ�ﲨ���:"<<"0,0,0,0,0,0,0"<<endl;
	}else if(m_nChannelCount==8){
		ofile<<"Y������ͨ����:"<<8<<endl;
		ofile<<"Y�����ݾ���p���(����):"<<"0,24,48,72,96,120,144,168"<<endl;
		ofile<<"ֱ�ﲨ���:"<<"0,0,0,0,0,0,0,0"<<endl;
		//ofile<<"ֱ�ﲨ���:"<<startingPoint<<endl;
	}else if(m_nChannelCount==12){
		ofile<<"Y������ͨ����:"<<12<<endl;
		ofile<<"Y�����ݾ�����(����):"<<"0,14,28,42,56,70,84,98,112,126,140,154"<<endl;
		ofile<<"ֱ�ﲨ���:"<<"0,0,0,0,0,0,0,0,0,0,0,0"<<endl;
	}else if(m_nChannelCount==15){
		ofile<<"Y������ͨ����:"<<15<<endl;
		ofile<<"Y�����ݾ�����(����):"<<"0,12,24,36,48,60,72,84,96,108,120,132,144,156,168"<<endl;
		ofile<<"ֱ�ﲨ���:"<<"0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"<<endl;
		//ofile<<"ֱ�ﲨ���:"<<startingPoint<<endl;
	}else if(m_nChannelCount==16){
		ofile<<"Y������ͨ����:"<<16<<endl;
		ofile<<"Y�����ݾ�����(����):"<<"0,14,28,42,56,70,84,98,112,126,140,154,155,156,157,158"<<endl;
		ofile<<"ֱ�ﲨ���:"<<"0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"<<endl;
	}else{
		ofile<<"Y������ͨ����:"<<6<<endl;
		ofile<<"Y�����ݾ�����(����):"<<"0,24,48,72,96,120"<<endl;
		ofile<<"ֱ�ﲨ���:"<<0<<endl;
	}
	ofile.close();//�ر��ļ�
}


void MeasureProject::uploadTxtFileForGroup( int part ){
	USES_CONVERSION;
	//����10�й��ɱ����ļ�·�� ��ʽ��path\Nametab\NameTabArrayset.txt
	std::string strPathTxt = W2A( m_strSavePath );
	strPathTxt += _projectName;
	strPathTxt += _projectTab;
	strPathTxt += "\\";
	strPathTxt += _projectName;
	strPathTxt += _projectTab;
	strPathTxt+="Arrayset.txt";

	//��ȡftp���� ip �˿� �û� ����
	ConfigureSet *cfg = RadarManager::Instance()->getConfigureSet();
	string ftpServerIp = cfg->get("ftp", "serverIP");
	int port = atoi( cfg->get("ftp", "port").c_str());
	string account = cfg->get("ftp", "account");
	string password = cfg->get("ftp", "password");

	//����ftp����������
	/*HINTERNET hint;
	HINTERNET hftp;
	hint = InternetOpen(0, INTERNET_OPEN_TYPE_PRECONFIG, 0, 0, 0);
	if (hint==NULL)  {   
		AfxMessageBox(L"û����");
		return;  
	} 
	hftp = InternetConnectA(hint, ftpServerIp.c_str(), port, account.c_str(), password.c_str(), INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
	if (hftp==NULL) {
		AfxMessageBox(L"����ftpʧ��,���ڳ�������");
		return;
	}*/
	HINTERNET hint;
	HINTERNET hftp;
	hint = InternetOpen(0, INTERNET_OPEN_TYPE_PRECONFIG, 0, 0, 0);
	int hintFailCount=0;
	while(hint==NULL&&hintFailCount<100){
		Sleep(1000);
		hintFailCount=hintFailCount+1;
		hint = InternetOpen(0, INTERNET_OPEN_TYPE_PRECONFIG, 0, 0, 0);
	}
	if (hint==NULL)  {   
		AfxMessageBox(L"û����");
		return;  
	}
	hftp = InternetConnectA(hint, ftpServerIp.c_str(), port, account.c_str(), password.c_str(), INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
	int hftpFailCount=0;
	while(hftp==NULL&&hftpFailCount<100){
		Sleep(1000);
		hftpFailCount=hftpFailCount+1;
		hftp = InternetConnectA(hint, ftpServerIp.c_str(), port, account.c_str(), password.c_str(), INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
	}
	if (hftp==NULL) {
		AfxMessageBox(L"����ftpʧ�ܡ�");
		return;
	}


	//����ftp�ϴ�·��
	std::string strOnlinePathTxt = m_strFtpFolderPath;
	strOnlinePathTxt += "Arrayset.txt";

	//������Arrayset.txt�ϴ���ftp��
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
				string strTemp="�ϴ���"+ssPart.str()+"part Arrayset.txtʧ�ܡ�Ŀ��·��:";
				strTemp+=strOnlinePathTxt.c_str();
				strTemp+="\n";
				fprintf(m_fpLog,strTemp.c_str() );
				//AfxMessageBox(StringToCString(strTemp)); 
				//AfxMessageBox(L"Arrayset.txt�ϴ�ʧ�ܡ�");
				break;
			}
		}
	}else{
		string strTemp="��ȡ";
		strTemp+=strPathTxt.c_str();
		strTemp+="ʧ��";
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
	//��socket
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
	//��������
	char *gpsJsonBuff = new char[1024];
	char *gpsSendBuff = new char[1024];
	time_t mytime = time(NULL);
	std::string strTimeStamp = intToString(mytime-10);
	//nGpsDataBufferLen+=sprintf(gpsDataBuffer+nGpsDataBufferLen, "{\"wheelCount\":%s, \"latitude\":\"%s\", \"longitude\":\"%s\", \"time\":\"%s %s\"},", dataList[1].c_str(), dataList[2].c_str(), dataList[3].c_str(), dataList[5].c_str(), dataList[6].c_str());//����İ��ա������ݡ�����
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
		//���ݻظ��������в���
		gpsRecieveBuff[nGpsRecieveBuffLen]='\0';
		std::string strRecieve=string(gpsRecieveBuff);
	}else{
		//û���յ��ظ�����
	}
	delete []gpsRecieveBuff;
	
	delete []gpsJsonBuff;
	delete []gpsSendBuff;
	closesocket(sta_socket);
}


//���ڷָ��ַ��ķ�������
std::vector< std::string > SplitStr( std::string const& str, char splitChar ){//���ַ�splitChar�ָ�Ŀ���ַ���str������vector
	std::vector< std::string > result;
	std::string tmpStr = str;
	size_t findP;
	while ((findP = tmpStr.find(splitChar)) != std::string::npos){//��ͷ��������� �ҳ�ÿ��splitChar��λ�ò��ָ�
		std::string subStr = tmpStr.substr(0, findP);
		tmpStr = tmpStr.substr(findP + 1, tmpStr.length() - findP - 1);
		result.push_back( subStr );
	}
	if (tmpStr.length() > 0){
		result.push_back(tmpStr);
	}
	return result;
}

//��д����д�����ĵ���csv�ļ�
void MeasureProject::writeNameCsvFile(){
	std::vector< std::string > namePart = SplitStr( _projectName, '_');//���������Ʒָ�õ�������Ϣ
	if ( namePart.size() < 2 ){
		return;
	}

	USES_CONVERSION;
	//����ļ�·�� ��ʽpath\SurveyLineName.csv
	std::string strPathCsv = W2A( m_strSavePath );
	strPathCsv+="SurveyLineName.csv";

	//���빤�������е���Ϣ
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

	//���������Ϣ
	ProjectNameTab::ProjectNameRow *lpProjectRow = dynamic_cast<ProjectNameTab::ProjectNameRow *>(_projectRow.get());
	ConfigureSet set;
	set.fromGroupXML( lpProjectRow->_paramXML );
	float distanceInterval = RadarManager::Instance()->GetTrueAccuracyFromPreclenAndPrecindex(atoi ( set.get("radar", "preclen").c_str() ),(float)atoi( set.get("radar", "precision").c_str() )) / 100.0;
	float totalDistance=distanceInterval*(float)m_arrnRecordTotalWheelCount[0];
	fprintf(_lpFile, "%d", (int)totalDistance);

	//��β����
	fprintf(_lpFile, "\n");

	//�ر��ļ�
	fclose( _lpFile );
	_lpFile = 0;

}

//16ͨ����28ͨ��/1-12������17-28
void MeasureProject::copyRadarData(){
	int nCopyChannelCount;
	if(m_nChannelCount==16){
		nCopyChannelCount=12;
	}else{
		nCopyChannelCount=m_nChannelCount;
	}
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
	//strPathOriginForHide+="\\moreChannels\\";
	strPathOriginForHide += "\\moreChannels\\";
	if (0 != access(strPathOriginForHide.c_str(), 0)){
		// if this folder not exist, create a new one.
		mkdir(strPathOriginForHide.c_str());   // ���� 0 ��ʾ�����ɹ���-1 ��ʾʧ��
	}
	strPathOriginForHide += _projectName;
	strPathOriginForHide += _projectTab;
	
	std::vector<std::string> vecStrPathIn;
	for(int i=0;i<nCopyChannelCount;i++){
		std::string strPath;
		strPath=strPathOrigin;
		strPath+="_";
		//intתchar*
		stringstream ssTemp;
		ssTemp << (i+1);
		string strTemp = ssTemp.str();
		strPath+=strTemp;
		strPath+=".rd3";
		//strcat(strPathIn[i],".gdd");
		vecStrPathIn.push_back(strPath);
	}

	std::vector<std::string> vecStrPathOut;
	for(int i=0;i<nCopyChannelCount;i++){
		std::string strPath;
		strPath=strPathOriginForHide;
		strPath+="_";
		//intתchar*
		stringstream ssTemp;
		ssTemp << (i+m_nChannelCount+1);
		string strTemp = ssTemp.str();
		strPath+=strTemp;
		strPath+=".rd3";
		//strcat(strPathIn[i],".gdd");
		vecStrPathOut.push_back(strPath);
	}

	ofstream ofile; 
	ifstream ifile;
	for(int i=0;i<nCopyChannelCount;i++){
		ofile.open(vecStrPathOut[i].c_str(),ios::binary); 
		ifile.open(vecStrPathIn[i].c_str(),ios::binary); 
		if(ofile!=NULL && ifile!=NULL){
			ofile<<ifile.rdbuf();
		}
		ifile.close();
		ofile.close();
	}
	/*char strPathIn[12][300];
	for(int i=0;i<12;i++){
		strPathIn[i][0]='\0';
	}
	//int inIndex[12]={0,1,2,3,4,5,6,7,8,9,10,11};
	for(int i=0;i<12;i++){
		if(i<12){
			strcat(strPathIn[i],strPathOrigin.c_str());
		}
		strcat(strPathIn[i],"_");
		//intתchar*
		stringstream ssTemp;
		ssTemp << (i+1);
		string strTemp = ssTemp.str();
		char* cIndex = (char*) strTemp.c_str();
		strcat(strPathIn[i],cIndex);//(char*)(48+ i) int i ת��char*
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
		//intתchar*
		stringstream ssTemp;
		ssTemp << (i+16+1);
		string strTemp = ssTemp.str();
		char* cIndex = (char*) strTemp.c_str();
		strcat(strPathOut[i],cIndex);//(char*)(48+ i) int i ת��char*
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
	}*/
}

/*void MeasureProject::copyRadarDataForSevenChannel(){
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
	//strPathOriginForHide+="\\moreChannels\\";
	strPathOriginForHide += "\\moreChannels\\";
	if (0 != access(strPathOriginForHide.c_str(), 0)){
		// if this folder not exist, create a new one.
		mkdir(strPathOriginForHide.c_str());   // ���� 0 ��ʾ�����ɹ���-1 ��ʾʧ��
	}
	strPathOriginForHide += _projectName;
	strPathOriginForHide += _projectTab;
	
	char strPathIn[7][300];
	for(int i=0;i<7;i++){
		strPathIn[i][0]='\0';
	}

	for(int i=0;i<7;i++){
		if(i<7){
			strcat(strPathIn[i],strPathOrigin.c_str());
		}
		strcat(strPathIn[i],"_");
		//intתchar*
		stringstream ssTemp;
		ssTemp << (i+1);
		string strTemp = ssTemp.str();
		char* cIndex = (char*) strTemp.c_str();
		strcat(strPathIn[i],cIndex);//(char*)(48+ i) int i ת��char*
		strcat(strPathIn[i],".rd3");
		//strcat(strPathIn[i],".gdd");
	}

	char strPathOut[7][300];
	for(int i=0;i<7;i++){
		strPathOut[i][0]='\0';
	}
	//int outIndex[13]={12,13,14,15,16,17,18,19,20,21,22,23,24};
	for(int i=0;i<7;i++){
		strcat(strPathOut[i],strPathOriginForHide.c_str());
		strcat(strPathOut[i],"_");
		//intתchar*
		stringstream ssTemp;
		ssTemp << (i+7+1);
		string strTemp = ssTemp.str();
		char* cIndex = (char*) strTemp.c_str();
		strcat(strPathOut[i],cIndex);//(char*)(48+ i) int i ת��char*
		strcat(strPathOut[i],".rd3");
		//strcat(strPathOut[i],".gdd");
	}

	//ifstream in(strPathIn[0],ios::binary);
	//ofstream out(strPathOut[0],ios::binary);
	ofstream ofile; 
	ifstream ifile;
	for(int i=0;i<7;i++){
		ofile.open(strPathOut[i],ios::binary); 
		ifile.open(strPathIn[i],ios::binary); 
		if(ofile!=NULL && ifile!=NULL){
			ofile<<ifile.rdbuf();
		}
		ifile.close();
		ofile.close();
	}
}*/

//16ͨ����28ͨ��/1-12������17-28
/*void MeasureProject::copyRadarDataForPart(){
	int needCopiedChannelCount;
	if(m_nChannelCount==16){
		needCopiedChannelCount=12;
	}else{
		needCopiedChannelCount=m_nChannelCount;
	}
	for(int part=1;part<m_dRoadPart+1;part++){
		USES_CONVERSION;
		std::string strPathOrigin = W2A( m_strSavePath );
		strPathOrigin += _projectName;
		strPathOrigin += _projectTab;
		strPathOrigin += "\\";
		strPathOrigin += _projectName;
		strPathOrigin += _projectTab;
		strPathOrigin += "_";
		strPathOrigin += intToString(part);
		strPathOrigin += "\\";
		strPathOrigin += _projectName;
		strPathOrigin += _projectTab;
		strPathOrigin += "_";
		strPathOrigin += intToString(part);

		std::string strPathOriginForHide = W2A( m_strSavePath );
		strPathOriginForHide += _projectName;
		strPathOriginForHide += _projectTab;
		strPathOriginForHide += "\\";
		strPathOriginForHide += _projectName;
		strPathOriginForHide += _projectTab;
		strPathOriginForHide += "_";
		strPathOriginForHide += intToString(part);
		//strPathOriginForHide+="\\moreChannels\\";
		strPathOriginForHide += "\\moreChannels\\";
		if (0 != access(strPathOriginForHide.c_str(), 0)){
			// if this folder not exist, create a new one.
			mkdir(strPathOriginForHide.c_str());   // ���� 0 ��ʾ�����ɹ���-1 ��ʾʧ��
		}
		strPathOriginForHide += _projectName;
		strPathOriginForHide += _projectTab;
		strPathOriginForHide += "_";
		strPathOriginForHide += intToString(part);
		
		std::vector<std::string> vecStrRd3PathIn;
		for(int i=0;i<needCopiedChannelCount;i++){
			std::string strPathIn;
			strPathIn=strPathOrigin;
			strPathIn+="_";
			strPathIn+=intToString(i+1);
			strPathIn+=".rd3";
			vecStrRd3PathIn.push_back(strPathIn);
		}

		std::vector<std::string> vecStrRadPathIn;
		for(int i=0;i<needCopiedChannelCount;i++){
			std::string strPathIn;
			strPathIn=strPathOrigin;
			strPathIn+="_";
			strPathIn+=intToString(i+1);
			strPathIn+=".rad";
			vecStrRadPathIn.push_back(strPathIn);
		}

		std::vector<std::string> vecStrRd3PathOut;
		for(int i=0;i<needCopiedChannelCount;i++){
			std::string strPathOut;
			strPathOut=strPathOriginForHide;
			strPathOut+="_";
			strPathOut+=intToString(i+1+m_nChannelCount);
			strPathOut+=".rd3";
			vecStrRd3PathOut.push_back(strPathOut);
		}

		std::vector<std::string> vecStrRadPathOut;
		for(int i=0;i<needCopiedChannelCount;i++){
			std::string strPathOut;
			strPathOut=strPathOriginForHide;
			strPathOut+="_";
			strPathOut+=intToString(i+1+m_nChannelCount);
			strPathOut+=".rad";
			vecStrRadPathOut.push_back(strPathOut);
		}

		ofstream ofile; 
		ifstream ifile;
		for(int i=0;i<needCopiedChannelCount;i++){
			ofile.open(vecStrRd3PathOut[i].c_str(),ios::binary); 
			ifile.open(vecStrRd3PathIn[i].c_str(),ios::binary); 
			if(ofile!=NULL && ifile!=NULL){
				ofile<<ifile.rdbuf();
			}
			ifile.close();
			ofile.close();

			ofile.open(vecStrRadPathOut[i].c_str(),ios::binary); 
			ifile.open(vecStrRadPathIn[i].c_str(),ios::binary); 
			if(ofile!=NULL && ifile!=NULL){
				ofile<<ifile.rdbuf();
			}
			ifile.close();
			ofile.close();
		}
	}
}*/

void MeasureProject::copyRadarDataForPart(int part){
	int needCopiedChannelCount;
	if(m_nChannelCount==16){
		needCopiedChannelCount=12;
	}else{
		needCopiedChannelCount=m_nChannelCount;
	}
	USES_CONVERSION;
	std::string strPathOrigin = W2A( m_strSavePath );
	strPathOrigin += _projectName;
	strPathOrigin += _projectTab;
	strPathOrigin += "\\";
	strPathOrigin += _projectName;
	strPathOrigin += _projectTab;
	strPathOrigin += "_";
	strPathOrigin += intToString(part);
	strPathOrigin += "\\";
	strPathOrigin += _projectName;
	strPathOrigin += _projectTab;
	strPathOrigin += "_";
	strPathOrigin += intToString(part);

	std::string strPathOriginForHide = W2A( m_strSavePath );
	strPathOriginForHide += _projectName;
	strPathOriginForHide += _projectTab;
	strPathOriginForHide += "\\";
	strPathOriginForHide += _projectName;
	strPathOriginForHide += _projectTab;
	strPathOriginForHide += "_";
	strPathOriginForHide += intToString(part);
	//strPathOriginForHide+="\\moreChannels\\";
	strPathOriginForHide += "\\moreChannels\\";
	if (0 != access(strPathOriginForHide.c_str(), 0)){
		// if this folder not exist, create a new one.
		mkdir(strPathOriginForHide.c_str());   // ���� 0 ��ʾ�����ɹ���-1 ��ʾʧ��
	}
	strPathOriginForHide += _projectName;
	strPathOriginForHide += _projectTab;
	strPathOriginForHide += "_";
	strPathOriginForHide += intToString(part);
	
	std::vector<std::string> vecStrRd3PathIn;
	for(int i=0;i<needCopiedChannelCount;i++){
		std::string strPathIn;
		strPathIn=strPathOrigin;
		strPathIn+="_";
		strPathIn+=intToString(i+1);
		strPathIn+=".rd3";
		vecStrRd3PathIn.push_back(strPathIn);
	}

	std::vector<std::string> vecStrRadPathIn;
	for(int i=0;i<needCopiedChannelCount;i++){
		std::string strPathIn;
		strPathIn=strPathOrigin;
		strPathIn+="_";
		strPathIn+=intToString(i+1);
		strPathIn+=".rad";
		vecStrRadPathIn.push_back(strPathIn);
	}

	std::vector<std::string> vecStrRd3PathOut;
	for(int i=0;i<needCopiedChannelCount;i++){
		std::string strPathOut;
		strPathOut=strPathOriginForHide;
		strPathOut+="_";
		strPathOut+=intToString(i+1+m_nChannelCount);
		strPathOut+=".rd3";
		vecStrRd3PathOut.push_back(strPathOut);
	}

	std::vector<std::string> vecStrRadPathOut;
	for(int i=0;i<needCopiedChannelCount;i++){
		std::string strPathOut;
		strPathOut=strPathOriginForHide;
		strPathOut+="_";
		strPathOut+=intToString(i+1+m_nChannelCount);
		strPathOut+=".rad";
		vecStrRadPathOut.push_back(strPathOut);
	}

	ofstream ofile; 
	ifstream ifile;
	for(int i=0;i<needCopiedChannelCount;i++){
		ofile.open(vecStrRd3PathOut[i].c_str(),ios::binary); 
		ifile.open(vecStrRd3PathIn[i].c_str(),ios::binary); 
		if(ofile!=NULL && ifile!=NULL){
			ofile<<ifile.rdbuf();
		}
		ifile.close();
		ofile.close();

		ofile.open(vecStrRadPathOut[i].c_str(),ios::binary); 
		ifile.open(vecStrRadPathIn[i].c_str(),ios::binary); 
		if(ofile!=NULL && ifile!=NULL){
			ofile<<ifile.rdbuf();
		}
		ifile.close();
		ofile.close();
	}

	m_nCopyFinishedFlag=part;
}

//��������Araayset.txt�ļ�����·���ļ�
void MeasureProject::copyTxtFileForPart(int part){
	USES_CONVERSION;
	std::string strPathIn = W2A( m_strSavePath );
	strPathIn += _projectName;
	strPathIn += _projectTab;
	strPathIn += "\\";
	strPathIn += _projectName;
	strPathIn += _projectTab;
	strPathIn += "Arrayset.txt";

	std::string strPathOut = W2A( m_strSavePath );
	strPathOut += _projectName;
	strPathOut += _projectTab;
	strPathOut += "\\";
	strPathOut += _projectName;
	strPathOut += _projectTab;
	strPathOut += "_";
	strPathOut += intToString(part);
	strPathOut += "\\";
	strPathOut += _projectName;
	strPathOut += _projectTab;
	strPathOut += "_";
	strPathOut += intToString(part);
	strPathOut += "Arrayset.txt";
	
	ofstream ofile; 
	ifstream ifile;
	ofile.open(strPathOut.c_str()); 
	ifile.open(strPathIn.c_str()); 
	if(ofile!=NULL && ifile!=NULL){
		ofile<<ifile.rdbuf();
	}
	ifile.close();
	ofile.close();
}

void MeasureProject::StartAutoCorrection(){
	m_nVecVecAutoCorrectionPointIndex.clear();
	//m_nVecAutoCorrectionPointIndexCount.clear();
	m_nVecAutoCorrectionResult.clear();
	for(int i=0;i<m_nChannelCount;i++){
		std::vector<int> vecTemp;
		/*for(int j=0;j<100;j++){
			vecTemp.push_back(-1);
		}*/
		m_nVecVecAutoCorrectionPointIndex.push_back(vecTemp);
		//m_nVecAutoCorrectionPointIndexCount.push_back(0);
		m_nVecAutoCorrectionResult.push_back(-1);
	}
	if (_lpThreadAutoCorrection){
		delete _lpThreadAutoCorrection;
		_lpThreadAutoCorrection = NULL;
	}
	_lpThreadAutoCorrection = new AutoCorrectionCmd;
	((AutoCorrectionCmd*)_lpThreadAutoCorrection)->_MeasureProject = this;
	_lpThreadAutoCorrection->start();
}

//��ͨ��������״�����������ݵ��Զ�΢���ӳ��߳�
void MeasureProject::addDataToAutoCorrection( RadarData *rd, int channelIndex ){
	if(channelIndex>=m_nChannelCount){
		return;
	}

	((AutoCorrectionCmd*)_lpThreadAutoCorrection)->addData(rd);
		
}

void MeasureProject::AutoCorrection(RadarData *rd ){
	//������������vector��Ӧλ����
	short sWaveStartThresHold=2000;
	short sPeakValue=sWaveStartThresHold;
	int nPeakIndex=-1;
	//��ȡͨ�����index
	int len = 0;//���ڲ鿴��������
	unsigned char *tempBuff = ( unsigned char *)(rd->getData( len ));
	int index = tempBuff[1] - 114; //TrTr��r��ascii����114
	//ת��short��Ѱ�Ҳ���
	short *lpBuff = ( short *)(rd->getData( len ));//ת��short
	if ( !lpBuff ){
		return;
	}
	int nSampleCount=len/2;
	int nWaveOverwhelmIndex=0;//���ڼ�¼������ʱ��ʼ��index
	bool bIsWaveOverwhelm=false;//�жϹ�����
	/*short arrTemp[512];
	for(int i=0;i<512;i++){

	}*/
	for(int i=4;i<nSampleCount-4;i++){//��ͷ��8���ֽ���������¼��Ϣ��
		if(lpBuff[i]<sWaveStartThresHold){
			continue;
		}
		if(lpBuff[i]>sPeakValue){
			sPeakValue=lpBuff[i];
		}else if(lpBuff[i]==sPeakValue&&sPeakValue==32767&&bIsWaveOverwhelm==false){//��¼������ʱ��ʼ��index
			nWaveOverwhelmIndex=i;
			bIsWaveOverwhelm=true;
		}else if(lpBuff[i]<sPeakValue&&sPeakValue==32767&&bIsWaveOverwhelm==true){//��¼������ʱ��������һ��index
			if((i+(nWaveOverwhelmIndex-1))%2==0){//����index�պ����м��ֱ����
				nPeakIndex=(i+nWaveOverwhelmIndex-1)/2;
			}
			//�ȽϹ����Ϳ�ʼǰһ���������һ����ֵ�ó�����index
			if(lpBuff[nWaveOverwhelmIndex-1]>=lpBuff[i]){
				nPeakIndex=(i+nWaveOverwhelmIndex-1-1)/2;
			}else{
				nPeakIndex=(i+nWaveOverwhelmIndex-1+1)/2;
			}
			break;
		}else{
			nPeakIndex=i-1;
			break;
		}
	}
	if(m_nVecVecAutoCorrectionPointIndex[index].size()>99){//��ֹ��μ���
		return;
	}
	m_nVecVecAutoCorrectionPointIndex[index].push_back(nPeakIndex);	
	//m_nVecVecAutoCorrectionPointIndex[index][m_nVecAutoCorrectionPointIndexCount[index]]=nPeakIndex;
	//m_nVecAutoCorrectionPointIndexCount[index]+=1;

	//��indexͨ�������������� ������ƽ��ֵ��
	if(m_nVecVecAutoCorrectionPointIndex[index].size()>99){
	//if(m_nVecAutoCorrectionPointIndexCount[index]>99){
		for(int i=0;i<m_nVecVecAutoCorrectionPointIndex[index].size();i++){
			m_nVecAutoCorrectionResult[index]=m_nVecAutoCorrectionResult[index]+m_nVecVecAutoCorrectionPointIndex[index][i];
		}
		m_nVecAutoCorrectionResult[index]=m_nVecAutoCorrectionResult[index]/m_nVecVecAutoCorrectionPointIndex[index].size();
	}

	//����ͨ���ķ�ֵ����ֵ�����˾ͼ��㲢���ͽ��
	bool bFinished=true;
	for(int i=0;i<m_nChannelCount;i++){
		if(m_nVecAutoCorrectionResult[i]==-1){
			bFinished=false;
			break;
		}
	}
	if(bFinished){
		int nMinPeakIndex=nSampleCount;
		for(int i=0;i<m_nChannelCount;i++){//�ҳ����в�������λ�õ���Сֵ
			if(m_nVecAutoCorrectionResult[i]!=0){
				if(nMinPeakIndex>m_nVecAutoCorrectionResult[i]){
					nMinPeakIndex=m_nVecAutoCorrectionResult[i];
				}
			}
		}
		vector<int> nVecResult;
		for(int i=0;i<m_nChannelCount;i++){//��ȡÿ��ͨ����У��ֵ
			nVecResult.push_back(m_nVecAutoCorrectionResult[i]-nMinPeakIndex);
		}
		for(int i=0;i<m_nChannelCount;i++){//��ȡÿ��ͨ����У��ֵ
			if(nVecResult[i]>nSampleCount-4){
				nVecResult[i]=0;
			}
		}

		//���ͽ����RadarManager
		RadarManager::Instance()->SetAutoCorrectionResult(nVecResult);
		//���ڴ�
		//m_nVecVecAutoCorrectionPointIndex.clear();
		//m_nVecAutoCorrectionResult.clear();
	}
}
/*
//���ftp���ļ�·��
void MeasureProject::getOnlinePath(){
	USES_CONVERSION;

	//��ȡftp�˿���Ϣ ip �˿� �û� ����
	ConfigureSet *cfg = RadarManager::Instance()->getConfigureSet();
	string ftpServerIp = cfg->get("ftp", "serverIP");
	int port = atoi( cfg->get("ftp", "port").c_str());
	string account = cfg->get("ftp", "account");
	string password = cfg->get("ftp", "password");

	//��ȡftp���Ӿ��
	HINTERNET hint;
	HINTERNET hftp;
	hint = InternetOpen(0, INTERNET_OPEN_TYPE_PRECONFIG, 0, 0, 0);
	if (hint==NULL)  {   
		AfxMessageBox(L"û����");
		return;  
	} 
	hftp = InternetConnectA(hint, ftpServerIp.c_str(), port, account.c_str(), password.c_str(), INTERNET_SERVICE_FTP, 0, 0);
	if (hftp==NULL) {
		AfxMessageBox(L"����ftpʧ�ܡ�");
		return;
	}
	while(hftp==NULL){
		hftp = InternetConnectA(hint, ftpServerIp.c_str(), port, account.c_str(), password.c_str(), INTERNET_SERVICE_FTP, 0, 0);
	}
	
	//���ftp���ļ�·�� ��ʽ.\radarData\NameTab\NameTab_1\NameTab_1_i.rd3��.\radarData\NameTab\NameTab_1\moreChannels\NameTab_1_i.rd3
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
	
		//intתchar*
		stringstream ssTemp;
		//ssTemp << (RadarManager::Instance()->originalIndexToRecordIndex(i)+1);
		ssTemp << (i+1);
		string strTemp = ssTemp.str();
		char* cIndex = (char*) strTemp.c_str();
		
		strcat(_strOnlineRadarDataPath[i],cIndex);
		strcat(_strOnlineRadarDataPath[i],".rd3");
	}

	for(int i=0;i<CHANNELCOUNT;i++){//�ַ�����β������ֹ��
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
//���ftp���ļ�·��
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
	//	mkdir(strPathOrigin.c_str());   // ���� 0 ��ʾ�����ɹ���-1 ��ʾʧ��
	//}
	//strPathOrigin+="\\";
	//strPathOrigin += _projectName;
	//strPathOrigin += _projectTab;
	//strPathOrigin+="_1";
	//if (0 != access(strPathOrigin.c_str(), 0)){
	//	// if this folder not exist, create a new one.
	//	mkdir(strPathOrigin.c_str());   // ���� 0 ��ʾ�����ɹ���-1 ��ʾʧ��
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
	//	mkdir(strPathOriginForHide.c_str());   // ���� 0 ��ʾ�����ɹ���-1 ��ʾʧ��
	//}
	//strPathOriginForHide += "\\";
	//strPathOriginForHide += _projectName;
	//strPathOriginForHide += _projectTab;
	//strPathOriginForHide += "_1";
	//
	//for(int i=0;i<CHANNELCOUNT;i++){//ʹ�����strcat��Ч
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
	//	//intתchar*
	//	stringstream ssTemp;
	//	//ssTemp << (RadarManager::Instance()->originalIndexToRecordIndex(i)+1);
	//	ssTemp << (i+1);
	//	string strTemp = ssTemp.str();
	//	char* cIndex = (char*) strTemp.c_str();
	//	
	//	strcat(_strOnlineRadarDataPath[i],cIndex);//(char*)(48+ i) int i ת��char*
	//	strcat(_strOnlineRadarDataPath[i],".rd3");
	//}

	//for(int i=0;i<CHANNELCOUNT;i++){//�ַ�����β������ֹ��
	//	if(i<9){
	//		_strOnlineRadarDataPath[i][strPathOrigin.length()+6]='\0';
	//	}else if(i<12){
	//		_strOnlineRadarDataPath[i][strPathOrigin.length()+7]='\0';
	//	}else{
	//		_strOnlineRadarDataPath[i][strPathOriginForHide.length()+7]='\0';
	//	}
	//}

	//��ȡftp�˿���Ϣ ip �˿� �û� ����
	ConfigureSet *cfg = RadarManager::Instance()->getConfigureSet();
	string ftpServerIp = cfg->get("ftp", "serverIP");
	int port = atoi( cfg->get("ftp", "port").c_str());
	string account = cfg->get("ftp", "account");
	string password = cfg->get("ftp", "password");

	//��ȡftp���Ӿ��
	HINTERNET hint;
	HINTERNET hftp;
	hint = InternetOpen(0, INTERNET_OPEN_TYPE_PRECONFIG, 0, 0, 0);
	if (hint==NULL)  {   
		AfxMessageBox(L"û����");
		return;  
	} 
	hftp = InternetConnectA(hint, ftpServerIp.c_str(), port, account.c_str(), password.c_str(), INTERNET_SERVICE_FTP, 0, 0);
	if (hftp==NULL) {
		AfxMessageBox(L"����ftpʧ�ܡ�");
	}
	while(hftp==NULL){
		hftp = InternetConnectA(hint, ftpServerIp.c_str(), port, account.c_str(), password.c_str(), INTERNET_SERVICE_FTP, 0, 0);
	}
	//��ҵ��/��·��/�豸��/ʱ���/
	//���ftp���ļ�·�� ��ʽ.\radarData\NameTab\NameTab_1\NameTab_1_i.rd3��.\radarData\NameTab\NameTab_1\moreChannels\NameTab_1_i.rd3
	//���ftp���ļ�·�� ��ʽ.\radarData\ProjectName\roadName\equipmentName\Tab\NameTab_1\NameTab_1_i.rd3��.\radarData\ProjectName\roadName\equipmentName\Tab\NameTab_1\moreChannels\NameTab_1_i.rd3
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
	
		//intתchar*
		stringstream ssTemp;
		//ssTemp << (RadarManager::Instance()->originalIndexToRecordIndex(i)+1);
		ssTemp << (i+1);
		string strTemp = ssTemp.str();
		char* cIndex = (char*) strTemp.c_str();
		
		strcat(_strOnlineRadarDataPath[i],cIndex);
		strcat(_strOnlineRadarDataPath[i],".rd3");
	}

	for(int i=0;i<CHANNELCOUNT;i++){//�ַ�����β������ֹ��
		if(i<9){
			_strOnlineRadarDataPath[i][strPathOrigin.length()+6]='\0';
		}else if(i<12){
			_strOnlineRadarDataPath[i][strPathOrigin.length()+7]='\0';
		}else{
			_strOnlineRadarDataPath[i][strPathOriginForHide.length()+7]='\0';
		}
	}
}*/


//////��ĳpart�������ϴ���ftp��
////void MeasureProject::copyRadarDataOnline(int part){
////	
////	ConfigureSet *cfg = RadarManager::Instance()->getConfigureSet();
////	//��ȡftp����Ϣ ip��ַ �˿� �û� ����
////	string ftpServerIp = cfg->get("ftp", "serverIP");
////	int port = atoi( cfg->get("ftp", "port").c_str());
////	string account = cfg->get("ftp", "account");
////	string password = cfg->get("ftp", "password");
////
////	//��ȡftp���Ӿ��
////	HINTERNET hint;
////	HINTERNET hftp;
////	hint = InternetOpen(0, INTERNET_OPEN_TYPE_PRECONFIG, 0, 0, 0);
////	if (hint==NULL)  {   
////		AfxMessageBox(L"û����");
////		return;  
////	} 
////	hftp = InternetConnectA(hint, ftpServerIp.c_str(), port, account.c_str(), password.c_str(), INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
////	if (hftp==NULL) {
////		AfxMessageBox(L"����ftpʧ�ܡ�");
////		return;
////	}
////	while(hftp==NULL){
////		hftp = InternetConnectA(hint, ftpServerIp.c_str(), port, account.c_str(), password.c_str(), INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
////	}
////
////	//����cor��rd3�ı���������·��
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
////	//�����ϴ�·��
////	for(int i=0; i<m_nChannelCountForUpload; i++){
////		//==============================��part�õ���ǰ���豾��·��������·��==============================//
////		//ǰ12��.\radarData\NameTab\NameTab_1\NameTab_1_i.rd3��.\radarData\NameTab\NameTab_1\moreChannels\NameTab_1_i.rd3���
////		//ǰ12��.\radarData\ProjectName\roadName\equipmentName\Tab\NameTab_1\NameTab_1_i.rd3��.\radarData\ProjectName\roadName\equipmentName\Tab\NameTab_1\moreChannels\NameTab_1_i.rd3
////		//ǰ12��.\radarData\NameTab\NameTab_part\NameTab_part_i.rd3��.\radarData\NameTab\NameTab_part\moreChannels\NameTab_part_i.rd3
////		//ǰ12��localPath\NameTab\NameTab_1\NameTab_1_i.rd3��localPath\NameTab\NameTab_1\moreChannels\NameTab_1_i.rd3���
////		//ǰ12��localPath\NameTab\NameTab_part\NameTab_part_i.rd3��localPath\NameTab\NameTab_part\moreChannels\NameTab_part_i.rd3
////		
////		std::stringstream ss;
////		ss << part;
////
////		std::string localFilePath,onlineFilePath;
////		localFilePath = _strLocalRadarDataPath[i];
////		//��NameTab_1�ļ����޸ĳ�NameTab_part
////		localFilePath.replace(_documentPathLength+_namePathLength*2+2, 1, ss.str() );//string.replace(a,b,c)��aλ��ʼ��b���ַ��滻��c
////		//��NameTab_1_i�ļ����޸ĳ�NameTab_part_i
////		if(i<12){//ǰ12��
////			if(part<10){
////				localFilePath.replace(_documentPathLength+_namePathLength*3+5, 1, ss.str() );
////			}else if(part<100){
////				localFilePath.replace(_documentPathLength+_namePathLength*3+6, 1, ss.str() );
////			}else{
////				localFilePath.replace(_documentPathLength+_namePathLength+7*3, 1, ss.str() );
////			}
////		}else{//��12�����ļ�·����ʽ��ͬ
////			if(part<10){
////				localFilePath.replace(_documentPathLength+_namePathLength*3+18, 1, ss.str() );
////			}else if(part<100){
////				localFilePath.replace(_documentPathLength+_namePathLength*3+19, 1, ss.str() );
////			}else{
////				localFilePath.replace(_documentPathLength+_namePathLength*3+20, 1, ss.str() );
////			}
////		}
////		strcat(vecStrRadarDataLocalPath[i],localFilePath.c_str());
////		if(i==0){//���cor��local��online·��
////			localCorFilePath = localFilePath.substr(0,localFilePath.length() - 6);
////			localCorFilePath+="gpsForUpload.cor";
////		}
////	}
////
////	//ftp��·��	
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
////	//��local·����online·�������ϴ�
////	//�ϴ�cor����
////	if(!(fopen(localCorFilePath.c_str(),"rb")==NULL)){
////		int failCount = 0;
////		while(!(FtpPutFileA(hftp,localCorFilePath.c_str(),onlineCorFilePath.c_str(), FTP_TRANSFER_TYPE_BINARY, 0))){
////			failCount=failCount+1;
////			Sleep(1000);
////			if(failCount>10){
////				stringstream ssPart;
////				ssPart<<part;
////				string strTemp="�ϴ���"+ssPart.str()+"part"+"corʧ�ܡ�Ŀ��·��:";
////				strTemp+=onlineCorFilePath.c_str();
////				strTemp+="\n";
////				fprintf(m_fpLog,strTemp.c_str() );
////				//AfxMessageBox(StringToCString(strTemp)); 
////				break;
////			}
////		}
////	}else{
////		string strTemp="��ȡ";
////		strTemp+=localCorFilePath.c_str();
////		strTemp+="ʧ��";
////		strTemp+="\n";
////		fprintf(m_fpLog,strTemp.c_str() );
////		//AfxMessageBox(StringToCString(strTemp)); 
////	}
////	//�ϴ�rd3����
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
////					string strTemp="�ϴ���"+ssPart.str()+"part"+"��"+ssIndex.str()+"ͨ��rd3ʧ�ܡ�Ŀ��·��:";
////					strTemp+=vecStrRadarDataOnlinePath[i];
////					strTemp+="\n";
////					fprintf(m_fpLog,strTemp.c_str() );
////					//AfxMessageBox(StringToCString(strTemp)); 
////					break;
////				}
////			}
////		}else{
////			string strTemp="��ȡ";
////			strTemp+=vecStrRadarDataLocalPath[i];
////			strTemp+="ʧ��";
////			strTemp+="\n";
////			fprintf(m_fpLog,strTemp.c_str() );
////			//AfxMessageBox(StringToCString(strTemp)); 
////		}
////		//FtpPutFileA(hftp,vecStrRadarDataLocalPath[i],vecStrRadarDataOnlinePath[i], FTP_TRANSFER_TYPE_BINARY, 0);
////		//FtpPutFileA(hftp,vecStrRadarDataLocalPath[i],vecStrRadarDataOnlinePath[i+12], FTP_TRANSFER_TYPE_BINARY, 0);
////	}
////	//�Ͽ�ftp���
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
////	//����cor��rd3�ı���·��
////	//�����ϴ�·��	
////	std::string localCorFilePath;
////	char strLocalPath[m_nChannelCount][300];
////	for(int i=0;i<m_nChannelCount;i++){
////		strLocalPath[i][0]='\0';
////	}
////	for(int i=0; i<m_nChannelCount; i++){
////		//==============================��part�õ���ǰ���豾��·��������·��==============================//
////		//ǰ12��.\radarData\NameTab\NameTab_1\NameTab_1_i.rd3��.\radarData\NameTab\NameTab_1\moreChannels\NameTab_1_i.rd3���
////		//ǰ12��.\radarData\ProjectName\roadName\equipmentName\Tab\NameTab_1\NameTab_1_i.rd3��.\radarData\ProjectName\roadName\equipmentName\Tab\NameTab_1\moreChannels\NameTab_1_i.rd3
////		//ǰ12��.\radarData\NameTab\NameTab_part\NameTab_part_i.rd3��.\radarData\NameTab\NameTab_part\moreChannels\NameTab_part_i.rd3
////		//ǰ12��localPath\NameTab\NameTab_1\NameTab_1_i.rd3��localPath\NameTab\NameTab_1\moreChannels\NameTab_1_i.rd3���
////		//ǰ12��localPath\NameTab\NameTab_part\NameTab_part_i.rd3��localPath\NameTab\NameTab_part\moreChannels\NameTab_part_i.rd3
////		
////		std::stringstream ss;
////		ss << part;
////
////		std::string localFilePath;
////		localFilePath = _strLocalRadarDataPath[i];
////		//��NameTab_1�ļ����޸ĳ�NameTab_part
////		localFilePath.replace(_documentPathLength+_namePathLength*2+2, 1, ss.str() );//string.replace(a,b,c)��aλ��ʼ��b���ַ��滻��c
////		//��NameTab_1_i�ļ����޸ĳ�NameTab_part_i
////		if(i<12){//ǰ12��
////			if(part<10){
////				localFilePath.replace(_documentPathLength+_namePathLength*3+5, 1, ss.str() );
////			}else if(part<100){
////				localFilePath.replace(_documentPathLength+_namePathLength*3+6, 1, ss.str() );
////			}else{
////				localFilePath.replace(_documentPathLength+_namePathLength+7*3, 1, ss.str() );
////			}
////		}else{//��12�����ļ�·����ʽ��ͬ
////			if(part<10){
////				localFilePath.replace(_documentPathLength+_namePathLength*3+18, 1, ss.str() );
////			}else if(part<100){
////				localFilePath.replace(_documentPathLength+_namePathLength*3+19, 1, ss.str() );
////			}else{
////				localFilePath.replace(_documentPathLength+_namePathLength*3+20, 1, ss.str() );
////			}
////		}
////		strcat(strLocalPath[i],localFilePath.c_str());
////		if(i==0){//���cor��local��online·��
////			localCorFilePath = localFilePath.substr(0,localFilePath.length() - 6);
////			localCorFilePath+="gpsForUpload.cor";
////		}
////	}
////
////	ConfigureSet *cfg = RadarManager::Instance()->getConfigureSet();
////	//��ȡsocket����Ϣ ip��ַ �˿� �û� ���� �Լ� ��Ŀ���� �豸��� opr
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
////		//��ȡ�״�����
////		fp = fopen(localCorFilePath.c_str(),"r");
////		fread(radarDataBuffer, 1, radarDataSize, fp);
////		fclose(fp);
////		//json
////		char *content = new char[strlen(jsonFormat)+strlen(projectName.c_str())+strlen(equipmentName.c_str())+strlen(opr.c_str())+strlen(intToString(i).c_str())+strlen(intToString(part).c_str())+radarDataSize];
////		sprintf(content, jsonFormat, projectName, equipmentName, opr, m_strTimeTab, i, part, radarDataBuffer);//ƴ��json
////		//post_form_data("xxx", "xxx", data, 1);//����ƴ��indexʹ��1
////		//len = strlen(data);
////		//post_form_data("xxx", "xxx", data + len, 0);//�������indexʹ��0
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
////			//���ݻظ��������в���
////		}else{
////			//û���յ��ظ�����
////		}
////		free(send_buf);
////		free(content);
////		free(radarDataBuffer);
////	}
////
////	//�����״�����
////	for(int i=0; i<6;i++){
////		//��ȡradarData��С
////		FILE *fp = fopen(strLocalPath[i],"rb");
////		fseek(fp,0,SEEK_END);
////		int radarDataSize = ftell(fp);
////		fclose(fp);
////		char *radarDataBuffer = new char[radarDataSize];
////		//��ȡ�״�����
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
////			//���ݻظ��������в���
////		}else{
////			//û���յ��ظ�����
////		}
////
////		free(send_buf);
////		//free(content);
////		free(radarDataBuffer);
////	}
////	closesocket(sta_socket);
////	*/
////}


//��ĳpart�������ϴ���ftp��
void MeasureProject::uploadDataForPart(int part){
	
	ConfigureSet *cfg = RadarManager::Instance()->getConfigureSet();
	//��ȡftp����Ϣ ip��ַ �˿� �û� ����
	string ftpServerIp = cfg->get("ftp", "serverIP");
	int port = atoi( cfg->get("ftp", "port").c_str());
	string account = cfg->get("ftp", "account");
	string password = cfg->get("ftp", "password");

	//��ȡftp���Ӿ��
	HINTERNET hint;
	HINTERNET hftp;
	hint = InternetOpen(0, INTERNET_OPEN_TYPE_PRECONFIG, 0, 0, 0);
	if (hint==NULL)  {   
		AfxMessageBox(L"û����");
		return;  
	} 
	hftp = InternetConnectA(hint, ftpServerIp.c_str(), port, account.c_str(), password.c_str(), INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
	if (hftp==NULL) {
		AfxMessageBox(L"����ftpʧ�ܡ�");
		return;
	}

	//����txt��cor��rad��rd3�ı���������·��
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

	//ftp��·��	
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
	
	//ǰ�õ�thread->addData����
	/*
	std::vector<bool> vecTemp;
	for(int i=0;i<15;i++){//һ��15�� txt cor csv rad*6 rd3*6
		vecTemp.push_back(true);
	}
	m_vecVecUploadSuccessFlag.push_back(vecTemp);
	*/

	FILE *fp;
	//��local·����online·�������ϴ�
	//�ϴ�txt����
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
				string strTemp="�ϴ���"+ssPart.str()+"part"+"txtʧ�ܡ�Ŀ��·��:";
				strTemp+=onlineTxtFilePath.c_str();
				strTemp+="\n";
				fprintf(m_fpLog,strTemp.c_str() );
				//AfxMessageBox(StringToCString(strTemp)); 
				((SaveOnlineForRepairmentThread*)_lpThreadSaveOnlineForRepairment)->addPart(localTxtFilePath,onlineTxtFilePath,part);
				m_vecVecUploadSuccessFlag[part-1][0]=false;
				break;
			}
		}
		if(m_vecVecUploadSuccessFlag[part-1][0]==true){
			m_vecVecProcessFinishedFlag[part-1][0]=true;
		}
		//m_vecVecUploadSuccessFlag[part-1][0]=true;//����Ū���ϴ��ɹ����ж�
	}else{
		string strTemp="��ȡ";
		strTemp+=localTxtFilePath.c_str();
		strTemp+="ʧ��";
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

	//�ϴ�cor����
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
				string strTemp="�ϴ���"+ssPart.str()+"part"+"corʧ�ܡ�Ŀ��·��:";
				strTemp+=onlineCorFilePath.c_str();
				strTemp+="\n";
				fprintf(m_fpLog,strTemp.c_str() );
				//AfxMessageBox(StringToCString(strTemp)); 
				((SaveOnlineForRepairmentThread*)_lpThreadSaveOnlineForRepairment)->addPart(localCorFilePath,onlineCorFilePath,part);
				m_vecVecUploadSuccessFlag[part-1][1]=false;
				break;
			}
		}
		if(m_vecVecUploadSuccessFlag[part-1][1]==true){
			m_vecVecProcessFinishedFlag[part-1][1]=true;
		}
		//m_vecVecUploadSuccessFlag[part-1][1]=true;
	}else{
		string strTemp="��ȡ";
		strTemp+=localCorFilePath.c_str();
		strTemp+="ʧ��";
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

	//�ϴ�csv����
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
				string strTemp="�ϴ���"+ssPart.str()+"part"+"csvʧ�ܡ�Ŀ��·��:";
				strTemp+=onlineCsvFilePath.c_str();
				strTemp+="\n";
				fprintf(m_fpLog,strTemp.c_str() );
				//AfxMessageBox(StringToCString(strTemp)); 
				((SaveOnlineForRepairmentThread*)_lpThreadSaveOnlineForRepairment)->addPart(localCsvFilePath,onlineCsvFilePath,part);
				m_vecVecUploadSuccessFlag[part-1][2]=false;
				break;
			}
		}
		if(m_vecVecUploadSuccessFlag[part-1][2]==true){
			m_vecVecProcessFinishedFlag[part-1][2]=true;
		}
	}else{
		string strTemp="��ȡ";
		strTemp+=localCsvFilePath.c_str();
		strTemp+="ʧ��";
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


	//�ϴ�rad����
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
					string strTemp="�ϴ���"+ssPart.str()+"part"+"��"+ssIndex.str()+"ͨ��radʧ�ܡ�Ŀ��·��:";
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
			if(m_vecVecUploadSuccessFlag[part-1][3+i]==true){
				m_vecVecProcessFinishedFlag[part-1][3+i]=true;
			}
		}else{
			string strTemp="��ȡ";
			strTemp+=vecStrRadarDataHeaderLocalPath[i];
			strTemp+="ʧ��";
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
	//�ϴ�rd3����
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
					string strTemp="�ϴ���"+ssPart.str()+"part"+"��"+ssIndex.str()+"ͨ��rd3ʧ�ܡ�Ŀ��·��:";
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
			if(m_vecVecUploadSuccessFlag[part-1][9+i]==true){
				m_vecVecProcessFinishedFlag[part-1][9+i]=true;
			}
		}else{
			string strTemp="��ȡ";
			strTemp+=vecStrRadarDataLocalPath[i];
			strTemp+="ʧ��";
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

	//�Ͽ�ftp���
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

//���ݱ���·�����ϴ�·�������ϴ�ĳ�����ݣ�������part�����¸ò��ֵ��ϴ�������
void MeasureProject::uploadDataForPartForRepairment(std::string strLocalPath, std::string strOnlinePath, int part){
	ConfigureSet *cfg = RadarManager::Instance()->getConfigureSet();
	//��ȡftp����Ϣ ip��ַ �˿� �û� ����
	string ftpServerIp = cfg->get("ftp", "serverIP");
	int port = atoi( cfg->get("ftp", "port").c_str());
	string account = cfg->get("ftp", "account");
	string password = cfg->get("ftp", "password");

	//��ȡftp���Ӿ��
	HINTERNET hint;
	HINTERNET hftp;
	hint = InternetOpen(0, INTERNET_OPEN_TYPE_PRECONFIG, 0, 0, 0);
	int hintFailCount=0;
	while(hint==NULL&&hintFailCount<100){
		Sleep(1000);
		hintFailCount=hintFailCount+1;
		hint = InternetOpen(0, INTERNET_OPEN_TYPE_PRECONFIG, 0, 0, 0);
	}
	if (hint==NULL)  {   
		AfxMessageBox(L"û����");
		return;  
	}
	hftp = InternetConnectA(hint, ftpServerIp.c_str(), port, account.c_str(), password.c_str(), INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
	int hftpFailCount=0;
	while(hftp==NULL&&hftpFailCount<100){
		Sleep(1000);
		hftpFailCount=hftpFailCount+1;
		hftp = InternetConnectA(hint, ftpServerIp.c_str(), port, account.c_str(), password.c_str(), INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
	}
	if (hftp==NULL) {
		AfxMessageBox(L"����ftpʧ�ܡ�");
		return;
	}
	

	bool uploadSuccessFlag=true;
	FILE *fp;
	if((fp=fopen(strLocalPath.c_str(),"rb"))!=NULL){
		fclose(fp);
		fp=NULL;
		int failCount = 0;
		if(strLocalPath.substr(strLocalPath.length()-3)=="rd3"){//rd3��2���Ƶ�
			while(!(FtpPutFileA(hftp,strLocalPath.c_str(),strOnlinePath.c_str(), FTP_TRANSFER_TYPE_BINARY, 0))){
				failCount=failCount+1;
				Sleep(1000);
				if(failCount>100){
					string strTemp="�ϴ�ʧ�ܡ�Ŀ��·��:";
					strTemp+=strOnlinePath.c_str();
					strTemp+="\n";
					fprintf(m_fpLogForRepairment,strTemp.c_str() );
					uploadSuccessFlag=false;
					//AfxMessageBox(StringToCString(strTemp)); 
					break;
				}
			}
		}else{//����������
			while(!(FtpPutFileA(hftp,strLocalPath.c_str(),strOnlinePath.c_str(), FTP_TRANSFER_TYPE_ASCII, 0))){
				failCount=failCount+1;
				Sleep(1000);
				if(failCount>100){
					string strTemp="�ϴ�ʧ�ܡ�Ŀ��·��:";
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
		string strTemp="��ȡ";
		strTemp+=strLocalPath.c_str();
		strTemp+="ʧ��";
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

		/*bool postFlag=true;
		for(int i=0;i<15;i++){
			if(m_vecVecUploadSuccessFlag[part-1][i]==false){
				postFlag=false;
				break;
			}
		}
		if(postFlag==true){
			m_vecbUploadConditionForEachPart[part-1]=true;
			addDataToPostFinishedMessageThread(part);
		}*/
		if(isDataInPartUploaded(part)){
			addDataToPostFinishedMessageThread(part);
		}
	}
	
	//��������������
	int processIndex=0;
	string fileType=strLocalPath.substr(strLocalPath.length()-3);
	if(fileType=="txt"){
		processIndex=0;
	}else if(fileType=="cor"){
		processIndex=1;
	}else if(fileType=="csv"){
		processIndex=2;
	}else if(fileType=="rad"){
		processIndex=2+atoi(strLocalPath.substr(strLocalPath.length()-5,1).c_str());
	}else if(fileType=="rd3"){
		processIndex=8+atoi(strLocalPath.substr(strLocalPath.length()-5,1).c_str());
	}
	m_vecVecProcessFinishedFlag[part-1][processIndex]=true;
}

void MeasureProject::uploadFileToFtp(std::string strLocalPath, std::string strOnlinePath, int part){
	ConfigureSet *cfg = RadarManager::Instance()->getConfigureSet();
	//��ȡftp����Ϣ ip��ַ �˿� �û� ����
	string ftpServerIp = cfg->get("ftp", "serverIP");
	int port = atoi( cfg->get("ftp", "port").c_str());
	string account = cfg->get("ftp", "account");
	string password = cfg->get("ftp", "password");

	//��ȡftp���Ӿ��
	/*HINTERNET hint;
	HINTERNET hftp;
	hint = InternetOpen(0, INTERNET_OPEN_TYPE_PRECONFIG, 0, 0, 0);
	if (hint==NULL)  {   
		AfxMessageBox(L"û����");
		return;  
	} 
	hftp = InternetConnectA(hint, ftpServerIp.c_str(), port, account.c_str(), password.c_str(), INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
	if (hftp==NULL) {
		AfxMessageBox(L"����ftpʧ�ܡ�");
		return;
	}*/
	HINTERNET hint;
	HINTERNET hftp;
	hint = InternetOpen(0, INTERNET_OPEN_TYPE_PRECONFIG, 0, 0, 0);
	int hintFailCount=0;
	while(hint==NULL&&hintFailCount<100){
		Sleep(1000);
		hintFailCount=hintFailCount+1;
		hint = InternetOpen(0, INTERNET_OPEN_TYPE_PRECONFIG, 0, 0, 0);
	}
	if (hint==NULL)  {   
		AfxMessageBox(L"û����");
		return;  
	}
	hftp = InternetConnectA(hint, ftpServerIp.c_str(), port, account.c_str(), password.c_str(), INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
	int hftpFailCount=0;
	while(hftp==NULL&&hftpFailCount<100){
		Sleep(1000);
		hftpFailCount=hftpFailCount+1;
		hftp = InternetConnectA(hint, ftpServerIp.c_str(), port, account.c_str(), password.c_str(), INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
	}
	if (hftp==NULL) {
		AfxMessageBox(L"����ftpʧ�ܡ�");
		return;
	}


	bool uploadSuccessFlag=true;
	FILE *fp;
	if((fp=fopen(strLocalPath.c_str(),"rb"))!=NULL){
		fclose(fp);
		fp=NULL;
		int failCount = 0;
		if(strLocalPath.substr(strLocalPath.length()-3)=="rd3"){//rd3��2���Ƶ�
			while(!(FtpPutFileA(hftp,strLocalPath.c_str(),strOnlinePath.c_str(), FTP_TRANSFER_TYPE_BINARY, 0))){
				failCount=failCount+1;
				Sleep(1000);
				if(failCount>100){
					string strTemp="�ϴ�ʧ�ܡ�Ŀ��·��:";
					strTemp+=strOnlinePath.c_str();
					strTemp+="\n";
					fprintf(m_fpLogForRepairment,strTemp.c_str() );
					uploadSuccessFlag=false;
					//AfxMessageBox(StringToCString(strTemp)); 
					break;
				}
			}
		}else{//������2����
			while(!(FtpPutFileA(hftp,strLocalPath.c_str(),strOnlinePath.c_str(), FTP_TRANSFER_TYPE_ASCII, 0))){
				failCount=failCount+1;
				Sleep(1000);
				if(failCount>100){
					string strTemp="�ϴ�ʧ�ܡ�Ŀ��·��:";
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
		string strTemp="��ȡ";
		strTemp+=strLocalPath.c_str();
		strTemp+="ʧ��";
		strTemp+="\n";
		fprintf(m_fpLogForRepairment,strTemp.c_str() );
		uploadSuccessFlag=false;
		//AfxMessageBox(StringToCString(strTemp)); 
	}

	if(fp!=NULL){
		fclose(fp);
		fp=NULL;
	}
	
	//�㻹ûŪ�Ǹ��ϴ�����������֤!!!!!!//���ϸ�������
	if(uploadSuccessFlag){
		bool postFlag=true;
		for(int i=0;i<15;i++){
			if(m_vecVecUploadSuccessFlag[part-1][i]==false){
				postFlag=false;
				break;
			}
		}
		if(postFlag==true){
			m_vecbUploadConditionForEachPart[part-1]=true;
			addDataToPostFinishedMessageThread(part);
		}
	}
}


void MeasureProject::postEndMessage(int part){
	/*bool postFlag=true;
	for(int i=0;i<15;i++){
		if(m_vecVecUploadSuccessFlag[part-1][i]==false){
			postFlag=false;
			break;
		}
	}
	if(postFlag==false){
		return;
	}*/
	//m_nFinishedPart = part;

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

	//д�뷢������
	//time_t mytime = time(NULL);
	//string strTimeStamp = intToString(mytime-10);
	int mytime = getOnlineTime();
	string strTimeStamp = intToString(mytime-1);
	char* completeJsonBuff = new char[1024];
	char* completeSendBuff = new char[1024];
	//int nCompleteJsonBuffLen=sprintf(completeJsonBuff, COMPLETE_JSON_FORMAT, m_strOprTab.c_str(), m_strTimeTab.c_str(),  m_strTaskCode.c_str(), m_strEquipmentName.c_str(), strTimeStamp.c_str());
	int nCompleteJsonBuffLen=sprintf(completeJsonBuff, COMPLETE_JSON_FORMAT, m_strOprTab.c_str(), m_vecStrTimeTab[part-1].c_str(),  m_strTaskCode.c_str(), m_strEquipmentName.c_str(), strTimeStamp.c_str());
	sprintf(completeSendBuff, POST_FORMAT, "/externalservice/completeUploadFile.ht", m_strPostServerIp.c_str(), m_nPostPort, intToString(nCompleteJsonBuffLen).c_str(), completeJsonBuff);
	//����
	int sendLen=-1;
	time_t sendTime = time(NULL);
	string strSendTimeStamp = intToString(sendTime);
	string strSendRecording="���ָ���ʱ��Ϊ:"+strSendTimeStamp+"\n";
	fprintf(m_fpLog,strSendRecording.c_str() );
	while(sendLen<0){
		sendLen=send(sta_socket, completeSendBuff, (unsigned int)strlen(completeSendBuff)+1, 0);
	}
	//���շ���
	char* completeRecieveBuff = new char[1024];
	int nCompleteRecieveBuffLen = recv(sta_socket, completeRecieveBuff, 1024, 0);
	if(nCompleteRecieveBuffLen>0){
		//���ݻظ��������в���
		completeRecieveBuff[nCompleteRecieveBuffLen]='\0';
		//string strRecieve=string(completeRecieveBuff);
		string strRecieveRecording="�ϴ�"+ m_vecStrTimeTab[part-1]+"���ָ��ͳɹ���\n�ظ�����Ϊ��";
		string strTemp2=completeRecieveBuff;
		strRecieveRecording=strRecieveRecording+strTemp2;
		time_t recieveTime = time(NULL);
		string strRecieveTimeStamp = intToString(recieveTime);
		string strTemp3="����ʱ��Ϊ:"+strRecieveTimeStamp+"\n";
		strRecieveRecording=strRecieveRecording+strTemp3;
		fprintf(m_fpLog,strRecieveRecording.c_str() );
	}else{
		//û���յ��ظ�����
		string strTemp="�ϴ�"+ m_vecStrTimeTab[part-1]+"���ָ��ͳɹ���\n�ظ�����Ϊ�ա�\n";
		fprintf(m_fpLog,strTemp.c_str() );
	}	
	delete []completeRecieveBuff;
	delete []completeSendBuff;
	delete []completeJsonBuff;
	closesocket(sta_socket);

}

////void MeasureProject::writeEndFile(){
////	USES_CONVERSION;
////	//����10�й��ɱ����ļ�·�� ��ʽ��path\Nametab\NameTabArrayset.txt
////	std::string strPathTxt = W2A( m_strSavePath );
////	strPathTxt += _projectName;
////	strPathTxt += _projectTab;
////	strPathTxt += "\\";
////	if (0 != access(strPathTxt.c_str(), 0)){
////		mkdir(strPathTxt.c_str());   // ���� 0 ��ʾ�����ɹ���-1 ��ʾʧ��
////	}
////	strPathTxt += _projectName;
////	strPathTxt += _projectTab;
////	strPathTxt+="complete.txt";
////	
////	//�����ǰ�����������ļ���
////	ofstream ofile; 
////	ofile.open(strPathTxt.c_str()); 
////	ofile<<"complete"<<endl;
////	ofile.close();//�ر��ļ�
////}

////void MeasureProject::uploadEndFile(){
////	USES_CONVERSION;
////	//����10�й��ɱ����ļ�·�� ��ʽ��path\Nametab\complete.txt
////	std::string strPathTxt = W2A( m_strSavePath );
////	strPathTxt += _projectName;
////	strPathTxt += _projectTab;
////	strPathTxt += "\\";
////	if (0 != access(strPathTxt.c_str(), 0)){
////		mkdir(strPathTxt.c_str());   // ���� 0 ��ʾ�����ɹ���-1 ��ʾʧ��
////	}
////	strPathTxt += _projectName;
////	strPathTxt += _projectTab;
////	strPathTxt+="complete.txt";
////
////	//��ȡftp���� ip �˿� �û� ����
////	ConfigureSet *cfg = RadarManager::Instance()->getConfigureSet();
////	string ftpServerIp = cfg->get("ftp", "serverIP");
////	int port = atoi( cfg->get("ftp", "port").c_str());
////	string account = cfg->get("ftp", "account");
////	string password = cfg->get("ftp", "password");
////
////	//����ftp����������
////	HINTERNET hint;
////	HINTERNET hftp;
////	hint = InternetOpen(0, INTERNET_OPEN_TYPE_PRECONFIG, 0, 0, 0);
////	if (hint==NULL)  {   
////		AfxMessageBox(L"û����");
////		return;  
////	} 
////	hftp = InternetConnectA(hint, ftpServerIp.c_str(), port, account.c_str(), password.c_str(), INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
////	if (hftp==NULL) {
////		AfxMessageBox(L"����ftpʧ��,���ڳ�������");
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
////	//������complete.txt�ϴ���ftp��
////	if(!(fopen(strPathTxt.c_str(),"rb")==NULL)){
////		int failCount=0;
////		while(!FtpPutFileA(hftp,strPathTxt.c_str(),strPathOrigin.c_str(), FTP_TRANSFER_TYPE_ASCII, 0)){
////			failCount=failCount+1;
////			Sleep(1000);
////			if(failCount>10){
////				stringstream ssPart;
////				ssPart<<part;
////				string strTemp="�ϴ���"+ssPart.str()+"part complete.txtʧ�ܡ�Ŀ��·��:";
////				strTemp+=strPathOrigin.c_str();
////				AfxMessageBox(StringToCString(strTemp)); 
////				break;
////			}
////		}
////	}else{
////		string strTemp="��ȡ";
////		strTemp+=strPathTxt.c_str();
////		strTemp+="ʧ��";
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
	//time_t mytime = time(NULL);
	//string strTimeStamp = intToString(mytime-10);//-1��ֹtimeout
	int mytime = getOnlineTime();
	string strTimeStamp = intToString(mytime-1);//-1��ֹtimeout
	osg::Vec3d pos = RadarManager::Instance()->getGpsReader()->getCurPos();
	//std::vector< std::string > vecStrGPGGA;
	//RadarManager::Instance()->getGpsReader()->getCurrentGpsImformation(pos,vecStrGPGGA);
	//RadarManager::Instance()->getGpsReader()->getCurrentGpsImformation(pos,NULL);

	char* initJsonBuff = new char[1024];
	char* initSendBuff = new char[1024];
	//int nInitJsonBuffLen=sprintf(initJsonBuff, INIT_JSON_FORMAT, m_strTaskCode.c_str(), m_strEquipmentName.c_str(), strTimeStamp.c_str());
	int nInitJsonBuffLen=sprintf(initJsonBuff, INIT_JSON_FORMAT, m_strTaskCode.c_str(), m_strEquipmentName.c_str(), strTimeStamp.c_str(), pos.x(), pos.y());
	sprintf(initSendBuff, POST_FORMAT, "/externalservice/initOprCode.ht", m_strPostServerIp.c_str(), m_nPostPort, intToString(nInitJsonBuffLen).c_str(), initJsonBuff);
	//����
	int sendLen=-1;
	while(sendLen<0){
		sendLen=send(sta_socket, initSendBuff, (unsigned int)strlen(initSendBuff)+1, 0);
	}
	//���շ���
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
		
		//���ݻظ��������в���
	}else{
		//û���յ��ظ�����
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

//���ݸո��µ�timeTab���ºϳ��µ�ftp
void MeasureProject::changeFtpFolderPath(){
	ConfigureSet *cfg = RadarManager::Instance()->getConfigureSet();
	//��ȡftp����Ϣ ip��ַ �˿� �û� ����
	string ftpServerIp = cfg->get("ftp", "serverIP");
	int port = atoi( cfg->get("ftp", "port").c_str());
	string account = cfg->get("ftp", "account");
	string password = cfg->get("ftp", "password");
	//��ȡftp���Ӿ��
	/*HINTERNET hint;
	HINTERNET hftp;
	hint = InternetOpen(0, INTERNET_OPEN_TYPE_PRECONFIG, 0, 0, 0);
	if (hint==NULL)  {   
		AfxMessageBox(L"û����");
		return;  
	}
	hftp = InternetConnectA(hint, ftpServerIp.c_str(), port, account.c_str(), password.c_str(), INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
	if (hftp==NULL) {
		AfxMessageBox(L"����ftpʧ�ܡ�");
		return;
	}*/
	HINTERNET hint;
	HINTERNET hftp;
	hint = InternetOpen(0, INTERNET_OPEN_TYPE_PRECONFIG, 0, 0, 0);
	int hintFailCount=0;
	while(hint==NULL&&hintFailCount<100){
		Sleep(1000);
		hintFailCount=hintFailCount+1;
		hint = InternetOpen(0, INTERNET_OPEN_TYPE_PRECONFIG, 0, 0, 0);
	}
	if (hint==NULL)  {   
		AfxMessageBox(L"û����");
		return;  
	}
	hftp = InternetConnectA(hint, ftpServerIp.c_str(), port, account.c_str(), password.c_str(), INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
	int hftpFailCount=0;
	while(hftp==NULL&&hftpFailCount<100){
		Sleep(1000);
		hftpFailCount=hftpFailCount+1;
		hftp = InternetConnectA(hint, ftpServerIp.c_str(), port, account.c_str(), password.c_str(), INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0);
	}
	if (hftp==NULL) {
		AfxMessageBox(L"����ftpʧ�ܡ�");
		return;
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

//��postFinishedMessage�̼߳Ӷ���
void MeasureProject::addDataToPostFinishedMessageThread(int part){
	((PostFinishedMessageThread*)_lpThreadPostFinishedMessage)->addPart(part);
}
	
bool MeasureProject::isDataInPartUploaded(int part){
	bool finishedFlag=true;
	for(int i=0;i<15;i++){
		if(m_vecVecUploadSuccessFlag[part-1][i]==false){
			finishedFlag=false;
			break;
		}
	}
	if(finishedFlag==true){
		m_vecbUploadConditionForEachPart[part-1]=true;
	}
	return finishedFlag;
}

//���ô���ģʽ
void MeasureProject::SetTERThread( CString strPath ){
	m_strTERSavePath = strPath;

	m_bTERIsBegin = true;

	CString cstrCreateTime;
	USES_CONVERSION;

	std::string strTERPath = W2A( strPath );
	strTERPath += _projectName;
	strTERPath += _projectTab;
	strTERPath += ".tmp";

	if (m_ThreadTERSaveFile){
		delete m_ThreadTERSaveFile;
		m_ThreadTERSaveFile = NULL;
	}
	m_ThreadTERSaveFile = new SaveTERDataThread;
	if (m_TERDataWriter){
		delete m_TERDataWriter;
		m_TERDataWriter = NULL;
	}
	m_TERDataWriter = new TERDataWriter;
	//m_SGYWriter[i]->openSGY(vecStrSGYPath[i].c_str(),lpProjectRow->_paramXML ,0);
	m_TERDataWriter->openUep(strTERPath.c_str());
	//m_TERDataWriter->writeUepHead();
	((SaveTERDataThread*)m_ThreadTERSaveFile)->_TERDataWriter = m_TERDataWriter;
	m_ThreadTERSaveFile->start();
}

void MeasureProject::addTERData( TERData *data){
	/*osg::Vec3d pos = RadarManager::Instance()->getGpsReader()->getCurPos();//��ȡ��ǰgps����
	float curLen = RadarManager::Instance()->getGpsReader()->getCurLen() + _lastLen;//��ȡ��ǰ������gps���볤��

	//��ȫ�״�����
	data->setPosition( pos );
	data->setTime( time(NULL) );
	data->setLen( curLen );

	((SaveTERDataThread*)m_ThreadTERSaveFile)->addData(data);*/

	osg::ref_ptr<SaveTERFileDataCmd> lpCmd = new SaveTERFileDataCmd;//osg����߳�
	TERData *tmpRD = data;
	lpCmd->_lpTERData = tmpRD;

	osg::Vec3d pos = RadarManager::Instance()->getGpsReader()->getCurPos();//��ȡ��ǰgps����
	float curLen = RadarManager::Instance()->getGpsReader()->getCurLen() + _lastLen;//��ȡ��ǰ������gps���볤��

	//��ȫ�״�����
	tmpRD->setPosition( pos );
	tmpRD->setTime( time(NULL) );
	tmpRD->setLen( curLen );

	((SaveTERDataThread*)m_ThreadTERSaveFile)->addData(lpCmd);
}


