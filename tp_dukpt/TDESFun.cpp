/************************************************/
/*TDES Function body					       */
/************************************************/

#include "stdafx.h"
#include "TDESFun.h"


static void MyLibMemCpy( BYTE *des,BYTE *src,BYTE bSize );
static void MyLibMemSet( BYTE *des,BYTE bFill,BYTE bSize );

//************************************************************
//2. AF_DESsub_EXPAND_ARRANGE:
//   Expand 32bits to sRB3(**** **00)48bits
//************************************************************
// input : 1.psKey(8bytes)-encrption key
// return
//		psRB3( 8bytes )
//============================================================
static void DESsub_EXPAND_ARRANGE( BYTE *psRB3,BYTE *psKey );
//

//
//************************************************************
//3. AF_DESsub_F_FUNCTION:
//************************************************************
static void DESsub_F_FUNCTION( BYTE *psKey,BYTE *psRB3,BYTE *psKeyTable );
//

//
//************************************************************
//4. AF_DESsub_F_FUNCTION_DECORD:
//************************************************************
static void DESsub_F_FUNCTION_DECORD( BYTE *psKey,BYTE *psRB3,BYTE *psKeyTable );
//

//
//************************************************************
//5. DESsub_FINAL_ARRANGE:
//************************************************************
// input : 1.psDESOutData( 8bytes)
// return
//		Good: psDESOutData( 8bytes),return TRUE;
//		FALSE: return FALSE;
//============================================================
static BOOL DESsub_FINAL_ARRANGE( BYTE *psDESOutData,BYTE *psKey );
//

//
//************************************************************
//6. AF_DESsub_I_A
//************************************************************
// input : 1.psKey( 8bytes)
// return
//			psDESOutData( 8bytes)
//============================================================
static void DESsub_I_A( BYTE *psDESOutData,BYTE *psKey );
//

//
//
//************************************************************
//7. AF_DESsub_KEY_GENERATOR
//	generate Key Table.
//************************************************************
// input :	1.psDESOutData( 8bytes)
//			2.psKey( 8bytes)
// return
//		psKeyTable( 128bytes)
//============================================================
static void DESsub_KEY_GENERATOR( BYTE *psKeyTable,BYTE *psKey,BYTE *psDESOutData );
//

//
//************************************************************
//8. AF_DESsub_mov_8_BYTES
//	move Src(8bytes) to Des(8bytes)
//************************************************************
// input : 1.psSrc( 8bytes)
// return
//		psDes( 8bytes)
//============================================================
static void DESsub_mov_8_BYTES( BYTE *psDes,BYTE *psSrc );
//

//
//************************************************************
//09. AF_DESsub_movE_movE
//************************************************************
//	input : 1.psKey( 8bytes)
//			2.psDESOutData9 8bytes )
//	Output: 1.psKey( 8bytes)
//			2.psDESOutData9 8bytes )
//============================================================
static void DESsub_movE_movE( BYTE *psDESOutData,BYTE *psKey );
//

//
//************************************************************
//10. AF_DESsub_PC_1_ARRANGE
//************************************************************
// input : 1.psKey( 8bytes)
// return
//			psDESOutData( 8bytes)
//============================================================
static void DESsub_PC_1_ARRANGE( BYTE *psDESOutData,BYTE *psKey );
//

//************************************************************
//11. AF_DESsub_PC_2_ARRANGE
//************************************************************
//	input : 1.psKey( 8bytes)
//	Output: 1.psDes( 8bytes)
//============================================================
static void DESsub_PC_2_ARRANGE( BYTE *psDes,BYTE *psKey );
//

//************************************************************
//12. AF_DESsub_ROLATION_RIGHT
//************************************************************
//	input : 1.psKey( 8bytes)
//	Output: 1.psKey( 8bytes)
//============================================================
static void DESsub_ROLATION_RIGHT( BYTE *psKey );
//

//************************************************************
//13. AF_DESsub_S_BOXES
//************************************************************
//	input : 1.psKey( 8bytes)
//	Output: 1.psRB3( 8bytes )
//============================================================
static void DESsub_S_BOXES( BYTE *psRB3,BYTE *psKey );
//

//************************************************************
//14. AF_DESsub_SHRINK_ARRANGE
//************************************************************
//	input : 1.psKey( 8bytes)
//			2.psRB3( 8bytes )
//	Output: 1.psRB3( 8bytes )
//			2.psKey( 8bytes)
//============================================================
static void DESsub_SHRINK_ARRANGE( BYTE *psRB3,BYTE *psKey );
//

//************************************************************
//15. AF_DESsub_XOR_4_BYTES
//************************************************************
//	input : 1.psRB3( 8bytes)
//	Output: 1.psKey( 8bytes )
//============================================================
static void DESsub_XOR_4_BYTES( BYTE *psKey,BYTE *psRB3,BYTE *psDESOutData );
//

