// DeviceEvent.h : header file
// Last update date : 2007.04.06 
// Last update date : 2008.04.23 - DeviceChangeMsgHandler, DeviceMultiChangeMsgHandler fix bug.
// Last update date : 2008.05.29 - add DeviceChangeMsgHandlerEx()

#if !defined(__DEVICE_EVENT_HEADER__)
#define __DEVICE_EVENT_HEADER__

#include "Dbt.h"

////////////////////////////////////////////////////////////
//the definition of return value of DeviceChangeMsgHandler()
#define	DEV_CHANGE_PLUGIN_WANTED_USB				1
#define	DEV_CHANGE_PLUGOUT_WANTED_USB				2
#define	DEV_CHANGE_PLUGIN_THE_OTHER_USB				3
#define	DEV_CHANGE_PLUGOUT_THE_OTHER_USB			4
#define	DEV_CHANGE_WILL_BE_PLUGOUT_WANTED_DEVICE	6
#define	DEV_CHANGE_WILL_BE_PLUGOUT_THE_OTHER_DEVICE	8


#define	DEV_CHANGE_NO_NEED_EVENT					100



/////////////////////////////////////////////////////////////////////////////
//exported function
//register user device for device-change-notification
HDEVNOTIFY RegisterEventDevice( HANDLE hTargetWnd,GUID InterfaceClassGuid );

//register user device-handle for device-change-notification
HDEVNOTIFY RegisterEventDeviceHandle( HANDLE hTargetWnd,HANDLE hDev,HDEVNOTIFY hDevnotify );

//unregister device of RegisterEventDevice()
BOOL UnRegisterEventDevice( HDEVNOTIFY hNotify );

//unregister device of RegisterEventDeviceHandle()
BOOL UnRegisterEventDeviceHandle( HDEVNOTIFY hNotify );

//this function is called on WM_DEVICECHANGE message handler
int DeviceChangeMsgHandler( UINT nEventType, DWORD dwData,WORD wVID,WORD wPID,GUID ClassGuid );

//this function is called on WM_DEVICECHANGE message handler with Multi device(2007.04.06)
int DeviceMultiChangeMsgHandler( UINT nEventType, DWORD dwData,WORD *pwVID,WORD *pwPID,int nDevCnt,GUID ClassGuid );

//this function is called on WM_DEVICECHANGE message handler . return device name
int DeviceChangeMsgHandlerEx( UINT nEventType, DWORD dwData,WORD wVID,WORD wPID,GUID ClassGuid,HANDLE hWantedDev );

/////////////////////////////////////////////////////////////////////////////
//internal function
//Get string type of VID & PID from device path
//this function is called in IsWantUSBDevice()
BOOL ParsingUSBDevicePath(TCHAR* psVID,TCHAR *psPID,TCHAR *pDevicePath);

//checks whether the given VID & PID exsit in the given device path  
BOOL IsWantUSBDevice(TCHAR *psDevicePath,WORD wVID,WORD wPID);

#endif//#if !defined(__DEVICE_EVENT_HEADER__)
