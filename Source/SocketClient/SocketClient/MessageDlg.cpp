// MessageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SocketClient.h"
#include "MessageDlg.h"
#include "afxdialogex.h"

#include "lib.h"

// MessageDlg dialog

IMPLEMENT_DYNAMIC(MessageDlg, CDialogEx)

MessageDlg::MessageDlg(SOCKET sockClient, int countedMessage, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MESSAGEDLG, pParent), _socketClient(sockClient), _countHad(countedMessage)
	, _clientLog(_T(""))
{

}

MessageDlg::~MessageDlg()
{
}

void MessageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_LBString(pDX, IDC_CLIENTSLOG, _clientLog);
}


BEGIN_MESSAGE_MAP(MessageDlg, CDialogEx)
	ON_BN_CLICKED(IDC_REFRESH, &MessageDlg::OnBnClickedRefresh)
END_MESSAGE_MAP()


// MessageDlg message handlers

void MessageDlg::ServerCloseInMessageDlg()
{
	MessageBox(_T("Server is close!"), _T("ERROR"), 0);
	OnClose(); //
}


void MessageDlg::OnBnClickedRefresh()
{
	// TODO: Add your control notification handler code here
	_lb = (CListBox*)GetDlgItem(IDC_CLIENTSLOG);

	int msgError = sendMessageToServer(_socketClient, COMMAND_SENDMESSAGE);
	if (msgError == 0 || msgError == SOCKET_ERROR)
	{
		ServerCloseInMessageDlg();
		return;
	}

	int Size;

	msgError = receiveMessageFromServer(_socketClient, _countNow);
	if (msgError == 0 || msgError == SOCKET_ERROR)
	{
		ServerCloseInMessageDlg();
		return;
	}

	char buff[MAXMSGCONSOLE];
	//CListBox* lb = (CListBox*)GetDlgItem(IDC_CLIENTSLOG);
	for (int i = 0, ccount = _lb->GetCount(); i < ccount; i++)	_lb->DeleteString(0);


	CString str;
	for (int i = 0; i < _countNow; i++)
	{
		msgError = receiveMessageFromServer(_socketClient, Size);
		if (msgError == 0 || msgError == SOCKET_ERROR)
		{
			ServerCloseInMessageDlg();
			return;
		}
		msgError = receiveDataFromServer(_socketClient, buff, Size);
		if (msgError == 0 || msgError == SOCKET_ERROR)
		{
			ServerCloseInMessageDlg();
			return;
		}

		if (i >= _countHad)
		{
			//str = "\r\n";
			str = "";
			str += (CString)buff;
			//_clientLog += str;
			_lb->AddString(str);
		}
	}


	//UpdateData(false);
}
