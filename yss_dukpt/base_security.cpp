

#include "stdafx.h"
#include "base_security.h"
#include <algorithm>
#include <iterator>

#include "des.h"
#include "des3.h"
#include "aes.h"
#include "base64.h"
#include "cipher_mode_ecb.h"
#include "cipher_mode_cbc.h"

namespace _base_security{

	using namespace std;
	
	//! local function prototype.
	static void _en_ebc_des( const typeBytes & key, const typeBytes & in, typeBytes & out, unsigned char cPadding );
	static void _de_ebc_des( const typeBytes & key, const typeBytes & in, typeBytes & out );
	static void _en_cbc_des( const typeBytes & key, typeBytes & iv, const typeBytes & in, typeBytes & out, unsigned char cPadding );
	static void _de_cbc_des( const typeBytes & key, typeBytes & iv, const typeBytes & in, typeBytes & out );

	static void _en_ebc_tdes( const typeBytes & key, const typeBytes & in, typeBytes & out, unsigned char cPadding );
	static void _de_ebc_tdes( const typeBytes & key, const typeBytes & in, typeBytes & out );
	static void _en_cbc_tdes( const typeBytes & key, typeBytes & iv, const typeBytes & in, typeBytes & out, unsigned char cPadding );
	static void _de_cbc_tdes( const typeBytes & key, typeBytes & iv, const typeBytes & in, typeBytes & out );

	static void _en_ebc_aes( const typeBytes & key, const typeBytes & in, typeBytes & out, unsigned char cPadding );
	static void _de_ebc_aes( const typeBytes & key, const typeBytes & in, typeBytes & out );
	static void _en_cbc_aes( const typeBytes & key, typeBytes & iv, const typeBytes & in, typeBytes & out, unsigned char cPadding );
	static void _de_cbc_aes( const typeBytes & key, typeBytes & iv, const typeBytes & in, typeBytes & out );
	
	static void _dukpt_blackbox( unsigned char new_session_key[], const unsigned char cur_session_key[], const unsigned char ksn[] ); 

