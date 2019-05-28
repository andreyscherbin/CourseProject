#include "library.h"
#pragma once

#define FTP_DOWNLOAD	1
#define FTP_UPLOAD		2
#define FTP_RENAME		3
#define FTP_DELETE		4
#define FTP_CREATE_DIR	5
#define FTP_LIST		6

class User
{
public:

	User();
	User(string,string);	
	virtual ~User();

public:
	
	User (const User &user);				
	User &operator=(User &user);	

	string Name;
	string Password;
	string  strDir;
	BOOL    AllowDownload;
	BOOL    AllowUpload;
	BOOL    AllowRename;
	BOOL    AllowDelete;
	BOOL    AllowCreateDirectory;
	BOOL    IsHomeDir;
	BOOL  AnonymousMode;
};


class UserManager  
{
public:
	void GetUserList(vector<User>&array);
	void UpdateUserList(vector<User>&array);
	UserManager();
	virtual ~UserManager();
	
	vector<User> UserArray;

	//char currentDirectory[MAX_SIZE_STRING]; 

	BOOL workWithFile(bool mode);
	BOOL GetUser(string nameUser, User &user);
	//BOOL ConvertPathToLocal(LPCTSTR lpszUser, string &strDirectoryIn, string &strDirectoryOut);
	//int CheckDirectory(LPCTSTR lpszUser, string dirname, string strCurrentdir, int nOption, string &strResult);
	 int CheckFileName(string namefile,char* result,char* currentDirectory);
	 int ChangeDirectory(char* currentDirectory,string waytocwd);
	 int ChangeDirectoryCDUP(char* currentDirectory);
	 vector <string> GetDirectoryList(char * currentDirectory); //LPCTSTR lpszUser, LPCTSTR lpszDirectory, string &strResult
	//BOOL FileExists(LPCTSTR lpszFileName, BOOL bIsDirCheck = TRUE);
public:
	//BOOL CheckAccessRights(LPCTSTR lpszUser, LPCTSTR lpszDirectory, int nOption);
	//CString GetFileDate(CFileFind &find);
	string Filename;
	//CCriticalSection m_CriticalSection;
};





