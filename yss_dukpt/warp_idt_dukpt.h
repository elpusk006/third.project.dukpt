
#pragma once

#include <Windows.h>

class CWarp_idt_dukpt
{
private:
	typedef	void (__stdcall* type_method_CurrentCryptoKey)( const int , const BYTE *, const BYTE *, BYTE * );
	typedef	void (__stdcall* type_method_TDES_Encryption)(BYTE *, const BYTE *, const BYTE *, const int );
	typedef	void (__stdcall* type_method_TDES_Decryption)(BYTE *, const BYTE *, const BYTE *, const int );
	typedef	void (__stdcall* type_method_AES_Encryption)(BYTE *, const BYTE *, const BYTE *, const int );
	typedef	void (__stdcall* type_method_AES_Decryption)(BYTE *, const BYTE *, const BYTE *, const int );
	typedef	void (__stdcall* type_method_GetInitalKey)(const BYTE *, const BYTE *, BYTE *);

	typedef	void (__stdcall* type_method_dukpt_generate_ipek)( unsigned char ipek[16], const unsigned char in_ksn[10], const unsigned char in_bdk[16] );
	typedef	void (__stdcall* type_method_dukpt_generate_sk)( unsigned char session_key[16], const unsigned char ksn[10], const unsigned char ipek[16] );
	typedef	void (__stdcall* type_method_dukpt_generate_pin_encryption_key)( unsigned char pin_encryption_key[16], const unsigned char session_key[16] );
	typedef	void (__stdcall* type_method_dukpt_generate_mac_request_key)( unsigned char mac_request_key[16], const unsigned char session_key[16] );
	typedef	void (__stdcall* type_method_dukpt_generate_mac_response_key)( unsigned char mac_response_key[16], const unsigned char session_key[16] );
	typedef	void (__stdcall* type_method_dukpt_generate_data_encryption_request_key)( unsigned char data_encryption_key[16], const unsigned char session_key[16] );
	typedef	void (__stdcall* type_method_dukpt_generate_data_encryption_response_key)( unsigned char data_encryption_key[16], const unsigned char session_key[16] );

	typedef	void (__stdcall* type_method_dukpt_generate_mac)( unsigned char mac8[8], const unsigned char mac_encryption_key[16], const unsigned char message[], unsigned int nMsg  );
	typedef	void (__stdcall* type_method_dukpt_get_mac4_from_mac8)( unsigned char mac4[4], const unsigned char mac8[8] );


	typedef	void (__stdcall* type_method_encrypt_ebc_des)( unsigned char *p_out, unsigned int *pn_out, const unsigned char *p_in, const unsigned int n_in, const unsigned char key[8], unsigned char cPadding );
	typedef	void (__stdcall* type_method_decrypt_ebc_des)( unsigned char *p_out, unsigned int *pn_out, const unsigned char *p_in, const unsigned int n_in, const unsigned char key[8] );

	typedef	void (__stdcall* type_method_encrypt_cbc_des)( unsigned char *p_out, unsigned int *pn_out, const unsigned char *p_in, const unsigned int n_in, const unsigned char key[8], const unsigned char iv[8], unsigned char cPadding );
	typedef	void (__stdcall* type_method_decrypt_cbc_des)( unsigned char *p_out, unsigned int *pn_out, const unsigned char *p_in, const unsigned int n_in, const unsigned char key[8], const unsigned char iv[8] );

	typedef	void (__stdcall* type_method_encrypt_ebc_tdes)( unsigned char *p_out, unsigned int *pn_out, const unsigned char *p_in, const unsigned int n_in, const unsigned char key[16], unsigned char cPadding );
	typedef	void (__stdcall* type_method_decrypt_ebc_tdes)( unsigned char *p_out, unsigned int *pn_out, const unsigned char *p_in, const unsigned int n_in, const unsigned char key[16] );

	typedef	void (__stdcall* type_method_encrypt_cbc_tdes)( unsigned char *p_out, unsigned int *pn_out, const unsigned char *p_in, const unsigned int n_in, const unsigned char key[16], const unsigned char iv[8], unsigned char cPadding );
	typedef	void (__stdcall* type_method_decrypt_cbc_tdes)( unsigned char *p_out, unsigned int *pn_out, const unsigned char *p_in, const unsigned int n_in, const unsigned char key[16], const unsigned char iv[8] );

