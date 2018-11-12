// OCRImageProcess.cpp: implementation of the COCRImageProcess class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../DigitRec.h"
#include "OCRImgProcess.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COCRImageProcess::COCRImageProcess()
{

}

COCRImageProcess::~COCRImageProcess()
{

}

// �ӻ����ת��
COCRImageProcess& COCRImageProcess::operator = (CImg& img)
{
	// ����ͼ���С
	if ( IsValidate() )
	{
		ImResize(img.GetHeight(), img.GetWidthPixel());
	}
	else
	{
		throw "Error in operaor '=': Left hand operand not initialized.";
		return *this;
	}

	// ����ͼ������
	for (int i=0; i<GetHeight(); i++)
	{
		memcpy(m_lpData[i], img.m_lpData[i], img.GetWidthByte() * sizeof(BYTE));
	}

	return *this;
}



/******************* 
void COCRImageProcess::SlopeAdjust(COCRImageProcess* pTo)
���ܣ�
	����ͼ����ǰ���������б�ȣ�ʹ�価������һ��ˮƽλ���ϡ��Ƚ��ʺ��ں���ֲ������壬���ַ��ĵ���
 ע��
	ֻ�ܴ���2ֵͼ��

 ������
 COCRImageProcess* pTo: Ŀ��ͼ��� COCRImageProcess ָ��
 
 ����ֵ��
	��
 *******************/

void COCRImageProcess::SlopeAdjust(COCRImageProcess* pTo)
{
	int i,j;
	double dAvgLHeight = 0; //ͼ����벿��ǰ�������ƽ���߶�
	double dAvgRHeight = 0; //ͼ���Ұ벿��ǰ�������ƽ���߶�

	//ȡ��ͼ��ĸߺͿ�
	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	int nGray;
	int nWeightSum = 0; //ͳ��ƽ����Ȩ�߶�ʱ��Ȩ�غ�

	//����ɨ����벿��ͼ�񣬼����ɫ���ص�ƽ����Ȩ�߶�
	for(i=0; i<nHeight; i++)
	{
		for(j=0; j<nWidth/2; j++)
		{
			nGray = GetGray(j, i);
			if(nGray == 0)//��ǰ�����壨�ڣ�
			{
				//����߶ȵļ�Ȩ�ͣ��������ߵ����ط�������Ȩ��
				nWeightSum += nWidth/2 - j;
				dAvgLHeight += i*(nWidth/2 - j);
			}
		}//for j
	}//for i

	dAvgLHeight /= nWeightSum; //ƽ����Ȩ�߶�
	nWeightSum = 0;

	//����ɨ���Ұ벿��ͼ�񣬼����ɫ���ص�ƽ����Ȩ�߶�
	for(i=0; i<nHeight; i++)
	{
		for(j=nWidth/2; j<nWidth; j++)
		{
			nGray = GetGray(j, i);

			if(nGray == 0)//��ǰ�����壨�ڣ�
			{
				//����߶ȵļ�Ȩ�ͣ��������ߵ����ط�������Ȩ��
				nWeightSum += j - nWidth/2;
				dAvgRHeight += i*(j - nWidth/2);			
			}
		}//for j
	}//for i

	dAvgRHeight /= nWeightSum;

	//����б��
	double dSlope = (dAvgLHeight - dAvgRHeight) / (nWidth/2);
	int nYSrc; //y��Դ����

	//ɨ����ͼ�񣬸���б�ʵõ����¾�ͼ���ӳ���ϵΪÿһ�����ظ�ֵ
	for(i=0; i<nHeight; i++)
	{
		for(j=0; j<nWidth; j++)
		{
			//�ҵ�����ͼ��ĵ�ǰ���Ӧ�ľ�ͼ����ˮƽ����(��ˮƽ�����е�Ϊ����)
			nYSrc = int(i - (j - nWidth/2) * dSlope);
			if( nYSrc < 0 || nYSrc >= nHeight ) //��Ӧ���ڲ���ͼ������֮��
				nGray = 255;
			else
				nGray = GetGray(j, nYSrc);
			pTo->SetPixel(j, i, RGB(nGray, nGray, nGray)); //����Դͼ���Ӧ������ֵΪ��ͼ�����ظ�ֵ
		}//for j
	}//for i
}



/******************* 
vector<RECT> COCRImageProcess::ObjectSegment()
���ܣ�
	��ǰ��Ŀ�꣨���ַ������л��֣��������ַ��������η���
 ע��
	ֻ�ܴ���2ֵͼ��

 ������
	��
 
 ����ֵ��
	vecRECT: ���������ַ��������ε�RECT����
 *******************/
