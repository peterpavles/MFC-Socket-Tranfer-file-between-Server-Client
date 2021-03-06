
// SocketServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SocketServer.h"
#include "SocketServerDlg.h"
#include "afxdialogex.h"

#include "EditDlg.h"
#include "lib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSocketServerDlg dialog



CSocketServerDlg::CSocketServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SOCKETSERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//_client_log = _T("");
	//_file_log = _T("");
	//_client_online = _T("");
}

void CSocketServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Text(pDX, IDC_CLIENTLOG, _client_log);
	//DDX_Text(pDX, IDC_FILELOG, _file_log);
	//DDX_Text(pDX, IDC_CONNECTINGCLIENT, _client_online);
}

BEGIN_MESSAGE_MAP(CSocketServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_STOPLISTENING, &CSocketServerDlg::OnBnClickedStoplistening)
	ON_BN_CLICKED(IDC_LISTEN, &CSocketServerDlg::OnBnClickedListen)
	ON_BN_CLICKED(IDC_EDIT, &CSocketServerDlg::OnBnClickedEdit)
END_MESSAGE_MAP()


// CSocketServerDlg message handlers

BOOL CSocketServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	AfxSocketInit(NULL);
	GetDlgItem(IDC_STOPLISTENING)->EnableWindow(FALSE);

	_lbConnect = (CListBox*)GetDlgItem(IDC_CONNECTINGCLIENT);
	_lbClientLog = (CListBox*)GetDlgItem(IDC_CLIENTLOG);
	_lbFileLog = (CListBox*)GetDlgItem(IDC_FILELOG);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSocketServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSocketServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSocketServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSocketServerDlg::OnBnClickedStoplistening()
{
	// TODO: Add your control notification handler code here
	AfxSocketInit(NULL);
	_listening = false;
	UpdateData();

	//
	CSocket socketClose;
	socketClose.Create();
	socketClose.Connect(CA2W("127.0.0.1"), PORTNUMBER);
	socketClose.Close();
	//
	::WaitForSingleObject(cTh->m_hThread, INFINITE);


	GetDlgItem(IDC_LISTEN)->EnableWindow(TRUE);
	GetDlgItem(IDC_STOPLISTENING)->EnableWindow(FALSE);

	//OnCancel(); //
}


void CSocketServerDlg::OnBnClickedListen()
{
	// TODO: Add your control notification handler code here
	//_client_log = _T("");
	//_file_log = _T("");
	//_client_online = _T("");
	UpdateData();
	// tao thread accept
	cTh = AfxBeginThread(ParentThreadHelper, this);
	th_Handler = cTh->m_hThread;
}

UINT CSocketServerDlg::ParentThreadFunc()
{
	AfxSocketInit(NULL);

	// khởi tạo 
	if (AfxSocketInit(NULL) == false)
	{
		MessageBox(_T("Can't initialize Socket Libraray"), _T("ERROR"), 0);
		return 0;
	}

	if (_sockServer.Create(PORTNUMBER) == 0) // chọn giao thức TCP	
	{
		MessageBox(_T("Failed to initialize!!!"), _T("ERROR"), 0);
		_sockServer.GetLastError();
		return 0;
	}

	if (_sockServer.Bind(PORTNUMBER) == -1)
	{
		MessageBox(_T("Failed to bind!!!"), _T("ERROR"), 0);
		_sockServer.Close();
		return 0;
	}

	if (_sockServer.Listen(5) == false)
	{
		MessageBox(_T("Can't listen in this port!!!"), _T("ERROR"), 0);
		_sockServer.Close();
		return 0;
	}

	GetDlgItem(IDC_LISTEN)->EnableWindow(FALSE);
	GetDlgItem(IDC_STOPLISTENING)->EnableWindow(TRUE);

	_listening = true;


	// accept thread - old
	vector<MySocket*> msClients;
	int num_client = 0;
	MySocket* mstemp;

	do
	{
		CSocket tempClient;
		if (_sockServer.Accept(tempClient))
		{
			mstemp = new MySocket;
			mstemp->_sockClient = tempClient.Detach();
			mstemp->_nameClient = NULL;

			if (_listening == false)
			{		
				_sockServer.Close();
				break;
			}
			else
			{
				num_client++;
				msClients.push_back(mstemp);

				// tạo thread cho mỗi client
				ThreadAndMySocket tms;
				tms._cssd = this;
				tms._ms = mstemp;
				AfxBeginThread(SocketClientThreadHelper, (LPVOID)&tms);
			}
		}
		else
		{
			MessageBox(_T("Server is disconnect!"), _T("ERROR"), 0);
			break;
		}
	} while (1);

	// hoàn thành công việc
	//for (int i = 0; i < num_client; i++)
	//{
	//	freeMySocket(msClients[i]);
	//}

	return 0;
}

