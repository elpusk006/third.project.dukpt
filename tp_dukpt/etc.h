#pragma once

#include <vector>

namespace __etc{
	typedef	std::vector<unsigned char> typeBytes;
	typedef	std::vector<CString> typeStrings;

bool AsciisToBytes( typeBytes & vBytes, const CString & sAsciis );

bool HexsToBytes( typeBytes & vBytes, const CString & sInHexs );

void BytesToHexs( CString & sHex, const unsigned char *sByte, unsigned int nByte );

}