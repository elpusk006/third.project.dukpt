
// tp_lpu239_hardDoc.cpp : implementation of the Ctp_lpu239_hardDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "tp_lpu239_hard.h"
#endif

#include "tp_lpu239_hardDoc.h"
#include "MainFrm.h"
#include "InfoView.h"
#include "InputDlg.h"

#include "warp_idt_dukpt.h"

#include <propkey.h>

#include <tg_dev_manager.h>
#include <tg_dev_hid_lpu239.h>
#include <xmllite.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
using namespace thirdgeneration;
using namespace device_elpusk;

// Ctp_lpu239_hardDoc

IMPLEMENT_DYNCREATE(Ctp_lpu239_hardDoc, CDocument)

BEGIN_MESSAGE_MAP(Ctp_lpu239_hardDoc, CDocument)
	ON_COMMAND(ID_TOOL_GET_RAMDUM_NUMBER, &Ctp_lpu239_hardDoc::OnToolGetRamdumNumber)
	ON_COMMAND(ID_TOOL_CHANGE_AUTHENTICATION_KEY, &Ctp_lpu239_hardDoc::OnToolChangeAuthenticationKey)
	ON_COMMAND(ID_TOOL_CHANGE_IPEK, &Ctp_lpu239_hardDoc::OnToolChangeIpek)
	ON_COMMAND(ID_TOOL_CHANGE_KSN, &Ctp_lpu239_hardDoc::OnToolChangeKsn)
	ON_COMMAND(ID_TOOL_ENTER_CONFIG_MODE, &Ctp_lpu239_hardDoc::OnToolEnterConfigMode)
	ON_COMMAND(ID_TOOL_EXTERNAL_AUTHENTICATION, &Ctp_lpu239_hardDoc::OnToolExternalAuthentication)
	ON_COMMAND(ID_TOOL_LEAVE_CONFIG_MODE, &Ctp_lpu239_hardDoc::OnToolLeaveConfigMode)
	ON_COMMAND(ID_INPUT_AUTHENTICATION_KEY, &Ctp_lpu239_hardDoc::OnInputAuthenticationKey)
	ON_COMMAND(ID_INPUT_KSN, &Ctp_lpu239_hardDoc::OnInputKsn)
	ON_COMMAND(ID_INPUT_ENCRYPTION_KEY, &Ctp_lpu239_hardDoc::OnInputEncryptionKey)
	ON_COMMAND(ID_TOOL_READ_UID, &Ctp_lpu239_hardDoc::OnToolReadUid)
	ON_COMMAND(ID_TOOL_APPLY, &Ctp_lpu239_hardDoc::OnToolApply)
	ON_COMMAND(ID_TOOL_GOTO_BOOTLOADER, &Ctp_lpu239_hardDoc::OnToolGotoBootloader)
	ON_COMMAND(ID_TOOL_CHANGE_INTERFACE, &Ctp_lpu239_hardDoc::OnToolChangeInterface)
END_MESSAGE_MAP()


// Ctp_lpu239_hardDoc construction/destruction

Ctp_lpu239_hardDoc::Ctp_lpu239_hardDoc()
{
	// TODO: add one-time construction code here

}

Ctp_lpu239_hardDoc::~Ctp_lpu239_hardDoc()
{
}

BOOL Ctp_lpu239_hardDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// Ctp_lpu239_hardDoc serialization

void Ctp_lpu239_hardDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void Ctp_lpu239_hardDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void Ctp_lpu239_hardDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void Ctp_lpu239_hardDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// Ctp_lpu239_hardDoc diagnostics

#ifdef _DEBUG
void Ctp_lpu239_hardDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void Ctp_lpu239_hardDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Ctp_lpu239_hardDoc commands

void Ctp_lpu239_hardDoc::DisplayRightInfoView( LPCTSTR sInfo )
{
	((CMainFrame*)::AfxGetApp()->GetMainWnd())->getRightInfoView()->DisplayInfo( sInfo );
}

