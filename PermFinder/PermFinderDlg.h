
// PermFinderDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CPermFinderDlg dialog
class CPermFinderDlg : public CDialogEx
{
// Construction
public:
	CPermFinderDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_PERMFINDER_DIALOG };

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
	afx_msg void OnBnClickedBtnArray();
	CEdit m_editArrayPath;
	CEdit m_editSavePath;
	CEdit m_editArrayData;
	CEdit m_editResult;
	CEdit m_editUpto;

private:
	CString GetArrayData(CString path);
	CString ReadLastString(CString path);
	void	SaveResult();
public:
	afx_msg void OnBnClickedBtnCalc();
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnClose();
	CEdit m_editFeederPath;
	afx_msg void OnBnClickedBtnFeederBrowser();
	afx_msg void OnBnClickedBtnCalcTesting();
	afx_msg void OnBnClickedBtnCalcCompact();
	afx_msg void OnBnClickedBtnCalcLentotal();
	CEdit m_txtFrom;
	CEdit m_txtSummaryReport;
	CEdit m_editSummaryPath;
	afx_msg void OnBnClickedBtnSummary();
};
