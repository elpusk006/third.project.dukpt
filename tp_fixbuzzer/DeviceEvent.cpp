//////////////////////////////////////////////////////////////
// DeviceEvent.cpp : implementation file

#include "stdafx.h"
#include "DeviceEvent.h"
#include "tchar.h"

//register user device for device-change-notification
HDEVNOTIFY RegisterEventDevice( HANDLE hTargetWnd,GUID InterfaceClassGuid )
{
	////////////////////////////////////
	// interface class.
	HDEVNOTIFY diNotifyHandle;
	DEV_BROADCAST_DEVICEINTERFACE broadcastInterface;

	ZeroMemory(&broadcastInterface, sizeof(broadcastInterface));

	broadcastInterface.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
	broadcastInterface.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	broadcastInterface.dbcc_classguid = InterfaceClassGuid;

	
	diNotifyHandle = RegisterDeviceNotification(
		hTargetWnd,
		&broadcastInterface,
		DEVICE_NOTIFY_WINDOW_HANDLE
		);

	return diNotifyHandle;
}

//register user device-handle for device-change-notification
HDEVNOTIFY RegisterEventDeviceHandle( HANDLE hTargetWnd,HANDLE hDev,HDEVNOTIFY hDevnotify )
{
	////////////////////////////////////
	// interface class.
	HDEVNOTIFY diNotifyHandle;
	DEV_BROADCAST_HANDLE filter;

	ZeroMemory(&filter, sizeof(DEV_BROADCAST_HANDLE));

	filter.dbch_size = sizeof(DEV_BROADCAST_HANDLE);
	filter.dbch_devicetype = DBT_DEVTYP_HANDLE;
	filter.dbch_handle = hDev;
	//filter.dbch_hdevnotify = hDevnotify;

	diNotifyHandle = RegisterDeviceNotification(
		hTargetWnd,
		&filter,
		DEVICE_NOTIFY_WINDOW_HANDLE
		);

	return diNotifyHandle;
}

//unregister device of RegisterEventDevice()
BOOL UnRegisterEventDevice( HDEVNOTIFY hNotify )
{
	return UnregisterDeviceNotification( hNotify );
}

//unregister device of RegisterEventDeviceHandle()
BOOL UnRegisterEventDeviceHandle( HDEVNOTIFY hNotify )
{
	return UnregisterDeviceNotification( hNotify );
}

//this function is called on WM_DEVICECHANGE message handler
int DeviceChangeMsgHandler( UINT nEventType, DWORD dwData,WORD wVID,WORD wPID,GUID ClassGuid )
{
	PDEV_BROADCAST_HDR broadcastHdr;
	PDEV_BROADCAST_DEVICEINTERFACE broadcastInterface;
	int nReturn=DEV_CHANGE_NO_NEED_EVENT;

	switch(nEventType){
		case DBT_DEVICEARRIVAL:
			broadcastHdr = (PDEV_BROADCAST_HDR) dwData;

			if( broadcastHdr->dbch_devicetype==DBT_DEVTYP_DEVICEINTERFACE ) {
				broadcastInterface = (PDEV_BROADCAST_DEVICEINTERFACE) dwData;
				if( broadcastInterface->dbcc_classguid==ClassGuid ){
					if( IsWantUSBDevice( (LPTSTR)(broadcastInterface->dbcc_name),wVID,wPID ) ){
						//"Plugin pinpad device"
						nReturn=DEV_CHANGE_PLUGIN_WANTED_USB;
					}
					else//"USB device(except pinpad)Plugin"
						nReturn=DEV_CHANGE_PLUGIN_THE_OTHER_USB;
				}
				else{
					if( IsWantUSBDevice( (LPTSTR)(broadcastInterface->dbcc_name),wVID,wPID ) ){
						//"Plugin pinpad device"
						nReturn=DEV_CHANGE_PLUGIN_WANTED_USB;
					}
					else//"USB device(except pinpad)Plugin"
						nReturn=DEV_CHANGE_PLUGIN_THE_OTHER_USB;
				}
			}
			break;
		case DBT_DEVICEREMOVECOMPLETE:
			broadcastHdr = (PDEV_BROADCAST_HDR) dwData;
			if( broadcastHdr->dbch_devicetype==DBT_DEVTYP_DEVICEINTERFACE ){
				broadcastInterface = (PDEV_BROADCAST_DEVICEINTERFACE) dwData;

				if( IsWantUSBDevice( (LPTSTR)(broadcastInterface->dbcc_name),wVID,wPID ) ){
					//"Plugout pinpad device"
					nReturn=DEV_CHANGE_PLUGOUT_WANTED_USB;
				}
				else//"USB device(except pinpad)Plugout"
					nReturn=DEV_CHANGE_PLUGOUT_THE_OTHER_USB;
			}
			break;
		case DBT_DEVICEQUERYREMOVE:break;//Permission to remove a device is requested. Any application can deny this request and cancel the removal.
		case DBT_DEVICEQUERYREMOVEFAILED:break;//Request to remove a device has been canceled.
		case DBT_DEVICEREMOVEPENDING:break;//Device is about to be removed. Cannot be denied.
		case DBT_DEVICETYPESPECIFIC:break;//Device-specific event.
		case DBT_CONFIGCHANGED:break;//Current configuration has changed.
		case DBT_DEVNODES_CHANGED:break;//Device node has changed. 
		default:break;
	}//end switch

	return nReturn;
}

