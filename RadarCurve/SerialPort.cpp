#include "stdafx.h"
#include <RadarCurve/SerialPort.h>

#include <assert.h>
#include <RadarCurve/GpsReader.h>

#include <RadarCurve/RadarManager.h>
#include <RadarCurve/ConfigureSet.h>


class ComThread : public OpenThreads::Thread
{
public:
	ComThread(CSerialPort *lpSerialPort )
	{
		_lpSerialPort = lpSerialPort;
	}
	virtual void run();
	virtual int cancel()
	{
		OpenThreads::Thread::cancel();
		if ( isRunning() )
		{
			join();

		}

		return 0;
	}

	CSerialPort *_lpSerialPort;
};

void ComThread::run()
{
	while( !testCancel() )
	{
		CSerialPort::CommThread( _lpSerialPort );
	}
}
 
//
// Constructor
//


CSerialPort::CSerialPort()
{
	m_hComm = INVALID_HANDLE_VALUE;
	// initialize overlapped structure members to zero
	m_ov.Offset = 0;
	m_ov.OffsetHigh = 0;

	// create events
	m_ov.hEvent = NULL;
	m_hWriteEvent = NULL;
	m_hShutdownEvent = NULL;

	m_szWriteBuffer = NULL;
	m_nWriteSize=1;

	//m_bThreadAlive = FALSE;

	m_pOwner = NULL;

	_gpsReader = NULL;
	_lpThread = NULL;

	m_nScanInterval=100;
	
}

//
// Delete dynamic memory
//
CSerialPort::~CSerialPort()
{
// 	do
// 	{
// 		SetEvent(m_hShutdownEvent);
// 	} while (m_bThreadAlive);

	
	// if the port is still opened: close it 
	if (m_hComm != NULL)
	{
		CloseHandle(m_hComm);
		m_hComm = NULL;
	}
	// Close Handles  
	if(m_hShutdownEvent!=NULL)
		CloseHandle( m_hShutdownEvent); 
	if(m_ov.hEvent!=NULL)
		CloseHandle( m_ov.hEvent ); 
	if(m_hWriteEvent!=NULL)
		CloseHandle( m_hWriteEvent ); 

	TRACE("Thread ended\n");
	delete [] m_szWriteBuffer;
}

void CSerialPort::SetGpsReader( GpsReader *lpReader )
{
	_gpsReader = lpReader;

}


