
// fishEyeDemo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CfishEyeDemoApp:
// �йش����ʵ�֣������ fishEyeDemo.cpp
//

class CfishEyeDemoApp : public CWinApp
{
public:
	CfishEyeDemoApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CfishEyeDemoApp theApp;