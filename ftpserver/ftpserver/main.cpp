#include "library.h"
#include "ftpserver.h"


int  main() 
{
	setlocale(LC_ALL,"Russian");
	printf("FTP SERVER \n");
    ftpserver server;	
    server.start();

	while(true)
	{
		if(!(server.m_ListenSocket.OnAccept()))
		{
           WSACleanup();
		   break;
		}

		else
			printf("\nNEW CONNECT\n");
			
	}	
	printf("Server ended\n");
	getch();	
    WSACleanup();
    return 0;
}
