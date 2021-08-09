
// tp_dukptDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "tp_dukpt.h"
#include "tp_dukptDlg.h"
#include "afxdialogex.h"

#include <algorithm>
#include <iterator>

#include "DlgDecryption.h"
#include "warp_idt_dukpt.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Ctp_dukptDlg 대화 상자




Ctp_dukptDlg::Ctp_dukptDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Ctp_dukptDlg::IDD, pParent)
	, m_sBDK(_T("0123456789ABCDEFFEDCBA9876543210"))
	, m_sKSN_ini(_T("9876543210E00000"))
	, m_sIPEK(_T(""))
	, m_sSessionKey(_T(""))
	, m_sPinEncryptionKey(_T(""))
	, m_sMacReqEncryptionKey(_T(""))
	, m_sMacRespEncryptionKey(_T(""))
	, m_sDataReqEncryptionKey(_T(""))
	, m_sDataRespEncryptionKey(_T(""))
	, m_sKSN_cur(_T(""))
	, m_sPlainData_hex(_T("31323334"))
	, m_sPlainData_ascii(_T("4012345678909D987"))
	, m_sPlainMsg_hex(_T("31323334"))
	, m_sPlainMsg_ascii(_T("4012345678909D987"))
	, m_sPlainPin_hex(_T("041274EDCBA9876F"))
	, m_sPlainPin_ascii(_T(""))
	, m_pPlainData(&m_sPlainData_ascii)
	, m_pPlainMsg(&m_sPlainMsg_ascii)
	, m_pPlainPin(&m_sPlainPin_hex)
	, m_sXor1(_T(""))
	, m_sXor2(_T("F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0"))
	, m_sXorResult(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Ctp_dukptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_BDK, m_sBDK);
	DDV_MaxChars(pDX, m_sBDK, 32);
	DDX_Text(pDX, IDC_EDIT_KSN_INI, m_sKSN_ini);
	DDV_MaxChars(pDX, m_sKSN_ini, 20);
	DDX_Text(pDX, IDC_EDIT_IPEK, m_sIPEK);
	DDV_MaxChars(pDX, m_sIPEK, 32);
	DDX_Text(pDX, IDC_EDIT_SESSION_KEY, m_sSessionKey);
	DDV_MaxChars(pDX, m_sSessionKey, 32);
	DDX_Control(pDX, IDC_LIST_INFO, m_ListInfo);
	DDX_Text(pDX, IDC_EDIT_REAL_SESSION_KEY, m_sPinEncryptionKey);
	DDX_Text(pDX, IDC_EDIT_MAC_REQ_EN_KEY, m_sMacReqEncryptionKey);
	DDX_Text(pDX, IDC_EDIT_MAC_RESP_EN_KEY, m_sMacRespEncryptionKey);
	DDX_Text(pDX, IDC_EDIT_DATA_REQ_EN_KEY, m_sDataReqEncryptionKey);
	DDX_Text(pDX, IDC_EDIT_DATA_RESP_EN_KEY, m_sDataRespEncryptionKey);
	DDV_MaxChars(pDX, m_sPinEncryptionKey, 32);
	DDV_MaxChars(pDX, m_sMacReqEncryptionKey, 32);
	DDV_MaxChars(pDX, m_sMacRespEncryptionKey, 32);
	DDV_MaxChars(pDX, m_sDataReqEncryptionKey, 32);
	DDV_MaxChars(pDX, m_sDataRespEncryptionKey, 32);
	DDX_Text(pDX, IDC_EDIT_KSN_CUR, m_sKSN_cur);
	DDX_Text(pDX, IDC_EDIT_PLAIN_DATA, *m_pPlainData);
	DDX_Text(pDX, IDC_EDIT_PLAIN_MSG, *m_pPlainMsg);
	DDX_Text(pDX, IDC_EDIT_PLAIN_PIN, *m_pPlainPin);
	DDX_Control(pDX, IDC_CHECK_PLAIN_DATA_IS_ASCII, m_CheckPlainDataIsAscii);
	DDX_Control(pDX, IDC_CHECK_PLAIN_MSG_IS_ASCII, m_CheckPlainMsgIsAscii);
	DDX_Control(pDX, IDC_CHECK_PLAIN_PIN_IS_ASCII, m_CheckPlainPinIsAscii);
	DDX_Text(pDX, IDC_EDIT_XOR1, m_sXor1);
	DDX_Text(pDX, IDC_EDIT_XOR2, m_sXor2);
	DDX_Text(pDX, IDC_EDIT_XOR_RESULT, m_sXorResult);
	DDV_MaxChars(pDX, m_sXor1, 32);
	DDV_MaxChars(pDX, m_sXor2, 32);
}

BEGIN_MESSAGE_MAP(Ctp_dukptDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_GEN_IPEK, &Ctp_dukptDlg::OnBnClickedButtonGenIpek)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_INFOLIST, &Ctp_dukptDlg::OnBnClickedButtonClearInfolist)
	ON_BN_CLICKED(IDC_BUTTON_SK, &Ctp_dukptDlg::OnBnClickedButtonSk)
	ON_BN_CLICKED(IDC_CHECK_PLAIN_DATA_IS_ASCII, &Ctp_dukptDlg::OnBnClickedCheckPlainDataIsAscii)
	ON_BN_CLICKED(IDC_CHECK_PLAIN_MSG_IS_ASCII, &Ctp_dukptDlg::OnBnClickedCheckPlainMsgIsAscii)
	ON_BN_CLICKED(IDC_CHECK_PLAIN_PIN_IS_ASCII, &Ctp_dukptDlg::OnBnClickedCheckPlainPinIsAscii)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE_COUNTER, &Ctp_dukptDlg::OnBnClickedButtonChangeCounter)
	ON_BN_CLICKED(IDC_BUTTON_DECRYPTION, &Ctp_dukptDlg::OnBnClickedButtonDecryption)
	ON_BN_CLICKED(IDC_BUTTON_DECRYPTION_TDES, &Ctp_dukptDlg::OnBnClickedButtonDecryptionTdes)
	ON_BN_CLICKED(IDC_BUTTON_DECRYPTION_AES, &Ctp_dukptDlg::OnBnClickedButtonDecryptionAes)
	ON_BN_CLICKED(IDC_BUTTON_ENCRYPTION_TDES, &Ctp_dukptDlg::OnBnClickedButtonEncryptionTdes)
	ON_BN_CLICKED(IDC_BUTTON_ENCRYPTION_AES, &Ctp_dukptDlg::OnBnClickedButtonEncryptionAes)
	ON_COMMAND(ID_COPY, &Ctp_dukptDlg::OnCopy)
	ON_BN_CLICKED(IDC_BUTTON_ENCODE_BASE64, &Ctp_dukptDlg::OnBnClickedButtonEncodeBase64)
	ON_BN_CLICKED(IDC_BUTTON_DECODE_BASE64, &Ctp_dukptDlg::OnBnClickedButtonDecodeBase64)
	ON_BN_CLICKED(IDC_BUTTON_XOR, &Ctp_dukptDlg::OnBnClickedButtonXor)
