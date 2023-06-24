#ifndef RADARSOCKETCLIENT_H
#define RADARSOCKETCLIENT_H

#include <RadarCurve/export.h>
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>


#define RADARMSG AfxMessageBox

#define SUCCESS		0
#define FAILURE		-1

#define MAX_SELECT_TIMEOUT 256 
//#define MAX_RETURN_TIMEOUT 300 

#define HEADER_LEN			3
#define PACKET_HEADER		"123"
#define PACKET_HEADER2      0xfedc

typedef struct packet_header
{
	char header[HEADER_LEN + 1];/*包头测试内容*/
	short param_num;			/*网络包携带参数的个数*/
}Packet_Header;

typedef struct whole_packet_header 
{
	Packet_Header packet_header;
	short data_tmp;
}Whole_Header;

////////////////////////////////////////////////
struct MyHeader
{
	short header;
};

class RADAR_EXPORT CRadarSocketClient
{
public:
	CRadarSocketClient(HANDLE hnd = NULL);
	virtual ~CRadarSocketClient();

	int m_iReadTimeOut;
	int m_iWriteTimeOut;
	bool volatile m_bUnStop;

	std::string m_strIPAddress;
	int     m_iPortNo;
	
public:

	void setServerIP( std::string const& serverIP );
	void setServerPort( unsigned int port );
	bool Connect();
	bool ReConnect();
	int CreateRadarSocketClient();


	int Send(byte *param, long param_num);
	int Recv(byte *buf, long buf_len);
	
	int Sendn(SOCKET sFD, const void *vPtr, long lnWantSendLen, int iFlag);
	int Recvn(SOCKET sFD, const void *vPtr, long lnWantRecvLen, int iFlag);

	void CloseDevice();

	bool m_bConnected;
	SOCKET m_sSocket;

protected:
	int SocketStartUp(void);

private:
	unsigned short m_usIPPort;

	unsigned int m_noDataCount;
};

#endif
