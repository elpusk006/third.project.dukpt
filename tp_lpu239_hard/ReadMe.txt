================================================================================
    MICROSOFT FOUNDATION CLASS LIBRARY : tp_lpu239_hard Project Overview
===============================================================================

The application wizard has created this tp_lpu239_hard application for
you.  This application not only demonstrates the basics of using the Microsoft
Foundation Classes but is also a starting point for writing your application.

This file contains a summary of what you will find in each of the files that
make up your tp_lpu239_hard application.

tp_lpu239_hard.vcxproj
    This is the main project file for VC++ projects generated using an application wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    application wizard.

tp_lpu239_hard.vcxproj.filters
    This is the filters file for VC++ projects generated using an Application Wizard. 
    It contains information about the association between the files in your project 
    and the filters. This association is used in the IDE to show grouping of files with
    similar extensions under a specific node (for e.g. ".cpp" files are associated with the
    "Source Files" filter).

tp_lpu239_hard.h
    This is the main header file for the application.  It includes other
    project specific headers (including Resource.h) and declares the
    Ctp_lpu239_hardApp application class.

tp_lpu239_hard.cpp
    This is the main application source file that contains the application
    class Ctp_lpu239_hardApp.

tp_lpu239_hard.rc
    This is a listing of all of the Microsoft Windows resources that the
    program uses.  It includes the icons, bitmaps, and cursors that are stored
    in the RES subdirectory.  This file can be directly edited in Microsoft
    Visual C++. Your project resources are in 1033.

res\tp_lpu239_hard.ico
    This is an icon file, which is used as the application's icon.  This
    icon is included by the main resource file tp_lpu239_hard.rc.

res\tp_lpu239_hard.rc2
    This file contains resources that are not edited by Microsoft
    Visual C++. You should place all resources not editable by
    the resource editor in this file.

tp_lpu239_hard.reg
    This is an example .reg file that shows you the kind of registration
    settings the framework will set for you.  You can use this as a .reg
    file to go along with your application or just delete it and rely
    on the default RegisterShellFileTypes registration.


/////////////////////////////////////////////////////////////////////////////

For the main frame window:
    Windows Explorer Style: The project will include a Windows Explorer-like
    interface, with two frames.

MainFrm.h, MainFrm.cpp
    These files contain the frame class CMainFrame, which is derived from
    CFrameWnd and controls all SDI frame features.

LeftView.h, LeftView.cpp
    These files contain the left frame class CLeftView, which is derived from
    CTreeView.

res\Toolbar.bmp
    This bitmap file is used to create tiled images for the toolbar.
    The initial toolbar and status bar are constructed in the CMainFrame
    class. Edit this toolbar bitmap using the resource editor, and
    update the IDR_MAINFRAME TOOLBAR array in tp_lpu239_hard.rc to add
    toolbar buttons.
/////////////////////////////////////////////////////////////////////////////

The application wizard creates one document type and one view:

tp_lpu239_hardDoc.h, tp_lpu239_hardDoc.cpp - the document
    These files contain your Ctp_lpu239_hardDoc class.  Edit these files to
    add your special document data and to implement file saving and loading
    (via Ctp_lpu239_hardDoc::Serialize).
    The Document will have the following strings:
        File extension:      emi
        File type ID:        tplpu239hard.Document
        Main frame caption:  tester for lpu239( hard )
        Doc type name:       tp_lpu239_hard
        Filter name:         elpusk msr ini Files (*.emi)
        File new short name: tp_lpu239_hard
        File type long name: tp_lpu239_hard.Document

tp_lpu239_hardView.h, tp_lpu239_hardView.cpp - the view of the document
    These files contain your Ctp_lpu239_hardView class.
    Ctp_lpu239_hardView objects are used to view Ctp_lpu239_hardDoc objects.





/////////////////////////////////////////////////////////////////////////////

Other Features:

ActiveX Controls
    The application includes support to use ActiveX controls.

/////////////////////////////////////////////////////////////////////////////

Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named tp_lpu239_hard.pch and a precompiled types file named StdAfx.obj.

Resource.h
    This is the standard header file, which defines new resource IDs.
    Microsoft Visual C++ reads and updates this file.

tp_lpu239_hard.manifest
	Application manifest files are used by Windows XP to describe an applications
	dependency on specific versions of Side-by-Side assemblies. The loader uses this
	information to load the appropriate assembly from the assembly cache or private
	from the application. The Application manifest  maybe included for redistribution
	as an external .manifest file that is installed in the same folder as the application
	executable or it may be included in the executable in the form of a resource.
/////////////////////////////////////////////////////////////////////////////

Other notes:

The application wizard uses "TODO:" to indicate parts of the source code you
should add to or customize.

If your application uses MFC in a shared DLL, you will need
to redistribute the MFC DLLs. If your application is in a language
other than the operating system's locale, you will also have to
redistribute the corresponding localized resources MFC100XXX.DLL.
For more information on both of these topics, please see the section on
redistributing Visual C++ applications in MSDN documentation.

/////////////////////////////////////////////////////////////////////////////