	//! local function body
	void _dukpt_blackbox( unsigned char new_session_key[], const unsigned char cur_session_key[], const unsigned char ksn[] )
	{
		unsigned char ksn_prime[10];
		unsigned char ksn_prime_mask[] =
		{
			0,0,0xff,0xff,0xff,0xff,0xff,0xe0,0,0
		};

		//generate ksn prime
		for( auto i=0; i<10; i++ ){	ksn_prime[i] = ksn[i] & ksn_prime_mask[i];	}//end for
		//AddInfoList( ksn_prime, 10, false, _T(" - KSN prime : ") );

		//generate key modification
		unsigned char ksn_mod[8];

		for( auto i=0; i<8; i++ ){	ksn_mod[i] = ksn_prime[i+2];	}

		unsigned char ksn_count_mask[] = 
		{
			0,0,0,0,0,0,0,0x1F,0xFF,0xFF	//21bits mask
		};

		unsigned char ksn_count[8];
		for( auto i=0; i<8; i++ ){
			ksn_count[i] = ksn[i+2] & ksn_count_mask[i+2];
		}

		for( auto i=0; i<8; i++ ){
			ksn_mod[i] |= ksn_count[i];
		}
		//AddInfoList( ksn_mod, 8, false, _T(" - KSN modification : ") );

		///////////////////////////////////////////////////////////////////////
		//generates session key LSB 8bytes(right of session key )
		unsigned char left_key[8];
		for( auto i=0; i<8; i++ ){	left_key[i] = cur_session_key[i];	}
		//AddInfoList( left_key, 8, false, _T(" - left_key : ") );

		unsigned char right_key[8];
		for( auto i=0; i<8; i++ ){	right_key[i] = cur_session_key[i+8];	}
		//AddInfoList( right_key, 8, false, _T(" - right_key : ") );

		unsigned char msg[8];
		for( auto i=0; i<8; i++ ){	msg[i] = right_key[i] ^ ksn_mod[i];	}
		//AddInfoList( msg, 8, false, _T(" - msg : ") );
		
		unsigned char out[8];
		//des_context ctx;
		//des_set_key( &ctx, left_key );
		//des_encrypt( &ctx, msg, out );
		DesContext context;
		desCipherAlgo.init( &context, left_key, sizeof(left_key) );
		ecbEncrypt( &desCipherAlgo, &context, msg, out, 8 );

		unsigned char sk_lsb8[8];//CR2
		for( auto i=0; i<8; i++ ){	sk_lsb8[i] = out[i] ^ right_key[i];	}
		//AddInfoList( sk_lsb8, 8, false, _T(" - session key LSB 8bytes : ") );

		//generates session key MSB 8bytes(left of session key )
		unsigned char sk_mod[16];
		unsigned char sk_p[] =
		{
			0xc0,0xc0,0xc0,0xc0,0,0,0,0,
			0xc0,0xc0,0xc0,0xc0,0,0,0,0
		};
		for( auto i=0; i<16; i++ ){	sk_mod[i] = cur_session_key[i] ^ sk_p[i];	}
		//AddInfoList( sk_mod, 16, false, _T(" - Session key modification : ") );

		for( auto i=0; i<8; i++ ){	left_key[i] = sk_mod[i];	}
		//AddInfoList( left_key, 8, false, _T(" - left_key(mod) : ") );

		for( auto i=0; i<8; i++ ){	right_key[i] = sk_mod[i+8];	}
		//AddInfoList( right_key, 8, false, _T(" - right_key(mod) : ") );

		for( auto i=0; i<8; i++ ){	msg[i] = right_key[i] ^ ksn_mod[i];	}
		//AddInfoList( msg, 8, false, _T(" - msg(mod) : ") );

		//des_set_key( &ctx, left_key );
		//des_encrypt( &ctx, msg, out );
		desCipherAlgo.init( &context, left_key, sizeof(left_key) );
		ecbEncrypt( &desCipherAlgo, &context, msg, out, 8 );

		//AddInfoList( out, 8, false, _T(" - des out(mod) : ") );

		unsigned char sk_msb8[8];//CR1
		for( auto i=0; i<8; i++ ){	sk_msb8[i] = out[i] ^ right_key[i];	}
		//AddInfoList( sk_msb8, 8, false, _T(" - session key MSB 8bytes : ") );

		//generates new session key.
		for( auto i=0; i<8; i++ ){
			new_session_key[i] = sk_msb8[i];
			new_session_key[i+8] = sk_lsb8[i];
		}
		//AddInfoList( new_session_key, 16, false, _T(" - new session key  : ") );
	}

	void _en_ebc_des( const typeBytes & key, const typeBytes & in, typeBytes & out, unsigned char cPadding )
	{
		if( key.size() != 8 ){
			out.resize(0);
			return;
		}

		typeBytes vInMsg;
		int nCnt = in.size() / 8;

		if( in.size() % 8 != 0 ){		nCnt++;		}
		vInMsg.resize( nCnt*8, cPadding );
		out.resize( vInMsg.size(), 0 );

		copy( in.begin(), in.end(), vInMsg.begin() );
		//
		DesContext context;
		desCipherAlgo.init( &context, &key[0], key.size() );
		for( int i=0; i<nCnt; i++ ){		ecbEncrypt( &desCipherAlgo, &context, &vInMsg[i*8], &out[i*8], 8 );			}//end for
	}

	void _de_ebc_des( const typeBytes & key, const typeBytes & in, typeBytes & out )
	{
		if( key.size() != 8 ){
			out.resize(0);
			return;
		}
		
		if( in.size() % 8 != 0 ){
			out.resize(0);
			return;
		}

		int nCnt = in.size()/8;

		DesContext context;
		desCipherAlgo.init( &context, &key[0], key.size() );
		out.resize( in.size(), 0 );
		for( int i=0; i<nCnt; i++ ){		ecbDecrypt( &desCipherAlgo, &context, &in[i*8], &out[i*8], 8  );			}//end for
	}

