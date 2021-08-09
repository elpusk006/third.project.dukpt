
// tp_lpu239_hardView.cpp : implementation of the Ctp_lpu239_hardView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "tp_lpu239_hard.h"
#endif

#include "tp_lpu239_hardDoc.h"
#include "tp_lpu239_hardView.h"
#include "LeftView.h"
#include "MainFrm.h"

#include <tg_dev_manager.h>
#include <tg_dev_hid_lpu239.h>
#include <xmllite.h>


using namespace std;
using namespace thirdgeneration;
using namespace device_elpusk;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 

// Ctp_lpu239_hardView

IMPLEMENT_DYNCREATE(Ctp_lpu239_hardView, CListView)

BEGIN_MESSAGE_MAP(Ctp_lpu239_hardView, CListView)
	ON_WM_STYLECHANGED()
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// Ctp_lpu239_hardView construction/destruction

Ctp_lpu239_hardView::Ctp_lpu239_hardView()
{
	// TODO: add construction code here

}

Ctp_lpu239_hardView::~Ctp_lpu239_hardView()
{
}

BOOL Ctp_lpu239_hardView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style &= ~LVS_TYPEMASK;
	cs.style |= LVS_REPORT;
	
	//

	return CListView::PreCreateWindow(cs);
}

void Ctp_lpu239_hardView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();


	// TODO: You may populate your ListView with items by directly accessing
	//  its list control through a call to GetListCtrl().

	// this code only works for a report-mode list view
	ASSERT(GetStyle() & LVS_REPORT);

	//CLeftView *pView = (CLeftView*)( (CMainFrame*)AfxGetMainWnd() )->m_wndSplitter.GetPane(0, 0);
	GetListCtrl().SetExtendedStyle( LVS_EX_FULLROWSELECT );
}

void Ctp_lpu239_hardView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void Ctp_lpu239_hardView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// Ctp_lpu239_hardView diagnostics

#ifdef _DEBUG
void Ctp_lpu239_hardView::AssertValid() const
{
	CListView::AssertValid();
}

void Ctp_lpu239_hardView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

Ctp_lpu239_hardDoc* Ctp_lpu239_hardView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Ctp_lpu239_hardDoc)));
	return (Ctp_lpu239_hardDoc*)m_pDocument;
}
#endif //_DEBUG


// Ctp_lpu239_hardView message handlers
void Ctp_lpu239_hardView::OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
{
	//TODO: add code to react to the user changing the view style of your window	
	CListView::OnStyleChanged(nStyleType,lpStyleStruct);	
}

void Ctp_lpu239_hardView::DisplayDeviceInfo( const thirdgeneration::CDev::typeUid & uid )
{
	CListCtrl& listCtrl = GetListCtrl();
	listCtrl.DeleteAllItems();

	for( int i = 0; i<2; i++ )
		listCtrl.DeleteColumn(0);

	CDevManager *pManager = CDevManager::getManager();
	CDevHidLpu239 & msr( static_cast<CDevHidLpu239 &>(pManager->getManagedDev( uid )) );

	if( msr == CDev::dt_null ){
		return;
	}

	int i( 0 );
	listCtrl.InsertColumn( 0, _T("property"), LVCFMT_CENTER, 100, 0);
	listCtrl.InsertColumn( i+1, msr.getUidString().c_str(), LVCFMT_LEFT, 100+150*(i+1), i+1);

	// name section
	int nProperty(0);
	listCtrl.InsertItem( nProperty, _T("name") );
	listCtrl.SetItem( nProperty, i+1, LVIF_TEXT, msr.getNameString().c_str() , 0, 0, 0, NULL );

	// version section
	++nProperty;
	listCtrl.InsertItem( nProperty, _T("version") );
	listCtrl.SetItem( nProperty, i+1, LVIF_TEXT, msr.getVersion().getString().c_str() , 0, 0, 0, NULL );

	// interface section
	++nProperty;
	listCtrl.InsertItem( nProperty, _T("interface") );
	listCtrl.SetItem( nProperty, i+1, LVIF_TEXT, msr.getInterfaceString().c_str() , 0, 0, 0, NULL );

	// language section
	++nProperty;
	listCtrl.InsertItem( nProperty, _T("language") );
	listCtrl.SetItem( nProperty, i+1, LVIF_TEXT, msr.getLanguageString().c_str() , 0, 0, 0, NULL );

}