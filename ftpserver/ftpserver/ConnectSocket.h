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
	void getArgsFromCommand(string &command, string &args);
	ftpserver * server;
	ConnectThread *a_Thread;

	string UserName;

	BOOL SendResponse(char*);

	void OnClose();	
	void OnReceive(int numberThread);

protected:	
	void ParseCommand(string,int);

	string command;	
	char currentDirectory[MAX_SIZE_STRING];
};

