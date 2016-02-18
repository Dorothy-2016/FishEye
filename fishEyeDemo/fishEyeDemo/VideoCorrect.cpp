#include "stdafx.h"
#include "VideoCorrect.h"


CVideoCorrect::CVideoCorrect()
{
	//fishEyeCorrect = new CFourAreaCorrect();
	fishEyeCorrect = new CUpDownCorrect();
	//fishEyeCorrect = new CPanoramaCorrect();
	//fishEyeCorrect = new CAnyAreaCorrect();
}
CVideoCorrect::~CVideoCorrect()
{
	if (fishEyeCorrect!= NULL)
	{
		delete fishEyeCorrect;
	}
}

void CVideoCorrect::correctInit()
{
	fishEyeCorrect->ParamInit();
	flag = false;
	winID = 0;
}

void CVideoCorrect::ImgCorrect(Mat &Image)
{
	if (!flag)
	{
		fishEyeCorrect->FishEyeInit(Image);
		flag = true;
	}
	Mat orgImg = fishEyeCorrect->Standard_circle(Image);
	fishEyeCorrect->ImageCorrect(orgImg,dstImg,winID);

}