//
// Initialize the port. This can be port 1 to 4.
//
BOOL CSerialPort::InitPort(CWnd* pPortOwner,	// the owner (CWnd) of the port (receives message)
						   UINT  portnr,		// portnumber (1..4)
						   UINT  baud,			// baudrate
						   char  parity,		// parity 
						   UINT  databits,		// databits 
						   UINT  stopbits,		// stopbits 
						   DWORD dwCommEvents,	// EV_RXCHAR, EV_CTS etc
						   UINT  writebuffersize)	// size to the writebuffer
{
	//assert(portnr > 0 && portnr < 5);


	// if the thread is alive: Kill
// 	if (m_bThreadAlive)
// 	{
// 		do
// 		{
// 			SetEvent(m_hShutdownEvent);
// 		} while (m_bThreadAlive);
// 		TRACE("Thread ended\n");
// 	}

	TRACE("Initialisation for communicationport %d completed.\nUse Startmonitor to communicate.\n", portnr);
	/////////////////////////////////////////////////////////////////////////////////////
	//InitializeCriticalSection(&m_csCommunicationSync);
	char comPort[256] = {0};
	sprintf(comPort, "\\\\.\\COM%d", portnr );//串口号


	m_hComm = CreateFileA( comPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL );

	if ( m_hComm == INVALID_HANDLE_VALUE )
	{
		//LeaveCriticalSection(&m_csCommunicationSync);
		return false;
	}

	//0-4=None,Odd,Even,Mark,Space
	int ip = 0;
	switch( parity )
	{

	case 'N':
		ip = 0;
		break;
	case 'O':
		ip = 1;
		break;
	case 'E':
		ip = 2;
		break;
	case 'M':
		ip = 3;
		break;
	case 'S':
		ip = 4;
		break;
	}


	DCB dcb;
	dcb.DCBlength = sizeof(DCB);
	GetCommState(m_hComm, &dcb);
	dcb.BaudRate = baud;
	dcb.fBinary = 1;
	dcb.fParity = FALSE;
	dcb.fNull = NULL;
	dcb.StopBits = stopbits;
	dcb.Parity = ip;
	dcb.ByteSize = databits;

	dcb.fBinary = TRUE;
	dcb.fDsrSensitivity = FALSE;
	dcb.fNull = FALSE;
	dcb.fAbortOnError = TRUE;

	dcb.fOutxCtsFlow = FALSE;
	dcb.fOutxDsrFlow = FALSE;
	dcb.fDtrControl = DTR_CONTROL_DISABLE;
	dcb.fOutX = FALSE;
	dcb.fInX = FALSE;
	dcb.fRtsControl = RTS_CONTROL_DISABLE;

	//SetCommState( m_hComm, &dcb );
	if ( !SetCommState( m_hComm, &dcb ) )//把dcb赋值给m_hComm指向的串口句柄  出错就是被占用或不存在
	{
		CloseHandle( m_hComm );
		m_hComm = INVALID_HANDLE_VALUE;
		//LeaveCriticalSection(&m_csCommunicationSync);
		return false;
	}
	//SetupComm(m_hComm, 1024, 1024);
	if ( !SetupComm(m_hComm, 1024, 1024) )//该函数初始化一个指定的通信设备的通信参数
	{
		CloseHandle( m_hComm );
		m_hComm = INVALID_HANDLE_VALUE;
		//LeaveCriticalSection(&m_csCommunicationSync);
		return false;
	}
	COMMTIMEOUTS commTimeOuts;
	::GetCommTimeouts( m_hComm, &commTimeOuts );
	commTimeOuts.ReadIntervalTimeout = 0;
	commTimeOuts.ReadTotalTimeoutMultiplier = 0;
	commTimeOuts.ReadTotalTimeoutConstant = 0;
	commTimeOuts.WriteTotalTimeoutMultiplier = 0;
	commTimeOuts.WriteTotalTimeoutConstant = 0;
	if ( !SetCommTimeouts( m_hComm, &commTimeOuts ) )//设置串口数据传输超时的时间阈值
	{
		CloseHandle( m_hComm );
		m_hComm = INVALID_HANDLE_VALUE;
		//LeaveCriticalSection(&m_csCommunicationSync);
		return false;
	}

	/////////////////////////////////////////////////////////////////////////////////////

	//LeaveCriticalSection(&m_csCommunicationSync);

	ConfigureSet *cfg = RadarManager::Instance()->getConfigureSet();
	m_nScanInterval = 1000/atoi( cfg->get("com", "frequency").c_str() );

	return TRUE;
}

//
//  The CommThread Function.
//
UINT CSerialPort::CommThread(LPVOID pParam)
{
	// Cast the void pointer passed to the thread back to
	// a pointer of CSerialPort class
	CSerialPort *port = (CSerialPort*)pParam;
	
	// Set the status variable in the dialog class to
	// TRUE to indicate the thread is running.
	//port->m_bThreadAlive = TRUE;	
		
	// Misc. variables
	DWORD BytesTransfered = 0; 
	DWORD Event = 0;
	DWORD CommEvent = 0;
	DWORD dwError = 0;
	COMSTAT comstat;
	memset(&comstat, 0, sizeof(COMSTAT));
	BOOL  bResult = TRUE;
		

	unsigned long cBytes, i;


	unsigned char buff[2048]={'\0'};
	// begin forever loop.  This loop will run as long as the thread is alive.
	//for (;;) 
	{ 
		DWORD lpErrors;
		COMSTAT lpStat;

		i = 0;

		if ( port->m_hComm == INVALID_HANDLE_VALUE)
		{
			return 0;
		}

		ClearCommError( port->m_hComm, &lpErrors, &lpStat );

		if ( lpErrors != 0 )
		{


		}

		if (port->m_hComm)
		{
			
		}

		if ( lpStat.cbInQue > 0 )
		{
			DWORD readData = 0;
			int x = 0;

			if ((lpStat.cbInQue) >= 2048)
			{
				readData = 2048;
			}
			else
			{
				readData = lpStat.cbInQue;
			}

			BOOL bReadState = ReadFile(port->m_hComm, buff, readData, &cBytes, 0);
 			for ( int i = 0; i < readData; i ++ )
 			{
 				TRACE("%c", buff[i]);
 			}
 			TRACE("\r\n");
			{
				port->_gpsReader->onReadData( buff, readData);
				port->_gpsReader->parseData();
			}
		

		} // end switch

		/*for(int i=0;i<m_nScanIntervalCount;i++){
			Sleep( 100 );
		}*/

		Sleep(port->m_nScanInterval);

	} // close forever loop

	return 0;
}

