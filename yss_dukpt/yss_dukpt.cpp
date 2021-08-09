// yss_dukpt.cpp : DLL 응용 프로그램을 위해 내보낸 함수를 정의합니다.
//

#include "stdafx.h"
#include "yss_dukpt.h"
#include "base_security.h"

//! for IDtech reader.......
//
// Get the Current Crypto Key	
void __stdcall CurrentCryptoKey( const int KeyManagment, const BYTE *InitialKey, const BYTE *KSN, BYTE *CurrentKey)
{
	//void dukpt_generate_sk( unsigned char session_key[16], const unsigned char ksn[10], const unsigned char ipek[16] );
	//void dukpt_generate_data_encryption_request_key( unsigned char data_encryption_key[], const unsigned char session_key[] );

	//
	unsigned char session_key[16];
	_base_security::dukpt_generate_sk( session_key, KSN, InitialKey );
	_base_security::dukpt_generate_data_encryption_request_key( CurrentKey, session_key );
	//_base_security::dukpt_generate_data_encryption_response_key( CurrentKey, session_key );
	//_base_security::dukpt_generate_pin_encryption_key( CurrentKey, session_key );
	//_base_security::dukpt_generate_sk( CurrentKey, KSN, InitialKey );
}


// TDES Encryption
void __stdcall TDES_Encryption(BYTE *Data, const BYTE *Key, const BYTE *InitalVector, const int Length)
{
	if( Data != NULL && Key != NULL && InitalVector != NULL ){
		//void tdes_encrypt_with_padding( typeBytes &out, const typeBytes & in, const unsigned char key[16], unsigned char cPadding  )
		
		_base_security::typeBytes out,in;

		in.resize( Length );
		std::copy( &Data[0], &Data[Length], in.begin() );
		_base_security::tdes_cbc_encrypt_with_padding( out, in, Key, InitalVector, 0x00  );

		if( out.size() > Length ){
			std::copy( out.begin(), out.begin()+Length, Data );
		}
		else{
			std::copy( out.begin(), out.end(), Data );
		}
	}
}


// TDES Decryption
void __stdcall TDES_Decryption(BYTE *Data, const BYTE *Key, const BYTE *InitalVector, const int Length)
{
	if( Data != NULL && Key != NULL && InitalVector != NULL ){
		//void tdes_descrypt( typeBytes &out, const typeBytes &in, const unsigned char key[16] )

		_base_security::typeBytes out,in;

		in.resize( Length );
		std::copy( &Data[0], &Data[Length], in.begin() );
		_base_security::tdes_cbc_descrypt( out, in, Key, InitalVector );
	
		if( out.size() > Length ){
			std::copy( out.begin(), out.begin()+Length, Data );
		}
		else{
			std::copy( out.begin(), out.end(), Data );
		}
	}
}

// AES Encryption
void __stdcall AES_Encryption(BYTE *Data, const BYTE *Key, const BYTE *InitalVector, const int Length)
{
	if( Data != NULL && Key != NULL && InitalVector != NULL ){
		//void aes_encryt_with_padding( typeBytes &out, const typeBytes & in, const unsigned char key[16], const unsigned char iv[16], unsigned char cPadding );

		_base_security::typeBytes out,in;

		in.resize( Length );
		std::copy( Data, &Data[Length], in.begin() );

		_base_security::aes_cbc_encrypt_with_padding( out, in, Key, InitalVector, 0 );

		if( out.size() > Length ){
			std::copy( out.begin(), out.begin()+Length, Data );
		}
		else{
			std::copy( out.begin(), out.end(), Data );
		}
	}
}

// AES Decryption
void __stdcall AES_Decryption(BYTE *Data, const BYTE *Key, const BYTE *InitalVector, const int Length)
{
	if( Data != NULL && Key != NULL && InitalVector != NULL ){
		//void aes_descrypt( typeBytes &out, const typeBytes &in, const unsigned char key[16], const unsigned char iv[16] );

		_base_security::typeBytes out,in;

		in.resize( Length );
		std::copy( Data, &Data[Length], in.begin() );

		_base_security::aes_cbc_descrypt( out, in, Key, InitalVector );
		if( out.size() > Length ){
			std::copy( out.begin(), out.begin()+Length, Data );
		}
		else{
			std::copy( out.begin(), out.end(), Data );
		}
	}
}

// Get the Initial Key
void __stdcall GetInitalKey(const BYTE *KSN, const BYTE *BDK, BYTE *InitialKey)
{
	if( KSN !=NULL  && BDK !=NULL  && InitialKey !=NULL  ){
		//_base_security::dukpt_generate_ipek( unsigned char ipek[], const unsigned char in_ksn[], const unsigned char in_bdk[] );
		_base_security::dukpt_generate_ipek( InitialKey, KSN, BDK );
	}
}

//!
// my dukpt function body
//
void __stdcall dukpt_generate_ipek( unsigned char ipek[16], const unsigned char in_ksn[10], const unsigned char in_bdk[16] )
{
	_base_security::dukpt_generate_ipek( ipek, in_ksn, in_bdk );
}

