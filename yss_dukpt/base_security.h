#pragma once
	#include <vector>

namespace _base_security{

	typedef	std::vector<unsigned char> typeBytes;
	
	void dukpt_padding_ksn( unsigned char out_ksn[10], const unsigned char in_ksn[], unsigned int nIn_ksn );
	void dukpt_generate_ipek( unsigned char ipek[16], const unsigned char in_ksn[10], const unsigned char in_bdk[16] );
	bool dukpt_increase_ksn( unsigned char ksn[10] );
	void dukpt_generate_sk( unsigned char session_key[16], const unsigned char ksn[10], const unsigned char ipek[16] );

	void dukpt_generate_pin_encryption_key( unsigned char pin_encryption_key[16], const unsigned char session_key[16] );
	void dukpt_generate_mac_request_key( unsigned char mac_request_key[16], const unsigned char session_key[16] );
	void dukpt_generate_mac_response_key( unsigned char mac_response_key[16], const unsigned char session_key[16] );
	void dukpt_generate_data_encryption_request_key( unsigned char data_encryption_key[16], const unsigned char session_key[16] );
	void dukpt_generate_data_encryption_response_key( unsigned char data_encryption_key[16], const unsigned char session_key[16] );
	void dukpt_generate_mac( unsigned char mac8[8], const unsigned char mac_encryption_key[16], const unsigned char message[], unsigned int nMsg  );
	void dukpt_get_mac4_from_mac8( unsigned char mac4[4], const unsigned char mac8[8] );

	void aes_cbc_descrypt( typeBytes &out, const typeBytes &in, const unsigned char key[16], const unsigned char iv[16] );
	void aes_cbc_encrypt_with_padding( typeBytes &out, const typeBytes & in, const unsigned char key[16], const unsigned char iv[16], unsigned char cPadding );

	void tdes_cbc_descrypt( typeBytes &out, const typeBytes &in, const unsigned char key[16], const unsigned char iv[8] );
	void tdes_cbc_encrypt_with_padding( typeBytes &out, const typeBytes & in, const unsigned char key[16], const unsigned char iv[8], unsigned char cPadding  );

	void tdes_ebc_descrypt( typeBytes &out, const typeBytes &in, const unsigned char key[16] );
	void tdes_ebc_encrypt_with_padding( typeBytes &out, const typeBytes & in, const unsigned char key[16], unsigned char cPadding  );

	void des_cbc_descrypt( typeBytes &out, const typeBytes &in, const unsigned char key[8], const unsigned char iv[8] );
	void des_cbc_encrypt_with_padding( typeBytes &out, const typeBytes & in, const unsigned char key[8], const unsigned char iv[8], unsigned char cPadding  );

	void des_ebc_descrypt( typeBytes &out, const typeBytes &in, const unsigned char key[8] );
	void des_ebc_encrypt_with_padding( typeBytes &out, const typeBytes & in, const unsigned char key[8], unsigned char cPadding  );

	void base64_encode( typeBytes &out, const typeBytes &in );
	void base64_decode( typeBytes &out, const typeBytes &in );

}