END_MESSAGE_MAP()


// Ctp_dukptDlg 메시지 처리기

BOOL Ctp_dukptDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_CheckPlainDataIsAscii.SetCheck(1);
	m_CheckPlainMsgIsAscii.SetCheck(1);
	m_CheckPlainPinIsAscii.SetCheck(0);

	dukpt_ini();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void Ctp_dukptDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void Ctp_dukptDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR Ctp_dukptDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void Ctp_dukptDlg::OnBnClickedButtonGenIpek()
{
	UpdateData();
	//
	typeBytes vBDK, vKSN;

	// converts String BDK & KSN values to digits as bigendian.
	if( !HexsToBytes( vBDK, m_sBDK ) ){		TRACE( _T(" = Error converts BDK.\n") );	}
	if( !HexsToBytes( vKSN, m_sKSN_ini ) ){	TRACE( _T(" = Error converts KSN.\n") );	}

	// getting BDK
	copy( vBDK.begin(), vBDK.end(), m_bdk );

	// padding ksn to 10 bytes.
	unsigned char ksn_ini[10];
	dukpt_padding_ksn( m_ksn_ini, &vKSN[0], vKSN.size() );
	BytesToHexs( m_sKSN_ini, m_ksn_ini, sizeof(m_ksn_ini) );

	copy( m_ksn_ini, &m_ksn_ini[10], m_ksn_cur );
	BytesToHexs( m_sKSN_cur, m_ksn_cur, sizeof(m_ksn_cur) );

	// generate ipek.
	CWarp_idt_dukpt::get_security_object()->dukpt_generate_ipek( m_ipek, m_ksn_ini, m_bdk );
	BytesToHexs( m_sIPEK, m_ipek, sizeof(m_ipek) );

	UpdateData( FALSE );

	return;
	////////////////////////////////////////////////////
	//

	/*
	m_x924.PowerOnReset();
	m_x924.LoadInitialKey( m_bdk, &m_ksn_ini[2] );

	typeStrings vsX924;
	m_x924.GetString( vsX924 );

	//AddInfoList
	for_each( vsX924.begin(), vsX924.end(), [=]( typeStrings::value_type v ){
		AddInfoList( v );
	});

	unsigned char formated_and_encrypted_pin_block[18];
	unsigned char AccountNum[] = { 0x04, 0x01, 0x23, 0x45, 0x67, 0x89, 0x09 };
	unsigned char Pin[] = { 0x12, 0x34 };

	m_x924.RequestPINEntry( formated_and_encrypted_pin_block, AccountNum, Pin, sizeof(Pin) );
	m_x924.GetString( vsX924 );

	//AddInfoList
	for_each( vsX924.begin(), vsX924.end(), [=]( typeStrings::value_type v ){
		AddInfoList( v );
	});
	*/
}

#include "DlgChangeCounter.h"

void Ctp_dukptDlg::OnBnClickedButtonChangeCounter()
{
	UpdateData();

	// getting counter from ksn ( right 21 bits )
	unsigned char cnt[10] =
	{
		0,0,0,0,0,0,0,0x1f,0xff,0xff
	};

	int i = 0;
	for_each( m_ksn_cur, &m_ksn_cur[10], [&]( unsigned char c ){
		cnt[i] &= c;
		i++;
	});

	CDlgChangeCounter dlg( cnt );

	if( dlg.DoModal() == IDOK ){
		m_ksn_cur[7] &= 0xe0;
		m_ksn_cur[8] = m_ksn_cur[9] = 0;
		//
		dlg.GetCounter( cnt );

		m_ksn_cur[7] |= cnt[7];
		m_ksn_cur[8] |= cnt[8];
		m_ksn_cur[9] |= cnt[9];

		BytesToHexs( m_sKSN_cur, m_ksn_cur, sizeof(m_ksn_cur)  );
		UpdateData( FALSE );
		Processing( false );
	}
	//
	
}

void Ctp_dukptDlg::OnBnClickedButtonSk()
{
	Processing();
}

void Ctp_dukptDlg::OnBnClickedButtonDecryption()
{
	CDlgDecryption dlg;
	
	if( dlg.DoModal() == IDOK ){
		typeBytes bytes;

		if( HexsToBytes( bytes, dlg.m_sEncrytionText ) ){

			if( bytes.size() % 8 == 0 ){
				typeBytes out;
				TdesDescryption( out, bytes, m_data_en_request_key );
				AddInfoList( &out[0], out.size(), false, _T(" - decryption data by data request key : ") );//
			}
		}
	}
}

