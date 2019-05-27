#include "ConnectSocket.h"
#include "library.h"
#include "ftpserver.h"
#include "ConnectSocket.h"
#include "ConnectThread.h"
#include "DataSocket.h"

ConnectSocket::ConnectSocket(void)
{
	Loggedon = FALSE;
	RenameFile = FALSE;
	a_DataSocket = NULL;
	RemoteHost = "";
	RemotePort = -1;
	RestartOffset = 0;
	PassiveMode = FALSE;
	clientsocket = INVALID_SOCKET;

	strcpy(currentDirectory,"D:/usb");
}


ConnectSocket::~ConnectSocket(void)
{
}

int ConnectSocket::OnReceive(void)   // ПОТОК-НОВОЙ КЛИЕНТ ( ТОТ ЖЕ САМЫЙ)
   {

int global = 1;
int iRecv;    
struct addrinfo *result = NULL;
struct addrinfo hints;
int iSendResult;
int iResult;
  	
//HANDLE mutexPASV = CreateMutex(NULL,FALSE,"mutex PASV");	

int bytes = 0;

	   //ReleaseMutex(mutexPASV);
	char workingDirectory[MAX_SIZE_STRING] = "D:/usb";  	
	char oldfilename[MAX_SIZE_STRING] = "oldfilename.txt";                   // старое имя файла
    char newfilename[MAX_SIZE_STRING] = "newfilename.txt";                   // новое имя файла
	char deletefilename[MAX_SIZE_STRING]="deletefilename";
	char newdirectoryname[MAX_SIZE_STRING]="newdirectoryname";
	char createnewfilename[MAX_SIZE_STRING]="createnewfilename";
	char downloadfilename[DEFAULT_BUFLEN]="downloadfilename";
	char responseHELLO[DEFAULT_BUFLEN] = "220 Andrey Scherbin FTP-Server";
	char responseOK[DEFAULT_BUFLEN]="220 OK";
	char responseUSER[DEFAULT_BUFLEN] = "331 Anonymous login ok, send your complete email address as your password ";
	char responsePASS[DEFAULT_BUFLEN] = "230 Logged in anonymously";	
    char responsePWD[DEFAULT_BUFLEN] =  "";
	char responseSYST[DEFAULT_BUFLEN] = "215 WIN";
	char responseFEAT[DEFAULT_BUFLEN] = "211 Extensions supported";
	char responseTYPE[DEFAULT_BUFLEN] = "200 Switching to Binary mode";
	char responsePASV[DEFAULT_BUFLEN] = "227 Entering Passive Mode (172,20,10,5,0,20).";
	char responsePORT[DEFAULT_BUFLEN] = "200 Entering Active Mode .";
	char responseLIST1[DEFAULT_BUFLEN] = "150 Here comes the directory listing."; 
	char responseLIST2[DEFAULT_BUFLEN] = "226 The directory listing send OK.";   
	char responseDELE[DEFAULT_BUFLEN] =  "250 file delete successfully.";    
	char responseRMD[DEFAULT_BUFLEN] =  "250 directory delete successfully.";
	char responseCWD[DEFAULT_BUFLEN] =  "250 go to directory success.";
	char responseCWDerror[DEFAULT_BUFLEN] =  "550 No such file or directory..";
	char responseMKD[DEFAULT_BUFLEN] =  "257 create directory successfully.";
	char responseQUIT[DEFAULT_BUFLEN] =  "221 quit successfully.";
	char responseRETR1[DEFAULT_BUFLEN] = "150 Starting download file."; 
	char responseRETR2[DEFAULT_BUFLEN] = "226 End download file.";   
	char responseSTOR1[DEFAULT_BUFLEN] = "150 Starting upload file."; 
	char responseSTOR2[DEFAULT_BUFLEN] = "226 End upload file.";
	char responseABOR1[DEFAULT_BUFLEN] = "426 Starting cancel transfer."; 
	char responseABOR2[DEFAULT_BUFLEN] = "226 End cancel transfer.";
	char responseRNFR[DEFAULT_BUFLEN] =  "350 Chose file to rename success.";
	char responseRNTO[DEFAULT_BUFLEN] =  "350 Rename file success.";
	char responseCDUP[DEFAULT_BUFLEN] =  "250 CDUP command successful";
	char responseREST[DEFAULT_BUFLEN] =  "350 Restarting";
	


	   char command[5];
	   char recvbuf[DEFAULT_BUFLEN];
	   char sendvbuf[DEFAULT_BUFLEN];
       int recvbuflen = DEFAULT_BUFLEN;
	   int sendvbuflen = DEFAULT_BUFLEN;
	   SOCKET DataPasvSocket = INVALID_SOCKET;
       SOCKET ListenDataPasvSocket = INVALID_SOCKET;
	   SOCKET DataPortSocket = INVALID_SOCKET;
	  
	 
	   int index = global;
	   global++;      
	   printf("Accept client succesfull %d socket = %d \n",index,clientsocket);                                         // Приглашение клиенту
	   //send(clientsocket,responseHELLO,sizeof(responseHELLO),0 );
	
		do {
			
			iRecv = recv(clientsocket,recvbuf, recvbuflen, 0);		  
		    sscanf(recvbuf, "%s", command);	
			string msg = "";
	        for(int x = 0; x < iRecv; x++)
	         {
	          msg.push_back(recvbuf[x]);
	          }	

		if(!strcmp(command, "USER"))
	{
	  printf("USER %d\n",index);
	  send(clientsocket,responseUSER,sizeof(responseUSER),0 );	 
	}
      if(!strcmp(command, "REST"))
	{
	  printf("REST %d\n",index);
	  send(clientsocket,responseREST,sizeof(responseREST),0 );	 
	}

     if(!strcmp(command, "CDUP"))
	 {
	   int nResult = server->m_UserManager.ChangeDirectoryCDUP(currentDirectory);
	  switch(nResult)
			{				
				case 1:
					send(clientsocket,responseCDUP,sizeof(responseUSER),0);
					break;
				default:
					send(clientsocket,responseCDUP,sizeof(responseUSER),0);
					break;
			}
	 }
		if(!strcmp(command, "PASS"))
	{
	  printf("PASS %d\n",index);	  
	  send(clientsocket,responsePASS,sizeof(responsePASS),0 );
	}
		if(!strcmp(command, "PWD"))
	{
	   printf("PWD %d\n",index);
	   responsePWD[0] = 0;
	   string a  = "257 ";
	   a  = a + "\"" + currentDirectory + "\"" + " is current directory";	  
	   strcat(responsePWD,a.c_str());
	   send(clientsocket,responsePWD,strlen(responsePWD)+1,0 );
	   
	}
		if(!strcmp(command, "SYST"))
	{
	  printf("SYST\n");	  
	   send(clientsocket,responseSYST,sizeof(responseSYST),0 );
	}	

		if(!strcmp(command, "FEAT"))
	{
	  printf("FEAT\n");	  
	  send(clientsocket,responseFEAT,sizeof(responseFEAT),0 );
	}	
		if(!strcmp(command, "TYPE"))
	{
	  printf("TYPE %d\n",index);	  
	   send(clientsocket,responseTYPE,sizeof(responseTYPE),0 );
	}
		if(!strcmp(command, "DELE"))
	{
	  printf("DELE\n");
	  memset(deletefilename, '\0',MAX_SIZE_STRING);
	  memcpy(deletefilename,recvbuf+5,MAX_SIZE_STRING);
	  int i;
	  for(i=0;deletefilename[i]!='\n';i++)
	  {}
	  deletefilename[i-1]='\0';	 
	  if(remove(deletefilename))
	  {
     perror("delete file error:");
	  }     
	   send(clientsocket,responseDELE,sizeof(responseDELE),0 );
	}
     if(!strcmp(command, "RMD"))
	{
	  printf("RMD\n");	
	   send(clientsocket,responseRMD,sizeof(responseRMD),0 );
	}
	 if(!strcmp(command, "CWD"))
	{	
		printf("CWD %d\n",index);
       int nResult = server->m_UserManager.ChangeDirectory(currentDirectory,recvbuf);
	  switch(nResult)
			{				
				case 1:
					send(clientsocket,responseCWD,sizeof(responseCWD),0 );
					break;
				default:
					send(clientsocket,responseCWD,sizeof(responseCWD),0 );
					break;
			}

	}
	 if(!strcmp(command, "MKD"))
	{
	  printf("MKD\n");	  
	  memset(newdirectoryname, '\0',MAX_SIZE_STRING);
	  memcpy(newdirectoryname,recvbuf+4,MAX_SIZE_STRING);
	  int i;
	  for(i=0;newdirectoryname[i]!='\n';i++)
	  {}
	  newdirectoryname[i-1]='\0';	 
	  if(mkdir(newdirectoryname))
	  {      
      printf("Error create new directory\n");
	  } 
	  send(clientsocket,responseMKD,sizeof(responseMKD),0 );
	}
	  if(!strcmp(command, "QUIT"))
	{
	   printf("QUIT\n");	   
	   send(clientsocket,responseQUIT,sizeof(responseQUIT),0 );
	   return 0;
	}

	  if(msg.substr(0,4) == "PORT")
	{
	   printf("PORT\n");  
	   int c2 = msg.rfind(",");
	   int c1 = msg.rfind(",",c2-1);	 
	   int portNumber = atoi(msg.substr(c1+1,c2-c1).c_str()) * 256;
	   portNumber += atoi(msg.substr(c2+1).c_str());      
	   string address = msg.substr(5,c1-5);
	   for(int i = 0; i < address.size(); i++)
		{
		  if(address.substr(i,1) == ",")
		     {
		      address.replace(i,1,".");
		     }
		 }
	   this->PassiveMode=FALSE;
	   this->RemotePort=portNumber;
	   this->RemoteHost=address;

	  send(clientsocket,responsePORT,sizeof(responsePORT),0 );	   
	}

	    if(!strcmp(command, "RETR"))
	{
	   printf("RETR %d\n",index);	   
	   vector <string> list;
	   char result[DEFAULT_BUFLEN];
	   int nResult = server->m_UserManager.CheckFileName(recvbuf,result);
			switch(nResult)
			{				
				case 0:					
					send(clientsocket,"550 File not found.",sizeof("550 File not found."),0 );
					break;
				default:
					send(clientsocket,responseRETR1,sizeof(responseRETR1),0);
				    list.push_back(string(result));					
					if (!CreateDataConnection(1,list))
					{
						DestroyDataConnection();
					}
					break;
			}
			break;	    
	}

		if(!strcmp(command, "STOR"))
	{
	   printf("STOR\n");	 
	   send(clientsocket,responseSTOR1,sizeof(responseSTOR1),0 );

	  memset(createnewfilename, '\0',MAX_SIZE_STRING);
	  memcpy(createnewfilename,recvbuf+5,MAX_SIZE_STRING);
	  int i;
	  for(i=0;createnewfilename[i]!='\n';i++)
	  {}
	  createnewfilename[i-1]='\0';

	   ofstream fout(createnewfilename);
	   send(clientsocket,responseSTOR2,sizeof(responseSTOR2),0 );	
	   shutdown(DataPasvSocket,SD_SEND);
	   closesocket(DataPasvSocket);
	   closesocket(ListenDataPasvSocket);
	}
		if(!strcmp(command, "ABOR"))
	{
	   printf("ABOR\n");	 
	   send(clientsocket,responseABOR1,sizeof(responseABOR1),0 );	  
	   send(clientsocket,responseABOR2,sizeof(responseABOR2),0 );	 
	}
		if(!strcmp(command, "RNFR"))
	{
	   printf("RNFR\n");
	   memset(oldfilename, '\0',MAX_SIZE_STRING);
	   memcpy(oldfilename,recvbuf+5,MAX_SIZE_STRING);
	  // printf("OLDFILENAME = %s\n",oldfilename);
	   send(clientsocket,responseRNFR,sizeof(responseRNFR),0 );	  	 
	}

		if(!strcmp(command, "RNTO"))
	{
	   printf("RNTO\n");
	   memset(newfilename, '\0',MAX_SIZE_STRING);
	   memcpy(newfilename,recvbuf+5,MAX_SIZE_STRING);
	   int i;
	   for(i=0;oldfilename[i]!='\n';i++)
	  {}
	  oldfilename[i-1]='\0';
	  
	  for(i=0;newfilename[i]!='\n';i++)
	  {}
	  newfilename[i-1]='\0';
	   if ( rename( oldfilename,newfilename) == 0 )
	   {            
		   cout << "File renamed succes\n";
	   }
       else
	   {  
		   perror("fail file rename:");           
	   }
	   //printf("NEWFILENAME  = %s\n",recvbuf+5);
	   send(clientsocket,responseRNTO,sizeof(responseRNTO),0);	  	 
	}
			if(!strcmp(command, "PASV"))
	{
		/*printf("PASV %d\n",index);
	  // WaitForSingleObject(mutexPASV,INFINITE);
	 
	   send(clientsocket,responsePASV,sizeof(responsePASV),0 );
	     
       getaddrinfo(NULL, DEFAULT_DATA_PORT, &hints, &result);
         if ( iResult != 0 ) 
        printf("data connection getaddrinfo failed with error: %d\n %d", iResult,index);
   
    ListenDataPasvSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenDataPasvSocket == INVALID_SOCKET) {
        printf("data connection socket failed with error: %ld\n %d", WSAGetLastError(),index);          
    }   
    bind(ListenDataPasvSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("data connection bind failed with error: %d %d\n", WSAGetLastError(),index);        
    }
   

    listen(ListenDataPasvSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("data connection listen failed with error: %d\n %d", WSAGetLastError(),index);      
    }	
	
	printf("do data connection accept success %d\n",index);
    DataPasvSocket = accept(ListenDataPasvSocket, NULL, NULL);
    if (DataPasvSocket == INVALID_SOCKET) {
        printf("data connection accept failed with error: %d %d\n", WSAGetLastError(),index);      
    }
	printf("data connection accept success %d\n",index);*/
	
	}
			if(!strcmp(command, "LIST"))
    {	
	printf("LIST %d\n",index);
	send(clientsocket,responseLIST1,sizeof(responseLIST1),0 );	
	vector <string> list = this->server->m_UserManager.GetDirectoryList();
	if (!CreateDataConnection(0,list))
	{
	  DestroyDataConnection();
	}	
	 
	}
    } while (iRecv > 0);
	  cout << "THREAD IS BREAKING " << index <<  endl;
	  // shutdown(DataPasvSocket,SD_SEND);
	  //closesocket(DataPasvSocket);	
	  //closesocket(ListenDataPasvSocket);
	  //ReleaseMutex(mutexPASV);
	  //shutdown(DataPortSocket,SD_SEND);
	  //closesocket(DataPortSocket);
	  shutdown(clientsocket,SD_SEND);
	  closesocket(clientsocket);
   }



   BOOL ConnectSocket::CreateDataConnection(int nTransferType,vector <string> list)
{
	
	if (!PassiveMode)
	{
		a_DataSocket = new DataSocket(this, nTransferType);
		if (a_DataSocket->Create())
		{
			a_DataSocket->SetData(list);

			if (a_DataSocket->Connect(RemoteHost,RemotePort) == 0)
			{
				if (GetLastError() != WSAEWOULDBLOCK)        // проверка на полноту буферов
				{					
					send(clientsocket,"425 Can't open data connection.",sizeof("425 Can't open data connection."),0 );
					printf("425 Can't open data connection.\n");
					return false;
				}
			}
            a_DataSocket->OnSend();
			//DestroyDataConnection();


			/*switch(nTransferType)
			{
				case 0:
					send(clientsocket,"150 Opening ASCII mode data connection for directory list.",sizeof("150 Opening ASCII mode data connection for directory list."),0 );					
					break;
				case 1:
				case 2:
					send(clientsocket,"150 Opening BINARY mode data connection for file transfer.",sizeof("150 Opening BINARY mode data connection for file transfer."),0 );
					break;
			}*/

		}
		else
		{			
			send(clientsocket,"421 Failed to create data connection socket.",sizeof("421 Failed to create data connection socket."),0 );
			return FALSE;
		}
		
	}
	else
	{
		//a_DataSocket->SetRestartOffset(m_dwRestartOffset);
		//a_DataSocket->SetData(lpszData);
		//a_DataSocket->SetTransferType(nTransferType, TRUE);		
	}
	return true;
}

   void ConnectSocket::DestroyDataConnection()
{
	if (!a_DataSocket)
		return;

	delete a_DataSocket;

	// reset transfer status
	a_DataSocket = NULL;
	RemoteHost = "";
	RemotePort = -1;
	RestartOffset = 0;
	PassiveMode = FALSE;
}