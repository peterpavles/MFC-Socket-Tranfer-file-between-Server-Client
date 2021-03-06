#include "stdafx.h"
#include "lib.h"

#include "afxdialogex.h"




bool SendFile(MySocket *MSClient, FILE* _pRead, char* namefile)
{
	// viết để debug
	char temp[MAXNAMEFILE];
	strcpy(temp, MSClient->_nameClient);
	strcpy(temp + strlen(temp), (char*) "_");
	strcpy(temp + strlen(temp), namefile);
	strcpy(namefile, temp);
	//


	char msg_console[MAXMSGCONSOLE];
	// quá trình gửi file
	// gửi tên file
	fseek(_pRead, 0, SEEK_END);
	int count = strlen(namefile) + 1, msgError, // dem so ki tu trong ten file
		sizeOfFile = ftell(_pRead);
	fseek(_pRead, 0, SEEK_SET);

	msgError = sendMessageToClient(MSClient, count);
	if (msgError == 0 || msgError == SOCKET_ERROR)
	{
		return false;
	}
	msgError = sendDataToClient(MSClient, namefile, count);
	if (msgError == 0 || msgError == SOCKET_ERROR)
	{
		return false;
	}
	msgError = sendMessageToClient(MSClient, sizeOfFile);
	if (msgError == 0 || msgError == SOCKET_ERROR)
	{
		return false;
	}

	// gửi file
	char buf[MAXREAD];
	count = MAXREAD; // dem so bytes doc dc

	CSocket cstemp;
	cstemp.Attach(MSClient->_sockClient);
	do
	{
		count = fread(buf, 1, MAXREAD, _pRead);

		//(CString)buf;

		msgError = cstemp.Send((char*)&count, sizeof(int), 0);
		if (msgError == 0 || msgError == SOCKET_ERROR) break;
		msgError = cstemp.Send(buf, count, 0);
		if (msgError == 0 || msgError == SOCKET_ERROR) break;

	} while (count == MAXREAD);

	MSClient->_sockClient = cstemp.Detach();
	// kiem tra gửi hết file chưa -> client thoát đột ngột
	if (msgError == 0 || msgError == SOCKET_ERROR) return false;
	return true;
}



bool accountHasExisted(char* name, char* pass)
{
	// đăng kí 1 tài khoản mới 
	// ghi thêm vào file account.txt
	ifstream sav;
	sav.open("account.txt");
	if (!sav.fail()) {
		char *user_name;
		string line;

		while (!sav.eof())
		{
			sav >> line;
			const char* InStr = line.c_str();
			user_name = new char[strlen(InStr) + 1];
			strcpy(user_name, InStr);
			sav >> line;

			if (!(strcmp(user_name, name))) {
				delete user_name;
				sav.close();
				return true;
			}
			delete user_name;

		}

		//Tài khoản chưa tồn tại thì lưu tài khoản mới vào cuối file
		sav.close();
	}
	ofstream out;
	out.open("account.txt", ios::app);
	//out.seekp(0, ios::ate);
	out << name << endl;
	out << pass << endl;
	out.close();

	return false;
}
//Nhập tài khoản (username và password)
int receiveAccount(MySocket *MSClient, char *&Client_name, char *&Client_pass)
{
	int count_name, count_pass, msgError;

	msgError = receiveMessageFromClient(MSClient, count_name);
	if (msgError == 0 || msgError == SOCKET_ERROR)// doc ten
		return EXITED;
	Client_name = new char[count_name];
	msgError = receiveDataFromClient(MSClient, Client_name, count_name); // doc ten
	if (msgError == 0 || msgError == SOCKET_ERROR)// doc ten
		return EXITED;

	msgError = receiveMessageFromClient(MSClient, count_pass);
	if (msgError == 0 || msgError == SOCKET_ERROR)// doc pass
		return EXITED;
	Client_pass = new char[count_pass];
	msgError = receiveDataFromClient(MSClient, Client_pass, count_pass); // doc pass
	if (msgError == 0 || msgError == SOCKET_ERROR) return EXITED;

	return 1;
}
bool isValidAccount(char* name, char* pass)
{
	// viết hàm kiểm tra tài khoản có hợp lệ hay không
	// hàm này đọc các dòng tài khoản từ file account.txt
	// kiểm tra có name = name, pass = pass (Lưu ý cả 2 pass đều dc mã hóa rồi)

	ifstream sav;
	sav.open("account.txt");
	if (!sav) return false;

	char* user_name;
	char* user_pass;
	string line;
	sav.seekg(0, ios::beg);
	while (!sav.eof())
	{
		sav >> line;
		const char* InStr = line.c_str();
		user_name = new char[strlen(InStr) + 1];
		strcpy(user_name, InStr);
		sav >> line;
		InStr = line.c_str();
		user_pass = new char[strlen(InStr) + 1];
		strcpy(user_pass, InStr);

		if (!(strcmp(user_name, name)) && !(strcmp(user_pass, pass))) {
			sav.close();
			delete user_name;
			delete user_pass;
			return true;
		}
		delete user_name;
		delete user_pass;
	}

	sav.close();
	return false;
}

