#ifndef FTPSERVER_H
#define FTPSERVER_H
#pragma once
#include "library.h"
#include "ListenSocket.h"
#include "UserManager.h"
#include "ConnectThread.h"

class ftpserver
{

public:
     ListenSocket m_ListenSocket;
public:
	UserManager m_UserManager;
	int nclients;
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

	string GetWelcomeMessage() { return WelcomeMessage; };
	string GetGoodbyeMessage() { return GoodbyeMessage; };
};

#endif // FTPSERVER_H
