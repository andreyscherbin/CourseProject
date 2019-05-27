#include "UserManager.h"


Directory::Directory()
{
}

Directory::Directory(string one , string two)
{
	this-> strDir = one;
    this-> strAlias = two;
}

Directory::~Directory()
{
}


Directory::Directory(const Directory &dir)
{
	strDir = dir.strDir;
	strAlias = dir.strAlias;
	AllowDownload = dir.AllowDownload;
	AllowUpload = dir.AllowUpload;
	AllowRename = dir.AllowRename;
	AllowDelete = dir.AllowDelete;
	AllowCreateDirectory = dir.AllowCreateDirectory;
	IsHomeDir = dir.IsHomeDir;
}


Directory& Directory::operator=(const Directory &dir)
{
	if (&dir != this)
	{
		strDir = dir.strDir;
		strAlias = dir.strAlias;
		AllowDownload = dir.AllowDownload;
		AllowUpload = dir.AllowUpload;
		AllowRename = dir.AllowRename;
		AllowDelete = dir.AllowDelete;
		AllowCreateDirectory = dir.AllowCreateDirectory;
		IsHomeDir = dir.IsHomeDir;
	}
	return *this;
}

User::User()
{
	AccountDisabled = FALSE;
}


User::User(string name , string password)
{  
	this->Name = name;
	this->Password = password;
	AccountDisabled = FALSE;
}

User::~User()
{
}


User::User(const User &user)
{
	Name = user.Name;
	Password = user.Password;
	AccountDisabled = user.AccountDisabled;
	for (int i=0; i < user.DirectoryArray.size(); i++)
		DirectoryArray.push_back(user.DirectoryArray[i]);
}

User& User::operator=( User &user)
{
	if (&user != this)
	{
		Name = user.Name;
		Password = user.Password;
		AccountDisabled = user.AccountDisabled;
		for (int i=0; i < user.DirectoryArray.size(); i++)
			DirectoryArray.push_back(user.DirectoryArray[i]);
	}
	return *this;
}


UserManager::UserManager()
{
	
	Filename += "C:/users.dat";
	strcpy(currentDirectory,"D:/usb");
}


UserManager::~UserManager(void)
{
}

BOOL UserManager::GetUser(LPCTSTR lpszUser, User &user)
{
	
	//m_CriticalSection.Lock();
	for (int i=0; i<UserArray.size(); i++)
	{
		if (UserArray[i].Name == lpszUser)                           // ÑÐÀÂÍÈÂÀÅÌ ÍÅ Ó×ÈÒÛÂÀß ÐÅÃÈÑÒÐ ( âåðõíèé èëè íèæíèé)
		{
			user = UserArray[i];
			//m_CriticalSection.Unlock();
			return TRUE;
		}
	}
	//m_CriticalSection.Unlock();
	return FALSE;
}

void UserManager::UpdateUserList(vector<User>&array)
{
	//m_CriticalSection.Lock();
	UserArray.clear();
	for (int i=0; i<array.size();i++)
	{
		UserArray.push_back(array[i]);
	}
	//m_CriticalSection.Unlock();
	//Serialize(TRUE);
}

void UserManager::GetUserList(vector<User>&array)
{
	//m_CriticalSection.Lock();
	for (int i=0; i<UserArray.size();i++)
	{
		array.push_back(UserArray[i]);
	}
	//m_CriticalSection.Unlock();
}

