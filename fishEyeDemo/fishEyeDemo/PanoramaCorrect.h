#pragma once
#include "FishEyeBase.h"

class CPanoramaCorrect:public CFishEyeBase
{
public:
	CPanoramaCorrect();
	virtual ~CPanoramaCorrect();

private:
	/// 最大水平视角(镜头朝上：0到360度 镜头超前 0到180度)   
	int iAngHMax;
	/// 最小水平视角(镜头朝上：0到360度 镜头超前 0到180度)
	int iAngHMin;
	/// 最大垂直视角(镜头朝上：0到90度 镜头超前 -90到90度))
	int iAngVMax;
	/// 最小垂直视角(镜头朝上：0到90度 镜头超前 -90到90度)
	int iAngVMin;
	/// 是否按照等弧长计算
	bool isEqualArc;
	double dRScale;

	Mat dstImg;
	//输出图像的宽度
	int iWidthOut;
	//输出图像的高度
	int iHeightOut;

	double RX_Out;
	double RY_Out;

	int *position;
	long *positionShift;

public:
	virtual void ParamInit();
	virtual void FishEyeInit(const Mat & Image);
	virtual void ImageCorrect(const Mat& orgImg,Mat& dstImg,int winID);
private:
	void UV2XY_Cylinder(double dU, double dV, double &dX,  double &dY);
	void CalcPositon();
	void FixPosition(const Mat& orgImg);
};