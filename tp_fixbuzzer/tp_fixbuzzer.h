
// tp_fixbuzzer.h : tp_fixbuzzer ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// Ctp_fixbuzzerApp:
// �� Ŭ������ ������ ���ؼ��� tp_fixbuzzer.cpp�� �����Ͻʽÿ�.
//

class Ctp_fixbuzzerApp : public CWinAppEx
{
public:
	Ctp_fixbuzzerApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern Ctp_fixbuzzerApp theApp;