//this function is called on WM_DEVICECHANGE message handler . return will be removed handle.
int DeviceChangeMsgHandlerEx( UINT nEventType, DWORD dwData,WORD wVID,WORD wPID,GUID ClassGuid,HANDLE hWantedDev )
{
	PDEV_BROADCAST_HDR broadcastHdr;
	PDEV_BROADCAST_DEVICEINTERFACE broadcastInterface;
	int nReturn=DEV_CHANGE_NO_NEED_EVENT;
	PDEV_BROADCAST_HANDLE pDBHandle;

	switch(nEventType){
		case DBT_DEVICEARRIVAL:
			broadcastHdr = (PDEV_BROADCAST_HDR) dwData;

			if( broadcastHdr->dbch_devicetype==DBT_DEVTYP_DEVICEINTERFACE ) {
				broadcastInterface = (PDEV_BROADCAST_DEVICEINTERFACE) dwData;

				if( broadcastInterface->dbcc_classguid==ClassGuid ){
					if( IsWantUSBDevice( (LPTSTR)(broadcastInterface->dbcc_name),wVID,wPID ) ){
						//"Plugin pinpad device"
						nReturn=DEV_CHANGE_PLUGIN_WANTED_USB;
					}
					else//"USB device(except pinpad)Plugin"
						nReturn=DEV_CHANGE_PLUGIN_THE_OTHER_USB;
				}
				else{
					if( IsWantUSBDevice( (LPTSTR)(broadcastInterface->dbcc_name),wVID,wPID ) ){
						//"Plugin pinpad device"
						nReturn=DEV_CHANGE_PLUGIN_WANTED_USB;
					}
					else//"USB device(except pinpad)Plugin"
						nReturn=DEV_CHANGE_PLUGIN_THE_OTHER_USB;
				}
			}
			break;
		case DBT_DEVICEREMOVECOMPLETE:
			broadcastHdr = (PDEV_BROADCAST_HDR) dwData;
			if( broadcastHdr->dbch_devicetype==DBT_DEVTYP_DEVICEINTERFACE ){
				broadcastInterface = (PDEV_BROADCAST_DEVICEINTERFACE) dwData;

				if( IsWantUSBDevice( (LPTSTR)(broadcastInterface->dbcc_name),wVID,wPID ) ){
					//"Plugout pinpad device"
					nReturn=DEV_CHANGE_PLUGOUT_WANTED_USB;
				}
				else//"USB device(except pinpad)Plugout"
					nReturn=DEV_CHANGE_PLUGOUT_THE_OTHER_USB;
			}
			else if( broadcastHdr->dbch_devicetype==DBT_DEVTYP_HANDLE ){
				pDBHandle = (PDEV_BROADCAST_HANDLE) broadcastHdr;

				if( pDBHandle->dbch_handle==hWantedDev ){
					nReturn=DEV_CHANGE_WILL_BE_PLUGOUT_WANTED_DEVICE;
				}
				else{
					nReturn=DEV_CHANGE_WILL_BE_PLUGOUT_THE_OTHER_DEVICE;
				}
			}
			break;
		case DBT_DEVICEQUERYREMOVE://Permission to remove a device is requested. Any application can deny this request and cancel the removal.
			//device will be removed on system.. you must close device handle.
			//If the operating system forcefully removes of a device, it may not send a DBT_DEVICEQUERYREMOVE message before doing so.
			broadcastHdr = (PDEV_BROADCAST_HDR) dwData;

			if(broadcastHdr->dbch_devicetype==DBT_DEVTYP_HANDLE){

				pDBHandle = (PDEV_BROADCAST_HANDLE) broadcastHdr;

				if( pDBHandle->dbch_handle==hWantedDev ){
					nReturn=DEV_CHANGE_WILL_BE_PLUGOUT_WANTED_DEVICE;
				}
				else{
					nReturn=DEV_CHANGE_WILL_BE_PLUGOUT_THE_OTHER_DEVICE;
				}
			}
			break;
		case DBT_DEVICEQUERYREMOVEFAILED:break;//Request to remove a device has been canceled.
		case DBT_DEVICEREMOVEPENDING:break;//Device is about to be removed. Cannot be denied.
		case DBT_DEVICETYPESPECIFIC:break;//Device-specific event.
		case DBT_CONFIGCHANGED:break;//Current configuration has changed.
		case DBT_DEVNODES_CHANGED:break;//Device node has changed. 
		default:break;
	}//end switch

	return nReturn;
}

