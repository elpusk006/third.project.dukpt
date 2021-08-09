#include "stdafx.h"

#include "etc.h"

namespace __etc{

static bool HexToByte(  unsigned char & cByte, const CString & sHex );

bool HexToByte(  unsigned char & cByte, const CString & sHex )
{
	unsigned char cH =0, cL =0;

	if( sHex.GetLength() == 2 ){
		cH = static_cast<unsigned char>(sHex[0]);
		cL = static_cast<unsigned char>(sHex[1]);
		//
		if( cH >= 'a' && cH <='z' ){			cH = cH-'a'+0x0A;		}
		else if( cH >= 'A' && cH <='Z' ){		cH = cH-'A'+0x0A;		}
		else if( cH >= '0' && cH <='9' ){		cH = cH-'0';		}
		else return false;

		cH <<= 4;
		/////////
		if( cL >= 'a' && cL <='z' ){			cL = cL-'a'+0x0A;		}
		else if( cL >= 'A' && cL <='Z' ){		cL = cL-'A'+0x0A;		}
		else if( cL >= '0' && cL <='9' ){		cL = cL-'0';		}
		else return false;

		cByte = cH | cL;
		return true;
	}
	else
		return false;
}

bool AsciisToBytes( typeBytes & vBytes, const CString & sAsciis )
{
	vBytes.clear();

	if( sAsciis.IsEmpty() )
		return false;

	#ifdef	_UNICODE
			vBytes.resize( (sAsciis.GetLength()+1)*sizeof(TCHAR), 0 );
			auto nByte = WideCharToMultiByte(
				CP_ACP,
				0,
				(LPCTSTR)sAsciis,
				sAsciis.GetLength(),
				reinterpret_cast<LPSTR>(&vBytes[0]),
				vBytes.size(),
				NULL,
				NULL
				);

			vBytes.resize( nByte );
	#else
			vBytes.resize( sAsciis.GetLength(), 0 );
			copy( vBytes.begin(), vBytes.end(), (LPCTSTR)sAsciis );
	#endif

			return true;
}

bool HexsToBytes( typeBytes & vBytes, const CString & sInHexs )
{
	CString sHexs( sInHexs );
	sHexs.Replace( _T(" "), _T("") );

	if( sHexs.IsEmpty() )
		return false;
	if( (sHexs.GetLength()% 2) != 0 )
		return false;

	vBytes.resize(0);
	CString sHex;
	unsigned char cByte;

	for( int i =0; i<(sHexs.GetLength()/2); i++ ){
		sHex = sHexs.Mid( i*2,2 );

		
		if( !HexToByte( cByte, sHex ) )
			return false;

		vBytes.push_back( cByte );
	}


	return true;
}

void BytesToHexs( CString & sHex, const unsigned char *sByte, unsigned int nByte )
{
	sHex.Empty();

	if( sByte ){
		CString stemp;

		for( auto i=0; i<nByte; i++ ){
			stemp.Format( _T("%02X"), sByte[i] );
			sHex += stemp;
		}//end for

	}
}


}