void __stdcall dukpt_generate_sk( unsigned char session_key[16], const unsigned char ksn[10], const unsigned char ipek[16] )
{
	_base_security::dukpt_generate_sk( session_key, ksn, ipek );
}

void __stdcall dukpt_generate_pin_encryption_key( unsigned char pin_encryption_key[16], const unsigned char session_key[16] )
{
	_base_security::dukpt_generate_pin_encryption_key( pin_encryption_key, session_key );
}

void __stdcall dukpt_generate_mac_request_key( unsigned char mac_request_key[16], const unsigned char session_key[16] )
{
	_base_security::dukpt_generate_mac_request_key( mac_request_key, session_key );
}

void __stdcall dukpt_generate_mac_response_key( unsigned char mac_response_key[16], const unsigned char session_key[16] )
{
	_base_security::dukpt_generate_mac_response_key( mac_response_key, session_key );
}

void __stdcall dukpt_generate_data_encryption_request_key( unsigned char data_encryption_key[16], const unsigned char session_key[16] )
{
	_base_security::dukpt_generate_data_encryption_request_key( data_encryption_key, session_key );
}

void __stdcall dukpt_generate_data_encryption_response_key( unsigned char data_encryption_key[16], const unsigned char session_key[16] )
{
	_base_security::dukpt_generate_data_encryption_response_key( data_encryption_key, session_key );
}

void __stdcall dukpt_generate_mac( unsigned char mac8[8], const unsigned char mac_encryption_key[16], const unsigned char message[], unsigned int nMsg  )
{
	_base_security::dukpt_generate_mac( mac8, mac_encryption_key, message, nMsg );
}

void __stdcall dukpt_get_mac4_from_mac8( unsigned char mac4[4], const unsigned char mac8[8] )
{
	_base_security::dukpt_get_mac4_from_mac8( mac4, mac8 );
}


//
void __stdcall encrypt_ebc_des( unsigned char *p_out, unsigned int *pn_out, const unsigned char *p_in, const unsigned int n_in, const unsigned char key[8], unsigned char cPadding )
{
	if( p_in != NULL && n_in > 0 ){
		_base_security::typeBytes out,in;

		in.resize( n_in );
		std::copy( &p_in[0], &p_in[n_in], in.begin() );
		_base_security::des_ebc_encrypt_with_padding( out, in, key, cPadding );

		if( pn_out )
			*pn_out = static_cast<unsigned int>( out.size() );
		if( p_out )
			std::copy( out.begin(), out.end(), &p_out[0] );
	}
}

void __stdcall decrypt_ebc_des( unsigned char *p_out, unsigned int *pn_out, const unsigned char *p_in, const unsigned int n_in, const unsigned char key[8] )
{
	if( p_in != NULL && n_in > 0 ){
		_base_security::typeBytes out,in;

		in.resize( n_in );
		std::copy( &p_in[0], &p_in[n_in], in.begin() );
		_base_security::des_ebc_descrypt( out, in, key );

		if( pn_out )
			*pn_out = static_cast<unsigned int>( out.size() );
		if( p_out )
			std::copy( out.begin(), out.end(), &p_out[0] );
	}
}


void __stdcall encrypt_cbc_des( unsigned char *p_out, unsigned int *pn_out, const unsigned char *p_in, const unsigned int n_in, const unsigned char key[8], const unsigned char iv[8], unsigned char cPadding )
{
	if( p_in != NULL && n_in > 0 ){
		_base_security::typeBytes out,in;

		in.resize( n_in );
		std::copy( &p_in[0], &p_in[n_in], in.begin() );
		_base_security::des_cbc_encrypt_with_padding( out, in, key, iv, cPadding );

		if( pn_out )
			*pn_out = static_cast<unsigned int>( out.size() );
		if( p_out )
			std::copy( out.begin(), out.end(), &p_out[0] );
	}
}

void __stdcall decrypt_cbc_des( unsigned char *p_out, unsigned int *pn_out, const unsigned char *p_in, const unsigned int n_in, const unsigned char key[8], const unsigned char iv[8] )
{
	if( p_in != NULL && n_in > 0 ){
		_base_security::typeBytes out,in;

		in.resize( n_in );
		std::copy( &p_in[0], &p_in[n_in], in.begin() );
		_base_security::des_cbc_descrypt( out, in, key, iv );

		if( pn_out )
			*pn_out = static_cast<unsigned int>( out.size() );
		if( p_out )
			std::copy( out.begin(), out.end(), &p_out[0] );
	}
}


void __stdcall encrypt_ebc_tdes( unsigned char *p_out, unsigned int *pn_out, const unsigned char *p_in, const unsigned int n_in, const unsigned char key[16], unsigned char cPadding )
{
	if( p_in != NULL && n_in > 0 ){
		_base_security::typeBytes out,in;

		in.resize( n_in );
		std::copy( &p_in[0], &p_in[n_in], in.begin() );
		_base_security::tdes_ebc_encrypt_with_padding( out, in, key, cPadding );

		if( pn_out )
			*pn_out = static_cast<unsigned int>( out.size() );
		if( p_out )
			std::copy( out.begin(), out.end(), &p_out[0] );
	}
}