void Ctp_lpu239_hardDoc::OnToolGetRamdumNumber()
{
	CDev::typeUid uid(  get_current_uid() );
	if( uid.empty() ){
		DisplayRightInfoView( _T(" : [ERROR  ] : not selected device.") );
		return;
	}
	//
	if( device_get_randum_number() ){
		DisplayRightInfoView( _T(" : [SUCCESS] : Get Randum number.") );

		_tstring rn;
		CDev::typeBuffer vRn = m_vRn;

		if( vRn.size() != (CDevHidLpu239::const_size_random_number+CDevHidLpu239::const_size_mac4) ){
			rn = _T(" NO randum number");
		}
		else{
			_tstringstream ss;
			ss << hex;
			for_each( vRn.begin(), vRn.end(), [&]( CDev::typeBuffer::value_type c ){
				ss.fill(_T('0') );
				ss.width(2);
				ss << c;
			});
			rn = ss.str();
		}
		
		_tstring sinfo = _T(" : [received randum number+mac] : ") + rn;
		DisplayRightInfoView( sinfo.c_str() );
		
	}
	else{
		DisplayRightInfoView( _T(" : [ERROR  ] : Get Randum number.") );
	}
}


void Ctp_lpu239_hardDoc::OnToolChangeAuthenticationKey()
{
	CDev::typeUid uid(  get_current_uid() );
	if( uid.empty() ){
		DisplayRightInfoView( _T(" : [ERROR  ] : not selected device.") );
		return;
	}

	if( device_change_authentication_key0( m_vAuthKey[0] ) ){
		DisplayRightInfoView( _T(" : [SUCCESS] : change authentication key 0.") );
	}
	else{
		DisplayRightInfoView( _T(" : [ERROR  ] : change authentication key 0.") );
		return;
	}
	
	if( device_change_authentication_key0( m_vAuthKey[1] ) ){
		DisplayRightInfoView( _T(" : [SUCCESS] : change authentication key 1.") );
	}
	else{
		DisplayRightInfoView( _T(" : [ERROR  ] : change authentication key 1.") );
		return;
	}
}


void Ctp_lpu239_hardDoc::OnToolChangeIpek()
{
	CDev::typeUid uid(  get_current_uid() );
	if( uid.empty() ){
		DisplayRightInfoView( _T(" : [ERROR  ] : not selected device.") );
		return;
	}

	if( device_change_ksn( m_vKsn ) ){
		DisplayRightInfoView( _T(" : [SUCCESS] : change KSN.") );
	}
	else{
		DisplayRightInfoView( _T(" : [ERROR  ] : change KSN.") );
	}
}


void Ctp_lpu239_hardDoc::OnToolChangeKsn()
{
	CDev::typeUid uid(  get_current_uid() );
	if( uid.empty() ){
		DisplayRightInfoView( _T(" : [ERROR  ] : not selected device.") );
		return;
	}

	if( device_change_ipek0( m_vEnKey[0] ) ){
		DisplayRightInfoView( _T(" : [SUCCESS] : change encryption key(IPEK) 0.") );
	}
	else{
		DisplayRightInfoView( _T(" : [ERROR  ] : change encryption key(IPEK) 0.") );
		return;
	}

	if( device_change_ipek0( m_vEnKey[1] ) ){
		DisplayRightInfoView( _T(" : [SUCCESS] : change encryption key(IPEK) 1.") );
	}
	else{
		DisplayRightInfoView( _T(" : [ERROR  ] : change encryption key(IPEK) 1.") );
		return;
	}
}


void Ctp_lpu239_hardDoc::OnToolEnterConfigMode()
{
	CDev::typeUid uid(  get_current_uid() );
	if( uid.empty() ){
		DisplayRightInfoView( _T(" : [ERROR  ] : not selected device.") );
		return;
	}

	if( device_enter_config() ){
		DisplayRightInfoView( _T(" : [SUCCESS] : enter config mode.") );
	}
	else{
		DisplayRightInfoView( _T(" : [ERROR  ] : enter config mode.") );
	}
}