	typedef	void (__stdcall* type_method_encrypt_cbc_aes)( unsigned char *p_out, unsigned int *pn_out, const unsigned char *p_in, const unsigned int n_in, const unsigned char key[16], const unsigned char iv[16], unsigned char cPadding );
	typedef	void (__stdcall* type_method_decrypt_cbc_aes)( unsigned char *p_out, unsigned int *pn_out, const unsigned char *p_in, const unsigned int n_in, const unsigned char key[16], const unsigned char iv[16] );

	typedef	void (__stdcall* type_method_encode_base64)( unsigned char *p_out, unsigned int *pn_out, const unsigned char *p_in, const unsigned int n_in );
	typedef	void (__stdcall* type_method_decode_base64)( unsigned char *p_out, unsigned int *pn_out, const unsigned char *p_in, const unsigned int n_in );

public:
	static CWarp_idt_dukpt *get_security_object()
	{
		static CWarp_idt_dukpt security;
		return &security;
	}

public:
	~CWarp_idt_dukpt()
	{
		if( m_hMod ){
			FreeLibrary( m_hMod );
		}
	}

	void CurrentCryptoKey(const int KeyManagment, const BYTE *InitialKey, const BYTE *KSN, BYTE *CurrentKey)
	{
		m_method_CurrentCryptoKey( KeyManagment, InitialKey, KSN, CurrentKey );
	}

	// TDES Encryption
	void TDES_Encryption(BYTE *Data, const BYTE *Key, const BYTE *InitalVector, const int Length)
	{
		m_method_TDES_Encryption( Data, Key, InitalVector, Length );
	}

	// TDES Decryption
	void TDES_Decryption(BYTE *Data, const BYTE *Key, const BYTE *InitalVector, const int Length)
	{
		m_method_TDES_Decryption( Data, Key, InitalVector, Length);
	}

	// AES Encryption
	void AES_Encryption(BYTE *Data, const BYTE *Key, const BYTE *InitalVector, const int Length)
	{
		m_method_AES_Encryption( Data, Key, InitalVector, Length );
	}

	// AES Decryption
	void AES_Decryption(BYTE *Data, const BYTE *Key, const BYTE *InitalVector, const int Length)
	{
		m_method_AES_Decryption( Data, Key, InitalVector, Length );
	}

	// Get the Initial Key
	void GetInitalKey(const BYTE *KSN, const BYTE *BDK, BYTE *InitialKey)
	{
		m_method_GetInitalKey( KSN, BDK, InitialKey );
	}

	void dukpt_generate_ipek( unsigned char ipek[16], const unsigned char in_ksn[10], const unsigned char in_bdk[16] )
	{
		m_method_dukpt_generate_ipek( ipek, in_ksn, in_bdk );
	}

	void dukpt_generate_sk( unsigned char session_key[16], const unsigned char ksn[10], const unsigned char ipek[16] )
	{
		m_method_dukpt_generate_sk( session_key, ksn, ipek );
	}

	void dukpt_generate_pin_encryption_key( unsigned char pin_encryption_key[16], const unsigned char session_key[16] )
	{
		m_method_dukpt_generate_pin_encryption_key( pin_encryption_key, session_key );
	}

	void dukpt_generate_mac_request_key( unsigned char mac_request_key[16], const unsigned char session_key[16] )
	{
		m_method_dukpt_generate_mac_request_key( mac_request_key, session_key );
	}

	void dukpt_generate_mac_response_key( unsigned char mac_response_key[16], const unsigned char session_key[16] )
	{
		m_method_dukpt_generate_mac_response_key( mac_response_key, session_key );
	}

	void dukpt_generate_data_encryption_request_key( unsigned char data_encryption_key[16], const unsigned char session_key[16] )
	{
		m_method_dukpt_generate_data_encryption_request_key( data_encryption_key, session_key );
	}

	void dukpt_generate_data_encryption_response_key( unsigned char data_encryption_key[16], const unsigned char session_key[16] )
	{
		m_method_dukpt_generate_data_encryption_response_key( data_encryption_key, session_key );
	}

