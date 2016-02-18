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
*���ܣ����Դͼ��������������
*�����������Ƶ����ͼ
*�����������
*����ֵ�������������ڵľ���
******************************/

Rect CFishEyeBase::GetArea(const Mat &inputImage)
{
	FindCorrectArea(inputImage);
	return correctArea;
}

/*****************************
*���ܣ����Դͼ��������������
*�����������Ƶ����ͼ
*�����������
*����ֵ����
******************************/

void CFishEyeBase::FindCorrectArea(const Mat &inputImage)
{
	Mat grayImage;
	cvtColor(inputImage,grayImage,CV_BGR2GRAY,1);
	/*��ֵ��*/
	threshold(grayImage,grayImage,40,255,THRESH_BINARY);
	vector<vector<Point > > contours;
	/*������*/
	findContours(grayImage,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
	Rect minAreaTemp;
	int areaSize(0); 
	int areaSizeTemp(0);
	int areaIter(0);
	/*�ҵ��������,��������Բ������*/
	for (int contoursIter = 0;contoursIter != contours.size();++contoursIter)
	{
		/*�������İ�Χ����*/
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
*���ܣ�����ԲתΪ��׼Բ
*�����������Ƶ����ͼ
*�����������
*����ֵ����׼����ͼ
******************************/
Mat CFishEyeBase::Standard_circle(const Mat& img)
{
	Mat circleImg =  img(correctArea);
	float u =1.0*circleImg.cols /2;
	float v = 1.0*circleImg.rows/ 2;
	float beta  = 1.0 * circleImg.cols / circleImg.rows;  //��͸ߵı�ֵ
	Mat imgOut;
	if( circleImg.cols < circleImg.rows )
		//������ߣ������С�ģ�����
		imgOut = Mat::zeros(min(circleImg.cols,circleImg.rows),min(circleImg.cols,circleImg.rows),CV_8UC3);
	else
		//������ߡ������ģ�����
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