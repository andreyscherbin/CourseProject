#include "ConnectSocket.h"
#include "library.h"
#include "ftpserver.h"
#include "ConnectSocket.h"
#include "ConnectThread.h"
#include "DataSocket.h"

ConnectSocket::ConnectSocket(void)
{	
	a_DataSocket = NULL;
	RemoteHost = "";
	RemotePort = -1;	
	PassiveMode = FALSE;
	clientsocket = INVALID_SOCKET;
	strcpy(currentDirectory,"D:/usb");
}


ConnectSocket::~ConnectSocket(void)
{
	DestroyDataConnection();	
}

void ConnectSocket::OnClose() 
{ 
	shutdown(clientsocket,SD_SEND);
	closesocket(clientsocket);
}

void ConnectSocket::OnReceive(int numberThread)   
{
	char buff[DEFAULT_BUFLEN];
	while(true)
	{
	int nRead = recv(clientsocket, buff, DEFAULT_BUFLEN, 0);

	switch (nRead)
	{
		case 0:
			OnClose();
			printf("THREAD IS CLOSING %d\n",numberThread);
			return;

		case SOCKET_ERROR:
			if (GetLastError() != WSAEWOULDBLOCK) 
			{
				char szError[256];
				wsprintf(szError, "OnReceive error: %d", GetLastError());				
			}
			break;

		default:
			if (nRead != SOCKET_ERROR && nRead != 0)
			{					
			
			command = "";
	        for(int i = 0; i < nRead; i++)
	         {
	          command.push_back(buff[i]);
	         }	
			ParseCommand(command,numberThread);
			}	
			break;
	}
	}
}

BOOL ConnectSocket::SendResponse(char* responseChar)
{
	string responseString = string(responseChar);
	responseString+="\r\n";
	int nBytes = send(clientsocket,responseString.c_str(),responseString.size(),0);

	if (nBytes == SOCKET_ERROR)
	{
		OnClose();
		return FALSE;
	}	
	return TRUE;
}

