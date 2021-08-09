
// tp_lpu239_hard.h : main header file for the tp_lpu239_hard application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// Ctp_lpu239_hardApp:
// See tp_lpu239_hard.cpp for the implementation of this class
//

class Ctp_lpu239_hardApp : public CWinAppEx
{
public:
	Ctp_lpu239_hardApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
};

extern Ctp_lpu239_hardApp theApp;
