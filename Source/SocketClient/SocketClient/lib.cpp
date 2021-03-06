#include "stdafx.h"
#include "lib.h"


int sendMessageToServer(SOCKET CSServer, int msg)
{
	if (CSServer == NULL) return SOCKET_ERROR;
	CSocket cstemp;
	cstemp.Attach(CSServer);
	int msgError = cstemp.Send((char*)&msg, sizeof(int), 0);
	CSServer = cstemp.Detach();
	return msgError;
}
int sendDataToServer(SOCKET CSServer, char* buff, int sizeOfData)
{
	if (CSServer == NULL) return SOCKET_ERROR;
	CSocket cstemp;
	cstemp.Attach(CSServer);
	int msgError = cstemp.Send(buff, sizeOfData, 0);
	CSServer = cstemp.Detach();
	return msgError;
}
int receiveMessageFromServer(SOCKET CSServer, int& msg)
{
	if (CSServer == NULL) return SOCKET_ERROR;
	CSocket cstemp;
	cstemp.Attach(CSServer);
	int msgError = cstemp.Receive((char*)&msg, sizeof(int), 0);
	CSServer = cstemp.Detach();
	return msgError;
}
int receiveDataFromServer(SOCKET CSServer, char* buff, int sizeOfData)
{
	if (CSServer == NULL) return SOCKET_ERROR;
	CSocket cstemp;
	cstemp.Attach(CSServer);
	int msgError = cstemp.Receive(buff, sizeOfData, 0);
	CSServer = cstemp.Detach();
	return msgError;
}



int requestLogin(SOCKET sockClient, char* Client_name, char* Client_pass)
{
	int msgFromServer, msgError, count_name, count_pass;

	count_name = strlen(Client_name) + 1;
	count_pass = strlen(Client_pass) + 1;

	msgError = sendMessageToServer(sockClient, count_name); // gui ten
	if (msgError == 0 || msgError == SOCKET_ERROR) return msgError;
	msgError = sendDataToServer(sockClient, Client_name, count_name); // gui ten
	if (msgError == 0 || msgError == SOCKET_ERROR) return msgError;

	msgError = sendMessageToServer(sockClient, count_pass); // gui pass
	if (msgError == 0 || msgError == SOCKET_ERROR) return msgError;
	msgError = sendDataToServer(sockClient, Client_pass, count_pass); // gui pass
	if (msgError == 0 || msgError == SOCKET_ERROR) return msgError;

	msgError = receiveMessageFromServer(sockClient, msgFromServer); // nhận thông báo tài khoản hợp lệ không
	if (msgError == 0 || msgError == SOCKET_ERROR) return msgError;
	return msgFromServer;
}

bool chooseNameFile(char* namefile)
{
	// chọn 1 tên file từ tệp Client'sName_allowedDownload.txt
	// và lưu vào namefile

	FILE* pRead = fopen(FILENAMEALLOWEDDOWNLOAD, "rt");
	if (pRead == NULL) return 0;
	// file rỗng
	fseek(pRead, SEEK_END, 0);
	if (ftell(pRead) == 0)
	{
		fclose(pRead);
		return 0;
	}
	fseek(pRead, SEEK_SET, 0);
	//
	char buf[MAXNAMEFILE];


	do
	{

		while (!feof(pRead))
		{
			buf[0] = '\0';
			fgets(buf, MAXNAMEFILE, pRead);
			if (strcmp(buf, namefile) == 0)
				break;
		}
		if (feof(pRead))
		{
			fclose(pRead);
			return false;
		}
		else
			break;
	} while (1);
	fclose(pRead);

	if (strlen(namefile) == 0) return 0;
	namefile[strlen(namefile) - 1] = '\0';
	return 1;
}
int requestFile(SOCKET sockClient, char* namefile)
{
	int msgFromServer, count = strlen(namefile) + 1, msgError;
	msgError = sendMessageToServer(sockClient, count);
	if (msgError == 0 || msgError == SOCKET_ERROR) return msgError;
	msgError = sendDataToServer(sockClient, namefile, count);
	if (msgError == 0 || msgError == SOCKET_ERROR) return msgError;

	// nhận thông báo file có tìm thấy không
	msgError = receiveMessageFromServer(sockClient, msgFromServer);
	if (msgError == 0 || msgError == SOCKET_ERROR) return msgError;
	return msgFromServer;
}




void Hash(char *&str)
{
	for (int i = 0; i < strlen(str); i++)
		str[i] += str[i + 1];

	unsigned long hash = 5381;
	char ch = 0;
	string str1 = "internet";
	int i = 0;
	while (i != strlen(str))
	{
		do
		{
			ch = (ch + ((hash << 2) + hash) + *(str + i)) % 175;
		} while (ch < 25 || ch > 150);

		if (i % 3 == 0)	str1 += ch;
		else if (i % 3 == 1) {
			string temp;
			temp = ch;
			str1.insert(str1.length() / 2, temp);
		}
		else str1 = ch + str1;
		i++;
	}

	for (int i = 0; i < str1.length(); i++) {
		if (str1[i] == ' ') str1[i] = '.';
		if (str1[i] == '\n') str1[i] == ',';
	}
	const char* InStr = str1.c_str();
	
	free(str);
	str = (char*)malloc(str1.length() + 1);
	strcpy(str, InStr);
}
int TienDo(long long maxSize, long long loadedSize)
{
	// trả về % tiến độ hoàn thành
	// maxSize là kích thước của file
	// loadedSize là kích thước đã load dc
	return (loadedSize * 100 ) / maxSize;
}

int sendRequestRegister(SOCKET sockClient, char*name, char*pass)
{
	// gửi đăng kí 1 tài khoản mới 
	// gửi yêu cầu cho server để đăng kí
	int msgFromServer, count_name, count_pass;

	count_name = strlen(name) + 1;
	count_pass = strlen(pass) + 1;

	msgFromServer = sendMessageToServer(sockClient, count_name); // gui ten
	if(msgFromServer == 0 || msgFromServer == SOCKET_ERROR)	return EXITED;
	msgFromServer = sendDataToServer(sockClient, name, count_name); // gui ten
	if (msgFromServer == 0 || msgFromServer == SOCKET_ERROR) return EXITED;

	msgFromServer = sendMessageToServer(sockClient, count_pass); // gui pass
	if (msgFromServer == 0 || msgFromServer == SOCKET_ERROR) return EXITED;
	msgFromServer = sendDataToServer(sockClient, pass, count_pass); // gui pass
	if (msgFromServer == 0 || msgFromServer == SOCKET_ERROR) return EXITED;

	if (receiveMessageFromServer(sockClient, msgFromServer) <= 0) // nhận thông báo tài khoản hợp lệ không
		return EXITED;

	return msgFromServer;
}