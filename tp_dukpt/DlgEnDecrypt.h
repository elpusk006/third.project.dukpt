#pragma once


// CDlgEnDecrypt ��ȭ �����Դϴ�.

class CDlgEnDecrypt : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgEnDecrypt)

public:
	CDlgEnDecrypt(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgEnDecrypt();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_EN_DECRYPTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CString m_sKey;
	CString m_sData;
	afx_msg void OnBnClickedOk();
};
