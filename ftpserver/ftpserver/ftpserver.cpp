#include "library.h"
#include "ftpserver.h"


   ftpserver::ftpserver(void)
   {
	Port = 21;
	MaxUsers = 10;
	WelcomeMessage = "220 Andrey Scherbin FTP-Server";
	GoodbyeMessage = "Bye";
	Timeout = 5;
	Running = FALSE;
	nclients = 0;
	
	ConnectionCount = 0;
	TotalSentBytes = 0;
	TotalReceivedBytes = 0;
	TotalConnections = 0;
	FilesDownloaded = 0;
	FilesUploaded = 0;
	FailedDownloads = 0;
	FailedUploads = 0;

	m_UserManager.workWithFile(true);

	ThreadExitEvent=CreateEvent(NULL,TRUE,FALSE,"ThreadExitEvent");
	ThreadExitSemaphore = CreateSemaphore(NULL,1,1,"ThreadExitSemaphore");	
   }

   bool ftpserver ::start()
{ 
    char buff[1024];
	WSAStartup(0x0202,(WSADATA *) &buff[0]);
	if (Running)
		return FALSE;
	if (m_ListenSocket.Create(Port))
	{
		m_ListenSocket.Server = this;
		printf("FTP SERVER started on port %d .\n", Port);
		return true;
	}
    printf("FTP Server failed to listen on port %d.",Port);
	return false;
}
   void ftpserver ::setNumberExitThread(ConnectThread *exitThread)
   {
	   this->exitThread = exitThread;
   }


   ftpserver::~ftpserver(void)
   {
   }

   DWORD WINAPI ftpserver::FunctionForCheckEvents(LPVOID pParam)
{
	ftpserver *server = static_cast<ftpserver*>(pParam);
	while(true)
	{
	if(WaitForSingleObject(server->ThreadExitEvent,INFINITE) == WAIT_OBJECT_0)
	{		
	ConnectThread *exitThread = server->exitThread;	
	list<ConnectThread*>::iterator findIter = find(server->ThreadList.begin(), server->ThreadList.end(),exitThread);	
	WaitForSingleObject((*findIter)->thread,INFINITE);
	CloseHandle((*findIter)->thread);
	delete *findIter;
	server->ThreadList.remove(*findIter);
	server->decNumClients();	
    ResetEvent(server->ThreadExitEvent);
	ReleaseSemaphore(server->ThreadExitSemaphore,1,NULL);
	}
	}
	return 0;	
	
}
   void ftpserver ::incNumClients()
   {	  
	   this->nclients++;	 
   }

     void ftpserver ::decNumClients()
   {	  
	   this->nclients--;
   }

   int ftpserver::getNumClients()
   {	   
	   return nclients;
   }


