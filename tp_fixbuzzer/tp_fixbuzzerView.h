
// tp_fixbuzzerView.h : Ctp_fixbuzzerView 클래스의 인터페이스
//

#pragma once

#include "DeviceEvent.h"


#define	FRQ_DEV_IN_OUT			2400	//2.4KHz
#define	DUR_DEV_IN_OUT			500	//Duration of FRQ_DEV_IN_OUT(msec)


#include <tg_dev_manager.h>
#include <tg_dev_hid_lpu237.h>

using namespace thirdgeneration;
using namespace device_elpusk;

class Ctp_fixbuzzerCntrItem;

class Ctp_fixbuzzerView : public CRichEditView
{
protected: // serialization에서만 만들어집니다.
	Ctp_fixbuzzerView();
	DECLARE_DYNCREATE(Ctp_fixbuzzerView)

// 특성입니다.
public:
	Ctp_fixbuzzerDoc* GetDocument() const;

// 작업입니다.
private:
	BOOL RegisterDevice();
	void AddInformation( LPCTSTR sInfo, COLORREF col = RGB(0,0,0) );
	bool _GetDevicePathList( CDevManager::typeDevicePathList & DevPathList, CString & sOutMsg );
	bool _FixBuzzer(  CString & sOutMsg );

	HDEVNOTIFY m_diNotifyHandle;
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.

// 구현입니다.
public:
	virtual ~Ctp_fixbuzzerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnDestroy();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileSave();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSaveAs();
	afx_msg void OnFileNew();
	afx_msg void OnClose();
	afx_msg BOOL OnDeviceChange( UINT nEventType, DWORD dwData );
};

#ifndef _DEBUG  // tp_fixbuzzerView.cpp의 디버그 버전
inline Ctp_fixbuzzerDoc* Ctp_fixbuzzerView::GetDocument() const
   { return reinterpret_cast<Ctp_fixbuzzerDoc*>(m_pDocument); }
#endif

