#pragma once


class CFishEyeBase
{
public:
	CFishEyeBase();
	virtual ~CFishEyeBase();

protected:
	Rect correctArea; //鱼眼区域
	double cx;  //鱼眼中心坐标X
	double cy;//鱼眼中心坐标Y
	double r;    //鱼眼半径
public:
	virtual void ParamInit();  // 参数初始化
	virtual void FishEyeInit(const Mat & Image); //校正初始化
	virtual Rect GetArea(const Mat &inputImage);  //得到鱼眼区域
	virtual void ImageCorrect(const Mat& orgImg,Mat& dstImg,int winID);  //校正函数
	virtual 	Mat Standard_circle(const Mat& img); //将椭圆转化为标准圆
private:
	virtual void FindCorrectArea(const Mat &inputImage);   //寻找鱼眼区域
};