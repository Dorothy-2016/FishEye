#include "stdafx.h"
#include "VideoWindowDraw.h"
#include "CvvImage.h"

CVideoWindowDraw::CVideoWindowDraw(CVideoFrame* pVideoFrame,HWND hWnd)
	:CVideoDrawBase(pVideoFrame)
{
	m_hWnd = hWnd;
}


CVideoWindowDraw::~CVideoWindowDraw(void)
{
	
}

// 绘制到窗口上
int CVideoWindowDraw::Draw()
{
	Mat mImg;
	m_pVideoFrame->PopData(mImg);
	
	HDC hDC = ::GetDC(this->m_hWnd); 
	RECT rect;  
	::GetWindowRect(this->m_hWnd, &rect);   

	int width;
	int height;
	width = rect.right - rect.left;
	height =rect.bottom  - rect.top;

	width = max(100,width);
	height = max(100,height);
	
		// 复制图片
	Mat Scale_mImag;//放缩到设备大小的图像暂存空间
	resize(mImg,Scale_mImag,Size(width,height));
	if (Scale_mImag.data != NULL)
	{
		CString str0=_T("../");
		str0=str0+_T("d.bmp");
		string str1=str0+"";
		//imwrite("D:\\1.png",Scale_mImag);
	}
	IplImage image=Scale_mImag;
	CvvImage cimg;
	cimg.CopyOf(&image);

	
	// 将图片绘制到显示控件的指定区域内	
	rect.left=0;
	rect.top=0;
	rect.right = width;
	rect.bottom = height;

	cimg.DrawToHDC(hDC, &rect );
	Scale_mImag.release();
	cimg.Destroy();

	 ::ReleaseDC(this->m_hWnd, hDC);
	return 0;
}