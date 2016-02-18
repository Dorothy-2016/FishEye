
// fishEyeDemoDlg.h : 头文件
//

#pragma once
#include "VideoCorrectManager.h"
// CfishEyeDemoDlg 对话框
class CfishEyeDemoDlg : public CDialogEx
{
// 构造
public:
	CfishEyeDemoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_FISHEYEDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