	void _en_cbc_des( const typeBytes & key, typeBytes & iv, const typeBytes & in, typeBytes & out, unsigned char cPadding )
	{
		if( key.size() != 8 ){
			out.resize(0);
			return;
		}

		typeBytes vInMsg;
		int nCnt = in.size() / 8;

		if( in.size() % 8 != 0 ){		nCnt++;		}
		vInMsg.resize( nCnt*8, cPadding );
		out.resize( vInMsg.size(), 0 );

		copy( in.begin(), in.end(), vInMsg.begin() );
		//
		DesContext context;
		desCipherAlgo.init( &context, &key[0], key.size() );
		for( int i=0; i<nCnt; i++ ){		cbcEncrypt( &desCipherAlgo, &context, &iv[0], &vInMsg[i*8], &out[i*8], 8 );			}//end for
	}

	void _de_cbc_des( const typeBytes & key, typeBytes & iv, const typeBytes & in, typeBytes & out )
	{
		if( key.size() != 8 ){
			out.resize(0);
			return;
		}
		
		if( in.size() % 8 != 0 ){
			out.resize(0);
			return;
		}

		int nCnt = in.size()/8;

		DesContext context;
		desCipherAlgo.init( &context, &key[0], key.size() );
		out.resize( in.size(), 0 );
		for( int i=0; i<nCnt; i++ ){		cbcDecrypt( &desCipherAlgo, &context, &iv[0], &in[i*8], &out[i*8], 8  );			}//end for
	}
	//

	void _en_ebc_tdes( const typeBytes & key, const typeBytes & in, typeBytes & out, unsigned char cPadding )
	{
		if( key.size() != 16 ){
			out.resize(0);
			return;
		}

		typeBytes vInMsg;
		int nCnt = in.size() / 8;

		if( in.size() % 8 != 0 ){		nCnt++;		}
		vInMsg.resize( nCnt*8, cPadding );
		out.resize( vInMsg.size(), 0 );

		copy( in.begin(), in.end(), vInMsg.begin() );
		//
		Des3Context context;
		des3CipherAlgo.init( &context, &key[0], key.size() );
		for( int i=0; i<nCnt; i++ ){		ecbEncrypt( &des3CipherAlgo, &context, &vInMsg[i*8], &out[i*8], 8 );			}//end for
	}

	void _de_ebc_tdes( const typeBytes & key, const typeBytes & in, typeBytes & out )
	{
		if( key.size() != 16 ){
			out.resize(0);
			return;
		}
		
		if( in.size() % 8 != 0 ){
			out.resize(0);
			return;
		}

		int nCnt = in.size()/8;

		Des3Context context;
		des3CipherAlgo.init( &context, &key[0], key.size() );
		out.resize( in.size(), 0 );
		for( int i=0; i<nCnt; i++ ){		ecbDecrypt( &des3CipherAlgo, &context, &in[i*8], &out[i*8], 8  );			}//end for
	}

	void _en_cbc_tdes( const typeBytes & key, typeBytes & iv, const typeBytes & in, typeBytes & out, unsigned char cPadding )
	{
		if( key.size() != 16 ){
			out.resize(0);
			return;
		}

		typeBytes vInMsg;
		int nCnt = in.size() / 8;

		if( in.size() % 8 != 0 ){		nCnt++;		}
		vInMsg.resize( nCnt*8, cPadding );
		out.resize( vInMsg.size(), 0 );

		copy( in.begin(), in.end(), vInMsg.begin() );
		//
		Des3Context context;
		des3CipherAlgo.init( &context, &key[0], key.size() );
		for( int i=0; i<nCnt; i++ ){		cbcEncrypt( &des3CipherAlgo, &context, &iv[0], &vInMsg[i*8], &out[i*8], 8 );			}//end for
	}