//************************************************************
//16. AF_DESsub_XOR_80H_18H
//************************************************************
//	input : 1.psKey( 8bytes)
//			2.psRB3( 8bytes )
//	Output: 1.psRB3( 8bytes )
//			2.psKey( 8bytes)
//============================================================
static void DESsub_XOR_80H_18H( BYTE *psKey,BYTE *psRB3,BYTE *psKeyTable );
//

//************************************************************
//17. AF_DESsub_XOR_FFH_1FH
//************************************************************
//	input : 1.psKey( 8bytes)
//			2.psRB3( 8bytes )
//	Output: 1.psRB3( 8bytes )
//			2.psKey( 8bytes)
//============================================================
static void DESsub_XOR_FFH_1FH( BYTE *psKey,BYTE *psRB3,BYTE *psKeyTable );
//




static const BYTE CT_ROL_TIMES[]={		//rotation table
	0x01,0x01,0x02,0x02,0x02,0x02,0x02,0x02,
	0x01,0x02,0x02,0x02,0x02,0x02,0x02,0x01
};


static const BYTE CT_S_BOXES_DATA[]={	//primitive function Sn( 1��n��8 )
	14,  0,  4, 15, 13,  7,  1,  4,  2, 14, 15,  2, 11, 13,  8,  1,
	3, 10, 10,  6,  6, 12, 12, 11,  5,  9,  9,  5,  0,  3,  7,  8,
	4, 15,  1, 12, 14,  8,  8,  2, 13,  4,  6,  9,  2,  1, 11,  7,
	15,  5, 12, 11,  9,  3,  7, 14,  3, 10, 10,  0,  5,  6,  0, 13,
               
	15,  3,  1, 13,  8,  4, 14,  7,  6, 15, 11,  2,  3,  8,  4, 14,
	9, 12,  7,  0,  2,  1, 13, 10, 12,  6,  0,  9,  5, 11, 10,  5,
	0, 13, 14,  8,  7, 10, 11,  1, 10,  3,  4, 15, 13,  4,  1,  2,
	5, 11,  8,  6, 12,  7,  6, 12,  9,  0,  3,  5,  2, 14, 15,  9,

	10, 13,  0,  7,  9,  0, 14,  9,  6,  3,  3,  4, 15,  6,  5, 10,
	1,  2, 13,  8, 12,  5,  7, 14, 11, 12,  4, 11,  2, 15,  8,  1,
	13,  1,  6, 10,  4, 13,  9,  0,  8,  6, 15,  9,  3,  8,  0,  7,
	11,  4,  1, 15,  2, 14, 12,  3,  5, 11, 10,  5, 14,  2,  7, 12,

	7, 13, 13,  8, 14, 11,  3,  5,  0,  6,  6, 15,  9,  0, 10,  3,
	1,  4,  2,  7,  8,  2,  5, 12, 11,  1, 12, 10,  4, 14, 15,  9,
	10,  3,  6, 15,  9,  0,  0,  6, 12, 10, 11,  1,  7, 13, 13,  8,
	15,  9,  1,  4,  3,  5, 14, 11,  5, 12,  2,  7,  8,  2,  4, 14,

	2, 14, 12, 11,  4,  2,  1, 12,  7,  4, 10,  7, 11, 13,  6,  1,
	8,  5,  5,  0,  3, 15, 15, 10, 13,  3,  0,  9, 14,  8,  9,  6,
	4, 11,  2,  8,  1, 12, 11,  7, 10,  1, 13, 14,  7,  2,  8, 13,
	15,  6,  9, 15, 12,  0,  5,  9,  6, 10,  3,  4,  0,  5, 14,  3,

	12, 10,  1, 15, 10,  4, 15,  2,  9,  7,  2, 12,  6,  9,  8,  5,
	0,  6, 13,  1,  3, 13,  4, 14, 14,  0,  7, 11,  5,  3, 11,  8,
	9,  4, 14,  3, 15,  2,  5, 12,  2,  9,  8,  5, 12, 15,  3, 10,
	7, 11,  0, 14,  4,  1, 10,  7,  1,  6, 13,  0, 11,  8,  6, 13,

	4, 13, 11,  0,  2, 11, 14,  7, 15,  4,  0,  9,  8,  1, 13, 10,
	3, 14, 12,  3,  9,  5,  7, 12,  5,  2, 10, 15,  6,  8,  1,  6,
	1,  6,  4, 11, 11, 13, 13,  8, 12,  1,  3,  4,  7, 10, 14,  7,
	10,  9, 15,  5,  6,  0,  8, 15,  0, 14,  5,  2,  9,  3,  2, 12,

	13,  1,  2, 15,  8, 13,  4,  8,  6, 10, 15,  3, 11,  7,  1,  4,
	10, 12,  9,  5,  3,  6, 14, 11,  5,  0,  0, 14, 12,  9,  7,  2,
	7,  2, 11,  1,  4, 14,  1,  7,  9,  4, 12, 10, 14,  8,  2, 13,
	0, 15,  6, 12, 10,  9, 13,  0, 15,  3,  3,  5,  5,  6,  8, 11
};
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
BOOL DESFun( BYTE *psDESOutData,BYTE *psDESInData,BYTE *psKey,BYTE bMode )
{
	BYTE sKeyTable[128];
	BYTE *psKeyTable;
	BYTE sRB3[8];
	BYTE bi;
	//
	MyLibMemSet( sKeyTable,0x00,128 );
	MyLibMemSet( sRB3,0x00,8 );
	//
	if( bMode==CDES_ENCRYPT ){
		DESsub_KEY_GENERATOR( sKeyTable,psKey,psDESOutData );
		DESsub_mov_8_BYTES( psKey,psDESInData );
		DESsub_I_A( psDESOutData,psKey );
		DESsub_mov_8_BYTES( psKey,psDESOutData );
		//
		//mov	B,#cKEYTEMP
		psKeyTable=sKeyTable;	//set Key table pointer
		for( bi=0; bi<16; bi++ ){//set counter
			DESsub_F_FUNCTION( psKey,sRB3,psKeyTable );
			DESsub_XOR_4_BYTES( psKey,sRB3,psDESOutData );
			DESsub_movE_movE( psDESOutData,psKey );
			psKeyTable+=8;
		}
		DESsub_FINAL_ARRANGE( psDESOutData,psKey );
	}
	else if( bMode==CDES_DECRYPT ){
		DESsub_KEY_GENERATOR( sKeyTable,psKey,psDESOutData );  
		DESsub_mov_8_BYTES( psKey,psDESInData );
		DESsub_I_A( psDESOutData,psKey );
		DESsub_mov_8_BYTES( psKey,psDESOutData );
		//
		//	mov	B,#cKEYTEMP+127
		psKeyTable=&(sKeyTable[127-7]);	//set Key table pointer
		for( bi=0; bi<16; bi++ ){//set counter
			DESsub_F_FUNCTION_DECORD( psKey,sRB3,psKeyTable );
			DESsub_XOR_4_BYTES( psKey,sRB3,psDESOutData );
			DESsub_movE_movE( psDESOutData,psKey );
			psKeyTable-=8;
		}
		DESsub_FINAL_ARRANGE( psDESOutData,psKey );
	}
 	//
	return FALSE;
}
//

