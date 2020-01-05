
// PermFinderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PermFinder.h"
#include "PermFinderDlg.h"
#include "afxdialogex.h"
#include "CalcEngine.h"

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
	DDX_Control(pDX, IDC_EDIT_FEEDER_PATH, m_editFeederPath);
}

BEGIN_MESSAGE_MAP(CPermFinderDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_ARRAY, &CPermFinderDlg::OnBnClickedBtnArray)
	ON_BN_CLICKED(IDC_BTN_CALC, &CPermFinderDlg::OnBnClickedBtnCalc)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CPermFinderDlg::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CPermFinderDlg::OnBnClickedBtnClose)
	ON_BN_CLICKED(IDC_BTN_FEEDER_BROWSER, &CPermFinderDlg::OnBnClickedBtnFeederBrowser)	
	ON_BN_CLICKED(IDC_BTN_CALC_TESTING, &CPermFinderDlg::OnBnClickedBtnCalcTesting)
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
	m_editUpto.SetWindowTextA("9");

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

CString CPermFinderDlg::GetArrayData(CString path)
{
	CStdioFile fp;
	CString m_Buffer;
	CString m_TempBuffer;

	if( fp.Open(path, CFile::modeRead) == FALSE )
		return "";

	while (!feof (fp.m_pStream))
	{
		fp.ReadString( m_TempBuffer );
		m_Buffer += m_TempBuffer;
		m_Buffer += "\r\n";
	}

	fp.Close();

	return m_Buffer;
}

CString CPermFinderDlg::ReadLastString(CString path)
{
	CStdioFile fp;
	CString m_Buffer;
	
	fp.Open(path, CFile::modeRead);

	while (!feof (fp.m_pStream))
	{
		fp.ReadString( m_Buffer );
	}

	fp.Close();

	return m_Buffer;
}

void CPermFinderDlg::OnBnClickedBtnArray()
{
	const TCHAR szFilter[] = _T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE, _T("txt"), NULL, OFN_HIDEREADONLY, szFilter, this);    
	if(dlg.DoModal() == IDOK)
	{
		CString sFilePath = dlg.GetPathName();
		m_editArrayPath.SetWindowText(sFilePath);
		CString result = GetArrayData(sFilePath);
		m_editArrayData.SetWindowTextA(result);
	}
}


void CPermFinderDlg::OnBnClickedBtnCalc()
{
	CString strData;
	m_editArrayData.GetWindowTextA(strData);	
	
	CString strFeedPath;
	m_editFeederPath.GetWindowTextA(strFeedPath);
	if( strFeedPath.IsEmpty() == FALSE )
	{
		// read last string
		CString last_row = ReadLastString(strFeedPath);				
		strData += "\r\n";
		strData += last_row;
		strData.Replace("\r\n\r\n", "\r\n");

		m_editArrayData.SetWindowTextA(strData);
	}

	int row = 0, col = 0;
	BYTE **x = parseInputData(strData, row, col);

	if( x == NULL )
	{
		MessageBox(_T("Input Data Error"), _T("Error"), MB_ICONERROR);
		return;
	}

	CString buffer;
	m_editUpto.GetWindowTextA(buffer);

	int upto = _ttoi(buffer.GetBuffer(0));
	if( upto < 1 )
		upto = 4;

	CString ret = calcPath(x, row, col, upto);

	m_editResult.SetWindowTextA(ret);

	SaveResult();

	// Free Memeory
	for(int i = 0; i < row; i++)
		free(x[i]);
	free(x);
}


void CPermFinderDlg::OnBnClickedBtnSave()
{
	const TCHAR szFilter[] = _T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||");
	CFileDialog dlg(FALSE, _T("csv"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);    
	if(dlg.DoModal() == IDOK)
	{
		CString sFilePath = dlg.GetPathName();
		m_editSavePath.SetWindowText(sFilePath);
	}
}


void CPermFinderDlg::OnBnClickedBtnClose()
{
	// TODO: Add your control notification handler code here
	OnOK();
}


void CPermFinderDlg::OnBnClickedBtnFeederBrowser()
{
	const TCHAR szFilter[] = _T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE, _T("txt"), NULL, OFN_HIDEREADONLY, szFilter, this);    
	if(dlg.DoModal() == IDOK)
	{
		CString sFilePath = dlg.GetPathName();
		m_editFeederPath.SetWindowText(sFilePath);		
	}
}

void CPermFinderDlg::OnBnClickedBtnCalcTesting()
{
	CString strData;
	m_editArrayData.GetWindowTextA(strData);	

	int row1 = 0, col1 = 0;
	BYTE **x = parseInputData(strData, row1, col1);

	if( x == NULL )
	{
		MessageBox(_T("Input Data Error"), _T("Error"), MB_ICONERROR);
		return;
	}

	// Free Memeory
	for(int i = 0; i < row1; i++)
		free(x[i]);
	free(x);

	CString strFeedPath;
	m_editFeederPath.GetWindowTextA(strFeedPath);
	CString strFeedData = GetArrayData(strFeedPath);

	CString totalData = strData + "\r\n" + strFeedData;
	totalData.Replace("\r\n\r\n", "\r\n");

	int row = 0, col = 0;
	x = parseInputData(totalData, row, col);

	if( x == NULL )
	{
		MessageBox(_T("Input Data Error"), _T("Error"), MB_ICONERROR);
		return;
	}

	CString buffer;
	m_editUpto.GetWindowTextA(buffer);

	int upto = _ttoi(buffer.GetBuffer(0));
	if( upto < 1 )
		upto = 4;

	CString ret = calcPathTesting(x, row, col, row1, upto);

	m_editResult.SetWindowTextA(ret);

	SaveResult();
	
	// Free Memeory
	for(int i = 0; i < row; i++)
		free(x[i]);
	free(x);
}

void CPermFinderDlg::SaveResult()
{
	CString strResult;
	m_editResult.GetWindowTextA(strResult);

	CString path;
	m_editSavePath.GetWindowText(path);

	if( strResult.IsEmpty() || path.IsEmpty() )
		return;


	CStdioFile file;

	if(file.Open(path, CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate))
	{
		file.SeekToEnd();
		file.WriteString(strResult);
		file.Close();
	}
}