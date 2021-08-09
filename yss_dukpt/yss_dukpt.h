#ifndef	__20140915_YSS_DUKPT__
#define	__20140915_YSS_DUKPT__

extern "C"{

//! for IDtech reader.......
//
// Get the Current Crypto Key	
void __stdcall CurrentCryptoKey( const int KeyManagment, const BYTE *InitialKey, const BYTE *KSN, BYTE *CurrentKey);

// TDES Encryption
void __stdcall TDES_Encryption(BYTE *Data, const BYTE *Key, const BYTE *InitalVector, const int Length);

// TDES Decryption
void __stdcall TDES_Decryption(BYTE *Data, const BYTE *Key, const BYTE *InitalVector, const int Length);

// AES Encryption
void __stdcall AES_Encryption(BYTE *Data, const BYTE *Key, const BYTE *InitalVector, const int Length);

// AES Decryption
void __stdcall AES_Decryption(BYTE *Data, const BYTE *Key, const BYTE *InitalVector, const int Length);

// Get the Initial Key(IPEK)
void __stdcall GetInitalKey(const BYTE *KSN, const BYTE *BDK, BYTE *InitialKey);

//!
// my dukpt function prototype
//
void __stdcall dukpt_generate_ipek( unsigned char ipek[16], const unsigned char in_ksn[10], const unsigned char in_bdk[16] );
void __stdcall dukpt_generate_sk( unsigned char session_key[16], const unsigned char ksn[10], const unsigned char ipek[16] );
void __stdcall dukpt_generate_pin_encryption_key( unsigned char pin_encryption_key[16], const unsigned char session_key[16] );
void __stdcall dukpt_generate_mac_request_key( unsigned char mac_request_key[16], const unsigned char session_key[16] );
void __stdcall dukpt_generate_mac_response_key( unsigned char mac_response_key[16], const unsigned char session_key[16] );
void __stdcall dukpt_generate_data_encryption_request_key( unsigned char data_encryption_key[16], const unsigned char session_key[16] );
void __stdcall dukpt_generate_data_encryption_response_key( unsigned char data_encryption_key[16], const unsigned char session_key[16] );
void __stdcall dukpt_generate_mac( unsigned char mac8[8], const unsigned char mac_encryption_key[16], const unsigned char message[], unsigned int nMsg  );
void __stdcall dukpt_get_mac4_from_mac8( unsigned char mac4[4], const unsigned char mac8[8] );


void __stdcall encrypt_ebc_des( unsigned char *p_out, unsigned int *pn_out, const unsigned char *p_in, const unsigned int n_in, const unsigned char key[8], unsigned char cPadding );
void __stdcall decrypt_ebc_des( unsigned char *p_out, unsigned int *pn_out, const unsigned char *p_in, const unsigned int n_in, const unsigned char key[8] );

void __stdcall encrypt_cbc_des( unsigned char *p_out, unsigned int *pn_out, const unsigned char *p_in, const unsigned int n_in, const unsigned char key[8], const unsigned char iv[8], unsigned char cPadding );
void __stdcall decrypt_cbc_des( unsigned char *p_out, unsigned int *pn_out, const unsigned char *p_in, const unsigned int n_in, const unsigned char key[8], const unsigned char iv[8] );

void __stdcall encrypt_ebc_tdes( unsigned char *p_out, unsigned int *pn_out, const unsigned char *p_in, const unsigned int n_in, const unsigned char key[16], unsigned char cPadding );
void __stdcall decrypt_ebc_tdes( unsigned char *p_out, unsigned int *pn_out, const unsigned char *p_in, const unsigned int n_in, const unsigned char key[16] );

void __stdcall encrypt_cbc_tdes( unsigned char *p_out, unsigned int *pn_out, const unsigned char *p_in, const unsigned int n_in, const unsigned char key[16], const unsigned char iv[8], unsigned char cPadding );
void __stdcall decrypt_cbc_tdes( unsigned char *p_out, unsigned int *pn_out, const unsigned char *p_in, const unsigned int n_in, const unsigned char key[16], const unsigned char iv[8] );

void __stdcall encrypt_cbc_aes( unsigned char *p_out, unsigned int *pn_out, const unsigned char *p_in, const unsigned int n_in, const unsigned char key[16], const unsigned char iv[16], unsigned char cPadding );
void __stdcall decrypt_cbc_aes( unsigned char *p_out, unsigned int *pn_out, const unsigned char *p_in, const unsigned int n_in, const unsigned char key[16], const unsigned char iv[16] );

void __stdcall encode_base64( unsigned char *p_out, unsigned int *pn_out, const unsigned char *p_in, const unsigned int n_in );
void __stdcall decode_base64( unsigned char *p_out, unsigned int *pn_out, const unsigned char *p_in, const unsigned int n_in );

}

#endif		//__20140915_YSS_DUKPT__