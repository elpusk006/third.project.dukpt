
// tp_lpu239_hardView.h : interface of the Ctp_lpu239_hardView class
//

#pragma once

#include <tg_dev_manager.h>

class Ctp_lpu239_hardView : public CListView
{
protected: // create from serialization only
	Ctp_lpu239_hardView();
	DECLARE_DYNCREATE(Ctp_lpu239_hardView)

// Attributes
public:
	Ctp_lpu239_hardDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void DisplayDeviceInfo( const thirdgeneration::CDev::typeUid & uid );

protected:
	virtual void OnInitialUpdate(); // called first time after construct

	
// Implementation
public:
	virtual ~Ctp_lpu239_hardView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct);
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in tp_lpu239_hardView.cpp
inline Ctp_lpu239_hardDoc* Ctp_lpu239_hardView::GetDocument() const
   { return reinterpret_cast<Ctp_lpu239_hardDoc*>(m_pDocument); }
#endif

