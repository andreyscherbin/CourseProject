#include "library.h"
#pragma once

#define FTP_DOWNLOAD	1
#define FTP_UPLOAD		2
#define FTP_RENAME		3
#define FTP_DELETE		4
#define FTP_CREATE_DIR	5
#define FTP_LIST		6


class Directory 
{
public:

	Directory();
	Directory(string,string);
	virtual ~Directory();

public:
	//virtual void Serialize(CArchive&);
	Directory (const Directory &dir);				// copy-constructor
	Directory &operator=(const Directory &dir);	// =-operator

    string  strDir;
	string  strAlias;
	BOOL    AllowDownload;
	BOOL    AllowUpload;
	BOOL    AllowRename;
	BOOL    AllowDelete;
	BOOL    AllowCreateDirectory;
	BOOL    IsHomeDir; 
};




class User
{
public:

	User();
	User(string,string);	
	virtual ~User();

public:
	//virtual void Serialize(CArchive&);
	User (const User &user);				// copy-constructor
	User &operator=(User &user);	// =-operator

	string Name;
	string Password;

	vector <Directory> DirectoryArray;

	BOOL AccountDisabled;
};


class UserManager  
{
public:
	void GetUserList(vector<User>&array);
	void UpdateUserList(vector<User>&array);
	UserManager();
	virtual ~UserManager();
	
	vector<User> UserArray;

	char currentDirectory[MAX_SIZE_STRING]; 

	
	BOOL GetUser(LPCTSTR lpszUser, User &user);
	//BOOL ConvertPathToLocal(LPCTSTR lpszUser, string &strDirectoryIn, string &strDirectoryOut);
	//int CheckDirectory(LPCTSTR lpszUser, string dirname, string strCurrentdir, int nOption, string &strResult);
	 int CheckFileName(char* nameFile,char* result);
	 int ChangeDirectory(char* currentDirectory,char* wayToCWD);
	 int ChangeDirectoryCDUP(char* currentDirectory);
	 vector <string> GetDirectoryList(); //LPCTSTR lpszUser, LPCTSTR lpszDirectory, string &strResult
	//BOOL FileExists(LPCTSTR lpszFileName, BOOL bIsDirCheck = TRUE);
public:
	//BOOL CheckAccessRights(LPCTSTR lpszUser, LPCTSTR lpszDirectory, int nOption);
	//CString GetFileDate(CFileFind &find);
	string Filename;
	//CCriticalSection m_CriticalSection;
};