void Ctp_dukptDlg::Processing(  bool bIncreaseCounter /*= true*/ )
{
	UpdateData();
	//
	if( m_sKSN_cur.IsEmpty() ){		return;	}

	// increase KSN
	if( bIncreaseCounter )
		dukpt_increase_ksn( m_ksn_cur );

	// generate session key.
	CWarp_idt_dukpt::get_security_object()->dukpt_generate_sk( m_session_key, m_ksn_cur, m_ipek );

	// generate pin encryption key.
	CWarp_idt_dukpt::get_security_object()->dukpt_generate_pin_encryption_key( m_pin_encryption_key, m_session_key );

	// generate mac request key.
	CWarp_idt_dukpt::get_security_object()->dukpt_generate_mac_request_key( m_mac_request_key, m_session_key );

	// generate mac response key.
	CWarp_idt_dukpt::get_security_object()->dukpt_generate_mac_response_key( m_mac_response_key, m_session_key );

	// generate data encryption request key.
	CWarp_idt_dukpt::get_security_object()->dukpt_generate_data_encryption_request_key( m_data_en_request_key, m_session_key );

	// generate data encryption response key.
	CWarp_idt_dukpt::get_security_object()->dukpt_generate_data_encryption_response_key( m_data_en_response_key, m_session_key );

	// update display
	BytesToHexs( m_sKSN_cur, m_ksn_cur, sizeof(m_ksn_cur) );
	BytesToHexs( m_sSessionKey, m_session_key, sizeof(m_session_key) );
	BytesToHexs( m_sPinEncryptionKey, m_pin_encryption_key, sizeof(m_pin_encryption_key) );
	BytesToHexs( m_sMacReqEncryptionKey, m_mac_request_key, sizeof(m_mac_request_key) );
	BytesToHexs( m_sMacRespEncryptionKey, m_mac_response_key, sizeof(m_mac_response_key) );
	BytesToHexs( m_sDataReqEncryptionKey, m_data_en_request_key, sizeof(m_data_en_request_key) );
	BytesToHexs( m_sDataRespEncryptionKey, m_data_en_response_key, sizeof(m_data_en_response_key) );

	m_ListInfo.ResetContent();

	// getting plain pin
	CString sPlainPin( *m_pPlainPin );
	if( sPlainPin.GetLength() > 0 ){
		bool bResult(true);
		typeBytes vPlainPin;

		if( m_CheckPlainPinIsAscii.GetCheck()==0 )	bResult = HexsToBytes( vPlainPin, sPlainPin );//hex format.
		else		AsciisToBytes( vPlainPin, sPlainPin );
		//
		if( bResult ){
			typeBytes vOutMsg;

			TdesEncrytionWithPadding( vOutMsg, vPlainPin, m_pin_encryption_key, 0xff );
			AddInfoList( &vOutMsg[0], vOutMsg.size(), false, _T(" - encryted pin by pin key : ") );//
		}
	}

	// getting plain message
	CString sPlainMsg( *m_pPlainMsg );
	if( sPlainMsg.GetLength() > 0 ){
		bool bResult(true);
		typeBytes vPlainMsg;

		if( m_CheckPlainMsgIsAscii.GetCheck()==0 )	bResult = HexsToBytes( vPlainMsg, sPlainMsg );//hex format.
		else		AsciisToBytes( vPlainMsg, sPlainMsg );
		//
		if( bResult ){
			unsigned char mac[8];
			CWarp_idt_dukpt::get_security_object()->dukpt_generate_mac( mac, m_mac_request_key , &vPlainMsg[0], vPlainMsg.size() );
			AddInfoList( mac, sizeof(mac),  false, _T(" - message mac by mac request key : ") );
			CWarp_idt_dukpt::get_security_object()->dukpt_generate_mac( mac, m_mac_response_key , &vPlainMsg[0], vPlainMsg.size() );
			AddInfoList( mac, sizeof(mac),  false, _T(" - message mac by mac response key : ") );
		}
	}

	// getting plain text
	CString sPlainData( *m_pPlainData );

	if( sPlainData.GetLength() > 0 ){
		bool bResult(true);
		typeBytes vPlainData;

		if( m_CheckPlainDataIsAscii.GetCheck()==0 )	bResult = HexsToBytes( vPlainData, sPlainData );//hex format.
		else		AsciisToBytes( vPlainData, sPlainData );
		//
		if( bResult ){
			typeBytes vOutMsg;

			TdesEncrytionWithPadding( vOutMsg, vPlainData, m_data_en_request_key, 0 );
			AddInfoList( &vOutMsg[0], vOutMsg.size(), false, _T(" - encryted data by data request key(tdes) : ") );//.
		}
	}

	UpdateData( FALSE );
}

/**
*/

void Ctp_dukptDlg::dukpt_ini()
{
	memset( m_bdk, 0, sizeof(m_bdk) );
	memset( m_ksn_ini, 0, sizeof(m_ksn_ini) );
	memset( m_ksn_cur, 0, sizeof(m_ksn_cur) );
	memset( m_ipek, 0, sizeof(m_ipek) );
	memset( m_session_key, 0, sizeof(m_session_key) );
	memset( m_pin_encryption_key, 0, sizeof(m_pin_encryption_key) );

	memset( m_mac_request_key, 0, sizeof(m_mac_request_key) );
	memset( m_mac_response_key, 0, sizeof(m_mac_response_key) );
	memset( m_data_en_request_key, 0, sizeof(m_data_en_request_key) );
	memset( m_data_en_response_key, 0, sizeof(m_data_en_response_key) );
}

void Ctp_dukptDlg::dukpt_padding_ksn( unsigned char out_ksn[], const unsigned char in_ksn[], unsigned int nIn_ksn )
{
	unsigned int nPad = 10-nIn_ksn;
	auto i = 0;

	for( ; i<nPad; i++ ){
		out_ksn[i] = 0xFF;
	}//end for

	for( auto j=0; i<10; i++, j++ ){
		out_ksn[i] = in_ksn[j];
	}
}

bool Ctp_dukptDlg::dukpt_increase_ksn( unsigned char ksn[] )
{
	bool bResult = true;
	// increase KSN
	unsigned char count[10];
	unsigned char count_p[] = 
	{
		0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x1F, 0xFF, 0xFF
	};

	// remove counter part.
	for( auto i=0; i<10; i++ ){
		count[i] = ksn[i] & count_p[i];
		ksn[i] = ksn[i] & (~count_p[i]);
	}//end for

	// increase counter.
	if( count[9] < 0xFF )
		count[9]++;
	else{
		if( count[8] < 0xFF ){
			count[9] = 0;
			count[8]++;
		}
		else{
			if( count[7] < 0x1F ){
				count[9] = count[8] = 0;
				count[7]++;
			}
			else{
				count[9] = count[8] = count[7] = 0;
				bResult = false;
			}
		}
	}

	for( auto i=0; i<10; i++ ){
		ksn[i] = ksn[i] | count[i];
	}
	//
	return bResult;
}

/**
*/

void Ctp_dukptDlg::TdesDescryption( typeBytes &out, const typeBytes &in, const unsigned char *pkey )
{
	if( pkey == NULL )
		return;
	//
	out.resize( in.size(), 0 );
	unsigned int n_out(0);

	CWarp_idt_dukpt::get_security_object()->decrypt_ebc_tdes( &out[0], &n_out, &in[0], in.size(), pkey );
}

void Ctp_dukptDlg::TdesEncrytionWithPadding( typeBytes &out, const typeBytes & in, const unsigned char *pkey, unsigned char cPadding  )
{
	if( pkey == NULL )
		return;
	//
	auto nCnt (in.size()/8);

	if( in.size() % 8 != 0 ){		nCnt++;		}
	out.resize( nCnt*8, 0 );
	unsigned int n_out(0);
	CWarp_idt_dukpt::get_security_object()->encrypt_ebc_tdes( &out[0], &n_out, &in[0], in.size(), pkey, cPadding );
}

