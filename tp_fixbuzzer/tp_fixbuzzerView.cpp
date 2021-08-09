
// tp_fixbuzzerView.cpp : Ctp_fixbuzzerView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "tp_fixbuzzer.h"
#endif

#include "tp_fixbuzzerDoc.h"
#include "CntrItem.h"
#include "resource.h"
#include "tp_fixbuzzerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma comment(lib, "tg_device.lib")
#pragma comment(lib, "tg_device_elpusk.lib")

//{a5dcbf10-6530-11d2-901f-00c04fb951ed}
#define USB_CLASS_GUID \
 { 0xa5dcbf10, 0x6530, 0x11d2, { 0x90, 0x1f, 0x00, 0xc0, 0x4f, 0xb9, 0x51, 0xed } }

// Ctp_fixbuzzerView

IMPLEMENT_DYNCREATE(Ctp_fixbuzzerView, CRichEditView)

BEGIN_MESSAGE_MAP(Ctp_fixbuzzerView, CRichEditView)
	ON_WM_DESTROY()
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_FILE_SAVE, &Ctp_fixbuzzerView::OnFileSave)
	ON_COMMAND(ID_FILE_OPEN, &Ctp_fixbuzzerView::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE_AS, &Ctp_fixbuzzerView::OnFileSaveAs)
	ON_COMMAND(ID_FILE_NEW, &Ctp_fixbuzzerView::OnFileNew)
	ON_WM_CLOSE()
	ON_WM_DEVICECHANGE()
END_MESSAGE_MAP()

// Ctp_fixbuzzerView 생성/소멸

Ctp_fixbuzzerView::Ctp_fixbuzzerView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

Ctp_fixbuzzerView::~Ctp_fixbuzzerView()
{
}

BOOL Ctp_fixbuzzerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CRichEditView::PreCreateWindow(cs);
}

void Ctp_fixbuzzerView::OnInitialUpdate()
{
	CRichEditView::OnInitialUpdate();


	// 인쇄 여백(720 twips = 1/2 inch)을 설정합니다.
	SetMargins(CRect(720, 720, 720, 720));

	RegisterDevice();
	TRACE( _T("Ctp_fixbuzzerView::OnInitialUpdate().\r\n") );
}

void Ctp_fixbuzzerView::OnDestroy()
{
	UnRegisterEventDevice( m_diNotifyHandle );
	// 소멸 중인 항목을 비활성화합니다.
	// 분할 창이 사용될 경우 이 작업은 중요합니다.
   COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
   if (pActiveItem != NULL && pActiveItem->GetActiveView() == this)
   {
      pActiveItem->Deactivate();
      ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
   }
   CRichEditView::OnDestroy();
}


void Ctp_fixbuzzerView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void Ctp_fixbuzzerView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// Ctp_fixbuzzerView 진단

#ifdef _DEBUG
void Ctp_fixbuzzerView::AssertValid() const
{
	CRichEditView::AssertValid();
}

void Ctp_fixbuzzerView::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}

Ctp_fixbuzzerDoc* Ctp_fixbuzzerView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Ctp_fixbuzzerDoc)));
	return (Ctp_fixbuzzerDoc*)m_pDocument;
}
#endif //_DEBUG


// Ctp_fixbuzzerView 메시지 처리기


void Ctp_fixbuzzerView::OnFileSave()
{
	
}


void Ctp_fixbuzzerView::OnFileOpen()
{

}


void Ctp_fixbuzzerView::OnFileSaveAs()
{

}


void Ctp_fixbuzzerView::OnFileNew()
{
	//
	TRACE( _T("Ctp_fixbuzzerView::OnFileNew().\r\n") );
}



void Ctp_fixbuzzerView::OnClose()
{
	CRichEditView::OnClose();
}

BOOL Ctp_fixbuzzerView::OnDeviceChange( UINT nEventType, DWORD dwData )
{
	int nResult;
	GUID PINClassGuid =  USB_CLASS_GUID;//{0x4D1E55B2, 0xF16F, 0x11CF, { 0x88, 0xCB, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30 } };//hid guid
	WORD wVID[1];;
	WORD wPID[1];
	int i;
	
	wVID[0]=0x134b;
	wPID[0]=0x0206;	

	CDevManager::typeDevicePathList DevPathList;
	CString sMsg;

	//HidD_GetHidGuid(&PINClassGuid);
	nResult=DeviceMultiChangeMsgHandler( nEventType,dwData,wVID,wPID,1,PINClassGuid );

	switch(nResult){
		case DEV_CHANGE_PLUGIN_WANTED_USB:
			
			Beep( FRQ_DEV_IN_OUT,DUR_DEV_IN_OUT );
			if( _GetDevicePathList( DevPathList, sMsg ) ){
				//_FixBuzzer( sMsg );
				AddInformation( sMsg, RGB(0,128,0) );
			}
			//OnButtonSearch();
			i=1;//exit for
			break;
		case DEV_CHANGE_PLUGOUT_WANTED_USB:
			AddInformation( _T("Plugout lpu237\r\n"),RGB(0,128,0) );
			Beep( FRQ_DEV_IN_OUT,DUR_DEV_IN_OUT );
			//_GetDevicePathList( DevPathList, sMsg );
			//m_PinIO_Obj.Close();
			//OnButtonSearch();
			i=1;//exit for
			break;
		case DEV_CHANGE_PLUGIN_THE_OTHER_USB:
			AddInformation( _T("USB device(except pinpad)Plugin\r\n"),RGB(0,255,0) );
			break;
		case DEV_CHANGE_PLUGOUT_THE_OTHER_USB:
			AddInformation( _T("USB device(except pinpad)Plugout\r\n"),RGB(0,255,0) );
			break;
		case DEV_CHANGE_NO_NEED_EVENT:
			break;
	}//end switch.
	//
	return TRUE;
}