void ConnectSocket::getArgsFromCommand(string &command, string &args)
{		
	string delimiter = " ";
	int positionDelimiter = command.find(delimiter);
	if(positionDelimiter == -1){
		args="";
		return;
	}
	args = command.substr(command.find(delimiter)+1,command.size());		
}

   void ConnectSocket::ParseCommand(string command,int number)
{
	char commandChar[COMMAND_BUFLEN];
	sscanf(command.c_str(), "%s",commandChar);
	string args;	
	getArgsFromCommand(command,args);	
	
		if(!strcmp(commandChar, "USER"))
	{
	  printf("USER %d\n",number);	  
	  UserName = args;	  
	  SendResponse("331 User name ok, need password");	 
	}

		if(!strcmp(commandChar, "PASS"))
	{
	  printf("PASS %d\n",number);
	  User user;

	  if (server->m_UserManager.GetUser(UserName, user))
				{
					args.pop_back();
                    args.pop_back();
					if ((user.Password == args ||  user.AnonymousMode))
					{						
						
						SendResponse("230 User successfully logged in.");						
					}
					else
					{
						SendResponse("530 Not logged in, password incorrect!");
					}
	           }
	  else
	  {
	  SendResponse("530 Not logged in, user incorrect!");
	  }  	 
	}

      if(!strcmp(commandChar, "REST"))
	{
	  printf("REST %d\n",number);	 
	  SendResponse("350 Restarting");
	}

     if(!strcmp(commandChar, "CDUP"))
	 {
	   int nResult = server->m_UserManager.ChangeDirectoryCDUP(currentDirectory);
	  switch(nResult)
			{				
				case 1:
					SendResponse("250 CDUP command successful");
					break;
				default:
					SendResponse("250 CDUP command successful");
					break;
			}
	 }
		
		if(!strcmp(commandChar, "PWD"))
	{
	   printf("PWD %d\n",number);
	   char string[MAX_SIZE_STRING];
	   wsprintf(string,"257 \"%s\" is current directory.",currentDirectory);
	   SendResponse(string);	   
	}
		if(!strcmp(commandChar, "SYST"))
	{
	  printf("SYST\n");	 
	  SendResponse("215 WIN");	   
	}	

		if(!strcmp(commandChar, "FEAT"))
	{
	  printf("FEAT\n");		 
	  SendResponse("211 Extensions supported");
	}	
		if(!strcmp(commandChar, "TYPE"))
	{
	   printf("TYPE %d\n",number);
	   SendResponse("200 Switching to Binary mode");  // открываем файлы в бинарном режиме  
	}
		if(!strcmp(commandChar, "DELE"))
	{
	 /* printf("DELE\n");
	  memset(deletefilename, '\0',MAX_SIZE_STRING);
	  memcpy(deletefilename,buff+5,MAX_SIZE_STRING);
	  int i;
	  for(i=0;deletefilename[i]!='\n';i++)
	  {}
	  deletefilename[i-1]='\0';	 
	  if(remove(deletefilename))
	  {
     perror("delete file error:");
	  } 	 
	  SendResponse("250 file delete successfully.");*/
	}
     if(!strcmp(commandChar, "RMD"))
	{
	  printf("RMD\n");	
	  SendResponse("250 directory delete successfully.");	  
	}
	 if(!strcmp(commandChar, "CWD"))
	{	
		printf("CWD %d\n",number);
       int nResult = server->m_UserManager.ChangeDirectory(currentDirectory,args);
	  switch(nResult)
			{				
				case 1:					
					SendResponse("250 go to directory success.");
					break;
				default:					
					SendResponse("250 go to directory success.");
					break;
			}

	}
	 if(!strcmp(commandChar, "MKD"))
	{
	 /* printf("MKD\n");	  
	  memset(newdirectoryname, '\0',MAX_SIZE_STRING);
	  memcpy(newdirectoryname,buff+4,MAX_SIZE_STRING);
	  int i;
	  for(i=0;newdirectoryname[i]!='\n';i++)
	  {}
	  newdirectoryname[i-1]='\0';	 
	  if(mkdir(newdirectoryname))
	  {      
      printf("Error create new directory\n");
	  } 
	  "257 create directory successfully."
	  send(clientsocket,responseMKD,sizeof(responseMKD),0 );*/
	}
	  if(!strcmp(commandChar, "QUIT"))
	{
	   printf("QUIT\n");	   
	   SendResponse("221 quit successfully.");	   
	}

	  if(command.substr(0,4) == "PORT")  // specify IP and port (PORT a1,a2,a3,a4,p1,p2) -> IP address a1.a2.a3.a4, port p1*256+p2.
	{
	   printf("PORT\n");  
	   int c2 = command.rfind(",");
	   int c1 = command.rfind(",",c2-1);	 
	   int portNumber = atoi(command.substr(c1+1,c2-c1).c_str()) * 256;
	   portNumber += atoi(command.substr(c2+1).c_str());      
	   string address = command.substr(5,c1-5);
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
	  SendResponse("200 Entering Active Mode .");
	}

	    if(!strcmp(commandChar, "RETR"))
	{
	   printf("RETR %d\n",number);	   
	   vector <string> list;
	   char result[DEFAULT_BUFLEN];
	   int nResult = server->m_UserManager.CheckFileName(args,result,currentDirectory);
			switch(nResult)
			{				
				case 0:						
					SendResponse("550 File not found.");
					break;
				default:					
					SendResponse("150 Starting download file.");
				    list.push_back(string(result));					
					if (!CreateDataConnection(1,list))
					{
						DestroyDataConnection();
					}
					break;
			}
				    
	}

		if(!strcmp(commandChar, "STOR"))
	{
	  /* printf("STOR\n");		 
	  SendResponse("150 Starting upload file.");
	  memset(createnewfilename, '\0',MAX_SIZE_STRING);
	  memcpy(createnewfilename,buff+5,MAX_SIZE_STRING);
	  int i;
	  for(i=0;createnewfilename[i]!='\n';i++)
	  {}
	  createnewfilename[i-1]='\0';
	 
	   ofstream fout(createnewfilename);
	    SendResponse("226 End upload file.");
	  */	  
	  
	}
		if(!strcmp(commandChar, "ABOR"))
	{
	   printf("ABOR\n");	 
	   SendResponse("426 Starting cancel transfer.");
	   SendResponse("226 End cancel transfer.");	  
	}
		if(!strcmp(commandChar, "RNFR"))
	{
	   /*printf("RNFR\n");
	   memset(oldfilename, '\0',MAX_SIZE_STRING);
	   memcpy(oldfilename,buff+5,MAX_SIZE_STRING);
	  // printf("OLDFILENAME = %s\n",oldfilename);	
	  SendResponse("350 Chose file to rename success.");*/
	  
	}

		if(!strcmp(commandChar, "RNTO"))
	{
	  /* printf("RNTO\n");
	   memset(newfilename, '\0',MAX_SIZE_STRING);
	   memcpy(newfilename,buff+5,MAX_SIZE_STRING);
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
	   SendResponse("350 Rename file success.");*/
	  
	}
			if(!strcmp(commandChar, "PASV"))
	{
		printf("PASV %d\n",number);	
		DestroyDataConnection();		
		a_DataSocket = new DataSocket(this, -1);

		if (!a_DataSocket->Create())
			{
				DestroyDataConnection();	
				SendResponse("421 Failed to create socket.");				
			}
		else
		{

		}


	  // WaitForSingleObject(mutexPASV,INFINITE);
	 
	  /* send(clientsocket,responsePASV,sizeof(responsePASV),0 );
	   "227 Entering Passive Mode (172,20,10,5,0,20)." 
       getaddrinfo(NULL, DEFAULT_DATA_PORT, &hints, &result);
         if ( iResult != 0 ) 
        printf("data connection getaddrinfo failed with error: %d\n %d", iResult,number);
   
    ListenDataPasvSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenDataPasvSocket == INVALID_SOCKET) {
        printf("data connection socket failed with error: %ld\n %d", WSAGetLastError(),number);          
    }   
    bind(ListenDataPasvSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("data connection bind failed with error: %d %d\n", WSAGetLastError(),number);        
    }
   

    listen(ListenDataPasvSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("data connection listen failed with error: %d\n %d", WSAGetLastError(),number);      
    }	
	
	printf("do data connection accept success %d\n",number);
    DataPasvSocket = accept(ListenDataPasvSocket, NULL, NULL);
    if (DataPasvSocket == INVALID_SOCKET) {
        printf("data connection accept failed with error: %d %d\n", WSAGetLastError(),number);      
    }
	printf("data connection accept success %d\n",number);*/
	
	}
			if(!strcmp(commandChar, "LIST"))
    {	
	printf("LIST %d\n",number);	
	SendResponse("150 Here comes the directory listing.");
	vector <string> list = this->server->m_UserManager.GetDirectoryList(currentDirectory);
	if (!CreateDataConnection(0,list))
	{
	  DestroyDataConnection();
	}	
	 
	} 
      if(!strcmp(commandChar, "NOOP"))
    {
		SendResponse("200 OK");
	}
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
			DestroyDataConnection();


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
	PassiveMode = FALSE;
}