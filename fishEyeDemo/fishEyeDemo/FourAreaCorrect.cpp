#include "stdafx.h"
#include "FourAreaCorrect.h"

CFourAreaCorrect::~CFourAreaCorrect(){
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

/*****************************
*将度转为弧度
******************************/

void CFourAreaCorrect::ParamFix()
{
	phi0 = phi0 * piParam;
	sita0 = (90 - sita0) * piParam;
	gama0 = gama0 * piParam;

	phi1 = phi1 * piParam;
	sita1 = (90 - sita1) * piParam;
	gama1 = gama1 * piParam;

	phi2 = phi2 * piParam;
	sita2 = (90-sita2)*piParam;
	gama2 = gama2 * piParam;

	phi3 = phi3 * piParam;
	sita3 = (90-sita3)*piParam;
	gama3 = gama3 * piParam;
}

/*****************************
*构造函数，初始化piParam
******************************/

CFourAreaCorrect::CFourAreaCorrect():piParam(0.01745329)
{

}
//参数初始化
void CFourAreaCorrect::ParamInit()
{
	dstWidth = 400;
	dstHeight = 300;
	wRatio = 6;
	hRatio = 5;

	phi0 = 0;
	sita0 = 50;
	gama0 = 95;

	phi1 = 180;
	sita1 = 55;
	gama1 = 100;

	phi2 = 90;
	sita2 = 55;
	gama2 = 100;

	phi3 = 270;
	sita3 = 45;
	gama3 = 85;
}


//校正初始化
void CFourAreaCorrect::FishEyeInit(const Mat & Image)
{
	//分配空间
	dstImg =  Mat::zeros(Size(dstWidth*2,dstHeight*2),CV_8UC3);
	position = (int*)malloc(dstWidth*dstHeight*8*4*sizeof(int));
	positionShift = (long*)malloc(dstWidth*dstHeight*4*4*sizeof(long));

	GetArea(Image);
	orgImg = Standard_circle(Image);
	ParamFix();
	CalculateParam(orgImg.cols,orgImg.rows);
	CalcPositon(orgImg);
	FixPosition(orgImg);

}


/*****************************
*功能：计算所需参数
*输入参数：鱼眼区域的宽高
*输出参数：空
*返回值：空
******************************/
void CFourAreaCorrect::CalculateParam(int width, int height)
{
	r=0.5*(width<height?width:height);
	f= r*2/PI;
	cx=(width-1)/2;  
	cy=(height-1)/2;   
}


/*****************************
*功能：展开图在鱼眼区域坐标
*输入参数：展开图中坐标，展开图的方位角，仰角，视角
*输出参数：鱼眼区域中的对应坐标
*返回值：空
******************************/

void CFourAreaCorrect::Fix(int &u,int &v,double &x,double &y,const double &phi,const double &sita,const double &gama)
{
	/*展开平面与校正图比例，平面与空间坐标关系*/
	double wp,hp,dux,duy,duz,dvx,dvy,dvz,xp,yp,zp,SQRT,sit,c_p,s_p;
	//1
	/*展开图像上的一个像素大小对应透视投影平面上的宽高大小*/
	wp=r*tan(gama/2)/(cx+0.5);     
	hp=r*tan(gama/2)/(cy+0.5);     
	//2
	/*展开图U坐标正方向单位像素对应相机坐标系x,y,z三个轴上的变化量*/
	dux=wp*sin(phi),duy=-wp*cos(phi),duz=0;
	/*展开图V坐标正方向单位像素对应相机坐标系x,y,z三个轴上的变化量*/
	dvx=-hp*cos(sita)*cos(phi),dvy=-hp*cos(sita)*sin(phi);
	dvz=hp*sin(sita);
	//3
	/*展开图中一个坐标在球面坐标系x,y,z的坐标值*/
	xp=r*sin(sita)*cos(phi)+(cx-u)*dux+(v-cy)*dvx;
	yp=r*sin(sita)*sin(phi)+(cx-u)*duy+(v-cy)*dvy;
	zp=r*cos(sita)+(v-cy)*dvz;
	//4
	/*求出斜边长度*/
	SQRT=sqrt(xp*xp+yp*yp);
	/*求入射角*/
	sit=atan(SQRT/zp);
	/*求cos值*/
	c_p=xp/SQRT;   
	/*求sin值*/          
	s_p=yp/SQRT;
	//5
	/*对应鱼眼图中坐标*/
	x=cx+f*sit*c_p;
	y=cy+f*sit*s_p;
}

/*****************************
*功能：建立四幅展开图对应的鱼眼图的索引表
*输入参数：鱼眼图
*输出参数：空
*返回值：空
******************************/

void CFourAreaCorrect::CalcPositon(Mat &fishImage )
{
	double x1 = 0 ;
	double y1 = 0;
	int tempPos(0);
	hRatio /= 2;
	wRatio /= 2;
	for (int row = 0;row < dstHeight*2 ; ++row)
	{
		for (int col = 0;col < dstWidth*2; ++col)
		{
			int temprow = row * hRatio;
			int tempcol = col * wRatio;
			tempPos = row*dstWidth*8 + col*2;
			Fix(temprow,tempcol, x1,y1,phi0,sita0,gama0);
			position[tempPos] = (int)x1;
			position[tempPos + 1] = (int)y1;

			Fix(temprow,tempcol, x1,y1,phi1,sita1,gama1);
			position[tempPos += dstWidth*4] = (int)x1;
			position[tempPos + 1] = (int)y1;

			tempPos = (row+dstHeight*2)*8*dstWidth + col*2;
			Fix(temprow,tempcol, x1,y1,phi2,sita2,gama2);
			position[tempPos] = (int)x1;
			position[tempPos + 1] = (int)y1;

			Fix(temprow,tempcol, x1,y1,phi3,sita3,gama3);
			position[tempPos += dstWidth*4] = (int)x1;
			position[tempPos + 1] = (int)y1;
		}
	}

}


/*****************************
*功能：将建好的二维索引表拉成一维，减少指针跳转耗时
*输入参数：鱼眼图像
*输出参数：空
*返回值：空
******************************/
void CFourAreaCorrect::FixPosition(const Mat& orgImg)
{
	int orgImgLineWidht = orgImg.step[0];
	int orgImgEleWidht = orgImg.step[1];
	long tempPos(0);
	for (int row = 0;row < dstHeight*4; ++row)
	{
		for (int col = 0;col <dstWidth*4; ++col)
		{
			tempPos = (row * dstWidth * 8) + (col << 1);
			positionShift[dstWidth*row*4 + col] = (double)orgImgLineWidht*(double)position[tempPos] + (double)((position[tempPos + 1])<<1) + (double)position[tempPos + 1];
		}
	}
}


/*****************************
*功能：鱼眼图校正
*输入参数：鱼眼图像、校正图像、窗口号、
*输出参数：空
*返回值：空
******************************/

void CFourAreaCorrect::ImageCorrect(const Mat& src,Mat& dst,int flag)
{
	//orgImg = src(correctArea);
	uchar* dstImgData = dstImg.data; 
	uchar* orgImgData = src.data;
	unsigned int temDstPos(0);
	switch(flag)
	{
		/* 四分屏图像信息 */
	case 0:
		{
			for (int row = 0;row < dstHeight*4; row += 2 )
			{
				for (int col = 0;col <dstWidth*4; col += 2)
				{
					memcpy(dstImgData + temDstPos,orgImgData+positionShift[dstWidth*row*4+col],3);
					temDstPos +=3;
				}
			}
		};
		break;
		/* 区域一全屏图像信息 */    	
	case 1:
		{
			for (int row = 0;row < dstHeight*2; ++row)
			{
				for (int col = 0;col <dstWidth*2; ++col)
				{
					memcpy(dstImgData + temDstPos,orgImgData+positionShift[dstWidth*row*4+col],3);
					temDstPos +=3;
				}
			}
		};
		break;
		/* 区域二全屏图像信息 */       
	case 2:
		{
			for (int row = 0;row < dstHeight*2; ++row)
			{
				for (int col = dstWidth*2;col <dstWidth*4; ++col)
				{
					memcpy(dstImgData + temDstPos,orgImgData+positionShift[dstWidth*row*4+col],3);
					temDstPos +=3;
				}
			}
		};
		break;
		/* 区域三全屏图像信息 */  
	case 3:
		{
			for (int row = dstHeight*2;row < dstHeight*4; ++row)
			{
				for (int col = 0;col <dstWidth*2; ++col)
				{
					memcpy(dstImgData + temDstPos,orgImgData+positionShift[dstWidth*row*4+col],3);
					temDstPos +=3;
				}
			}
		};
		break;
		/* 区域四全屏图像信息 */  
	case 4:
		{  
			for (int row = dstHeight*2;row < dstHeight*4; ++row)
			{
				for (int col = dstWidth*2;col <dstWidth*4;++col)
				{
					memcpy(dstImgData + temDstPos,orgImgData+positionShift[dstWidth*row*4+col],3);
					temDstPos +=3;
				}
			}
		};
		break;
	default:
		break;
	}
	dstImg.copyTo(dst);
}

