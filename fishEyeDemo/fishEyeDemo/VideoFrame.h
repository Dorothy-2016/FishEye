#pragma once
#include <afxmt.h>
#include <afxtempl.h>
#include <algorithm> 
#include <list> 
// ��Ƶ֡���սӿ�
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
	// �������
	void PushData(/*const*/ Mat& data);
	void PopData(Mat& data);

	// ע��ص�����
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

	// �洢��Ƶ����
	Mat _videoData;

	// �ٽ���
	CCriticalSection _lock;

	// �ص��б�
	std::list<IVideoFrameReceiver*> _receivers;
};