vector<RECT> COCRImageProcess::ObjectSegment()
{	
	vector<RECT> vecRoughRECT; //���Զ��������ľ�����������
	vector<RECT> vecRECT; //��������������ľ�����������

	//���������ʾÿ����������� vector
	vecRoughRECT.clear();
	vecRECT.clear();

	int i, j;
	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();
	
	int nTop, nBottom; // ����ǰ����������±߽�
	int nGray; //���ػҶ�
	int nObjCnt = 0; //������Ŀ

	//������������ɨ�裬�ҵ�����ǰ��������ϱ߽�
	for(i=0; i<nHeight; i++)
	{
		for(j=0; j<nWidth; j++)
		{
			nGray = GetGray(j, i);
			if(nGray == 0)
			{
				nTop = i;
				i = nHeight; //��i����ֵ��ʹ����break�����ڲ�ѭ����ֱ�ӿ������������ѭ��
				break;
			}
		}// for j
	}// for i

	//������������ɨ�裬�ҵ�����ǰ��������±߽�
	for(i=nHeight-1; i>=0; i--)
	{
		for(j=0; j<nWidth; j++)
		{
			nGray = GetGray(j, i);
			if(nGray == 0)
			{
				nBottom = i;
				i = -1; //��i��Сֵ��ʹ����break�����ڲ�ѭ����ֱ�ӿ������������ѭ��
				break;
			}
		}// for j
	}// for i

	bool bStartSeg = false; //�Ƿ��ѿ�ʼĳһ������ķָ�
	bool bBlackInCol; //ĳһ�����Ƿ������ɫ����

	RECT rt;

	//����ɨ�裬�ҵ�ÿһ��Ŀ������ұ߽�
	for(j=0; j<nWidth; j++)
	{
		bBlackInCol = false;
		for(i=0; i<nHeight; i++)
		{
			nGray = GetGray(j, i);
			if( nGray == 0 )
			{
				bBlackInCol = true; //�����з��ֺڵ�
				if(!bStartSeg) //��û�н���һ������ķָ�
				{
					rt.left = j;

					bStartSeg = true; //Ŀ��ָʼ
				}
				else //�Դ���ĳһ������֮��
					break;
			}// if( gray == 0 )
		}// for i

		if( j == (nWidth-1) ) //ɨ�赽����һ����,˵������ͼ��ɨ�����
			break;
		if(bStartSeg && !bBlackInCol) //�����ڷָ�״̬��ɨ����һ�ж�û�з��ֺ����أ�������ǰ����ָ����
		{
			rt.right = j; //�����ұ߽�ȷ��

			//����Ĵ������±߽磨�д�������
			rt.top = nTop;
			rt.bottom = nBottom;

			::InflateRect(&rt, 1, 1); //���ο�����1�����أ��������ʱѹ���ַ�

			vecRoughRECT.push_back(rt); //���� vector

			bStartSeg = false; //��ǰ�ָ����
			nObjCnt ++; //������Ŀ��1
		}

		//������һ�е�ɨ��
	}// for j
	
	RECT rtNew; //��ž�����������ľ��ο�

	//�����Ѿ��õ��˾�ȷ�����ұ߽磬���ڿ��Ծ������ο�����±߽�
	int nSize = vecRoughRECT.size();
	for(int nObj=0; nObj<nSize; nObj++)
	{

		rt = vecRoughRECT[nObj];

		rtNew.left = rt.left - 1;
		rtNew.right = rt.right + 1;

		//������������ɨ��ȷ���ϱ߽�
		for(i=rt.top; i<rt.bottom; i++)
		{
			for(j=rt.left; j<rt.right; j++)
			{
				nGray = GetGray(j, i);
				if(nGray == 0)
				{
					rtNew.top = i-1;
					i = rt.bottom; //��i����ֵ��ʹ����break�����ڲ�ѭ����ֱ�ӿ������������ѭ��
					break;
				}
			}// for j
		}//for i


		//������������ɨ��ȷ���±߽�
		for(i=rt.bottom-1; i>=rt.top; i--)
		{
			for(j=rt.left; j<rt.right; j++)
			{
				nGray = GetGray(j, i);
				if(nGray == 0)
				{
					rtNew.bottom = i+1;
					i = rt.top-1; //��i��Сֵ��ʹ����break�����ڲ�ѭ����ֱ�ӿ������������ѭ��
					break;
				}
			}// for j
		}//for i

		vecRECT.push_back(rtNew);
	}//for nObj

	return vecRECT; //���ذ����˸����ָ���ַ�����������
}




