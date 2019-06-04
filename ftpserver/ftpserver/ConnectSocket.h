#pragma once
#include "library.h"
class DataSocket;
class ftpserver;
class ConnectThread;
class ConnectSocket
{
	enum 
	{
		TOK_ABOR, TOK_BYE, TOK_CDUP, TOK_CWD,
		TOK_DELE, TOK_DIR, TOK_HELP, TOK_LIST,
		TOK_MKD, TOK_NOOP, TOK_PASS, TOK_PASV, 
		TOK_PORT, TOK_PWD, TOK_QUIT,TOK_RETR,
		TOK_RMD, TOK_RNFR, TOK_RNTO, 
		TOK_STOR, TOK_SYST, TOK_TYPE, 
		TOK_USER, TOK_ERROR,
	};

public:
	DataSocket *a_DataSocket;
	SOCKET CLIENTSOCKET;
	int PassiveMode;
	int RemotePort;
	string RemoteHost;
	string getRemoteHost();
	void printfResponse(string response,string args);

	struct FTPCommand
	{
		int TokenID;
		char *Name;
		BOOL HasArguments;
		char *Description;
	};

public:
	ConnectSocket(void);
	 ~ConnectSocket(void);
	 BOOL CreateDataConnection(int nTransferType,vector <string> result);
	void DestroyDataConnection();
	BOOL getArgsFromCommand(string &command, string &args);
	ftpserver * server;
	ConnectThread *a_Thread;
	BOOL Loggedon;
	string getLoggedon();
	string UserName;
	

	BOOL SendResponse(char*);

	void OnClose();	
	void OnReceive(int numberThread);

public:	
	void ParseCommand(string,int);
	
	string command;	
	string currentDir;
	string renamefile;
};

