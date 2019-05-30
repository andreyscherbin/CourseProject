#include "ConnectThread.h"

ConnectThread::ConnectThread(int number)
{
	ReceivedBytes = 0;
	SentBytes = 0;
	thread = 0;
	threadID = 0;		
	numberThread = number;
	
}

ConnectThread::~ConnectThread(void)
{
}


DWORD WINAPI ConnectThread::StartThread(LPVOID pParam)
{
	
	ConnectThread *pThread = static_cast<ConnectThread*>(pParam);		
	pThread->a_ConnectSocket.server = pThread->Server;
	pThread->a_ConnectSocket.a_Thread = pThread;
	string str = pThread->Server->GetWelcomeMessage();
	str+="\r\n";
	printf("(%06d) %s - %s (%s) > %s",pThread->numberThread,currentDateTime().c_str(),pThread->a_ConnectSocket.getLoggedon().c_str(),pThread->a_ConnectSocket.getRemoteHost().c_str(),str.c_str());
	send(pThread->a_ConnectSocket.clientsocket,str.c_str(),str.size(),0); 
	pThread->a_ConnectSocket.OnReceive(1);	
	return 0;	
	
}

bool ConnectThread::ExitThread()
{	
	HANDLE ThreadExitEvent=  CreateEvent(NULL,TRUE,FALSE,"ThreadExitEvent"); 
	HANDLE ThreadExitSemaphore = CreateSemaphore(NULL,1,1,"ThreadExitSemaphore");
	WaitForSingleObject(ThreadExitSemaphore,INFINITE);
	this->Server->setNumberExitThread(this);
	SetEvent(ThreadExitEvent);
	CloseHandle(ThreadExitSemaphore);	
	return 0;
}

