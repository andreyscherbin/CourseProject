#include "library.h"
#include "ftpserver.h"


int  main() 
{
	setlocale(LC_ALL,"Russian");
	printf("FTP SERVER \n");
    ftpserver server;	
    server.start();
	server.ThreadForCheckEvents =  CreateThread(NULL,0,ftpserver::FunctionForCheckEvents,&server,0,&(server.ThreadForCheckEventsID));

	while(true)
	{
		if(!(server.m_ListenSocket.OnAccept()))
		{
           WSACleanup();
		   break;
		}			
	}	
	printf("Server ended\n");
	getch();	
    WSACleanup();
    return 0;
}
