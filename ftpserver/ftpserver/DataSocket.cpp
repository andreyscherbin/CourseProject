#include "library.h"
#include "DataSocket.h"
#include "ConnectSocket.h"
#include "ConnectThread.h"

DataSocket::DataSocket(ConnectSocket *pSocket, int nTransferType)
{
	TransferType = nTransferType;
	m_ConnectSocket = pSocket;
	Status = MODE_IDLE;            		
	Connected = FALSE;
	RestartOffset = 0;
	Initialized = FALSE;
}

DataSocket::~DataSocket(void)
{
	Connected = FALSE;	
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

void DataSocket::Listen()
{	
    struct sockaddr_in address;    
    int addrlen = sizeof(address); 

	address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons(0);       
    
    if ((LISTENSOCKET = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 

	if (bind(LISTENSOCKET, (struct sockaddr*) &address, sizeof(struct sockaddr_in)) != 0)    
        printf("Unable to bind ANY PORT\n");

	if (listen(LISTENSOCKET, 3) < 0) 
    { 
        perror("LISTEN SERVER"); 
        exit(EXIT_FAILURE); 
    }
	else		

	 if (getsockname(LISTENSOCKET, (struct sockaddr *)&address, & addrlen) == -1){
           perror("getsockname");		 
	  }
}

bool DataSocket::Accept()
{	
	struct sockaddr_in address;    
    int addrlen = sizeof(address); 

	if ((DATASOCKET = accept(LISTENSOCKET, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0) 
    { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    } 

	Connected = TRUE;

	if (!Initialized)
		SetTransferType(TransferType);
	return true;
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

	if (bind(DATASOCKET, (struct sockaddr*) &my_addr1, sizeof(struct sockaddr_in)) != 0) 
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
	  OnConnect();   
	  return true;
	}
}

void DataSocket::OnConnect() 
{	
		switch (TransferType)
		{
			case 0:	
				Status = MODE_LIST;
				Connected = TRUE;
				Send();
				Close();
				break;
			case 1:							
				Status = MODE_SEND;
				Connected = TRUE;
				Send();
				Close();				
				break;
			case 2:					
				Status = MODE_RECEIVE;
				Connected = TRUE;
				Receive();
				Close();							
				break;
		}		
}

void DataSocket::SetData(vector <string> list)
{
	listData = list;	
}

bool DataSocket::getSockInfo(int &port,string &address)
{
	struct sockaddr_in sin;
    socklen_t len = sizeof(sin);
     if (getsockname(LISTENSOCKET, (struct sockaddr *)&sin, &len) == -1){
           perror("getsockname");
	  }	          
     port = ntohs(sin.sin_port);
	return true;
}

void DataSocket::Send() 
{	
	switch(Status)
	{
		case MODE_LIST:
		{
			for(int i=0;i<this->listData.size();i++)
			{
			send(DATASOCKET,listData[i].c_str(),strlen(listData[i].c_str()),0);
			}					
			this->m_ConnectSocket->SendResponse("226 The directory listing send OK.");
			this->m_ConnectSocket->printfResponse("226 The directory listing send OK\n","");
	        
		}break;

		case MODE_SEND:
		{  
		FILE*fp;
		char buffer[MAXLINE],char_num_blks[MAXLINE],char_num_last_blk[MAXLINE];
	   int lSize,num_blks,num_last_blk,i;

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
				if (GetLastError() == WSAEWOULDBLOCK) 
					{
						Sleep(0);						
					}
			}
		}
		sprintf(char_num_last_blk,"%d",num_last_blk);	
		if (num_last_blk > 0) { 
			fread (buffer,sizeof(char),num_last_blk,fp);
			send(DATASOCKET,buffer,num_last_blk, 0);			
		}
		fclose(fp);		
		this->m_ConnectSocket->SendResponse("226 End download file.");
		this->m_ConnectSocket->printfResponse("226 End download file\n","");
		}break;
	}	
}

void DataSocket::Receive()
{
	int readed;
	char buffer[MAXLINE];
	FILE *fp;

	if (Status == MODE_RECEIVE)
	{

	if((fp=fopen(listData[0].c_str(),"wb"))==NULL){
			cout<<"Error in creating file name = " << listData[0].c_str() << endl;				
	}
	else
		{	
			do{
			readed = recv(DATASOCKET, buffer, MAXLINE,0);
			fwrite(buffer,sizeof(char),readed,fp);		
			}
			while(readed);			
			fclose(fp);			
			m_ConnectSocket->SendResponse("226 Transfer complete");
			this->m_ConnectSocket->printfResponse("226 Transfer complete\n","");
			
		}
	}
	
	return;
}

void DataSocket::Close() 
{
	shutdown(DATASOCKET,SD_SEND);
	shutdown(LISTENSOCKET,SD_SEND);
	closesocket(DATASOCKET);
	closesocket(LISTENSOCKET);
}

int DataSocket::GetStatus()
{
	return Status;
}

void DataSocket::SetTransferType(int nType, BOOL bWaitForAccept)
{
	TransferType = nType; 

	if (bWaitForAccept && !Connected)
	{
		Initialized = FALSE;
		return;
	}
	Initialized = TRUE;

	switch(TransferType)
	{
		case 0:	
			Status = MODE_LIST;
			Send();
			Close();
			break;
		case 1:				
			Status = MODE_SEND;
			Connected = TRUE;
			Send();	
			Close();			
			break;
		case 2:	
			Status = MODE_RECEIVE;
			Connected = TRUE;
			Receive();
			Close();
			break;
		default:
			Initialized = FALSE;
			break;
	}
}