	void _de_cbc_tdes( const typeBytes & key, typeBytes & iv, const typeBytes & in, typeBytes & out )
	{
		if( key.size() != 16 ){
			out.resize(0);
			return;
		}
		
		if( in.size() % 8 != 0 ){
			out.resize(0);
			return;
		}

		int nCnt = in.size()/8;

		Des3Context context;
		des3CipherAlgo.init( &context, &key[0], key.size() );
		out.resize( in.size(), 0 );
		for( int i=0; i<nCnt; i++ ){		cbcDecrypt( &des3CipherAlgo, &context, &iv[0], &in[i*8], &out[i*8], 8  );			}//end for
	}
	//

	void _en_ebc_aes( const typeBytes & key, const typeBytes & in, typeBytes & out, unsigned char cPadding )
	{
		if( key.size() != 16 ){
			out.resize(0);
			return;
		}

		typeBytes vInMsg;
		int nCnt = in.size() / 16;

		if( in.size() % 16 != 0 ){		nCnt++;		}
		vInMsg.resize( nCnt*16, cPadding );
		out.resize( vInMsg.size(), 0 );

		copy( in.begin(), in.end(), vInMsg.begin() );
		//
		AesContext context;
		aesCipherAlgo.init( &context, &key[0], key.size() );
		for( int i=0; i<nCnt; i++ ){		ecbEncrypt( &aesCipherAlgo, &context, &vInMsg[i*16], &out[i*16], 16 );			}//end for
	}

	void _de_ebc_aes( const typeBytes & key, const typeBytes & in, typeBytes & out )
	{
		if( key.size() != 16 ){
			out.resize(0);
			return;
		}
		
		if( in.size() % 16 != 0 ){
			out.resize(0);
			return;
		}

		int nCnt = in.size()/16;

		AesContext context;
		aesCipherAlgo.init( &context, &key[0], key.size() );
		out.resize( in.size(), 0 );
		for( int i=0; i<nCnt; i++ ){		ecbDecrypt( &aesCipherAlgo, &context, &in[i*16], &out[i*16], 16  );			}//end for
	}

	void _en_cbc_aes( const typeBytes & key, typeBytes & iv, const typeBytes & in, typeBytes & out, unsigned char cPadding )
	{
		if( key.size() != 16 ){
			out.resize(0);
			return;
		}

		typeBytes vInMsg;
		int nCnt = in.size() / 16;

		if( in.size() % 16 != 0 ){		nCnt++;		}
		vInMsg.resize( nCnt*16, cPadding );
		out.resize( vInMsg.size(), 0 );

		copy( in.begin(), in.end(), vInMsg.begin() );
		//
		AesContext context;
		aesCipherAlgo.init( &context, &key[0], key.size() );
		for( int i=0; i<nCnt; i++ ){		cbcEncrypt( &aesCipherAlgo, &context, &iv[0], &vInMsg[i*16], &out[i*16], 16 );			}//end for
	}

	void _de_cbc_aes( const typeBytes & key, typeBytes & iv, const typeBytes & in, typeBytes & out )
	{
		if( key.size() != 16 ){
			out.resize(0);
			return;
		}
		
		if( in.size() % 16 != 0 ){
			out.resize(0);
			return;
		}

		int nCnt = in.size()/16;

		AesContext context;
		aesCipherAlgo.init( &context, &key[0], key.size() );
		out.resize( in.size(), 0 );
		for( int i=0; i<nCnt; i++ ){		cbcDecrypt( &aesCipherAlgo, &context, &iv[0], &in[i*16], &out[i*16], 16  );			}//end for
	}


	//! exported function body
	void dukpt_padding_ksn( unsigned char out_ksn[10], const unsigned char in_ksn[], unsigned int nIn_ksn )
	{
		unsigned int nPad = 10-nIn_ksn;
		auto i = 0;

		for( ; i<nPad; i++ ){
			out_ksn[i] = 0xFF;
		}//end for

		for( auto j=0; i<10; i++, j++ ){
			out_ksn[i] = in_ksn[j];
		}
	}

