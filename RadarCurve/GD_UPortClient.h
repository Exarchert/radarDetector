#pragma once

#include "RadarCurve\CyAPI.h"
#include "RadarCurve\GlobalVar.h"
#include <math.h>
#include "MyMacro.h"

#define BUF_LENGTH			1024

class GD_UPortClient
{
public:
	GD_UPortClient(void)
	{
		m_iTotalByte = 1024;
		m_iAddTimeIndex = 0;
		m_iSampleNum = 0;
		m_iSampleNumIndex = 0;			
		m_iSampleRateIndex = 0;			
		m_iBit = 0;						
		m_iLevel = 0;					
		m_attendigital = 0;				
		m_AddTime = 0;					
		m_attenselindex = 0;			
		m_bMode = true;				   
		m_iMeasureIndex = 0;		
		m_iMeasure = 0;

		m_btGainBuf = NULL;
		m_bSample = false;
		OutEndpt = NULL;
		InEndpt = NULL;
		USBDevice = new CCyUSBDevice;
	}
	~GD_UPortClient(void)
	{
		StopSample();
		if (USBDevice->IsOpen())
		{
			USBDevice->Close();
		}
	}
	//找设备--给两个变量赋值
	void FindLoopDevice()
	{
		// Look for a device programmed for Bulk Loop (VID = 0547, PID = 1003)
		int devices = USBDevice->DeviceCount();
		for (int di = 0; di < devices; ++di)
		{
			 // Open automatically calls Close( ) if necessary
			if ((USBDevice->VendorID == 0x04b4) && (USBDevice->ProductID == 0x1004))
			{
				USBDevice->Open(di); 
				break;
			}
		}
		if ((USBDevice->VendorID == 0x04b4) && (USBDevice->ProductID == 0x1004))
		{
			// Load the endpoint combo boxes
			int epts = USBDevice->EndPointCount();
			for (int ei = 0; ei < epts; ++ei)
			{
				CCyUSBEndPoint *endpt;
				endpt = USBDevice->EndPoints[ei];

				if (endpt->Attributes == 2){   // bulk
					if (endpt->Address & 0x80){// InEndpoint
						InEndpt = endpt;
					}else{
						OutEndpt = endpt;      // OutEndpoint
					}
				}  
			}
		}
		return ;

	}

	//发送数据
	bool SendData( byte* SendDataBuf,long DataLen )
	{		
		LONG Len = DataLen;
		OutEndpt->XferData((byte*)SendDataBuf, Len);
		if (Len != DataLen)
		{
			return false;
		}
		return true;
	}
	//发送数据接
	bool SendParameter(int iType);
	//发送工作状态
	bool SendWorkState(int iType = 0)
	{
		// 参数
		byte para[4] = {0x00,0x0f,0x00,0x0f};
		LONG len = 4;
		OutEndpt->XferData((UCHAR*)para, len);
		if (len != 4)
		{
			//AfxMessageBox(L"命令发送失败!\n", MB_OK|MB_ICONQUESTION);
			return false;
		}
		if (1 == iType )
		{
			byte para1[2] = {0x00,0xB};
			LONG len1 = 2;
			OutEndpt->XferData((UCHAR*)para1, len1);
			if (2 != len1)
			{
				return false;
			}
		}
		return true;
	}
	void StopSample()
	{
		if (NULL == InEndpt || NULL == OutEndpt)
		{
			return;
		}
		m_bSample = FALSE;
		byte para[2] = {0x00, 0x00};
		LONG len = 2;

		OutEndpt->XferData((UCHAR*)para, len);
	}


	bool ReadData()
	{
		LONG Len = BUF_LENGTH;

		UCHAR inndDataBuf[BUF_LENGTH];
		memset(inndDataBuf,0,BUF_LENGTH);

		
		InEndpt->XferData((UCHAR*)inndDataBuf, Len);
		if (Len != 1024)
		{
			AfxMessageBox(L"命令发送失败!\n", MB_OK|MB_ICONQUESTION);
			return false;
		}
		return true;
	}

	bool Recv(UCHAR* cBuf , LONG & lLen)
	{
		return InEndpt->XferData(cBuf, lLen);
	}


	bool Init(int iType);
public:


protected:
	CCyUSBDevice *USBDevice;
	CCyUSBEndPoint *OutEndpt;
	CCyUSBEndPoint *InEndpt;

public:
	int m_iTotalByte;
	int m_iAddTimeIndex;
	int m_iSampleNum;               
	int m_iSampleNumIndex;			
	int m_iSampleRateIndex;			
	int m_iBit;						
	int m_iLevel;					
	int m_attendigital;				
	int m_AddTime;					
	int m_attenselindex;		
	int m_bMode;					
	int m_iMeasure;					
	int m_iMeasureIndex;			

	byte* m_btGainBuf;
	bool m_bSample;

	StuSendInfo m_stuSend;


};
