// DlgBase64.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "tp_dukpt.h"
#include "DlgBase64.h"
#include "afxdialogex.h"
#include "etc.h"
#include "warp_idt_dukpt.h"

using namespace __etc;


// CDlgBase64 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgBase64, CDialogEx)

CDlgBase64::CDlgBase64(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgBase64::IDD, pParent)
	, m_sIn(_T(""))
	, m_sOut(_T(""))
	, m_bEncodeMode( true )
{

}

CDlgBase64::~CDlgBase64()
{
}

void CDlgBase64::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_IN, m_sIn);
	DDX_Text(pDX, IDC_EDIT_OUT, m_sOut);
}


BEGIN_MESSAGE_MAP(CDlgBase64, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgBase64::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgBase64 메시지 처리기입니다.


BOOL CDlgBase64::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetWindowText( m_sTitle );
	GetDlgItem( IDOK )->SetWindowText( m_sDoButtonText );

	return TRUE;  // return TRUE unless you set the focus to a control
}


void CDlgBase64::OnBnClickedOk()
{
	unsigned int nLen = 0;;
	typeBytes byteOut;
	// TODO:
	UpdateData();

	if( m_bEncodeMode ){

		typeBytes byteEncode;

		if( !HexsToBytes( byteEncode, m_sIn ) ){
			return;
		}

		//void encode_base64( unsigned char *p_out, unsigned int *pn_out, const unsigned char *p_in, const unsigned int n_in )
		

		CWarp_idt_dukpt::get_security_object()->encode_base64( NULL, &nLen, &byteEncode[0], byteEncode.size() );

		if( nLen==0 ){
			return;
		}

		byteOut.resize( nLen, 0 );

		CWarp_idt_dukpt::get_security_object()->encode_base64( &byteOut[0], &nLen, &byteEncode[0], byteEncode.size() );

		BytesToHexs( m_sOut, &byteOut[0], byteOut.size() );

		m_sOut +=_T('(');

		TCHAR t;

		for( auto i=0; i< byteOut.size(); i++ ){
			t = (TCHAR)(byteOut[i]);
			m_sOut += t;
		}

		m_sOut +=_T(')');
	}
	else{//decoding mode.
		typeBytes byteDecode;

		if( !HexsToBytes( byteDecode, m_sIn ) ){
			return;
		}

		unsigned int nLen(0);
		CWarp_idt_dukpt::get_security_object()->decode_base64( NULL, &nLen, &byteDecode[0], byteDecode.size() );

		if( nLen == 0 ){
			return;
		}

		typeBytes byteOut( nLen, 0 );
		CWarp_idt_dukpt::get_security_object()->decode_base64( &byteOut[0], &nLen, &byteDecode[0], byteDecode.size() );
		BytesToHexs( m_sOut, &byteOut[0], byteOut.size() );
	}



	UpdateData( FALSE );


	//CDialogEx::OnOK();
}


void CDlgBase64::OnCancel()
{
	UpdateData();

	CDialogEx::OnCancel();
}
