
// tp_lpu239_hard.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "tp_lpu239_hard.h"
#include "MainFrm.h"

#include "tp_lpu239_hardDoc.h"
#include "LeftView.h"

#include <tg_dev_manager.h>
#include <tg_dev_hid_lpu239.h>

#include <xmllite.h>

#include <algorithm>

using namespace std;
using namespace thirdgeneration;
using namespace device_elpusk;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ctp_lpu239_hardApp

BEGIN_MESSAGE_MAP(Ctp_lpu239_hardApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &Ctp_lpu239_hardApp::OnAppAbout)
	// Standard file based document commands
	//ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	//ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	ON_COMMAND(ID_FILE_NEW, &Ctp_lpu239_hardApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &Ctp_lpu239_hardApp::OnFileOpen)
END_MESSAGE_MAP()


// Ctp_lpu239_hardApp construction

Ctp_lpu239_hardApp::Ctp_lpu239_hardApp()
{
	m_bHiColorIcons = TRUE;

	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("elpusk.lpu239.tester.20141222"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only Ctp_lpu239_hardApp object

Ctp_lpu239_hardApp theApp;


// Ctp_lpu239_hardApp initialization

BOOL Ctp_lpu239_hardApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control	
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Elpusk"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)
	//LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,	RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(Ctp_lpu239_hardDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CLeftView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);


	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();
	return TRUE;
}

int Ctp_lpu239_hardApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// Ctp_lpu239_hardApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void Ctp_lpu239_hardApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// Ctp_lpu239_hardApp customization load/save methods

void Ctp_lpu239_hardApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void Ctp_lpu239_hardApp::LoadCustomState()
{
	CDevManager *pManager = CDevManager::getManager();
	CDevManager::typeDevicePathList DevPathList;
	pManager->UpdateDevicePathList()->getCurDevicePathList( DevPathList )->filter( DevPathList, 0x134b, 0x0210, 1 );

	if( DevPathList.empty() ){
		return;
	}

	// 관리를 원하는 device 를 생성하여 device manager에 관리를 요청한다.
	for_each( DevPathList.begin(), DevPathList.end(), [&]( const CDevManager::typeDevicePathList::value_type & path ){
		CDevHidLpu239 & msr = static_cast< CDevHidLpu239 & >(pManager->addManagedDev( CDevManager::typeDevPtr( new CDevHidLpu239( path ) ) ) );
		
		if( msr.open( true ) ){
			msr.df_enterConfig();
			msr.df_getSystemParameters();
			msr.df_leaveConfig();
			msr.close();
		}
	});

}

void Ctp_lpu239_hardApp::SaveCustomState()
{
}

// Ctp_lpu239_hardApp message handlers





void Ctp_lpu239_hardApp::OnFileNew()
{
	static bool bFirst( true );

	if( bFirst ){
		bFirst = false;
		CWinAppEx::OnFileNew();
	}
}


void Ctp_lpu239_hardApp::OnFileOpen()
{
	//
	CWinAppEx::OnFileOpen();
}
