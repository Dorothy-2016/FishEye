#pragma once
#include "FishEyeBase.h"

class CUpDownCorrect:public CFishEyeBase
{
public:
	CUpDownCorrect();
	virtual ~CUpDownCorrect();

public:
	int *position;
	long *positionShift;
	int m_height;
	int m_width;
public:
	virtual void FishEyeInit(const Mat & Image);
	virtual void ImageCorrect(const Mat& orgImg,Mat& dstImg,int winID);
private:
	void CalcPositon();
	void FixPosition(const Mat& orgImg);

};