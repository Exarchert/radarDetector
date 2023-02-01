#include "StdAfx.h"
#include "TERDataWriter.h"
//#include "UepHeader.h"
//#include "UepBlock.h"
#include "C_SamplingParam.h"

#include <RadarCurve/RadarManager.h>
#include "VideoTemplateTab.h"
//C++
#include <sstream>
#include <io.h>
#include <fcntl.h>
#include <iomanip>
#include <string> 
#include <vector>
#include <iostream>
#include <direct.h>//用来创建文件夹
#include <sys/stat.h> // struct stat 用来看文件大小
#include <cstdio>//用来删文件 remove(char *fileName)
#include <math.h>//用来获取M_PI
using namespace std;

struct UEP_ReelBlock // *.uep
{
	DWORD workTag;//作业标识号（任务识别码）
	DWORD lineNum;//测线号

	DWORD triggerSum;//激发次数
	short traceSum;//每个记录的道数
	short frameSum;//该卷每道的采样点数
    
	float sampleInterval;//该卷采样间隔
	float samplingTimelapse;//原始记录采样延时
	
	short sendtime;// 发射时间

	////////////////////////////////////////////////////////////
	//						新增扩展  采集参数 于 打标
	////////////////////////////////////////////////////////////
	int  m_has_sample_param;		//是否有采集参数
	int	 m_sample_param_pos;		//采集参数距离文件开头的位置
	int	 m_sample_param_size;		//采集参数总大小

	int	 m_has_tags;						//是否有“中断标”
	int	 m_tags_pos;						//“中断标”距离文件开头的位置
	int	 m_tags_num;					//有多少“中断标”
	int	 m_tags_begin_val;			//数据包起始采集时标志值
	int	 m_tags_end_val;				//数据包终止采集时标志值
	int	 m_is_time_tag;					//是否为时间标
	int	 m_val_per_ch;					//每一道采样数据间隔多少标值

	int	 m_has_gps_gdjw;					//是否有国电经纬GPS
	 
	// 剩余自定义
	char untapped[326];	
};//共400字节

struct UEP_NewMastBlock 
{
	BOOL	bSwitch;		//1

	int	nCycleNum;			//2 线圈匝数
	
	float fLength;			//3 长
	float fWidth;			//4 宽

	float x1;				//5
	float y1;				//6

	float x2;				//7
	float y2;				//8

	COLORREF color;			//9


	float fStartElectro;	//10

	float z1;
	float z2;

	float xzAngle;
	float yzAngle;

	int type;
// 一共60个字节
};

struct UEP_MastBlock 
{
	// 发送线圈
	UEP_NewMastBlock UEPSendMast[5];	// 300字节
	// 接受线圈
	UEP_NewMastBlock UEPRcvMast[5];		// 300字节

	UEP_MastBlock()
	{
		// 发送线圈1
		UEPSendMast[0].bSwitch = TRUE;
		UEPSendMast[0].nCycleNum = 100;
		UEPSendMast[0].fLength = 1.0;
		UEPSendMast[0].fWidth = 1.0;
		UEPSendMast[0].x1 = -5.0;

		UEPSendMast[0].y1 = 0.0;
		UEPSendMast[0].x2 = 5.0;
		UEPSendMast[0].y2 = 0.0;
		UEPSendMast[0].color = RGB(255, 255, 0);
		UEPSendMast[0].fStartElectro = 15.0;

		UEPSendMast[0].z1 = 0.0;
		UEPSendMast[0].z2 = 0.0;
		UEPSendMast[0].xzAngle = 0.0;
		UEPSendMast[0].yzAngle = 0.0;
		UEPSendMast[0].type = 0;


		// 接受线圈1
		UEPRcvMast[0].bSwitch = TRUE;
		UEPRcvMast[0].nCycleNum = 100;
		UEPRcvMast[0].fLength = 1.0;
		UEPRcvMast[0].fWidth = 1.0;
		UEPRcvMast[0].x1 = -5.0;

		UEPRcvMast[0].y1 = 0.0;
		UEPRcvMast[0].x2 = 5.0;
		UEPRcvMast[0].y2 = 0.0;
		UEPRcvMast[0].color = RGB(255, 255, 0);
		UEPRcvMast[0].fStartElectro = 15.0;

		UEPRcvMast[0].z1 = 0.0;
		UEPRcvMast[0].z2 = 0.0;
		UEPRcvMast[0].xzAngle = 0.0;
		UEPRcvMast[0].yzAngle = 0.0;
		UEPRcvMast[0].type = 0;
	}
};

struct UEP_HeadBlock 
{
	int EmissionCurrent;// 发射电流
	int TurnoffCurrent;// 关断电流
	int TransitionCurrent[20];// 过渡电流
};

struct UEP_GpsBlock 
{
	double longitude;// 经度
	double latitude;// 纬度
	double altitude;// 高程
	double gpsDistance;// gps距离
};

TERDataWriter::TERDataWriter(void){
	//m_strTimeTab="";
	m_strPath="";
	m_lpTERDataFile = NULL;
	m_dTotalLen = 0;
	m_nDataCount = 0;
	m_bIsDataOne=true;
}

TERDataWriter::~TERDataWriter(void){
}

