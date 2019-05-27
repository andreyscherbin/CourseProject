#include "ListenSocket.h"
#include "ConnectThread.h"

struct parametr_for_thread{
    ConnectThread thread;
	int numberThread;
	
	parametr_for_thread(ConnectThread thread,int numberThread){
        this->thread = thread;   
		this->numberThread = numberThread;
    }
};



ListenSocket::ListenSocket(void)
{
	 CLIENTSOCKET = INVALID_SOCKET;
	 LISTENSOCKET = INVALID_SOCKET; 
}


ListenSocket::~ListenSocket(void)
{
}

bool ListenSocket ::Create(int Port)
{   
	struct addrinfo *result = NULL;
    struct addrinfo hints;
	int iResult;

	ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

	iResult = getaddrinfo(NULL, DEFAULT_COMMAND_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);        
        return false;
    }

	LISTENSOCKET = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (LISTENSOCKET == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
        return false;
    }

	iResult = bind( LISTENSOCKET, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(LISTENSOCKET);       
        return false;
    }

	iResult = listen(LISTENSOCKET, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(LISTENSOCKET);       
        return false;
    }	
	return true;
}

bool ListenSocket::OnAccept() 
{
	CLIENTSOCKET = accept(LISTENSOCKET, NULL, NULL);
    if (CLIENTSOCKET == INVALID_SOCKET) {
        printf("accept failed with error: %d\n"
			, WSAGetLastError());
        closesocket(LISTENSOCKET);        
        return false;
    } 
	else
	{  
		ConnectThread pThread;

		pThread.Server = this->Server;
		pThread.a_ConnectSocket.clientsocket = this->CLIENTSOCKET;

		parametr_for_thread* Param=new parametr_for_thread(pThread,1);                            // �� ������ ������� ������������ ������
	    pThread.thread =  CreateThread(NULL,0,InitInstance,(LPVOID)Param,0,&pThread.threadID);		
	}
  return true;
}