void Ctp_lpu239_hardDoc::OnToolExternalAuthentication()
{
	CDev::typeUid uid(  get_current_uid() );
	if( uid.empty() ){
		DisplayRightInfoView( _T(" : [ERROR  ] : not selected device.") );
		return;
	}

	if( m_vRn.size() != (CDevHidLpu239::const_size_random_number+CDevHidLpu239::const_size_mac4) ){
		DisplayRightInfoView( _T(" : [ERROR  ] : executes get randum before ExternalAuthentication.") );
		return;
	}

	if( m_vAuthKey[0].size() != 16 || m_vAuthKey[1].size() != 16 ){
		DisplayRightInfoView( _T(" : [ERROR  ] : Input authentication key before ExternalAuthentication.") );
		return;
	}

	// build authen key
	CDev::typeBuffer vAuthKey;

	for( int i = 0; i<16; i++ ){
		vAuthKey.push_back( m_vAuthKey[0][i] ^ m_vAuthKey[1][i] );
	}//end for

	CDev::typeBuffer vPlain_Rn( CDevHidLpu239::const_size_random_number, 0 ), iv(8,0),  vout( 16, 0 );
	unsigned int n_out(0);

	//get random number( plaintext )
	CWarp_idt_dukpt::get_security_object()->decrypt_cbc_tdes( &vPlain_Rn[0], &n_out, &m_vRn[0], CDevHidLpu239::const_size_random_number, &vAuthKey[0], &iv[0] );

	_tstring rn;
	_tstringstream ss;
	ss << hex;
	for_each( vPlain_Rn.begin(), vPlain_Rn.end(), [&]( CDev::typeBuffer::value_type c ){
		ss.fill(_T('0') );
		ss.width(2);
		ss << c;
	});
	rn = ss.str();
		
	_tstring sinfo = _T(" : [Plaintext randum number] : ") + rn;
	DisplayRightInfoView( sinfo.c_str() );

	//check mac value
	//CWarp_idt_dukpt::get_security_object()->encrypt_cbc_tdes( &vout[0], &n_out, &vAuthKey[0], vAuthKey.size(), &vPlain_Rn[0], &iv[0],0 );
	CDev::typeBuffer vMacKey( 16, 0 );
	CDev::typeBuffer vMac8( 8, 0 );
	CDev::typeBuffer vMac4( 4, 0 );

	CWarp_idt_dukpt::get_security_object()->dukpt_generate_mac_response_key( &vMacKey[0], &vAuthKey[0] );
	CWarp_idt_dukpt::get_security_object()->dukpt_generate_mac( &vMac8[0], &vMacKey[0], &vPlain_Rn[0], vPlain_Rn.size() );
	CWarp_idt_dukpt::get_security_object()->dukpt_get_mac4_from_mac8( &vMac4[0], &vMac8[0] );
	//
	ss.str(_T(""));
	ss << _T(" : [received mac] : ");
	ss << hex;
	for_each( m_vRn.begin()+16, m_vRn.end(), [&]( CDev::typeBuffer::value_type c ){
		ss.fill(_T('0') );
		ss.width(2);
		ss << c;
	});
	DisplayRightInfoView( _tstring( ss.str() ).c_str() );
	//
	ss.str(_T(""));
	ss << _T(" : [calculated mac] : ");
	ss << hex;
	for_each( vMac4.begin(), vMac4.end(), [&]( CDev::typeBuffer::value_type c ){
		ss.fill(_T('0') );
		ss.width(2);
		ss << c;
	});
	DisplayRightInfoView( _tstring( ss.str() ).c_str() );

	for( int i = 0; i<4; i++ ){
		if( m_vRn[16+i] != vMac4[i] ){
			DisplayRightInfoView( _T(" : [ERROR  ] : mismatching mac-value.") );
			return;
		}
	}//end for

	//
	unsigned int nFail(0);

	if( device_external_authentication( vPlain_Rn, nFail ) ){
		sinfo = _T(" : {SUCCESS] : external authentication.");
	}
	else{
		sinfo = _T(" : {ERROR  ] : external authentication. error counter : ");
		sinfo += to_tstring( static_cast<long long>(nFail) );
	}

	DisplayRightInfoView( sinfo.c_str() );
}


void Ctp_lpu239_hardDoc::OnToolLeaveConfigMode()
{
	CDev::typeUid uid(  get_current_uid() );
	if( uid.empty() ){
		DisplayRightInfoView( _T(" : [ERROR  ] : not selected device.") );
		return;
	}

	if( device_leave_config() ){
		DisplayRightInfoView( _T(" : [SUCCESS] : leave config mode.") );
	}
	else{
		DisplayRightInfoView( _T(" : [ERROR  ] : leave config mode.") );
	}
}


bool Ctp_lpu239_hardDoc::device_open()
{
	if( m_CurUid.empty() )
		return false;

	CDevManager *pManager = CDevManager::getManager();
	CDevHidLpu239 & msr( static_cast<CDevHidLpu239 &>(pManager->getManagedDev( m_CurUid )) );

	if( msr == CDev::dt_null ){
		return false;
	}
	//

	if( msr.open( true ) )
		return true;
	else
		return false;
}

