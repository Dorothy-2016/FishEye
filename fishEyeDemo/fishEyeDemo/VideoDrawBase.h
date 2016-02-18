#pragma once

#include "VideoFrame.h"

class CVideoDrawBase :public IVideoFrameReceiver
{
public:
	CVideoDrawBase(CVideoFrame* pVideoFrame);
	virtual ~CVideoDrawBase(void);

	virtual int Draw()=0;

	// �����յ�֡����
	virtual void OnFrameArrived(const Mat& data);


protected:
	CVideoFrame* m_pVideoFrame;
};

