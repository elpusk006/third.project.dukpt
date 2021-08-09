// ListBoxEx.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "tp_dukpt.h"
#include "ListBoxEx.h"


// CListBoxEx

IMPLEMENT_DYNAMIC(CListBoxEx, CListBox)

CListBoxEx::CListBoxEx()
{

}

CListBoxEx::~CListBoxEx()
{
}


BEGIN_MESSAGE_MAP(CListBoxEx, CListBox)
	ON_WM_RBUTTONUP()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_COPY, &CListBoxEx::OnCopy)
END_MESSAGE_MAP()



// CListBoxEx 메시지 처리기입니다.




void CListBoxEx::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 

	CListBox::OnRButtonUp(nFlags, point);
}

void CListBoxEx::CopyStrToClipboard(CString str)
{
	OpenClipboard();
	EmptyClipboard();

	int nLen =  (str.GetLength() + 1) *sizeof(TCHAR);
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, nLen );
	LPTSTR pGlobal = (LPTSTR)GlobalLock(hGlobal);

	USES_CONVERSION_EX;
	CopyMemory( pGlobal, T2CW_EX(str, _ATL_SAFE_ALLOCA_DEF_THRESHOLD), nLen);
	SetClipboardData(CF_UNICODETEXT, hGlobal);
	GlobalUnlock(hGlobal);
	GlobalFree( hGlobal );
	CloseClipboard();
}

LRESULT CListBoxEx::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO ;

	return CListBox::WindowProc(message, wParam, lParam);
}


void CListBoxEx::OnContextMenu(CWnd* pWnd, CPoint point)
{
	if((point.x == -1) && (point.y == -1)){
		// Keystroke invocation
		CRect rect;

		GetClientRect(rect);
		ClientToScreen(rect);

		point = rect.TopLeft();
		point.Offset(5, 5);
	}

	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_POPUP_INFO_LISTBOX));

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	CWnd* pWndPopupOwner = this;

	while(pWndPopupOwner->GetStyle() & WS_CHILD)
	pWndPopupOwner = pWndPopupOwner->GetParent();

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, 
							point.x, 
							point.y,
							pWndPopupOwner);
}


void CListBoxEx::OnCopy()
{
	int nSel( GetCurSel() );

	if( nSel >=0 ){
		CString sData;
		GetText( nSel, sData );
		CopyStrToClipboard( sData );
		//MessageBox( sData, _T("") );
	}
}
