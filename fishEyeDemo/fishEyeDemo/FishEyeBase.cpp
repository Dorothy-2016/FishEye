#include "stdafx.h"
#include "FishEyeBase.h"

CFishEyeBase::CFishEyeBase()
{

}

CFishEyeBase::~CFishEyeBase()
{

}

void CFishEyeBase::ParamInit()
{

}

void CFishEyeBase::FishEyeInit(const Mat & Image)
{
	GetArea(Image);
	Mat tmp = Standard_circle(Image);
	int width = tmp.cols;
	int height = tmp.rows;
	r = 0.5*(width<height?width:height);
	cx=(width-1)/2;  
	cy=(height-1)/2;   
}

/*****************************
*功能：求出源图中鱼眼所在区域
*输入参数：视频输入图
*输出参数：空
*返回值：鱼眼区域所在的矩形
******************************/

Rect CFishEyeBase::GetArea(const Mat &inputImage)
{
	FindCorrectArea(inputImage);
	return correctArea;
}

/*****************************
*功能：求出源图中鱼眼所在区域
*输入参数：视频输入图
*输出参数：空
*返回值：空
******************************/

void CFishEyeBase::FindCorrectArea(const Mat &inputImage)
{
	Mat grayImage;
	cvtColor(inputImage,grayImage,CV_BGR2GRAY,1);
	/*二值化*/
	threshold(grayImage,grayImage,40,255,THRESH_BINARY);
	vector<vector<Point > > contours;
	/*找轮廓*/
	findContours(grayImage,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
	Rect minAreaTemp;
	int areaSize(0); 
	int areaSizeTemp(0);
	int areaIter(0);
	/*找到最大轮廓,就是鱼眼圆的轮廓*/
	for (int contoursIter = 0;contoursIter != contours.size();++contoursIter)
	{
		/*求轮廓的包围盒子*/
		minAreaTemp=boundingRect(contours[contoursIter]);
		areaSizeTemp=minAreaTemp.width*minAreaTemp.height;
		if (areaSize<areaSizeTemp)
		{
			correctArea=minAreaTemp;
			areaSize=areaSizeTemp;
		}
	}	
}

/*****************************
*功能：将椭圆转为标准圆
*输入参数：视频输入图
*输出参数：空
*返回值：标准鱼眼图
******************************/
Mat CFishEyeBase::Standard_circle(const Mat& img)
{
	Mat circleImg =  img(correctArea);
	float u =1.0*circleImg.cols /2;
	float v = 1.0*circleImg.rows/ 2;
	float beta  = 1.0 * circleImg.cols / circleImg.rows;  //宽和高的比值
	Mat imgOut;
	if( circleImg.cols < circleImg.rows )
		//如果宽《高，则跟随小的，即宽
		imgOut = Mat::zeros(min(circleImg.cols,circleImg.rows),min(circleImg.cols,circleImg.rows),CV_8UC3);
	else
		//如果宽》高。跟随大的，即宽
		imgOut = Mat::zeros(max(circleImg.cols,circleImg.rows),max(circleImg.cols,circleImg.rows),CV_8UC3);

	float a[] = { 1.0, 0, 0,
		0, beta, 0,
		0, 0, 1.0 };
	Mat M(3,3,CV_32FC1,a);
	warpPerspective(circleImg,imgOut,M,imgOut.size());
	M.release();
	return imgOut;
}


void CFishEyeBase::ImageCorrect(const Mat& orgImg,Mat& dstImg,int winID)
{
	orgImg.copyTo(dstImg);
}