#include "stdafx.h"
#include "VideoFrame.h"


CVideoFrame::CVideoFrame()
{
	
}

CVideoFrame::~CVideoFrame()
{
}


void CVideoFrame::PushData(/*const*/ Mat& data)
{
	if(data.rows == 0 || data.cols == 0)
		return;
	_lock.Lock();
	data.copyTo(_videoData);
	_lock.Unlock();

	// 通知所有订阅者
	for(std::list<IVideoFrameReceiver*>::iterator iter=_receivers.begin();iter!=_receivers.end();++iter)
	{
		IVideoFrameReceiver* receiver = *iter;
		receiver->OnFrameArrived(data);
	}
}


void CVideoFrame::PopData(Mat& data)
{
	_lock.Lock();

	_videoData.copyTo(data);
	
	_lock.Unlock();
}