UINT __cdecl CSocketServerDlg::ParentThreadHelper(LPVOID pParam)
{
	CSocketServerDlg *pYourClass = reinterpret_cast<CSocketServerDlg*>(pParam);
	pYourClass->ParentThreadFunc();
	return 0;
}

UINT __cdecl CSocketServerDlg::SocketClientThread(LPVOID pParam)
{
	AfxSocketInit(NULL);

	MySocket *msClient = reinterpret_cast<MySocket*>(pParam);

	int msg_code, msg_error = 0, mERROR;
	char* Client_name, *Client_pass;

	Client_name = Client_pass = NULL;

	while (1)
	{
		mERROR = receiveMessageFromClient(msClient, msg_code);
		if (mERROR == 0 || mERROR == SOCKET_ERROR)
		{
			msg_code = EXITED;
			break;
		}

		if (msg_code == REGISTRATION)
		{

			msg_error = receiveAccount(msClient, Client_name, Client_pass);
			if (msg_error == EXITED) // client exit
			{
				break;
			}
			else
			{
				if (accountHasExisted(Client_name, Client_pass))
				{
					mERROR = sendMessageToClient(msClient, HAS_EXISTED);
					if (mERROR == 0 || mERROR == SOCKET_ERROR)
					{
						msg_code = EXITED;
						break;
					}
				}
				else
				{
					mERROR = sendMessageToClient(msClient, REGISTERED);
					if (mERROR == 0 || mERROR == SOCKET_ERROR)
					{
						msg_code = EXITED;
					}
				}
			}
		}
		else if (msg_code == LOGIN)
		{

			msg_error = receiveAccount(msClient, Client_name, Client_pass);

				
			if (msg_code == EXITED)
			{
				break;
			}
			else
			{
				//
				//MessageBox((CString)Client_name, _T("ERROR"), 0);
				//MessageBox((CString)Client_pass, _T("ERROR"), 0);
				//

				if (isValidAccount(Client_name, Client_pass) == true) {
					msClient->_nameClient = Client_name;

					mERROR = sendMessageToClient(msClient, VALIDACCOUNT);
					if (mERROR == 0 || mERROR == SOCKET_ERROR)
					{
						msg_code = EXITED;
					}
					break;
				}
				else
				{
					mERROR = sendMessageToClient(msClient, ERROR_INVALIDACCOUNT);
					if (mERROR == 0 || mERROR == SOCKET_ERROR)
					{
						msg_code = EXITED;
						break;
					}
				}
			}
		}

		if (msg_code == EXITED || msg_error == EXITED)
		{
			break;
		}
	}

	if (msg_code == LOGIN && msg_error != EXITED)
	{

		CString buff = _T("");
		// thong bao đã tiếp nhận client len man hinh
		//if (_client_log.GetLength() > 0) buff +="\r\n-----\r\n";
		buff = Client_name;
		buff += " login"; // dang viet
		//_client_log += buff;
		_lbClientLog->AddString(buff);


		//buff = _T("");
		//if (_client_online.GetLength() > 0) buff ="\r\n-----\r\n";
		buff = Client_name;
		//_client_online += buff;
		_lbConnect->AddString(buff);

		int er = sendMessageToClient(msClient, _lbClientLog->GetCount());

		if (er != 0 && er != SOCKET_ERROR && mERROR != 0 && mERROR != SOCKET_ERROR)
		{
			// phiên làm việc của 1 client dc phép
			// gửi tệp các file dc phép download
			if (sendFileAllowedDownload(msClient) == false)
			{
				CString buff = _T("");
				// dang viet
				//if (_file_log.GetLength() > 0) buff +="\r\n-----\r\n";
				buff += "Failed to read/send file ";
				buff += FILENAMEALLOWEDDOWNLOAD;
				buff += " to client ";
				buff += msClient->_nameClient;
				//_file_log += buff;
				_lbFileLog->AddString(buff);
				//
				/*CWnd *pEditField = GetDlgItem(IDC_FILELOG);
				if (pEditField != NULL)
					pEditField->SetWindowText(_file_log);*/
				//UpdateData();
			}
			else
			{
				// nhận lệnh cần thực hiện từ client
				int command, er;
				while (1)
				{
					command = 0;
					er = receiveMessageFromClient(msClient, command);
					if (er == 0 || er == SOCKET_ERROR)
						break;

					if(command == COMMAND_READFILE)
						if(ReadFile(msClient) == false)
							break;
					if (command == COMMAND_SENDMESSAGE)
						if (sendAMessage(msClient) == false)
							break;
				}
			}
			// xong cong viec cua socketclient


		}
		else
		{
			// khong dang nhap
			// "----- Client exit!!! ----\n"
		}
	}
	else
	{
		// khong dang nhap
		// "----- Client exit!!! ----\n"
	}

	if (msg_code == EXITED || msg_error == EXITED)
	{
	}
	else
	{
		CString buff = _T("");
		//if (_client_log.GetLength() > 0) buff +="\r\n-----\r\n";
		buff = Client_name;
		buff += " logoff"; // dang viet
		//_client_log += buff;
		_lbClientLog->AddString(buff);

		////buff ="\r\n\r\n";
		buff = Client_name;
		//_client_online.Replace(buff, _T(""));

		int pos = _lbConnect->FindStringExact(0, buff);
		if(pos != LB_ERR)
			_lbConnect->DeleteString(pos);


	}


	delete[] Client_pass;
	freeMySocket(msClient);
	return 0;
}
UINT __cdecl CSocketServerDlg::SocketClientThreadHelper(LPVOID pParam)
{
	ThreadAndMySocket *pYourClass = reinterpret_cast<ThreadAndMySocket*>(pParam);
	pYourClass->_cssd->SocketClientThread(pYourClass->_ms);
	return 0;
}