	void dukpt_generate_mac( unsigned char mac8[8], const unsigned char mac_encryption_key[16], const unsigned char message[], unsigned int nMsg  )
	{
		m_method_dukpt_generate_mac( mac8, mac_encryption_key, message, nMsg );
	}

	void dukpt_get_mac4_from_mac8( unsigned char mac4[4], const unsigned char mac8[8] )
	{
		m_method_dukpt_get_mac4_from_mac8( mac4, mac8 );
	}

	void encrypt_ebc_des( unsigned char *p_out, unsigned int *pn_out, const unsigned char *p_in, const unsigned int n_in, const unsigned char key[8], unsigned char cPadding )
	{
		m_method_encrypt_ebc_des( p_out, pn_out, p_in, n_in, key, cPadding );
	}
	void decrypt_ebc_des( unsigned char *p_out, unsigned int *pn_out, const unsigned char *p_in, const unsigned int n_in, const unsigned char key[8] )
	{
		m_method_decrypt_ebc_des( p_out, pn_out, p_in, n_in, key );
	}

	void encrypt_cbc_des( unsigned char *p_out, unsigned int *pn_out, const unsigned char *p_in, const unsigned int n_in, const unsigned char key[8], const unsigned char iv[8], unsigned char cPadding )
	{
		m_method_encrypt_cbc_des( p_out, pn_out, p_in, n_in, key, iv, cPadding );
	}
	void decrypt_cbc_des( unsigned char *p_out, unsigned int *pn_out, const unsigned char *p_in, const unsigned int n_in, const unsigned char key[8], const unsigned char iv[8] )
	{
		m_method_decrypt_cbc_des( p_out, pn_out, p_in, n_in, key, iv );
	}

	void encrypt_ebc_tdes( unsigned char *p_out, unsigned int *pn_out, const unsigned char *p_in, const unsigned int n_in, const unsigned char key[16], unsigned char cPadding )
	{
		m_method_encrypt_ebc_tdes( p_out, pn_out, p_in, n_in, key, cPadding );
	}
	void decrypt_ebc_tdes( unsigned char *p_out, unsigned int *pn_out, const unsigned char *p_in, const unsigned int n_in, const unsigned char key[16] )
	{
		m_method_decrypt_ebc_tdes( p_out, pn_out, p_in, n_in, key );
	}

	void encrypt_cbc_tdes( unsigned char *p_out, unsigned int *pn_out, const unsigned char *p_in, const unsigned int n_in, const unsigned char key[16], const unsigned char iv[8], unsigned char cPadding )
	{
		m_method_encrypt_cbc_tdes( p_out, pn_out, p_in, n_in, key, iv, cPadding );
	}
	void decrypt_cbc_tdes( unsigned char *p_out, unsigned int *pn_out, const unsigned char *p_in, const unsigned int n_in, const unsigned char key[16], const unsigned char iv[8] )
	{
		m_method_decrypt_cbc_tdes( p_out, pn_out, p_in, n_in, key, iv );
	}

	void encrypt_cbc_aes( unsigned char *p_out, unsigned int *pn_out, const unsigned char *p_in, const unsigned int n_in, const unsigned char key[16], const unsigned char iv[16], unsigned char cPadding )
	{
		m_method_encrypt_cbc_aes( p_out, pn_out, p_in, n_in, key, iv, cPadding );
	}
	void decrypt_cbc_aes( unsigned char *p_out, unsigned int *pn_out, const unsigned char *p_in, const unsigned int n_in, const unsigned char key[16], const unsigned char iv[16] )
	{
		m_method_decrypt_cbc_aes( p_out, pn_out, p_in, n_in, key, iv );
	}

	void encode_base64( unsigned char *p_out, unsigned int *pn_out, const unsigned char *p_in, const unsigned int n_in )
	{
		m_method_encode_base64( p_out, pn_out, p_in, n_in );
	}

