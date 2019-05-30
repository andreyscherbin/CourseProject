#undef UNICODE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>    
#include <stdio.h>
#include <conio.h>
#include <cstdlib>
#include <cstdio>
#include <direct.h>
#include <string>
#include <iostream>
#include <fstream>     
#include <stdlib.h> 
#include <vector>
#include <list>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <fcntl.h>
#include <cstring>
#include <string>
#include <time.h>
   
using namespace std;   
#pragma comment (lib, "Ws2_32.lib")
#pragma once
#define DEFAULT_BUFLEN 512
#define COMMAND_BUFLEN 10
#define DEFAULT_COMMAND_PORT "21"
#define DEFAULT_DATA_PORT 20
#define MAXLINE 4096
#define MAX_SIZE_STRING 200
#define BUFFERSIZE 4096
#define LOCALHOST "172,20,10,5"


static const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);   
    strftime(buf, sizeof(buf), "%d-%m-%Y %X", &tstruct);
    return buf;
}