BOOL ParsingUSBDevicePath(TCHAR *pDevicePath,TCHAR* psVID,TCHAR *psPID)
{
	TCHAR seps[]=_T("\\#&");
	TCHAR *token;
	int i=0;
	//
	if( pDevicePath==NULL || psVID==NULL || psPID==NULL )
		return FALSE;

	/* Establish string and get the first token: */
	token = _tcstok( pDevicePath, seps );

	while( token != NULL ){
		/* While there are tokens in "pDevicePath" */
		switch( i ){
			case 1://may be USB device is "USB"
				//Perform a lowercase comparison of strings
				if( _tcsicmp( (LPCTSTR)token,_T("USB") )!=0 )
					return FALSE;//this device is none-usb device.
				break;
			case 2://may be Vid string(Vid_????)
				if( token[0]!='V' && token[0]!='v' )
					return FALSE;
				if( token[1]!='I' && token[1]!='i' )
					return FALSE;
				if( token[2]!='D' && token[2]!='d' )
					return FALSE;
				if( token[3]!='_' )
					return FALSE;
				//
				if( _tcslen( token )!=8 )
					return FALSE;
				//Copy vender ID
				_tcscpy( psVID,&(token[4]) );
				break;
			case 3://may be Pid string(Pid_????)
				if( token[0]!='P' && token[0]!='p' )
					return FALSE;
				if( token[1]!='I' && token[1]!='i' )
					return FALSE;
				if( token[2]!='D' && token[2]!='d' )
					return FALSE;
				if( token[3]!='_' )
					return FALSE;
				//
				if( _tcslen( token )!=8 )
					return FALSE;
				//Copy product ID
				_tcscpy( psPID,&(token[4]) );
				return TRUE;
			default:
				break;
		}//end switch
		//m_ListReport.AddString( (LPCTSTR)token );
		//m_ListReport.SetCurSel( m_ListReport.GetCount()-1 );
		/* Get next token: */
		token = _tcstok( NULL, seps );
		i++;
	}
	return TRUE;
}

BOOL IsWantUSBDevice(TCHAR *psDevicePath,WORD wVID,WORD wPID)
{
	TCHAR sVID[8];
	TCHAR sPID[8];
	TCHAR *psID;
	WORD wPathVID,wPathPID;
	BYTE *pcID;
	BYTE cItem;
	int i,j;
	TCHAR sDevPath[256];

	_tcscpy( sDevPath,psDevicePath );
	//
	if( !ParsingUSBDevicePath( sDevPath,(TCHAR*)sVID,(TCHAR*)sPID ) )
		return FALSE;
	//convert lower case
	_tcslwr( (TCHAR*)sVID );	_tcslwr( (TCHAR*)sPID );
	
	wPathVID=wPathPID=0x0000;
	pcID=(BYTE*)(&wPathVID);
	pcID++;
	psID=sVID;
	//convert VID and PID string type to VID and PID word type.
	for( j=0; j<2; j++ ){
		for( i=0; i<4; i++ ){
			//high nibble
			if( psID[i]>='0' && psID[i]<='9' ){
				cItem=(BYTE)(psID[i]-'0');
				cItem=cItem<<4;
			}
			else if( psID[i]>='a' && psID[i]<='f' ){
				cItem=(BYTE)(psID[i]-'a'+0x0A);
				cItem=cItem<<4;
			}
			else
				return FALSE;

			i++;
			//low nibble
			if( psID[i]>='0' && psID[i]<='9' ){
				cItem+=(BYTE)(psID[i]-'0');
			}
			else if( psID[i]>='a' && psID[i]<='f' ){
				cItem+=(BYTE)(psID[i]-'a'+0x0A);
			}
			else
				return FALSE;
			*pcID=cItem;
			pcID--;
		}//end for
		//
		pcID=(BYTE*)(&wPathPID);
		pcID++;
		psID=sPID;
	}//end for

	if( wVID==wPathVID && wPID==wPathPID )
		return TRUE;
	else
		return FALSE;
}

