#pragma once


// CListBoxEx

class CListBoxEx : public CListBox
{
	DECLARE_DYNAMIC(CListBoxEx)

public:
	CListBoxEx();
	virtual ~CListBoxEx();

private:
	void CopyStrToClipboard(CString str);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnCopy();
};


