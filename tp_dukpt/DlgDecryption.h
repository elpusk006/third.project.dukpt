#pragma once


// CDlgDecryption 대화 상자입니다.

class CDlgDecryption : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgDecryption)

public:
	CDlgDecryption(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgDecryption();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_DECRYPTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	CString m_sEncrytionText;
};
