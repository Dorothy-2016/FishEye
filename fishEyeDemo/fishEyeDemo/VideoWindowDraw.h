#pragma once

#include "VideoDrawBase.h"


class CVideoWindowDraw :public CVideoDrawBase
{
public:
	CVideoWindowDraw(CVideoFrame* pVideoFrame,HWND hWnd);
	virtual ~CVideoWindowDraw(void);

	virtual int Draw(); 	

private:

	HWND m_hWnd;
};

