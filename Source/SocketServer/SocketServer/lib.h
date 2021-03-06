#pragma once
#include "stdafx.h"
#include "afxsock.h"
#include "math.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

#define PORTNUMBER 1234

#define MAXREAD 8192
#define MAXROWREAD 100 //

#define MAXNAMECLIENT 50
#define MAXNAMEFILE 50
#define MAXMSGCONSOLE 1000

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
#define SERVEREXIT 9549

#define REQUESTSENDDATAAGAIN 123
#define REQUESTCONTINUESENDDATA 124

#define FILENAMEALLOWEDDOWNLOAD "allowedDownload.txt"

#define COMMAND_READFILE 1210
#define COMMAND_SENDMESSAGE 1211

struct MySocket
{
	SOCKET _sockClient;
	char* _nameClient;
};

bool SendFile(MySocket* MSClient, FILE* _pRead, char* namefile);
//bool ReadFile(MySocket* MSClient); chuyen qua class dlg


bool isValidAccount(char* name, char* pass);
int receiveAccount(MySocket *MSClient, char *&Client_name, char *&Client_pass);
bool accountHasExisted(char* name, char* pass);

bool sendFileAllowedDownload(MySocket* MSClient);
bool addFileToAllowedDownload(char* filename);
bool dropFileFromAllowedDownload(char* filename);

int sendMessageToClient(MySocket* MSClient, int msg);
int sendDataToClient(MySocket* MSClient, char* buff, int sizeOfData);
int receiveMessageFromClient(MySocket* MSClient, int& msg);
int receiveDataFromClient(MySocket* MSClient, char* buff, int sizeOfData);


void freeMySocket(MySocket*& msClient);

void ghepSoVaoChuoiMsgConsole(char* des, int k);