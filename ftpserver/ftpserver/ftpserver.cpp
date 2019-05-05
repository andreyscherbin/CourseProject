#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "21"

int __cdecl main(void) 
{
    WSADATA wsaData;
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
	char sendvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
	int sendvbuflen = DEFAULT_BUFLEN;

	char responseHELLO[DEFAULT_BUFLEN] = "220 Andrey Scherbin FTP-Server";
	char responseUSER[DEFAULT_BUFLEN] = "331 Anonymous login ok, send your complete email address as your password ";
	char responsePASS[DEFAULT_BUFLEN] = "230 Logged in anonymously";
	char responsePASV[DEFAULT_BUFLEN] = "227 Entering passive mode";
    char responsePWD[DEFAULT_BUFLEN] = "257 ""/folder"" is current directory";


	





    
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
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
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
	else


		

		printf("Accep client succesfull\n");
	    
	    iSendResult = send( ClientSocket,responseHELLO,sizeof(responseHELLO),0 );
		
		iSendResult = send( ClientSocket,responseUSER,sizeof(responseUSER),0 );
		
		iSendResult = send( ClientSocket,responsePASS,sizeof(responsePASS),0 );
	    iSendResult = send( ClientSocket,responsePWD,sizeof(responsePWD),0 );

		//printf("%d\n",iSendResult);
	    printf("1\n");
    // No longer need server socket
    closesocket(ListenSocket);
	    printf("2\n");
    // Receive until the peer shuts down the connection
    do {
		 printf("3\n");
		
        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		//printf("recv 1 = %s",recvbuf);
		if(!strcmp(recvbuf, "ls"))
	{
	  system("ls >temps.txt");
	  printf("ls\n");

	  //i = 0;
	 // stat("temps.txt",&obj);
	 // size = obj.st_size;
	  //send(sock2, &size, sizeof(int),0);
	  //filehandle = open("temps.txt", O_RDONLY);
	  //sendfile(sock2,filehandle,NULL,size);
	}
        if (iResult > 0) {
            printf("Bytes received: %d\n", iResult);

        // Echo the buffer back to the sender
            iSendResult = send( ClientSocket, recvbuf, iResult, 0 );
            if (iSendResult == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                return 1;
            }
            printf("Bytes sent: %d\n", iSendResult);
        }
        else if (iResult == 0)
            printf("Connection closing...\n");
        else  {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            return 1;
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
    closesocket(ClientSocket);
    WSACleanup();

    return 0;
}