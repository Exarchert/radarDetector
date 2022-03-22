#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>



#include <RadarCurve/radarsocketclient.h>


CRadarSocketClient::CRadarSocketClient(HANDLE hnd)
{
	m_strIPAddress = "192.168.1.1";
	m_iPortNo = 5050;

	m_iReadTimeOut = 9;
	m_iWriteTimeOut = 3;

	m_bConnected = false;
}

CRadarSocketClient::~CRadarSocketClient()
{
	CloseDevice();
}

void CRadarSocketClient::setServerIP( std::string const& serverIP )
{
	m_strIPAddress = serverIP;
}
void CRadarSocketClient::setServerPort( unsigned int port )
{
	m_iPortNo = port;
}

bool CRadarSocketClient::Connect()
{
	m_sSocket = INVALID_SOCKET;

	m_usIPPort = m_iPortNo;

	int iReturn = CreateRadarSocketClient();

	// 创建无线网络通信失败
	if (iReturn != SUCCESS)
	{
		m_bConnected = false;
	}else{
		m_bConnected = true;
	}

	return m_bConnected;
}

void CRadarSocketClient::ReConnect()
{
	if (m_sSocket != INVALID_SOCKET)
	{
		closesocket(m_sSocket);
		m_sSocket = INVALID_SOCKET;		
	}
	
	WSACleanup();


	Sleep(200);

	Connect();
}

int CRadarSocketClient::SocketStartUp()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(1, 1);
	
	if (WSAStartup(wVersionRequested, &wsaData) != 0)
	{
		return FAILURE;
	}

	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		return FAILURE;
	}

	return SUCCESS;	
}

int CRadarSocketClient::CreateRadarSocketClient()
{
	SOCKADDR_IN sSockAddr;
	sSockAddr.sin_family = AF_INET;
	sSockAddr.sin_addr.S_un.S_addr = inet_addr(m_strIPAddress.c_str());
	sSockAddr.sin_port = htons(m_usIPPort);
	
	if (SocketStartUp() == FAILURE)
	{
		RADARMSG(_T("初始化套接字库失败！"));
		return FAILURE;
	}

	m_sSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sSocket == INVALID_SOCKET)
	{
		RADARMSG(_T("创建套接字设备失败！"));
		return FAILURE;
	}
	
	const int iArgp = 1L;  // 非阻塞模式

	if (ioctlsocket(m_sSocket, FIONBIO, (u_long FAR *)&iArgp) != 0 )
	{
		RADARMSG(_T("套接口模式设置失败！"));
		goto FAILRETURN;
	}

	int iTryTimes = 0;
	while (1)
	{
		if (connect(m_sSocket, (sockaddr*)&sSockAddr, sizeof(sockaddr)) == SOCKET_ERROR)
		{
			if (WSAGetLastError() == WSAEISCONN)
			{
				break;
			}

			iTryTimes++;			
			if (iTryTimes > 10)
			{
				goto FAILRETURN;
			}
			Sleep(300);
		}
		else
		{
			break;
		}
	}
	if (ioctlsocket(m_sSocket, FIONBIO, (u_long FAR *)&iArgp) != 0 )
	{
		RADARMSG(_T("套接口模式设置失败！"));
		goto FAILRETURN;
	}
	return SUCCESS;

FAILRETURN:
	closesocket(m_sSocket);
	m_sSocket = INVALID_SOCKET;
	return FAILURE;
}

int CRadarSocketClient::Send(byte *pSendBuf, long lnWantSendLen)
{
	// 发送参数
	int iReturn = Sendn(m_sSocket, &pSendBuf[0], lnWantSendLen, 0);

	return iReturn;
}

int CRadarSocketClient::Sendn(SOCKET sFD, const void *vPtr, long lnWantSendLen, int iFlag)
{
	int iWritten = send(sFD, (char*)vPtr, lnWantSendLen, iFlag);
	return iWritten;
}

int CRadarSocketClient::Recv(byte *pBuf, long lnBufLen)
{
	// 无线网络未连接
	if (!m_bConnected)
	{
		return 0;
	}

	int iReturn = Recvn(m_sSocket, (char*)pBuf, lnBufLen, 0);
	if ( iReturn < 0 )
	{
		CloseDevice();
	}

	return iReturn;

	
}

int CRadarSocketClient::Recvn(SOCKET sFD, const void *vPtr, long lnWantRecvLen, int iFlag)
{
	int iLeft = lnWantRecvLen, iRead;
	char* pChar = (char*)vPtr;
	timeval sTimeout;
	fd_set sRFD;

	int iTotalTimes = m_iReadTimeOut;
	sTimeout.tv_sec = 1;
	sTimeout.tv_usec = 0;

	int iSelRtn = 0;
	m_bUnStop = true;

	{
		FD_ZERO(&sRFD);
		FD_SET(sFD, &sRFD); 

		if ((iSelRtn = select(sFD+1, &sRFD, NULL, NULL, &sTimeout)) > 0)
		{
			if (FD_ISSET(sFD, &sRFD)) 
			{

				if ((iRead = recv(sFD, pChar, iLeft, iFlag)) < 0)
				{
					iRead = -1;
				}
				m_noDataCount = 0; 

				return iRead;
			}
		}
		else
		{
			if ( iSelRtn < 0 )
			{
				return -1;
			}
			iSelRtn = WSAGetLastError();

			m_noDataCount ++;
			// 如果两百次检测不到数据， 就认为已经断开了， 返回-1， 准备重连
			//if ( m_noDataCount > 200 )
			{
			//	return -1;
			}

		}
	}
	
	return 0; 
}

void CRadarSocketClient::CloseDevice()
{
	if (m_sSocket != INVALID_SOCKET)
	{
		closesocket(m_sSocket);
		m_sSocket = INVALID_SOCKET;		
	}
}