bool Ctp_lpu239_hardDoc::device_close()
{
	if( m_CurUid.empty() )
		return false;

	CDevManager *pManager = CDevManager::getManager();
	CDevHidLpu239 & msr( static_cast<CDevHidLpu239 &>(pManager->getManagedDev( m_CurUid )) );

	if( msr == CDev::dt_null ){
		return false;
	}
	//

	return msr.close();
}

bool Ctp_lpu239_hardDoc::device_enter_config()
{
	if( m_CurUid.empty() )
		return false;

	CDevManager *pManager = CDevManager::getManager();
	CDevHidLpu239 & msr( static_cast<CDevHidLpu239 &>(pManager->getManagedDev( m_CurUid )) );

	if( msr == CDev::dt_null ){
		return false;
	}
	//

	return msr.df_enterConfig();
}

bool Ctp_lpu239_hardDoc::device_leave_config()
{
	if( m_CurUid.empty() )
		return false;

	CDevManager *pManager = CDevManager::getManager();
	CDevHidLpu239 & msr( static_cast<CDevHidLpu239 &>(pManager->getManagedDev( m_CurUid )) );

	if( msr == CDev::dt_null ){
		return false;
	}
	//
	return msr.df_leaveConfig();
}

bool Ctp_lpu239_hardDoc::device_get_randum_number()
{
	if( m_CurUid.empty() )
		return false;

	CDevManager *pManager = CDevManager::getManager();
	CDevHidLpu239 & msr( static_cast<CDevHidLpu239 &>(pManager->getManagedDev( m_CurUid )) );

	if( msr == CDev::dt_null ){
		return false;
	}
	//
	m_vRn = msr.getRandomNumer();

	if( m_vRn.empty() )
		return false;
	else
		return true;
}

bool Ctp_lpu239_hardDoc::device_external_authentication( CDev::typeBuffer & vRn, unsigned int & nFail )
{
	if( m_CurUid.empty() )
		return false;

	CDevManager *pManager = CDevManager::getManager();
	CDevHidLpu239 & msr( static_cast<CDevHidLpu239 &>(pManager->getManagedDev( m_CurUid )) );

	if( msr == CDev::dt_null ){
		return false;
	}
	//
	if( msr.df_external_authentication( vRn, nFail ) )
		return true;
	else{

		return false;
	}
}

bool Ctp_lpu239_hardDoc::device_change_authentication_key0( const CDev::typeBuffer & vKey )
{
	if( m_CurUid.empty() )
		return false;

	CDevManager *pManager = CDevManager::getManager();
	CDevHidLpu239 & msr( static_cast<CDevHidLpu239 &>(pManager->getManagedDev( m_CurUid )) );

	if( msr == CDev::dt_null ){
		return false;
	}
	//
	return msr.setAuthKey0( vKey ).df_setAuthKey0();
}

bool Ctp_lpu239_hardDoc::device_change_authentication_key1(  const CDev::typeBuffer & vKey  )
{
	if( m_CurUid.empty() )
		return false;

	CDevManager *pManager = CDevManager::getManager();
	CDevHidLpu239 & msr( static_cast<CDevHidLpu239 &>(pManager->getManagedDev( m_CurUid )) );

	if( msr == CDev::dt_null ){
		return false;
	}
	//
	return msr.setAuthKey1( vKey ).df_setAuthKey1();
}

bool Ctp_lpu239_hardDoc::device_change_ksn(  const CDev::typeBuffer & vksn  )
{
	if( m_CurUid.empty() )
		return false;

	CDevManager *pManager = CDevManager::getManager();
	CDevHidLpu239 & msr( static_cast<CDevHidLpu239 &>(pManager->getManagedDev( m_CurUid )) );

	if( msr == CDev::dt_null ){
		return false;
	}
	//
	return msr.setKsn( vksn ).df_setKsn();
}

bool Ctp_lpu239_hardDoc::device_change_ipek0(  const CDev::typeBuffer & vKey  )
{
	if( m_CurUid.empty() )
		return false;

	CDevManager *pManager = CDevManager::getManager();
	CDevHidLpu239 & msr( static_cast<CDevHidLpu239 &>(pManager->getManagedDev( m_CurUid )) );

	if( msr == CDev::dt_null ){
		return false;
	}
	//
	return msr.setEnKey0( vKey ).df_setEnKey0();
}

bool Ctp_lpu239_hardDoc::device_change_ipek1(  const CDev::typeBuffer & vKey  )
{
	if( m_CurUid.empty() )
		return false;

	CDevManager *pManager = CDevManager::getManager();
	CDevHidLpu239 & msr( static_cast<CDevHidLpu239 &>(pManager->getManagedDev( m_CurUid )) );

	if( msr == CDev::dt_null ){
		return false;
	}
	//
	return msr.setEnKey1( vKey ).df_setEnKey1();
}

