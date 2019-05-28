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
	struct sockaddr_in my_addr, my_addr1; 
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons (RemotePort);
	my_addr.sin_addr.S_un.S_addr = inet_addr(RemoteHost.c_str());

	my_addr1.sin_family = AF_INET; 
    my_addr1.sin_addr.s_addr = INADDR_ANY; 
    my_addr1.sin_port = htons(DEFAULT_DATA_PORT);         // binding socket to 20 port 

	if (bind(DATASOCKET, (struct sockaddr*) &my_addr1, sizeof(struct sockaddr_in)) == 0) 
        printf("Binded Correctly to 20 PORT\n"); 
    else
        printf("Unable to bind 20 PORT\n");

    if((connect(DATASOCKET, (struct sockaddr *)&my_addr, sizeof(struct sockaddr))) == SOCKET_ERROR)
	{
		printf("Error in Connection\n");
		return false;
	}
	else
	{
	  struct sockaddr_in sin;
      socklen_t len = sizeof(sin);
      if (getsockname(DATASOCKET, (struct sockaddr *)&sin, &len) == -1){
           perror("getsockname");
	  }
         else{
          printf("server data port %d\n", ntohs(sin.sin_port));
		 }
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
			this->m_ConnectSocket->SendResponse("226 The directory listing send OK.");
	        
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
			{
				printf("DATA SOCKET_ERROR\n");
				break;
			}
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

