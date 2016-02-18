#include "stdafx.h"
#include "UpDownCorrect.h"

CUpDownCorrect::CUpDownCorrect()
{

}

CUpDownCorrect::~CUpDownCorrect()
{
	if (position!= NULL)
	{
		free(position);
		position = NULL;
	}
	if (positionShift != NULL)
	{
		free(positionShift);
		positionShift = NULL;
	}
}



void CUpDownCorrect::FishEyeInit(const Mat & Image)
{
	GetArea(Image);
	Mat img =  Standard_circle(Image);
	r=1.0*img.cols/2;
	cx=1.0*img.cols/2;
	cy=1.0*img.cols/2;
	m_height=r;
	m_width=(int)(183*PI*r/180);
	CalcPositon();
	positionShift = (long*)malloc(m_width*m_height*2*sizeof(long));
	FixPosition(img);
}

void CUpDownCorrect::ImageCorrect(const Mat& orgImg,Mat& dstImg,int winID)
{
	//Mat img =  Standard_circle(orgImg);
	unsigned int temDstPos(0);
	uchar* orgImgData = orgImg.data;
	if (winID ==1)
	{
		dstImg = Mat::zeros(m_height,m_width,CV_8UC3);
		uchar* dstImgData = dstImg.data; 
		for (int i = 0;i<m_height;i++)
		{
			for (int j = 0;j<m_width;j++)
			{
				memcpy(dstImgData + temDstPos,orgImgData+positionShift[m_width*i+j],3);
				temDstPos +=3;
			}
		}
	//	imwrite("test.jpg",dstImg);
	}
	else if (winID == 2)
	{

		dstImg = Mat::zeros(m_height,m_width,CV_8UC3);
		uchar* dstImgData = dstImg.data; 
		for (int i = 0;i<m_height;i++)
		{
			for (int j = 0;j<m_width;j++)
			{
				memcpy(dstImgData + temDstPos,orgImgData+positionShift[m_width*(i+m_height)+j],3);
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
				memcpy(dstImgData + temDstPos,orgImgData+positionShift[m_width*i+j],3);
				temDstPos +=3;
			}
		}

	}

}


void CUpDownCorrect::CalcPositon()
{
	int R=r;
	float theta=360;
	int r0=0;
	int x3=0;
	float x4=0;
	float gap=PI*R/m_width;
	int tempPos(0);
	position = (int*)malloc(m_width*2*m_height*2*sizeof(int));
	int offset = m_width*2*m_height;

	for(r0=R;r0>0;r0--)
	{
		for (theta=360;theta>=177;)
		{   
		
			int u0 = m_height-r0;
			int v0  = m_width-x3;

			if (u0<0)
			{
				u0 = 0;
			}
			if (u0 >=m_height)
			{
				u0 = m_height -1;
			}

			if (v0<0)
			{
				v0 = 0;
			}
			if (v0 >=m_width)
			{
				v0 = m_width -1;
			}

			int j00=(int)(cx+r0*cos(PI*theta/180));//列  上半部分展开图
			int i00=(int)(cy+r0*sin(PI*theta/180));//行

			tempPos = u0*m_width*2 + v0*2;
			position[tempPos] = i00;
			position[tempPos + 1] = j00;

			int j01=(int)(cx+r0*cos(PI*(theta-180)/180)) ;//列  下半部分展开图
			int i01=(int)(cy+r0*sin(PI*(theta-180)/180));//行
			tempPos = tempPos+offset;
			position[tempPos] = i01;
			position[tempPos + 1] = j01;

			x4=x4+gap;
			int temp =int( x4 + 0.5);
			x3=(int)x4;
			if (temp>x3)
			{
				x3 = temp;
			} 
		
			float f = (floor(gap/(float)(PI*R)*183.0*100))/100;
			theta=theta-f;
		}

		x3=0;
		x4=0;
	}	

}


void CUpDownCorrect::FixPosition(const Mat& orgImg)
{
	int orgImgLineWidht = orgImg.step[0];
	int orgImgEleWidht = orgImg.step[1];
	long tempPos(0);
	for (int row = 0;row < m_height*2; ++row)
	{
		for (int col = 0;col <m_width; ++col)
		{
			tempPos = (row * m_width * 2) + (col << 1);
			positionShift[m_width*row + col] = (double)orgImgLineWidht*(double)position[tempPos] + (double)position[tempPos + 1]*(double)orgImgEleWidht;
		}
	}
}
