#pragma once
#include "FishEyeBase.h"


class CAnyAreaCorrect: public CFishEyeBase
{
public:
	CAnyAreaCorrect();
	virtual ~CAnyAreaCorrect();


private:
	int m_height;
	int m_width;
	//int *position;
	long *positionShift;

public:
	virtual void FishEyeInit(const Mat & Image);
	virtual void ImageCorrect(const Mat& orgImg,Mat& dstImg,int winID);

private:
	int* anyangle(float theta1, float theta2,float rmin,float rmax);
	//void CalcPositon();
	void FixPosition(const Mat& orgImg);
};