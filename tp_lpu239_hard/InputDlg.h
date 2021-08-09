#pragma once


#include <tg_dev_manager.h>
#include <tg_dev_hid_lpu239.h>
#include <xmllite.h>
#include "afxwin.h"

// CInputDlg 대화 상자입니다.

class CInputDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInputDlg)

public:
	CInputDlg( LPCTSTR sTitle, unsigned int nSize = 0, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CInputDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_INPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonTitle();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	_tstring get_Input(){		return m_sInput;	}
	void set_Input( const _tstring & sinput ){	m_sInput = sinput;	}
	void set_Title( const _tstring & sTitle ){	m_sTitle = sTitle;		}

	CDev::typeBuffer get_input_bytes(){	return m_vInput;	}

private:
	enum type_mode{
		mode_input_hex,
		mode_input_ascii
	};

private:
	CDev::typeBuffer m_vInput;
	CEdit m_EditInput;
	_tstring m_sTitle;
	_tstring m_sInput;
	unsigned int m_nSize;
	type_mode m_mode;
	

private:
	// don't call methods
	CInputDlg();
};
