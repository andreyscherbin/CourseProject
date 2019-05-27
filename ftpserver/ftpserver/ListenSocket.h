#ifndef LISTENSOCKET_H
#define LISTENSOCKET_H
#pragma once
#include "library.h"




class ftpserver;
class ListenSocket
{
	
public:
	ListenSocket(void);
	~ListenSocket(void);
	bool Create(int);

	ftpserver* Server;
	SOCKET CLIENTSOCKET;
	SOCKET LISTENSOCKET; 	

	bool OnAccept();
};

#endif // LISTENSOCKET_H