//
// start comm watching
//
BOOL CSerialPort::StartMonitoring()
{
// 	if (!(m_Thread = AfxBeginThread(CommThread, this)))
// 		return FALSE;
	if ( _lpThread )
	{
		_lpThread->cancel();
		delete _lpThread;
	}

	_lpThread = new ComThread ( this );
	_lpThread->start();
	TRACE("Thread started\n");
	return TRUE;	
}

//
// Restart the comm thread
//
BOOL CSerialPort::RestartMonitoring()
{
	//TRACE("Thread resumed\n");
	//m_Thread->ResumeThread();
	return TRUE;	
}


//
// Suspend the comm thread
//
BOOL CSerialPort::StopMonitoring()
{
	TRACE("Thread suspended\n");
	//m_Thread->SuspendThread(); 
	if ( _lpThread )
	{
		_lpThread->cancel();
		delete _lpThread;
		_lpThread = NULL;
	}


	ClosePort();
	return TRUE;	
}


//
// If there is a error, give the right message
//
void CSerialPort::ProcessErrorMessage(char* ErrorText)
{
	char *Temp = new char[200];
	
	LPVOID lpMsgBuf;

	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
	);

	sprintf(Temp, "WARNING:  %s Failed with the following error: \n%s\nPort: %d\n", (char*)ErrorText, lpMsgBuf, m_nPortNr); 
	MessageBoxA(NULL, Temp, "Application Error", MB_ICONSTOP);

	LocalFree(lpMsgBuf);
	delete [] Temp;
}