	void dukpt_generate_ipek( unsigned char ipek[16], const unsigned char in_ksn[10], const unsigned char in_bdk[16]  )
	{
		// input : m_bdk[16], m_ksn[10]
		// output : m_ipek[16]
		// immedidate : m_ksn_ipek[8]
		unsigned char i;
		unsigned char ksn[10];
		unsigned char bdk[16];

		copy( in_ksn, &in_ksn[10], ksn );
		copy( in_bdk, &in_bdk[16], bdk );

		//AddInfoList( bdk, 16, false, _T(" - BDK : ") );
		//AddInfoList( ksn, 10, false, _T(" - KSN : ") );

	//reset 21 bits.
		ksn[7] &= 0xE0;		ksn[8] &= 0x00;		ksn[9] &= 0x00;

		//right shift 16 bits.
		unsigned char ksn_ipek[8];
		copy( &ksn[0], &ksn[8], ksn_ipek );
			//AddInfoList( ksn_ipek, 8, false, _T(" - ksn_ipek : ") );
	//
		unsigned char ipek_left[8], ipek_right[8];

		//des3_context ctx3;
		//des3_set_3keys( &ctx3, &bdk[0], &bdk[8], &bdk[0] );
		//des3_encrypt( &ctx3, ksn_ipek, ipek_left );
		Des3Context context;
		des3CipherAlgo.init( &context, &bdk[0], 16 );
		ecbEncrypt( &des3CipherAlgo, &context, ksn_ipek, ipek_left, 8 );	

		unsigned char bdk_mod[16];
		unsigned char bdk_p[] =
		{
			0xc0,0xc0,0xc0,0xc0,0,0,0,0,
			0xc0,0xc0,0xc0,0xc0,0,0,0,0,
		};

		for( i=0; i<16; i++ ){	bdk_mod[i] = bdk[i] ^ bdk_p[i];	}

		//des3_set_3keys( &ctx3, &bdk_mod[0], &bdk_mod[8], &bdk_mod[0] );
		//des3_encrypt( &ctx3, ksn_ipek, ipek_right );
		des3CipherAlgo.init( &context, &bdk_mod[0], 16 );
		ecbEncrypt( &des3CipherAlgo, &context, ksn_ipek, ipek_right, 8 );	

		//concate left & right
		copy( ipek_left, &ipek_left[8], ipek );
		copy( ipek_right, &ipek_right[8], &ipek[8] );
	}
	
	bool dukpt_increase_ksn( unsigned char ksn[10] )
	{
		bool bResult = true;
		// increase KSN
		unsigned char count[10];
		unsigned char count_p[] = 
		{
			0x00, 0x00, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x1F, 0xFF, 0xFF
		};

		// remove counter part.
		for( auto i=0; i<10; i++ ){
			count[i] = ksn[i] & count_p[i];
			ksn[i] = ksn[i] & (~count_p[i]);
		}//end for

		// increase counter.
		if( count[9] < 0xFF )
			count[9]++;
		else{
			if( count[8] < 0xFF ){
				count[9] = 0;
				count[8]++;
			}
			else{
				if( count[7] < 0x1F ){
					count[9] = count[8] = 0;
					count[7]++;
				}
				else{
					count[9] = count[8] = count[7] = 0;
					bResult = false;
				}
			}
		}

		for( auto i=0; i<10; i++ ){
			ksn[i] = ksn[i] | count[i];
		}
		//
		return bResult;
	}

