#include "ConnectSocket.h"
#include "library.h"
#include "ftpserver.h"
#include "ConnectSocket.h"
#include "ConnectThread.h"
#include "DataSocket.h"

ConnectSocket::ConnectSocket(void)
{	
	Loggedon = FALSE;
	a_DataSocket = NULL;
	RemoteHost = "";
	RemotePort = -1;	
	PassiveMode = FALSE;
	clientsocket = INVALID_SOCKET;	
}


ConnectSocket::~ConnectSocket(void)
{
	DestroyDataConnection();	
}

void ConnectSocket::OnClose() 
{ 
	shutdown(clientsocket,SD_SEND);
	closesocket(clientsocket);
}

void ConnectSocket::OnReceive(int numberThread)   
{
	char buff[DEFAULT_BUFLEN];
	while(true)
	{
	int nRead = recv(clientsocket, buff, DEFAULT_BUFLEN, 0);

	switch (nRead)
	{
		case 0:
			OnClose();			
			a_Thread->ExitThread();
			printfResponse("disconnected\n","");
			return;

		case SOCKET_ERROR:
			if (GetLastError() != WSAEWOULDBLOCK) 
			{
				a_Thread->ExitThread();				
				printfResponse("disconnected\n","");				
				return;
			}
			break;

		default:
			if (nRead != SOCKET_ERROR && nRead != 0)
			{					
			
			command = "";
	        for(int i = 0; i < nRead; i++)
	         {
	          command.push_back(buff[i]);
	         }	
			ParseCommand(command,numberThread);
			}	
			break;
	}
	}
}

BOOL ConnectSocket::SendResponse(char* responseChar)
{
	string responseString = string(responseChar);
	responseString+="\r\n";
	int nBytes = send(clientsocket,responseString.c_str(),responseString.size(),0);

	if (nBytes == SOCKET_ERROR)
	{
		OnClose();
		a_Thread->ExitThread();
		return FALSE;
	}	
	return TRUE;
}

BOOL ConnectSocket::getArgsFromCommand(string &command, string &args)
{		
	string delimiter = " ";
	int positionDelimiter = command.find(delimiter);
	if(positionDelimiter == -1){
		args="";
		return TRUE;
	}
	args = command.substr(command.find(delimiter)+1,command.size());	
	return TRUE;
}

string ConnectSocket::getLoggedon()
{		
	if(!Loggedon)
		return "(not logged in)";
	else
	{
		string username = UserName;
		username.pop_back();
		username.pop_back();		
		return username; 
	}
}

string ConnectSocket::getRemoteHost()
{
	sockaddr_in SockAddr;
	int addrlen= sizeof(SockAddr);
	getpeername(this->clientsocket,(LPSOCKADDR)&SockAddr,&addrlen);		
	char ipstr[16];
	strcpy(ipstr,inet_ntoa(SockAddr.sin_addr));	
	return ipstr;
}

