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
	
	ConnectionCount = 0;
	TotalSentBytes = 0;
	TotalReceivedBytes = 0;
	TotalConnections = 0;
	FilesDownloaded = 0;
	FilesUploaded = 0;
	FailedDownloads = 0;
	FailedUploads = 0;
	
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


   ftpserver::~ftpserver(void)
   {
   }