	void dukpt_generate_sk( unsigned char session_key[16], const unsigned char ksn[10], const unsigned char ipek[16] )
	{
		unsigned char en_cnt[3] = 
		{ 
			ksn[7] & 0x1f,
			ksn[8],
			ksn[9]
		};

		unsigned char ksn_base[10];
		copy( ksn, &ksn[10], ksn_base );
		ksn_base[7] &= 0xe0;
		ksn_base[8] = 0;
		ksn_base[9] = 0;

		unsigned char ksn_in[10];
		unsigned char key_in[16];
		unsigned char key_out[16];
	
		copy( ipek, &ipek[16], key_in );
		unsigned char cMask(0x10);

		for( int i=0; i<5; i++ ){
			if( en_cnt[0] & cMask ){
				copy( ksn_base, &ksn_base[10], ksn_in );
				ksn_in[7] |= cMask;
				ksn_in[8] |= 0;
				ksn_in[9] |= 0;

				_dukpt_blackbox( key_out, key_in, ksn_in );
				copy( key_out, &key_out[16], key_in );
				copy( ksn_in, &ksn_in[10], ksn_base );
			}
			cMask >>= 1;
		}//end for

		cMask = 0x80;
		for( int i=0; i<8; i++ ){
			if( en_cnt[1] & cMask ){
				copy( ksn_base, &ksn_base[10], ksn_in );
				ksn_in[7] |= 0;
				ksn_in[8] |= cMask;
				ksn_in[9] |= 0;

				_dukpt_blackbox( key_out, key_in, ksn_in );
				copy( key_out, &key_out[16], key_in );
				copy( ksn_in, &ksn_in[10], ksn_base );
			}
			cMask >>= 1;
		}//end for

		cMask = 0x80;
		for( int i=0; i<8; i++ ){
			if( en_cnt[2] & cMask ){
				copy( ksn_base, &ksn_base[10], ksn_in );
				ksn_in[7] |= 0;
				ksn_in[8] |= 0;
				ksn_in[9] |= cMask;

				_dukpt_blackbox( key_out, key_in, ksn_in );
				copy( key_out, &key_out[16], key_in );
				copy( ksn_in, &ksn_in[10], ksn_base );
			}
			cMask >>= 1;
		}//end for

		copy( key_out, &key_out[16], session_key );

	}

	void dukpt_generate_pin_encryption_key( unsigned char pin_encryption_key[16], const unsigned char session_key[16] )
	{
		unsigned char pin_encryption_key_p[]=
		{
			0,0,0,0,0,0,0,0xff,
			0,0,0,0,0,0,0,0xff
		};

		for( auto i=0; i<16; i++ ){
			pin_encryption_key[i] = session_key[i] ^ pin_encryption_key_p[i];
		}
	}

	void dukpt_generate_mac_request_key( unsigned char mac_request_key[16], const unsigned char session_key[16] )
	{
		unsigned char mac_request_key_p[]=
		{
			00,00,00,00,00,00,0xff,00,
			00,00,00,00,00,00,0xff,00
		};

		for( auto i=0; i<16; i++ ){
			mac_request_key[i] = session_key[i] ^ mac_request_key_p[i];
		}
	}

	void dukpt_generate_mac_response_key( unsigned char mac_response_key[16], const unsigned char session_key[16] )
	{
		unsigned char mac_response_key_p[]=
		{
			00,00,00,00,0xff,00,00,00,
			00,00,00,00,0xff,00,00,00
		};

		for( auto i=0; i<16; i++ ){
			mac_response_key[i] = session_key[i] ^ mac_response_key_p[i];
		}
	}

	void dukpt_generate_data_encryption_request_key( unsigned char data_encryption_key[16], const unsigned char session_key[16] )
	{
		unsigned char data_encryption_key_p[]=
		{
			00,00,00,00,00,0xff,00,00,
			00,00,00,00,00,0xff,00,00
		};

		unsigned char data_encryption_key_var[16];

		for( auto i=0; i<16; i++ ){
			data_encryption_key_var[i] = session_key[i] ^ data_encryption_key_p[i];
		}

		//des3_context ctx3;
		//des3_set_3keys( &ctx3, &data_encryption_key_var[0], &data_encryption_key_var[8], &data_encryption_key_var[0] );
		//des3_encrypt( &ctx3, &data_encryption_key_var[0], &data_encryption_key[0] );
		//des3_encrypt( &ctx3, &data_encryption_key_var[8], &data_encryption_key[8] );

		Des3Context context;
		des3CipherAlgo.init( &context, &data_encryption_key_var[0], 16 );
		ecbEncrypt( &des3CipherAlgo, &context, &data_encryption_key_var[0], &data_encryption_key[0], 8 );
		ecbEncrypt( &des3CipherAlgo, &context, &data_encryption_key_var[8], &data_encryption_key[8], 8 );
	}