	void decode_base64( unsigned char *p_out, unsigned int *pn_out, const unsigned char *p_in, const unsigned int n_in )
	{
		m_method_decode_base64( p_out, pn_out, p_in, n_in );
	}

protected:

private:
	CWarp_idt_dukpt() : 
	   m_method_CurrentCryptoKey(NULL),
	   m_method_TDES_Encryption(NULL),
	   m_method_TDES_Decryption(NULL),
	   m_method_AES_Encryption(NULL),
	   m_method_AES_Decryption(NULL),
	   m_method_GetInitalKey(NULL),

	   m_method_dukpt_generate_ipek(NULL),
	   m_method_dukpt_generate_sk(NULL),
	   m_method_dukpt_generate_pin_encryption_key(NULL),
	   m_method_dukpt_generate_mac_request_key(NULL),
	   m_method_dukpt_generate_mac_response_key(NULL),
	   m_method_dukpt_generate_data_encryption_request_key(NULL),
	   m_method_dukpt_generate_data_encryption_response_key(NULL),
	   m_method_dukpt_generate_mac(NULL),
	   m_method_dukpt_get_mac4_from_mac8(NULL),

	   m_method_encrypt_ebc_des(NULL),
	   m_method_decrypt_ebc_des(NULL),
	   m_method_encrypt_cbc_des(NULL),
	   m_method_decrypt_cbc_des(NULL),

	   m_method_encrypt_ebc_tdes(NULL),
	   m_method_decrypt_ebc_tdes(NULL),
	   m_method_encrypt_cbc_tdes(NULL),
	   m_method_decrypt_cbc_tdes(NULL),

	   m_method_encrypt_cbc_aes(NULL),
	   m_method_decrypt_cbc_aes(NULL),
	   m_method_encode_base64(NULL),
	   m_method_decode_base64(NULL)
	{
		m_hMod = LoadLibrary( _T("yss_dukpt.dll") );

		m_method_CurrentCryptoKey = (type_method_CurrentCryptoKey)GetProcAddress( m_hMod, "CurrentCryptoKey" );
		m_method_TDES_Encryption = (type_method_TDES_Encryption)GetProcAddress( m_hMod, "TDES_Encryption" );
		m_method_TDES_Decryption = (type_method_TDES_Decryption)GetProcAddress( m_hMod, "TDES_Decryption" );
		m_method_AES_Encryption = (type_method_AES_Encryption)GetProcAddress( m_hMod, "AES_Encryption" );
		m_method_AES_Decryption = (type_method_AES_Decryption)GetProcAddress( m_hMod, "AES_Decryption" );
		m_method_GetInitalKey = (type_method_GetInitalKey)GetProcAddress( m_hMod, "GetInitalKey" );

		m_method_dukpt_generate_ipek = (type_method_dukpt_generate_ipek)GetProcAddress( m_hMod, "dukpt_generate_ipek" );
		m_method_dukpt_generate_sk = (type_method_dukpt_generate_sk)GetProcAddress( m_hMod, "dukpt_generate_sk" );
		m_method_dukpt_generate_pin_encryption_key = (type_method_dukpt_generate_pin_encryption_key)GetProcAddress( m_hMod, "dukpt_generate_pin_encryption_key" );
		m_method_dukpt_generate_mac_request_key = (type_method_dukpt_generate_mac_request_key)GetProcAddress( m_hMod, "dukpt_generate_mac_request_key" );
		m_method_dukpt_generate_mac_response_key = (type_method_dukpt_generate_mac_response_key)GetProcAddress( m_hMod, "dukpt_generate_mac_response_key" );
		m_method_dukpt_generate_data_encryption_request_key = (type_method_dukpt_generate_data_encryption_request_key)GetProcAddress( m_hMod, "dukpt_generate_data_encryption_request_key" );
		m_method_dukpt_generate_data_encryption_response_key = (type_method_dukpt_generate_data_encryption_response_key)GetProcAddress( m_hMod, "dukpt_generate_data_encryption_response_key" );

		m_method_dukpt_generate_mac = (type_method_dukpt_generate_mac)GetProcAddress( m_hMod, "dukpt_generate_mac" );
		m_method_dukpt_get_mac4_from_mac8 = (type_method_dukpt_get_mac4_from_mac8)GetProcAddress( m_hMod, "dukpt_get_mac4_from_mac8" );

	   m_method_encrypt_ebc_des = (type_method_encrypt_ebc_des)GetProcAddress( m_hMod, "encrypt_ebc_des" );
	   m_method_decrypt_ebc_des = (type_method_decrypt_ebc_des)GetProcAddress( m_hMod, "decrypt_ebc_des" );
	   m_method_encrypt_cbc_des = (type_method_encrypt_cbc_des)GetProcAddress( m_hMod, "encrypt_cbc_des" );
	   m_method_decrypt_cbc_des = (type_method_decrypt_cbc_des)GetProcAddress( m_hMod, "decrypt_cbc_des" );

	   m_method_encrypt_ebc_tdes = (type_method_encrypt_ebc_tdes)GetProcAddress( m_hMod, "encrypt_ebc_tdes" );
	   m_method_decrypt_ebc_tdes = (type_method_decrypt_ebc_tdes)GetProcAddress( m_hMod, "decrypt_ebc_tdes" );
	   m_method_encrypt_cbc_tdes = (type_method_encrypt_cbc_tdes)GetProcAddress( m_hMod, "encrypt_cbc_tdes" );
	   m_method_decrypt_cbc_tdes = (type_method_decrypt_cbc_tdes)GetProcAddress( m_hMod, "decrypt_cbc_tdes" );

	   m_method_encrypt_cbc_aes = (type_method_encrypt_cbc_aes)GetProcAddress( m_hMod, "encrypt_cbc_aes" );
	   m_method_decrypt_cbc_aes = (type_method_decrypt_cbc_aes)GetProcAddress( m_hMod, "decrypt_cbc_aes" );

	   m_method_encode_base64 = (type_method_encode_base64)GetProcAddress( m_hMod, "encode_base64" );
	   m_method_decode_base64 = (type_method_decode_base64)GetProcAddress( m_hMod, "decode_base64" );
	}


private:
	CWarp_idt_dukpt( const CWarp_idt_dukpt & );
	CWarp_idt_dukpt & operator=( const CWarp_idt_dukpt & );


protected:
private:
	HMODULE m_hMod;

