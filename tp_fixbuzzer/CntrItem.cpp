
// CntrItem.cpp : Ctp_fixbuzzerCntrItem 클래스의 구현
//

#include "stdafx.h"
#include "tp_fixbuzzer.h"

#include "tp_fixbuzzerDoc.h"
#include "tp_fixbuzzerView.h"
#include "CntrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ctp_fixbuzzerCntrItem 구현

IMPLEMENT_SERIAL(Ctp_fixbuzzerCntrItem, CRichEditCntrItem, 0)

Ctp_fixbuzzerCntrItem::Ctp_fixbuzzerCntrItem(REOBJECT* preo, Ctp_fixbuzzerDoc* pContainer)
	: CRichEditCntrItem(preo, pContainer)
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.
}

Ctp_fixbuzzerCntrItem::~Ctp_fixbuzzerCntrItem()
{
	// TODO: 여기에 정리 코드를 추가합니다.
}


// Ctp_fixbuzzerCntrItem 진단

#ifdef _DEBUG
void Ctp_fixbuzzerCntrItem::AssertValid() const
{
	CRichEditCntrItem::AssertValid();
}

void Ctp_fixbuzzerCntrItem::Dump(CDumpContext& dc) const
{
	CRichEditCntrItem::Dump(dc);
}
#endif