	void dukpt_generate_data_encryption_response_key( unsigned char data_encryption_key[16], const unsigned char session_key[16] )
	{
		unsigned char data_encryption_key_p[]=
		{
			00,00,00,0xff,00,00,00,00,
			00,00,00,0xff,00,00,00,00
		};

		unsigned char data_encryption_key_var[16];

		for( auto i=0; i<16; i++ ){
			data_encryption_key_var[i] = session_key[i] ^ data_encryption_key_p[i];
		}

		//des3_context ctx3;
		//des3_set_3keys( &ctx3, &data_encryption_key_var[0], &data_encryption_key_var[8], &data_encryption_key_var[0] );
		//des3_encrypt( &ctx3, &data_encryption_key_var[0], &data_encryption_key[0] );
		//des3_encrypt( &ctx3, &data_encryption_key_var[8], &data_encryption_key[8] );

		Des3Context context;
		des3CipherAlgo.init( &context, &data_encryption_key_var[0], 16 );
		ecbEncrypt( &des3CipherAlgo, &context, &data_encryption_key_var[0], &data_encryption_key[0], 8 );
		ecbEncrypt( &des3CipherAlgo, &context, &data_encryption_key_var[8], &data_encryption_key[8], 8 );
	}

	void dukpt_generate_mac( unsigned char mac8[8], const unsigned char mac_encryption_key[16], const unsigned char message[], unsigned int nMsg  )
	{
		unsigned char mac_key[16];
		copy( mac_encryption_key, &mac_encryption_key[16], mac_key );
		unsigned char data[8], data_in[8];
		unsigned int nLoop = nMsg/8;
		bool bRun = true;

		fill( data_in, &data_in[8], 0 );
		//des_context ctx;
		DesContext context;
		unsigned int i;
		for( i = 0; i<nLoop; i++ ){
			copy( &message[i*8], &message[i*8+8], data );

			for( int j =0; j<8; j++ ){		data_in[j] ^= data[j]; }
		
			//des_set_key( &ctx, mac_key );
			//des_encrypt( &ctx, data_in, data );
			desCipherAlgo.init( &context, &mac_key[0], 8 );
			ecbEncrypt( &desCipherAlgo, &context, data_in, data, 8 );
			copy( data, &data[8], data_in );
		}//end for

		if( nMsg % 8 != 0 ){
			fill( data, &data[8], 0 );
			copy( &message[i*8], &message[i*8+(nMsg%8)], data );

			for( int j =0; j<8; j++ ){		data_in[j] ^= data[j]; }

			//des_set_key( &ctx, mac_key );
			//des_encrypt( &ctx, data_in, data );
			desCipherAlgo.init( &context, &mac_key[0], 8 );
			ecbEncrypt( &desCipherAlgo, &context, data_in, data, 8 );
			copy( data, &data[8], data_in );
		}

		//des_set_key( &ctx, &mac_key[8] );
		//des_decrypt( &ctx, data_in, data );
		desCipherAlgo.init( &context, &mac_key[8], 8 );
		ecbDecrypt( &desCipherAlgo, &context, data_in, data, 8 );
		copy( data, &data[8], data_in );

		//des_set_key( &ctx, mac_key );
		//des_encrypt( &ctx, data_in, data );
		desCipherAlgo.init( &context, &mac_key[0], 8 );
		ecbEncrypt( &desCipherAlgo, &context, data_in, data, 8 );
		copy( data, &data[8], mac8 );
	}

	void dukpt_get_mac4_from_mac8( unsigned char mac4[4], const unsigned char mac8[8] )
	{
		copy( mac8, &mac8[4], mac4 );
	}

	////////////////////
	void aes_cbc_descrypt( typeBytes &out, const typeBytes &in, const unsigned char key[16], const unsigned char iv[16] )
	{
		// decryption AES CBC mode.
		typeBytes vkey(16), viv(16);
		copy( &key[0], &key[16], vkey.begin() );
		copy( &iv[0], &iv[16], viv.begin() );

		_de_cbc_aes( vkey, viv, in, out );
	}

