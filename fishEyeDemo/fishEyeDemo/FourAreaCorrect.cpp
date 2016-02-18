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
*����תΪ����
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
*���캯������ʼ��piParam
******************************/

CFourAreaCorrect::CFourAreaCorrect():piParam(0.01745329)
{

}
//������ʼ��
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


//У����ʼ��
void CFourAreaCorrect::FishEyeInit(const Mat & Image)
{
	//����ռ�
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
*���ܣ������������
*�����������������Ŀ��
*�����������
*����ֵ����
******************************/
void CFourAreaCorrect::CalculateParam(int width, int height)
{
	r=0.5*(width<height?width:height);
	f= r*2/PI;
	cx=(width-1)/2;  
	cy=(height-1)/2;   
}


/*****************************
*���ܣ�չ��ͼ��������������
*���������չ��ͼ�����꣬չ��ͼ�ķ�λ�ǣ����ǣ��ӽ�
*������������������еĶ�Ӧ����
*����ֵ����
******************************/

void CFourAreaCorrect::Fix(int &u,int &v,double &x,double &y,const double &phi,const double &sita,const double &gama)
{
	/*չ��ƽ����У��ͼ������ƽ����ռ������ϵ*/
	double wp,hp,dux,duy,duz,dvx,dvy,dvz,xp,yp,zp,SQRT,sit,c_p,s_p;
	//1
	/*չ��ͼ���ϵ�һ�����ش�С��Ӧ͸��ͶӰƽ���ϵĿ�ߴ�С*/
	wp=r*tan(gama/2)/(cx+0.5);     
	hp=r*tan(gama/2)/(cy+0.5);     
	//2
	/*չ��ͼU����������λ���ض�Ӧ�������ϵx,y,z�������ϵı仯��*/
	dux=wp*sin(phi),duy=-wp*cos(phi),duz=0;
	/*չ��ͼV����������λ���ض�Ӧ�������ϵx,y,z�������ϵı仯��*/
	dvx=-hp*cos(sita)*cos(phi),dvy=-hp*cos(sita)*sin(phi);
	dvz=hp*sin(sita);
	//3
	/*չ��ͼ��һ����������������ϵx,y,z������ֵ*/
	xp=r*sin(sita)*cos(phi)+(cx-u)*dux+(v-cy)*dvx;
	yp=r*sin(sita)*sin(phi)+(cx-u)*duy+(v-cy)*dvy;
	zp=r*cos(sita)+(v-cy)*dvz;
	//4
	/*���б�߳���*/
	SQRT=sqrt(xp*xp+yp*yp);
	/*�������*/
	sit=atan(SQRT/zp);
	/*��cosֵ*/
	c_p=xp/SQRT;   
	/*��sinֵ*/          
	s_p=yp/SQRT;
	//5
	/*��Ӧ����ͼ������*/
	x=cx+f*sit*c_p;
	y=cy+f*sit*s_p;
}

/*****************************
*���ܣ������ķ�չ��ͼ��Ӧ������ͼ��������
*�������������ͼ
*�����������
*����ֵ����
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
*���ܣ������õĶ�ά����������һά������ָ����ת��ʱ
*�������������ͼ��
*�����������
*����ֵ����
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
*���ܣ�����ͼУ��
*�������������ͼ��У��ͼ�񡢴��ںš�
*�����������
*����ֵ����
******************************/

void CFourAreaCorrect::ImageCorrect(const Mat& src,Mat& dst,int flag)
{
	//orgImg = src(correctArea);
	uchar* dstImgData = dstImg.data; 
	uchar* orgImgData = src.data;
	unsigned int temDstPos(0);
	switch(flag)
	{
		/* �ķ���ͼ����Ϣ */
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
		/* ����һȫ��ͼ����Ϣ */    	
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
		/* �����ȫ��ͼ����Ϣ */       
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
		/* ������ȫ��ͼ����Ϣ */  
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
		/* ������ȫ��ͼ����Ϣ */  
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

