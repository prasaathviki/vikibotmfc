// vikibotDlg.cpp : implementation file
//

#include "stdafx.h"
#include "vikibot.h"
#include "vikibotDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

GUID GUID_INTERFACE_DEV = { 0x25dbce51, 0x6c8f, 0x4a72, 0x8a,0x6d,0xb5,0x4c,0x2b,0x4f,0xc8,0x35 };


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
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

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CvikibotDlg dialog




CvikibotDlg::CvikibotDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CvikibotDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CvikibotDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CvikibotDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_WM_POWERBROADCAST()
	//ON_WM_DEVICECHANGE()
	ON_MESSAGE(WM_DEVICECHANGE, OnMyDeviceChange)
	ON_BN_CLICKED(IDCANCEL, &CvikibotDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_SHOW, &CvikibotDlg::OnBnClickedBtnShow)
	ON_BN_CLICKED(IDOK, &CvikibotDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CvikibotDlg message handlers

BOOL CvikibotDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_TextSpeaker.Speak(L"hi viki");

	CTime t = CTime::GetCurrentTime();  

	if (t.GetHour() >= 5 && t.GetHour() < 12)
	{
		m_TextSpeaker.Speak(L"Good morning");
	}
	if (t.GetHour() >= 12 && t.GetHour() < 18)
	{
		m_TextSpeaker.Speak(L"Good afternoon");
	}
	if (t.GetHour() >= 18 && t.GetHour() <= 20)
	{
		m_TextSpeaker.Speak(L"Good evening");
	}

	/*Good morning:5 AM to 12 PM or 00:00 to 24:00
	Good afternoon: 12 PM to 6 PM (?)
	Good evening:6 PM to 10 PM
	Good night: 10 PM onwards (or when you go to sleep, in fact ,or say goodbye for the rest of the day)*/

	

	// Register for notifications about the power status.
	m_hPowerSchemeNotify = RegisterPowerSettingNotification ( m_hWnd, &GUID_POWERSCHEME_PERSONALITY, DEVICE_NOTIFY_WINDOW_HANDLE );

	if ( NULL == m_hPowerSchemeNotify )
		ATLTRACE("Failed to register for notification of power scheme changes!\n");

	m_hPowerSourceNotify = RegisterPowerSettingNotification ( m_hWnd, &GUID_ACDC_POWER_SOURCE, DEVICE_NOTIFY_WINDOW_HANDLE );

	if ( NULL == m_hPowerSourceNotify )
		ATLTRACE("Failed to register for notification of power source changes!\n");

	m_hBatteryPowerNotify = RegisterPowerSettingNotification ( m_hWnd, &GUID_BATTERY_PERCENTAGE_REMAINING, DEVICE_NOTIFY_WINDOW_HANDLE );

	if ( NULL == m_hBatteryPowerNotify )
		ATLTRACE("Failed to register for notification of battery power changes!\n");

	HDEVNOTIFY hDevNotify;
	DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;
	ZeroMemory( &NotificationFilter, sizeof(NotificationFilter) );
	NotificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
	NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	for(int i=0; i<sizeof(GUID_DEVINTERFACE_LIST)/sizeof(GUID); i++) {
		NotificationFilter.dbcc_classguid = GUID_DEVINTERFACE_LIST[i];
		hDevNotify = RegisterDeviceNotification(this->GetSafeHwnd(), &NotificationFilter, DEVICE_NOTIFY_WINDOW_HANDLE);
		if( !hDevNotify ) {
			AfxMessageBox(CString("Can't register device notification: "), MB_ICONEXCLAMATION);
			return FALSE;
		}
	}


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CvikibotDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CvikibotDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CvikibotDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CvikibotDlg::OnDestroy()
{
	m_TextSpeaker.Speak(L"Bye.");
	CTime t = CTime::GetCurrentTime();  
	if (t.GetHour() > 20)
	{
		m_TextSpeaker.Speak(L"Good night");
	}
	m_TextSpeaker.WaitTalkerToFinish();
	CDialog::OnDestroy();
	if ( NULL != m_hPowerSchemeNotify )
		UnregisterPowerSettingNotification ( m_hPowerSchemeNotify );

	if ( NULL != m_hPowerSourceNotify )
		UnregisterPowerSettingNotification ( m_hPowerSourceNotify );

	if ( NULL != m_hBatteryPowerNotify )
		UnregisterPowerSettingNotification ( m_hBatteryPowerNotify );
	
}

UINT CvikibotDlg::OnPowerBroadcast(UINT nPowerEvent, UINT nEventData)
{
	//return CDialog::OnPowerBroadcast(nPowerEvent, nEventData);

	UINT uRet = CDialog::OnPowerBroadcast(nPowerEvent, nEventData);

	if ( PBT_POWERSETTINGCHANGE != nPowerEvent )
	{
		return uRet;
	}

	POWERBROADCAST_SETTING* pps = (POWERBROADCAST_SETTING*) nEventData;

	if ( sizeof(GUID) == pps->DataLength &&	pps->PowerSetting == GUID_POWERSCHEME_PERSONALITY )
	{
		// This is a power scheme change notification
		GUID newScheme = *(GUID*)(DWORD_PTR) pps->Data;

		if ( GUID_MAX_POWER_SAVINGS == newScheme )
		{
			// New scheme: max power savings
			m_eCurrPowerScheme = pwrPowerSaver;
		}
		else if ( GUID_MIN_POWER_SAVINGS == newScheme )
		{
			// New scheme: min power savings (max perf)
			m_eCurrPowerScheme = pwrMaxPerf;
		}
		else if ( GUID_TYPICAL_POWER_SAVINGS == newScheme )
		{
			// New scheme: balanced
			m_eCurrPowerScheme = pwrBalanced;
		}
		else
		{
			// Unrecognized scheme, we'll treat this like balanced
			m_eCurrPowerScheme = pwrBalanced;
		}
	}
	else if ( sizeof(int) == pps->DataLength && pps->PowerSetting == GUID_ACDC_POWER_SOURCE )
	{
		// This is a power source change notification
		int nPowerSrc = *(int*)(DWORD_PTR) pps->Data;
		m_bOnBattery = (0 != nPowerSrc);

		if (m_bOnBattery == true)
			m_TextSpeaker.Speak(L"working on batteries");
		else
			m_TextSpeaker.Speak(L"working on power");

	}
	else if ( sizeof(int) == pps->DataLength && pps->PowerSetting == GUID_BATTERY_PERCENTAGE_REMAINING )
	{
		// This is a battery power notification
		int nPercentLeft = *(int*)(DWORD_PTR) pps->Data;

		if(nPercentLeft < 10 && m_bOnBattery == true)
		{
			CString sPercentLeft;
			sPercentLeft.Format ( _T("Low battery power %d percentage remaining."), nPercentLeft );
			m_TextSpeaker.Speak(sPercentLeft);
		}
		else if(nPercentLeft  > 90 && m_bOnBattery == false)
		{
			CString sPercentLeft;
			sPercentLeft.Format ( _T("Battery power %d percentage charged please switch off the power supply."), nPercentLeft );
			m_TextSpeaker.Speak(sPercentLeft);
		}
	
		m_nBatteryPower = nPercentLeft;
	}

	return uRet;
}

void CvikibotDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	
	OnCancel();
}