/******************* 
void COCRImageProcess::ObjectNorm(COCRImageProcess* pTo, int nTargWidth, int nTargHeight, vector<RECT> &vecRT)
���ܣ�
	�Ը���������гߴ�Ĺ�һ�����������Ǿ�����ͬ�Ŀ�͸ߣ��Է�����������ȡ��
	Ӧ����ȡ�˶�����������֮��ʹ��
ע��
	ֻ�ܴ���2ֵͼ��
	��Ҫ��������������Ϊ���������Ӧ����ȡ�˶�����������֮��ʹ��

������
	COCRImageProcess* pTo: Ŀ��ͼ��� COCRImageProcess ָ��
	nTargWidth: ��һ����Ŀ����
	nTargHeight: ��һ����Ŀ��߶�
	vecRT: ��ʾ�ַ��ָ�ľ�����������
 
����ֵ��
	��
*******************/
void COCRImageProcess::ObjectNorm(COCRImageProcess* pTo, int nTargWidth, int nTargHeight, vector<RECT> &vecRT)
{
	pTo->InitPixels(255); //Ŀ��ͼ���ð�

	int nSize = vecRT.size(); //ȡ�ö���(�ַ�)����������Ŀ


	//����ÿ���������ε���׼�ߴ�
	for(int nObj=0; nObj<nSize; nObj++)
	{
		RECT rt = vecRT[nObj]; //ȡ��һ����������
		
		vecRT[nObj] = RgnZoom(pTo, nTargWidth, nTargHeight, &rt);
	}
	
}

 



/******************* 
void COCRImageProcess::RgnZoom(COCRImageProcess* pTo, int nTargWidth, int nTargHeight, LPRECT lpRect)
���ܣ�
	�� pTo ָ���ͼ������ lpRect ָʾ�ľ������������� nTargWidth ��nTargHeight ��
ע��
	ֻ�ܴ���2ֵͼ��nTargWidth �� nTargHeight �����Ĵ�С���ܳ���ͼ��Χ

������
	COCRImageProcess* pTo: Ŀ��ͼ��� COCRImageProcess ָ��
	nTargWidth: ��һ����Ŀ����
	nTargHeight: ��һ����Ŀ��߶�
	lpRect: ��׼������֮ǰ����������
 
����ֵ��
	���ź���������
********************/
RECT COCRImageProcess::RgnZoom(COCRImageProcess* pTo, int nTargWidth, int nTargHeight, LPRECT lpRect)
{
	RECT retRT; //���ź���������

	double dXScale; //ˮƽ�������������
	double dYScale; //��ֱ�������������

	int i, j;
	
	
	//ȷ������ϵ��
	dXScale = (double)nTargWidth / (lpRect->right - lpRect->left + 1);
	dYScale = (double)nTargHeight / (lpRect->bottom - lpRect->top + 1);

	int nSrc_i, nSrc_j; //ӳ��Դ����


	retRT.top = lpRect->top;
	retRT.bottom = retRT.top + nTargHeight;
	retRT.left = lpRect->left;
	retRT.right = retRT.left + nTargWidth;

	//����ͼ������ɨ�裬ͨ������ӳ���������
	for(i=retRT.top; i<retRT.bottom; i++)
	{
		for(j=retRT.left; j<retRT.right; j++)
		{
			//����ӳ���Դ����(����ڲ�ֵ)
			nSrc_i = retRT.top + int( (i-retRT.top) / dYScale ) ;
			nSrc_j = retRT.left + int( (j-retRT.left) / dXScale ) ;

			//��Ӧ���ظ�ֵ
			int nGray = GetGray(nSrc_j, nSrc_i);
			pTo->SetPixel( j, i, RGB(nGray, nGray, nGray) );

		}//for j
	}//for i

	return retRT;
}

