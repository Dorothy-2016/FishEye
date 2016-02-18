#include "stdafx.h"
#include "AnyAreaCorrect.h"


CAnyAreaCorrect::CAnyAreaCorrect()
{

}

CAnyAreaCorrect::~CAnyAreaCorrect()
{
	if (positionShift != NULL)
	{
		free(positionShift);
		positionShift = NULL;
	}
}

void CAnyAreaCorrect::FishEyeInit(const Mat & Image)
{
	GetArea(Image);
	Mat img =  Standard_circle(Image);
	r=1.0*img.cols/2;
	cx=1.0*img.cols/2;
	cy=1.0*img.cols/2;
	m_height=r;
	m_width=(int)(120*PI*r/180);

	//position = (int*)malloc(m_height*2*m_width*2*sizeof(int));
	positionShift = (long*)malloc(m_width*m_height*2*sizeof(long));
	//CalcPositon();
	FixPosition(img);
}

void CAnyAreaCorrect::ImageCorrect(const Mat& orgImg,Mat& dstImg,int winID)
{
	long temDstPos(0);
	long temOrgPos(0);
	uchar* orgImgData = orgImg.data;
	long total = orgImg.cols*orgImg.rows*3;
	
	if (winID == 1)
	{
		dstImg = Mat::zeros(m_height,m_width,CV_8UC3);
		uchar* dstImgData = dstImg.data; 
		for (int i = 0;i<m_height;i++)
		{
			for (int j = 0;j<m_width;j++)
			{
				temOrgPos = positionShift[m_width*i+j];
				if (temOrgPos >=0 && temOrgPos <= total)
				{
					memcpy(dstImgData + temDstPos,orgImgData+temOrgPos,3);
				}
				temDstPos +=3;
			}
		}

	}
	else if (winID == 2)
	{
		dstImg = Mat::zeros(m_height,m_width/2,CV_8UC3);
		uchar* dstImgData = dstImg.data; 
		for (int i = 0;i<m_height;i++)
		{
			for (int j = 0;j<m_width/2;j++)
			{
				temOrgPos = positionShift[m_width*(i+m_height)+j];
				if (temOrgPos >=0 && temOrgPos <= total)
				{
					memcpy(dstImgData + temDstPos,orgImgData+temOrgPos,3);
				}
				temDstPos +=3;
			}
		}
	}
	else if (winID == 3)
	{
		dstImg = Mat::zeros(m_height,m_width/2,CV_8UC3);
		uchar* dstImgData = dstImg.data; 
		for (int i = 0;i<m_height;i++)
		{
			for (int j = 0;j<m_width/2;j++)
			{
				temOrgPos = positionShift[m_width*(i+m_height)+j+m_width/2];
				if (temOrgPos >=0 && temOrgPos <= total)
				{
					memcpy(dstImgData + temDstPos,orgImgData+temOrgPos,3);
				}
				temDstPos +=3;
			}
		}
	}
	else
	{
		dstImg = Mat::zeros(m_height*2,m_width,CV_8UC3);
		uchar* dstImgData = dstImg.data; 
		for (int i = 0;i<m_height*2;i++)
		{
			for (int j = 0;j<m_width;j++)
			{
				temOrgPos = positionShift[m_width*i+j];
				if (temOrgPos >=0 && temOrgPos <= total)
				{
					memcpy(dstImgData + temDstPos,orgImgData+temOrgPos,3);
				}
				temDstPos +=3;
			}
		}
	}

}

