// ImgProcess.h: interface for the CImgProcess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGE_H__D566CE6A_BA81_4157_8E6A_748DA096668F__INCLUDED_)
#define AFX_IMAGE_H__D566CE6A_BA81_4157_8E6A_748DA096668F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Img.h"

#include "math.h"
#include <complex>
#include <vector>
using namespace std;

extern float Template_Log[];
extern float Template_VSobel[];
extern float Template_HSobel[];
extern float Template_Smooth_Gauss[];
extern float Template_Smooth_Avg[];
extern float Template_Laplacian1[];
extern float Template_Laplacian2[];


#define PI 3.1415926


struct SLineInfo
{
	int nPixels;
	int nAngle;  // -180   +180
	int nDist;   // 0      +Max
	int GetLineAngle(SLineInfo *pLine)
	{
		
		int nRet = abs(pLine->GetAngle360() - GetAngle360());
		if(nRet > 180)
			nRet = 360 - nRet;

		if(nRet > 90)
			nRet = 180 - nRet;

	
		return abs(nRet);
	}

	int GetLineDist(SLineInfo *pLine)
	{
		if(nAngle * pLine->nAngle < 0)
			return abs(nDist + pLine->nDist);
	
		return abs(nDist - pLine->nDist);
	}

	void AddDistWithAngle(int nAdd, int n)
	{
		if(n < 0)
		{
			nAdd *= -1;
		}

		nDist += nAdd;
	}

	int GetDistWithAngle(int n)
	{
		int nRet = nDist;
		if(n < 0)
		{
			nRet *= -1;
		}

		return nRet;
	}

	int GetAngle360()
	{
		int nRet = nAngle;
		if(nAngle < 0)
		{
			nRet = abs(nAngle + 180) + 180;
		}
		return nRet;
	}

	int GetY(int x)
	{
		int nRet = 0;
		double fRadian = nAngle*PI/180.0;


		if(sin(fRadian) != 0)
			nRet = (nDist - x*cos(fRadian))/sin(fRadian);

		return nRet;
		
	}

	int GetX(int y)
	{
		int nRet = 0;
		double fRadian = nAngle*PI/180.0;


		if(cos(fRadian) != 0)
			nRet = (nDist - y*sin(fRadian))/cos(fRadian);

		return nRet;
	}

};



struct MYPOINT
{
	double x;
	double y;
};

// CImgProcess��װ�˸���ͼ����ı�׼�㷨
class CImgProcess : public CImg  
{
public:
	CImgProcess();
	virtual ~CImgProcess();

	// �ӻ����ת��
	CImgProcess& operator = (CImg& img);




	//***************��3�� ͼ��ĵ�����*****************
	BOOL GenHist(double * hist, int n = 256);// ���ɻҶ�ֱ��ͼ
	BOOL ParLinTran(CImgProcess * pTo, BYTE x1, BYTE x2, BYTE y1, BYTE y2);//�ֶ����Ա任
	BOOL LinTran(CImgProcess * pTo, double dFa, double dFb);//���Ա任
	BOOL LogTran(CImgProcess * pTo, double C);//�����任
	BOOL GammaTran(CImgProcess * pTo, double gamma, double comp=0);//٤��任
	BOOL WindowTran(CImgProcess * pTo, BYTE lowThre, BYTE highThre);//���ڱ任
	BOOL Histeq(CImgProcess * pTo);//�ҶȾ��⻯
	BOOL Histst(CImgProcess * pTo, double* pdStdHist);//ֱ��ͼ�涨����ֱ��ƥ��ֱ��ͼ
	BOOL Histst(CImgProcess * pTo, CImgProcess* pStd);//ֱ��ͼ�涨����ƥ���׼ͼ���ֱ��ͼ
	



	//***************��4�� ͼ��ļ��α任*****************
	void ImMove(CImgProcess* pTo, int x, int y);//ͼ��ƽ��
	void HorMirror(CImgProcess* pTo);//ͼ��ˮƽ����
	void VerMirror(CImgProcess* pTo);//ͼ��ֱ����
	void Transpose(CImgProcess* pTo);//ͼ��ת��
	void Scale(CImgProcess* pTo,double times);//ͼ������
	void Rotate(CImgProcess* pTo,float ang);//ͼ����ת

	//**************ͶӰ�任��ԭ***************//////////////////
	int m_nBasePt; //=4 ��׼��Զ�����Ŀ
	BOOL ImProjRestore(CImgProcess* pTo, CPoint *pPointBase, CPoint *pPointSampl, bool bInterp);//ͶӰ����У��
	void GetProjPara(CPoint *pPointBase, CPoint *pPointSampl, double *pDbProjPara); //����4�Զ���׼��ȷ���任����
	BOOL InvMat(double** ppDbMat, int nLen); //��������
	void ProdMat(double** ppDbMat, double* pDbSrc2, double* pDbDest, int y, int x, int z);
	struct MYPOINT ProjTrans(CPoint pt, double* pDbProjPara); //���ݱ任�����Ե�ptʵʩͶӰ�任