void Ctp_lpu239_hardDoc::OnInputAuthenticationKey()
{
	for( int i = 0; i<2; ++i ){
		m_vAuthKey[i].resize( 16, 0 );
	}//end for
	//{'-','E','L','P','U','S','K','2','0','1','4','1','1','0','5','-'},

	CInputDlg dlg( _T("Input Authentication Key PART1"), 16 );
	dlg.set_Input( _tstring( _T("2d454c5055534b32303134313130352d") ) );

	if( dlg.DoModal() == IDOK ){
		m_vAuthKey[0] = dlg.get_input_bytes();

		dlg.set_Title( _T("Input Authentication Key PART2") );
		dlg.set_Input( _tstring( _T("00000000000000000000000000000000") ) );

		if( dlg.DoModal() == IDOK ){
			m_vAuthKey[1] = dlg.get_input_bytes();
			DisplayRightInfoView( _T(" : [SUCCESS] : you can execute \"change Authentication key\" of Tool menu.") );
			return;
		}
	}

	for( int i = 0; i<2; ++i ){
		m_vAuthKey[i].resize( 16, 0 );
	}//end for

}

void Ctp_lpu239_hardDoc::OnInputKsn()
{
	m_vKsn.resize( 10, 0 );

	CInputDlg dlg( _T("Input KSN"), 10 );
	dlg.set_Input( _tstring( _T("FFFF9876543210E00000") ) );

	if( dlg.DoModal() == IDOK ){
		m_vKsn = dlg.get_input_bytes();
		DisplayRightInfoView( _T(" : [SUCCESS] : you can execute \"change KSN\" of Tool menu.") );
		return;
	}
}

void Ctp_lpu239_hardDoc::OnInputEncryptionKey()
{
	for( int i = 0; i<2; ++i ){
		m_vEnKey[i].resize( 16, 0 );
	}//end for
	//

	CInputDlg dlg( _T("Input Encryption Key or IPEK  PART1"), 16 );
	dlg.set_Input( _tstring( _T("6AC292FAA1315B4D858AB3A3D7D5933A") ) );

	if( dlg.DoModal() == IDOK ){
		m_vEnKey[0] = dlg.get_input_bytes();

		dlg.set_Title( _T("Input Encryption Key or IPEK  PART2") );
		dlg.set_Input( _tstring( _T("00000000000000000000000000000000") ) );

		if( dlg.DoModal() == IDOK ){
			m_vEnKey[1] = dlg.get_input_bytes();
			DisplayRightInfoView( _T(" : [SUCCESS] : you can execute \"change Encryption key(IPEK)\" of Tool menu.") );
			return;
		}
	}

	for( int i = 0; i<2; ++i ){
		m_vEnKey[i].resize( 16, 0 );
	}//end for

}


void Ctp_lpu239_hardDoc::OnToolReadUid()
{
	CDevManager *pManager = CDevManager::getManager();
	CDevHidLpu239 & msr( static_cast<CDevHidLpu239 &>(pManager->getManagedDev( m_CurUid )) );

	_tstring sInfo;

	if( msr == CDev::dt_null ){
		sInfo = _T(" : [ERROR  ] : Selects a target device.");
	}
	else{
		sInfo = _T(" : [SUCCESS] : Device UID : ") + msr.getUidString() ;
	}

	DisplayRightInfoView( sInfo.c_str() );

}


void Ctp_lpu239_hardDoc::OnToolApply()
{
	CDevManager *pManager = CDevManager::getManager();
	CDevHidLpu239 & msr( static_cast<CDevHidLpu239 &>(pManager->getManagedDev( m_CurUid )) );

	_tstring sInfo;

	if( msr == CDev::dt_null ){
		sInfo = _T(" : [ERROR  ] : Selects a target device.");
	}
	else{
		if( msr.df_applyConfig() )
			sInfo = _T(" : [SUCCESS] : Saved the current parameters to none-volatile memory.");
		else
			sInfo = _T(" : [ERROR  ] : failure saving the current parameters to none-volatile memory. ");
	}

	DisplayRightInfoView( sInfo.c_str() );

}


void Ctp_lpu239_hardDoc::OnToolGotoBootloader()
{
	
}


void Ctp_lpu239_hardDoc::OnToolChangeInterface()
{

}
