
// CntrItem.h : Ctp_fixbuzzerCntrItem Ŭ������ �������̽�
//

#pragma once

class Ctp_fixbuzzerDoc;
class Ctp_fixbuzzerView;

class Ctp_fixbuzzerCntrItem : public CRichEditCntrItem
{
	DECLARE_SERIAL(Ctp_fixbuzzerCntrItem)

// �������Դϴ�.
public:
	Ctp_fixbuzzerCntrItem(REOBJECT* preo = NULL, Ctp_fixbuzzerDoc* pContainer = NULL);
		// ����: IMPLEMENT_SERIALIZE�� Ȱ��ȭ�ϱ� ���� pContainer�� NULL�� ���� �� �ֽ��ϴ�.
		//  IMPLEMENT_SERIALIZE�� �����ڿ� �μ��� �ʿ� ���� Ŭ������ �䱸�մϴ�.
		//  �Ϲ������� OLE �׸��� NULL�� �ƴ� ���� �����͸� ����Ͽ�
		//  �����˴ϴ�.

// Ư���Դϴ�.
public:
	Ctp_fixbuzzerDoc* GetDocument()
		{ return reinterpret_cast<Ctp_fixbuzzerDoc*>(CRichEditCntrItem::GetDocument()); }
	Ctp_fixbuzzerView* GetActiveView()
		{ return reinterpret_cast<Ctp_fixbuzzerView*>(CRichEditCntrItem::GetActiveView()); }

// �����Դϴ�.
public:
	~Ctp_fixbuzzerCntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

