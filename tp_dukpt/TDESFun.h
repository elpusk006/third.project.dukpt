/************************************************/
//	Tripple DES header 
//	for AT90USB162, and Xtal : 8MHz
/************************************************/
#if !defined( _TDES_FUN_MODUAL_HEADER_ )
#define _TDES_FUN_MODUAL_HEADER_


//=============================
//Constants for DES
//=============================
#define		CDES_ENCRYPT		0x0e
#define		CDES_DECRYPT		0x0d

//==============================
// the function prototype
//==============================

//************************************************************
//1.DES function
//************************************************************
// input :	1. psDESInData(8bytes)-plain text
//			2. psKey(8bytes)-encrption key
//			3. Acc	(1)encrypt  : input constant CDES_ENCRYPT
//					(2)decrypt  : input cinstant CDES_DECRYPT
//return:	
//		psDESOutData( 8bytes) - chipperd text
//============================================================
BOOL DESFun( BYTE *psDESOutData,BYTE *psDESInData,BYTE *psKey,BYTE bMode );
//

//************************************************************
//18. TDESFun function
//************************************************************
//	input :	1. psDESInData(8bytes)
//			2. psKeyOriginal(16bytes)
//			3. bMode(1)encrypt  : input CDES_ENCRYPT
//					(2)decrypt  : input CDES_DECRYPT
//	return:	
//			1. psDESOutData( 8bytes)
//============================================================
BOOL TDESFun( BYTE *psTDESOutData,BYTE *psTDESInData,BYTE *psTDESKey,BYTE bMode );
 

#endif//end _TDES_FUN_MODUAL_HEADER_
