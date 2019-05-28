#pragma once
#include "library.h"
#include "ftpserver.h"
#include "ConnectSocket.h"


DWORD WINAPI InitInstance(LPVOID pParam);
DWORD WINAPI ExitInstance(LPVOID pParam);

class ftpserver;
class ConnectThread
{
public:
	ftpserver* Server;	
	ConnectSocket a_ConnectSocket;
	DWORD threadID;
    HANDLE thread;
	ConnectThread(void);
	~ConnectThread(void);
	int ReceivedBytes;
	int SentBytes;
	void IncReceivedBytes(int nBytes);
	void IncSentBytes(int nBytes);
	void UpdateStatistic(int nType);	
};