void Ctp_dukptDlg::AesDescryption( typeBytes &out, const typeBytes &in, const unsigned char key[16], const unsigned char iv[16] )
{
	// decryption AES CBC mode.
	if( in.size() % 16 )
		return;
	//
	out.resize( in.size() );
	unsigned int n_out(0);
	CWarp_idt_dukpt::get_security_object()->decrypt_cbc_aes( &out[0], &n_out, &in[0], in.size(), key, iv );
}

void Ctp_dukptDlg::AesEncrytionWithPadding( typeBytes &out, const typeBytes & in, const unsigned char *pkey, const unsigned char iv[16], unsigned char cPadding )
{
	// encryption AES CBC mode.
	unsigned int n_out(0);

	if( in.size() % 16 == 0 ){
		n_out = in.size();
	}
	else{
		n_out = in.size() / 16;
		n_out = n_out * 16;
		n_out++;
	}
	
	CWarp_idt_dukpt::get_security_object()->encrypt_cbc_aes( &out[0], &n_out, &in[0], in.size(), pkey, iv, cPadding );
}

void Ctp_dukptDlg::deb_trace_hexs( unsigned char *sHex, unsigned int nHex, bool bBigEndian /*= false*/, LPCTSTR lpPreMsg /*= NULL*/ )
{
	vector<unsigned char> vHex;

	for( unsigned int i = 0; i<nHex; i++ ){
		vHex.push_back( *sHex );
		sHex++;
	}//end for

	if( bBigEndian ){
		reverse( vHex.begin(), vHex.end() );
	}
	//
	if( lpPreMsg ){
		TRACE( _T("%s"), lpPreMsg );
	}
	//
	for_each( vHex.begin(), vHex.end(), [=]( vector<unsigned char>::value_type c ){
		TRACE( _T("%02X"), c );
	});

	TRACE( _T("\n") );
}

void Ctp_dukptDlg::AddInfoList( LPCTSTR lptstr )
{
	if( lptstr ){
		m_ListInfo.AddString( lptstr );
		m_ListInfo.SetCurSel( m_ListInfo.GetCount()-1 );
	}
}

void Ctp_dukptDlg::AddInfoList( unsigned char *psHex, unsigned int nHex, bool bBigEndian /*= false*/, LPCTSTR lpPreMsg /*= NULL*/ )
{
	vector<unsigned char> vHex;

	for( unsigned int i = 0; i<nHex; i++ ){
		vHex.push_back( *psHex );
		psHex++;
	}//end for

	if( bBigEndian ){
		reverse( vHex.begin(), vHex.end() );
	}
	//
	CString msg;

	if( lpPreMsg ){
		msg.Format( _T("%s"), lpPreMsg  );
	}
	//

	CString stemp;
	int i = 0;
	for_each( vHex.begin(), vHex.end(), [&]( vector<unsigned char>::value_type c ){
		i++;
		if( i%8==0 ){
			stemp.Format( _T("%02X  "), c );
		}
		else
			stemp.Format( _T("%02X"), c );
		msg += stemp;
	});

	AddInfoList( (LPCTSTR)msg );
}

void Ctp_dukptDlg::OnBnClickedButtonClearInfolist()
{
	m_ListInfo.ResetContent();
}

void Ctp_dukptDlg::OnBnClickedCheckPlainDataIsAscii()
{
	UpdateData();

	if( m_CheckPlainDataIsAscii.GetCheck()==0 ){
		m_pPlainData = &m_sPlainData_hex;
	}
	else{
		m_pPlainData = &m_sPlainData_ascii;
	}

	UpdateData( FALSE );
}

void Ctp_dukptDlg::OnBnClickedCheckPlainMsgIsAscii()
{
	UpdateData();

	if( m_CheckPlainMsgIsAscii.GetCheck()==0 ){
		m_pPlainMsg = &m_sPlainMsg_hex;
	}
	else{
		m_pPlainMsg = &m_sPlainMsg_ascii;
	}

	UpdateData( FALSE );
}

void Ctp_dukptDlg::OnBnClickedCheckPlainPinIsAscii()
{
	UpdateData();

	if( m_CheckPlainPinIsAscii.GetCheck()==0 ){
		m_pPlainPin = &m_sPlainPin_hex;
	}
	else{
		m_pPlainPin = &m_sPlainPin_ascii;
	}

	UpdateData( FALSE );
}

void Ctp_dukptDlg::OnBnClickedButtonDecryptionTdes()
{
	if( m_Dlg_tdes_de.DoModal() == IDOK ){
		typeBytes bytes_key;

		if( !HexsToBytes( bytes_key, m_Dlg_tdes_de.m_sKey ) ){
			return;
		}

		if( bytes_key.size() != 16 ){
			return;
		}

		typeBytes bytes_data;

		if( !HexsToBytes( bytes_data, m_Dlg_tdes_de.m_sData ) ){
			return;
		}

		if( bytes_data.size() % 8 != 0 ){
			return;
		}
		/////
		typeBytes out;
		TdesDescryption( out, bytes_data, &bytes_key[0] );
		AddInfoList( &out[0], out.size(), false, _T(" - decryption data by TDES : ") );//
	}
}

void Ctp_dukptDlg::OnBnClickedButtonEncryptionTdes()
{
	if( m_Dlg_tdes_en.DoModal() == IDOK ){
		typeBytes bytes_key;

		if( !HexsToBytes( bytes_key, m_Dlg_tdes_en.m_sKey ) ){
			return;
		}
		if( bytes_key.size() != 16 ){
			return;
		}

		typeBytes bytes_data;

		if( !HexsToBytes( bytes_data, m_Dlg_tdes_en.m_sData ) ){
			return;
		}
		/////
		typeBytes out;
		TdesEncrytionWithPadding( out, bytes_data, &bytes_key[0], 0 );
		AddInfoList( &out[0], out.size(), false, _T(" - encryption data by TDES : ") );//
	}
}

void Ctp_dukptDlg::OnBnClickedButtonDecryptionAes()
{
	if( m_Dlg_aes_de.DoModal() == IDOK ){
		typeBytes bytes_key;

		if( !HexsToBytes( bytes_key, m_Dlg_aes_de.m_sKey ) ){
			return;
		}

		if( bytes_key.size() != 16 ){
			return;
		}

		typeBytes bytes_data;

		if( !HexsToBytes( bytes_data, m_Dlg_aes_de.m_sData ) ){
			return;
		}

		if( bytes_data.size() % 16 != 0 ){
			return;
		}
		/////
		typeBytes out;
		typeBytes iv(16,0);
		AesDescryption( out, bytes_data, &bytes_key[0], &iv[0] );
		AddInfoList( &out[0], out.size(), false, _T(" - decryption data by AES : ") );//
	}
}

