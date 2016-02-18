#pragma once
#include <afxmt.h>
#include <afxtempl.h>
#include <algorithm> 
#include <list> 
// 视频帧接收接口
class IVideoFrameReceiver
{
public:
   virtual void OnFrameArrived(const Mat& data)=0;
};

//
class CVideoFrame
{
public:
	CVideoFrame();
	virtual ~CVideoFrame();
	// 加入队列
	void PushData(/*const*/ Mat& data);
	void PopData(Mat& data);

	// 注册回调函数
	void Advise(IVideoFrameReceiver* reveiver)
	{
		_receivers.push_back(reveiver);
	}

	void UnAdvise(IVideoFrameReceiver* reveiver)
	{
		std::list<IVideoFrameReceiver*>::iterator it = std::find(_receivers.begin(), _receivers.end(), reveiver); 
		if(it != _receivers.end())
		{
			_receivers.remove(reveiver);
		}
	}
private:

	// 存储视频数据
	Mat _videoData;

	// 临界锁
	CCriticalSection _lock;

	// 回调列表
	std::list<IVideoFrameReceiver*> _receivers;
};


