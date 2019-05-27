#undef UNICODE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>    
#include <stdio.h>
#include <conio.h>
#include <direct.h>
#include <string>
#include <iostream>
#include <fstream>     
#include <stdlib.h> 
#include <vector>
#include <list>
   
using namespace std;   
#pragma comment (lib, "Ws2_32.lib")
#pragma once
#define DEFAULT_BUFLEN 512
#define DEFAULT_COMMAND_PORT "21"
#define DEFAULT_DATA_PORT "20"
#define MAXLINE 4096
#define MAX_SIZE_STRING 200




