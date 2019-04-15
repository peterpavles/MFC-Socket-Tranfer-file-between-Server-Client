#pragma once


// MessageDlg dialog

class MessageDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MessageDlg)

public:
	MessageDlg(SOCKET sClient, int countedMessage, CWnd* pParent = nullptr);   // standard constructor
	virtual ~MessageDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MESSAGEDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	SOCKET _socketClient;
	int _countHad, _countNow;

	CListBox* _lb;
public:
	void ServerCloseInMessageDlg();
	afx_msg void OnBnClickedRefresh();
	CString _clientLog;
};
