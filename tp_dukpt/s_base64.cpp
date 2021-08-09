
#include "stdafx.h"
#include "s_base64.h"

static const char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";


static bool is_base64(unsigned char c){
	return (isalnum(c) || (c == '+') || (c == '/'));
}

static unsigned char local_find_in_base64_chars( unsigned char c )
{
	unsigned char i =0;

	for( i=0; i<sizeof(base64_chars); i++ ){
		if( base64_chars[i] == c ){
			break;//exit for
		}
	}

	return i;
}

// out : base64 codes.
unsigned int base64_encode( const unsigned char *in, unsigned int nIn, unsigned char *out )
{
	unsigned int nOut = 0;
	int i = 0;
	int j = 0;
	int k = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	if( in == NULL ||  nIn == 0 )
		return nOut;

	while( nIn-- ) {
		char_array_3[i++] = in[k++];
		if (i == 3) {
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for(i = 0; (i <4) ; i++){
				if( out )
					out[nOut] += base64_chars[char_array_4[i]];
				nOut++;
			}
			i = 0;
		}
	}

	if( i ){
		for(j = i; j < 3; j++)
			char_array_3[j] = '\0';

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;

		for (j = 0; (j < i + 1); j++){
			if( out )
				out[nOut] += base64_chars[char_array_4[j]];
			nOut++;
		}

		while((i++ < 3)){
			if( out )
				out[nOut] = '=';
			nOut++;
		}
	}

	return nOut;
}

// in : base64 codes.
unsigned int base64_decode( const unsigned char *in, unsigned int nIn, unsigned char *out )
{
	int in_len = nIn;
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned int nOut = 0;
	unsigned char char_array_4[4], char_array_3[3];

	while (in_len-- && ( in[in_] != '=') && is_base64(in[in_])) {
		char_array_4[i++] = in[in_]; in_++;
		if (i ==4) {
			for (i = 0; i <4; i++)
				char_array_4[i] = local_find_in_base64_chars(char_array_4[i]);

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (i = 0; (i < 3); i++){
				if( out )
					out[nOut] = char_array_3[i];
				nOut++;
			}
			i = 0;
		}
	}

	if (i) {
		for (j = i; j <4; j++)
			char_array_4[j] = 0;

		for (j = 0; j <4; j++)
			char_array_4[j] = local_find_in_base64_chars(char_array_4[j]);

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

		for (j = 0; (j < i - 1); j++){
			if( out )
				out[nOut] = char_array_3[j];
			nOut++;
		}
	}

	return nOut;
}