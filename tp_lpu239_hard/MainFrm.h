
// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "LeftView.h"
#include "tp_lpu239_hardView.h"

class Ctp_lpu239_hardView;
class CInfoView;

class CMainFrame : public CFrameWndEx
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
protected:
	CSplitterWnd m_wndSplitter;
	CSplitterWnd m_wndSplitterRight;
public:
	CLeftView *getLeftView()
	{
		return (CLeftView*)( m_wndSplitter.GetPane(0, 0) );
	}

	Ctp_lpu239_hardView *getRightView()
	{
		return (Ctp_lpu239_hardView*)( m_wndSplitterRight.GetPane( 0, 0 ) );
	}

	CInfoView *getRightInfoView()
	{
		return (CInfoView*)( m_wndSplitterRight.GetPane( 1, 0 ) );
	}
	
	// Operations
public:

// Overrides
public:

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
	Ctp_lpu239_hardView* GetRightPane();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CMFCMenuBar       m_wndMenuBar;
	CMFCStatusBar     m_wndStatusBar;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnUpdateViewStyles(CCmdUI* pCmdUI);
	afx_msg void OnViewStyle(UINT nCommandID);
	afx_msg void OnViewCustomize();
	DECLARE_MESSAGE_MAP()

};