//
// Write a character.
//
void CSerialPort::WriteChar(CSerialPort* port)
{
	BOOL bWrite = TRUE;
	BOOL bResult = TRUE;

	DWORD BytesSent = 0;

	ResetEvent(port->m_hWriteEvent);

	// Gain ownership of the critical section
	//EnterCriticalSection(&port->m_csCommunicationSync);

	if (bWrite)
	{
		// Initailize variables
		port->m_ov.Offset = 0;
		port->m_ov.OffsetHigh = 0;

		// Clear buffer
		PurgeComm(port->m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

		bResult = WriteFile(port->m_hComm,							// Handle to COMM Port
							port->m_szWriteBuffer,					// Pointer to message buffer in calling finction
//							strlen((char*)port->m_szWriteBuffer),	// Length of message to send
							port->m_nWriteSize,	// Length of message to send
							&BytesSent,								// Where to store the number of bytes sent
							&port->m_ov);							// Overlapped structure

		// deal with any error codes
		if (!bResult)  
		{
			DWORD dwError = GetLastError();
			switch (dwError)
			{
				case ERROR_IO_PENDING:
					{
						// continue to GetOverlappedResults()
						BytesSent = 0;
						bWrite = FALSE;
						break;
					}
				default:
					{
						// all other error codes
						port->ProcessErrorMessage("WriteFile()");
					}
			}
		} 
		else
		{
			//LeaveCriticalSection(&port->m_csCommunicationSync);
		}
	} // end if(bWrite)

	if (!bWrite)
	{
		bWrite = TRUE;
	
		bResult = GetOverlappedResult(port->m_hComm,	// Handle to COMM port 
									  &port->m_ov,		// Overlapped structure
									  &BytesSent,		// Stores number of bytes sent
									  TRUE); 			// Wait flag

		//LeaveCriticalSection(&port->m_csCommunicationSync);

		// deal with the error code 
//		if (!bResult)  
		{
//			port->ProcessErrorMessage("GetOverlappedResults() in WriteFile()");
		}	
	} // end if (!bWrite)

	//Verify that the data size send equals what we tried to send
	if (BytesSent != port->m_nWriteSize)	// Length of message to send)
	{
		TRACE("WARNING: WriteFile() error.. Bytes Sent: %d; Message Length: %d\n", BytesSent, strlen((char*)port->m_szWriteBuffer));
	}
//	::SendMessage((port->m_pOwner)->m_hWnd, WM_COMM_TXEMPTY_DETECTED, (WPARAM) RXBuff, (LPARAM) port->m_nPortNr);
//	::SendMessage((port->m_pOwner)->m_hWnd, WM_COMM_TXEMPTY_DETECTED,0,(LPARAM) port->m_nPortNr);
}

//
// Character received. Inform the owner
//
void CSerialPort::ReceiveChar(CSerialPort* port, COMSTAT comstat)
{
	BOOL  bRead = TRUE; 
	BOOL  bResult = TRUE;
	DWORD dwError = 0;
	DWORD BytesRead = 0;
	unsigned char RXBuff[256];

	for (;;) 
	{ 
		// Gain ownership of the comm port critical section.
		// This process guarantees no other part of this program 
		// is using the port object. 
		
		//EnterCriticalSection(&port->m_csCommunicationSync);

		// ClearCommError() will update the COMSTAT structure and
		// clear any other errors.
		
		bResult = ClearCommError(port->m_hComm, &dwError, &comstat);

		//LeaveCriticalSection(&port->m_csCommunicationSync);

		// start forever loop.  I use this type of loop because I
		// do not know at runtime how many loops this will have to
		// run. My solution is to start a forever loop and to
		// break out of it when I have processed all of the
		// data available.  Be careful with this approach and
		// be sure your loop will exit.
		// My reasons for this are not as clear in this sample 
		// as it is in my production code, but I have found this 
		// solutiion to be the most efficient way to do this.
		
		if (comstat.cbInQue == 0)
		{
			// break out when all bytes have been read
			break;
		}
						
		//EnterCriticalSection(&port->m_csCommunicationSync);

		if (bRead)
		{
			bResult = ReadFile(port->m_hComm,		// Handle to COMM port 
							   RXBuff,				// RX Buffer Pointer
							   sizeof( RXBuff),					// Read one byte
							   &BytesRead,			// Stores number of bytes read
							   &port->m_ov);		// pointer to the m_ov structure
			// deal with the error code 
			if (!bResult)  
			{ 
				switch (dwError = GetLastError()) 
				{ 
					case ERROR_IO_PENDING: 	
						{ 
							// asynchronous i/o is still in progress 
							// Proceed on to GetOverlappedResults();
							bRead = FALSE;
							break;
						}
					default:
						{
							// Another error has occured.  Process this error.
							port->ProcessErrorMessage("ReadFile()");
							break;
						} 
				}
			}
			else
			{
				// ReadFile() returned complete. It is not necessary to call GetOverlappedResults()
				bRead = TRUE;
			}
		}  // close if (bRead)

		if (!bRead)
		{
			bRead = TRUE;
			bResult = GetOverlappedResult(port->m_hComm,	// Handle to COMM port 
										  &port->m_ov,		// Overlapped structure
										  &BytesRead,		// Stores number of bytes read
										  TRUE); 			// Wait flag

			// deal with the error code 
			if (!bResult)  
			{
				port->ProcessErrorMessage("GetOverlappedResults() in ReadFile()");
			}	
		}  // close if (!bRead)
				
		//LeaveCriticalSection(&port->m_csCommunicationSync);

		// notify parent that a byte was received
		// 不需要通道父窗口了, 在这里直接处理.
		if ( port->m_pOwner )
		{
			::SendMessage((port->m_pOwner)->m_hWnd, WM_COMM_RXCHAR, (WPARAM) RXBuff, (LPARAM) port->m_nPortNr);
		}

		if ( port->_gpsReader )
		{

			port->_gpsReader->onReadData( RXBuff, BytesRead );
		}

	} // end forever loop

}

//
// Write a string to the port
//
void CSerialPort::WriteToPort(char* string)
{		
	assert(m_hComm != 0);
	
	memset(m_szWriteBuffer, 0, sizeof(m_szWriteBuffer));
	strcpy(m_szWriteBuffer, string);
	m_nWriteSize=strlen(string);

	// set event for write
	SetEvent(m_hWriteEvent);
}


void CSerialPort::WriteToPort(char* string,int n)
{		
	assert(m_hComm != 0);

	memset(m_szWriteBuffer, 0, sizeof(m_szWriteBuffer));
//	memset(m_szWriteBuffer, 0, n);
//	strncpy(m_szWriteBuffer, string, n);
	memcpy(m_szWriteBuffer, string, n);
	m_nWriteSize=n;

	// set event for write
	SetEvent(m_hWriteEvent);
}

void CSerialPort::WriteToPort(CString string)
{		
	assert(m_hComm != 0);

	memset(m_szWriteBuffer, 0, sizeof(m_szWriteBuffer));
	//strcpy(m_szWriteBuffer, string);
	int i=0;
	for(i=0;i<string.GetLength();i++)
	{
		m_szWriteBuffer[i]=string.GetAt(i);
	}
	m_nWriteSize=string.GetLength();

	// set event for write
	SetEvent(m_hWriteEvent);
}

void CSerialPort::WriteToPort(LPCTSTR string,int n)
{		
	assert(m_hComm != 0);

	memset(m_szWriteBuffer, 0, sizeof(m_szWriteBuffer));
//	strncpy(m_szWriteBuffer, string, n);
	memcpy(m_szWriteBuffer, string, n);
	m_nWriteSize=n;
	// set event for write
	SetEvent(m_hWriteEvent);
}

//
// Return the device control block
//
DCB CSerialPort::GetDCB()
{
	return m_dcb;
}

//
// Return the communication event masks
//
DWORD CSerialPort::GetCommEvents()
{
	return m_dwCommEvents;
}

//
// Return the output buffer size
//
DWORD CSerialPort::GetWriteBufferSize()
{
	return m_nWriteBufferSize;
}


void CSerialPort::ClosePort()
{
	if ( m_hComm == INVALID_HANDLE_VALUE )
	{
		return;
	}
	CloseHandle( m_hComm );

	SetEvent(m_hShutdownEvent);
	// Close Handles  
	if(m_hShutdownEvent!=NULL)
		CloseHandle( m_hShutdownEvent); 
	if(m_ov.hEvent!=NULL)
		CloseHandle( m_ov.hEvent ); 
	if(m_hWriteEvent!=NULL)
		CloseHandle( m_hWriteEvent ); 


	m_hComm = INVALID_HANDLE_VALUE;

	TRACE("Thread ended\n");
	if ( m_szWriteBuffer )
	{
		delete [] m_szWriteBuffer;
		m_szWriteBuffer = NULL;
	}

}

/*
void CSerialPort::ClosePort()
{
	do
	{
		SetEvent(m_hShutdownEvent);
	} while (m_bThreadAlive);

	
	// if the port is still opened: close it 
	if (m_hComm != NULL)
	{
		CloseHandle(m_hComm);
		m_hComm = NULL;
	}
	// Close Handles  
	if(m_hShutdownEvent!=NULL)
		CloseHandle( m_hShutdownEvent); 
	if(m_ov.hEvent!=NULL)
		CloseHandle( m_ov.hEvent ); 
	if(m_hWriteEvent!=NULL)
		CloseHandle( m_hWriteEvent ); 

	TRACE("Thread ended\n");
	delete [] m_szWriteBuffer;
}

*/