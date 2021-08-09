
// LeftView.h : interface of the CLeftView class
//


#pragma once


#include <tg_dev_manager.h>
#include <tg_dev_hid_lpu239.h>
#include <xmllite.h>

#include <map>

class Ctp_lpu239_hardDoc;

class CLeftView : public CTreeView
{
protected: // create from serialization only
	CLeftView();
	DECLARE_DYNCREATE(CLeftView)

// Attributes
public:
	Ctp_lpu239_hardDoc* GetDocument();

	
// Operations
public:

// Overrides
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CLeftView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

private:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
};

#ifndef _DEBUG  // debug version in LeftView.cpp
inline Ctp_lpu239_hardDoc* CLeftView::GetDocument()
   { return reinterpret_cast<Ctp_lpu239_hardDoc*>(m_pDocument); }
#endif

