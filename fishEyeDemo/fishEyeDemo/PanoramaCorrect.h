#pragma once
#include "FishEyeBase.h"

class CPanoramaCorrect:public CFishEyeBase
{
public:
	CPanoramaCorrect();
	virtual ~CPanoramaCorrect();

private:
	/// ���ˮƽ�ӽ�(��ͷ���ϣ�0��360�� ��ͷ��ǰ 0��180��)   
	int iAngHMax;
	/// ��Сˮƽ�ӽ�(��ͷ���ϣ�0��360�� ��ͷ��ǰ 0��180��)
	int iAngHMin;
	/// ���ֱ�ӽ�(��ͷ���ϣ�0��90�� ��ͷ��ǰ -90��90��))
	int iAngVMax;
	/// ��С��ֱ�ӽ�(��ͷ���ϣ�0��90�� ��ͷ��ǰ -90��90��)
	int iAngVMin;
	/// �Ƿ��յȻ�������
	bool isEqualArc;
	double dRScale;

	Mat dstImg;
	//���ͼ��Ŀ��
	int iWidthOut;
	//���ͼ��ĸ߶�
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