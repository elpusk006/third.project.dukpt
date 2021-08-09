// InputDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "tp_lpu239_hard.h"
#include "InputDlg.h"
#include "afxdialogex.h"

#include <algorithm>

// CInputDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CInputDlg, CDialogEx)

CInputDlg::CInputDlg( LPCTSTR sTitle, unsigned int nSize/*=0*/, CWnd* pParent /*=NULL*/)
	: CDialogEx(CInputDlg::IDD, pParent), m_nSize(nSize), m_mode(mode_input_hex)
{
	if( sTitle ){
		m_sTitle = sTitle;
	}
	else
		m_sTitle = _T("");
	//
}

CInputDlg::~CInputDlg()
{
}

void CInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_INPUT, m_EditInput);
}


BEGIN_MESSAGE_MAP(CInputDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_TITLE, &CInputDlg::OnBnClickedButtonTitle)
	ON_BN_CLICKED(IDOK, &CInputDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CInputDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CInputDlg 메시지 처리기입니다.


BOOL CInputDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetWindowText( m_sTitle.c_str() );
	m_EditInput.SetWindowText( get_Input().c_str() );

	if( m_mode == mode_input_ascii )	GetDlgItem( IDC_BUTTON_TITLE )->SetWindowText( _T("Please Input.( ASCII format )") );
	else										GetDlgItem( IDC_BUTTON_TITLE )->SetWindowText( _T("Please Input.( HEX format )") );
	//
	m_vInput.resize(0);

	return TRUE;  // return TRUE unless you set the focus to a control

}


void CInputDlg::OnBnClickedButtonTitle()
{
	if( m_mode == mode_input_hex ){
		m_mode = mode_input_ascii;
		GetDlgItem( IDC_BUTTON_TITLE )->SetWindowText( _T("Please Input.( ASCII format )") );
	}
	else{
		m_mode = mode_input_hex;
		GetDlgItem( IDC_BUTTON_TITLE )->SetWindowText( _T("Please Input.( HEX format )") );
	}
}


void CInputDlg::OnBnClickedOk()
{
	CString stemp;
	m_EditInput.GetWindowText( stemp );

	m_vInput.resize(0);

	_tstring sIn = stemp;

	if( m_mode == mode_input_hex ){
		if( sIn.size() % 2 != 0 || sIn.empty() ){
			stemp.Format( _T(" the lenght of input must be multiples of two.( current - %d )"), sIn.size()  );
			MessageBox( stemp, _T(" [ ERROR ] ") ); 
			return;
		}

		if( (sIn.size()/2) != m_nSize && m_nSize != 0 ){
			stemp.Format( _T("the lenght of input must be %d.( current - %d )"), m_nSize*2, sIn.size()  );
			MessageBox( stemp, _T(" [ ERROR ] ") ); 
			return;
		}

		_tstring::size_type found = sIn.find_first_not_of( _T("0123456789abcdefABCDEF") );
		if( found != _tstring::npos ){
			stemp.Format( _T("the input must be hexcimal format.")  );
			MessageBox( stemp, _T(" [ ERROR ] ") ); 
			return;
		}

		std::transform( sIn.begin(), sIn.end(), sIn.begin(), std::toupper );
		set_Input( sIn );
		//

		for( auto i=0; i<sIn.size()/2; ++i ){
			unsigned char ch = static_cast<unsigned char>(sIn[ 2*i ]);
			unsigned char cl = static_cast<unsigned char>(sIn[ 2*i+1 ]);

			if( ch >= '0' && ch <='9' ){		ch -= '0';			}
			else if( ch >= 'A' && ch <='F' ){	ch -= 'A';		ch += 10;		}

			ch <<= 4;
			//
			if( cl >= '0' && cl <='9' ){			cl -= '0';		}
			else if( cl >= 'A' && cl <='F' ){	cl -= 'A';		cl += 10;			}

			m_vInput.push_back( ch | cl );
		}//end for
	}
	else{//ASCII format mode... here
		if( sIn.size() != m_nSize && m_nSize != 0 ){
			stemp.Format( _T("the lenght of input must be %d.( current - %d )"), m_nSize, sIn.size()  );
			MessageBox( stemp, _T(" [ ERROR ] ") ); 
			return;
		}

		set_Input( sIn );

		for_each( sIn.begin(), sIn.end(), [&]( CDev::typeBuffer::value_type c ){
			m_vInput.push_back( static_cast<unsigned char>( c ) );
		});
	}

	CDialogEx::OnOK();
}


void CInputDlg::OnBnClickedCancel()
{
	// TODO: 
	set_Input( _tstring() );

	CDialogEx::OnCancel();
}
