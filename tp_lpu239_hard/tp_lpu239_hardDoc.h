
// tp_lpu239_hardDoc.h : interface of the Ctp_lpu239_hardDoc class
//


#pragma once

#include <tg_dev_manager.h>
#include <tg_dev_hid_lpu239.h>
#include <xmllite.h>

#include <map>

class Ctp_lpu239_hardDoc : public CDocument
{
protected: // create from serialization only
	Ctp_lpu239_hardDoc();
	DECLARE_DYNCREATE(Ctp_lpu239_hardDoc)

// Attributes
public:
	typedef	std::map< _tstring, CDev::typeUid > typeStrToUid; 
// Operations
public:
	void reset_StrToUid_Map()
	{
		m_mapStrToUid.clear();
	}

	void insert_StrToUid_Map( const _tstring & sId, const CDev::typeUid & uid )
	{
		m_mapStrToUid[sId] = uid;
	}

	void get_Uid_FromMap( const _tstring & sId, CDev::typeUid & Outuid )
	{
		typeStrToUid::iterator it = m_mapStrToUid.find( sId );

		if( it != m_mapStrToUid.end() ){
			Outuid = it->second;
		}
		else{
			Outuid.resize(0);
		}
	}

	CDev::typeUid get_current_uid(){	return m_CurUid;	}
	void set_current_uid( const CDev::typeUid & uid )
	{
		m_CurUid = uid;
	}


	bool device_open();
	bool device_close();
	bool device_enter_config();
	bool device_leave_config();
	bool device_get_randum_number();
	bool device_external_authentication( CDev::typeBuffer & vRn, unsigned int & nFail );
	bool device_change_authentication_key0(  const CDev::typeBuffer & vKey );
	bool device_change_authentication_key1(  const CDev::typeBuffer & vKey );
	bool device_change_ksn( const CDev::typeBuffer & vksn );
	bool device_change_ipek0(  const CDev::typeBuffer & vKey );
	bool device_change_ipek1( const CDev::typeBuffer & vKey );

	void DisplayRightInfoView( LPCTSTR sInfo );

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~Ctp_lpu239_hardDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	

private:
	typeStrToUid m_mapStrToUid;
	CDev::typeUid m_CurUid;
	CDev::typeBuffer m_vRn;	//randum number.
	CDev::typeBuffer m_vAuthKey[2];
	CDev::typeBuffer m_vEnKey[2];
	CDev::typeBuffer m_vKsn;

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnToolGetRamdumNumber();
	afx_msg void OnToolChangeAuthenticationKey();
	afx_msg void OnToolChangeIpek();
	afx_msg void OnToolChangeKsn();
	afx_msg void OnToolEnterConfigMode();
	afx_msg void OnToolExternalAuthentication();
	afx_msg void OnToolLeaveConfigMode();
	afx_msg void OnInputAuthenticationKey();
	afx_msg void OnInputKsn();
	afx_msg void OnInputEncryptionKey();
	afx_msg void OnToolReadUid();
	afx_msg void OnToolApply();
	afx_msg void OnToolGotoBootloader();
	afx_msg void OnToolChangeInterface();
};
