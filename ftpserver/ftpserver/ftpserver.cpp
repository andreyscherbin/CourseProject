#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_COMMAND_PORT "21"
#define DEFAULT_DATA_PORT "20"

int __cdecl main(void) 
{
    WSADATA wsaData;
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

	SOCKET DataSocket = INVALID_SOCKET;
	SOCKET DataSocket1 = INVALID_SOCKET;

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
	char sendvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
	int sendvbuflen = DEFAULT_BUFLEN;
	char command[5];

	char file1[50] = "-r andrey andrey 0 56 01 02   2019 file.txt";
	char file2[50] = "dr andrey andrey 0 00 05 05   2018 coursework";

	char responseHELLO[DEFAULT_BUFLEN] = "220 Andrey Scherbin FTP-Server";
	char responseUSER[DEFAULT_BUFLEN] = "331 Anonymous login ok, send your complete email address as your password ";
	char responsePASS[DEFAULT_BUFLEN] = "230 Logged in anonymously";	
    char responsePWD[DEFAULT_BUFLEN] =  "257  \" /folder \" is current directory ";
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
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        printf("accept failed with error: %d\n"
			, WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
	
	printf("Accept client succesfull\n");                                         // Приглашение клиенту
	iSendResult = send( ClientSocket,responseHELLO,sizeof(responseHELLO),0 );

		do {			

			iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		    sscanf(recvbuf, "%s", command);
			printf("command = %s\n",command);
		
		if(!strcmp(command, "USER"))
	{
	  printf("USER command response\n");
	  iSendResult = send( ClientSocket,responseUSER,sizeof(responseUSER),0 );	 
	}

		if(!strcmp(command, "PASS"))
	{
	  printf("PASS command response\n");	  
	  iSendResult = send( ClientSocket,responsePASS,sizeof(responsePASS),0 );
	}
		if(!strcmp(command, "PWD"))
	{
	  printf("PWD command response\n");	  
	   iSendResult = send( ClientSocket,responsePWD,sizeof(responsePWD),0 );
	}
		if(!strcmp(command, "SYST"))
	{
	  printf("SYST command response\n");	  
	   iSendResult = send( ClientSocket,responseSYST,sizeof(responseSYST),0 );
	}	

		if(!strcmp(command, "FEAT"))
	{
	  printf("FEAT command response\n");	  
	   iSendResult = send( ClientSocket,responseFEAT,sizeof(responseFEAT),0 );
	}	
		if(!strcmp(command, "TYPE"))
	{
	  printf("TYPE command response\n");	  
	   iSendResult = send( ClientSocket,responseTYPE,sizeof(responseTYPE),0 );
	}
		if(!strcmp(command, "DELE"))
	{
	  printf("DELE command response\n");	
	   iSendResult = send( ClientSocket,responseDELE,sizeof(responseDELE),0 );
	}
     if(!strcmp(command, "RMD"))
	{
	  printf("RMD command response\n");	
	   iSendResult = send( ClientSocket,responseRMD,sizeof(responseRMD),0 );
	}
	 if(!strcmp(command, "CWD"))
	{
	  printf("CWD command response\n");	
	   iSendResult = send( ClientSocket,responseCWD,sizeof(responseCWD),0 );
	}
	 if(!strcmp(command, "MKD"))
	{
	  printf("MKD command response\n");	
	   iSendResult = send(ClientSocket,responseMKD,sizeof(responseMKD),0 );
	}
	  if(!strcmp(command, "QUIT"))
	{
	   printf("QUIT command response\n");
	   printf("FTP server quitting..\n");
	   iSendResult = send(ClientSocket,responseQUIT,sizeof(responseQUIT),0 );
	   exit(1);
	}

	  if(!strcmp(command, "PORT"))
	{
	   printf("PORT command response\n");	 
	   iSendResult = send(ClientSocket,responsePORT,sizeof(responsePORT),0 );	   
	}

	    if(!strcmp(command, "RETR"))
	{
	   printf("RETR command response\n");	 
	   iSendResult = send(ClientSocket,responseRETR1,sizeof(responseRETR1),0 );	  
	   iSendResult = send(ClientSocket,responseRETR2,sizeof(responseRETR2),0 );	 
	}
		if(!strcmp(command, "STOR"))
	{
	   printf("STOR command response\n");	 
	   iSendResult = send(ClientSocket,responseSTOR1,sizeof(responseSTOR1),0 );	  
	   iSendResult = send(ClientSocket,responseSTOR2,sizeof(responseSTOR2),0 );	 
	}
		if(!strcmp(command, "ABOR"))
	{
	   printf("ABOR command response\n");	 
	   iSendResult = send(ClientSocket,responseABOR1,sizeof(responseABOR1),0 );	  
	   iSendResult = send(ClientSocket,responseABOR2,sizeof(responseABOR2),0 );	 
	}
		if(!strcmp(command, "RNFR"))
	{
	   printf("RNFR command response\n");	 
	   iSendResult = send(ClientSocket,responseRNFR,sizeof(responseRNFR),0 );	  	 
	}

		if(!strcmp(command, "RNTO"))
	{
	   printf("RNTO command response\n");	 
	   iSendResult = send(ClientSocket,responseRNTO,sizeof(responseRNTO),0 );	  	 
	}





			if(!strcmp(command, "PASV"))
	{
	   printf("PASV command response\n");	  
	   iSendResult = send( ClientSocket,responsePASV,sizeof(responsePASV),0 );

	      // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_DATA_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("data connection getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for connecting to server
    DataSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (DataSocket == INVALID_SOCKET) {
        printf("data connection socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind( DataSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("data connection bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(DataSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    iResult = listen(DataSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("data connection listen failed with error: %d\n", WSAGetLastError());
        closesocket(DataSocket);
        WSACleanup();
        return 1;
    }
		 
		

    DataSocket1 = accept(DataSocket, NULL, NULL);
    if (DataSocket1 == INVALID_SOCKET) {
        printf("data connection accept failed with error: %d\n"
			, WSAGetLastError());
        closesocket(DataSocket);
        WSACleanup();
        return 1;
    }
	
	 printf("data connection Accept client succesfull\n");
	 iResult =  1;

	}
			if(!strcmp(command, "LIST"))
    {
	  printf("LIST command response\n"); 	 
	  iResult = send( ClientSocket,responseLIST1,sizeof(responseLIST1),0 );	  
	  iResult = send(DataSocket1,file1,sizeof(file1),0 );
	  iResult = send(DataSocket1,file2,sizeof(file2),0 );
	  iResult = send( ClientSocket,responseLIST2,sizeof(responseLIST2),0 );
	  shutdown(DataSocket1,SD_SEND);
	  closesocket(DataSocket);
	  closesocket(DataSocket1);
	}


    } while (iResult > 0);



   
		

	
  
    

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