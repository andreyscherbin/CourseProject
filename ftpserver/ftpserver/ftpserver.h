#ifndef FTPSERVER_H
#define FTPSERVER_H
#pragma once
#include "library.h"
#include "ListenSocket.h"
#include "UserManager.h"
#include "ConnectThread.h"

class ConnectThread;
class ftpserver
{    
public:
	UserManager m_UserManager;
	ListenSocket m_ListenSocket;
	int nclients;
	list<ConnectThread*> ThreadList;
	HANDLE ThreadExitSemaphore;
	HANDLE ThreadExitEvent;

	int	Timeout;
	ftpserver(void);
	~ftpserver(void);
	bool start();
	int GetTimeout() { return Timeout; }

	int		Port;
	int		MaxUsers;
	string	WelcomeMessage;
	string	GoodbyeMessage;	
	BOOL	Running;

	DWORD TotalReceivedBytes;
	DWORD TotalSentBytes;
	int	ConnectionCount;
	int TotalConnections;
	int FilesDownloaded;
	int FilesUploaded;
	int FailedDownloads;
	int FailedUploads;

	
	ConnectThread *exitThread;

	DWORD ThreadForCheckEventsID;
	HANDLE ThreadForCheckEvents;

	static DWORD WINAPI FunctionForCheckEvents(LPVOID pParam);

	string GetWelcomeMessage() { return WelcomeMessage; };
	string GetGoodbyeMessage() { return GoodbyeMessage; };
	void setNumberExitThread(ConnectThread *exitThread);
	void incNumClients();
	void decNumClients();
	int getNumClients();
};

#endif // FTPSERVER_H
