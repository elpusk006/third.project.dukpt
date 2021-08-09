// DlgDecryption.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "tp_dukpt.h"
#include "DlgDecryption.h"
#include "afxdialogex.h"


// CDlgDecryption 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgDecryption, CDialogEx)

CDlgDecryption::CDlgDecryption(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgDecryption::IDD, pParent)
	, m_sEncrytionText(_T(""))
{

}

CDlgDecryption::~CDlgDecryption()
{
}

void CDlgDecryption::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ENCRYPTION, m_sEncrytionText);
}


BEGIN_MESSAGE_MAP(CDlgDecryption, CDialogEx)
END_MESSAGE_MAP()


// CDlgDecryption 메시지 처리기입니다.


BOOL CDlgDecryption::OnInitDialog()
{
	CDialogEx::OnInitDialog();



	return TRUE;  // return TRUE unless you set the focus to a control

}


void CDlgDecryption::OnOK()
{
	UpdateData();

	CDialogEx::OnOK();
}
