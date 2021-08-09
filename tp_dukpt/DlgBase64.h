#pragma once


// CDlgBase64 ��ȭ �����Դϴ�.

class CDlgBase64 : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgBase64)

public:
	CDlgBase64(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgBase64();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_BASE64 };

	void setTitle( const CString & sTitle ){	m_sTitle = sTitle;	}
	void setDoButtonText( const CString & sDoButtonText ){	m_sDoButtonText = sDoButtonText;	}
	void setEncodeMode( bool bEncodeMode = true ){	m_bEncodeMode = bEncodeMode;	}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

private:
	CString m_sTitle;
	CString m_sDoButtonText;
	bool m_bEncodeMode;
public:
	afx_msg void OnBnClickedOk();
	CString m_sIn;
	CString m_sOut;
	virtual void OnCancel();
};
