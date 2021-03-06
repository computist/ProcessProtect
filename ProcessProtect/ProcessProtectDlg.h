﻿
// ProcessProtectDlg.h : header file
#pragma once


// CProcessProtectDlg dialog
class CProcessProtectDlg : public CDialogEx
{
// Construction
public:
	CProcessProtectDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_PROCESSPROTECT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;


	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:	

	afx_msg void OnBnClickedProtect();
	afx_msg void InstallDriver();
	afx_msg void OnBnClickedHide();
	afx_msg void OnClose();
	long PROCPID;
};
