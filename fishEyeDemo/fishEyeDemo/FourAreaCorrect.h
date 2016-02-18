#pragma once
#include "FishEyeBase.h"

class CFourAreaCorrect:public CFishEyeBase
{
public:
	CFourAreaCorrect();
	virtual ~CFourAreaCorrect();

private:
	int dstWidth;
	int dstHeight;
	double phi0;
	double sita0;
	double gama0;
	double phi1;
	double sita1;
	double gama1;
	double phi2;
	double sita2;
	double gama2;
	double phi3;
	double sita3;
	double gama3;
	double wRatio;
	double hRatio;
	const double piParam;
	double f;
	int *position;
	long *positionShift;
	Mat orgImg;
	Mat dstImg;

public:
	virtual void ParamInit();
	virtual void FishEyeInit(const Mat & Image);
	virtual void ImageCorrect(const Mat& orgImg,Mat& dstImg,int winID);

private:
	void ParamFix();
	void FixPosition(const Mat& orgImg);
	void CalculateParam(int width, int height);
	void Fix(int &u,int &v,double &x,double &y,const double &phi,const double &sita,const double &gama);
	void CalcPositon(Mat &fishImage);
};