void Ctp_fixbuzzerView::AddInformation( LPCTSTR sInfo, COLORREF col /*= RGB(0,0,0)*/ )
{
	Ctp_fixbuzzerDoc *pDoc( GetDocument() );

	SetWindowText( sInfo );
	
	CRichEditCtrl& r = GetRichEditCtrl();
	r.SetSel(r.GetTextLength(), r.GetTextLength());
}


BOOL Ctp_fixbuzzerView::RegisterDevice()
{
	GUID USBClassGuid=USB_CLASS_GUID;

	m_diNotifyHandle=RegisterEventDevice( this->m_hWnd,USBClassGuid );
	if( m_diNotifyHandle==NULL )
		return FALSE;

	return TRUE;
}

bool Ctp_fixbuzzerView::_GetDevicePathList( CDevManager::typeDevicePathList & DevPathList, CString & sOutMsg )
{
	CDevManager::typeDevicePathList pathList;
	CDevManager *pManager = CDevManager::getManager();
	pManager->UpdateDevicePathList()->getCurDevicePathList( pathList )->filter( pathList, 0x134b, 0x0206, 1 );

	//remove header device wit version.
	for_each( pathList.begin(), pathList.end(), [&]( _tstring path ){

		CDevHidLpu237 msr(path);

		if( !msr.open(true) ) return;
		if( !msr.df_enterConfig() ){ 	msr.close();	return;	}
		if( !msr.df_getSystemParameters() ){ 	msr.close();	return;	}

		CDevHidLpu237::typeVersion version( msr.getVersion() );

		sOutMsg.Format( _T(" Device version : %s.\r\n Buzzer frequency : %d.\r\n"), version.getString().c_str(), msr.getBuzzerFrequency() );

		if( version.getMajor() != 4 || version.getMinor() != 10 || version.getFix() != 7 ){
			DevPathList.push_back( path );
		}

		if( version.getMajor() >=5 ){
			//fix buzzer value
			if( msr.getBuzzerFrequency() != 2500 ){
				msr.setBuzzerFrequency( 2500 );
				CString stemp( _T(" * Fixed Buzzer value to 2500.\r\n") );
				sOutMsg += stemp;
			}
		}//

		if( !msr.df_applyConfig() ){ 	msr.close();	return;	}
		if( !msr.df_leaveConfig() ){ 	msr.close();	return;	}

		msr.df_leaveOpos();	//ringing buzzer.
		msr.close();

	});

	if( DevPathList.empty() )		return false;
	else	return true;
}

bool Ctp_fixbuzzerView::_FixBuzzer(  CString & sOutMsg )
{
	bool bResult(false);

	CDevManager::typeDevPtrList listDevPtr;
	CDevManager::getManager()->getManagedDevList( listDevPtr );

	if( !listDevPtr.empty() ){

		CDevManager::typeDevPtr ptrDev( *listDevPtr.begin() );
		CDevHidLpu237 *pMsr( static_cast<CDevHidLpu237*>(ptrDev.get()) );

		try{
			if( !pMsr->open() )				throw 1;
			if( !pMsr->df_enterConfig() )	throw 2;
			if( !pMsr->df_getSystemParameters() ) throw 3;

			if( pMsr->getVersion().getMajor() >=5 ){
				//fix buzzer value
				if( pMsr->getBuzzerFrequency() != 2500 ){
					pMsr->setBuzzerFrequency( 2500 );
				}
			}//

			//if( !pMsr->setInterface( inf ).df_setInterface() )	throw 4;
			if( !pMsr->df_applyConfig() )	throw 5;
			if( !pMsr->df_leaveConfig() )	throw 6;

			pMsr->df_leaveOpos();	//ringing buzzer.
			pMsr->close();
			bResult = true;
		}
		catch( int nError ){
			if( nError != 1 ){
				pMsr->close();
			}
		}
	}

	return bResult;
}