#pragma once


class CFishEyeBase
{
public:
	CFishEyeBase();
	virtual ~CFishEyeBase();

protected:
	Rect correctArea; //��������
	double cx;  //������������X
	double cy;//������������Y
	double r;    //���۰뾶
public:
	virtual void ParamInit();  // ������ʼ��
	virtual void FishEyeInit(const Mat & Image); //У����ʼ��
	virtual Rect GetArea(const Mat &inputImage);  //�õ���������
	virtual void ImageCorrect(const Mat& orgImg,Mat& dstImg,int winID);  //У������
	virtual 	Mat Standard_circle(const Mat& img); //����Բת��Ϊ��׼Բ
private:
	virtual void FindCorrectArea(const Mat &inputImage);   //Ѱ����������
};