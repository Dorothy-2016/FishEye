
// fishEyeDemoDlg.h : ͷ�ļ�
//

#pragma once
#include "VideoCorrectManager.h"
// CfishEyeDemoDlg �Ի���
class CfishEyeDemoDlg : public CDialogEx
{
// ����
public:
	CfishEyeDemoDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_FISHEYEDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	VideoCapture  m_Video; 
	CVideoCorrectManager * m_VideoCorrect;
	HWND m_orgWindow;
	HWND m_dstWindow;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBut0();
	afx_msg void OnBnClickedBut1();
	afx_msg void OnBnClickedBut2();
	afx_msg void OnBnClickedBut3();
	afx_msg void OnBnClickedBut4();
};
