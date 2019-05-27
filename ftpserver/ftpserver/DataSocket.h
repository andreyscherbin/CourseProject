#pragma once
#include "library.h"
class ConnectSocket;

#define MODE_IDLE	4
#define MODE_LIST	0
#define MODE_SEND	1
#define MODE_RECEIVE 2
#define MODE_ERROR	3

class DataSocket
{
public:
	DataSocket(ConnectSocket *pSocket, int nTransferType = 0);	
	~DataSocket(void);
	bool Create();
	bool Connect(string RemoteHost,int RemotePort);

	SOCKET DATASOCKET; 
public:
	void SetData(vector <string>);
	void OnSend();
	void OnClose();

protected:
	DWORD RestartOffset;
	BOOL Connected;
	BOOL Initialized;
	//int Receive();
	//BOOL PrepareReceiveFile(LPCTSTR lpszFilename);
	//BOOL PrepareSendFile(LPCTSTR lpszFilename);
	DWORD TotalBytesTransfered;
	DWORD TotalBytesReceive;
	DWORD TotalBytesSend;
	int TransferType;
	vector <string> listData;
	int Status;
	ConnectSocket *m_ConnectSocket;
};