	int InterpBilinear(double x, double y);//����(x,y)��˫���Բ�ֵ��ĻҶ�
	


	//***************��5�� ͼ����ǿ*****************
	
	// ͨ��ģ�����
	void Template(CImgProcess *pTo, int nTempH, int nTempW, int nTempMY, int nTempMX, FLOAT *pfArray, FLOAT fCoef);
	int GetMedianValue(int * pAryGray, int nFilterLen); //ȡ������ͳ����ֵ
	void MedianFilter(CImgProcess *pTo, int nFilterH, int nFilterW, int nFilterMY, int nFilterMX); //��ֵ�˲�
	void AdaptiveMedianFilter(CImgProcess *pTo, int nFilterH, int nFilterW, int nFilterMY, int nFilterMX); //����Ӧ��ֵ�˲�
	void FilterSobel(CImgProcess *pTo); //Sobel�ݶ���
	void EnhanceFilter(CImgProcess *pTo, double dProportion,
						 int nTempH, int nTempW, 
						 int nTempMY, int nTempMX, FLOAT *pfArray, FLOAT fCoef); //�����˲�


	

	//***************��6�� Ƶ����ͼ����ǿ*****************
private:
	// FFT
	void FFT(complex<double> * TD, complex<double> * FD, int r);
	// IFFT
	void IFFT(complex<double> * FD, complex<double> * TD, int r);
public:
	// FFT2
	BOOL FFT2(CImgProcess * pTo, BOOL bExpand = FALSE, complex<double> * pOutput = NULL, BYTE bFillColor = 255);
	// IFFT2
	BOOL IFFT2(CImgProcess * pTo, complex<double> * pInput, long lWidth, long lHeight, long lOutW = 0, long lOutH = 0);
	// Ƶ���˲�
	BOOL FreqFilt(CImgProcess * pTo, double * pdFilter, BYTE bFillColor = 255);
	// ����Ƶ�������ͨ�˲���
	BOOL FreqIdealLPF(double * pdFilter, int nFreq);
	// Ƶ���˹��ͨ�˲���
	BOOL FreqGaussLPF(double * pdFilter, double dSigma);
	// ��˹��ͨ�˲���
	BOOL FreqGaussHPF(double * pdFilter, double dSigma);
	// Ƶ��������˹�˲���
	BOOL FreqLaplace(double * pdFilter);
	// ��˹�����˲���
	BOOL FreqGaussBRF(double * pdFilter , int nFreq, int nWidth);

	/**************************************************
	LONG GetFreqWidth(BOOL isExtending = true)

	���ܣ�
		����Ƶ���˾���Ƶ��ͼ��Ӧ�еĿ��

	������
		BOOL isExtending
			ָ���Կ�ȵ�������ϲ�ȡ�ķ�����
			trueΪ��չ����Ӧ�ø�����ɫ����ͼ����Ҳࣻ
			falseΪѹ������Ӧ���Ҳ�ü�ͼ��
			Ĭ��ֵΪtrue��

	����ֵ��
		LONG���ͣ�������Ŀ�ȼ�����
	***************************************************/

	inline LONG GetFreqWidth(BOOL isExtending = true)
	{
		LONG w = 1;

		while(w * 2 <= GetWidthPixel())
		{
			w *= 2;
		}

		// �����Ҫ��չͼ���ȣ���ͼ���Ȳ�ǡ����2�����ݣ���
		if ( (w != GetWidthPixel()) && (isExtending) )
		{
			w *= 2;
		}

		return w;
	}

	/**************************************************
	LONG GetFreqHeight(BOOL isExtending = true)

	���ܣ�
		����Ƶ���˾���Ƶ��ͼ��Ӧ�еĸ߶�

	������
		BOOL isExtending
			ָ���Կ�ȵ�������ϲ�ȡ�ķ�����
			trueΪ��չ����Ӧ�ø�����ɫ����ͼ��ĵײ���
			falseΪѹ������Ӧ�ӵײ��ü�ͼ��
			Ĭ��ֵΪtrue��

	����ֵ��
		LONG���ͣ�������ĸ߶ȼ�����
	***************************************************/

	inline LONG GetFreqHeight(BOOL isExtending = true)
	{
		LONG h = 1;

		while(h * 2 <= GetHeight())
		{
			h *= 2;
		}

		// �����Ҫ��չͼ��߶ȣ���ͼ��߶Ȳ�ǡ����2�����ݣ���
		if ( (h != GetHeight()) && (isExtending) )
		{
			h *= 2;
		}

		return h;
	}

	


