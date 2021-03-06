#pragma once
#include "stdafx.h"
#include "afxsock.h"
#include <ctime>
#include <string>
#include <string.h>
#include <iostream>
using namespace std;

#define PORTNUMBER 1234 // cùng port với server

#define MAXWRITE 8192
#define MAXROWWRITE 100 // 

#define MAXNAMECLIENT 50
#define MAXPASSCLIENT 50
#define MAXMSGCONSOLE 1000
#define MAXNAMEFILE 50
#define MAXSERVERADDR 1000

#define PING 555

#define ERROR_FINDFILE 404
#define ERROR_INVALIDACCOUNT 200

#define VALIDACCOUNT 002
#define SENDFILE 001
#define REGISTRATION 003
#define LOGIN 004
#define HAS_EXISTED 005
#define REGISTERED 006

#define EXITED 888

#define REQUESTSENDDATAAGAIN 123
#define REQUESTCONTINUESENDDATA 124

#define FILENAMEALLOWEDDOWNLOAD "allowedDownload.txt"

#define COMMAND_READFILE 1210
#define COMMAND_SENDMESSAGE 1211

int sendMessageToServer(SOCKET CSServer, int msg);
int sendDataToServer(SOCKET CSServer, char* buff, int sizeOfData);
int receiveMessageFromServer(SOCKET CSServer, int& msg);
int receiveDataFromServer(SOCKET CSServer, char* buff, int sizeOfData);


int requestLogin(SOCKET sockClient, char* Client_name, char* Client_pass);


bool chooseNameFile(char* namefile);
int requestFile(SOCKET sockClient, char* namefile);


void Hash(char *&str);
int TienDo(long long maxSize, long long loadedSize);
int sendRequestRegister(SOCKET sockClient, char*name, char*pass);