/******************* 
void COCRImageProcess::DelScatterNoise(COCRImageProcess* pTo, int lowerThres, int upperThres)
���ܣ�
	ȥ��ͼ���е���ɢ����:�˳���С���� lowerThres ����ͨ����;�˳���С���� upperThres ����ͨ����;��������
ע��
	ֻ�ܴ���2ֵͼ��

������
	COCRImageProcess* pTo: Ŀ��ͼ��� COCRImageProcess ָ��
	lowerThres: ������ֵ
	upperThres: ������ֵ
����ֵ��
	��
********************/
void COCRImageProcess::DelScatterNoise(COCRImageProcess* pTo, int lowerThres, int upperThres)
{
	
	if(upperThres < lowerThres)
	{
		AfxMessageBox("������ֵ�������������ֵ��");
		return;
	}

	if(lowerThres < 0)
		lowerThres = 0;
	if(upperThres > 1500)
		upperThres = 1500; //Ϊ��ֹ��ȵݹ�ջ�洦���޶�upperThres�����ֵΪ1000

	COCRImageProcess image_bkp = *this;
	COCRImageProcess image_res = *pTo;

	int nHeight = pTo->GetHeight();
	int nWidth = pTo->GetWidthPixel();

	int i,j;
	unsigned char pixel;

	LPBYTE lpVisited = new BYTE[nHeight*nWidth]; //��Ǹ�λ���Ƿ��ѱ����ʹ�

	for(i=0;i<nHeight*nWidth;i++)
		lpVisited[i] = false; //��ʼ���ʱ������

	int curConnRgnSize = 0; //��ǰ���ֵ���ͨ���Ĵ�С

	
	int nPtArySize = upperThres + 10; //��¼���ʵ���������Ĵ�С����һ������С�� upperThres ����
	CPoint* ptVisited;//��¼������ͨ����һ��̽���з��ʹ��ĵ������
	ptVisited = new CPoint[nPtArySize]; 

	int k = 0;

	for(i=0;i<nHeight;i++)
	{
		for(j=0;j<nWidth;j++)
		{
			
			for(k=0;k<curConnRgnSize;k++)
				lpVisited[ptVisited[k].y*nWidth + ptVisited[k].x] = false;//��ԭ lpVisited ����

			curConnRgnSize = 0; //����Ϊ0

			pixel = image_bkp.GetGray(j, i);
			
			if( pixel == 0 ) //�ҵ�1�������أ�����̽���������������ͨ����Ĵ�С
			{
				int nRet = TestConnRgn(&image_bkp, lpVisited, nWidth, nHeight, j, i, ptVisited, lowerThres, upperThres, curConnRgnSize);
			
				if( (nRet == 1) || (nRet == -1) ) // >upperThres or <lowerThres
				{
					//�˳�
					for(k=0; k<curConnRgnSize; k++)
					{
						image_res.SetPixel(ptVisited[k].x, ptVisited[k].y, RGB(255, 255, 255));
					}
				}
			}

		}// for j
	}// for i

	
	*pTo = image_res;


	delete []lpVisited;
	delete []ptVisited;
}



/******************* 
void COCRImageProcess::ObjectAlign(COCRImageProcess* pTo, vector<RECT> &vecRT)
���ܣ�
	Ŀ������ַ����Ľ����Ͷ��룬��һ��֮��ʹ��
ע��
	ֻ�ܴ���2ֵͼ��

������
	COCRImageProcess* pTo: Ŀ��ͼ��� COCRImageProcess ָ��
	vecRT: ��ʾ�ַ��ָ�ľ�����������
	
����ֵ��
	��
********************/
void COCRImageProcess::ObjectAlign(COCRImageProcess* pTo, vector<RECT> &vecRT)
{
	pTo->InitPixels(255); //Ŀ��ͼ���ð�

	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();
	int i, j;

	int nSize = vecRT.size(); //ȡ�ö���(�ַ�)����������Ŀ

	int nNormW, nNormH; //��һ��֮���ͳһ���ο���
	//ȡ�ù�һ��֮���ͳһ���ο���
	if( nSize > 0 )
	{
		nNormW = vecRT[0].right - vecRT[0].left + 1;
		nNormH = vecRT[0].bottom - vecRT[0].top + 1;
	}
	else //û���������Σ�ֱ�ӷ���
		return;
	
	int nSrc_i, nSrc_j; //ӳ��Դ����
	RECT rt, rtNew;
	int nMargin = 2; //����������ͼ���������ϱ߽��

	//���ε���ÿ���������������λ��
	for(int nObj=0; nObj<nSize; nObj++)
	{
		rt = vecRT[nObj];

		//�����������������Σ���ͼ�����Ͻǿ�ʼ������������������
		rtNew.left = nObj * nNormW + nMargin; //��߽�
		rtNew.right = (nObj+1) * nNormW + nMargin; //�ұ߽�
		rtNew.top = 0 + nMargin; //�ϱ߽�
		rtNew.bottom = nNormH + nMargin; //�±߽�

		vecRT[nObj] = rtNew;

		//��ԭ���ο��ڵ�����ӳ�䵽�µľ��ο���
		for(i=0; i<nNormH; i++)
		{
			for(j=nObj*nNormW; j<(nObj+1)*nNormW; j++)
			{
				//����ӳ��Դ����
				nSrc_i = rt.top + i;
				nSrc_j = rt.left + j - nObj*nNormW;

				//��������
				int nGray = GetGray(nSrc_j, nSrc_i);
				pTo->SetPixel(j+nMargin, i+nMargin, RGB(nGray, nGray, nGray));
			}// for j
		}// for i
	}//for nObj

}

