// 对任意给定的角度进行校正
int* CAnyAreaCorrect::anyangle(float theta1, float theta2,float rmin,float rmax)
{
	if(theta2<theta1)
	{
		theta2=theta2+360;//水平向右是0度，如果起始角度小于终止角度
	}
	int * index;
	int height=rmax - rmin;
	int width=(int)((theta2 -theta1 )*PI*r/180);
	index = (int*)malloc(width*2*height*sizeof(int));

	int x3=0;
	float x4=0;
	int tempPos(0);
	float gap =width*0.01/(theta2-theta1);
	for (int r0=rmax;r0>rmin;r0--)
	{
		for(float theta=theta2;theta>=theta1;)
		{
			int j00=(int)(abs)(cx+r0*cos(PI*theta/180));//列  
			int i00=(int)(abs)(cy-r0*sin(PI*theta/180));//行
			int u0 = height-r0+rmin;
			int v0  = x3;
			if (u0<0)
			{
				u0 = 0;
			}
			if (u0 >=height)
			{
				u0 = height -1;
			}

			if (v0<0)
			{
				v0 = 0;
			}
			if (v0 >=width)
			{
				v0 = width -1;
			}

			tempPos = u0*width*2 + v0*2;
			index[tempPos] = i00;
			index[tempPos + 1] = j00;

			x4=x4+gap;
			x3=(int)x4;
			theta=theta-0.01;
		}
		x3=0;
		x4=0;
	}
	return index;
}

//
//void CAnyAreaCorrect::CalcPositon()
//{
//	//上
//	int *topIndex = anyangle(30,150,0,r);
//	long tempPos(0);
//	long length = m_width*2*m_height*sizeof(int);
//	memcpy(position,topIndex,length);
//	tempPos += m_width*2*m_height;
//	//左
//	int *leftIndex = anyangle(150,210,0,r);
//	length = m_width*2*m_height*sizeof(int);
//	memcpy(position+tempPos,leftIndex,length);
//	tempPos += m_width*2*m_height;
//	//右
//	int *rightIndex = anyangle(-30,30,0,r);
//	length = m_width*2*m_height*sizeof(int);
//	memcpy(position+tempPos,rightIndex,length);
//	
//	if (topIndex != NULL)
//	{
//		free(topIndex);
//		topIndex = NULL;
//	}
//	if (leftIndex != NULL)
//	{
//		free(leftIndex);
//		leftIndex = NULL;
//	}
//	if (rightIndex != NULL)
//	{
//		free(rightIndex);
//		rightIndex = NULL;
//	}
//}

void CAnyAreaCorrect::FixPosition(const Mat& orgImg)
{
	int orgImgLineWidht = orgImg.step[0];
	int orgImgEleWidht = orgImg.step[1];
	long tempPos(0);
	int *topIndex = anyangle(30,150,0,r);
	for (int row = 0;row < m_height; ++row)
	{
		for (int col = 0;col <m_width; ++col)
		{
			positionShift[m_width*row + col] = (double)orgImgLineWidht*(double)topIndex[tempPos] + (double)topIndex[tempPos + 1]*(double)orgImgEleWidht;
			tempPos += 2;
		}
	}
	tempPos = 0;
	int *leftIndex = anyangle(150,210,0,r);
	for (int row = m_height;row < m_height*2; ++row)
	{
		for (int col = 0;col <m_width/2; ++col)
		{
			positionShift[m_width*row + col] = (double)orgImgLineWidht*(double)leftIndex[tempPos] + (double)leftIndex[tempPos + 1]*(double)orgImgEleWidht;
			tempPos+= 2;
		}
	}
	tempPos = 0;
	int *rightIndex = anyangle(-30,30,0,r);
	for (int row = m_height;row < m_height*2; ++row)
	{
		for (int col = m_width/2;col <m_width-1; ++col)
		{
			positionShift[m_width*row + col] = (double)orgImgLineWidht*(double)rightIndex[tempPos] + (double)rightIndex[tempPos + 1]*(double)orgImgEleWidht;
			tempPos+= 2;
		}
	}

	if (topIndex != NULL)
	{
		free(topIndex);
		topIndex = NULL;
	}
	if (leftIndex != NULL)
	{
		free(leftIndex);
		leftIndex = NULL;
	}
	if (rightIndex != NULL)
	{
		free(rightIndex);
		rightIndex = NULL;
	}

}
