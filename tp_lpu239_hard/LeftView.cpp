
// LeftView.cpp : implementation of the CLeftView class
//

#include "stdafx.h"
#include "tp_lpu239_hard.h"

#include "tp_lpu239_hardDoc.h"
#include "LeftView.h"
#include "MainFrm.h"
#include "tp_lpu239_hardView.h"
#include "InfoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLeftView

IMPLEMENT_DYNCREATE(CLeftView, CTreeView)

BEGIN_MESSAGE_MAP(CLeftView, CTreeView)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CLeftView::OnNMDblclk)
END_MESSAGE_MAP()


// CLeftView construction/destruction

CLeftView::CLeftView()
{
	// TODO: add construction code here
}

CLeftView::~CLeftView()
{
}

BOOL CLeftView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs

	return CTreeView::PreCreateWindow(cs);
}

void CLeftView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	// TODO: You may populate your TreeView with items by directly accessing
	//  its tree control through a call to GetTreeCtrl().
	GetDocument()->reset_StrToUid_Map();
	GetDocument()->set_current_uid( CDev::typeUid(0) );

	CDevManager *pManager = CDevManager::getManager();
	CDevManager::typeDevPtrList DevList;
	pManager->getManagedDevList( DevList );

	for_each( DevList.begin(), DevList.end(), [&]( const CDevManager::typeDevPtrList::value_type & ptrDev ){
		GetTreeCtrl().InsertItem( ptrDev->getUidString().c_str() );
		GetDocument()->insert_StrToUid_Map( ptrDev->getUidString(), ptrDev->getUid() );
	});
	
}


// CLeftView diagnostics

#ifdef _DEBUG
void CLeftView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CLeftView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

Ctp_lpu239_hardDoc* CLeftView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Ctp_lpu239_hardDoc)));
	return (Ctp_lpu239_hardDoc*)m_pDocument;
}
#endif //_DEBUG


// CLeftView message handlers


void CLeftView::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActive = reinterpret_cast<LPNMITEMACTIVATE>( pNMHDR );

	if( pNMItemActive->iItem != -1 ){
		HTREEITEM hItem( GetTreeCtrl().GetNextItem(NULL, TVGN_CARET) );
		_tstring sID( GetTreeCtrl().GetItemText( hItem ) );

		CDev::typeUid uid;
		GetDocument()->get_Uid_FromMap( sID, uid );
		GetDocument()->set_current_uid( uid );

		if( !uid.empty() ){
			( (CMainFrame*)AfxGetMainWnd() )->getRightView()->DisplayDeviceInfo( uid );
			( (CMainFrame*)AfxGetMainWnd() )->getRightInfoView()->DisplayInfo( _T(" : Loaded the device information.") );
		}
		else{
			*pResult = 0;
			return;
		}

		if( GetDocument()->device_open() ){
			GetDocument()->DisplayRightInfoView( _T(" : [SUCCESS] : device open ") );
		}
		else{
			GetDocument()->DisplayRightInfoView( _T(" : [ERROR] : device open ") );
			*pResult = 0;
			return;
		}

		// when parenet string
		//hItem = GetTreeCtrl().GetNextItem( hItem, TVGN_PARENT ) ;
		//GetTreeCtrl().GetItemText( hItem );
		//MessageBox( sID.c_str() );

	}
	//
	*pResult = 0;


}
