#pragma once
#include "library.h"
#include "ftpserver.h"
#include "ConnectSocket.h"

class ftpserver;
class ConnectThread
{
public:
	ftpserver* Server;	
	ConnectSocket a_ConnectSocket;
	DWORD threadID;
    HANDLE thread;	
	int numberThread;

	ConnectThread(int number);
	~ConnectThread(void);	
	int ReceivedBytes;
	int SentBytes;
	void IncReceivedBytes(int nBytes);
	void IncSentBytes(int nBytes);
	void UpdateStatistic(int nType);

	static DWORD WINAPI StartThread(LPVOID pParam);
	bool ExitThread();
};