void Ctp_dukptDlg::OnBnClickedButtonEncryptionAes()
{
	if( m_Dlg_aes_en.DoModal() == IDOK ){
		typeBytes bytes_key;

		if( !HexsToBytes( bytes_key, m_Dlg_aes_en.m_sKey ) ){
			return;
		}
		if( bytes_key.size() != 16 ){
			return;
		}

		typeBytes bytes_data;

		if( !HexsToBytes( bytes_data, m_Dlg_aes_en.m_sData ) ){
			return;
		}
		/////
		typeBytes out;
		typeBytes iv( 16, 0 );
		AesEncrytionWithPadding( out, bytes_data, &bytes_key[0], &iv[0], 0 );
		AddInfoList( &out[0], out.size(), false, _T(" - encryption data by AES : ") );//
	}
}


void Ctp_dukptDlg::OnBnClickedButtonEncodeBase64()
{
	m_Dlg_base64.setEncodeMode();
	m_Dlg_base64.setTitle( CString( _T("Encoding to Base64") ) );
	m_Dlg_base64.setDoButtonText( CString( _T("Encode") ) );
	m_Dlg_base64.DoModal();
}


void Ctp_dukptDlg::OnBnClickedButtonDecodeBase64()
{
	m_Dlg_base64.setEncodeMode( false );
	m_Dlg_base64.setTitle( CString( _T("Decoding to original") ) );
	m_Dlg_base64.setDoButtonText( CString( _T("Decode") ) );
	m_Dlg_base64.DoModal();
}

void Ctp_dukptDlg::OnCopy()
{
	m_ListInfo.OnCopy();
}
void Ctp_dukptDlg::OnBnClickedButtonXor()
{
	UpdateData();

	typeBytes vXOR[2];

	// converts String BDK & KSN values to digits as bigendian.
	if( !HexsToBytes( vXOR[0], m_sXor1 ) ){		TRACE( _T(" = Error converts xor0.\n") );	return;}
	if( !HexsToBytes( vXOR[1], m_sXor2 ) ){	TRACE( _T(" = Error converts xor1.\n") );	return;}
	//
	typeBytes vXOR_result;

	for( size_t i=0; i<vXOR[0].size(); i++){
		vXOR_result.push_back(  vXOR[0][i] ^ vXOR[1][i] );
	}//end for

	BytesToHexs( m_sXorResult, &vXOR_result[0], vXOR_result.size() );
	
	UpdateData(FALSE);
}