void ConnectSocket::printfResponse(string response,string args)
{
	if(args!="")
	{
	response+=args;	
	}

	printf("(%06d) %s - %s (%s) > %s",this->a_Thread->numberThread,
	currentDateTime().c_str(),
	getLoggedon().c_str(),
	getRemoteHost().c_str(),
	response.c_str());
}



   void ConnectSocket::ParseCommand(string command,int number)
{
	static FTPCommand commandList[] = 
	{
		{TOK_ABOR,	"ABOR", FALSE,	"Abort transfer: ABOR"}, 
		{TOK_BYE,	"BYE",  FALSE,	"Logout or break the connection: BYE"},
		{TOK_CDUP,	"CDUP", FALSE,	"Change to parent directory: CDUP"},
		{TOK_CWD,	"CWD",	TRUE,	"Change working directory: CWD [directory-name]"},
		{TOK_DELE,	"DELE", TRUE ,	"Delete file: DELE file-name"},
		{TOK_DIR,	"DIR",  FALSE,	"Get directory listing: DIR [path-name]"},
		{TOK_HELP,	"HELP",  FALSE, "Show help: HELP [command]"},
		{TOK_LIST,	"LIST", FALSE,	"Get directory listing: LIST [path-name]"}, 
		{TOK_MKD,	"MKD",	TRUE,	"Make directory: MKD path-name"},
		{TOK_NOOP,	"NOOP", FALSE,	"Do nothing: NOOP"},
		{TOK_PASS,	"PASS", TRUE,	"Supply a user password: PASS password"},
		{TOK_PASV,	"PASV", FALSE,	"Set server in passive mode: PASV"},
		{TOK_PORT,	"PORT", TRUE,	"Specify the client port number: PORT a0,a1,a2,a3,a4,a5"},
		{TOK_PWD,	"PWD",	FALSE,	"Get current directory: PWD"},
		{TOK_QUIT,	"QUIT",  FALSE,	"Logout or break the connection: QUIT"},		
		{TOK_RETR,	"RETR", TRUE,	"Get file: RETR file-name"},
		{TOK_RMD,	"RMD",	TRUE,	"Remove directory: RMD path-name"},
		{TOK_RNFR,	"RNFR", TRUE,	"Specify old path name of file to be renamed: RNFR file-name"},
		{TOK_RNTO,	"RNTO", TRUE,	"Specify new path name of file to be renamed: RNTO file-name"},		
		{TOK_STOR,	"STOR", TRUE,	"Store file: STOR file-name"},
		{TOK_SYST,	"SYST", FALSE,	"Get operating system type: SYST"},
		{TOK_TYPE,	"TYPE", TRUE,	"Set filetype: TYPE [A | I]"},
		{TOK_USER,	"USER", TRUE,	"Supply a username: USER username"},
		{TOK_ERROR,	"",		FALSE,  ""},
	};

	char commandChar[COMMAND_BUFLEN];
	sscanf(command.c_str(), "%s",commandChar);
	string strCommand = string(commandChar);
	string args;	
	getArgsFromCommand(command,args);

	int nCommand;
	
	for (nCommand = TOK_ABOR; nCommand < TOK_ERROR; nCommand++)
	{		
		if (strCommand == commandList[nCommand].Name)
		{			
			if (commandList[nCommand].HasArguments && (args == ""))
			{
				SendResponse("501 Syntax error: Invalid number of parameters.");
				return;
			}
			break;			
		}
	}

	if (nCommand == TOK_ERROR)
	{		
		SendResponse("501 Syntax error: Command not understood.");
		return;
	}

	if ((nCommand != TOK_USER && nCommand != TOK_PASS) && !Loggedon)
	{
		SendResponse("530 Please login with USER and PASS.");
		return;
	}	
	
	printfResponse(command.c_str(),"");


	switch(nCommand)
	{
	
		case TOK_USER:
		{
	 	
	  Loggedon = FALSE;
	  UserName = args;
	  printfResponse("331 UserName ok,need password for ",args);
	  SendResponse("331 User name ok, need password");	 
	   }
       break;

	  case TOK_PASS:
		{
			if (Loggedon)
			{
				printfResponse("503 Login with USER first.\n","");
				SendResponse("503 Login with USER first.");
			}
			else
			{
	  User user;

	  if (server->m_UserManager.GetUser(UserName, user))
				{
					args.pop_back();
                    args.pop_back();
					if ((user.Password == args ||  user.AnonymousMode))
					{						
						currentDir = user.strDir;
						Loggedon = TRUE;
						printfResponse("230 User successfully logged in.\n","");
						SendResponse("230 User successfully logged in.");
						break;
					}					
	           }
	         printfResponse("530 Not logged in, password incorrect!\n","");
	         SendResponse("530 Not logged in, password incorrect!");
			}
	}break;     

    case TOK_CDUP:
		{
			User user;
	  if (server->m_UserManager.GetUser(UserName, user))
				{
					if (user.strDir == currentDir)
					{
					    printfResponse("550 CDUP command failed\n","");
						SendResponse("550 CDUP command failed");					
						break;
					}				
	           }	
		
	   int nResult = server->m_UserManager.ChangeDirectoryCDUP(currentDir);
	  switch(nResult)
			{				
				case 1:
					printfResponse("250 CDUP command successful\n","");
					SendResponse("250 CDUP command successful");
					break;
				default:
					printfResponse("250 CDUP command successful\n","");
					SendResponse("250 CDUP command successful");
					break;
			}
	 }break;
		
		case TOK_PWD:
		{
	   
	   char stringg[MAX_SIZE_STRING];	 
	   wsprintf(stringg,"257 \"%s\" is current directory.",currentDir.c_str());
	   string str = currentDir + "\n";
	   printfResponse("257 current directory is ",str.c_str());
	   SendResponse(stringg);	   
	   }break;

		case TOK_SYST:
		{
	  
	  printfResponse("215 WIN\n","");
	  SendResponse("215 WIN");	   
	    }

		case TOK_TYPE:
		{
		printfResponse("200 Switching to Binary mode\n","");
	   SendResponse("200 Switching to Binary mode");  // открываем файлы в бинарном режиме  
	    }break;

		case TOK_DELE:
		{
	  args.pop_back();
      args.pop_back();
	  args = currentDir + "\\"  + args;
	  int nResult = server->m_UserManager.DeleteFile(args);
	  switch(nResult)
			{				
				case 1:
					printfResponse("200 file delete successfully ",args);
					SendResponse("250 file delete successfully.");
					break;
				default:
					printfResponse("450 Internal error deleting the file ",args);
					SendResponse("450 Internal error deleting the file");
					break;
			}	 
	}break;

    case TOK_RMD: 
		{
	
	  args.pop_back();
      args.pop_back();
	  args = currentDir + "\\"  + args;
	  int nResult = server->m_UserManager.DeleteDirectory(args);
	  switch(nResult)
			{				
				case 1:
					printfResponse("250 directory delete successfully ",args);
					SendResponse("250 directory delete successfully.");
					break;
				default:
					printfResponse("450 Internal error deleting the directory ",args);
					SendResponse("450 Internal error deleting the directory");
					break;
			}	 
	   
	}break;

	 case TOK_CWD:
		{		
       int nResult = server->m_UserManager.ChangeDirectory(currentDir,args);
	  switch(nResult)
			{				
				case 1:	
					printfResponse("250 go to directory success ",args);
					SendResponse("250 go to directory success.");
					break;
				default:	
					printfResponse("250 go to directory success ",args);
					SendResponse("250 go to directory success.");
					break;
			}

	  }break;

	 case TOK_MKD: 
		{
	
	  args.pop_back();
      args.pop_back();
	  args = currentDir + "\\"  + args;
	  int nResult = server->m_UserManager.CreateDirectory(args);
	  switch(nResult)
			{				
				case 1:
					printfResponse("250 directory create successfully ",args);
					SendResponse("250 directory create successfully.");
					break;
				default:
					printfResponse("450 Internal error creating the directory ",args);
					SendResponse("450 Internal error creating the directory");
					break;
			}	 
	}break;	 

	  case TOK_PORT:
		{
	 
	   int c2 = command.rfind(",");
	   int c1 = command.rfind(",",c2-1);	 
	   int portNumber = atoi(command.substr(c1+1,c2-c1).c_str()) * 256;
	   portNumber += atoi(command.substr(c2+1).c_str());      
	   string address = command.substr(5,c1-5);
	   for(int i = 0; i < address.size(); i++)
		{
		  if(address.substr(i,1) == ",")
		     {
		      address.replace(i,1,".");
		     }
		 }
	   this->PassiveMode=FALSE;
	   this->RemotePort=portNumber;
	   this->RemoteHost=address;	 
	  SendResponse("200 Entering Active Mode .");
	  printfResponse("200 Entering Active Mode\n","");
	   }break;


	   case TOK_RETR:
		{
	 

	  if(!PassiveMode && (RemoteHost == "" || RemotePort == -1))
			{
				SendResponse("503 Bad sequence of commands.");	
				printfResponse("503 Bad sequence of commands\n","");
				break;
			}			
	   vector <string> list;
	   char result[DEFAULT_BUFLEN];
	   int nResult = server->m_UserManager.CheckFileName(args,result,currentDir);
			switch(nResult)
			{				
				case 0:						
					SendResponse("550 File not found.");
					printfResponse("550 File not found\n","");
					break;
				default:					
					SendResponse("150 Starting download file.");
					printfResponse("150 Starting download file ",args);
				    list.push_back(string(result));					
					if (!CreateDataConnection(1,list))
					{
						DestroyDataConnection();
					}
					break;
			}				    
	}break;

		case TOK_STOR:
		{
	

	  if(!PassiveMode && (RemoteHost == "" || RemotePort == -1))
			{
				SendResponse("503 Bad sequence of commands.");				
			}
	  else
			{
	   vector <string> list;	  									
	   SendResponse("150 Starting upload file.");
	   printfResponse("150 Starting upload file ",args);
	   args.pop_back();
       args.pop_back();
	   args = currentDir + "\\"  + args;	  
	   list.push_back(args);	 
					if (!CreateDataConnection(2,list))
					{
						DestroyDataConnection();
					}
					
			}	  
	  }break;

		case TOK_ABOR:
		{
			if (a_DataSocket)
			{
				if (a_DataSocket->GetStatus() != MODE_IDLE)
				{
					printfResponse("426 Data connection closed\n","");
					SendResponse("426 Data connection closed.");
				}				
			}
			SendResponse("226 ABOR command successful.");
			break;
		} 

		case TOK_QUIT:
		case TOK_BYE:
		{			
			char string[MAX_SIZE_STRING];
			ConnectThread *pThread = (ConnectThread *)a_Thread;	     
	        wsprintf(string,"220 %s",pThread->Server->GetGoodbyeMessage().c_str());
			printfResponse("220 Bye\n","");			
	        SendResponse(string);
			OnClose();
			return;
		}
		case TOK_RNFR:
		{
	   args.pop_back();
       args.pop_back();
	   args = currentDir + "\\"  + args;
	   renamefile = args;
	   printfResponse("350 Chose newname\n","");
	   SendResponse("350 Chose newname.");	  
	   }break;

		case TOK_RNTO:
		{	
	  args.pop_back();
      args.pop_back();
	  args = currentDir + "\\"  + args;
	  int nResult = server->m_UserManager.Rename(renamefile,args);
	  switch(nResult)
			{				
				case 1:
					printfResponse("250 file renamed successfully to ",args);
					SendResponse("250 file  renamed successfully");
					break;
				default:
					printfResponse("450 Internal error renamed file\n","");
					SendResponse("450 Internal error renamed file");
					break;
			}	  
	  }break;

			case TOK_PASV:
		{		
		DestroyDataConnection();		
		a_DataSocket = new DataSocket(this, -1);
		if (!a_DataSocket->Create())
			{
				DestroyDataConnection();
				printfResponse("421 Failed to create socket\n","");
				SendResponse("421 Failed to create socket.");	
				break;
			}		
			a_DataSocket->Listen();
			int port;
			string address;
			a_DataSocket->getSockInfo(port,address);			
	        char stringchar[MAX_SIZE_STRING];
	        wsprintf(stringchar,"227 Entering Passive Mode (%s,%d,%d).",LOCALHOST,port/256,port%256);
	        SendResponse(stringchar);
			string str = stringchar;
			str+="\n";
			printfResponse(str,"");
			PassiveMode = TRUE;			
	   }break;

			case TOK_LIST:
    {	
	

	if(!PassiveMode && (RemoteHost == "" || RemotePort == -1))
			{
				SendResponse("503 Bad sequence of commands.");
				printfResponse("503 Bad sequence of commands\n","");
				break;
			}
		SendResponse("150 Here comes the directory listing.");
		string str = currentDir +"\n";
		printfResponse("150 Here comes the directory listing to ",str);
	vector <string> list = this->server->m_UserManager.GetDirectoryList(currentDir);
	if (!CreateDataConnection(0,list))
	   {
	  DestroyDataConnection();
	   } 
	}break;

      case TOK_NOOP:
    {
		SendResponse("200 OK");
	}break;
	
		case TOK_HELP:
			{

			if (args == "")
			{
				string strResponse = "214-The following commands are recognized:\r\n";

				for (int i = TOK_ABOR; i < TOK_ERROR; i++)
				{
					strResponse += commandList[i].Name;
					strResponse += "\r\n";
				}
				strResponse += "214 HELP command successful.";
				const int size = strResponse.size();
				char *stringchar = new char[size];
				memcpy(stringchar,strResponse.c_str(),size);
				SendResponse(stringchar);
				string str = stringchar;
				str+="\n";
				printfResponse(str,"");
				delete stringchar;
			}
			else
			{
				int nHelpCmd;
				
				for (nHelpCmd = TOK_ABOR; nHelpCmd < TOK_ERROR; nHelpCmd++)
				{					
					if (args == commandList[nHelpCmd].Name)
					{
						break;			
					}
				}
				if (nHelpCmd != TOK_ERROR)
				{					
					char string[MAX_SIZE_STRING];
			        ConnectThread *pThread = (ConnectThread *)a_Thread;	     
	                wsprintf(string,"214 %s",commandList[nHelpCmd].Description);
	                SendResponse(string);
					printfResponse(string,"");
				}
				else
				{
					char string[MAX_SIZE_STRING];
			        ConnectThread *pThread = (ConnectThread *)a_Thread;	     
	                wsprintf(string,"501 Unknown command %s",args);
	                SendResponse(string);
					printfResponse(string,"");
				}
			}
			break;
			}

	default:
			SendResponse("502 Command not implemented - Try HELP.");
			printfResponse("502 Command not implemented - Try HELP\n","");
			break;
	  }
}

   BOOL ConnectSocket::CreateDataConnection(int nTransferType,vector <string> list)
{	
	if (!PassiveMode)
	{
		a_DataSocket = new DataSocket(this, nTransferType);
		if (a_DataSocket->Create())
		{
			a_DataSocket->SetData(list);

			if (a_DataSocket->Connect(RemoteHost,RemotePort) == 0)
			{
				if (GetLastError() != WSAEWOULDBLOCK)        // проверка на полноту буферов
				{					
					SendResponse("425 Can't open data connection.");
					printfResponse("425 Can't open data connection\n","");
					return false;
				}
			}
		}
		else
		{			
			SendResponse("421 Failed to create data connection socket.");
			printfResponse("421 Failed to create data connection socket\n","");
			return FALSE;
		}
		DestroyDataConnection();
		
	}
	else
	{		
		a_DataSocket->Accept();	
		a_DataSocket->SetData(list);
		a_DataSocket->SetTransferType(nTransferType, TRUE);		
	}
	return true;
}

   void ConnectSocket::DestroyDataConnection()
{
	if (!a_DataSocket)
		return;

	delete a_DataSocket;
	
	a_DataSocket = NULL;
	RemoteHost = "";
	RemotePort = -1;
	PassiveMode = FALSE;
}