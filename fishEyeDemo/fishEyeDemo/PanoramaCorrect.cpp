#include "stdafx.h"
#include "PanoramaCorrect.h"


CPanoramaCorrect::CPanoramaCorrect()
{

}

CPanoramaCorrect::~CPanoramaCorrect()
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

void CPanoramaCorrect::ParamInit()
{
	iAngHMin = 0;
	iAngHMax = 360;
	iAngVMin = 0;
	iAngVMax = 90;
	isEqualArc = true;
	dRScale = 1.0;

}


void CPanoramaCorrect::FishEyeInit(const Mat & Image)
{
	GetArea(Image);
	Mat tmp = Standard_circle(Image);
	int width = tmp.cols;
	int height = tmp.rows;
	r= 0.5*(width<height?width:height);
	cy = (width-1)/2;
	cx= (height-1)/2;

	//输出图像的宽度
	 iWidthOut = 0;
	//输出图像的高度
	 iHeightOut = 0;

	iWidthOut = (int)ceil(r * (iAngHMax - iAngHMin)*PI/180);

	//上视角高度
	double dHeightUp = 0;
	//下视角高度
	double dHeightDown = 0;

	if (iAngVMin != -90 && iAngVMax != 90)
	{
		iHeightOut =(int)ceil( r * (tan(iAngVMax * PI / 180) - tan(iAngVMin * PI / 180)));
	}
	else if (iAngVMin < -89 && iAngVMax == 0)
	{
		isEqualArc = true;
		dHeightUp = r * dRScale;
		dHeightDown = 0;
		iHeightOut = (int)ceil(dHeightUp + dHeightDown);
	}
	else if (iAngVMin < -89 && iAngVMax > 89)
	{
		isEqualArc = true;
		dHeightUp = r * dRScale;
		dHeightDown = r * dRScale;
		iHeightOut = (int)ceil(dHeightUp + dHeightDown);
	}
	else if (iAngVMin == 0 && iAngVMax > 89)
	{
		isEqualArc = true;
		dHeightUp = 0;
		dHeightDown =r * dRScale;
		iHeightOut = (int)ceil(dHeightUp + dHeightDown);
	}
	else
	{
		iHeightOut = (int)ceil(r * (tan((double)iAngVMax) - tan((double)iAngVMin)));
	}

	dstImg = Mat::zeros(iHeightOut,iWidthOut,CV_8UC3);

	RX_Out = 0;
	RY_Out = 0;

	position = (int*)malloc(iWidthOut*2*iHeightOut*sizeof(int));
	positionShift = (long*)malloc(iWidthOut*iHeightOut*sizeof(long));

	CalcPositon();
	FixPosition(tmp);
}

void CPanoramaCorrect::ImageCorrect(const Mat& orgImg,Mat& dst,int winID)
{
	//Mat tmp = orgImg(correctArea);
	uchar* dstImgData = dstImg.data; 
	unsigned int temDstPos(0);
	uchar* orgImgData = orgImg.data;

	for (int i = 0;i < iHeightOut;i++)
	{
		for (int j = 0; j < iWidthOut; j++)
		{
			memcpy(dstImgData + temDstPos,orgImgData+positionShift[iWidthOut*i+j],3);
			temDstPos +=3;
		}
	}

	dstImg.copyTo(dst);
	//imwrite("dst.jpg",dst);
}

/// <summary>
/// 根据校正后坐标计算原图像坐标（圆柱体模型，镜头朝上）
/// </summary>
/// <param name="dU">校正后坐标X</param>
/// <param name="dV">校正后坐标Y</param>
/// <param name="dX">原图像坐标X</param>
/// <param name="dY">原图像坐标Y</param>
void CPanoramaCorrect::UV2XY_Cylinder(double dU, double dV, double &dX,  double &dY)
{
	dX = 0;
	dY = 0;
	//圆柱的弧度（0-2Pi）
	double dCylinderH = dV * (iAngHMax - iAngHMin) / dstImg.cols;
	dCylinderH +=iAngHMin;

	//圆的弧度（最上270，最下90）
	double dCirle = (270 - dCylinderH) * PI / 180;

	if (isEqualArc)
	{
		double dCylinderV = (iAngVMax - dU * (iAngVMax - iAngVMin) / dstImg.rows) * PI / 180;
		dY = (r * cos(dCylinderV)) * cos(dCirle);
		dX = (r * cos(dCylinderV)) * sin(dCirle);
	}
	else
	{
		double dCylinderV = atan((r * tan(iAngVMax * PI / 180) - dU) /r);
		dY = (r * cos(dCylinderV)) * cos(dCirle);
		dX = (r * cos(dCylinderV)) * sin(dCirle);
	}
}


void CPanoramaCorrect::CalcPositon()
{
	int x1, y1;
	int tempPos(0);
	for (int i = 0;i < iHeightOut;i++)
	{
		for (int j = 0; j < iWidthOut; j++)
		{
			double x = 0;
			double y = 0;
			UV2XY_Cylinder(i - RX_Out, j - RY_Out,  x ,y);
			double xRea = 0;
			double yRea = 0;
			xRea = x + cx;
			yRea = y + cy;
			x = xRea;
			y = yRea;
			x1 = (int)floor(x );
			y1 = (int)floor(y);
			
			if (x1 < 0)
			{
				x1 = 0;
			}
			if (x1> cx*2-1)
			{
				x1 = cx*2-1;
			}
			if (y1<0)
			{
				y1 = 0;
			}
			if (y1>cy*2-1)
			{
				y1 = cy*2-1;
			}

			tempPos = i*iWidthOut*2 + j*2;
			position[tempPos] = x1;
			position[tempPos + 1] = y1;
		}
	}
}


void CPanoramaCorrect::FixPosition(const Mat& orgImg)
{
	int orgImgLineWidht = orgImg.step[0];
	int orgImgEleWidht = orgImg.step[1];
	long tempPos(0);
	for (int row = 0;row < iHeightOut; ++row)
	{
		for (int col = 0;col <iWidthOut; ++col)
		{
			tempPos = (row * iWidthOut * 2) + (col << 1);
			positionShift[iWidthOut*row + col] = (double)orgImgLineWidht*(double)position[tempPos] + (double)position[tempPos + 1]*(double)orgImgEleWidht;
		}
	}
}
