
// CntrItem.cpp : Ctp_fixbuzzerCntrItem Ŭ������ ����
//

#include "stdafx.h"
#include "tp_fixbuzzer.h"

#include "tp_fixbuzzerDoc.h"
#include "tp_fixbuzzerView.h"
#include "CntrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ctp_fixbuzzerCntrItem ����

IMPLEMENT_SERIAL(Ctp_fixbuzzerCntrItem, CRichEditCntrItem, 0)

Ctp_fixbuzzerCntrItem::Ctp_fixbuzzerCntrItem(REOBJECT* preo, Ctp_fixbuzzerDoc* pContainer)
	: CRichEditCntrItem(preo, pContainer)
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.
}

Ctp_fixbuzzerCntrItem::~Ctp_fixbuzzerCntrItem()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
}


// Ctp_fixbuzzerCntrItem ����

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