//
static const BYTE CT_EXPAND_ARRANGE[]={//index
	7,4,5,
	4,5,6,
	5,6,7,
	6,7,4
};

//************************************************************
//2. AF_DESsub_EXPAND_ARRANGE:
//   Expand 32bits to sRB3(**** **00)48bits
//************************************************************
// input : 1.psKey(8bytes)-encrption key
// return
//		psRB3( 8bytes )
//============================================================
void DESsub_EXPAND_ARRANGE( BYTE *psRB3,BYTE *psKey )
{
	BYTE bTemp;
	BYTE i=0;
	BYTE index;
	//
	bTemp=0x00;
	
	for( i=0; i<4; i++ ){
		//even
		psRB3[2*i]=0;
		//index=pgm_read_byte(&(CT_EXPAND_ARRANGE[i*3]));
		index=CT_EXPAND_ARRANGE[i*3];
		
		if( (psKey[index]|0xFE)==0xFF ){
			psRB3[2*i]|=0x80;
		}
		
		//index=pgm_read_byte(&(CT_EXPAND_ARRANGE[i*3+1]));
		index=CT_EXPAND_ARRANGE[i*3+1];
		if( (psKey[index]|0x7F)==0xFF ){
			psRB3[2*i]|=0x40;
		}	
		if( (psKey[index]|0xBF)==0xFF ){
			bTemp|=0x20;
		}
		if( (psKey[index]|0xDF)==0xFF ){
			psRB3[2*i]|=0x10;
		}
		if( (psKey[index]|0xEF)==0xFF ){
			psRB3[2*i]|=0x08;
		}
		if( (psKey[index]|0xF7)==0xFF ){
			psRB3[2*i]|=0x04;
		}
		psRB3[2*i]=bTemp;
		//////////////////////////
		//odd
		psRB3[2*i+1]=0;
		//
		if( (psKey[index]|0xEF)==0xFF ){
			psRB3[2*i+1]|=0x80;
		}	
		if( (psKey[index]|0xF7)==0xFF ){
			psRB3[2*i+1]|=0x40;
		}
		if( (psKey[index]|0xFB)==0xFF ){
			psRB3[2*i+1]|=0x20;
		}
		if( (psKey[index]|0xFD)==0xFF ){
			psRB3[2*i+1]|=0x10;
		}
		if( (psKey[index]|0xFE)==0xFF ){
			psRB3[2*i+1]|=0x08;
		}
		
		//index=pgm_read_byte(&(CT_EXPAND_ARRANGE[i*3+2]));
		index=CT_EXPAND_ARRANGE[i*3+2];
		if( (psKey[index]|0x7F)==0xFF ){
			psRB3[2*i+1]|=0x04;
		}
		psRB3[2*i+1]=bTemp;
		
	}//end for
}
//

