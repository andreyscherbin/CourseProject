#include "UserManager.h"

User::User()
{
	AnonymousMode = FALSE;
}


User::User(string name , string password)
{  
	this->Name = name;
	this->Password = password;
	this->AnonymousMode = FALSE;
}

User::~User()
{
}


User::User(const User &user)
{
	Name = user.Name;
	Password = user.Password;
	AnonymousMode = user.AnonymousMode;
	strDir = user.strDir;
	/*for (int i=0; i < user.DirectoryArray.size(); i++)
		DirectoryArray.push_back(user.DirectoryArray[i]);*/
}

User& User::operator=( User &user)
{
	if (&user != this)
	{
		Name = user.Name;
		Password = user.Password;
		AnonymousMode = user.AnonymousMode;		
	}
	return *this;
}


UserManager::UserManager()
{
	
	Filename += "./UsersInfo.txt";
	
}

BOOL UserManager::workWithFile(bool mode)
{
	User user;	
	int i = 0;

	string line;
  ifstream myfile (Filename);
  if (myfile.is_open())
  {
    while ( getline (myfile,line) )
    {
		if(line == ""){
			cout <<"empty" << endl;
			continue;
		}
    switch(i)
	{
	case 0:
		{
			user.Name = line;
			i = 1;
		}break;
	case 1:
		{
			user.Password = line;			
			i = 2;
		}break;
	case 2:
		{	user.strDir = line;				
			this->UserArray.push_back(user);			
			i = 0;
		}break;	
	}		
    }
    myfile.close();
	return TRUE;	
  }
  else cout << "Unable to open file" << endl;
 return FALSE;
}


UserManager::~UserManager(void)
{
}

BOOL UserManager::GetUser(string nameUser, User &user)
{
	nameUser.pop_back();
	nameUser.pop_back();

	//m_CriticalSection.Lock();
	for (int i=0; i<UserArray.size(); i++)
	{		
		if (UserArray[i].Name == nameUser)                          
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

vector <string> UserManager::GetDirectoryList(char *currentDirectory) //LPCTSTR lpszUser, LPCTSTR lpszDirectory, string &strResult
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

 int UserManager::ChangeDirectory(char* currentDirectory,string waytocwd)
 {
	  int i = 0;
	  char wayToCWD[MAX_SIZE_STRING];	

	  for(i = 0;waytocwd[i]!='\n';i++)
	  {
		  wayToCWD[i] = waytocwd[i];		  
	  }
	  wayToCWD[i-1] = 0;
	  cout << currentDirectory << " " <<  wayToCWD << endl;
	  if(wayToCWD[1] != ':')
	  {
	  strcat(currentDirectory,"\\");
	  strcat(currentDirectory,wayToCWD);	  
	  cout << currentDirectory << endl;
	  
	  return 1;
	  }
	  else
	  {
      memset(currentDirectory, '\0',MAX_SIZE_STRING);
	  memcpy(currentDirectory,wayToCWD,MAX_SIZE_STRING);
	  cout << currentDirectory << endl;	 
	  return 1;
	  }
    
 }

 int UserManager::ChangeDirectoryCDUP(char* currentDirectory)
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

	  return 1;
    
 }

 int UserManager::CheckFileName(string namefile,char* result,char *currentDirectory)
 {
	   char downloadfilename[DEFAULT_BUFLEN]="\\";
	   char copydownloadfilename[DEFAULT_BUFLEN]="\\";
	   char copycurrentDirectory[MAX_SIZE_STRING];
	   char port[MAXLINE],buffer[MAXLINE],char_num_blks[MAXLINE],char_num_last_blk[MAXLINE];
	   int datasock,lSize,num_blks,num_last_blk,i;

	   memset(downloadfilename, '\0',MAX_SIZE_STRING);
	   memcpy(downloadfilename,namefile.c_str(),MAX_SIZE_STRING);	 
	   for(i=0;downloadfilename[i]!='\n';i++)
	   {}
	   downloadfilename[i-1]='\0';  
       FILE*fp;	   
	   strcpy(copycurrentDirectory,currentDirectory);
	   strcat(copydownloadfilename,downloadfilename);
	   strcat(copycurrentDirectory,copydownloadfilename);	  
	   if ((fp = fopen(copycurrentDirectory, "rb"))==NULL) {
		
         printf("CANNOT OPEN FILE NAME = %s.\n",copycurrentDirectory);
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