	//***************��7�� ��ɫͼ����*****************
	void CMY2RGB(CImgProcess *pTo);
	void RGB2CMY(CImgProcess *pTo);
	void RGB2HSI(CImgProcess* pTo);
	void HSI2RGB(CImgProcess *pTo);
	void Gray2RGB(CImgProcess *pTo);
	void YIQ2RGB(CImgProcess *pTo);
	void RGB2YIQ(CImgProcess *pTo);
	void YUV2RGB(CImgProcess *pTo);
	void RGB2YUV(CImgProcess *pTo);
	void HSV2RGB(CImgProcess *pTo);
	void RGB2HSV(CImgProcess *pTo);
	


	
	//**************��8�� ��̬ѧ����********************
	void FillRgn(CImgProcess *pTo, POINT ptStart); //��������㷨
	void TraceBoundary(CImgProcess *pTo); //�߽�����㷨
	void Thining(); //ϸ���㷨
	void Erode(CImgProcess* pTo, int se[3][3]); //��ʴ�㷨
	void Dilate(CImgProcess* pTo, int se[3][3]); //�����㷨
	void Convex(CImgProcess* pTo, BOOL bConstrain); //����͹��
	void Open(CImgProcess* pTo, int se[3][3]);//������
	void Close(CImgProcess* pTo, int se[3][3]);//������

	void PixelImage(CImgProcess* pTo, int lowerThres, int upperThres);
	int TestConnRgn(CImgProcess* pImage,  LPBYTE lpVisited, int nWidth, int nHeight,	int x,int y, CPoint ptVisited[], int lowerThres, int upperThres, int &curLianXuShu);
	void LabelConnRgn(CImgProcess* pTo, int nConn = 8); //��ע��ͨ����

	//�Ҷ���̬ѧ
	void GrayDilate(CImgProcess* pTo, int nTempH, int nTempW, int nTempMY, int nTempMX, int** se);//�Ҷ�����
	void GrayErode(CImgProcess* pTo, int nTempH, int nTempW, int nTempMY, int nTempMX, int** se);//�Ҷȸ�ʴ
	void GrayOpen(CImgProcess* pTo, int nTempH, int nTempW, int nTempMY, int nTempMX, int** se);//�Ҷȿ�����
	void GrayClose(CImgProcess* pTo, int nTempH, int nTempW, int nTempMY, int nTempMX, int** se);//�Ҷȱ�����
	void Tophat(CImgProcess* pTo, int nTempH, int nTempW, int nTempMY, int nTempMX, int** se);//��ñ�任




	//*********************��9�� ͼ��ָ�********************
	// Roberts��ԵѰ��
	BOOL EdgeRoberts(CImgProcess * pTo, BYTE bThre = 0, BYTE bEdgeType = 0, BOOL bThinning = true, BOOL bGOnly = false);
	// Sobel��ԵѰ��
	BOOL EdgeSobel(CImgProcess * pTo, BYTE bThre = 0, BYTE bEdgeType = 0, BOOL bThinning = true, BOOL bGOnly = false);
	// Prewitt��ԵѰ��
	BOOL EdgePrewitt(CImgProcess * pTo, BYTE bThre = 0, BYTE bEdgeType = 0, BOOL bThinning = true, BOOL bGOnly = false);
	// ��˹������˹��ԵѰ��
	void EdgeLoG(CImgProcess * pTo);
	// Canny�㷨
	BOOL EdgeCanny(CImgProcess * pTo, BYTE bThreL = 0, BYTE bThreH = 0, BOOL bThinning = true);

	// Hough�任 ����ֱ��
	BOOL Hough(SLineInfo *pInfoRet, int nLineRet);
	
	void Threshold(CImgProcess *pTo, BYTE bThre);//��ֵ�ָ�
	int DetectThreshold(int nMaxIter, int &nDiffRet);// ������ȡ���ŷ�ֵ
	void AutoThreshold(CImgProcess *pTo);//�Զ���ֵ�ָ�

	// ���������㷨
	BOOL RegionGrow(CImgProcess * pTo , int nSeedX, int nSeedY, BYTE bThre);
	// ��ˮ���㷨




	//***************��10�� ������ȡ*****************
	vector< vector<int> > GetGrayMatrix(POINT ptD1, POINT ptD2); //����Ҷȹ��־���




	//***************��11�� ʶ�����*****************
	void TemplateMatch(CImgProcess* pTo, CImgProcess* pTemplate); //ģ��ƥ��

	

};//class CImgProcess



#endif // !defined(AFX_IMAGE_H__D566CE6A_BA81_4157_8E6A_748DA096668F__INCLUDED_)
