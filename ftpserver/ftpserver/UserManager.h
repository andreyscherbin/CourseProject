#include "library.h"
#pragma once

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
	BOOL workWithFile(bool mode);
	BOOL GetUser(string nameUser, User &user);	
	 int CheckFileName(string namefile,char* result,string &currentDir);
	 BOOL DeleteFile(string namefile);
	 BOOL DeleteDirectory(string nameDirectory);
	 BOOL CreateDirectory(string nameDirectory);
	 BOOL Rename(string oldName,string newName); 
	 int ChangeDirectory(string &currentDir,string waytocwd);
	 int ChangeDirectoryCDUP(string &currentDir);
	 vector <string> GetDirectoryList(string &currentDir); 	
public:	
	string Filename;
	
};





