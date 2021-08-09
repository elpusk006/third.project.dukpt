// InfoView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "tp_lpu239_hard.h"
#include "InfoView.h"
#include "tp_lpu239_hardDoc.h"

// CInfoView

IMPLEMENT_DYNCREATE(CInfoView, CListView)

CInfoView::CInfoView()
{

}

CInfoView::~CInfoView()
{
}

BEGIN_MESSAGE_MAP(CInfoView, CListView)
END_MESSAGE_MAP()

BOOL CInfoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style &= ~LVS_TYPEMASK;
	cs.style |= LVS_REPORT;
	
	//

	return CListView::PreCreateWindow(cs);
}

void CInfoView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();


	// TODO: You may populate your ListView with items by directly accessing
	//  its list control through a call to GetListCtrl().

	// this code only works for a report-mode list view
	//ASSERT(GetStyle() & LVS_REPORT);

	//GetListCtrl().SetExtendedStyle( LVS_EX_FULLROWSELECT );

	//GetListCtrl().InsertColumn( 0, _T("infomation"), LVCFMT_LEFT, 100, 0);

	GetListCtrl().DeleteAllItems();
	GetListCtrl().DeleteColumn(0);

	GetListCtrl().InsertColumn( 0, _T("infomation"), LVCFMT_LEFT,500 );

	DisplayInfo( _T("Start .......") );
}

// CInfoView 진단입니다.

#ifdef _DEBUG
void CInfoView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void CInfoView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif

Ctp_lpu239_hardDoc* CInfoView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Ctp_lpu239_hardDoc)));
	return (Ctp_lpu239_hardDoc*)m_pDocument;
}
#endif //_DEBUG


// CInfoView 메시지 처리기입니다.
// Ctp_lpu239_hardView message handlers
void CInfoView::OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
{
	//TODO: add code to react to the user changing the view style of your window	
	CListView::OnStyleChanged(nStyleType,lpStyleStruct);	
}

void CInfoView::DisplayInfo( LPCTSTR sInfo )
{
	CListCtrl& listCtrl = GetListCtrl();

	if( listCtrl.GetSelectionMark() >= 0 ){
		listCtrl.SetItemState(  listCtrl.GetSelectionMark(), NULL, LVIS_SELECTED|LVIS_FOCUSED ); 
	}

	int nIndex = listCtrl.InsertItem( listCtrl.GetItemCount(), sInfo );

	if( nIndex < 0 )
		return;
	//
	listCtrl.SetSelectionMark( nIndex );
	listCtrl.SetItemState( nIndex, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED );
	listCtrl.SetFocus();
}