#pragma once


// CDlgDecryption ��ȭ �����Դϴ�.

class CDlgDecryption : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgDecryption)

public:
	CDlgDecryption(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgDecryption();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_DECRYPTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	CString m_sEncrytionText;
};
