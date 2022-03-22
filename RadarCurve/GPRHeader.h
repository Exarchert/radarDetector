#ifndef __GPR_HEADER_H_FILE__
#define __GPR_HEADER_H_FILE__


//****************************** GPR **************************************//
//***************************** GPR *************************************//
#pragma pack(push)
#pragma pack(1)
struct _GPR_HEAD
{
	// �ļ���Ϣ
	bool bShowFileTitle;  // ��ʾ�ļ����� false
	char chFileTitle[85]; // �ļ�����	  ""

	char chProjectTime[32];

	short  shTrcHeadPts;      // 45
	double dLength;           // �ܳ���
	double dLenInt;           // ���ȼ��
	char   chClrFileName[64]; // ��ɫ�ļ���	""
	CPoint sHandGainPt[10];   // �ֶ������	(0,0)
	int    iGainPtNum;        // �������	10
	double dMaxGainTime;      // ������汶��

	bool   bHasGPSInfo;       // ����GPS��Ϣ

	double dDepthInt; // ��ȼ��

	BYTE btReserve1[111]; 

	char   chFileFlag[4];// ��־:     "GPR" 
	double dDielectric;	 // ��糣��  5

	BYTE btReserve2[8];

    double dTimeWnd;	 // ʱ��      50
	int    iSmpNum;      // ��������  512
	int    iTrcNum;	     // ��������

	// �ؼ�����
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

	// ��ʾ����
	int iSwMode;  // ģʽ
	int iTrcInt;  // ���
	int iTrcDis;  // ����
	int iMaxAmp;  // ���
	bool bNormal; // ��һ

	BYTE btReserve4[128];

	// �������
	int iLfAxMode; // 1 (1����, 10ʱ��, 100���)
	int iRtAxMode; // 1 (1����, 10ʱ��, 100���)
	int iTpAxMode; // 1 (1����, 10����)  
	int iBtAxMode; // 1 (1����, 10����)  
	// ����
	int iLfMinSmp;     // 100
	double dLfMinTime; // 50
	double dLfMinDepth;// 1
	// ���� 
	int iRtMinSmp;     // 100
	double dRtMinTime; // 50
	double dRtMinDepth;// 1
	// ���� 
	int iTpMinTrc;   // 100
	double dTpMinDis;// 10
	// ���� 
	int iBtMinTrc;   // 100	
	double dBtMinDis;// 10
	// ���
	int iShowMileAxis;	  // 0
	float fStartBigMile;  // 0
	float fStartSmallMile;// 0
	char chMileDescrip[64];
	// ģʽ
	BOOL bAutoAxis;  // TRUE
	BOOL bChsAxis;   // TRUE 
	int iZeroPos;    // 0

	BYTE btReserve5[52];

	// �ļ�����
	char chDescription[128];
};
#pragma pack(pop)


// GPR������(90�ֽ�)
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