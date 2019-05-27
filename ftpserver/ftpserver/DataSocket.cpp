#include "library.h"
#include "DataSocket.h"
#include "ConnectSocket.h"
#include "ConnectThread.h"

DataSocket::DataSocket(ConnectSocket *pSocket, int nTransferType)
{
	TransferType = nTransferType;
	m_ConnectSocket = pSocket;
	Status = nTransferType;            // для упрощения сделал повторение TransferType и Status 		
	Connected = FALSE;
	RestartOffset = 0;
	Initialized = FALSE;
}


DataSocket::~DataSocket(void)
{
	Connected = FALSE;
	printf("DataSocket destroyed.\n");
}

bool DataSocket ::Create()
{  
	DATASOCKET = socket(AF_INET, SOCK_STREAM, 0);
	if(DATASOCKET == INVALID_SOCKET)
	{
	  return false;	
	}
	else
	{
	  return true;
	}
}
bool DataSocket ::Connect(string RemoteHost,int RemotePort)
{
	cout << RemoteHost << " " << RemotePort << endl;
	struct sockaddr_in anAddr;
    anAddr.sin_family = AF_INET;
    anAddr.sin_port = htons (RemotePort);
	anAddr.sin_addr.S_un.S_addr = inet_addr(RemoteHost.c_str());
    if((connect(DATASOCKET, (struct sockaddr *)&anAddr, sizeof(struct sockaddr))) == SOCKET_ERROR)
	{
		printf("CONNECT DATASOCKET ERROR\n");
		return false;
	}
	else
	{
	return true;
	}
}

void DataSocket::SetData(vector <string> list)
{
	listData = list;

	
	//m_nTotalBytesSend = m_strData.GetLength();
	//m_nTotalBytesTransfered = 0;
}

void DataSocket::OnSend() 
{
  cout << "STATUS RAVEN = " << Status << endl;
	
	switch(Status)
	{
		case MODE_LIST:
		{
			for(int i=0;i<this->listData.size();i++)
			{
			send(DATASOCKET,listData[i].c_str(),strlen(listData[i].c_str()),0);
			}
			OnClose();
			send(this->m_ConnectSocket->clientsocket,"226 The directory listing send OK.",sizeof("226 The directory listing send OK."),0 );	  
	        
	        //closesocket(DATASOCKET);
		}break;

		case MODE_SEND:
		{  
		FILE*fp;
		char port[MAXLINE],buffer[MAXLINE],char_num_blks[MAXLINE],char_num_last_blk[MAXLINE];
	   int datasock,lSize,num_blks,num_last_blk,i;

		if ((fp = fopen(listData[0].c_str(), "rb"))==NULL) {
		printf("CURRENT DIR IS = %s\n",getcwd(NULL,MAX_SIZE_STRING));
         printf("CANNOT OPEN FILE NAME = %s.\n",listData[0].c_str());
	   }
     
	    fseek (fp , 0 , SEEK_END);
		lSize = ftell (fp);
		rewind (fp);
		num_blks = lSize/MAXLINE;		
		num_last_blk = lSize%MAXLINE; 	
		sprintf(char_num_blks,"%d",num_blks);		
		for(i= 0; i < num_blks; i++) { 
			fread (buffer,sizeof(char),MAXLINE,fp);			
			if((send(DATASOCKET,buffer,MAXLINE, 0)) == SOCKET_ERROR)
				printf("SOCKET_ERROR\n");
		}
		sprintf(char_num_last_blk,"%d",num_last_blk);	
		if (num_last_blk > 0) { 
			fread (buffer,sizeof(char),num_last_blk,fp);
			send(DATASOCKET,buffer,num_last_blk, 0);			
		}
		fclose(fp);

		OnClose();
		send(this->m_ConnectSocket->clientsocket,"226 End download file.",sizeof("226 End download file."),0);			
		}break;
	}	
}


void DataSocket::OnClose() 
{
	shutdown(DATASOCKET,SD_SEND);
	closesocket(DATASOCKET);
}