//
//************************************************************
//3. AF_DESsub_F_FUNCTION:
//************************************************************
void DESsub_F_FUNCTION( BYTE *psKey,BYTE *psRB3,BYTE *psKeyTable )
{
	DESsub_EXPAND_ARRANGE( psRB3,psKey );
	DESsub_XOR_80H_18H( psKey,psRB3,psKeyTable );
	DESsub_S_BOXES( psRB3,psKey );
	DESsub_SHRINK_ARRANGE( psRB3,psKey );
}
//

//
//************************************************************
//4. AF_DESsub_F_FUNCTION_DECORD:
//************************************************************
void DESsub_F_FUNCTION_DECORD( BYTE *psKey,BYTE *psRB3,BYTE *psKeyTable )
{
	DESsub_EXPAND_ARRANGE( psRB3,psKey );
	DESsub_XOR_FFH_1FH( psKey,psRB3,psKeyTable );
	DESsub_S_BOXES( psRB3,psKey );
	DESsub_SHRINK_ARRANGE( psRB3,psKey );
}
//

//
static const BYTE CT_FINAL_ARRANGE[]={	//mask pattern
	0xFE,0xFD,0xFB,0xF7,
	0xEF,0xDF,0xBF,0x7F
};
//************************************************************
//5. AF_DESsub_EXPAND_ARRANGE:
//   Expand 32bits to sRB3(**** **00)48bits
//************************************************************
// input : 1.psDESOutData( 8bytes)
// return
//		Good: psDESOutData( 8bytes),return TRUE;
//		FALSE: return FALSE;
//============================================================
BOOL DESsub_FINAL_ARRANGE( BYTE *psDESOutData,BYTE *psKey )
{
	BYTE cMask;
	BYTE i;
	//
	if( psDESOutData==NULL )
		return FALSE;
	//
	psKey[0]=psDESOutData[4];
	psKey[1]=psDESOutData[5];
	psKey[2]=psDESOutData[6];
	psKey[3]=psDESOutData[7];
	psKey[4]=psDESOutData[0];
	psKey[5]=psDESOutData[1];
	psKey[6]=psDESOutData[2];
	psKey[7]=psDESOutData[3];
	//---PC-1 ARRANGE
	
	for( i=0; i<8; i++ ){
		psDESOutData[i]=0;
		//cMask=pgm_read_byte(&(CT_FINAL_ARRANGE[i]));
		cMask=CT_FINAL_ARRANGE[i];
		//
		if( (psKey[4]|cMask)==0xFF )	psDESOutData[i]|=0x80;
		if( (psKey[0]|cMask)==0xFF )	psDESOutData[i]|=0x40;
		if( (psKey[5]|cMask)==0xFF )	psDESOutData[i]|=0x20;
		if( (psKey[1]|cMask)==0xFF )	psDESOutData[i]|=0x10;
		if( (psKey[6]|cMask)==0xFF )	psDESOutData[i]|=0x08;
		if( (psKey[2]|cMask)==0xFF )	psDESOutData[i]|=0x04;
		if( (psKey[7]|cMask)==0xFF )	psDESOutData[i]|=0x02;
		if( (psKey[3]|cMask)==0xFF )	psDESOutData[i]|=0x01;
		
	}//end for
	//
	return TRUE;
}
//

//
static const BYTE CT_I_A[]={	//mask pattern
	0xBF,0xEF,0xFB,0xFE,
	0x7F,0xDF,0xF7,0xFD
};

//************************************************************
//6. AF_DESsub_I_A
//************************************************************
// input : 1.psKey( 8bytes)
// return
//			psDESOutData( 8bytes)
//============================================================
void DESsub_I_A( BYTE *psDESOutData,BYTE *psKey )
{
	BYTE i,j,cMask;
	BYTE cSubMask;
	//
	for( i=0; i<8; i++ ){
		psDESOutData[i]=0;
		//cMask=pgm_read_byte(&(CT_I_A[i]));
		cMask=CT_I_A[i];
		cSubMask=0x01;
		
		for( j=0; j<8; j++ ){
			if( (psKey[j]|cMask)==0xFF ){
				psDESOutData[i]|=cSubMask;
				cSubMask=cSubMask<<1;
			}
		}//end for j
	}//end for i
}
//

//
//
//************************************************************
//7. AF_DESsub_KEY_GENERATOR
//	generate Key Table.
//************************************************************
// input :	1.psDESOutData( 8bytes)
//			2.psKey( 8bytes)
// return
//		psKeyTable( 128bytes)
//============================================================
void DESsub_KEY_GENERATOR( BYTE *psKeyTable,BYTE *psKey,BYTE *psDESOutData )
{
	BYTE bi,bj;

	DESsub_PC_1_ARRANGE( psDESOutData,psKey );
	//After PC-1 arrange the data stored in RB0        
	//movE PC-1DATA(RB0)   TO 20H~27H
	DESsub_mov_8_BYTES( psKey,psDESOutData );

	//KEY GENERATION LOOP B=cKEYTEMP
	for( bi=0; bi<16; bi++ ){//
		for( bj=0; bj<CT_ROL_TIMES[bi]; bj++ )
			DESsub_ROLATION_RIGHT( psKey );
		//
		DESsub_PC_2_ARRANGE( psKeyTable+bi*8,psKey );	
	}
	//
}
//

