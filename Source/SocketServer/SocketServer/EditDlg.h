#pragma once


// EditDlg dialog

class EditDlg : public CDialogEx
{
	DECLARE_DYNAMIC(EditDlg)

public:
	EditDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~EditDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EDIT_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CString _AddFileName;
	CString _DropFileName;

public:
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedDrop();
};
