
// SocketServerDlg.h : header file
//

#pragma once
#include "lib.h"

// CSocketServerDlg dialog
class CSocketServerDlg : public CDialogEx
{
// Construction
public:
	CSocketServerDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SOCKETSERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	//CString _client_log;
	//CString _file_log;
	//CString _client_online;
	CListBox* _lbConnect;
	CListBox* _lbClientLog;
	CListBox* _lbFileLog;
public:
	CSocket _sockServer;
	bool _listening;

	HANDLE th_Handler;
	CWinThread *cTh;
public:
	static UINT __cdecl ParentThreadHelper(LPVOID pParam);
	UINT ParentThreadFunc();
	static UINT __cdecl SocketClientThreadHelper(LPVOID pParam);
	UINT __cdecl SocketClientThread(LPVOID pParam);

	bool ReadFile(MySocket* MSClient);
	bool sendAMessage(MySocket* MSClient);
public:
	afx_msg void OnBnClickedStoplistening();
	afx_msg void OnBnClickedListen();
	afx_msg void OnBnClickedEdit();
};


struct ThreadAndMySocket
{
	CSocketServerDlg* _cssd;
	MySocket* _ms;
};