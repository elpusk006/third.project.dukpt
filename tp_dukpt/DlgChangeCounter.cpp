// DlgChangeCounter.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "tp_dukpt.h"
#include "DlgChangeCounter.h"
#include "afxdialogex.h"


// CDlgChangeCounter 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgChangeCounter, CDialogEx)

CDlgChangeCounter::CDlgChangeCounter(unsigned char counter[10],CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgChangeCounter::IDD, pParent)
	, m_dwCounter(0)
{
	unsigned char *p = reinterpret_cast<unsigned char*>(&m_dwCounter);
	p[0] =	counter[9];
	p[1] =	counter[8];
	p[2] =	counter[7];

}

CDlgChangeCounter::~CDlgChangeCounter()
{
}

void CDlgChangeCounter::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_COUNTER, m_dwCounter);
	DDV_MinMaxUInt(pDX, m_dwCounter, 0, 0x1fffff);
}


BEGIN_MESSAGE_MAP(CDlgChangeCounter, CDialogEx)
END_MESSAGE_MAP()


// CDlgChangeCounter 메시지 처리기입니다.


BOOL CDlgChangeCounter::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//


	UpdateData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control

}


void CDlgChangeCounter::OnOK()
{
	UpdateData();

	CDialogEx::OnOK();
}

void CDlgChangeCounter::GetCounter( unsigned char counter[10] )
{
	unsigned char *p = reinterpret_cast<unsigned char*>(&m_dwCounter);
	counter[9] = p[0];
	counter[8] = p[1];
	counter[7] = p[2];
}