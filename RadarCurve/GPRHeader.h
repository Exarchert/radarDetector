#ifndef __GPR_HEADER_H_FILE__
#define __GPR_HEADER_H_FILE__


//****************************** GPR **************************************//
//***************************** GPR *************************************//
#pragma pack(push)
#pragma pack(1)
struct _GPR_HEAD
{
	// 文件信息
	bool bShowFileTitle;  // 显示文件标题 false
	char chFileTitle[85]; // 文件标题	  ""

	char chProjectTime[32];

	short  shTrcHeadPts;      // 45
	double dLength;           // 总长度
	double dLenInt;           // 长度间隔
	char   chClrFileName[64]; // 颜色文件名	""
	CPoint sHandGainPt[10];   // 手动增益点	(0,0)
	int    iGainPtNum;        // 增益点数	10
	double dMaxGainTime;      // 最大增益倍数

	bool   bHasGPSInfo;       // 有无GPS信息

	double dDepthInt; // 深度间隔

	BYTE btReserve1[111]; 

	char   chFileFlag[4];// 标志:     "GPR" 
	double dDielectric;	 // 介电常数  5

	BYTE btReserve2[8];

    double dTimeWnd;	 // 时窗      50
	int    iSmpNum;      // 采样点数  512
	int    iTrcNum;	     // 采样道数

	// 关键参数
	unsigned char ucRadarModel[6];
	unsigned char  ucWorkMode;
	unsigned char  ucTrigMode;
	float		   fScanSpeed;
	float		   fTrcInt;
	float          fSmpRate;
	unsigned char  ucGainMode;
	unsigned short usGainVal;
	unsigned short usAddTimes;
	unsigned short usTimeDelay;
	float		   fRepeatFreq;
	unsigned char  ucTrigLevel;
	unsigned char  ucSampleBit;
	double		   dDCValue;
	double		   dWheelPerimeter;
	unsigned short iWheelPulseNum;

	BYTE btReserve3[73]; 

	// 显示参数
	int iSwMode;  // 模式
	int iTrcInt;  // 抽道
	int iTrcDis;  // 道间
	int iMaxAmp;  // 振幅
	bool bNormal; // 归一

	BYTE btReserve4[128];

	// 坐标参数
	int iLfAxMode; // 1 (1样点, 10时间, 100深度)
	int iRtAxMode; // 1 (1样点, 10时间, 100深度)
	int iTpAxMode; // 1 (1道数, 10距离)  
	int iBtAxMode; // 1 (1道数, 10距离)  
	// 左轴
	int iLfMinSmp;     // 100
	double dLfMinTime; // 50
	double dLfMinDepth;// 1
	// 右轴 
	int iRtMinSmp;     // 100
	double dRtMinTime; // 50
	double dRtMinDepth;// 1
	// 上轴 
	int iTpMinTrc;   // 100
	double dTpMinDis;// 10
	// 下轴 
	int iBtMinTrc;   // 100	
	double dBtMinDis;// 10
	// 里程
	int iShowMileAxis;	  // 0
	float fStartBigMile;  // 0
	float fStartSmallMile;// 0
	char chMileDescrip[64];
	// 模式
	BOOL bAutoAxis;  // TRUE
	BOOL bChsAxis;   // TRUE 
	int iZeroPos;    // 0

	BYTE btReserve5[52];

	// 文件描述
	char chDescription[128];
};
#pragma pack(pop)


// GPR道参数(90字节)
#pragma pack(push)
#pragma pack(1)
struct _GPR_TRACE
{
	float		   fGPSOffset;

	char chReserve[2];

	unsigned char  ucYear;
	unsigned char  ucMonth;
	unsigned char  ucDay;
	unsigned char  ucHour;
	unsigned char  ucMin;
	unsigned char  ucSec;
	unsigned short usMilSec;

	double		   dPosX;
	double		   dPosY;
	double		   dPosZ;

	unsigned char  ucTrcCount[2];
	unsigned char  ucVoltage[2];

	float          fWheelOffset;

	char		   chPhotoName1[8];
	char		   chPhotoName2[8];

	unsigned short usMetalDiameter;
    unsigned short usMetalDepth;
	bool		   bMetalFlag;

	char		   chMarkName[17];
	float		   fMarkHeight;
	unsigned short usMarkFlag;
};
#pragma pack(pop)


#endif // _SGY_HEADER_H_FILE__