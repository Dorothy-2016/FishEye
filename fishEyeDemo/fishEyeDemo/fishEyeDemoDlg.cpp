
// fishEyeDemoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "fishEyeDemo.h"
#include "fishEyeDemoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CfishEyeDemoDlg �Ի���




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


// CfishEyeDemoDlg ��Ϣ�������

BOOL CfishEyeDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	CWnd *org =GetDlgItem(IDC_ORG);
	m_orgWindow = org->GetSafeHwnd();
	CWnd *dst =GetDlgItem(IDC_DST);
	m_dstWindow = dst->GetSafeHwnd();

	m_VideoCorrect = new CVideoCorrectManager(m_orgWindow,m_dstWindow);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CfishEyeDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CfishEyeDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CfishEyeDemoDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
			AfxMessageBox(_T("����Ƶʧ�ܣ�"),MB_OK);
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_VideoCorrect->winID = 0;
}


void CfishEyeDemoDlg::OnBnClickedBut1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_VideoCorrect->winID = 1;
}


void CfishEyeDemoDlg::OnBnClickedBut2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_VideoCorrect->winID = 2;
}


void CfishEyeDemoDlg::OnBnClickedBut3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_VideoCorrect->winID = 3;
}


void CfishEyeDemoDlg::OnBnClickedBut4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_VideoCorrect->winID = 4;
}