//this function is called on WM_DEVICECHANGE message handler with Multi device(2007.04.06)
int DeviceMultiChangeMsgHandler( UINT nEventType, DWORD dwData,WORD *pwVID,WORD *pwPID,int nDevCnt,GUID ClassGuid )
{
	PDEV_BROADCAST_HDR broadcastHdr;
	PDEV_BROADCAST_DEVICEINTERFACE broadcastInterface;
	int nReturn=DEV_CHANGE_NO_NEED_EVENT;
	int i;

	switch(nEventType){
		case DBT_DEVICEARRIVAL:
			broadcastHdr = (PDEV_BROADCAST_HDR) dwData;

			if( broadcastHdr->dbch_devicetype==DBT_DEVTYP_DEVICEINTERFACE ) {
				broadcastInterface = (PDEV_BROADCAST_DEVICEINTERFACE) dwData;
				if( broadcastInterface->dbcc_classguid==ClassGuid ){
					for( i=0; i<nDevCnt; i++ ){
						if( IsWantUSBDevice( (LPTSTR)(broadcastInterface->dbcc_name),pwVID[i],pwPID[i] ) ){
							//"Plugin pinpad device"
							nReturn=DEV_CHANGE_PLUGIN_WANTED_USB;
							break;
						}
					}//end for

					if( i==nDevCnt ){
						//"USB device(except pinpad)Plugin"
						nReturn=DEV_CHANGE_PLUGIN_THE_OTHER_USB;
					}
				}
				else{
					for( i=0; i<nDevCnt; i++ ){
						if( IsWantUSBDevice( (LPTSTR)(broadcastInterface->dbcc_name),pwVID[i],pwPID[i] ) ){
							//"Plugin pinpad device"
							nReturn=DEV_CHANGE_PLUGIN_WANTED_USB;
							break;
						}
					}//end for

					if( i==nDevCnt ){
						//"USB device(except pinpad)Plugin"
						nReturn=DEV_CHANGE_PLUGIN_THE_OTHER_USB;
					}
				}
			}
			break;
		case DBT_DEVICEREMOVECOMPLETE:
			broadcastHdr = (PDEV_BROADCAST_HDR) dwData;
			if( broadcastHdr->dbch_devicetype==DBT_DEVTYP_DEVICEINTERFACE ){
				broadcastInterface = (PDEV_BROADCAST_DEVICEINTERFACE) dwData;

				for( i=0; i<nDevCnt; i++ ){
					if( IsWantUSBDevice( (LPTSTR)(broadcastInterface->dbcc_name),pwVID[i],pwPID[i] ) ){
						//"Plugout pinpad device"
						nReturn=DEV_CHANGE_PLUGOUT_WANTED_USB;
						break;
					}
				}//end for

				if( i==nDevCnt ){
					//"USB device(except pinpad)Plugout"
					nReturn=DEV_CHANGE_PLUGOUT_THE_OTHER_USB;
				}
			}
			break;
		case DBT_DEVICEQUERYREMOVE:break;//Permission to remove a device is requested. Any application can deny this request and cancel the removal.
		case DBT_DEVICEQUERYREMOVEFAILED:break;//Request to remove a device has been canceled.
		case DBT_DEVICEREMOVEPENDING:break;//Device is about to be removed. Cannot be denied.
		case DBT_DEVICETYPESPECIFIC:break;//Device-specific event.
		case DBT_CONFIGCHANGED:break;//Current configuration has changed.
		case DBT_DEVNODES_CHANGED:break;//Device node has changed. 
		default:break;
	}//end switch

	return nReturn;
}
