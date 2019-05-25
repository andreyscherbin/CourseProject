

#undef UNICODE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>    
#include <stdio.h>
#include <conio.h>
#include <direct.h>
#include <string>
#include <iostream>
#include <fstream>     
#include <stdlib.h>    
   
using namespace std;   
#pragma comment (lib, "Ws2_32.lib")  
#define DEFAULT_BUFLEN 512
#define DEFAULT_COMMAND_PORT "21"
#define DEFAULT_DATA_PORT "20"
#define MAXLINE 4096
#define MAX_SIZE_STRING 200
DWORD WINAPI  ThreadFunc(LPVOID pParam);
SOCKET ClientSocket = INVALID_SOCKET;
DWORD threadID;
HANDLE thread;
SOCKET ListenSocket; 
WSADATA wsaData;
const int BUFFER_SIZE = 1024;   
struct addrinfo *result = NULL;
struct addrinfo hints;
int iSendResult;
int iResult;
int iRecv;   	
HANDLE mutexPASV = CreateMutex(NULL,FALSE,"mutex PASV");	
int global = 1;
int bytes = 0;

	

DWORD WINAPI  ThreadFunc(LPVOID ClientSocket)   // ПОТОК-НОВОЙ КЛИЕНТ ( ТОТ ЖЕ САМЫЙ)
   {

	   ReleaseMutex(mutexPASV);
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
	   SOCKET clientsocket = (SOCKET)ClientSocket;
	 
	   int index = global;
	   global++;      
	   printf("Accept client succesfull %d socket = %d \n",index,clientsocket);                                         // Приглашение клиенту
	   send(clientsocket,responseHELLO,sizeof(responseHELLO),0 );
	
		do {
			printf("THREAD NUMBER %d\n",index);
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
		 printf("CDUP\n");
		 int i;
		 char symbol;
		 for(i=strlen(workingDirectory);i>=0;i--)
		 {
           symbol=workingDirectory[i];
		   if(symbol==92 || workingDirectory[i-1] == ':')  // asci code '\'
		   {			 
			 break;
		   }
		 }
     if(symbol != ':' && workingDirectory[i-1] != ':' )
	 {
	 memset(workingDirectory+i,'\0',strlen(workingDirectory));	 
	 }
	 if(workingDirectory[i-1] == ':')
	 {
      memset(workingDirectory+i,'\0',strlen(workingDirectory));
	  strcat(workingDirectory,"\\");	
	 }

	  if((chdir(workingDirectory)))
	  {
		  printf("error CDUP chdir\n");	
	  }	  
	  send(clientsocket,responseCDUP,sizeof(responseUSER),0 );
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
	   a  = a + "\"" + workingDirectory + "\"" + " is current directory";	  
	   strcat(responsePWD,a.c_str());
	   send(clientsocket,responsePWD,strlen(responsePWD)+1,0 );
	   printf("PWD WORKING DIRECTORY = %s %d\n",workingDirectory,index);
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
	  int i = 0;
	  char wayToCWD[MAX_SIZE_STRING];
	  char wayToCWD0[MAX_SIZE_STRING];
	  printf("CWD %d\n",index);	

	  for(i = 0;recvbuf[4+i]!='\n';i++)
	  {
		  wayToCWD[i] = recvbuf[4+i];		  
	  }
	  wayToCWD[i-1] = 0;
	  cout << workingDirectory  << " " << index << " " <<  wayToCWD << endl;
	  if(wayToCWD[1] != ':')
	  {
	  strcat(workingDirectory,"\\");
	  strcat(workingDirectory,wayToCWD);
	  cout << workingDirectory << endl;
	  send(clientsocket,responseCWD,sizeof(responseCWD),0 );
	  }
	  else
	  {
      memset(workingDirectory, '\0',MAX_SIZE_STRING);
	  memcpy(workingDirectory,wayToCWD,MAX_SIZE_STRING);
	  cout << workingDirectory << endl;
	  send(clientsocket,responseCWD,sizeof(responseCWD),0 );
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
          DataPortSocket = socket(AF_INET, SOCK_STREAM, 0);
	      struct sockaddr_in anAddr;
          anAddr.sin_family = AF_INET;
          anAddr.sin_port = htons (portNumber);
		  anAddr.sin_addr.S_un.S_addr = inet_addr(address.c_str());
          connect(DataPortSocket, (struct sockaddr *)&anAddr, sizeof(struct sockaddr));
	      send(clientsocket,responsePORT,sizeof(responsePORT),0 );	   
	}

	    if(!strcmp(command, "RETR"))
	{
	   char copydownloadfilename[DEFAULT_BUFLEN]="\\";
	   char copyworkingDirectory[MAX_SIZE_STRING];
	   char port[MAXLINE],buffer[MAXLINE],char_num_blks[MAXLINE],char_num_last_blk[MAXLINE];
	   int datasock,lSize,num_blks,num_last_blk,i;
	   printf("RETR %d\n",index);
	   memset(downloadfilename, '\0',MAX_SIZE_STRING);
	   memcpy(downloadfilename,recvbuf+5,MAX_SIZE_STRING);	 
	   for(i=0;downloadfilename[i]!='\n';i++)
	   {}
	   downloadfilename[i-1]='\0';  
       FILE*fp;	   
	   strcpy(copyworkingDirectory,workingDirectory);
	   strcat(copydownloadfilename,downloadfilename);
	   strcat(copyworkingDirectory,copydownloadfilename);	  
	   if ((fp = fopen(copyworkingDirectory, "rb"))==NULL) {
		printf("CURRENT DIR IS = %s\n",getcwd(NULL,MAX_SIZE_STRING));
         printf("CANNOT OPEN FILE NAME = %s.\n",downloadfilename);
	   }
	   else
	   {	  
       send(clientsocket,responseRETR1,sizeof(responseRETR1),0 );
	   fseek (fp , 0 , SEEK_END);
		lSize = ftell (fp);
		rewind (fp);
		num_blks = lSize/MAXLINE;		
		num_last_blk = lSize%MAXLINE; 	
		sprintf(char_num_blks,"%d",num_blks);		
		for(i= 0; i < num_blks; i++) { 
			fread (buffer,sizeof(char),MAXLINE,fp);			
			send(DataPortSocket,buffer,MAXLINE, 0);		
		}
		sprintf(char_num_last_blk,"%d",num_last_blk);	
		if (num_last_blk > 0) { 
			fread (buffer,sizeof(char),num_last_blk,fp);
			send(DataPortSocket,buffer,num_last_blk, 0);			
		}
		fclose(fp);
		send(clientsocket,responseRETR2,sizeof(responseRETR2),0);		
		shutdown(DataPortSocket,SD_SEND);		
		closesocket(DataPortSocket);
		//closesocket(ListenDataPasvSocket);
		// ReleaseMutex(mutexPASV);
		// WSACleanup();
	   } 
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
		printf("PASV %d\n",index);
	   WaitForSingleObject(mutexPASV,INFINITE);
	 
	   send(clientsocket,responsePASV,sizeof(responsePASV),0 );

	      // Resolve the server address and port
       getaddrinfo(NULL, DEFAULT_DATA_PORT, &hints, &result);
         if ( iResult != 0 ) {
        printf("data connection getaddrinfo failed with error: %d\n %d", iResult,index);
        WSACleanup(); 
		return 1;
         }

    // Create a SOCKET for connecting to server
    ListenDataPasvSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenDataPasvSocket == INVALID_SOCKET) {
        printf("data connection socket failed with error: %ld\n %d", WSAGetLastError(),index);
        freeaddrinfo(result);
        WSACleanup(); 
		return 1;
    }

    // Setup the TCP listening socket
    bind(ListenDataPasvSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("data connection bind failed with error: %d %d\n", WSAGetLastError(),index);
        freeaddrinfo(result);
        closesocket(DataPasvSocket);
        WSACleanup(); 
		return 1;
    }
    freeaddrinfo(result);

    listen(ListenDataPasvSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("data connection listen failed with error: %d\n %d", WSAGetLastError(),index);
        closesocket(DataPasvSocket);
        WSACleanup();  
		return 1;
    }	
	
	printf("do data connection accept success %d\n",index);
    DataPasvSocket = accept(ListenDataPasvSocket, NULL, NULL);
    if (DataPasvSocket == INVALID_SOCKET) {
        printf("data connection accept failed with error: %d %d\n"
			, WSAGetLastError(),index);
        closesocket(DataPasvSocket);
        WSACleanup();
		return 1;
    }
	printf("data connection accept success %d\n",index);
	
	}
			if(!strcmp(command, "LIST"))
    {		
	printf("LIST %d\n",index);
	send(clientsocket,responseLIST1,sizeof(responseLIST1),0 );	
	char fileInfo[MAXLINE];
	char fileAttributes[MAXLINE];
	char fileSize[MAXLINE];
	char fileMonth[MAXLINE];
	char fileDay[MAXLINE];
	char fileTime[MAXLINE];
	char Time[MAXLINE];
	char currentDir[MAXLINE];

	FILETIME ftCreate, ftAccess, ftWrite;
    SYSTEMTIME stUTC, stLocal;	
	memset(fileInfo, '\0',MAXLINE);
	memset(fileAttributes, '\0',MAXLINE);
	memset(fileSize, '\0',MAXLINE);
	memset(fileMonth, '\0',MAXLINE);
	memset(fileDay, '\0',MAXLINE);
	memset(fileTime, '\0',MAXLINE);	
	memset(currentDir, '\0',MAXLINE);	
	WIN32_FIND_DATA fd; 
	strcpy(currentDir,workingDirectory);
	strcat(currentDir,"/*.*");
    HANDLE hFind = FindFirstFile(currentDir, &fd); 
    if(hFind != INVALID_HANDLE_VALUE) { 
        do {

			if(!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) {
                 strcat(fileInfo,"-");
            }
			else
			{
				strcat(fileInfo,"d");
			}
			sprintf(fileAttributes, "%d", fd.dwFileAttributes);
			sprintf(fileSize, "%d", fd.nFileSizeLow);
			FileTimeToSystemTime(&fd.ftLastWriteTime, &stUTC);
			SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
			sprintf(fileMonth, "%d", stLocal.wMonth);
			sprintf(fileDay, "%d", stLocal.wDay);
			sprintf(fileTime, "%d:%d",stLocal.wHour,stLocal.wMinute);
			strcat(fileInfo,fileAttributes);
			strcat(fileInfo," ");
			strcat(fileInfo,"andrey");
			strcat(fileInfo," ");
			strcat(fileInfo,"andrey");
			strcat(fileInfo," ");
			strcat(fileInfo,fileSize);
			strcat(fileInfo," ");
			strcat(fileInfo,fileMonth);
			strcat(fileInfo," ");
			strcat(fileInfo,fileDay);
			strcat(fileInfo," ");
			strcat(fileInfo,fileTime);
			strcat(fileInfo," ");
			strcat(fileInfo,fd.cFileName);
			strcat(fileInfo,"\r\n");
            send(DataPortSocket,fileInfo,MAXLINE, 0);
			   
         memset(fileInfo, '\0',MAXLINE);      
        }while(FindNextFile(hFind, &fd)); 
        FindClose(hFind); 
    } 		
	  send(clientsocket,responseLIST2,sizeof(responseLIST2),0 );	  
	  shutdown(DataPortSocket,SD_SEND);
	  closesocket(DataPortSocket);	 
	  closesocket(ListenDataPasvSocket);
	//  ReleaseMutex(mutexPASV);
	 
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

int  main() 
{
	int count = 1;
	setlocale(LC_ALL,"Russian");
	

	chdir("D:/usb");   // база сервера.

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_COMMAND_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
	

    // Accept a client socketz
	while(true)
	{
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        printf("accept failed with error: %d\n"
			, WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
	else
	{   printf("\nNEW CONNECT\n");		
		thread =  CreateThread(NULL,0,ThreadFunc,(LPVOID)ClientSocket,0,&threadID);
	}
	}

    // shutdown the connection since we're done
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }
    // cleanup
	closesocket(ListenSocket);
    closesocket(ClientSocket);
	printf("Server ended\n");
	getch();	
    WSACleanup();
    return 0;
}
