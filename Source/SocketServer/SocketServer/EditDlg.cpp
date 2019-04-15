// EditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SocketServer.h"
#include "EditDlg.h"
#include "afxdialogex.h"

#include "lib.h"

// EditDlg dialog

IMPLEMENT_DYNAMIC(EditDlg, CDialogEx)

EditDlg::EditDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EDIT_DLG, pParent)
{

}

EditDlg::~EditDlg()
{
}

void EditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_BOXADD, _AddFileName);
	DDX_Text(pDX, IDC_BOXDROP, _DropFileName);
}


BEGIN_MESSAGE_MAP(EditDlg, CDialogEx)
	ON_BN_CLICKED(IDC_ADD, &EditDlg::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_DROP, &EditDlg::OnBnClickedDrop)
END_MESSAGE_MAP()


// EditDlg message handlers


void EditDlg::OnBnClickedAdd()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if (_AddFileName.GetLength() == 0)
	{
		MessageBox(_T("Empty filename is not legal!"), _T("ERROR"), 0);
	}
	else
	{
		char CFileName[MAXNAMEFILE + 1];
		wcstombs(CFileName, _AddFileName, _AddFileName.GetLength() + 1);
		if(addFileToAllowedDownload(CFileName))
			MessageBox(_T("Add successfully!"), _T("SUCCESS"), 0);
		else
			MessageBox(_T("Add unsuccessfully!"), _T("ERROR"), 0);
	}
}


void EditDlg::OnBnClickedDrop()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if (_DropFileName.GetLength() == 0)
	{
		MessageBox(_T("Empty filename is not legal!"), _T("ERROR"), 0);
	}
	else
	{
		char CFileName[MAXNAMEFILE + 1];
		wcstombs(CFileName, _DropFileName, _DropFileName.GetLength() + 1);
		if (dropFileFromAllowedDownload(CFileName))
			MessageBox(_T("Drop successfully!"), _T("SUCCESS"), 0);
		else
			MessageBox(_T("Drop unsuccessfully!"), _T("ERROR"), 0);
	}
}