	type_method_CurrentCryptoKey m_method_CurrentCryptoKey;
	type_method_TDES_Encryption m_method_TDES_Encryption;
	type_method_TDES_Decryption m_method_TDES_Decryption;
	type_method_AES_Encryption m_method_AES_Encryption;
	type_method_AES_Decryption m_method_AES_Decryption;
	type_method_GetInitalKey m_method_GetInitalKey;

	type_method_dukpt_generate_ipek m_method_dukpt_generate_ipek;
	type_method_dukpt_generate_sk m_method_dukpt_generate_sk;
	type_method_dukpt_generate_pin_encryption_key m_method_dukpt_generate_pin_encryption_key;
	type_method_dukpt_generate_mac_request_key m_method_dukpt_generate_mac_request_key;
	type_method_dukpt_generate_mac_response_key m_method_dukpt_generate_mac_response_key;
	type_method_dukpt_generate_data_encryption_request_key m_method_dukpt_generate_data_encryption_request_key;
	type_method_dukpt_generate_data_encryption_response_key m_method_dukpt_generate_data_encryption_response_key;

	type_method_dukpt_generate_mac m_method_dukpt_generate_mac;
	type_method_dukpt_get_mac4_from_mac8 m_method_dukpt_get_mac4_from_mac8;

	type_method_encrypt_ebc_des m_method_encrypt_ebc_des;
	type_method_decrypt_ebc_des m_method_decrypt_ebc_des;
	type_method_encrypt_cbc_des m_method_encrypt_cbc_des;
	type_method_decrypt_cbc_des m_method_decrypt_cbc_des;

	type_method_encrypt_ebc_tdes m_method_encrypt_ebc_tdes;
	type_method_decrypt_ebc_tdes m_method_decrypt_ebc_tdes;
	type_method_encrypt_cbc_tdes m_method_encrypt_cbc_tdes;
	type_method_decrypt_cbc_tdes m_method_decrypt_cbc_tdes;

	type_method_encrypt_cbc_aes m_method_encrypt_cbc_aes;
	type_method_decrypt_cbc_aes m_method_decrypt_cbc_aes;

	type_method_encode_base64 m_method_encode_base64;
	type_method_decode_base64 m_method_decode_base64;
};
