#pragma once

#include "VideoFrame.h"
#include "VideoDrawBase.h"
#include "VideoWindowDraw.h"
#include "VideoCorrect.h"


class CVideoCorrectManager
{
public:
	CVideoCorrectManager(HWND org,HWND dst);
	~CVideoCorrectManager();
public:
	VideoCapture m_capture;
	CVideoFrame* m_pVideoFrameResult;	
	CVideoFrame* m_pVideoFrame;
	CVideoDrawBase* m_orgVideoDraw;
	CVideoDrawBase* m_dstVideoDraw;
	CVideoCorrect *m_Correct;
	int winID;
	bool isStop;
public:
	void Start();
	static DWORD WINAPI _workThreadProc(LPVOID lpParam);
	void _Correct();
	void _correctProc();

};