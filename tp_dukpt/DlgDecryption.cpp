// DlgDecryption.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "tp_dukpt.h"
#include "DlgDecryption.h"
#include "afxdialogex.h"


// CDlgDecryption ��ȭ �����Դϴ�.

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


// CDlgDecryption �޽��� ó�����Դϴ�.


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