vector <string> UserManager::GetDirectoryList() //LPCTSTR lpszUser, LPCTSTR lpszDirectory, string &strResult
{
	
	vector <string> resultList;

	/*User user;
	if (!GetUser(lpszUser, user))
	{
		// user not found -> no permissions
		return resultList;
	}*/
	char fileInfo[MAXLINE];
	char fileAttributes[MAXLINE];
	char fileSize[MAXLINE];
	char fileMonth[MAXLINE];
	char fileDay[MAXLINE];
	char fileTime[MAXLINE];
	char Time[MAXLINE];
	char currentDir[MAXLINE];

	FILETIME ftCreate, ftAccess, ftWrite;
    SYSTEMTIME stUTC, stLocal;	
	memset(fileInfo, '\0',MAXLINE);
	memset(fileAttributes, '\0',MAXLINE);
	memset(fileSize, '\0',MAXLINE);
	memset(fileMonth, '\0',MAXLINE);
	memset(fileDay, '\0',MAXLINE);
	memset(fileTime, '\0',MAXLINE);	
	memset(currentDir, '\0',MAXLINE);	
	WIN32_FIND_DATA fd; 
	strcpy(currentDir,currentDirectory);
	strcat(currentDir,"/*.*");
    HANDLE hFind = FindFirstFile(currentDir, &fd); 
    if(hFind != INVALID_HANDLE_VALUE) { 
        do {

			if(!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) {
                 strcat(fileInfo,"-");
            }
			else
			{
				strcat(fileInfo,"d");
			}
			sprintf(fileAttributes, "%d", fd.dwFileAttributes);
			sprintf(fileSize, "%d", fd.nFileSizeLow);
			FileTimeToSystemTime(&fd.ftLastWriteTime, &stUTC);
			SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
			sprintf(fileMonth, "%d", stLocal.wMonth);
			sprintf(fileDay, "%d", stLocal.wDay);
			sprintf(fileTime, "%d:%d",stLocal.wHour,stLocal.wMinute);
			strcat(fileInfo,fileAttributes);
			strcat(fileInfo," ");
			strcat(fileInfo,"andrey");
			strcat(fileInfo," ");
			strcat(fileInfo,"andrey");
			strcat(fileInfo," ");
			strcat(fileInfo,fileSize);
			strcat(fileInfo," ");
			strcat(fileInfo,fileMonth);
			strcat(fileInfo," ");
			strcat(fileInfo,fileDay);
			strcat(fileInfo," ");
			strcat(fileInfo,fileTime);
			strcat(fileInfo," ");
			strcat(fileInfo,fd.cFileName);
			strcat(fileInfo,"\r\n");			
			resultList.push_back(string(fileInfo));            
			   
         memset(fileInfo, '\0',MAXLINE);      
        }while(FindNextFile(hFind, &fd)); 
        FindClose(hFind); 
    } 
	return resultList;
}

 int UserManager::ChangeDirectory(char* currentDir,char* waytocwd)
 {
	  int i = 0;
	  char wayToCWD[MAX_SIZE_STRING];	

	  for(i = 0;waytocwd[4+i]!='\n';i++)
	  {
		  wayToCWD[i] = waytocwd[4+i];		  
	  }
	  wayToCWD[i-1] = 0;
	  cout << currentDirectory << " " <<  wayToCWD << endl;
	  if(wayToCWD[1] != ':')
	  {
	  strcat(currentDirectory,"\\");
	  strcat(currentDirectory,wayToCWD);	  
	  cout << currentDirectory << endl;

	  strcat(currentDir,"\\");
	  strcat(currentDir,wayToCWD);
	  return 1;
	  }
	  else
	  {
      memset(currentDirectory, '\0',MAX_SIZE_STRING);
	  memcpy(currentDirectory,wayToCWD,MAX_SIZE_STRING);
	  cout << currentDirectory << endl;

	  memset(currentDir, '\0',MAX_SIZE_STRING);
	  memcpy(currentDir,wayToCWD,MAX_SIZE_STRING);
	  return 1;
	  }
    
 }

 int UserManager::ChangeDirectoryCDUP(char* currentDir)
 {	  
		 int i;
		 char symbol;
		 for(i=strlen(currentDirectory);i>=0;i--)
		 {
           symbol=currentDirectory[i];
		   if(symbol==92 || currentDirectory[i-1] == ':')  // asci code '\'
		   {			 
			 break;
		   }
		 }
     if(symbol != ':' && currentDirectory[i-1] != ':' )
	 {
	 memset(currentDirectory+i,'\0',strlen(currentDirectory));	 
	 }
	 if(currentDirectory[i-1] == ':')
	 {
      memset(currentDirectory+i,'\0',strlen(currentDirectory));
	  strcat(currentDirectory,"\\");	
	 } 
	  memset(currentDir,'\0',strlen(currentDir));
	  memcpy(currentDir,currentDirectory,MAX_SIZE_STRING);

	  return 1;
    
 }

 int UserManager::CheckFileName(char* nameFile,char* result)
 {
	   char downloadfilename[DEFAULT_BUFLEN]="\\";
	   char copydownloadfilename[DEFAULT_BUFLEN]="\\";
	   char copycurrentDirectory[MAX_SIZE_STRING];
	   char port[MAXLINE],buffer[MAXLINE],char_num_blks[MAXLINE],char_num_last_blk[MAXLINE];
	   int datasock,lSize,num_blks,num_last_blk,i;

	   memset(downloadfilename, '\0',MAX_SIZE_STRING);
	   memcpy(downloadfilename,nameFile+5,MAX_SIZE_STRING);	 
	   for(i=0;downloadfilename[i]!='\n';i++)
	   {}
	   downloadfilename[i-1]='\0';  
       FILE*fp;	   
	   strcpy(copycurrentDirectory,currentDirectory);
	   strcat(copydownloadfilename,downloadfilename);
	   strcat(copycurrentDirectory,copydownloadfilename);	  
	   if ((fp = fopen(copycurrentDirectory, "rb"))==NULL) {
		printf("CURRENT DIR IS = %s\n",getcwd(NULL,MAX_SIZE_STRING));
         printf("CANNOT OPEN FILE NAME = %s.\n",downloadfilename);
		 return 0;
	   }
	   else
	   {
		   fclose(fp);
	   memset(result, '\0',MAX_SIZE_STRING);
	   memcpy(result,copycurrentDirectory,MAX_SIZE_STRING);		  
		return 1;
	   }
 }




