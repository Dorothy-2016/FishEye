#include "stdafx.h"
#include "VideoDrawBase.h"


CVideoDrawBase::CVideoDrawBase(CVideoFrame* pVideoFrame)
{
	m_pVideoFrame = pVideoFrame;
	// ∂©‘ƒ
	pVideoFrame->Advise(this);

}


CVideoDrawBase::~CVideoDrawBase(void)
{
	this->m_pVideoFrame->UnAdvise(this);
}

void CVideoDrawBase::OnFrameArrived(const Mat& data)
{
	// ªÊ÷∆ΩÁ√Ê
	Draw();
}