//
//************************************************************
//8. AF_DESsub_mov_8_BYTES
//	move Src(8bytes) to Des(8bytes)
//************************************************************
// input : 1.psSrc( 8bytes)
// return
//		psDes( 8bytes)
//============================================================
void DESsub_mov_8_BYTES( BYTE *psDes,BYTE *psSrc )
{
	BYTE bi;
	//
	for( bi=0; bi<8; bi++ )
		psDes[bi]=psSrc[bi];
	//
}
//

//
//************************************************************
//09. AF_DESsub_movE_movE
//************************************************************
//	input : 1.psKey( 8bytes)
//			2.psDESOutData9 8bytes )
//	Output: 1.psKey( 8bytes)
//			2.psDESOutData9 8bytes )
//============================================================
void DESsub_movE_movE( BYTE *psDESOutData,BYTE *psKey )
{
	psKey[0]=psDESOutData[4];
	psKey[1]=psDESOutData[5];
	psKey[2]=psDESOutData[6];
	psKey[3]=psDESOutData[7];
	;//
	psDESOutData[0]=psDESOutData[4];
	psDESOutData[1]=psDESOutData[5];
	psDESOutData[2]=psDESOutData[6];
	psDESOutData[3]=psDESOutData[7];
	;//
	psDESOutData[4]=psKey[4];
	psDESOutData[5]=psKey[5];
	psDESOutData[6]=psKey[6];
	psDESOutData[7]=psKey[7];
}
//

//
static const BYTE CT_PC_1_ARRANGE[]={	//mask pattern
	0x7F,0xBF,0xDF,0xEF,
	0xFD,0xFB,0xF7,0xEF
};

//************************************************************
//10. AF_DESsub_PC_1_ARRANGE
//************************************************************
// input : 1.psKey( 8bytes)
// return
//			psDESOutData( 8bytes)
//============================================================
void DESsub_PC_1_ARRANGE( BYTE *psDESOutData,BYTE *psKey )
{
	BYTE bTemp;
	BYTE i,cMask;
	//
	for( i=0; i<8; i++ ){
		//cMask=pgm_read_byte(&(CT_PC_1_ARRANGE[i]));
		cMask=CT_PC_1_ARRANGE[i];
		bTemp=0x00;
		
		if( i!=7 ){
			if( (psKey[7]|cMask)==0xFF )	bTemp|=0x80;
			if( (psKey[6]|cMask)==0xFF )	bTemp|=0x40;
			if( (psKey[5]|cMask)==0xFF )	bTemp|=0x20;
			if( (psKey[4]|cMask)==0xFF )	bTemp|=0x10;
		}
		
		if( i!=3 ){
			if( i==7 ){
				if( (psKey[3]|cMask)==0xFF )	bTemp|=0x80;
				if( (psKey[2]|cMask)==0xFF )	bTemp|=0x40;
				if( (psKey[1]|cMask)==0xFF )	bTemp|=0x20;
				if( (psKey[0]|cMask)==0xFF )	bTemp|=0x10;
			}
			else{
				if( (psKey[3]|cMask)==0xFF )	bTemp|=0x08;
				if( (psKey[2]|cMask)==0xFF )	bTemp|=0x04;
				if( (psKey[1]|cMask)==0xFF )	bTemp|=0x02;
				if( (psKey[0]|cMask)==0xFF )	bTemp|=0x01;
			}
		}		
		psDESOutData[i]=bTemp;
	}//end for
}
//

//
static const BYTE CT_PC_2_ARRANGE_INDEX[8][6]={	//index
	{1,2,1,2,0,0},
	{0,3,1,0,2,1},
	{2,2,1,0,3,0},
	{1,0,3,2,1,0},
	{5,6,4,5,6,7},
	{4,5,6,6,4,6},
	{5,6,5,7,4,7},
	{6,5,6,4,4,4}
};

static const BYTE CT_PC_2_ARRANGE_MASK[8][6]={	//index
	{0xFB,0x7F,0xDF,0xFE,0x7F,0xF7},
	{0xDF,0xEF,0xFD,0xFB,0xF7,0xBF},
	{0xFD,0xDF,0xEF,0xEF,0xBF,0xFE},
	{0xFE,0xFD,0xDF,0xEF,0xF7,0xBF},
	{0xF7,0xFE,0xDF,0x7F,0xDF,0xDF},
	{0xBF,0xEF,0xFD,0x7F,0xF7,0xEF},
	{0xFE,0xF7,0xDF,0xEF,0xFB,0x7F},
	{0xBF,0xFB,0xFB,0xFE,0x7F,0xEF}
};

