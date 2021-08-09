#pragma once

#include "tp_lpu239_hardDoc.h"

// CInfoView ���Դϴ�.

class CInfoView : public CListView
{
	DECLARE_DYNCREATE(CInfoView)

protected:
	CInfoView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CInfoView();

public:
	Ctp_lpu239_hardDoc* GetDocument() const;
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void DisplayInfo( LPCTSTR sInfo );

protected:
	virtual void OnInitialUpdate(); // called first time after construct


public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	afx_msg void OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct);
	DECLARE_MESSAGE_MAP()
};


#ifndef _DEBUG  // debug version in tp_lpu239_hardView.cpp
inline Ctp_lpu239_hardDoc* CInfoView::GetDocument() const
   { return reinterpret_cast<Ctp_lpu239_hardDoc*>(m_pDocument); }
#endif

