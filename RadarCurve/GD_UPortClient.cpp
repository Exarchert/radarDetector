#include "StdAfx.h"
#include "GD_UPortClient.h"

bool GD_UPortClient::Init(int iType)
{
	bool flag = false;

	FindLoopDevice();//链接
	if (NULL == OutEndpt || NULL == InEndpt)
	{
		return flag;
	}
	{
		InEndpt->TimeOut = 2000; 
		OutEndpt->TimeOut = 2000;


		CTime cStartTime = GetCurrentTime();
		CTime cNowTime;
		CTimeSpan cTimeDis;

		long lnReqLen = 4096;
		BYTE btReadBuf[4096];
		StopSample();
		while (lnReqLen == 4096 )
		{
			InEndpt->XferData((UCHAR*)btReadBuf, lnReqLen);
		}

	}

	if (!SendParameter(iType))
	{
		return flag;
	}
	if (!SendWorkState())
	{
		return flag;
	}
	return true;;
}
bool GD_UPortClient::SendParameter(int iType)
{
	bool flag = false;
	if (!USBDevice->IsOpen())
	{
		return flag;
	}
	
	if(RADAR_WORK_TYPE_EIGHT == iType)//7月25日增加 网口模块
	{
		m_iSampleNum = (int)(256*pow(2,(double)m_iSampleNumIndex));
		m_iTotalByte = 2*m_iSampleNum  ;
		m_AddTime = pow(2,(double)m_iAddTimeIndex)-1;

		m_stuSend.btSmpNumAdr = 1;
		m_stuSend.btSmpNum = m_iSampleNumIndex;//1  m_iSampleNumIndex
		m_stuSend.btSmpRateAdr = 2;
		m_stuSend.btSmpRate = m_iSampleRateIndex;//2
		m_stuSend.btGainModeAdr = 3;
		m_stuSend.btGainMode = m_iBit;//0m_iBit
		m_stuSend.btGainValAdr = 4;
		m_stuSend.btGainVal = m_iLevel;//m_iLevel
		m_stuSend.btTrigLevelAdr = 5;
		m_stuSend.btTrigLevel = 10;
		m_stuSend.btTimeDelayAdr = 7;
		m_stuSend.btTimeDelay = m_attendigital;//m_attendigital  3
		m_stuSend.btAddTimesAdr = 8;
		m_stuSend.btAddTimes = m_AddTime;
		m_stuSend.btReFreqAdr = 9;
		m_stuSend.btReFreq = 0;
		m_stuSend.btWheelAdr = 10;
		m_stuSend.btWheel = m_bMode;//
		m_stuSend.btPreciseAdr = 11;
		m_stuSend.btPrecise = m_iMeasure;//3   
		m_stuSend.btSmpModeAdr = 13;
		m_stuSend.btSmpMode = 0;//
		m_stuSend.btTrigModeAdr = 85;
		m_stuSend.btTrigMode = 0;
		m_stuSend.btSteelDiameterAdr = 49;
		m_stuSend.btSteelDiameter = 16;
		m_stuSend.btSteelRangeAdr = 50;
		m_stuSend.btSteelRange = 0;
		m_stuSend.btSteelModeAdr = 48;
		m_stuSend.btSteelMode = 1;

		if (SendData((BYTE*)&m_stuSend, sizeof(StuSendInfo)) < 0)
		{
			return false;
		}

		//BYTE bBUF[4];
		//bBUF[0] = 0;
		//bBUF[1] = 15;
		//bBUF[2] = 0;
		//bBUF[3] = 15;
		//if (SendData(bBUF, 4 )< 0)
		//{
		//	return false;
		//}
		return TRUE;
	}

	if(RADAR_WORK_TYPE_FOUR_USB == iType)
	{
		m_iSampleNum = (int)(256*pow(2,(double)m_iSampleNumIndex));
		m_iTotalByte = 2*m_iSampleNum  ;
		m_AddTime = pow(2,(double)m_iAddTimeIndex)-1;
	}
	if(RADAR_WORK_TYPE_DOUBLE_USB_OLD == iType)
	{
		m_iSampleNum = (int)(256*pow(2,(double)m_iSampleNumIndex));
		m_iTotalByte = 4*m_iSampleNum  ;
		m_AddTime = pow(2,(double)m_iAddTimeIndex)-1;
	}
	if(RADAR_WORK_TYPE_DOUBLE_USB == iType)
	{
		m_iSampleNum = (int)(256*pow(2,(double)m_iSampleNumIndex));
		m_iTotalByte = 2*m_iSampleNum;
		m_AddTime = pow(2,(double)m_iAddTimeIndex)-1;
		
		//1.17下至doubleusb中的return true
		m_stuSend.btSmpNumAdr = 1;
		m_stuSend.btSmpNum = m_iSampleNumIndex;//1  m_iSampleNumIndex
		m_stuSend.btSmpRateAdr = 2;
		m_stuSend.btSmpRate = m_iSampleRateIndex;//2
		m_stuSend.btGainModeAdr = 3;
		m_stuSend.btGainMode = m_iBit;//0m_iBit
		m_stuSend.btGainValAdr = 4;
		m_stuSend.btGainVal = m_iLevel;//m_iLevel
		m_stuSend.btTrigLevelAdr = 5;
		m_stuSend.btTrigLevel = 10;
		m_stuSend.btTimeDelayAdr = 7;
		m_stuSend.btTimeDelay = m_attendigital;//m_attendigital  3
		m_stuSend.btAddTimesAdr = 8;
		m_stuSend.btAddTimes = m_AddTime;
		m_stuSend.btReFreqAdr = 9;
		m_stuSend.btReFreq = 0;
		m_stuSend.btWheelAdr = 10;
		m_stuSend.btWheel = m_bMode;//
		m_stuSend.btPreciseAdr = 11;
		m_stuSend.btPrecise = m_iMeasure;//3   
		m_stuSend.btSmpModeAdr = 13;
		m_stuSend.btSmpMode = 0;//
		m_stuSend.btTrigModeAdr = 85;
		m_stuSend.btTrigMode = 0;
		m_stuSend.btSteelDiameterAdr = 49;
		m_stuSend.btSteelDiameter = 16;
		m_stuSend.btSteelRangeAdr = 50;
		m_stuSend.btSteelRange = 0;
		m_stuSend.btSteelModeAdr = 48;
		m_stuSend.btSteelMode = 1;

		if (SendData((BYTE*)&m_stuSend, sizeof(StuSendInfo)) < 0)
		{
			return false;
		}

		//BYTE bBUF[4];
		//bBUF[0] = 0;
		//bBUF[1] = 15;
		//bBUF[2] = 0;
		//bBUF[3] = 15;
		//if (SendData(bBUF, 4 )< 0)
		//{
		//	return false;
		//}
		return TRUE;
	}
	else if(RADAR_WORK_TYPE_ONE_USB == iType)
	{
		m_iSampleNum = (int)(256*pow(2,(double)m_iSampleNumIndex));
		m_iTotalByte = 2*m_iSampleNum  ;
		m_AddTime = pow(2,(double)m_iAddTimeIndex)-1;

		m_stuSend.btSmpNumAdr = 1;
		m_stuSend.btSmpNum = m_iSampleNumIndex;//1  m_iSampleNumIndex
		m_stuSend.btSmpRateAdr = 2;
		m_stuSend.btSmpRate = m_iSampleRateIndex;//2
		m_stuSend.btGainModeAdr = 3;
		m_stuSend.btGainMode = m_iBit;//0m_iBit
		m_stuSend.btGainValAdr = 4;
		m_stuSend.btGainVal = m_iLevel;//m_iLevel
		m_stuSend.btTrigLevelAdr = 5;
		m_stuSend.btTrigLevel = 10;
		m_stuSend.btTimeDelayAdr = 7;
		m_stuSend.btTimeDelay = m_attendigital;//m_attendigital  3
		m_stuSend.btAddTimesAdr = 8;
		m_stuSend.btAddTimes = m_AddTime;
		m_stuSend.btReFreqAdr = 9;
		m_stuSend.btReFreq = 0;
		m_stuSend.btWheelAdr = 10;
		m_stuSend.btWheel = m_bMode;//
		m_stuSend.btPreciseAdr = 11;
		m_stuSend.btPrecise = m_iMeasure;//3   
		m_stuSend.btSmpModeAdr = 13;
		m_stuSend.btSmpMode = 0;//
		m_stuSend.btTrigModeAdr = 85;
		m_stuSend.btTrigMode = 0;
		m_stuSend.btSteelDiameterAdr = 49;
		m_stuSend.btSteelDiameter = 16;
		m_stuSend.btSteelRangeAdr = 50;
		m_stuSend.btSteelRange = 0;
		m_stuSend.btSteelModeAdr = 48;
		m_stuSend.btSteelMode = 1;

		if (SendData((BYTE*)&m_stuSend, sizeof(StuSendInfo)) < 0)
		{
			return false;
		}

		//BYTE bBUF[4];
		//bBUF[0] = 0;
		//bBUF[1] = 15;
		//bBUF[2] = 0;
		//bBUF[3] = 15;
		//if (SendData(bBUF, 4 )< 0)
		//{
		//	return false;
		//}
		return TRUE;
	}

	byte para[18] = {
				  	 0x01,m_iSampleNumIndex,		
					 0x02,m_iSampleRateIndex,		
				 	 0x03,m_iBit,					
					 0x04,m_iLevel,					
					 //0x05                      
				   	 //0x06                        
					 0x07,m_attendigital,		
					 0x08,m_AddTime,					 
					 //0x09,0,			
					 0x0A,m_bMode,					
					 0x0B,m_iMeasure				
				   	 };

	OutEndpt->TimeOut = 2000;
	byte GainWorkComma[2] = {0xFF,0x01};
	SendData(GainWorkComma,2);//
	if(m_btGainBuf == NULL)
	{
		m_btGainBuf = new byte [8192];
	}
	SendData(m_btGainBuf,8192);
	SendData(para,18);
	if (m_btGainBuf != NULL)
	{
		delete []m_btGainBuf;
		m_btGainBuf = NULL; 
	}
	return true;
}
