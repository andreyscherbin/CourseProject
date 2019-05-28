#include "ConnectThread.h"

struct parametr_for_thread{
    ConnectThread thread;
	int numberThread;
	
	parametr_for_thread(ConnectThread thread,int numberThread){
        this->thread = thread;   
		this->numberThread = numberThread;
    }
};


ConnectThread::ConnectThread(void)
{
	ReceivedBytes = 0;
	SentBytes = 0;	
}


ConnectThread::~ConnectThread(void)
{
}

DWORD WINAPI InitInstance(LPVOID pParam)
{
	parametr_for_thread* Param=(parametr_for_thread*)pParam; 
	ConnectThread thread = Param->thread;
	printf("start thread %d socket = %d",Param->numberThread,Param->thread.a_ConnectSocket.clientsocket);
	thread.a_ConnectSocket.server = thread.Server;	
	thread.a_ConnectSocket.a_Thread = &thread;
	string str = thread.Server->GetWelcomeMessage();
	str+="\r\n";
	send(Param->thread.a_ConnectSocket.clientsocket,str.c_str(),str.size(),0); // вернуть из сервака приглос
	thread.a_ConnectSocket.OnReceive(Param->numberThread);
	return 0;
}

DWORD WINAPI ExitInstance(LPVOID pParam)
{  
	return 0;
}

