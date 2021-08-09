#pragma once


// CDlgChangeCounter 대화 상자입니다.

class CDlgChangeCounter : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgChangeCounter)

public:
	CDlgChangeCounter(unsigned char counter[10], CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgChangeCounter();

	void GetCounter( unsigned char counter[10] );

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_CHANGE_COUNTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	DWORD m_dwCounter;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
