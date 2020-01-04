
// PermFinderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PermFinder.h"
#include "PermFinderDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPermFinderDlg dialog




CPermFinderDlg::CPermFinderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPermFinderDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPermFinderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ARRAY_PATH, m_editArrayPath);
	DDX_Control(pDX, IDC_EDIT_SAVE_PATH, m_editSavePath);
	DDX_Control(pDX, IDC_EDIT_ARRAY_DATA, m_editArrayData);
	DDX_Control(pDX, IDC_EDIT_RESULT_DATA, m_editResult);
	DDX_Control(pDX, IDC_EDIT_UP_TO, m_editUpto);
}

BEGIN_MESSAGE_MAP(CPermFinderDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_ARRAY, &CPermFinderDlg::OnBnClickedBtnArray)
END_MESSAGE_MAP()


// CPermFinderDlg message handlers

BOOL CPermFinderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPermFinderDlg::OnPaint()
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
HCURSOR CPermFinderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPermFinderDlg::DisplayArrayData(CString path)
{
	CStdioFile fp;
	CString m_Buffer;
	CString m_TempBuffer;

	fp.Open(path, CFile::modeRead);

	while (!feof (fp.m_pStream))
	{
		fp.ReadString( m_TempBuffer );
		m_Buffer += m_TempBuffer;
		m_Buffer += "\r\n";
	}

	fp.Close();

	m_editArrayData.SetWindowText(m_Buffer);
}

void CPermFinderDlg::OnBnClickedBtnArray()
{
	const TCHAR szFilter[] = _T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||");
	CFileDialog dlg(FALSE, _T("csv"), NULL, OFN_HIDEREADONLY, szFilter, this);    
	if(dlg.DoModal() == IDOK)
	{
		CString sFilePath = dlg.GetPathName();
		m_editArrayPath.SetWindowText(sFilePath);
		DisplayArrayData(sFilePath);
	}
}
