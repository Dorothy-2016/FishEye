
// fishEyeDemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "fishEyeDemo.h"
#include "fishEyeDemoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CfishEyeDemoDlg 对话框




CfishEyeDemoDlg::CfishEyeDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CfishEyeDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CfishEyeDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CfishEyeDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CfishEyeDemoDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CfishEyeDemoDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUT0, &CfishEyeDemoDlg::OnBnClickedBut0)
	ON_BN_CLICKED(IDC_BUT1, &CfishEyeDemoDlg::OnBnClickedBut1)
	ON_BN_CLICKED(IDC_BUT2, &CfishEyeDemoDlg::OnBnClickedBut2)
	ON_BN_CLICKED(IDC_BUT3, &CfishEyeDemoDlg::OnBnClickedBut3)
	ON_BN_CLICKED(IDC_BUT4, &CfishEyeDemoDlg::OnBnClickedBut4)
END_MESSAGE_MAP()


// CfishEyeDemoDlg 消息处理程序

BOOL CfishEyeDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	CWnd *org =GetDlgItem(IDC_ORG);
	m_orgWindow = org->GetSafeHwnd();
	CWnd *dst =GetDlgItem(IDC_DST);
	m_dstWindow = dst->GetSafeHwnd();

	m_VideoCorrect = new CVideoCorrectManager(m_orgWindow,m_dstWindow);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CfishEyeDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CfishEyeDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CfishEyeDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CfishEyeDemoDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
	CString filter=_T("Video Files (*.avi;*.asf;*.mpg;*.mpeg;*.MTS;*.wmv;*.mp4)|*.avi;*.asf;*.mpg;*.mpeg;*.MTS;*.wmv;*.mp4|All Files (*.*)|*.*||");
	CFileDialog dlg( true, 0, 0, OFN_FILEMUSTEXIST,filter, NULL );
	if( dlg.DoModal() == IDOK )
	{
		// Get the video file name and open it.
		 CString VideoPath = dlg.GetPathName();
		 m_Video.open(VideoPath.GetBuffer(0));  
		if (!m_Video.isOpened())
		{
			AfxMessageBox(_T("打开视频失败！"),MB_OK);
		}
		else
		{
			m_VideoCorrect->m_capture = m_Video;
			m_VideoCorrect->Start();
		}
	}
}


void CfishEyeDemoDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	m_VideoCorrect->isStop = true;
	Sleep(500);
	if (m_VideoCorrect != NULL)
	{
		delete m_VideoCorrect;
	}

	CDialogEx::OnCancel();
}


void CfishEyeDemoDlg::OnBnClickedBut0()
{
	// TODO: 在此添加控件通知处理程序代码
	m_VideoCorrect->winID = 0;
}


void CfishEyeDemoDlg::OnBnClickedBut1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_VideoCorrect->winID = 1;
}


void CfishEyeDemoDlg::OnBnClickedBut2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_VideoCorrect->winID = 2;
}


void CfishEyeDemoDlg::OnBnClickedBut3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_VideoCorrect->winID = 3;
}


void CfishEyeDemoDlg::OnBnClickedBut4()
{
	// TODO: 在此添加控件通知处理程序代码
	m_VideoCorrect->winID = 4;
}
