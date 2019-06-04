#pragma once
#include "library.h"
class ConnectSocket;

#define MODE_IDLE	0
#define MODE_LIST	1
#define MODE_SEND	2
#define MODE_RECEIVE 3
#define MODE_ERROR	4

class DataSocket
{
public:
	DataSocket(ConnectSocket *pSocket, int nTransferType = 0);	
	~DataSocket(void);
	SOCKET DATASOCKET; 
	SOCKET LISTENSOCKET;

public:
	void SetData(vector <string> listString);
	void SetTransferType(int nType, BOOL bWaitForAccept = FALSE);
	void Send();
	void Receive();
	void Close();
	void Listen();
	bool Create();
	bool Accept();
	bool Connect(string RemoteHost,int RemotePort);
	void OnConnect();
	bool getSockInfo(int& socket,string& resultAdress);
	int GetStatus();

protected:
	DWORD RestartOffset;
	BOOL Connected;
	BOOL Initialized;	
	int TransferType;
	vector <string> listData;
	int Status;
	ConnectSocket *m_ConnectSocket;
};

