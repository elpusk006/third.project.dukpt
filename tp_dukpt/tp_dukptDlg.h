
// tp_dukptDlg.h : 헤더 파일
//

#pragma once

#include <vector>
#include "afxwin.h"
#include "DlgEnDecrypt.h"
#include "ListBoxEx.h"
#include "DlgBase64.h"
#include "etc.h"

using namespace std;
using namespace __etc;

// Ctp_dukptDlg 대화 상자
class Ctp_dukptDlg : public CDialogEx
{
public:
	/*
	class CANSX9_24_1{
	public:
		CANSX9_24_1();
		~CANSX9_24_1();

	public:
		bool LoadInitialKey( const unsigned char IniPinEnKey[16], const unsigned char IniKSN[8] );
		bool RequestPINEntry( unsigned char formated_and_encrypted_pin_block[18], const unsigned char AccountNum[7], const unsigned char *pPin, unsigned int nPin );
		bool PowerOnReset();

		void GetString( typeStrings & sValue );

	private:
		unsigned char GenerateLRC( const unsigned char Key[16] );
		unsigned int Get_SetBitCount_In_21Bits( const unsigned char s21Bit[3] );
		bool Shift_Right_one_bit_In_21Bits( unsigned char s21Bit[3] );
		unsigned int Get_Bit_Pos_In_21Bits( const unsigned char s21Bit[3] );	//return zero-base position
		bool Increase_one_In_21Bits( unsigned char s21Bit[3] );
		void Set_Bit( unsigned char s21Bit[3], const unsigned char sBase21Bit[3] );
		void NonReversibleKeyGenProcess();
		void EnCrypt_Tdes_self( unsigned char OutEnData[16], const unsigned char Key_and_data[16] );
		void EnCrypt_Tdes();
		void ClearEncrytionCounter();
		void EraseCurrentKeyValue();
		bool IsEncryptionCounterAllZeros();
		bool Label_NewKey();
		void GenerateClearTextPinBlock( unsigned char PinBlock[8], const unsigned char PrimaryAccount[7], const unsigned char Pin[2] );
	private:
		unsigned char m_reg_AccountNum[7];		//max value 0xE8D4A50FFF(999999999999) decimal 12 digit
		unsigned char m_reg_IniKSN[8];		//use 59 bits.
		unsigned char m_reg_KSN[8];

		unsigned char m_reg_EnCnt[3];		//use 21 bits.
		unsigned char m_reg_FutureKey[21][17];
		unsigned char *m_pCurKey;
		unsigned char m_reg_Shift[3];		//use 21 bits.
		unsigned char m_reg_Crypto1[8];
		unsigned char m_reg_Crypto2[8];
		unsigned char m_reg_Key[16];
		unsigned char m_reg_MacKey[16];
		unsigned char m_reg_MacResponseKey[16];
		unsigned char m_reg_DataEnKey[16];
		unsigned char m_reg_DataEnResponseKey[16];

	private:
		CANSX9_24_1( const CANSX9_24_1 & );
		CANSX9_24_1 & operator=( const CANSX9_24_1 & );

	} m_ANSX9_24_1;
	*/
// 생성입니다.
public:
	Ctp_dukptDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TP_DUKPT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_sBDK;
	CString m_sKSN_ini;
	CString m_sKSN_cur;
	CString m_sIPEK;
	CString m_sSessionKey;
	CString m_sPinEncryptionKey;
	CString m_sMacReqEncryptionKey;
	CString m_sMacRespEncryptionKey;
	CString m_sDataReqEncryptionKey;
	CString m_sDataRespEncryptionKey;
	CListBoxEx m_ListInfo;
	CString *m_pPlainData;
	CString *m_pPlainMsg;
	CString *m_pPlainPin;

	CString m_sPlainData_hex;
	CString m_sPlainData_ascii;
	CString m_sPlainMsg_hex;
	CString m_sPlainMsg_ascii;
	CString m_sPlainPin_hex;
	CString m_sPlainPin_ascii;

	CString m_sXor1;
	CString m_sXor2;
	CString m_sXorResult;

	CButton m_CheckPlainDataIsAscii;
	CButton m_CheckPlainMsgIsAscii;
	CButton m_CheckPlainPinIsAscii;
	afx_msg void OnBnClickedButtonGenIpek();

private:
	unsigned char m_bdk[16];
	unsigned char m_ksn_ini[10];
	unsigned char m_ksn_cur[10];
	unsigned char m_ipek[16];
	unsigned char m_session_key[16];
	unsigned char m_pin_encryption_key[16];
	unsigned char m_mac_request_key[16];
	unsigned char m_mac_response_key[16];
	unsigned char m_data_en_request_key[16];
	unsigned char m_data_en_response_key[16];

	//CANSX9_24_1 m_x924;

	CDlgEnDecrypt m_Dlg_tdes_en,m_Dlg_tdes_de,m_Dlg_aes_en,m_Dlg_aes_de;
	CDlgBase64 m_Dlg_base64;

private:
	void dukpt_ini();
	void dukpt_padding_ksn( unsigned char out_ksn[], const unsigned char in_ksn[], unsigned int nIn_ksn );
	bool dukpt_increase_ksn( unsigned char ksn[] );
	//

private:
	void TdesDescryption( typeBytes &out, const typeBytes &in, const unsigned char *pkey );
	void TdesEncrytionWithPadding( typeBytes &out, const typeBytes & in, const unsigned char *pkey, unsigned char cPadding );
	void AesDescryption( typeBytes &out, const typeBytes &in, const unsigned char key[16], const unsigned char iv[16] );
	void AesEncrytionWithPadding( typeBytes &out, const typeBytes & in, const unsigned char *pkey, const unsigned char iv[16], unsigned char cPadding );

	void deb_trace_hexs( unsigned char *sHex, unsigned int nHex, bool bBigEndian = false, LPCTSTR lpPreMsg = NULL );

	void AddInfoList( LPCTSTR lptstr );
	void AddInfoList( unsigned char *psHex, unsigned int nHex, bool bBigEndian = false, LPCTSTR lpPreMsg = NULL );

	void Processing( bool bIncreaseCounter = true );
	
public:
	afx_msg void OnBnClickedButtonClearInfolist();
	afx_msg void OnBnClickedButtonSk();
	afx_msg void OnBnClickedCheckPlainDataIsAscii();
	afx_msg void OnBnClickedCheckPlainMsgIsAscii();
	afx_msg void OnBnClickedCheckPlainPinIsAscii();
	afx_msg void OnBnClickedButtonChangeCounter();
	afx_msg void OnBnClickedButtonDecryption();
	afx_msg void OnBnClickedButtonDecryptionTdes();
	afx_msg void OnBnClickedButtonDecryptionAes();
	afx_msg void OnBnClickedButtonEncryptionTdes();
	afx_msg void OnBnClickedButtonEncryptionAes();
	afx_msg void OnCopy();
	afx_msg void OnBnClickedButtonEncodeBase64();
	afx_msg void OnBnClickedButtonDecodeBase64();
	afx_msg void OnBnClickedButtonXor();
};