//************************************************************
//11. AF_DESsub_PC_2_ARRANGE
//************************************************************
//	input : 1.psKey( 8bytes)
//	Output: 1.psDes( 8bytes)
//============================================================
void DESsub_PC_2_ARRANGE( BYTE *psDes,BYTE *psKey )
{
	BYTE bTemp;
	BYTE i,j;
	BYTE index,cMask;
	BYTE cSubMask;
	//
	
	for( i=0; i<8; i++ ){
		bTemp=0x00;
		cSubMask=0x80;
		
		for( j=0; j<6; j++ ){
			index=CT_PC_2_ARRANGE_INDEX[i][j];
			cMask=CT_PC_2_ARRANGE_MASK[i][j];

			if( (psKey[index] | cMask)==0xFF ){
				bTemp |= cSubMask;
			}
			cSubMask=cSubMask>>1;
		}
		//
		psDes[i]=bTemp;
		
	}//end for
}
//

//************************************************************
//12. AF_DESsub_ROLATION_RIGHT
//************************************************************
//	input : 1.psKey( 8bytes)
//	Output: 1.psKey( 8bytes)
//============================================================
void DESsub_ROLATION_RIGHT( BYTE *psKey )
{
	BYTE bTemp;
	BOOL bRo,bRo1;
	//
	//ROLATION_RIGHT  20H~23H 1 BIT
	if( (psKey[0]&0x80)==0x00 ){
		bTemp=psKey[3];
		//
		if( (bTemp&0x80)==0x00 ){
			//MSB is Zero
			bRo=FALSE;		
		}
		else{//MSB is one
			bRo=TRUE;
		}
		bTemp=bTemp<<1;
		bTemp=bTemp&0xEF;
	}
	else{
		bTemp=psKey[3];
		//
		if( (bTemp&0x80)==0x00 ){
			//MSB is Zero
			bRo=FALSE;		
		}
		else{//MSB is one
			bRo=TRUE;
		}
		bTemp=bTemp<<1;
		bTemp=bTemp|0x10;
	}
	//
	psKey[3]=bTemp;
	//
	bTemp=psKey[2];
	//
	if( (bTemp&0x80)==0x00 ){
		//MSB is Zero
		bRo1=FALSE;		
	}
	else{//MSB is one
		bRo1=TRUE;
	}
	//
	bTemp=bTemp<<1;
	if( bRo==TRUE )
		bTemp=bTemp|0x01;
	else
		bTemp=bTemp&0xFE;
	//
	//
	psKey[2]=bTemp;
	bTemp=psKey[1];
	//
	if( (bTemp&0x80)==0x00 ){
		//MSB is Zero
		bRo=FALSE;		
	}
	else{//MSB is one
		bRo=TRUE;
	}
	//
	bTemp=bTemp<<1;
	if( bRo1==TRUE )
		bTemp=bTemp|0x01;
	else
		bTemp=bTemp&0xFE;
	//
	psKey[1]=bTemp;
	bTemp=psKey[0];
	//
	bTemp=bTemp<<1;
	if( bRo==TRUE )
		bTemp=bTemp|0x01;
	else
		bTemp=bTemp&0xFE;
	//
	psKey[0]=bTemp;

	////////////////
	//ROLATION_RIGHT  24H~27H 1 BIT
	if( (psKey[4]&0x80)==0x00 ){
		bTemp=psKey[7];
		//
		if( (bTemp&0x80)==0x00 ){
			//MSB is Zero
			bRo=FALSE;		
		}
		else{//MSB is one
			bRo=TRUE;
		}
		bTemp=bTemp<<1;
		bTemp=bTemp&0xEF;
	}
	else{
		bTemp=psKey[7];
		//
		if( (bTemp&0x80)==0x00 ){
			//MSB is Zero
			bRo=FALSE;		
		}
		else{//MSB is one
			bRo=TRUE;
		}
		bTemp=bTemp<<1;
		bTemp=bTemp|0x10;
	}
	//
	psKey[7]=bTemp;
	//
	bTemp=psKey[6];
	//
	if( (bTemp&0x80)==0x00 ){
		//MSB is Zero
		bRo1=FALSE;		
	}
	else{//MSB is one
		bRo1=TRUE;
	}
	//
	bTemp=bTemp<<1;
	if( bRo==TRUE )
		bTemp=bTemp|0x01;
	else
		bTemp=bTemp&0xFE;
	//
	//
	psKey[6]=bTemp;
	bTemp=psKey[5];
	//
	if( (bTemp&0x80)==0x00 ){
		//MSB is Zero
		bRo=FALSE;		
	}
	else{//MSB is one
		bRo=TRUE;
	}
	//
	bTemp=bTemp<<1;
	if( bRo1==TRUE )
		bTemp=bTemp|0x01;
	else
		bTemp=bTemp&0xFE;
	//
	psKey[5]=bTemp;
	bTemp=psKey[4];
	//
	bTemp=bTemp<<1;
	if( bRo==TRUE )
		bTemp=bTemp|0x01;
	else
		bTemp=bTemp&0xFE;
	//
	psKey[4]=bTemp;

}
//

