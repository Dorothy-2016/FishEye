#include "stdafx.h"
#include "VideoCorrectManager.h"

CVideoCorrectManager::CVideoCorrectManager(HWND org,HWND dst)
{
	// ��Ƶ֡�洢��
		m_pVideoFrame = new CVideoFrame();
		m_pVideoFrameResult = new CVideoFrame();
	// ��Ƶ���Ƶ����ڵĶ���
	    m_orgVideoDraw = new CVideoWindowDraw(m_pVideoFrame, org);
		m_dstVideoDraw = new CVideoWindowDraw(m_pVideoFrameResult, dst);
	    m_Correct = new CVideoCorrect();
		winID = 0;
		isStop = false;
}

CVideoCorrectManager::~CVideoCorrectManager()
{
	if (m_Correct != NULL)
	{
		delete m_Correct;
	}
}

void CVideoCorrectManager::Start()
{
	 CreateThread(NULL,0,_workThreadProc,(LPVOID)(this),0,NULL);
}

DWORD CVideoCorrectManager::_workThreadProc(LPVOID lpParam)
{
	((CVideoCorrectManager *)lpParam)->_Correct();
	return 0;
}

void CVideoCorrectManager::_Correct()
{
	m_Correct->correctInit();
	for (int i = 0;i < m_capture.get(CV_CAP_PROP_FRAME_COUNT);i++)
	{
		if (isStop)
		{
			break;
		}
		DWORD tick = ::GetTickCount();
		Mat frame;
		m_capture.read(frame);
		//Mat tmp = Mat::zeros(int(frame.rows/2),int(frame.cols/2),CV_8UC3);
		//resize(frame,tmp,tmp.size());
		m_pVideoFrame->PushData(frame);
		_correctProc();
		// ��ʱ
		DWORD tick2 = ::GetTickCount();
		DWORD useTick = tick2 - tick;
		int remainTick =30 - useTick;
		DWORD sleepTick = MAX(remainTick,0);	 
		::Sleep(sleepTick);  
	}
}

void CVideoCorrectManager::_correctProc()
{
	//ȡ��
	Mat orgImg;
	m_pVideoFrame->PopData(orgImg);
	//У��
	m_Correct->winID = winID;
	m_Correct->ImgCorrect(orgImg);
	//��ʾ
	m_pVideoFrameResult->PushData(m_Correct->dstImg);

}

