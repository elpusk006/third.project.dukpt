
// CntrItem.h : Ctp_fixbuzzerCntrItem 클래스의 인터페이스
//

#pragma once

class Ctp_fixbuzzerDoc;
class Ctp_fixbuzzerView;

class Ctp_fixbuzzerCntrItem : public CRichEditCntrItem
{
	DECLARE_SERIAL(Ctp_fixbuzzerCntrItem)

// 생성자입니다.
public:
	Ctp_fixbuzzerCntrItem(REOBJECT* preo = NULL, Ctp_fixbuzzerDoc* pContainer = NULL);
		// 참고: IMPLEMENT_SERIALIZE를 활성화하기 위해 pContainer를 NULL로 만들 수 있습니다.
		//  IMPLEMENT_SERIALIZE는 생성자에 인수가 필요 없는 클래스를 요구합니다.
		//  일반적으로 OLE 항목은 NULL이 아닌 문서 포인터를 사용하여
		//  생성됩니다.

// 특성입니다.
public:
	Ctp_fixbuzzerDoc* GetDocument()
		{ return reinterpret_cast<Ctp_fixbuzzerDoc*>(CRichEditCntrItem::GetDocument()); }
	Ctp_fixbuzzerView* GetActiveView()
		{ return reinterpret_cast<Ctp_fixbuzzerView*>(CRichEditCntrItem::GetActiveView()); }

// 구현입니다.
public:
	~Ctp_fixbuzzerCntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