void CvikibotDlg::OnBnClickedBtnShow()
{
	ShowWindow(0);
}

void CvikibotDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

LRESULT CvikibotDlg::OnMyDeviceChange(WPARAM wParam, LPARAM lParam)
{
	if ( DBT_DEVICEARRIVAL == wParam || DBT_DEVICEREMOVECOMPLETE == wParam ) {
		PDEV_BROADCAST_HDR pHdr = (PDEV_BROADCAST_HDR)lParam;
		PDEV_BROADCAST_DEVICEINTERFACE pDevInf;
		PDEV_BROADCAST_HANDLE pDevHnd;
		PDEV_BROADCAST_OEM pDevOem;
		PDEV_BROADCAST_PORT pDevPort;
		PDEV_BROADCAST_VOLUME pDevVolume;
		switch( pHdr->dbch_devicetype ) {
			case DBT_DEVTYP_DEVICEINTERFACE:
				pDevInf = (PDEV_BROADCAST_DEVICEINTERFACE)pHdr;
				UpdateDevice(pDevInf, wParam);
				break;

			case DBT_DEVTYP_HANDLE:
				pDevHnd = (PDEV_BROADCAST_HANDLE)pHdr;
				break;

			case DBT_DEVTYP_OEM:
				pDevOem = (PDEV_BROADCAST_OEM)pHdr;
				break;

			case DBT_DEVTYP_PORT:
				pDevPort = (PDEV_BROADCAST_PORT)pHdr;
				break;

			case DBT_DEVTYP_VOLUME:
				pDevVolume = (PDEV_BROADCAST_VOLUME)pHdr;
				break;
		}
	}
return TRUE;
}
void CvikibotDlg::UpdateDevice(PDEV_BROADCAST_DEVICEINTERFACE pDevInf, WPARAM wParam)
{
	// pDevInf->dbcc_name: 
	// \\?\USB#Vid_04e8&Pid_503b#0002F9A9828E0F06#{a5dcbf10-6530-11d2-901f-00c04fb951ed}
	// szDevId: USB\Vid_04e8&Pid_503b\0002F9A9828E0F06
	// szClass: USB
	ASSERT(lstrlen(pDevInf->dbcc_name) > 4);
	CString szDevId = pDevInf->dbcc_name+4;
	int idx = szDevId.ReverseFind(_T('#'));
	ASSERT( -1 != idx );
	szDevId.Truncate(idx);
	szDevId.Replace(_T('#'), _T('\\'));
	szDevId.MakeUpper();

	CString szClass;
	idx = szDevId.Find(_T('\\'));
	ASSERT(-1 != idx );
	szClass = szDevId.Left(idx);
	CString szLog;
	CString szTmp;
	if ( DBT_DEVICEARRIVAL == wParam ) 
	{
		szTmp.Format(_T("Adding %s\r\n"), szDevId.GetBuffer());
		// TRACE("Adding %s\n", szDevId.GetBuffer());
	} else 
	{
		szTmp.Format(_T("Removing %s\r\n"), szDevId.GetBuffer());
		// TRACE("Removing %s\n", szDevId.GetBuffer());
	}
	szLog.Append(szTmp);

	// seems we should ignore "ROOT" type....
	if ( _T("ROOT") == szClass ) 
	{
		return;
	}

	DWORD dwFlag = DBT_DEVICEARRIVAL != wParam ? DIGCF_ALLCLASSES : (DIGCF_ALLCLASSES | DIGCF_PRESENT);
	HDEVINFO hDevInfo = SetupDiGetClassDevs(NULL,szClass,NULL,dwFlag);

	if( INVALID_HANDLE_VALUE == hDevInfo ) 
	{
		AfxMessageBox(CString("SetupDiGetClassDevs(): ")  + GetLastErrorMessage(), MB_ICONEXCLAMATION);
		return;
	}

	SP_DEVINFO_DATA spDevInfoData;
	if ( FindDevice(hDevInfo, szDevId, spDevInfoData) ) 
	{
		// OK, device found
		DWORD DataT ;
		TCHAR buf[MAX_PATH];
		DWORD nSize = 0;
		ZeroMemory(buf,MAX_PATH);
		// get Friendly Name or Device Description
		
		int nDeviceNameFound = false;
		if ( SetupDiGetDeviceRegistryProperty(hDevInfo, &spDevInfoData,SPDRP_FRIENDLYNAME, &DataT, (PBYTE)buf, sizeof(buf), &nSize) ) 
		{
			nDeviceNameFound = true;
		} 
		else if ( SetupDiGetDeviceRegistryProperty(hDevInfo, &spDevInfoData,SPDRP_DEVICEDESC, &DataT, (PBYTE)buf, sizeof(buf), &nSize) ) 
		{
			nDeviceNameFound = false;
		} 
		else 
		{
			lstrcpy(buf, _T("Unknown"));
		}

		if(nDeviceNameFound == true)
		{
			CString csDeviceName =buf;
			if (DBT_DEVICEARRIVAL == wParam)
			{
				csDeviceName += " has been detected.";
				m_TextSpeaker.Speak(csDeviceName);
			}
			else
			{
				csDeviceName += " has been removed.";
					m_TextSpeaker.Speak(csDeviceName);
			}
		}
	}

	SetupDiDestroyDeviceInfoList(hDevInfo);
}
BOOL CvikibotDlg::FindDevice(HDEVINFO& hDevInfo,CString& szDevId,SP_DEVINFO_DATA& spDevInfoData)
{
	spDevInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
	for(int i=0; SetupDiEnumDeviceInfo(hDevInfo, i, &spDevInfoData); i++) {
		DWORD nSize=0 ;
		TCHAR buf[MAX_PATH];

		if ( !SetupDiGetDeviceInstanceId(hDevInfo, &spDevInfoData, buf, sizeof(buf), &nSize) ) 
		{
			TRACE(CString("SetupDiGetDeviceInstanceId(): ") + GetLastErrorMessage());
			return FALSE;
		} 
		if ( szDevId == buf ) {
			// OK, device found
			return TRUE;
		}
	}
	return FALSE;
}
void CvikibotDlg::RegisterNotification(void)
{
_pDevIF = (DEV_BROADCAST_DEVICEINTERFACE *)malloc( sizeof(DEV_BROADCAST_DEVICEINTERFACE) );
	memset( _pDevIF, 0, sizeof(DEV_BROADCAST_DEVICEINTERFACE) );
_pDevIF->dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
_pDevIF->dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
_pDevIF->dbcc_classguid = GUID_INTERFACE_DEV;
_hNotifyDev = RegisterDeviceNotification( this->m_hWnd, _pDevIF, DEVICE_NOTIFY_WINDOW_HANDLE );
}
void CvikibotDlg::UnregisterNotification(void)
{
	UnregisterDeviceNotification( _hNotifyDev );
}
CString CvikibotDlg::GetLastErrorMessage()
{
	CString strError;	
	DWORD dwError = GetLastError();

	LPTSTR lpszTemp = 0;
	DWORD dwRet = ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ARGUMENT_ARRAY,
		0, dwError, LANG_NEUTRAL, (LPTSTR)&lpszTemp, 0, 0);

	if (!dwRet)
	{
		DWORD dwFormatError = ::GetLastError();	// use this to check the error if FormatMessage returns 0
		::SetLastError(dwError);	// keep the old error code
	}
	else 
	{
		lpszTemp[_tcsclen(lpszTemp) - 2] = 0; //remove cr/nl characters
		strError = lpszTemp;
	}

	if (lpszTemp)
		LocalFree(HLOCAL(lpszTemp));
	return strError;
}