bool sendFileAllowedDownload(MySocket *MSClient)
{
	char filename[MAXNAMEFILE];
	strcpy(filename, FILENAMEALLOWEDDOWNLOAD);

	FILE* pread = fopen(FILENAMEALLOWEDDOWNLOAD, "rb");
	if (pread == NULL) return false;
	else
	{
		if (SendFile(MSClient, pread, filename) == false)
		{
			fclose(pread);
			return false;
		}

		fclose(pread);
		return true;
	}
}
bool addFileToAllowedDownload(char* filename)
{
	// kiểm tra xem file filename có tồn tại không
	FILE* check = fopen(filename, "rb");
	if (check == NULL)	return false;
	fclose(check);

	FILE* pwrite = fopen(FILENAMEALLOWEDDOWNLOAD, "at");
	if (pwrite == NULL)
		return false;
	else
	{
		fprintf(pwrite, filename);
		fprintf(pwrite, "\n");
		fclose(pwrite);
		return true;
	}
}
bool dropFileFromAllowedDownload(char* filename)
{
	FILE* pwrite = fopen(FILENAMEALLOWEDDOWNLOAD, "rt");
	if (pwrite == NULL) return false;
	else
	{
		FILE* pwritenew = fopen("newTemp.txt", "wt");
		bool has = false;

		char buf[MAXNAMEFILE];
		char filename_fix[MAXNAMEFILE];
		strcpy(filename_fix, filename);
		strcpy(filename_fix + strlen(filename_fix), "\n");

		while (!feof(pwrite))
		{
			buf[0] = '\0';
			fgets(buf, MAXNAMEFILE, pwrite);
			if (strcmp(buf, filename_fix) != 0)
			{
				fprintf(pwritenew, buf);
			}
			else
			{
				has = true;
			}
		}
		fclose(pwritenew);
		fclose(pwrite);

		remove(FILENAMEALLOWEDDOWNLOAD);
		rename("newTemp.txt", FILENAMEALLOWEDDOWNLOAD);
		return has;
	}
}

int sendMessageToClient(MySocket* MSClient, int msg)
{
	CSocket cstemp;
	cstemp.Attach(MSClient->_sockClient);
	int msgError = cstemp.Send((char*)&msg, sizeof(int), 0);
	MSClient->_sockClient = cstemp.Detach();
	return msgError;
}
int sendDataToClient(MySocket* MSClient, char* buff, int sizeOfData)
{
	CSocket cstemp;
	cstemp.Attach(MSClient->_sockClient);
	int msgError = cstemp.Send(buff, sizeOfData, 0);
	MSClient->_sockClient = cstemp.Detach();
	return msgError;
}
int receiveMessageFromClient(MySocket* MSClient, int& msg)
{
	CSocket cstemp;
	cstemp.Attach(MSClient->_sockClient);
	int msgError = cstemp.Receive((char*)&msg, sizeof(int), 0);
	MSClient->_sockClient = cstemp.Detach();
	return msgError;
}
int receiveDataFromClient(MySocket* MSClient, char* buff, int sizeOfData)
{
	CSocket cstemp;
	cstemp.Attach(MSClient->_sockClient);
	int msgError = cstemp.Receive(buff, sizeOfData, 0);
	MSClient->_sockClient = cstemp.Detach();
	return msgError;
}


void freeMySocket(MySocket*& msClient)
{
	if (msClient != NULL)
	{
		if (msClient->_sockClient != NULL)
		{
			closesocket(msClient->_sockClient);
			msClient->_sockClient = NULL;
		}
		if (msClient->_nameClient != NULL)
		{
			delete[] msClient->_nameClient;
			msClient->_nameClient = NULL;
		}

		delete msClient;
		msClient = NULL;
	}
}


void ghepSoVaoChuoiMsgConsole(char* des, int k)
{
	char buff[12];
	strcpy(des, itoa(k, buff, 10));
}