bool TERDataWriter::openUep( std::string const& filePath ){	
	m_strPath=filePath;
	m_lpTERDataFile = fopen( filePath.c_str(), "wb" );
	if ( !m_lpTERDataFile ){
		return false;
	}

	m_nDataCount=0;

	//hjl20210408 更新参数获取
	ConfigureSet *cfg = RadarManager::Instance()->getConfigureSet();
	m_nSampleNum = atoi( cfg->get("receive", "sampleCount").c_str());//采样点
	m_fSampleRate =  atoi( cfg->get("receive", "samplingRate").c_str());//采样率

	//int precRatio = atoi( cfg->get("radar", "precratio").c_str() );
	int precRatio = RadarManager::Instance()->getPrecRatio(atoi( cfg->get("radar", "precRatio").c_str()));//脉冲数
	float precLen = atoi ( cfg->get("radar", "preclen").c_str() );
	//int precIndex = atoi( cfg->get("radar", "precindex").c_str() );
	int iDielectric = atoi( cfg->get("radar", "dielectric").c_str() );
	m_fDielectric = RadarManager::Instance()->getDielectric( iDielectric );//介电常数
	
	m_dTimeWnd = 1.0 / m_fSampleRate * m_nSampleNum;//时窗

	//float fJihuaAccuracy = RadarManager::Instance()->GetJihuaAccuracy(precIndex);
	float fJihuaAccuracy = (float)atoi( cfg->get("radar", "precision").c_str() );
	float fp = RadarManager::Instance()->GetTrueAccuracyFromPreclenAndPrecindex(precLen,fJihuaAccuracy);
	m_dPrecValue = fp / 100.0;
	

	return true;
}