//////////////////////////////////////////////////////////////////////////////////////
/*
Ctp_dukptDlg::CANSX9_24_1::CANSX9_24_1() : m_pCurKey(NULL)
{
	fill( m_reg_AccountNum, &m_reg_AccountNum[sizeof(m_reg_AccountNum)], 0 );
	fill( m_reg_IniKSN, &m_reg_IniKSN[sizeof(m_reg_IniKSN)], 0 );
	fill( m_reg_KSN, &m_reg_KSN[sizeof(m_reg_KSN)], 0 );
	fill( m_reg_EnCnt, &m_reg_EnCnt[sizeof(m_reg_EnCnt)], 0 );//use 21 bits.
	fill( m_reg_Shift, &m_reg_Shift[sizeof(m_reg_Shift)], 0 );//use 21 bits.

	fill( m_reg_Crypto1, &m_reg_Crypto1[sizeof(m_reg_Crypto1)], 0 );
	fill( m_reg_Crypto2, &m_reg_Crypto2[sizeof(m_reg_Crypto2)], 0 );
	fill( m_reg_Key, &m_reg_Key[sizeof(m_reg_Key)], 0 );
	fill( m_reg_MacKey, &m_reg_MacKey[sizeof(m_reg_MacKey)], 0 );
	fill( m_reg_MacResponseKey, &m_reg_MacResponseKey[sizeof(m_reg_MacResponseKey)], 0 );

	fill( m_reg_DataEnKey, &m_reg_DataEnKey[sizeof(m_reg_DataEnKey)], 0 );
	fill( m_reg_DataEnResponseKey, &m_reg_DataEnResponseKey[sizeof(m_reg_DataEnResponseKey)], 0 );
	//
	for( int i=0; i<21; i++ ){
		fill( m_reg_FutureKey[i], &m_reg_FutureKey[i][17], 0 );
	}//end for
}

Ctp_dukptDlg::CANSX9_24_1::~CANSX9_24_1()
{

}

unsigned char Ctp_dukptDlg::CANSX9_24_1::GenerateLRC( const unsigned char Key[16] )
{
	unsigned char cLrc = 0;

	for( int i=0; i<16; i++ )
		cLrc ^= Key[i];

	return cLrc;
}

unsigned int Ctp_dukptDlg::CANSX9_24_1::Get_SetBitCount_In_21Bits( const unsigned char s21Bit[3] )
{
	unsigned char cMask = 0x10;
	unsigned int nOneCnt = 0;

	for( int i = 0; i<5; i++ ){
		if( cMask & s21Bit[0] )
			nOneCnt++;
		cMask >>= 1;
	}
	
	for( int j=0; j<2; j++ ){
		cMask = 0x80;
		for( int i = 0; i<8; i++ ){
			if( cMask & s21Bit[j+1] )
				nOneCnt++;
			cMask >>= 1;
		}
	}

	return nOneCnt;
}

bool Ctp_dukptDlg::CANSX9_24_1::Shift_Right_one_bit_In_21Bits( unsigned char s21Bit[3] )
{
	bool bResult(true);

	if( s21Bit[0] > 0x01 )
		s21Bit[0] >>= 1;
	else if( s21Bit[0] == 0x01 ){
		s21Bit[0] = 0;
		s21Bit[1] = 0x80;
	}
	else{
		if( s21Bit[1] > 0x01 )
			s21Bit[1] >>= 1;
		else if( s21Bit[1] == 0x01 ){
			s21Bit[1] = 0;
			s21Bit[2] = 0x80;
		}
		else{
			if( s21Bit[2] > 0x01 )
				s21Bit[2] >>= 1;
			else if( s21Bit[2] == 0x01 )
				s21Bit[2] = 0;
		}
	}

	if( s21Bit[0] == 0 && s21Bit[1] == 0 && s21Bit[2] == 0 )
		bResult = false;

	return bResult;
}

unsigned int Ctp_dukptDlg::CANSX9_24_1::Get_Bit_Pos_In_21Bits( const unsigned char s21Bit[3] )
{
	unsigned int nPos(0);
	unsigned char cMask(0);

	cMask = 0x10;

	for( unsigned int i=0; i<5; i++ ){
		if( cMask & s21Bit[0] ){
			nPos = i;
			return nPos;
		}
		cMask >>= 1;
	}//end for i;


	for( unsigned int j = 1; j<3; j++ ){

		cMask = 0x80;
		for( unsigned int i=0; i<8; i++ ){
			if( cMask & s21Bit[j] ){
				nPos = 5 + (j-1)*8+i;
				return nPos;
			}

			cMask >>= 1;
		}//end for i;
	}//end for j

	return nPos;	//maybe dummy
}

bool Ctp_dukptDlg::CANSX9_24_1::Increase_one_In_21Bits( unsigned char s21Bit[3] )
{
	bool bResult(true);

	if( s21Bit[2] == 0xff ){
		s21Bit[2] = 0;

		if( s21Bit[1] == 0xff ){
			s21Bit[1] = 0;

			if( s21Bit[0] >= 0x1f ){
				s21Bit[0] = 0;
				bResult = false;
			}
			else{
				s21Bit[0]++;
			}
		}
		else{
			s21Bit[1]++;
		}
	}
	else{
		s21Bit[2]++;
	}

	return bResult;
}

void Ctp_dukptDlg::CANSX9_24_1::Set_Bit( unsigned char s21Bit[3], const unsigned char sBase21Bit[3] )
{
	fill( s21Bit, &s21Bit[3], 0 );
	//
	unsigned char cMask(0);

	for( int j=2; j<=0; j-- ){
		cMask = 0x01;

		for( int i=0; i<8; i++ ){
			if( sBase21Bit[j] & cMask ){
				s21Bit[j] = s21Bit[j] | cMask;
				return;
			}
			cMask <<= 1;
		}//end for i

	}//end for j
}

void Ctp_dukptDlg::CANSX9_24_1::NonReversibleKeyGenProcess()
{
	for( int i=0; i<8; i++ ){		m_reg_Crypto2[i] = m_reg_Crypto1[i] ^ m_reg_Key[i+8];		}//end for

	des_context ctx;
	unsigned char in[8];
	des_set_key( &ctx, m_reg_Key );
	copy( m_reg_Crypto2, &m_reg_Crypto2[8], in );
	des_encrypt( &ctx, in, m_reg_Crypto2 );

	for( int i=0; i<8; i++ ){		m_reg_Crypto2[i] ^= m_reg_Key[i+8];		}//end for

	unsigned char key_p[] =
	{
		0xc0, 0xc0, 0xc0, 0xc0, 0x00, 0x00, 0x00, 0x00,
		0xc0, 0xc0, 0xc0, 0xc0, 0x00, 0x00, 0x00, 0x00
	};
	for( int i=0; i<16; i++ ){		m_reg_Key[i] ^= key_p[i];		}//end for

	for( int i=0; i<8; i++ ){		m_reg_Crypto1[i] ^= m_reg_Key[i+8];		}//end for

	des_set_key( &ctx, m_reg_Key );
	copy( m_reg_Crypto1, &m_reg_Crypto1[8], in );
	des_encrypt( &ctx, in, m_reg_Crypto1 );

	for( int i=0; i<8; i++ ){		m_reg_Crypto1[i] ^= m_reg_Key[i+8];		}//end for

}

void Ctp_dukptDlg::CANSX9_24_1::EnCrypt_Tdes_self( unsigned char OutEnData[16], const unsigned char Key_and_data[16] )
{
	unsigned char in[16], key[16];

	copy( Key_and_data, &Key_and_data[16], in );
	copy( Key_and_data, &Key_and_data[16], key );

	des3_context ctx3;
	des3_set_3keys( &ctx3, &key[0], &key[8], &key[0] );
	des3_encrypt( &ctx3, in, OutEnData );
	des3_encrypt( &ctx3, &in[8], &OutEnData[8] );

}

void Ctp_dukptDlg::CANSX9_24_1::EnCrypt_Tdes()
{
	unsigned char in[8];

	copy( m_reg_Crypto1, &m_reg_Crypto1[8], in );

	des3_context ctx3;
	des3_set_3keys( &ctx3, &m_reg_Key[0], &m_reg_Key[8], &m_reg_Key[0] );
	des3_encrypt( &ctx3, in, m_reg_Crypto1 );
}

void Ctp_dukptDlg::CANSX9_24_1::ClearEncrytionCounter()
{
	fill( m_reg_EnCnt, &m_reg_EnCnt[3], 0 );

	m_reg_KSN[5] &= 0xE0;
	m_reg_KSN[6] = 0;
	m_reg_KSN[7] = 0;
}

void Ctp_dukptDlg::CANSX9_24_1::EraseCurrentKeyValue()
{
	if( m_pCurKey ){
		fill( &m_pCurKey[0], &m_pCurKey[16], 0 );
		m_pCurKey[16] = 0xff;
	}
}

bool Ctp_dukptDlg::CANSX9_24_1::IsEncryptionCounterAllZeros()
{
	if( m_reg_EnCnt[0] == 0 && m_reg_EnCnt[1] == 0 && m_reg_EnCnt[2] == 0 )
		return true;
	else
		return false;
}

bool Ctp_dukptDlg::CANSX9_24_1::Label_NewKey()
{
LNew_Key:
	unsigned int nOneCnt = Get_SetBitCount_In_21Bits( m_reg_EnCnt );
	if( nOneCnt < 10 )
		goto LNew_Key_1;

	//Clear current key.
	EraseCurrentKeyValue();

	unsigned char s218[] = {
		m_reg_EnCnt[0] + m_reg_Shift[0],
		m_reg_EnCnt[1] + m_reg_Shift[1],
		m_reg_EnCnt[2] + m_reg_Shift[2]
	};

	if( Get_SetBitCount_In_21Bits( m_reg_EnCnt ) <= 10 ){
		for( int i = 0; i<3; i++ ){
			m_reg_EnCnt[i] = s218[i];
		}
	}

	goto LNew_Key_2;
	//////////////////////////////////////////////////////////
LNew_Key_1:
	if( Shift_Right_one_bit_In_21Bits( m_reg_Shift ) )
		goto LNew_Key_3;
	else
		goto LNew_Key_4;
	//////////////////////////////////////////////////////////
LNew_Key_3:
	unsigned char SR64Bits[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	for( int i=0; i<3; i++ ){		SR64Bits[i+5] = m_reg_Shift[i]; 	}

	for( int i=0; i<8; i++ ){		m_reg_Crypto1[i] = m_reg_KSN[i] | SR64Bits[i];	}

	if( m_pCurKey )
		for( int i=0; i<16; i++ ){	m_reg_Key[i] = m_pCurKey[i];		 }

	NonReversibleKeyGenProcess();

	unsigned int nPos( Get_Bit_Pos_In_21Bits( m_reg_Shift ) );

	for( int i = 0; i<8; i++ ){	m_reg_FutureKey[nPos][i] = m_reg_Crypto1[i];	}
	for( int i = 8; i<16; i++ ){	m_reg_FutureKey[nPos][i] = m_reg_Crypto2[i-8];	}

	m_reg_FutureKey[nPos][16] = GenerateLRC( m_reg_FutureKey[nPos] );

	goto LNew_Key_1;
	//////////////////////////////////////////////////////////

LNew_Key_4:
	//Clear current furture key.
	EraseCurrentKeyValue();
	Increase_one_In_21Bits( m_reg_EnCnt );
	goto LNew_Key_2;
	///////////////////////////////////////////////////////////
LNew_Key_2:
	if( IsEncryptionCounterAllZeros() )
		return false;//over range using counter
	else
		goto LExit;
	//////////////////////////////////////////////////////////
LExit:
	return true;
}

void Ctp_dukptDlg::CANSX9_24_1::GenerateClearTextPinBlock( unsigned char PinBlock[8], const unsigned char PrimaryAccount[7], const unsigned char Pin[2] )
{
	fill( PinBlock, &PinBlock[8], 0xFF );

	PinBlock[0] = PrimaryAccount[0];
	PinBlock[1] = Pin[0];
	PinBlock[2] = Pin[1];
}

bool Ctp_dukptDlg::CANSX9_24_1::LoadInitialKey( const unsigned char IniPinEnKey[16], const unsigned char IniKSN[8] )
{
	copy( IniPinEnKey, &IniPinEnKey[16], m_reg_FutureKey[20] );//1)
	m_reg_FutureKey[20][16] = GenerateLRC( IniPinEnKey );//2)
	m_pCurKey = m_reg_FutureKey[20];//3)
	//
	copy( IniKSN, &IniKSN[8], m_reg_IniKSN );
	fill( m_reg_KSN, &m_reg_KSN[sizeof(m_reg_KSN)], 0 );
	copy( IniKSN, &IniKSN[8], m_reg_KSN );
	
	ClearEncrytionCounter();//clear encryption counter.

	//ini shift register
	fill( m_reg_Shift, &m_reg_Shift[sizeof(m_reg_Shift)], 0 );
	m_reg_Shift[0] = 0x10;//using only 21 bits
	//
LNew_Key:
	return Label_NewKey();
}

bool Ctp_dukptDlg::CANSX9_24_1::RequestPINEntry( 
	unsigned char formated_and_encrypted_pin_block[18],
	const unsigned char AccountNum[7],
	const unsigned char *pPin, unsigned int nPin )
{
	copy( AccountNum, &AccountNum[7], m_reg_AccountNum );

	if( nPin==0 )
		return false;

	// generate clear-text block. and transfer it to m_reg_Crypto1. <><><><><><><>
	GenerateClearTextPinBlock( m_reg_Crypto1, AccountNum, pPin );

	goto LRequestPINEntry1;
	/////////////////////////////////////////////////////////////////////////////

LRequestPINEntry1:
	Set_Bit( m_reg_Shift, m_reg_EnCnt );

	unsigned int nPos = Get_Bit_Pos_In_21Bits( m_reg_Shift );
	m_pCurKey = m_reg_FutureKey[nPos];

	if( m_pCurKey[16] == GenerateLRC( m_pCurKey ) ){
		goto LRequestPINEntry2;
	}

	unsigned char s218[] = {
		m_reg_EnCnt[0] + m_reg_Shift[0],
		m_reg_EnCnt[1] + m_reg_Shift[1],
		m_reg_EnCnt[2] + m_reg_Shift[2]
	};

	for( int i = 0; i<3; i++ ){
		m_reg_EnCnt[i] = s218[i];
	}

	if( IsEncryptionCounterAllZeros() )
		return false;//using counter over

	goto LRequestPINEntry1;
	/////////////////////////////////////////////////////////////////////////////
LRequestPINEntry2:

	if( m_pCurKey ){
		copy( m_pCurKey, &m_pCurKey[16], m_reg_Key );
	}

	// this part is option
	// ........ for generating message authentication process.
	unsigned char mac_p[] = {	
		00,00,00,00,00,00,0xff,00,
		00,00,00,00,00,00,0xff,00
	};

	for( int i=0; i<16; i++ ){	m_reg_MacKey[i] = m_reg_Key[i] ^ mac_p[i];	}

	unsigned char mac_res_p[] = {	
		00,00,00,00,0xff,00,00,00,
		00,00,00,00,0xff,00,00,00
	};

	for( int i=0; i<16; i++ ){	m_reg_MacResponseKey[i] = m_reg_Key[i] ^ mac_res_p[i];	}

	// this part is option
	// ........ for generating data encryption process.
	unsigned char data_p[] = {	
		00,00,00,00,00,0xff,00,00,
		00,00,00,00,00,0xff,00,00
	};
	unsigned char data[16];
	
	for( int i=0; i<16; i++ ){	data[i] = m_reg_Key[i] ^ data_p[i];	}
	EnCrypt_Tdes_self( m_reg_DataEnKey, data );

	unsigned char data_res_p[] = {	
		00,00,00,0xff,00,00,00,00,
		00,00,00,0xff,00,00,00,00
	};
	unsigned char data_res[16];
	
	for( int i=0; i<16; i++ ){	data_res[i] = m_reg_Key[i] ^ data_res_p[i];	}
	EnCrypt_Tdes_self( m_reg_DataEnResponseKey, data_res );

	//
	unsigned char var_key_p[] = {	
		00,00,00,00,00,00,00,0xff,
		00,00,00,00,00,00,00,0xff
	};
	unsigned char var_key[16];
	for( int i=0; i<16; i++ ){	var_key[i] = m_reg_Key[i] ^ var_key_p[i];	}

	EnCrypt_Tdes();

	//formatting
	//formated_and_encrypted_pin_block
	fill( formated_and_encrypted_pin_block, &formated_and_encrypted_pin_block[2], 0xff );
	copy( m_reg_KSN, &m_reg_KSN[8], &formated_and_encrypted_pin_block[2] );
	copy( m_reg_Crypto1, &m_reg_Crypto1[8], &formated_and_encrypted_pin_block[10] );

	/////////////////////////////////////////////////////////////////////////////
	// New Key
	return Label_NewKey();
}

bool Ctp_dukptDlg::CANSX9_24_1::PowerOnReset()
{
	unsigned char cMask = 0x10;
	for( int i=0; i<5; i++ ){
		if( m_reg_FutureKey[i][16] != GenerateLRC( m_reg_FutureKey[i] ) ){
			m_reg_EnCnt[0] |= cMask;
		}

		cMask >>= 1;
	}//end for

	cMask = 0x80;
	for( int i=5; i<13; i++ ){
		if( m_reg_FutureKey[i][16] != GenerateLRC( m_reg_FutureKey[i] ) ){
			m_reg_EnCnt[1] |= cMask;
		}

		cMask >>= 1;
	}//end for

	cMask = 0x80;
	for( int i=13; i<21; i++ ){
		if( m_reg_FutureKey[i][16] != GenerateLRC( m_reg_FutureKey[i] ) ){
			m_reg_EnCnt[2] |= cMask;
		}

		cMask >>= 1;
	}//end for

	/////////////////////////////////////////
	Increase_one_In_21Bits( m_reg_EnCnt );
	return true;
}

void Ctp_dukptDlg::CANSX9_24_1::GetString( typeStrings & sValue )
{
	sValue.clear();

	CString stemp, stemp1,stemp2;
	//
	stemp2.Empty();
	for( int i = 0; i<sizeof(m_reg_AccountNum); i++ ){
		stemp.Format( _T("%02X"), m_reg_AccountNum[i]  );
		stemp2 += stemp;
	}
	stemp1.Format( _T(" : m_reg_AccountNum : %s."), (LPCTSTR)stemp2 );
	sValue.push_back( stemp1 );
	//
	stemp2.Empty();
	for( int i = 0; i<sizeof(m_reg_IniKSN); i++ ){
		stemp.Format( _T("%02X"), m_reg_IniKSN[i]  );
		stemp2 += stemp;
	}

	stemp1.Format( _T(" : m_reg_IniKSN : %s."), (LPCTSTR)stemp2 );
	sValue.push_back( stemp1 );
	//
	stemp2.Empty();
	for( int i = 0; i<sizeof(m_reg_KSN); i++ ){
		stemp.Format( _T("%02X"), m_reg_KSN[i]  );
		stemp2 += stemp;
	}

	stemp1.Format( _T(" : m_reg_KSN : %s."), (LPCTSTR)stemp2 );
	sValue.push_back( stemp1 );
	//
	stemp2.Empty();
	for( int i = 0; i<sizeof(m_reg_EnCnt); i++ ){
		stemp.Format( _T("%02X"), m_reg_EnCnt[i]  );
		stemp2 += stemp;
	}

	stemp1.Format( _T(" : m_reg_EnCnt : %s."), (LPCTSTR)stemp2 );
	sValue.push_back( stemp1 );
	//
	stemp2.Empty();
	for( int i = 0; i<sizeof(m_reg_Shift); i++ ){
		stemp.Format( _T("%02X"), m_reg_Shift[i]  );
		stemp2 += stemp;
	}

	stemp1.Format( _T(" : m_reg_Shift : %s."), (LPCTSTR)stemp2 );
	sValue.push_back( stemp1 );
	//
	stemp2.Empty();
	for( int i = 0; i<sizeof(m_reg_Crypto1); i++ ){
		stemp.Format( _T("%02X"), m_reg_Crypto1[i]  );
		stemp2 += stemp;
	}

	stemp1.Format( _T(" : m_reg_Crypto1 : %s."), (LPCTSTR)stemp2 );
	sValue.push_back( stemp1 );
	//
	stemp2.Empty();
	for( int i = 0; i<sizeof(m_reg_Crypto2); i++ ){
		stemp.Format( _T("%02X"), m_reg_Crypto2[i]  );
		stemp2 += stemp;
	}

	stemp1.Format( _T(" : m_reg_Crypto2 : %s."), (LPCTSTR)stemp2 );
	sValue.push_back( stemp1 );
	//
	stemp2.Empty();
	for( int i = 0; i<sizeof(m_reg_Key); i++ ){
		stemp.Format( _T("%02X"), m_reg_Key[i]  );
		stemp2 += stemp;
	}

	stemp1.Format( _T(" : m_reg_Key : %s."), (LPCTSTR)stemp2 );
	sValue.push_back( stemp1 );
	//
	stemp2.Empty();
	for( int i = 0; i<sizeof(m_reg_MacKey); i++ ){
		stemp.Format( _T("%02X"), m_reg_MacKey[i]  );
		stemp2 += stemp;
	}

	stemp1.Format( _T(" : m_reg_MacKey : %s."), (LPCTSTR)stemp2 );
	sValue.push_back( stemp1 );
	//
	stemp2.Empty();
	for( int i = 0; i<sizeof(m_reg_MacResponseKey); i++ ){
		stemp.Format( _T("%02X"), m_reg_MacResponseKey[i]  );
		stemp2 += stemp;
	}

	stemp1.Format( _T(" : m_reg_MacResponseKey : %s."), (LPCTSTR)stemp2 );
	sValue.push_back( stemp1 );
	//
	stemp2.Empty();
	for( int i = 0; i<sizeof(m_reg_DataEnKey); i++ ){
		stemp.Format( _T("%02X"), m_reg_DataEnKey[i]  );
		stemp2 += stemp;
	}

	stemp1.Format( _T(" : m_reg_DataEnKey : %s."), (LPCTSTR)stemp2 );
	sValue.push_back( stemp1 );
	//
	stemp2.Empty();
	for( int i = 0; i<sizeof(m_reg_DataEnResponseKey); i++ ){
		stemp.Format( _T("%02X"), m_reg_DataEnResponseKey[i]  );
		stemp2 += stemp;
	}

	stemp1.Format( _T(" : m_reg_DataEnResponseKey : %s."), (LPCTSTR)stemp2 );
	sValue.push_back( stemp1 );
	//

	for( int j=0; j<21; j++ ){
		stemp2.Empty();
		for( int i = 0; i<17; i++ ){
			stemp.Format( _T("%02X"), m_reg_FutureKey[j][i]  );
			stemp2 += stemp;
		}

		stemp1.Format( _T(" : m_reg_FutureKey[%d] : %s."), j, (LPCTSTR)stemp2 );
		sValue.push_back( stemp1 );
	}
	
}
*/


