#pragma once


// CDlgEnDecrypt 대화 상자입니다.

class CDlgEnDecrypt : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgEnDecrypt)

public:
	CDlgEnDecrypt(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgEnDecrypt();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_EN_DECRYPTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_sKey;
	CString m_sData;
	afx_msg void OnBnClickedOk();
};