void TERDataWriter::writeUepHead(){
	//重新开文件
	fclose( m_lpTERDataFile );
	m_lpTERDataFile = 0;
	m_lpTERDataFile = fopen( (m_strPath.substr(0,m_strPath.length()-3)+"uep").c_str(), "wb" );

	/*unsigned char *temp = new unsigned char[1000];
	for(int i=0;i<1000;i++){
		temp[i]=0;
	}

	//01～04 任务识别码
	//05～08 测线号

	//09～12 电磁激发次数
	int nDataCount=m_nDataCount;
	temp[8]=*((unsigned char*)&nDataCount);
	temp[9]=*((unsigned char*)&nDataCount+1);
	temp[10]=*((unsigned char*)&nDataCount+2);
	temp[11]=*((unsigned char*)&nDataCount+3);

	//13～14 每个记录的数据道数(总道数)
	temp[12]=1;

	//15～16 该卷每道的采样点数
	short nSampleCount=(short)m_nSampleNum;
	temp[14]=*((unsigned char*)&nSampleCount);
	temp[15]=*((unsigned char*)&nSampleCount+1);

	//文件字节数
	struct stat statbuf;
	stat(m_strPath.c_str(),&statbuf);
	size_t filesize=statbuf.st_size;
	//int fileBytes=filesize;
	//int fileBytesWithoutTail=filesize-536;
	int fileBytes=filesize+1536;
	int fileBytesWithoutTail=filesize+1000;
	
	//31～34 采集参数距离文件开头的位置
	temp[32]=*((unsigned char*)&fileBytesWithoutTail);
	temp[33]=*((unsigned char*)&fileBytesWithoutTail+1);
	temp[34]=*((unsigned char*)&fileBytesWithoutTail+2);
	temp[35]=*((unsigned char*)&fileBytesWithoutTail+3);

	//35～38 采集参数数据字节总大小
	int nTail=536;
	temp[36]=*((unsigned char*)&nTail);
	temp[37]=*((unsigned char*)&nTail+1);
	temp[38]=*((unsigned char*)&nTail+2);
	temp[39]=*((unsigned char*)&nTail+3);

	//39～42 是否有“中断标”
	temp[40]=1;

	//43～46 “中断标”距离文件开头的位置
	temp[44]=*((unsigned char*)&fileBytes);
	temp[45]=*((unsigned char*)&fileBytes+1);
	temp[46]=*((unsigned char*)&fileBytes+2);
	temp[47]=*((unsigned char*)&fileBytes+3);

	temp[56]=255;
	temp[57]=255;
	temp[58]=255;
	temp[59]=255;
	temp[60]=1;
	
	temp[64]=1;

	//67～70 int 是否有北京国电经纬gps信息
	temp[66]=1;

	temp[400]=1;
	temp[404]=100;
	temp[411]=64;
	temp[415]=64;

	temp[700]=1;
	temp[704]=100;
	temp[711]=64;
	temp[715]=64;

	fwrite( (char*)temp, sizeof(char), 1000, m_lpTERDataFile);*/

	UEP_ReelBlock uepHeadBlock;
	uepHeadBlock.workTag=0;
	uepHeadBlock.lineNum=0;
	uepHeadBlock.triggerSum=m_nDataCount;
	uepHeadBlock.traceSum=1;
	uepHeadBlock.frameSum=m_nSampleNum;
	uepHeadBlock.sampleInterval=0.0;
	uepHeadBlock.samplingTimelapse=0.0;
	uepHeadBlock.sendtime=0;
	uepHeadBlock.m_has_sample_param=1;
	//文件字节数
	struct stat statbuf;
	stat(m_strPath.c_str(),&statbuf);
	size_t filesize=statbuf.st_size;
	//int fileBytesWithoutTail=1000+(m_nSampleNum*4+88)*m_nDataCount;
	//int fileBytes=filesize+1536;
	if(RadarManager::Instance()->getExportGpsPos()){
		uepHeadBlock.m_sample_param_pos=1000+(m_nSampleNum*4+88+32)*m_nDataCount;
	}else{
		uepHeadBlock.m_sample_param_pos=1000+(m_nSampleNum*4+88)*m_nDataCount;
	}
	uepHeadBlock.m_sample_param_size=540;
	uepHeadBlock.m_has_tags=1;
	if(RadarManager::Instance()->getExportGpsPos()){
		uepHeadBlock.m_tags_pos=1000+(m_nSampleNum*4+88+32)*m_nDataCount+540;
	}else{
		uepHeadBlock.m_tags_pos=1000+(m_nSampleNum*4+88)*m_nDataCount+540;
	}
	uepHeadBlock.m_tags_num=0;
	uepHeadBlock.m_tags_begin_val=0;
	uepHeadBlock.m_tags_end_val=-1;
	uepHeadBlock.m_is_time_tag=1;
	uepHeadBlock.m_val_per_ch=1;
	uepHeadBlock.m_has_gps_gdjw=RadarManager::Instance()->getExportGpsPos();
	int aaa=sizeof(uepHeadBlock);
	fwrite( (char*)&uepHeadBlock, sizeof(uepHeadBlock), 1, m_lpTERDataFile);

	
	//fwrite( (char*)&uepHeadBlock, 400, 1, m_lpTERDataFile);

	UEP_MastBlock uepMastBlock;
	uepMastBlock.UEPSendMast[0].bSwitch=1;
	uepMastBlock.UEPSendMast[0].nCycleNum=100;
	uepMastBlock.UEPSendMast[0].fLength=2.0;
	uepMastBlock.UEPSendMast[0].fWidth=2.0;
	uepMastBlock.UEPSendMast[0].x1=0.0;
	uepMastBlock.UEPSendMast[0].y1=0.0;
	uepMastBlock.UEPSendMast[0].x2=0.0;
	uepMastBlock.UEPSendMast[0].y2=0.0;
	uepMastBlock.UEPSendMast[0].color=0;
	uepMastBlock.UEPSendMast[0].fStartElectro=0.0;
	uepMastBlock.UEPSendMast[0].z1=0.0;
	uepMastBlock.UEPSendMast[0].z2=0.0;
	uepMastBlock.UEPSendMast[0].xzAngle=0.0;
	uepMastBlock.UEPSendMast[0].yzAngle=0.0;
	uepMastBlock.UEPSendMast[0].type=0;
	for(int i=1;i<5;i++){
		uepMastBlock.UEPSendMast[0].bSwitch=0;
		uepMastBlock.UEPSendMast[0].nCycleNum=0;
		uepMastBlock.UEPSendMast[0].fLength=0.0;
		uepMastBlock.UEPSendMast[0].fWidth=0.0;
		uepMastBlock.UEPSendMast[0].x1=0.0;
		uepMastBlock.UEPSendMast[0].y1=0.0;
		uepMastBlock.UEPSendMast[0].x2=0.0;
		uepMastBlock.UEPSendMast[0].y2=0.0;
		uepMastBlock.UEPSendMast[0].color=0;
		uepMastBlock.UEPSendMast[0].fStartElectro=0.0;
		uepMastBlock.UEPSendMast[0].z1=0.0;
		uepMastBlock.UEPSendMast[0].z2=0.0;
		uepMastBlock.UEPSendMast[0].xzAngle=0.0;
		uepMastBlock.UEPSendMast[0].yzAngle=0.0;
		uepMastBlock.UEPSendMast[0].type=0;
	}
	uepMastBlock.UEPSendMast[0].bSwitch=1;
	uepMastBlock.UEPRcvMast[0].nCycleNum=100;
	uepMastBlock.UEPRcvMast[0].fLength=2.0;
	uepMastBlock.UEPRcvMast[0].fWidth=2.0;
	uepMastBlock.UEPRcvMast[0].x1=0.0;
	uepMastBlock.UEPRcvMast[0].y1=0.0;
	uepMastBlock.UEPRcvMast[0].x2=0.0;
	uepMastBlock.UEPRcvMast[0].y2=0.0;
	uepMastBlock.UEPRcvMast[0].color=0;
	uepMastBlock.UEPRcvMast[0].fStartElectro=0.0;
	uepMastBlock.UEPRcvMast[0].z1=0.0;
	uepMastBlock.UEPRcvMast[0].z2=0.0;
	uepMastBlock.UEPRcvMast[0].xzAngle=0.0;
	uepMastBlock.UEPRcvMast[0].yzAngle=0.0;
	uepMastBlock.UEPRcvMast[0].type=0;
	for(int i=1;i<5;i++){
		uepMastBlock.UEPRcvMast[0].bSwitch=0;
		uepMastBlock.UEPRcvMast[0].nCycleNum=0;
		uepMastBlock.UEPRcvMast[0].fLength=0.0;
		uepMastBlock.UEPRcvMast[0].fWidth=0.0;
		uepMastBlock.UEPRcvMast[0].x1=0.0;
		uepMastBlock.UEPRcvMast[0].y1=0.0;
		uepMastBlock.UEPRcvMast[0].x2=0.0;
		uepMastBlock.UEPRcvMast[0].y2=0.0;
		uepMastBlock.UEPRcvMast[0].color=0;
		uepMastBlock.UEPRcvMast[0].fStartElectro=0.0;
		uepMastBlock.UEPRcvMast[0].z1=0.0;
		uepMastBlock.UEPRcvMast[0].z2=0.0;
		uepMastBlock.UEPRcvMast[0].xzAngle=0.0;
		uepMastBlock.UEPRcvMast[0].yzAngle=0.0;
		uepMastBlock.UEPRcvMast[0].type=0;
	}

	int bbbb=sizeof(uepMastBlock);
	fwrite( (char*)&uepMastBlock, sizeof(UEP_MastBlock), 1, m_lpTERDataFile);

	
	//fwrite( (char*)&uepMastBlock, 600, 1, m_lpTERDataFile);
}

