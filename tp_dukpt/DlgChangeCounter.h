#pragma once


// CDlgChangeCounter ��ȭ �����Դϴ�.

class CDlgChangeCounter : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgChangeCounter)

public:
	CDlgChangeCounter(unsigned char counter[10], CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgChangeCounter();

	void GetCounter( unsigned char counter[10] );

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_CHANGE_COUNTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	DWORD m_dwCounter;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
