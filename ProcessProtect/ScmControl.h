#pragma comment(lib,"advapi32.lib")
#include <winsvc.h>

// Define device type and status
#define FILE_DEVICE_UNKNOWN             0x00000022
#define FILE_ANY_ACCESS     0
#define METHOD_BUFFERED	0

class cDrvCtrl
{
public:
	
	cDrvCtrl()
	{
		// Initialize variables
		m_pSysPath = NULL;
		m_pServiceName = NULL;
		m_pDisplayName = NULL;
		m_hSCManager = NULL;
		m_hService = NULL;
		m_hDriver = INVALID_HANDLE_VALUE;
	}
	~cDrvCtrl()
	{
		// clear
		if (m_hService != NULL) {
			CloseServiceHandle(m_hService);
		}
		if (m_hSCManager != NULL) {
			CloseServiceHandle(m_hSCManager);
		}
		if (m_hDriver != NULL) {
			CloseHandle(m_hDriver);
		}		
	}
public:
	DWORD m_dwLastError;
	PCHAR m_pSysPath;
	PCHAR m_pServiceName;
	PCHAR m_pDisplayName;
	HANDLE m_hDriver;
	SC_HANDLE m_hSCManager;
	SC_HANDLE m_hService;
public:
	BOOL Install(PCHAR pSysPath,PCHAR pServiceName,PCHAR pDisplayName);
	BOOL Start();
	BOOL Stop();
	BOOL Remove();
	BOOL Open(PCHAR pLinkName);
	BOOL IoControl(DWORD dwIoCode, PVOID InBuff, DWORD InBuffLen, PVOID OutBuff, DWORD OutBuffLen, DWORD *RealRetBytes);
private:
	BOOL GetSvcHandle(PCHAR pServiceName);
	DWORD CTL_CODE_GEN(DWORD lngFunction);
protected:
	//null
};

BOOL cDrvCtrl::GetSvcHandle(PCHAR pServiceName)
{
	m_pServiceName = pServiceName;
	m_hSCManager = OpenSCManagerA(NULL,NULL,SC_MANAGER_ALL_ACCESS);
	if (NULL == m_hSCManager)
	{
		m_dwLastError = GetLastError();
		return FALSE;
	}
	m_hService = OpenServiceA(m_hSCManager,m_pServiceName,SERVICE_ALL_ACCESS);
	if (NULL == m_hService)
	{
		CloseServiceHandle(m_hSCManager);
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

// Install the service for the device
BOOL cDrvCtrl::Install(PCHAR pSysPath,PCHAR pServiceName,PCHAR pDisplayName)
{
	m_pSysPath = pSysPath;
	m_pServiceName = pServiceName;
	m_pDisplayName = pDisplayName;
	m_hSCManager = OpenSCManagerA(NULL,NULL,SC_MANAGER_ALL_ACCESS);
	if (NULL == m_hSCManager)
	{
		m_dwLastError = GetLastError();
		return FALSE;
	}
	m_hService = CreateServiceA(m_hSCManager,m_pServiceName,m_pDisplayName,
	                            SERVICE_ALL_ACCESS,SERVICE_KERNEL_DRIVER,SERVICE_DEMAND_START,SERVICE_ERROR_NORMAL,
	                            m_pSysPath,NULL,NULL,NULL,NULL,NULL);
	if (NULL == m_hService)
	{
		m_dwLastError = GetLastError();
		if (ERROR_SERVICE_EXISTS == m_dwLastError)
		{
			m_hService = OpenServiceA(m_hSCManager,m_pServiceName,SERVICE_ALL_ACCESS);
			if (NULL == m_hService)
			{
				CloseServiceHandle(m_hSCManager);
				return FALSE;
			}
		}
		else
		{
			CloseServiceHandle(m_hSCManager);
			return FALSE;
		}
	}
	return TRUE;
}

//Start the service for a device
BOOL cDrvCtrl::Start()
{
	if (!StartServiceA(m_hService,NULL,NULL))
	{
		m_dwLastError = GetLastError();
		return FALSE;
	}
	return TRUE;
}

// Stop the service of a device
BOOL cDrvCtrl::Stop()
{
	SERVICE_STATUS ss;
	GetSvcHandle(m_pServiceName);
	if (!ControlService(m_hService,SERVICE_CONTROL_STOP,&ss))
	{
		m_dwLastError = GetLastError();
		return FALSE;
	}
	return TRUE;

}

// Uninstall the service
BOOL cDrvCtrl::Remove()
{
	GetSvcHandle(m_pServiceName);
	if (!DeleteService(m_hService))
	{
		m_dwLastError = GetLastError();
		return FALSE;
	}
	return TRUE;
}

// Open the symbol link
BOOL cDrvCtrl::Open(PCHAR pLinkName)//example: \\\\.\\xxoo
{
	if (m_hDriver != INVALID_HANDLE_VALUE)
		return TRUE;
	m_hDriver = CreateFileA(pLinkName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if(m_hDriver != INVALID_HANDLE_VALUE)
		return TRUE;
	else
		return FALSE;
}

// IO with the device
BOOL cDrvCtrl::IoControl(DWORD dwIoCode, PVOID InBuff, DWORD InBuffLen, PVOID OutBuff, DWORD OutBuffLen, DWORD *RealRetBytes)
{
	DWORD dw;
	BOOL b=DeviceIoControl(m_hDriver,CTL_CODE_GEN(dwIoCode),InBuff,InBuffLen,OutBuff,OutBuffLen,&dw,NULL);
	if(RealRetBytes)
		*RealRetBytes=dw;
	return b;
}

DWORD cDrvCtrl::CTL_CODE_GEN(DWORD lngFunction)
{
	return (FILE_DEVICE_UNKNOWN * 65536) | (FILE_ANY_ACCESS * 16384) | (lngFunction * 4) | METHOD_BUFFERED;
}