/*extern float GetFloatFromPack( unsigned char* in_lp_num , char in_bits_flag ){
	int onepoint = 0;
	float op = 0;
	// float maxvalue = 5.0f;
	float maxvalue = 2.5f;

	switch(in_bits_flag){
		case 8:
			onepoint = ((int)in_lp_num[0]);
			onepoint = onepoint<<16;
			if ( onepoint == 0x00800000 ){
				op = -maxvalue;// * (float)onepoint * pow(2, 23) * 5 / (pow(2, 23) - 1);
			}else{
				if ( !(onepoint & 0x00800000) ){
					op = (float)onepoint * maxvalue / (pow( (float)2, (float)23) - 1);
				}else{
					onepoint = ((~onepoint)&0x00ffffff) + 0x000001;
					op = -1 * (float)onepoint * maxvalue / (pow((float)2, (float)23) - 1);
				}
			}
			break;
		case 16:
			onepoint = ((int)in_lp_num[0]) + ((int)in_lp_num[1]<<8);
			onepoint = onepoint<<8;
			if ( onepoint == 0x00800000 ){
				op = -maxvalue;// * (float)onepoint * pow(2, 23) * 5 / (pow(2, 23) - 1);
			}else{
				if ( !(onepoint & 0x00800000) ){
					op = (float)onepoint * maxvalue / (pow((float)2, (float)23) - 1);
				}else{
					onepoint = ((~onepoint)&0x00ffffff) + 0x000001;
					op = -1 * (float)onepoint * maxvalue / (pow((float)2, (float)23) - 1);
				}
			}
			break;
		case 24:
			// onepoint = ((int)in_lp_num[0]) + ((int)in_lp_num[1]<<8) + ((int)in_lp_num[2]<<16);
			onepoint = ((int)in_lp_num[2]) + ((int)in_lp_num[1]<<8) + ((int)in_lp_num[0]<<16);
			if ( onepoint == 0x00800000 ){
				op = -maxvalue;// * (float)onepoint * pow(2, 23) * 5 / (pow(2, 23) - 1);
			}else{
				if ( !(onepoint & 0x00800000) ){
					op = (float)onepoint * maxvalue / (pow((float)2, (float)23) - 1);
				}else{
					onepoint = ((~onepoint)&0x00ffffff) + 0x000001;
					op = -1 * (float)onepoint * maxvalue / (pow( (float)2, (float)23) - 1);
				}
			}
			break;
		default:
			break;
	}

	return op;
}*/

void TERDataWriter::writeUepData( TERData *lpData ){
	int len = 0;
	//unsigned short *lpBuff = ( unsigned short *)(lpData->getData( len ));
	float *lpBuff = (float*)lpData->getData( len );
	if ( !lpBuff ){
		return;
	}
	if(m_lpTERDataFile){
		/*gps体*/
		if(RadarManager::Instance()->getExportGpsPos()){
			UEP_GpsBlock uepGpsBlock;
			osg::Vec3d pos = lpData->getPosition();
			uepGpsBlock.longitude = pos.x();
			uepGpsBlock.latitude = pos.y();
			uepGpsBlock.altitude = pos.z();
			uepGpsBlock.gpsDistance = lpData->getLen();
			int a=sizeof( uepGpsBlock );
			fwrite( (char*)&uepGpsBlock, sizeof( uepGpsBlock ), 1, m_lpTERDataFile);//写入gps信息
		}
		/*电流体*/
		unsigned char *temp = new unsigned char[88];
		for(int i=0;i<88;i++){
			temp[i]='0';
		}
		fwrite( (char*)temp, sizeof(char), 88, m_lpTERDataFile);
		/*数据体*/
		fwrite( lpBuff, sizeof(float), m_nSampleNum, m_lpTERDataFile);//写入数据
		m_nDataCount=m_nDataCount+1;
	}

	/*for(int i=0;i<m_nSampleNum;i++){
		if(m_bIsDataOne){
			m_vecfDataOne.push_back(GetFloatFromPack(lpBuff+6+i*3,24));
		}else{
			m_vecfDataTwo.push_back(GetFloatFromPack(lpBuff+6+i*3,24));
		}
	}

	if(m_bIsDataOne==false){
		if(m_lpTERDataFile){
			unsigned char *temp = new unsigned char[88];
			for(int i=0;i<88;i++){
				temp[i]='0';
			}
			fwrite( (char*)temp, sizeof(char), 88, m_lpTERDataFile);

			float *fTemp = new float[m_nSampleNum];
			for(int i=0;i<m_nSampleNum;i++){
				fTemp[i]=m_vecfDataTwo[i]-m_vecfDataOne[i];
			}
			fwrite(fTemp, sizeof(float), m_nSampleNum, m_lpTERDataFile);//写入数据
			m_nDataCount=m_nDataCount+1;
			m_vecfDataTwo.clear();
			m_vecfDataOne.clear();
		}
	}

	if(m_bIsDataOne==true){
		m_bIsDataOne=false;
	}else{
		m_bIsDataOne=true;
	}*/
}

