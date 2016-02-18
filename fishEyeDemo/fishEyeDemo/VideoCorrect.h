#pragma once
#include "FourAreaCorrect.h"
#include "UpDownCorrect.h"
#include "PanoramaCorrect.h"
#include "AnyAreaCorrect.h"


class CVideoCorrect
{
public:
	CVideoCorrect();
	~CVideoCorrect();
public:
	Mat dstImg;
	CFishEyeBase*  fishEyeCorrect;
	bool flag;
	int winID;
public:
	void ImgCorrect(Mat &Image);
	void correctInit();
};