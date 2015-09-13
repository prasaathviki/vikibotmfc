// VikiBotInstallerDlg.h : header file
//

#pragma once
#include "shlwapi.h"

// CVikiBotInstallerDlg dialog
class CVikiBotInstallerDlg : public CDialog
{
// Construction
public:
	CVikiBotInstallerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_VIKIBOTINSTALLER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	bool CVikiBotInstallerDlg::fn_get_exe_path(CString &curdir);
};
