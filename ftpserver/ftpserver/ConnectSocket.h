#pragma once
#include "library.h"
class DataSocket;
class ftpserver;
class ConnectThread;
class ConnectSocket
{
public:
	DataSocket *a_DataSocket;
	SOCKET clientsocket;
	int PassiveMode;
	int RemotePort;
	string RemoteHost;

public:
	ConnectSocket(void);
	 ~ConnectSocket(void);

	 BOOL CreateDataConnection(int nTransferType,vector <string>);
	void DestroyDataConnection();

	ftpserver * server;
	ConnectThread *a_Thread;

	BOOL Loggedon;
	string UserName;
	
	int OnReceive(void);

	protected:
	//CStringList m_strCommands;
	//void GetRxLine();
	BOOL RenameFile;
	DWORD RestartOffset;
	//CString m_strRenameFile;
	//CString m_RxBuffer;
	char currentDirectory[MAX_SIZE_STRING];
};

