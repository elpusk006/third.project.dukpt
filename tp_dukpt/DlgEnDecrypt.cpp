// DlgEnDecrypt.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "tp_dukpt.h"
#include "DlgEnDecrypt.h"
#include "afxdialogex.h"


// CDlgEnDecrypt 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgEnDecrypt, CDialogEx)

CDlgEnDecrypt::CDlgEnDecrypt(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgEnDecrypt::IDD, pParent)
	, m_sKey(_T(""))
	, m_sData(_T(""))
{

}

CDlgEnDecrypt::~CDlgEnDecrypt()
{
}

void CDlgEnDecrypt::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_KEY, m_sKey);
	DDX_Text(pDX, IDC_EDIT_DATA, m_sData);
}


BEGIN_MESSAGE_MAP(CDlgEnDecrypt, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgEnDecrypt::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgEnDecrypt 메시지 처리기입니다.


void CDlgEnDecrypt::OnBnClickedOk()
{
	UpdateData();

	CDialogEx::OnOK();
}