bool CSocketServerDlg::ReadFile(MySocket* MSClient)
{
	char msgError;
	char namefile[MAXNAMEFILE];
	int count;

	msgError = receiveMessageFromClient(MSClient, count);
	if (msgError == 0 || msgError == SOCKET_ERROR)
	{
		return false;
	}
	msgError = receiveDataFromClient(MSClient, namefile, count);
	if (msgError == 0 || msgError == SOCKET_ERROR)
	{
		return false;
	}

	FILE* _pRead = fopen(namefile, "rb");


	if (_pRead == NULL)
	{
		// báo cho client là k tìm thấy file
		msgError = sendMessageToClient(MSClient, ERROR_FINDFILE);
		if (msgError == 0 || msgError == SOCKET_ERROR)
		{
			return false;
		}
	}
	else
	{
		// báo cho client chuẩn bị gửi file
		msgError = sendMessageToClient(MSClient, SENDFILE);
		if (msgError == 0 || msgError == SOCKET_ERROR)
		{
			fclose(_pRead);
			return false;
		}

		CString buff = _T("");
		if (SendFile(MSClient, _pRead, namefile) == false)
		{
			//if (_file_log.GetLength() > 0) buff +="\r\n-----\r\n";
			buff += "Failed to send file ";
			buff += namefile;
			buff += " to client ";
			buff += MSClient->_nameClient;
			//_file_log += buff;
			_lbFileLog->AddString(buff);
		}
		else
		{
			//if (_file_log.GetLength() > 0) buff ="\r\n------\r\n";
			buff += "Send file ";
			buff += namefile;
			buff += " to client ";
			buff += MSClient->_nameClient;
			buff += " successfully";
			//_file_log += buff;
			_lbFileLog->AddString(buff);
		}

		// xong việc gửi nhận
		fclose(_pRead);
		return true;
	}
}
bool CSocketServerDlg::sendAMessage(MySocket* MSClient)
{
	int msgError, count, Size;
	CString str;
	count = _lbClientLog->GetCount();
	
	msgError = sendMessageToClient(MSClient, count);
	if (msgError == 0 || msgError == SOCKET_ERROR)
	{
		return false;
	}

	char buff[MAXMSGCONSOLE];

	for (int i = 0; i < count; i++)
	{
		_lbClientLog->GetText(i, str);
		Size = str.GetLength() + 1;
		wcstombs(buff, str, Size);
		
		msgError = sendMessageToClient(MSClient, Size);
		if (msgError == 0 || msgError == SOCKET_ERROR)
		{
			return false;
		}
		msgError = sendDataToClient(MSClient, buff, Size);
		if (msgError == 0 || msgError == SOCKET_ERROR)
		{
			return false;
		}
	}

	return true;
}

void CSocketServerDlg::OnBnClickedEdit()
{
	// TODO: Add your control notification handler code here
	AfxSocketInit(NULL);
	EditDlg eddlg;
	eddlg.DoModal();
}