//************************************************************
//13. AF_DESsub_S_BOXES
//************************************************************
//	input : 1.psKey( 8bytes)
//	Output: 1.psRB3( 8bytes )
//============================================================
void DESsub_S_BOXES( BYTE *psRB3,BYTE *psKey )
{
	BYTE bTemp,bHNibble,bLNibble;
	const BYTE *psSBOX;//pgm_read_byte(&(CT_S_BOXES_DATA[0])),bTemp
	//pgm_read_byte(&(psSBOX[bTemp]))
	BYTE i=0;
	
	psSBOX=CT_S_BOXES_DATA;
	
	for( i=0; i<4; i++ ){
		bTemp=0x00;	
		//---20H
		if( (psKey[2*i]|0x7F)==0xFF )	bTemp|=0x20;
		if( (psKey[2*i]|0xBF)==0xFF )	bTemp|=0x10;
		if( (psKey[2*i]|0xDF)==0xFF )	bTemp|=0x08;
		if( (psKey[2*i]|0xEF)==0xFF )	bTemp|=0x04;
		if( (psKey[2*i]|0xF7)==0xFF )	bTemp|=0x02;
		if( (psKey[2*i]|0xFB)==0xFF )	bTemp|=0x01;
		bTemp=psSBOX[bTemp];
		//swap Temp
		bHNibble=bLNibble=bTemp;	bHNibble=bHNibble>>4;	bLNibble=bLNibble<<4;	bTemp=bHNibble|bLNibble;
		psRB3[i]=bTemp;

		//----21H
		bTemp=0x00;	psSBOX+=0x40;
		if( (psKey[2*i+1]|0x7F)==0xFF )	bTemp|=0x20;
		if( (psKey[2*i+1]|0xBF)==0xFF )	bTemp|=0x10;
		if( (psKey[2*i+1]|0xDF)==0xFF )	bTemp|=0x08;
		if( (psKey[2*i+1]|0xEF)==0xFF )	bTemp|=0x04;
		if( (psKey[2*i+1]|0xF7)==0xFF )	bTemp|=0x02;
		if( (psKey[2*i+1]|0xFB)==0xFF )	bTemp|=0x01;
		bTemp=psSBOX[bTemp];
		psRB3[i]=psRB3[i]|bTemp;
		
		psSBOX+=0x40;
	}//end for
}
//

//************************************************************
//14. AF_DESsub_SHRINK_ARRANGE
//************************************************************
//	input : 1.psKey( 8bytes)
//			2.psRB3( 8bytes )
//	Output: 1.psRB3( 8bytes )
//			2.psKey( 8bytes)
//============================================================
void DESsub_SHRINK_ARRANGE( BYTE *psRB3,BYTE *psKey )
{
	BYTE bTemp;

	DESsub_mov_8_BYTES( psKey,psRB3 );

	//---USE sRB3(18H~1FH) TO STORE THE SHRINK ARRANGE TABLE
	//---18H
	bTemp=0x00;
	if( (psKey[1]|0xFE)==0xFF )	bTemp|=0x80;
	if( (psKey[0]|0xFD)==0xFF )	bTemp|=0x40;
	if( (psKey[2]|0xEF)==0xFF )	bTemp|=0x20;
	if( (psKey[2]|0xF7)==0xFF )	bTemp|=0x10;
	if( (psKey[3]|0xF7)==0xFF )	bTemp|=0x08;
	if( (psKey[1]|0xEF)==0xFF )	bTemp|=0x04;
	if( (psKey[3]|0xEF)==0xFF )	bTemp|=0x02;
	if( (psKey[2]|0x7F)==0xFF )	bTemp|=0x01;
	psRB3[0]=bTemp;
	//---19H
	bTemp=0x00;
	if( (psKey[0]|0x7F)==0xFF )	bTemp|=0x80;
	if( (psKey[1]|0xFD)==0xFF )	bTemp|=0x40;
	if( (psKey[2]|0xFD)==0xFF )	bTemp|=0x20;
	if( (psKey[3]|0xBF)==0xFF )	bTemp|=0x10;
	if( (psKey[0]|0xF7)==0xFF )	bTemp|=0x08;
	if( (psKey[2]|0xBF)==0xFF )	bTemp|=0x04;
	if( (psKey[3]|0xFD)==0xFF )	bTemp|=0x02;
	if( (psKey[1]|0xBF)==0xFF )	bTemp|=0x01;
	psRB3[1]=bTemp;
	//
	//---1AH
	bTemp=0x00;
	if( (psKey[0]|0xBF)==0xFF )	bTemp|=0x80;
	if( (psKey[0]|0xFE)==0xFF )	bTemp|=0x40;
	if( (psKey[2]|0xFE)==0xFF )	bTemp|=0x20;
	if( (psKey[1]|0xFB)==0xFF )	bTemp|=0x10;
	if( (psKey[3]|0xFE)==0xFF )	bTemp|=0x08;
	if( (psKey[3]|0xDF)==0xFF )	bTemp|=0x04;
	if( (psKey[0]|0xDF)==0xFF )	bTemp|=0x02;
	if( (psKey[1]|0x7F)==0xFF )	bTemp|=0x01;
	psRB3[2]=bTemp;
	//
	//---1BH
	bTemp=0x00;
	if( (psKey[2]|0xDF)==0xFF )	bTemp|=0x80;
	if( (psKey[1]|0xF7)==0xFF )	bTemp|=0x40;
	if( (psKey[3]|0xFB)==0xFF )	bTemp|=0x20;
	if( (psKey[0]|0xFB)==0xFF )	bTemp|=0x10;
	if( (psKey[2]|0xFB)==0xFF )	bTemp|=0x08;
	if( (psKey[1]|0xDF)==0xFF )	bTemp|=0x04;
	if( (psKey[0]|0xEF)==0xFF )	bTemp|=0x02;
	if( (psKey[3]|0x7F)==0xFF )	bTemp|=0x01;
	psRB3[3]=bTemp;
	//
}
//