void TERDataWriter::copyUepData(){
	/*FILE *fp;
	//获取文件大小
	fp=fopen(m_strPath.c_str(),"rb");
	fseek(fp,0,SEEK_END);
	size_t fileBytes = ftell(fp);
	fclose(fp);*/

	struct stat statbuf;
	stat(m_strPath.c_str(),&statbuf);
	size_t filesize=statbuf.st_size;
	int fileBytes=filesize;
	
	FILE *fp;
	fp=fopen(m_strPath.c_str(),"rb");
	unsigned char *temp = new unsigned char[fileBytes];
	fread(temp,sizeof(unsigned char),fileBytes,fp);
	fwrite( temp, sizeof(unsigned char), fileBytes, m_lpTERDataFile);
	fclose(fp);
	
	remove(m_strPath.c_str());

}

void TERDataWriter::writeUepTail(){
	unsigned char *temp = new unsigned char[540];
	/*for(int i=0;i<540;i++){
		temp[i]=0;
	}*/
	/*temp[0]=0;
	temp[1]=1;
	temp[2]=1;
	temp[3]=1;
	temp[4]=1;
	temp[5]=1;
	temp[6]=1;
	temp[7]=1;
	temp[8]=1;
	temp[9]=1;
	temp[10]=1;
	temp[11]=1;
	temp[12]=1;
	temp[13]=1;
	temp[14]=1;
	temp[15]=1;
	temp[16]=1;
	temp[17]=1;
	temp[18]=1;
	temp[19]=1;
	temp[20]=1;
	temp[21]=1;
	temp[22]=1;
	temp[23]=1;
	temp[24]=1;
	temp[25]=1;
	temp[26]=1;
	temp[27]=1;
	temp[28]=1;
	temp[29]=1;
	temp[30]=1;
	temp[31]=1;
	temp[32]=1;
	temp[33]=1;
	temp[34]=1;
	temp[35]=1;
	temp[36]=1;
	temp[37]=1;
	temp[38]=1;
	temp[39]=1;
	temp[40]=1;
	temp[41]=1;
	temp[42]=1;
	temp[43]=1;
	temp[44]=1;
	temp[45]=1;
	temp[46]=1;
	temp[47]=1;
	temp[48]=1;
	temp[49]=1;
	temp[50]=1;
	temp[51]=1;
	temp[52]=1;
	temp[53]=1;
	temp[54]=1;
	temp[55]=1;
	temp[56]=1;
	temp[57]=1;
	temp[58]=1;
	temp[59]=1;
	temp[60]=1;
	temp[61]=1;
	temp[62]=1;
	temp[63]=1;
	temp[64]=1;
	temp[65]=1;
	temp[66]=1;
	temp[67]=1;
	temp[68]=1;
	temp[69]=1;
	temp[70]=1;
	temp[71]=1;
	temp[72]=1;
	temp[73]=0;
	temp[74]=0;
	temp[75]=0;
	temp[76]=0;
	temp[77]=0;
	temp[78]=0;
	temp[79]=0;
	temp[80]=0;
	temp[81]=0;
	temp[82]=0;
	temp[83]=0;
	temp[84]=0;
	temp[85]=0;
	temp[86]=0;
	temp[87]=0;
	temp[88]=0;
	temp[89]=0;
	temp[90]=0;
	temp[91]=0;
	temp[92]=0;
	temp[93]=0;
	temp[94]=0;
	temp[95]=0;
	temp[96]=0;
	temp[97]=0;
	temp[98]=0;
	temp[99]=0;
	temp[100]=0;
	temp[101]=0;
	temp[102]=0;
	temp[103]=0;
	temp[104]=0;
	temp[105]=0;
	temp[106]=0;
	temp[107]=0;
	temp[108]=0;
	temp[109]=0;
	temp[110]=0;
	temp[111]=0;
	temp[112]=0;
	temp[113]=0;
	temp[114]=0;
	temp[115]=0;
	temp[116]=0;
	temp[117]=0;
	temp[118]=0;
	temp[119]=0;
	temp[120]=0;
	temp[121]=0;
	temp[122]=0;
	temp[123]=0;
	temp[124]=0;
	temp[125]=0;
	temp[126]=0;
	temp[127]=0;
	temp[128]=0;
	temp[129]=0;
	temp[130]=0;
	temp[131]=0;
	temp[132]=0;
	temp[133]=0;
	temp[134]=0;
	temp[135]=0;
	temp[136]=0;
	temp[137]=0;
	temp[138]=0;
	temp[139]=0;
	temp[140]=0;
	temp[141]=0;
	temp[142]=0;
	temp[143]=0;
	temp[144]=0;
	temp[145]=0;
	temp[146]=0;
	temp[147]=0;
	temp[148]=0;
	temp[149]=0;
	temp[150]=0;
	temp[151]=0;
	temp[152]=0;
	temp[153]=0;
	temp[154]=0;
	temp[155]=0;
	temp[156]=0;
	temp[157]=0;
	temp[158]=0;
	temp[159]=0;
	temp[160]=0;
	temp[161]=0;
	temp[162]=0;
	temp[163]=0;
	temp[164]=0;
	temp[165]=0;
	temp[166]=0;
	temp[167]=0;
	temp[168]=0;
	temp[169]=0;
	temp[170]=0;
	temp[171]=0;
	temp[172]=0;
	temp[173]=0;
	temp[174]=0;
	temp[175]=0;
	temp[176]=0;
	temp[177]=0;
	temp[178]=0;
	temp[179]=0;
	temp[180]=0;
	temp[181]=0;
	temp[182]=0;
	temp[183]=0;
	temp[184]=0;
	temp[185]=0;
	temp[186]=0;
	temp[187]=0;
	temp[188]=0;
	temp[189]=0;
	temp[190]=0;
	temp[191]=0;
	temp[192]=0;
	temp[193]=0;
	temp[194]=0;
	temp[195]=0;
	temp[196]=0;
	temp[197]=0;
	temp[198]=0;
	temp[199]=0;
	temp[200]=0;
	temp[201]=0;
	temp[202]=0;
	temp[203]=0;
	temp[204]=0;
	temp[205]=0;
	temp[206]=0;
	temp[207]=0;
	temp[208]=0;
	temp[209]=0;
	temp[210]=0;
	temp[211]=0;
	temp[212]=0;
	temp[213]=0;
	temp[214]=0;
	temp[215]=0;
	temp[216]=0;
	temp[217]=0;
	temp[218]=0;
	temp[219]=0;
	temp[220]=0;
	temp[221]=0;
	temp[222]=0;
	temp[223]=0;
	temp[224]=0;
	temp[225]=0;
	temp[226]=0;
	temp[227]=0;
	temp[228]=0;
	temp[229]=0;
	temp[230]=0;
	temp[231]=0;
	temp[232]=0;
	temp[233]=0;
	temp[234]=0;
	temp[235]=0;
	temp[236]=0;
	temp[237]=0;
	temp[238]=0;
	temp[239]=0;
	temp[240]=0;
	temp[241]=0;
	temp[242]=0;
	temp[243]=0;
	temp[244]=0;
	temp[245]=0;
	temp[246]=0;
	temp[247]=0;
	temp[248]=0;
	temp[249]=0;
	temp[250]=0;
	temp[251]=0;
	temp[252]=0;
	temp[253]=0;
	temp[254]=0;
	temp[255]=0;
	temp[256]=0;
	temp[257]=0;
	temp[258]=0;
	temp[259]=0;
	temp[260]=0;
	temp[261]=0;
	temp[262]=0;
	temp[263]=0;
	temp[264]=0;
	temp[265]=0;
	temp[266]=0;
	temp[267]=0;
	temp[268]=0;
	temp[269]=0;
	temp[270]=0;
	temp[271]=0;
	temp[272]=0;
	temp[273]=0;
	temp[274]=0;
	temp[275]=0;
	temp[276]=0;
	temp[277]=0;
	temp[278]=0;
	temp[279]=0;
	temp[280]=0;
	temp[281]=0;
	temp[282]=0;
	temp[283]=0;
	temp[284]=0;
	temp[285]=0;
	temp[286]=0;
	temp[287]=0;
	temp[288]=0;
	temp[289]=0;
	temp[290]=0;
	temp[291]=0;
	temp[292]=0;
	temp[293]=0;
	temp[294]=0;
	temp[295]=0;
	temp[296]=0;
	temp[297]=0;
	temp[298]=0;
	temp[299]=0;
	temp[300]=0;
	temp[301]=0;
	temp[302]=0;
	temp[303]=0;
	temp[304]=0;
	temp[305]=0;
	temp[306]=0;
	temp[307]=0;
	temp[308]=0;
	temp[309]=0;
	temp[310]=0;
	temp[311]=0;
	temp[312]=0;
	temp[313]=0;
	temp[314]=0;
	temp[315]=0;
	temp[316]=0;
	temp[317]=0;
	temp[318]=0;
	temp[319]=0;
	temp[320]=1;
	temp[321]=0;
	temp[322]=0;
	temp[323]=0;
	temp[324]=1;
	temp[325]=0;
	temp[326]=0;
	temp[327]=0;
	temp[328]=1;
	temp[329]=0;
	temp[330]=0;
	temp[331]=0;
	temp[332]=160;
	temp[333]=134;
	temp[334]=1;
	temp[335]=0;
	temp[336]=1;
	temp[337]=0;
	temp[338]=0;
	temp[339]=0;
	temp[340]=0;
	temp[341]=0;
	temp[342]=0;
	temp[343]=0;
	temp[344]=1;
	temp[345]=2;
	temp[346]=136;
	temp[347]=19;
	temp[348]=0;
	temp[349]=0;
	temp[350]=0;
	temp[351]=0;
	temp[352]=0;
	temp[353]=0;
	temp[354]=1;
	temp[355]=1;
	temp[356]=1;
	temp[357]=1;
	temp[358]=1;
	temp[359]=1;
	temp[360]=1;
	temp[361]=0;
	temp[362]=0;
	temp[363]=0;
	temp[364]=0;
	temp[365]=0;
	temp[366]=0;
	temp[367]=0;
	temp[368]=8;
	temp[369]=0;
	temp[370]=0;
	temp[371]=0;
	temp[372]=0;
	temp[373]=0;
	temp[374]=0;
	temp[375]=0;
	temp[376]=3;
	temp[377]=0;
	temp[378]=0;
	temp[379]=0;
	temp[380]=0;
	temp[381]=0;
	temp[382]=0;
	temp[383]=0;
	temp[384]=0;
	temp[385]=0;
	temp[386]=0;
	temp[387]=0;
	temp[388]=0;
	temp[389]=0;
	temp[390]=0;
	temp[391]=0;
	temp[392]=74;
	temp[393]=1;
	temp[394]=0;
	temp[395]=0;
	temp[396]=13;
	temp[397]=0;
	temp[398]=0;
	temp[399]=0;
	temp[400]=10;
	temp[401]=0;
	temp[402]=0;
	temp[403]=0;
	temp[404]=0;
	temp[405]=0;
	temp[406]=0;
	temp[407]=0;
	temp[408]=231;
	temp[409]=3;
	temp[410]=0;
	temp[411]=0;
	temp[412]=160;
	temp[413]=134;
	temp[414]=1;
	temp[415]=0;
	temp[416]=0;
	temp[417]=0;
	temp[418]=0;
	temp[419]=0;
	temp[420]=0;
	temp[421]=0;
	temp[422]=0;
	temp[423]=0;
	temp[424]=0;
	temp[425]=0;
	temp[426]=0;
	temp[427]=0;
	temp[428]=0;
	temp[429]=0;
	temp[430]=0;
	temp[431]=0;
	temp[432]=0;
	temp[433]=8;
	temp[434]=0;
	temp[435]=0;
	temp[436]=64;
	temp[437]=1;
	temp[438]=0;
	temp[439]=0;
	temp[440]=0;
	temp[441]=0;
	temp[442]=0;
	temp[443]=0;
	temp[444]=0;
	temp[445]=0;
	temp[446]=0;
	temp[447]=0;
	temp[448]=0;
	temp[449]=0;
	temp[450]=0;
	temp[451]=0;
	temp[452]=0;
	temp[453]=0;
	temp[454]=0;
	temp[455]=0;
	temp[456]=0;
	temp[457]=0;
	temp[458]=0;
	temp[459]=0;
	temp[460]=0;
	temp[461]=0;
	temp[462]=0;
	temp[463]=0;
	temp[464]=0;
	temp[465]=0;
	temp[466]=0;
	temp[467]=0;
	temp[468]=0;
	temp[469]=0;
	temp[470]=1;
	temp[471]=0;
	temp[472]=0;
	temp[473]=0;
	temp[474]=0;
	temp[475]=0;
	temp[476]=204;
	temp[477]=0;
	temp[478]=0;
	temp[479]=0;
	temp[480]=0;
	temp[481]=0;
	temp[482]=0;
	temp[483]=0;
	temp[484]=0;
	temp[485]=0;
	temp[486]=0;
	temp[487]=63;
	temp[488]=131;
	temp[489]=249;
	temp[490]=162;
	temp[491]=61;
	temp[492]=0;
	temp[493]=0;
	temp[494]=150;
	temp[495]=67;
	temp[496]=0;
	temp[497]=0;
	temp[498]=32;
	temp[499]=65;
	temp[500]=0;
	temp[501]=0;
	temp[502]=0;
	temp[503]=0;
	temp[504]=5;
	temp[505]=0;
	temp[506]=0;
	temp[507]=0;
	temp[508]=3;
	temp[509]=0;
	temp[510]=0;
	temp[511]=0;
	temp[512]=5;
	temp[513]=0;
	temp[514]=0;
	temp[515]=0;
	temp[516]=100;
	temp[517]=0;
	temp[518]=0;
	temp[519]=0;
	temp[520]=5;
	temp[521]=0;
	temp[522]=0;
	temp[523]=0;
	temp[524]=0;
	temp[525]=0;
	temp[526]=0;
	temp[527]=0;
	temp[528]=1;
	temp[529]=0;
	temp[530]=0;
	temp[531]=0;
	temp[532]=0;
	temp[533]=0;
	temp[534]=0;
	temp[535]=0;
	temp[536]=204;
	temp[537]=204;
	temp[538]=204;
	temp[539]=204;
	fwrite( (char*)temp, sizeof(char), 540, m_lpTERDataFile);*/

	ConfigureSet *cfg = RadarManager::Instance()->getConfigureSet();


	C_SamplingParam uepSamplingParamBlock;
	for(int i=1;i<64;i++){
		uepSamplingParamBlock.m_trace_switch[0][i]=1;
	}
	for(int i=0;i<9;i++){
		uepSamplingParamBlock.m_trace_switch[1][i]=1;
	}
	for(int i=9;i<64;i++){
		uepSamplingParamBlock.m_trace_switch[1][i]=0;
	}
	for(int i=2;i<5;i++){
		for(int j=0;j<64;j++){
			uepSamplingParamBlock.m_trace_switch[i][j]=0;
		}
	}
	uepSamplingParamBlock.m_line_num=1;
	uepSamplingParamBlock.m_file_num=1;
	uepSamplingParamBlock.m_conn_mode=1;
	uepSamplingParamBlock.m_channel_num=100000;
	uepSamplingParamBlock.m_sampling_rate=1;
	uepSamplingParamBlock.m_data_bits=0;
	uepSamplingParamBlock.m_sample_mode=1;
	uepSamplingParamBlock.m_trigger_mode=2;/*atoi( cfg->get("send", "mode").c_str())+1;//触发模式//01-同步线缆触发 02-GPS同步触发 03-测量轮触发 04-手动触发 05-zigbee触发 */
	uepSamplingParamBlock.m_points_num=5000;
	uepSamplingParamBlock.m_buf_switch=false;
	for(int i=0;i<5;i++){
		uepSamplingParamBlock.m_pga[i]=0;
	}
	//uepSamplingParamBlock.pga=cfg->get("receive", "gain").c_str()+1;//接收机增益
	uepSamplingParamBlock.pga=1;//接收机增益
	for(int i=0;i<5;i++){
		uepSamplingParamBlock.m_adc_check[i]=true;
	}
	uepSamplingParamBlock.m_adc_adjust=true;
	uepSamplingParamBlock.m_stacking_fold=(unsigned int)atoi( cfg->get("receive", "stackingFold").c_str());//接收机叠加次数
	uepSamplingParamBlock.m_trigger_delay=atoi( cfg->get("receive", "delay").c_str());//接收机延时

	uepSamplingParamBlock.m_elec_single_sample_mode=0;//
	uepSamplingParamBlock.m_elec_trigger_mode=atoi( cfg->get("send", "mode").c_str())+1;//触发模式//01-同步线缆触发 02-GPS同步触发 03-测量轮触发 04-手动触发 05-zigbee触发 
	uepSamplingParamBlock.m_elec_highspeed_mode=0;
	uepSamplingParamBlock.m_elec_data_bits=0;
	uepSamplingParamBlock.m_elec_sample_enable=0;
	uepSamplingParamBlock.m_elec_sample_points_num=(unsigned short)atoi( cfg->get("receive", "sampleCount").c_str());//采样点

	uepSamplingParamBlock.m_elec_frequency_rank=atoi( cfg->get("send", "frequency").c_str())+1;//发射机频率
	uepSamplingParamBlock.m_elec_duty_ratio=atoi( cfg->get("send", "dutyRatio").c_str());//发射机占空比
	uepSamplingParamBlock.m_elec_trigger_delay=atoi( cfg->get("send", "delay").c_str());//发射机延时
	uepSamplingParamBlock.m_elec_send_time=999;
	uepSamplingParamBlock.m_elec_cycles_times=100000;
	uepSamplingParamBlock.m_elec_gps_start_time.wYear=0;
	uepSamplingParamBlock.m_elec_gps_start_time.wMonth=0;
	uepSamplingParamBlock.m_elec_gps_start_time.wDayOfWeek=0;
	uepSamplingParamBlock.m_elec_gps_start_time.wDay=0;
	uepSamplingParamBlock.m_elec_gps_start_time.wHour=0;
	uepSamplingParamBlock.m_elec_gps_start_time.wMinute=0;
	uepSamplingParamBlock.m_elec_gps_start_time.wSecond=0;
	uepSamplingParamBlock.m_elec_gps_start_time.wMilliseconds=0;
	uepSamplingParamBlock.m_elec_wheelpulse_times=RadarManager::Instance()->getPrecRatio(atoi( cfg->get("radar", "precRatio").c_str()))*4;//512*4
	uepSamplingParamBlock.m_elec_gps_sync_cycle=320;
	uepSamplingParamBlock.m_elec_start_longitude=0.0;
	uepSamplingParamBlock.m_elec_start_EW_longitude=0;
	uepSamplingParamBlock.m_elec_start_latitude=0.0;
	uepSamplingParamBlock.m_elec_start_SN_latitude=0;
	uepSamplingParamBlock.m_elec_end_longitude=0.0;
	uepSamplingParamBlock.m_elec_end_EW_longitude=0;
	uepSamplingParamBlock.m_elec_end_latitude=0.0;
	uepSamplingParamBlock.m_elec_end_SN_latitude=0;

	uepSamplingParamBlock.m_enable_trigger_wheel=true;
	uepSamplingParamBlock.m_trigger_wheel_com_id=0;
	uepSamplingParamBlock.m_pluses_per_trigger=(unsigned int)(float)(uepSamplingParamBlock.m_elec_wheelpulse_times)/(atof( cfg->get("send", "perimeter").c_str())/(1/atof( cfg->get("send", "measuringWheelPointPerMeter").c_str())));//2048/（周长/（1/n点每米））
	uepSamplingParamBlock.m_sample_times_per_trigger=0;
	uepSamplingParamBlock.m_wheel_forward_length=atof( cfg->get("send", "perimeter").c_str());//周长
	uepSamplingParamBlock.m_wheel_radius=uepSamplingParamBlock.m_wheel_forward_length/2.0/acos(-1.00000);//半径  acos(-1)为pi
	uepSamplingParamBlock.m_eq_velocity=300.000;
	uepSamplingParamBlock.m_elec_current=10.000;
	uepSamplingParamBlock.m_loop_type=0;
	uepSamplingParamBlock.m_elec_sampling_rate=atoi( cfg->get("receive", "samplingRate").c_str())+1;//接收机频率

	uepSamplingParamBlock.m_dp_frequency_rank=3;
	uepSamplingParamBlock.m_dp_sampling_rate=5;
	uepSamplingParamBlock.m_dp_cycles_times=100;
	uepSamplingParamBlock.m_dp_empty_cycles_times=5;
	uepSamplingParamBlock.m_dp_sendwave_type=0;
	uepSamplingParamBlock.m_dp_pga=1;

	uepSamplingParamBlock.m_device_type=0;
	uepSamplingParamBlock.m_device_childtype=0;

	//fwrite( (char*)&uepSamplingParamBlock, sizeof(UEP_SamplingParamBlock), 1, m_lpTERDataFile);
	fwrite( (char*)&uepSamplingParamBlock, 540, 1, m_lpTERDataFile);
}

bool TERDataWriter::closeUep(){
	fclose( m_lpTERDataFile );
	m_lpTERDataFile = 0;
	return true;
}