	void aes_cbc_encrypt_with_padding( typeBytes &out, const typeBytes & in, const unsigned char key[16], const unsigned char iv[16], unsigned char cPadding )
	{
		// encryption AES CBC mode.
		typeBytes vkey(16), viv(16);
		copy( &key[0], &key[16], vkey.begin() );
		copy( &iv[0], &iv[16], viv.begin() );

		_en_cbc_aes( vkey, viv, in, out, cPadding );
	}

	void tdes_cbc_descrypt( typeBytes &out, const typeBytes &in, const unsigned char key[16], const unsigned char iv[8] )
	{
		typeBytes vkey(16), viv(8);
		copy( &key[0], &key[16], vkey.begin() );
		copy( &iv[0], &iv[8], viv.begin() );

		_de_cbc_tdes( vkey, viv, in, out );
	}

	void tdes_cbc_encrypt_with_padding( typeBytes &out, const typeBytes & in, const unsigned char key[16], const unsigned char iv[8], unsigned char cPadding  )
	{
		typeBytes vkey(16), viv(8);
		copy( &key[0], &key[16], vkey.begin() );
		copy( &iv[0], &iv[8], viv.begin() );

		_en_cbc_tdes( vkey, viv, in, out, cPadding );
	}

	void tdes_ebc_descrypt( typeBytes &out, const typeBytes &in, const unsigned char key[16] )
	{
		typeBytes vkey(16);
		copy( &key[0], &key[16], vkey.begin() );

		_de_ebc_tdes( vkey, in, out );
	}

	void tdes_ebc_encrypt_with_padding( typeBytes &out, const typeBytes & in, const unsigned char key[16], unsigned char cPadding  )
	{
		typeBytes vkey(16);
		copy( &key[0], &key[16], vkey.begin() );

		_en_ebc_tdes( vkey, in, out, cPadding );
	}
	
	void des_cbc_descrypt( typeBytes &out, const typeBytes &in, const unsigned char key[8], const unsigned char iv[8] )
	{
		typeBytes vkey(18), viv(8);
		copy( &key[0], &key[8], vkey.begin() );
		copy( &iv[0], &iv[8], viv.begin() );

		_de_cbc_des( vkey, viv, in, out );
	}

	void des_cbc_encrypt_with_padding( typeBytes &out, const typeBytes & in, const unsigned char key[8], const unsigned char iv[8], unsigned char cPadding  )
	{
		typeBytes vkey(8), viv(8);
		copy( &key[0], &key[8], vkey.begin() );
		copy( &iv[0], &iv[8], viv.begin() );

		_en_cbc_des( vkey, viv, in, out, cPadding );
	}

	void des_ebc_descrypt( typeBytes &out, const typeBytes &in, const unsigned char key[8] )
	{
		typeBytes vkey(8);
		copy( &key[0], &key[8], vkey.begin() );

		_de_ebc_des( vkey, in, out );
	}

	void des_ebc_encrypt_with_padding( typeBytes &out, const typeBytes & in, const unsigned char key[8], unsigned char cPadding  )
	{
		typeBytes vkey(8);
		copy( &key[0], &key[8], vkey.begin() );

		_en_ebc_des( vkey, in, out, cPadding );
	}

	void base64_encode( typeBytes &out, const typeBytes &in )
	{
		if( in.empty() )
			return;
		//
		typeBytes::size_type len = in.size() * 8;

		if( len % 6 == 0 ){
			len = len/6;
		}
		else{
			len = len/6;	len++;
		}
		//
		out.resize( len, 0 );
		base64Encode( &in[0], in.size(), reinterpret_cast<char_t*>(&out[0]), &len );
	}

	void base64_decode( typeBytes &out, const typeBytes &in )
	{
		if( in.empty() )
			return;
		//
		typeBytes::size_type len = in.size() * 6;

		if( len % 8 == 0 ){
			len = len / 8;
		}
		else{
			len = len / 8;	len--;
		}

		out.resize( len, 0 );
		base64Decode( reinterpret_cast<const char_t*>(&in[0]), in.size(), &out[0], &len );
	}


}