//************************************************************
//15. AF_DESsub_XOR_4_BYTES
//************************************************************
//	input : 1.psRB3( 8bytes)
//	Output: 1.psKey( 8bytes )
//============================================================
void DESsub_XOR_4_BYTES( BYTE *psKey,BYTE *psRB3,BYTE *psDESOutData )
{
	BYTE bi;

	for( bi=0; bi<4; bi++ ){
		psKey[bi+4]=psRB3[bi]^psDESOutData[bi];
	}

}
//

//************************************************************
//16. AF_DESsub_XOR_80H_18H
//************************************************************
//	input : 1.psKey( 8bytes)
//			2.psRB3( 8bytes )
//	Output: 1.psRB3( 8bytes )
//			2.psKey( 8bytes)
//============================================================
void DESsub_XOR_80H_18H( BYTE *psKey,BYTE *psRB3,BYTE *psKeyTable )
{
	BYTE bi;

	for( bi=0; bi<8; bi++ ){
		psKey[bi]=psKeyTable[bi]^psRB3[bi];
	}
}
//

//************************************************************
//17. AF_DESsub_XOR_FFH_1FH
//************************************************************
//	input : 1.psKey( 8bytes)
//			2.psRB3( 8bytes )
//	Output: 1.psRB3( 8bytes )
//			2.psKey( 8bytes)
//============================================================
void DESsub_XOR_FFH_1FH( BYTE *psKey,BYTE *psRB3,BYTE *psKeyTable )
{

	BYTE bi;
	BYTE bj;
	//
	bj=7;
	//
	for( bi=0; bi<8; bi++ ){
		psKey[bj]=psKeyTable[bj]^psRB3[bj];
		bj--;
	}
}
//




//************************************************************
//18. TDESFun function
//************************************************************
//	input :	1. sDESInData(8bytes)
//			2. psKeyOriginal(16bytes)
//			3. bMode(1)encrypt  : input CDES_ENCRYPT
//					(2)decrypt  : input CDES_DECRYPT
//	return:	
//			1. psDESOutData( 8bytes)
//============================================================
BOOL TDESFun( BYTE *psTDESOutData,BYTE *psTDESInData,BYTE *psTDESKey,BYTE bMode )
{
	BYTE sKey8[8];
	//
	if( bMode==CDES_ENCRYPT ){
		MyLibMemCpy( sKey8,psTDESKey,8 );
		DESFun( psTDESOutData,psTDESInData,sKey8,CDES_ENCRYPT );
		MyLibMemCpy( psTDESInData,psTDESOutData,8 );
		//
		MyLibMemCpy( sKey8,&(psTDESKey[8]),8 );
		DESFun( psTDESOutData,psTDESInData,sKey8,CDES_DECRYPT );
		MyLibMemCpy( psTDESInData,psTDESOutData,8 );
		//
		MyLibMemCpy( sKey8,psTDESKey,8 );
		DESFun( psTDESOutData,psTDESInData,sKey8,CDES_ENCRYPT );

	}
	else if( bMode==CDES_DECRYPT ){
		MyLibMemCpy( sKey8,psTDESKey,8 );
		DESFun( psTDESOutData,psTDESInData,sKey8,CDES_DECRYPT );
		MyLibMemCpy( psTDESInData,psTDESOutData,8 );
		//
		MyLibMemCpy( sKey8,&(psTDESKey[8]),8 );
		DESFun( psTDESOutData,psTDESInData,sKey8,CDES_ENCRYPT );
		MyLibMemCpy( psTDESInData,psTDESOutData,8 );
		//
		MyLibMemCpy( sKey8,psTDESKey,8 );
		DESFun( psTDESOutData,psTDESInData,sKey8,CDES_DECRYPT );
	}
	else
		return FALSE;
	//
	return TRUE;
}


//////////////////////////////////////////////////
//////////////////////////////////////////////////
void MyLibMemCpy( BYTE *des,BYTE *src,BYTE bSize )
{
	BYTE bi;
	//
	for( bi=0; bi<bSize; bi++ )
		des[bi]=src[bi];

}


//////////////////////////////////////////////////
void MyLibMemSet( BYTE *des,BYTE bFill,BYTE bSize )
{
	BYTE bi;
	//
	for( bi=0; bi<bSize; bi++ )
		des[bi]=bFill;

}

/*********** the end of file ********************/
