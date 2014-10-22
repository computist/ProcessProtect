
// ProcessProtectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ProcessProtect.h"
#include "ProcessProtectDlg.h"
#include "afxdialogex.h"
#include "ScmControl.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#endif

cDrvCtrl dc;

// Initialize driver
bool InstallDriver(){
	char szSysFile[MAX_PATH]={0};
	char szSvcLnkName[]="ProcModule64";

	// Format the path string
	GetModuleFileNameA(0,szSysFile,MAX_PATH);
	for(SIZE_T i=strlen(szSysFile)-1;i>=0;i--)
	{
		if(szSysFile[i]=='\\')
		{
			szSysFile[i+1]='\0';
			break;
		}
	}

	strcat(szSysFile,"ProcModule64.sys");
	//Install the driver
	if (dc.Install(szSysFile,szSvcLnkName,szSvcLnkName) && dc.Start()){
		// Open symbolic link
		dc.Open("\\\\.\\ProcModule64");
		return true;
	}
	return false;
}


bool UninstallDriver(){
		CloseHandle(dc.m_hDriver);
		CloseHandle(dc.m_hDriver);
		dc.Stop();
		return dc.Remove();
}

void CProcessProtectDlg::OnBnClickedProtect()
{
	CWnd* pWnd = GetDlgItem(IDC_PID);
	CString sPid;
	pWnd -> GetWindowText(sPid);
	if (!sPid.IsEmpty()){
		dc.IoControl(0x800,0,0,0,0,0);
	}
}

// CProcessProtectDlg dialog




CProcessProtectDlg::CProcessProtectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProcessProtectDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CProcessProtectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CProcessProtectDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_PROTECT, &CProcessProtectDlg::OnBnClickedProtect)
	ON_BN_CLICKED(IDC_HIDE, &CProcessProtectDlg::OnBnClickedHide)
END_MESSAGE_MAP()


// CProcessProtectDlg message handlers

BOOL CProcessProtectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	::InstallDriver();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CProcessProtectDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CProcessProtectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CProcessProtectDlg::OnBnClickedHide()
{
	CWnd* pWnd = GetDlgItem(IDC_PID);
	CString sPid;
	pWnd -> GetWindowText(sPid);
	if (!sPid.IsEmpty()){
		
		dc.IoControl(0x801,0,0,0,0,0);
	}
}
