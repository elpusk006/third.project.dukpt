
// tp_dukpt.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// Ctp_dukptApp:
// �� Ŭ������ ������ ���ؼ��� tp_dukpt.cpp�� �����Ͻʽÿ�.
//

class Ctp_dukptApp : public CWinApp
{
public:
	Ctp_dukptApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern Ctp_dukptApp theApp;