
// tp_fixbuzzerDoc.cpp : Ctp_fixbuzzerDoc Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "tp_fixbuzzer.h"
#endif

#include "tp_fixbuzzerDoc.h"
#include "CntrItem.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Ctp_fixbuzzerDoc

IMPLEMENT_DYNCREATE(Ctp_fixbuzzerDoc, CRichEditDoc)

BEGIN_MESSAGE_MAP(Ctp_fixbuzzerDoc, CRichEditDoc)
	// �⺻ OLE �����̳� ������ Ȱ��ȭ�մϴ�.
	ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_LINKS, &CRichEditDoc::OnUpdateEditLinksMenu)
	ON_COMMAND(ID_OLE_EDIT_LINKS, &CRichEditDoc::OnEditLinks)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OLE_VERB_FIRST, ID_OLE_VERB_LAST, &CRichEditDoc::OnUpdateObjectVerbMenu)
END_MESSAGE_MAP()


// Ctp_fixbuzzerDoc ����/�Ҹ�

Ctp_fixbuzzerDoc::Ctp_fixbuzzerDoc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.

}

Ctp_fixbuzzerDoc::~Ctp_fixbuzzerDoc()
{
}

BOOL Ctp_fixbuzzerDoc::OnNewDocument()
{
	return TRUE;

	if (!CRichEditDoc::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	return TRUE;
}

CRichEditCntrItem* Ctp_fixbuzzerDoc::CreateClientItem(REOBJECT* preo) const
{
	return new Ctp_fixbuzzerCntrItem(preo, const_cast<Ctp_fixbuzzerDoc*>(this));
}




// Ctp_fixbuzzerDoc serialization

void Ctp_fixbuzzerDoc::Serialize(CArchive& ar)
{
	return;

	if (ar.IsStoring())
	{
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
	}

	// �⺻ Ŭ������ CRichEditDoc�� ȣ���ϸ� �����̳� ������ COleClientItem ��ü�� ���� serialization��
	//  Ȱ��ȭ�˴ϴ�.
	// TODO: �ؽ�Ʈ�� serialize�� ��� CRichEditDoc::m_bRTF = FALSE�� �����մϴ�.
	//CRichEditDoc::Serialize(ar);
}

#ifdef SHARED_HANDLERS

// ����� �׸��� �����մϴ�.
void Ctp_fixbuzzerDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// ������ �����͸� �׸����� �� �ڵ带 �����Ͻʽÿ�.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// �˻� ó���⸦ �����մϴ�.
void Ctp_fixbuzzerDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ������ �����Ϳ��� �˻� �������� �����մϴ�.
	// ������ �κ��� ";"�� ���еǾ�� �մϴ�.

	// ��: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void Ctp_fixbuzzerDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// Ctp_fixbuzzerDoc ����

#ifdef _DEBUG
void Ctp_fixbuzzerDoc::AssertValid() const
{
	CRichEditDoc::AssertValid();
}

void Ctp_fixbuzzerDoc::Dump(CDumpContext& dc) const
{
	CRichEditDoc::Dump(dc);
}
#endif //_DEBUG


// Ctp_fixbuzzerDoc ���
