#pragma once

#include "VideoFrame.h"

class CVideoDrawBase :public IVideoFrameReceiver
{
public:
	CVideoDrawBase(CVideoFrame* pVideoFrame);
	virtual ~CVideoDrawBase(void);

	virtual int Draw()=0;

	// 当接收到帧数据
	virtual void OnFrameArrived(const Mat& data);


protected:
	CVideoFrame* m_pVideoFrame;
};