void __stdcall decrypt_ebc_tdes( unsigned char *p_out, unsigned int *pn_out, const unsigned char *p_in, const unsigned int n_in, const unsigned char key[16] )
{
	if( p_in != NULL && n_in > 0 ){
		_base_security::typeBytes out,in;

		in.resize( n_in );
		std::copy( &p_in[0], &p_in[n_in], in.begin() );
		_base_security::tdes_ebc_descrypt( out, in, key );

		if( pn_out )
			*pn_out = static_cast<unsigned int>( out.size() );
		if( p_out )
			std::copy( out.begin(), out.end(), &p_out[0] );
	}
}


void __stdcall encrypt_cbc_tdes( unsigned char *p_out, unsigned int *pn_out, const unsigned char *p_in, const unsigned int n_in, const unsigned char key[16], const unsigned char iv[8], unsigned char cPadding )
{
	if( p_in != NULL && n_in > 0 ){
		_base_security::typeBytes out,in;

		in.resize( n_in );
		std::copy( &p_in[0], &p_in[n_in], in.begin() );
		_base_security::tdes_cbc_encrypt_with_padding( out, in, key, iv, cPadding );

		if( pn_out )
			*pn_out = static_cast<unsigned int>( out.size() );
		if( p_out )
			std::copy( out.begin(), out.end(), &p_out[0] );
	}
}

void __stdcall decrypt_cbc_tdes( unsigned char *p_out, unsigned int *pn_out, const unsigned char *p_in, const unsigned int n_in, const unsigned char key[16], const unsigned char iv[8] )
{
	if( p_in != NULL && n_in > 0 ){
		_base_security::typeBytes out,in;

		in.resize( n_in );
		std::copy( &p_in[0], &p_in[n_in], in.begin() );
		_base_security::tdes_cbc_descrypt( out, in, key, iv );

		if( pn_out )
			*pn_out = static_cast<unsigned int>( out.size() );
		if( p_out )
			std::copy( out.begin(), out.end(), &p_out[0] );
	}
}


void __stdcall encrypt_cbc_aes( unsigned char *p_out, unsigned int *pn_out, const unsigned char *p_in, const unsigned int n_in, const unsigned char key[16], const unsigned char iv[16], unsigned char cPadding )
{
	if( p_in != NULL && n_in > 0 ){
		_base_security::typeBytes out,in;

		in.resize( n_in );
		std::copy( &p_in[0], &p_in[n_in], in.begin() );
		_base_security::aes_cbc_encrypt_with_padding( out, in, key, iv, cPadding );

		if( pn_out )
			*pn_out = static_cast<unsigned int>( out.size() );
		if( p_out )
			std::copy( out.begin(), out.end(), &p_out[0] );
	}
}

void __stdcall decrypt_cbc_aes( unsigned char *p_out, unsigned int *pn_out, const unsigned char *p_in, const unsigned int n_in, const unsigned char key[16], const unsigned char iv[16] )
{
	if( p_in != NULL && n_in > 0 ){
		_base_security::typeBytes out,in;

		in.resize( n_in );
		std::copy( &p_in[0], &p_in[n_in], in.begin() );
		_base_security::aes_cbc_descrypt( out, in, key, iv );

		if( pn_out )
			*pn_out = static_cast<unsigned int>( out.size() );
		if( p_out )
			std::copy( out.begin(), out.end(), &p_out[0] );
	}
}

void __stdcall encode_base64( unsigned char *p_out, unsigned int *pn_out, const unsigned char *p_in, const unsigned int n_in )
{
	if( n_in == 0 )
		return;
	if( pn_out == NULL )
		return;
	if( p_out == NULL ){
		unsigned int len( n_in * 8 );

		if( len % 6 == 0 ){
			(*pn_out) = len / 6;
		}
		else{
			(*pn_out) = len / 6 + 1;
		}
		return;
	}
	//
	_base_security::typeBytes out;
	_base_security::typeBytes in( n_in );

	std::copy( &p_in[0], &p_in[n_in], in.begin() );
	_base_security::base64_encode( out, in );
	
	std::copy( out.begin(), out.end(), &p_out[0] );
	*pn_out = out.size();
}

void __stdcall decode_base64( unsigned char *p_out, unsigned int *pn_out, const unsigned char *p_in, const unsigned int n_in )
{
	if( n_in == 0 )
		return;
	if( pn_out == NULL )
		return;
	if( p_out == NULL ){
		unsigned int len( n_in * 6 );

		if( len % 8 == 0 ){
			*pn_out = len / 8;
		}
		else{
			*pn_out = len / 8;		(*pn_out)--;
		}
		return;
	}
	//
	_base_security::typeBytes out;
	_base_security::typeBytes in( n_in );

	std::copy( &p_in[0], &p_in[n_in], in.begin() );
	_base_security::base64_decode( out, in );

	std::copy( out.begin(), out.end(), &p_out[0] );
	*pn_out = out.size();
}
