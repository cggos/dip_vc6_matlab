// ImgProcess.cpp: implementation of the CImgProcess class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ImgProcess.h"




#include <vector>
#include <queue>
#include <math.h>


#define _EdgeAll 0;
#define _EdgeH 1;
#define _EdgeV 2;
#define _EdgeCW 3;
#define _EdgeCCW 4;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//����ģ������

// ƽ��ƽ��  1/9
float Template_Smooth_Avg[9]={1, 1, 1,
1, 1, 1,
1, 1, 1};
// Gaussƽ��  1/16
float Template_Smooth_Gauss[9]={1, 2, 1,
2, 4, 2,
1, 2, 1};
// Sobel��ֱ��Ե���
float Template_HSobel[9]={-1, 0, 1, 
-2, 0, 2, 
-1 ,0 , 1};

// Sobelˮƽ��Ե���
float Template_VSobel[9]={-1, -2, -1,
0, 0, 0, 
1, 2, 1};

// LOG��Ե���
float Template_Log[25]={0, 0, -1, 0, 0,
0, -1, -2, -1, 0,
-1, -2, 16, -2, -1,
0, -1, -2, -1, 0,
0, 0, -1, 0, 0};
//Sigma = 2����ƽ��
double Template_Log1[25]={0.0448,    0.0468,    0.0564,    0.0468,    0.0448,
    0.0468,    0.3167,    0.7146,    0.3167,    0.0468,
    0.0564,    0.7146,   -4.9048,    0.7146,    0.0564,
    0.0468,    0.3167,    0.7146,    0.3167,    0.0468,
    0.0448,    0.0468,    0.0564,    0.0468,    0.0448
};

// Laplacian��Ե���
float Template_Laplacian1[9] = {0, -1, 0,
-1, 4, -1,
0, -1, 0
};
float Template_Laplacian2[9] = {-1, -1, -1,
-1, 8, -1,
-1, -1, -1
};







// ���ι��캯��
CImgProcess::CImgProcess()
{
	m_nBasePt = 4;
}

CImgProcess::~CImgProcess()
{
	
}

/**************************************************
void CImgProcess::AutoThreshold(CImgProcess *pTo)

���ܣ�
	ͼ����Զ���ֵ��

������
	CImgProcess * pTo
		���CImgProcess�����ָ��

����ֵ��
	��
***************************************************/

void CImgProcess::AutoThreshold(CImgProcess *pTo)
{
	int nDiffGray;

	int nThres = DetectThreshold(100, nDiffGray);//ȡ�÷ָ���ֵ������������Ϊ100
	
	Threshold(pTo, nThres); //��ֵ�ָ�
		

}









/******************* 
void CImgProcessProcessing::Erode(CImgProcess* pTo, int se[3][3])
 
 ���ܣ�
	3*3�ṹԪ�صĶ�ֵͼ��ʴ����
 ע��
	ֻ�ܴ���2ֵͼ��

 ������
	ImgProcess* pTo: Ŀ�����ͼ��� CImgProcess ָ��
	se[3][3]: 3*3�ĽṹԪ�أ�������Ԫ�صĺϷ�ȡֵΪ: 
		1 --- ǰ��
		0 --- ����
	   -1 --- ������	 
 ����ֵ��
	��	
*******************/
void CImgProcess::Erode(CImgProcess *pTo, int se[3][3])
{

	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	int i, j; //ͼ��ѭ������
	int k, l; //�ṹԪ��ѭ������

	BOOL bMatch; //�ṹԪ���Ƿ���ֲ�ͼ��ƥ��


	pTo->InitPixels(255); //���Ŀ�����ͼ��

	//����ɨ��ͼ��Ϊ��ֹ����Խ�磬��������һ�����ؿ�Ŀձ�
	for(i=1; i<nHeight-1; i++)
	{
		for(j=1; j<nWidth-1; j++)
		{
			//����ʹ�õ���3*3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���������Ϻ����µ����������Լ���������ҵ���������
			bMatch = true;
			for(k=0; k<3; k++)
			{
				for(l=0; l<3; l++)
				{
					if( se[k][l] == -1 ) //������
						continue;

					if( se[k][l] == 1 ) //ǰ��
					{
						if( GetGray(j-1+l, i-1+k) != 0 )
						{
							bMatch = false;
							break;
						}
					}
					else if( se[k][l] == 0 ) //����
					{
						if( GetGray(j-1+l, i-1+k) != 255 )
						{
							bMatch = false;
							break;
						}
					}
					else
					{
						AfxMessageBox("�ṹԪ�غ��зǷ�ֵ������������趨��");
						return;
					}
					
				}//for l
			}//for k
			
			if( bMatch )
				pTo->SetPixel(j, i, RGB(0, 0, 0));

		}// for j
	}// for i


}


/******************* 
void CImgProcess::Dilate(CImgProcess* pTo, int se[3][3])
 
 ���ܣ�
	3*3�ṹԪ�صĶ�ֵͼ����������
 ע��
	ֻ�ܴ���2ֵͼ��

 ������
	Image* pTo: Ŀ�����ͼ��� CImgProcess ָ��
	se[3][3]: 3*3�ĽṹԪ�أ�������Ԫ�صĺϷ�ȡֵΪ: 
		1 --- ǰ��
	   -1 --- ������
	 
 ����ֵ��
	��
	
*******************/
void CImgProcess::Dilate(CImgProcess *pTo, int se[3][3])
{

	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	int i, j; //ͼ��ѭ������
	int k, l; //�ṹԪ��ѭ������


	//����se�������ĵĶԳƼ�
	int nTmp;
	for(i=0; i<2; i++)
	{
		for(j=0; j<3-i; j++)
		{
			nTmp = se[i][j];
			se[i][j] = se[2-i][2-j];
			se[2-i][2-j] = nTmp;
		}
	}

	
	pTo->InitPixels(255); //���Ŀ�����ͼ��

	//����ɨ��ͼ��Ϊ��ֹ����Խ�磬��������һ�����ؿ�Ŀձ�
	for(i=1; i<nHeight-1; i++)
	{
		for(j=1; j<nWidth-1; j++)
		{
			//����ʹ�õ���3*3�ĽṹԪ�أ�Ϊ��ֹԽ�磬���������Ϻ����µ����������Լ���������ҵ���������

			for(k=0; k<3; k++)
			{
				for(l=0; l<3; l++)
				{
					if( se[k][l] == -1 ) // ������
						continue;

					if( se[k][l] == 1 )
					{
						if( GetGray(j-1+l, i-1+k) ==  0)
						{
							//ԭͼ�ж�Ӧ�ṹԪ�صľֲ�������һ��Ϊ1���ͽ�Ŀ��ͼ���Ӧ�ڽṹԪ�����ĵ�������0

							pTo->SetPixel(j, i, RGB(0, 0, 0));
							break;
						}
					}
					else
					{
						AfxMessageBox("�ṹԪ�غ��зǷ�ֵ������������趨��");
						return;
					}

				}//for l
			}//for k
			
		}// for j
	}// for i


}

/******************* 
void CImgProcessProcessing::Open(CImgProcess* pTo, int se[3][3])
 
 ���ܣ�
	3*3�ṹԪ�صĶ�ֵͼ������
 ע��
	ֻ�ܴ���2ֵͼ��

 ������
	Image* pTo: Ŀ�����ͼ��� CImgProcess ָ��
	se[3][3]: 3*3�ĽṹԪ�أ�������Ԫ�صĺϷ�ȡֵΪ: 
		1 --- ǰ��
	   -1 --- ������
	 
 ����ֵ��
	��
	
*******************/
void CImgProcess::Open(CImgProcess* pTo, int se[3][3])
{
	pTo->InitPixels(255);

	Erode(pTo, se);

	*this = *pTo;

	Dilate(pTo, se);
}


/******************* 
void CImgProcessProcessing::Close(CImgProcess* pTo, int se[3][3])
 
 ���ܣ�
	3*3�ṹԪ�صĶ�ֵͼ�������
 ע��
	ֻ�ܴ���2ֵͼ��

 ������
	Image* pTo: Ŀ�����ͼ��� CImgProcess ָ��
	se[3][3]: 3*3�ĽṹԪ�أ�������Ԫ�صĺϷ�ȡֵΪ: 
		1 --- ǰ��
	   -1 --- ������
	 
 ����ֵ��
	��
	
*******************/
void CImgProcess::Close(CImgProcess* pTo, int se[3][3])
{
	pTo->InitPixels(255);



	Dilate(pTo, se);
	
	*this = *pTo;

	Erode(pTo, se);
}




/******************* 
void CImgProcessProcessing::Convex(CImgProcess* pTo, BOOL bConstrain)
 
 ���ܣ�
	����ͼ����ǰ�������͹��
 ע��
	ֻ�ܴ���2ֵͼ��

 ������
	Image* pTo: Ŀ�����ͼ��� CImgProcess ָ��
	BOOL bConstrain: �Ƿ�����͹�ǵ������ڰ�������������С����֮��
		 
 ����ֵ��
	��
	
*******************/
void CImgProcess::Convex(CImgProcess* pTo, BOOL bConstrain)
{
	//����͹����Ҫ��4���ṹԪ��
	/*
	se1 = 1 -1 -1	se2 = 1  1  1	se3 = -1 -1  1	se4 = -1 -1 -1
		  1	 0 -1		 -1  0 -1		  -1  0  1		  -1  0 -1
		  1 -1  1		 -1 -1 -1		  -1 -1  1		   1  1  1
	*/
	int se1[3][3] = {{1, -1, -1}, {1, 0, -1}, {1, -1, -1}}; //�ֲ��Ҳ��͹ȱ
	int se2[3][3] = {{1, 1, 1}, {-1, 0, -1}, {-1, -1, -1}}; //�ֲ��²��͹ȱ
	int se3[3][3] = {{-1, -1, 1}, {-1, 0, 1}, {-1, -1, 1}}; //�ֲ�����͹ȱ
	int se4[3][3] = {{-1, -1, -1}, {-1, 0, -1}, {1, 1, 1}}; //�ֲ��ϲ��͹ȱ
	
	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();
	int i, j; //ͼ��ѭ������

	
	pTo->InitPixels(255); //���Ŀ�����ͼ��


	// ���õ�1���ṹԪ��
	CImgProcess tmpImg1 = *this; //�ݴ���һ�ε�������
	

	while(true)
	{
		tmpImg1.Erode(pTo, se1); //����ȫ������Χ�Ļ��л����б任
		*pTo = *pTo | tmpImg1;
				
		if(tmpImg1 == *pTo)
			break; //�㷨������ֹ
		
		tmpImg1 = *pTo;
	}
	

	// ���õ�2���ṹԪ��
	CImgProcess tmpImg2 = *this; //�ݴ���һ�ε�������
	
	while(true)
	{
		tmpImg2.Erode(pTo, se2); //����ȫ������Χ�Ļ��л����б任
		*pTo = *pTo | tmpImg2;

		if(tmpImg2 == *pTo)
			break; //�㷨������ֹ

		tmpImg2 = *pTo;
	}

	// ���õ�3���ṹԪ��
	CImgProcess tmpImg3 = *this; //�ݴ���һ�ε�������
	
	while(true)
	{
		tmpImg3.Erode(pTo, se3); //����ȫ������Χ�Ļ��л����б任
		*pTo = *pTo | tmpImg3;
		
		if(tmpImg3 == *pTo)
			break; //�㷨������ֹ
		
		tmpImg3 = *pTo;
	}


	// ���õ�4���ṹԪ��
	CImgProcess tmpImg4 = *this; //�ݴ���һ�ε�������
	
	while(true)
	{
		tmpImg4.Erode(pTo, se4); //����ȫ������Χ�Ļ��л����б任
		*pTo = *pTo | tmpImg4;
		
		if(tmpImg4 == *pTo)
			break; //�㷨������ֹ
		
		tmpImg4 = *pTo;
	}



	// ����4���������Ĳ���
	pTo->InitPixels(255);

	for(i=0; i<nHeight; i++)
	{
		for(j=0; j<nWidth; j++)
		{
			if( (tmpImg1.GetGray(j, i) == 0) || (tmpImg2.GetGray(j, i) == 0) || (tmpImg3.GetGray(j, i) == 0) || (tmpImg4.GetGray(j, i) == 0) )
				pTo->SetPixel(j, i, RGB(0, 0, 0));			
		}
	}
	

	
	// ��Ҫ����͹�ǵ�����
	
	// �ҵ�ԭͼ��������ķ�Χ�������������С���Σ�
	int nTop = nHeight;
	int nBottom = 0;
	int nLeft = nWidth;
	int nRight = 0;
	for(i=0; i<nHeight; i++)
	{
		for(j=0; j<nWidth; j++)
		{
			if(GetGray(j, i) == 0)
			{
				if(i < nTop)
					nTop = i;
				if(i > nBottom)
					nBottom = i;
				if(j < nLeft)
					nLeft = j;
				if(j > nRight)
					nRight = j;
			}
		}
	}

	if(bConstrain)
	{
		for(i=0; i<nHeight; i++)
		{
			for(j=0; j<nWidth; j++)
			{
				if( (i<nTop) || (i>nBottom) || (j<nLeft) || (j>nRight) )
					pTo->SetPixel(j, i, RGB(255, 255, 255));
			}
		}
	}//if(bConstrain)

}

/*******************
void CImgProcess::GrayDilate(CImgProcess* pTo, int nTempH, int nTempW, int nTempMY, int nTempMX, int** se)
���ܣ��Ҷ�ͼ������

ע��ֻ����ƽ̹�ĽṹԪ��

������
	CImgProcess* pTo��Ŀ��ͼ��� CImgProcess ָ��
	int   nTempH��ģ��ĸ߶�
	int   nTempW�� ģ��Ŀ��
	int   nTempMY��ģ�������Ԫ��Y���� ( <= iTempH - 1)
	int   nTempMX��ģ�������Ԫ��X���� ( <= iTempW - 1)
	int** se���ṹԪ��
����ֵ:
	��
*******************/
void CImgProcess::GrayDilate(CImgProcess* pTo, int nTempH, int nTempW, int nTempMY, int nTempMX, int** se)
{
	// ѭ������
	int i, j, k, l;
		
	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();
	

	for(i=nTempMY;i<nHeight - nTempH + nTempMY + 1;i++)
	{
		for(j=nTempMX;j<nWidth - nTempW + nTempMX +1;j++)
		{
			BYTE maxVal = 0; //�ֲ����ֵ
			for(k=0;k<nTempH;k++)
			{
				for(l=0;l<nTempW;l++)
				{
					if( se[k][l] == 1 )
					{
						// ͼ���i - nTempMY + k�У���j - nTempMX + l�����صĻҶ�
						BYTE gray = GetGray(j-nTempMX+l, i-nTempMY+k);
						
						//��ֲ����ֵ
						if( gray > maxVal )
							maxVal = gray;
					}
				}//l
			}//k
		
			pTo->SetPixel(j, i, RGB(maxVal, maxVal, maxVal));			
		}// for j
	}//for i

}



/*******************
void CImgProcess::GrayErode(CImgProcess* pTo, int nTempH, int nTempW, int nTempMY, int nTempMX, int** se)
���ܣ��Ҷ�ͼ��ʴ

ע��ֻ����ƽ̹�ĽṹԪ��

������
	CImgProcess* pTo��Ŀ��ͼ��� CImgProcess ָ��
	int   nTempH��ģ��ĸ߶�
	int   nTempW��ģ��Ŀ��
	int   nTempMY��ģ�������Ԫ��Y���� ( <= iTempH - 1)
	int   nTempMX��ģ�������Ԫ��X���� ( <= iTempW - 1)
	int **se���ṹԪ��
����ֵ:
	��
*******************/
void CImgProcess::GrayErode(CImgProcess* pTo, int nTempH, int nTempW, int nTempMY, int nTempMX, int** se)
{
	// ѭ������
	int i, j, k, l;
		
	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();
	

	for(i=nTempMY;i<nHeight - nTempH + nTempMY + 1;i++)
	{
		for(j=nTempMX;j<nWidth - nTempW + nTempMX +1;j++)
		{
			BYTE minVal = 255; //�ֲ���Сֵ
			for(k=0;k<nTempH;k++)
			{
				for(l=0;l<nTempW;l++)
				{
					if( se[k][l] == 1 )
					{
						// ͼ���i - nTempMY + k�У���j - nTempMX + l�����صĻҶ�
						BYTE gray = GetGray(j-nTempMX+l, i-nTempMY+k);
						
						//��ֲ����ֵ
						if( gray < minVal )
							minVal = gray;
					}
				}//l
			}//k
		
			pTo->SetPixel(j, i, RGB(minVal, minVal, minVal));			
		}// for j
	}//for i

}


/*******************
void CImgProcess::GrayOpen(CImgProcess* pTo, int nTempH, int nTempW, int nTempMY, int nTempMX, int** se)
���ܣ��Ҷȿ�����

ע��ֻ����ƽ̹�ĽṹԪ��

������
	CImgProcess* pTo��Ŀ��ͼ��� CImgProcess ָ��
	int   nTempH��ģ��ĸ߶�
	int   nTempW��ģ��Ŀ��
	int   nTempMY��ģ�������Ԫ��Y���� ( <= iTempH - 1)
	int   nTempMX��ģ�������Ԫ��X���� ( <= iTempW - 1)
	int **se���ṹԪ��
����ֵ:
	��
*******************/
void CImgProcess::GrayOpen(CImgProcess* pTo, int nTempH, int nTempW, int nTempMY, int nTempMX, int** se)
{
	pTo->InitPixels(255);

	GrayErode(pTo, nTempH, nTempW, nTempMY, nTempMX, se);

	CImgProcess tmpImg = *pTo; //�ݴ港ʴͼ��

	tmpImg.GrayDilate(pTo, nTempH, nTempW, nTempMY, nTempMX, se);	
}

/*******************
void CImgProcess::GrayClose(CImgProcess* pTo, int nTempH, int nTempW, int nTempMY, int nTempMX, int** se)
���ܣ��Ҷȿ�����

ע��ֻ����ƽ̹�ĽṹԪ��

������
	CImgProcess* pTo��Ŀ��ͼ��� CImgProcess ָ��
	int   nTempH��ģ��ĸ߶�
	int   nTempW��ģ��Ŀ��
	int   nTempMY��ģ�������Ԫ��Y���� ( <= iTempH - 1)
	int   nTempMX��ģ�������Ԫ��X���� ( <= iTempW - 1)
	int **se���ṹԪ��
����ֵ:
	��
*******************/
void CImgProcess::GrayClose(CImgProcess* pTo, int nTempH, int nTempW, int nTempMY, int nTempMX, int** se)
{
	pTo->InitPixels(255);

	GrayDilate(pTo, nTempH, nTempW, nTempMY, nTempMX, se);

	CImgProcess tmpImg = *pTo; //�ݴ�����ͼ��

	tmpImg.GrayErode(pTo, nTempH, nTempW, nTempMY, nTempMX, se);	
}






/*******************
void CImgProcess::Tophat(CImgProcess* pTo, int nTempH, int nTempW, int nTempMY, int nTempMX, int** se)
���ܣ���ñ�任

ע��ֻ����ƽ̹�ĽṹԪ��

������
	CImgProcess* pTo��Ŀ��ͼ��� CImgProcess ָ��
	int   nTempH��ģ��ĸ߶�
	int   nTempW��ģ��Ŀ��
	int   nTempMY��ģ�������Ԫ��Y���� ( <= iTempH - 1)
	int   nTempMX��ģ�������Ԫ��X���� ( <= iTempW - 1)
	int **se���ṹԪ��
����ֵ:
	��
*******************/
void CImgProcess::Tophat(CImgProcess* pTo, int nTempH, int nTempW, int nTempMY, int nTempMX, int** se)
{
	GrayOpen(pTo, nTempH, nTempW, nTempMY, nTempMX, se); //�Ҷȿ�����

		
	*pTo = (*this) - (*pTo); //��ñ�任��ԭͼ���ȥ������ͼ��
		
}


/*******************
void CImgProcess::Template(CImgProcess *pTo, 
						 int nTempH, int nTempW, 
						 int nTempMY, int nTempMX, FLOAT *pfArray, FLOAT fCoef)

���ܣ�ģ�����

ע���ú�����ָ����ģ�壨�����С������ͼ����в���������iTempHָ��ģ��
	�ĸ߶ȣ�����iTempWָ��ģ��Ŀ�ȣ�����iTempMX��iTempMYָ��ģ�������
	Ԫ�����꣬����fpArrayָ��ģ��Ԫ�أ�fCoefָ��ϵ����

������
	CImgProcess* pTo�����ͼ��� CImgProcess ָ��
	int   nTempH��ģ��ĸ߶�
	int   nTempW�� ģ��Ŀ��
	int   nTempMY��ģ�������Ԫ��Y���� ( <= iTempH - 1)
	int   nTempMX��ģ�������Ԫ��X���� ( <= iTempW - 1)
	FLOAT * fpArray��ָ��ģ�������ָ��
	FLOAT fCoef��ģ��ϵ��

����ֵ:
	��
*******************/
void CImgProcess::Template(CImgProcess *pTo, 
						 int nTempH, int nTempW, 
						 int nTempMY, int nTempMX, FLOAT *pfArray, FLOAT fCoef)
{
	pTo->InitPixels(0); //Ŀ��ͼ���ʼ��
	
	int i, j; //ѭ������

	//ɨ��ͼ�����ģ�����
	for(i=nTempMY; i<GetHeight() - (nTempH - nTempMY) + 1; i++)
	{
		for(j=nTempMX; j<GetWidthPixel() - (nTempW - nTempMX) + 1; j++)
		{
			// (j,i)Ϊ���ĵ�
			float fResult = 0;
			for(int k=0; k<nTempH; k++)
			{
				for(int l=0; l<nTempW; l++)
				{
					//�����Ȩ��
					fResult += GetGray(j + l - nTempMX, i + k - nTempMY) * pfArray[k * nTempW + l];
				}
			}
			
			// ����ϵ��
			fResult *= fCoef;
			
			// ȡ��
			fResult = (FLOAT)fabs(fResult); //��ʱ�п��ܳ��ָ�ֵ
			
			BYTE byte;
			if(fResult > 255)
				byte = 255;
			else
				byte = fResult + 0.5; //��������
			
			pTo->SetPixel(j, i, RGB(byte, byte, byte));
		}//for j
	}//for i
}


/*******************
 int CImgProcess::GetMedianValue(int * pAryGray, int nFilterLen)
 
 ���ܣ�����ð�ݷ�������������򣬲���������Ԫ�ص���ֵ��

 ����:
	int * pAryGray��Ҫ������ȡ��ֵ������
    int nFilterLen�����鳤��
 
 ����ֵ:
    int ��ֵ
 
*******************/

int CImgProcess::GetMedianValue(int * pAryGray, int nFilterLen)
{
	int i, j;	
	int nMedianValue;
	int nTemp; //�м����
	
	//����
	for (j=0; j < nFilterLen - 1; j++)
	{
		for (i=0; i < nFilterLen - j - 1; i++)
		{
			if (pAryGray[i] > pAryGray[i + 1])
			{
				// ����λ��
				nTemp = pAryGray[i];
				pAryGray[i] = pAryGray[i + 1];
				pAryGray[i + 1] = nTemp;
			}//if
		}//for i
	}//for j
	
	// ������ֵ
	if ((nFilterLen & 1) > 0)
	{
		// ������������Ԫ�أ������м�һ��Ԫ��
		nMedianValue = pAryGray[(nFilterLen + 1) / 2];
	}
	else
	{
		// ������ż����Ԫ�أ������м�����Ԫ��ƽ��ֵ
		nMedianValue = (pAryGray[nFilterLen / 2] + pAryGray[nFilterLen / 2 + 1]) / 2;
	}
	
	// ������ֵ
	return nMedianValue;
}




/*******************
void CImgProcess::MedianFilter(CImgProcess *pTo, int nFilterH, int nFilterW, int nFilterMY, int nFilterMX)

���ܣ���ֵ�˲�

ע����ͻ�����������罷�������нϺõ�����Ч��

����:
	CImgProcess* pTo��Ŀ��ͼ��� CImgProcess ָ��
    int   nFilterH���˲����ĸ߶�
    int   nFilterW���˲����Ŀ��
    int   nFilterMX���˲���������Ԫ��Y����
    int   nFilterMY���˲���������Ԫ��X����
 
����ֵ:
    �� 
*******************/
void CImgProcess::MedianFilter(CImgProcess *pTo, int nFilterH, int nFilterW, int nFilterMY, int nFilterMX)
{
	pTo->InitPixels(0); //��ʼ��Ŀ��ͼ��
	
	int i, j, k, l;

	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	int nGray;

	int* pAryGray; //������������
	pAryGray = new int[nFilterH * nFilterW];

	// ����ɨ��ͼ�񣬽�����ֵ�˲�	
	for(i = nFilterMY; i < nHeight - nFilterH + nFilterMY + 1; i++)// ��(��ȥ��Ե����)
	{
		for(j = nFilterMX; j < nWidth - nFilterW + nFilterMX + 1; j++)// ��(��ȥ��Ե����)
		{
			// ��ȡ�˲�������
			for (k = 0; k < nFilterH; k++)
			{
				for (l = 0; l < nFilterW; l++)
				{
					//ԭͼ���i + k - nFilterMY�У���j + l - nFilterMX�е�����ֵ
					nGray = GetGray(j + l - nFilterMX, i + k -nFilterMY);					
					
					// ��������ֵ
					pAryGray[k * nFilterW + l] = nGray;
				}//l
			}//k
			
			nGray = GetMedianValue(pAryGray, nFilterH * nFilterW); //ͨ�������ȡ��ֵ
	
			pTo->SetPixel(j, i, RGB(nGray, nGray, nGray)); //����ֵ��Ϊ��Ӧ			
		}//j
	}//i

	delete [] pAryGray;
}



/*******************
void CImgProcess::AdaptiveMedianFilter(CImgProcess *pTo, int nFilterH, int nFilterW, int nFilterMY, int nFilterMX)

���ܣ��Ľ�����ֵ�˲�

ע���ڱ�׼��ֵ�˲��Ļ����ϣ�������ÿһ������ʱ���жϸ������Ƿ����˲��������������������صļ�����߼�Сֵ��
	����ǣ��������������ֵ�˲���������أ�������ǣ����账��

����:
	CImgProcess* pTo��Ŀ��ͼ��� CImgProcess ָ��
    int   nFilterH���˲����ĸ߶�
    int   nFilterW���˲����Ŀ��
    int   nFilterMX���˲���������Ԫ��Y����
    int   nFilterMY���˲���������Ԫ��X����
 
����ֵ:
    �� 
*******************/
void CImgProcess::AdaptiveMedianFilter(CImgProcess *pTo, int nFilterH, int nFilterW, int nFilterMY, int nFilterMX)
{
	pTo->InitPixels(0); //��ʼ��Ŀ��ͼ��
	
	int i, j, k, l;

	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	int nGray;

	int* pAryGray; //������������
	pAryGray = new int[nFilterH * nFilterW];

	// ����ɨ��ͼ�񣬽�����ֵ�˲�	
	for(i = nFilterMY; i < nHeight - nFilterH + nFilterMY + 1; i++)// ��(��ȥ��Ե����)
	{
		for(j = nFilterMX; j < nWidth - nFilterW + nFilterMX + 1; j++)// ��(��ȥ��Ե����)
		{
			// ��ȡ�˲�������
			for (k = 0; k < nFilterH; k++)
			{
				for (l = 0; l < nFilterW; l++)
				{
					//ԭͼ���i + k - nFilterMY�У���j + l - nFilterMX�е�����ֵ
					nGray = GetGray(j + l - nFilterMX, i + k -nFilterMY);					
					
					// ��������ֵ
					pAryGray[k * nFilterW + l] = nGray;
				}//l
			}//k
			
			nGray = GetMedianValue(pAryGray, nFilterH * nFilterW); //ͨ�������ȡ��ֵ
	
			//�жϵ�ǰ�����Ƿ�������ļ����Сֵ
			if( (GetGray(j, i) == pAryGray[0]) || (GetGray(j, i) == pAryGray[nFilterH * nFilterW - 1]) )
				pTo->SetPixel(j, i, RGB(nGray, nGray, nGray)); //����ֵ��Ϊ��Ӧ
			else
				pTo->SetPixel(j, i, GetGray(j, i)); //���Ǽ�ֵ�򲻸ı�ԭͼ���ֵ
		}//j
	}//i

	delete [] pAryGray;
}

/******************* 
void CImgProcess::FilterSobel(CImgProcess *pTo)
 ���ܣ�
	Sobel�ݶ�
 
 ������
	ImgProcess* pTo: Ŀ�����ͼ��� CImgProcess ָ��
		 
 ����ֵ��
	��	
*******************/
void CImgProcess::FilterSobel(CImgProcess *pTo)
{
	CImgProcess img1, img2;
	img1 = *pTo;
	img2 = *pTo;

	Template(&img1, 3, 3, 1, 1, Template_HSobel, 1);
	Template(&img2, 3, 3, 1, 1, Template_VSobel, 1);

	*pTo = img1 + img2;
}


/*******************
void CImgProcess::EnhanceFilter(CImgProcess *pTo, double dProportion,
						 int nTempH, int nTempW, 
						 int nTempMY, int nTempMX, FLOAT *pfArray, FLOAT fCoef)

���ܣ��������˲�

������
	CImgProcess* pTo�����ͼ��� CImgProcess ָ��
	double dProportion���������˲���ԭͼ��Ļ�ϱ���
	int   nTempH��ģ��ĸ߶�
	int   nTempW�� ģ��Ŀ��
	int   nTempMY��ģ�������Ԫ��Y���� ( < iTempH - 1)
	int   nTempMX��ģ�������Ԫ��X���� ( < iTempW - 1)
	FLOAT * fpArray��ָ��ģ�������ָ��
	FLOAT fCoef��ģ��ϵ��

����ֵ:
	��
*******************/
void CImgProcess::EnhanceFilter(CImgProcess *pTo, double dProportion,
						 int nTempH, int nTempW, 
						 int nTempMY, int nTempMX, FLOAT *pfArray, FLOAT fCoef)
{
	int i, j;
	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	*pTo = *this; //Ŀ��ͼ���ʼ��

	//GrayMat�ݴ水��������ͼ�񣨲�����CImg�������ֱ�ӽ���������ӣ���Ϊ��ӵĽ�����ܳ�����Χ[0,255]��
	vector< vector<int> > GrayMat;
	vector<int> vecRow(nWidth, 0); //GrayMat�е�һ�У���ʼ��Ϊ0��
	for(i=0; i<nHeight; i++)
	{
		GrayMat.push_back(vecRow);
	}

	//��ͼ�������������Ӧ������ԭͼ�񰴱�������			
	for(i=nTempMY; i<GetHeight() - (nTempH - nTempMY) + 1; i++)
	{
		for(j=nTempMX; j<GetWidthPixel() - (nTempW - nTempMX) + 1; j++)
		{
			// (j,i)Ϊ���ĵ�
			float fResult = 0;
			for(int k=0; k<nTempH; k++)
			{
				for(int l=0; l<nTempW; l++)
				{
					//�����Ȩ��
					fResult += GetGray(j + l - nTempMX, i + k - nTempMY) * pfArray[k * nTempW + l];
				}
			}
			
			// ����ϵ��
			fResult *= fCoef;

			//������Ӧֵ��Χ
			if(fResult > 255)
				fResult = 255;
			if(fResult < -255)
				fResult = -255;
			
			GrayMat[i][j] = dProportion * GetGray(j, i) + fResult + 0.5;//��ͣ������������
		}//for j
	}//for i
	
	
	
	int nMax = 0;//���ҶȺ�ֵ
	int nMin = 65535; //��С�ҶȺ�ֵ

	//ͳ�������Сֵ
	for(i=nTempMY; i<GetHeight() - (nTempH - nTempMY) + 1; i++)
	{
		for(j=nTempMX; j<GetWidthPixel() - (nTempW - nTempMX) + 1; j++)
		{
			if( GrayMat[i][j] > nMax)
				nMax = GrayMat[i][j];
			if( GrayMat[i][j] < nMin)
				nMin = GrayMat[i][j];
		}// j
	}// i

	//��GrayMat��ȡֵ��Χ���¹�һ����[0, 255]
	int nSpan = nMax - nMin;

	for(i=nTempMY; i<GetHeight() - (nTempH - nTempMY) + 1; i++)
	{
		for(j=nTempMX; j<GetWidthPixel() - (nTempW - nTempMX) + 1; j++)
		{
			BYTE bt;
			if(nSpan > 0)
				bt = (GrayMat[i][j] - nMin)*255/nSpan;
			else if(GrayMat[i][j] <= 255)
				bt = GrayMat[i][j] ;
			else
				bt = 255;
				
				pTo->SetPixel(j, i, RGB(bt, bt, bt));			

		}// for j
	}// for i
}






/**************************************************
int CImgProcess::DetectThreshold(int nMaxIter, int &nDiffRet)

���ܣ�
	���õ������Զ�ȷ����ֵ

������
	int nMaxIter������������
	int &nDiffRet��	ʹ�ø�����ֵȷ���ĵ������밵��ƽ���ҶȵĲ���ֵ
����ֵ��
	int���ͣ��㷨��ȷ������ֵ
***************************************************/

int CImgProcess::DetectThreshold(int nMaxIter, int &nDiffRet)
{
	int nThreshold;
	
	nDiffRet = 0;
	
	// ֱ��ͼ����
	int nHistogram[256] = { 0 };
	int i, j;
	
	BYTE bt;

	int nMin = 255;
	int nMax = 0;

	// ɨ��ͼ��,����������С�ҶȺ�ֱ��ͼ
	for(j = 0; j < GetHeight(); j ++)
	{
		for(i=0; i<GetWidthPixel(); i++)
		{
			bt = GetGray(i, j);
			
			if(bt < nMin)
				nMin = bt;
			if(bt > nMax)
				nMax = bt;
			
			nHistogram[bt] ++;
			
		}
	}
	
	int nTotalGray = 0; //�Ҷ�ֵ�ĺ�
	int nTotalPixel = 0; //�������ĺ�
	int nNewThreshold = (nMax + nMin)/2; //��ʼ��ֵ
	
	nDiffRet = nMax - nMin;

	if (nMax == nMin)
		nThreshold = nNewThreshold;

	else
	{
		nThreshold = 0;
		
		// ������ʼ,ֱ�����������ﵽ100������ֵ����һ�ֵõ�����ֵ��ȣ���������
		for(int nIterationTimes = 0; nThreshold != nNewThreshold && nIterationTimes < nMaxIter; nIterationTimes ++)
		{
			nThreshold = nNewThreshold;
			nTotalGray = 0;
			nTotalPixel = 0;

			//����ͼ����С�ڵ�ǰ��ֵ���ֵ�ƽ���Ҷ�
			for(i=nMin; i<nThreshold; i++)
			{
				nTotalGray += nHistogram[i]*i;
				nTotalPixel += nHistogram[i];
			}
			int nMean1GrayValue = nTotalGray/nTotalPixel;


			nTotalGray = 0;
			nTotalPixel = 0;
			
			//����ͼ���д��ڵ�ǰ��ֵ���ֵ�ƽ���Ҷ�
			for(i=nThreshold + 1; i<=nMax; i++)
			{
				nTotalGray += nHistogram[i]*i;
				nTotalPixel += nHistogram[i];
			}
			int nMean2GrayValue = nTotalGray/nTotalPixel;
			
			nNewThreshold = (nMean1GrayValue + nMean2GrayValue)/2; //������µ���ֵ
			nDiffRet = abs(nMean1GrayValue - nMean2GrayValue);
		}
	}
	
	return nThreshold;
}

/**************************************************
void CImgProcess::Threshold(CImgProcess *pTo, BYTE bThre)

���ܣ�
	ͼ�����ֵ�任
���ƣ�
	ֻ�ܴ���Ҷ�ͼ��

������
	CImgProcess * pTo
		���CImgProcess�����ָ��
	BYTE bThre
		���õĻ�׼��ֵ
����ֵ��
	��
***************************************************/

void CImgProcess::Threshold(CImgProcess *pTo, BYTE bThre)
{
	int i, j;
	BYTE bt;
	for(j = 0; j < m_pBMIH->biHeight; j ++)
	{
		for(i=0; i<m_pBMIH->biWidth; i++)
		{
			bt = GetGray(i, j);
			if(bt<bThre)
				bt = 0;
			else
				bt = 255;
			
			pTo->SetPixel(i, j, RGB(bt, bt, bt));
		}
	}
}







struct SMaxValue
{
	int nValue;
	int nDist;
	int nAngle;
};






/******************* 
void CImgProcessProcessing::TraceBoundary(CImgProcess* pTo)
 
 ���ܣ�
	���ٶ�ֵͼ���е�һ���ҵ�������ı߽�
 ע��
	ֻ�ܴ���2ֵͼ��

 ������
	CImgProcess* pTo: Ŀ�����ͼ��� CImgProcess ָ��
	 
 ����ֵ��
	��
	
*******************/
void CImgProcess::TraceBoundary(CImgProcess *pTo)
{
	pTo->InitPixels(255);//���Ŀ��ͼ��

	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();
	
	
	//ѭ������
	int i;
	int j;

		
	//��ʼ�߽���뵱ǰ�߽��
	POINT ptStart;
	POINT ptCur;

	//�����������飬{���£��£����£��ң����ϣ��ϣ����ϣ���}
	int Direction[8][2]={ {-1,1}, {0,1}, {1,1}, {1,0}, {1,-1}, {0,-1}, {-1,-1}, {-1,0} }; 
	

	int nCurDirect = 0; //��ǰ̽�鷽��

	int xPos;
	int yPos;

	BOOL bAtStartPt; //Ϊtrue��ʾ̽��ո�ʱ��ʼ��Ϊ������ptCur == ptStart�����������һ�ֿ�ʼ��һ�ֽ�����

	//�㷨������߽��ϵĵ㣬��ͼ������ܵ�һȦ�߽��ð�
	for(i=0; i<nHeight; i++)
	{
		SetPixel(0, i, RGB(255, 255, 255));
		SetPixel(nWidth-1, i, RGB(255, 255, 255));
	}
	for(j=0; j<nWidth; j++)
	{
		SetPixel(j, 0, RGB(255, 255, 255));
		SetPixel(j, nHeight-1, RGB(255, 255, 255));
	}

	//����ɨ��
	for(i=0; i<nHeight; i++)
	{
		for(j=0; j<nWidth; j++)
		{
			if(GetGray(j, i) == 0)//�ҵ������ϵĵ�һ���߽��
			{
				ptStart.x = j;
				ptStart.y = i;

				ptCur = ptStart;
				bAtStartPt = true;				
				while(	( (ptCur.x != ptStart.x) || (ptCur.y != ptStart.y) )	|| bAtStartPt	)
				{
					bAtStartPt = false;

					//��һ��̽��λ��
					xPos = ptCur.x + Direction[nCurDirect][0];
					yPos = ptCur.y + Direction[nCurDirect][1];
					int nSearchTimes = 1;

					while( GetGray(xPos, yPos) == 255 )
					{
						nCurDirect ++; //��ʱ����ת45��
						if(nCurDirect >= 8)
							nCurDirect -= 8;

						xPos = ptCur.x + Direction[nCurDirect][0];
						yPos = ptCur.y + Direction[nCurDirect][1];

						if( ++nSearchTimes >= 8 ) //8�����ж�û�б߽�㣬˵���ǹ�����
						{
							xPos = ptCur.x;
							yPos = ptCur.y;
							break;
						}
					}
									
					//�ҵ���һ���߽��
					ptCur.x = xPos;
					ptCur.y = yPos;
					pTo->SetPixel(ptCur.x, ptCur.y, RGB(0, 0, 0)); //����ͼ���ϱ�Ǳ߽�
					/************
					�ڴ˴�����ʵ��Ĵ��룬�����Ҫ���μ�¼�±߽��ptCur
					************/

					nCurDirect -= 2; //����ǰ̽�鷽��˳ʱ���ת90����Ϊ��һ�ε�̽���ʼ����
					if(nCurDirect < 0)
						nCurDirect += 8;
				}
				return;
			}// if
			
			/************
			�ڴ˴�����ʵ��Ĵ��룬��ȥ�������return��䣬�����Ҫ����ͼ������������ı߽硣
			************/

		}// for j
	}// for i
	
}



/******************* 
void CImgProcessProcessing::FillRgn(CImgProcess* pTo, POINT ptStart)
 
 ���ܣ�
	�� ptStart ��Ϊ��ʼ�㣨���ӣ�����ͼ��������
 ע��
	ֻ�ܴ���2ֵͼ�󣬱߽�ٶ�Ϊ 8 ��ͨ��ptStart������ԭͼ�� *this �ı߽�֮��

 ������
	CImgProcess* pTo: Ŀ�����ͼ��� CImgProcess ָ��
	POINT ptStart: ���ӵ�����
	 
 ����ֵ��
	��
	
*******************/

void CImgProcess::FillRgn(CImgProcess *pTo, POINT ptStart)
{
	int se[3][3] = {{-1, 1, -1}, {1, 1, 1}, {-1, 1, -1}}; // ʮ���νṹԪ��

	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	
	pTo->InitPixels(255); //���Ŀ�����ͼ��

	CImgProcess revImg = (*this);
	revImg = !revImg; //ԭͼ��Ĳ���������������

	pTo->SetPixel(ptStart.x, ptStart.y, RGB(0, 0, 0)); //��ʼ��Ŀ��ͼ��Ϊֻ�����ӵ�
	CImgProcess tmpImg = *pTo; //�ݴ���һ�ε�������
	
	
	while( true ) //ѭ��ֱ��ͼ����������Ͳ��ٲ����任
	{

		tmpImg.Dilate(pTo, se); //��ʮ�ֽṹԪ������

		*pTo = *pTo & revImg;//�������Ͳ��ᳬ��ԭʼ�߽�

		if( *pTo == tmpImg )//���ٱ仯ʱֹͣ
			break;
		tmpImg = *pTo;
	}

	//���յĽ��Ϊ�������ͽ����ԭʼ�߽�Ĳ���
	*pTo = *pTo | (*this);
}


/*******************
void CImgProcess::LabelConnRgn(CImgProcess* pTo)

���ܣ���ע��ͨ����

ע��ֻ�ܴ����ֵͼ��

������
	CImgProcess* pTo��Ŀ��ͼ��� CImgProcess ָ��
	int nConn��ȡֵΪ4��8����ʾ4��ͨ��8��ͨ��Ĭ��Ϊ8

����ֵ:
	int ��
*******************/
void CImgProcess::LabelConnRgn(CImgProcess* pTo, int nConn)
{
	int se[3][3] = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}}; // 8��ͨ - 3*3�ṹԪ��
	if(nConn == 4)//4��ͨ - ʮ���νṹԪ��
	{
		se[0][0] = -1;
		se[0][2] = -1;
		se[2][0] = -1;
		se[2][2] = -1;
	}

	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	int i, j, k, l; //ѭ������

	CImgProcess backupImg = *this; //����ԭͼ���Ա��ָ�
	CImgProcess tmpImg = *pTo; //�ݴ���һ�ε�����������ʱͼ��

	int nConnRgn = 1; //��ͨ�����ı��
	int nGray;

	//�㷨������߽��ϵĵ㣬��ͼ������ܵ�һȦ�߽��ð�
	for(i=0; i<nHeight; i++)
	{
		SetPixel(0, i, RGB(255, 255, 255));
		SetPixel(nWidth-1, i, RGB(255, 255, 255));
	}
	for(j=0; j<nWidth; j++)
	{
		SetPixel(j, 0, RGB(255, 255, 255));
		SetPixel(j, nHeight-1, RGB(255, 255, 255));
	}

	for(i=0; i<nHeight; i++)
	{
		for(j=0; j<nWidth; j++)
		{
			nGray = GetGray(j, i);

			if(nGray == 0)
			{
				pTo->InitPixels(255); //���Ŀ�����ͼ��

				//�ҵ�һ��ǰ���㣬��ȡ�����ڵ���ͨ����
				pTo->SetPixel(j, i, RGB(0, 0, 0)); //��ʼ��Ŀ��ͼ��Ϊֻ����ͨ���е�һ��
				tmpImg = *pTo; //�ݴ�ͼ��
				
				while( true ) //ѭ��ֱ��ͼ����������Ͳ��ٲ����任
				{

					tmpImg.Dilate(pTo, se); //�ýṹԪ������

					*pTo = *pTo & backupImg;//�����ԭͼ��Ľ����������Ͳ��ᳬ������
					
					if( *pTo == tmpImg ) //�������һ�δ�����ͼ����ͬ��˵������ͨ���Ѿ���ȡ���
						break;
					tmpImg = *pTo; //�ݴ�ͼ��
				}

				//��ע�ո��ҵ�����ͨ��
				for(k=0; k<nHeight; k++)
				{
					for(l=0; l<nWidth; l++)
					{
						nGray = pTo->GetGray(l, k);
						if(nGray == 0)
						{
							SetPixel(l, k, RGB(nConnRgn, nConnRgn, nConnRgn)); //�ڵ�ǰͼ���ϱ�ע��nConnRgn����ͨ��
						}
					}//for l
				
				}//for k

				nConnRgn ++; //��ͨ����ż�1
				if(nConnRgn > 255)
				{
					AfxMessageBox("Ŀǰ�ú������֧�ֱ�ע255����ͨ����");
					i = nHeight; //ǿ���������ѭ��
					break;
				}
			}//if
		}//for j
	}//for i


	*pTo = *this; //����Ŀ��ͼ�����Ǳ�ע���� *this��
	*this = backupImg; //�ָ�ԭͼ��

}










/******************* 
void CImgProcess::Thining(CImgProcess* pTo)
 
 ���ܣ�
	ϸ��
 ע��
	ֻ�ܴ���2ֵͼ��

 ������
	��
	
 ����ֵ��
	��	
*******************/

void CImgProcess::Thining()
{
	
	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	//�ĸ�����
	BOOL bCondition1;
	BOOL bCondition2;
	BOOL bCondition3;
	BOOL bCondition4;

	//5��5������������ֵ
	unsigned char neighbour[5][5];

	int i, j;
	int m, n;

	BOOL bModified = TRUE;


	while(bModified)
	{		
		bModified = FALSE;
		CImgProcess pic = *this;
		pic.InitPixels(255); //���Ŀ��ͼ��
		
		for(j=2; j<nHeight-2; j++)
		{
			for(i=2; i<nWidth-2; i++)
			{				
				bCondition1 = FALSE;
				bCondition2 = FALSE;
				bCondition3 = FALSE;
				bCondition4 = FALSE;
				
				BYTE data = GetGray(i, j);
				if(data == 255)
					continue;
				
				
				// ��õ�ǰ�����ڵ�5��5����������ֵ����ɫ��0������ɫ��1����
				for (m = 0;m < 5;m++ )
				{
					for (n = 0;n < 5;n++)
					{
						neighbour[m][n] = (GetGray(i + n - 2, j + m - 2) == 0);
					}
				}
				//				neighbour[][]
				//����ж�������
				//�ж�2<=NZ(P1)<=6
				int nCount =  neighbour[1][1] + neighbour[1][2] + neighbour[1][3] \
					+ neighbour[2][1] + neighbour[2][3] + \
					+ neighbour[3][1] + neighbour[3][2] + neighbour[3][3];
				if ( nCount >= 2 && nCount <=6)
					bCondition1 = TRUE;
				
				//�ж�Z0(P1)=1
				nCount = 0;
				if (neighbour[1][2] == 0 && neighbour[1][1] == 1)
					nCount++;
				if (neighbour[1][1] == 0 && neighbour[2][1] == 1)
					nCount++;
				if (neighbour[2][1] == 0 && neighbour[3][1] == 1)
					nCount++;
				if (neighbour[3][1] == 0 && neighbour[3][2] == 1)
					nCount++;
				if (neighbour[3][2] == 0 && neighbour[3][3] == 1)
					nCount++;
				if (neighbour[3][3] == 0 && neighbour[2][3] == 1)
					nCount++;
				if (neighbour[2][3] == 0 && neighbour[1][3] == 1)
					nCount++;
				if (neighbour[1][3] == 0 && neighbour[1][2] == 1)
					nCount++;
				if (nCount == 1)
					bCondition2 = TRUE;
				
				//�ж�P2*P4*P8=0 or Z0(p2)!=1
				if (neighbour[1][2]*neighbour[2][1]*neighbour[2][3] == 0)
					bCondition3 = TRUE;
				else
				{
					nCount = 0;
					if (neighbour[0][2] == 0 && neighbour[0][1] == 1)
						nCount++;
					if (neighbour[0][1] == 0 && neighbour[1][1] == 1)
						nCount++;
					if (neighbour[1][1] == 0 && neighbour[2][1] == 1)
						nCount++;
					if (neighbour[2][1] == 0 && neighbour[2][2] == 1)
						nCount++;
					if (neighbour[2][2] == 0 && neighbour[2][3] == 1)
						nCount++;
					if (neighbour[2][3] == 0 && neighbour[1][3] == 1)
						nCount++;
					if (neighbour[1][3] == 0 && neighbour[0][3] == 1)
						nCount++;
					if (neighbour[0][3] == 0 && neighbour[0][2] == 1)
						nCount++;
					if (nCount != 1)
						bCondition3 = TRUE;
				}
				
				//�ж�P2*P4*P6=0 or Z0(p4)!=1
				if (neighbour[1][2]*neighbour[2][1]*neighbour[3][2] == 0)
					bCondition4 = TRUE;
				else
				{
					nCount = 0;
					if (neighbour[1][1] == 0 && neighbour[1][0] == 1)
						nCount++;
					if (neighbour[1][0] == 0 && neighbour[2][0] == 1)
						nCount++;
					if (neighbour[2][0] == 0 && neighbour[3][0] == 1)
						nCount++;
					if (neighbour[3][0] == 0 && neighbour[3][1] == 1)
						nCount++;
					if (neighbour[3][1] == 0 && neighbour[3][2] == 1)
						nCount++;
					if (neighbour[3][2] == 0 && neighbour[2][2] == 1)
						nCount++;
					if (neighbour[2][2] == 0 && neighbour[1][2] == 1)
						nCount++;
					if (neighbour[1][2] == 0 && neighbour[1][1] == 1)
						nCount++;
					if (nCount != 1)
						bCondition4 = TRUE;
				}
				
				if(bCondition1 && bCondition2 && bCondition3 && bCondition4)
				{
					pic.SetPixel(i, j, RGB(255, 255, 255));
					bModified = TRUE;
				}
				else
				{
					pic.SetPixel(i, j, RGB(0, 0, 0));
				}
				
			} //for i
		} //for j
	
		*this = pic;
	}//while
}




/**************************************************
BOOL CImgProcess::Hough(SLineInfo *pInfoRet, int nLineRet)
���ܣ�
	����任
ע��
	ֻ�ܴ����ֵͼ��һ��ӦΪ������Ե�������ͼ�񣨺�ɫ��������ɫǰ����
������
	SLineInfo *pInfoRet
		�����ֱ����Ϣ
	int nLineRet
		��ҪѰ�ҵ�ֱ����Ŀ

����ֵ��
	BOOL���ͣ�trueΪ�ɹ���falseΪʧ��
***************************************************/
BOOL CImgProcess::Hough(SLineInfo *pInfoRet, int nLineRet)
{
	int i, j;
	
	
	// ���������е����Rho��Theta
	int nMaxDist = sqrt(GetHeight()*GetHeight() + GetWidthPixel()*GetWidthPixel());
	int nMaxAngle = 90;

	// Ϊ�����������ռ�
	int nAreaNum = nMaxAngle * nMaxDist * 2;
	int *pTransArea = new int[nAreaNum];	
	memset(pTransArea, 0, nAreaNum * sizeof(int));
	
	// ת������������
	BYTE bt;
	int nAngle, nDist; //�������µĽǶȺͼ���
	double fRadian; //����
	for(i = 0; i < GetHeight(); i ++)
	{
		for(j=0; j<GetWidthPixel(); j++)
		{
			bt = GetGray(j, i);
			if(bt == 255)
			{
				for(nAngle = 0; nAngle < nMaxAngle; nAngle ++)
				{
					fRadian = nAngle*2*PI/180.0; //ת��Ϊ����
					nDist = (j*cos(fRadian) + i*sin(fRadian)); //���㼫��
					
					if(nDist >= 0)//������
					{
						pTransArea[nDist*nMaxAngle + nAngle] ++;
					}
					else//������
					{
						nDist = fabs(nDist);
						pTransArea[nMaxAngle * nMaxDist + nDist*nMaxAngle + nAngle] ++;
					}
				}//for nAngle
			}//if
		}//for j
	}//for i
	
	
	SMaxValue MaxValue1;

	//����ʱ�ǶȺͼ����ķ�Χ
	int nMaxDisAllow = 20;
	int nMaxAngleAllow = 5;

	for(int nLine=0; nLine<nLineRet; nLine++) //Ѱ��ǰnLineRet����ֵ��
	{	
		// Ѱ������
		MaxValue1.nValue = 0;
		for(i=0; i<nAreaNum; i++)
		{
			if(pTransArea[i] > MaxValue1.nValue)
			{
				MaxValue1.nValue = pTransArea[i];
				MaxValue1.nAngle = i;
			}
		}
		
		if(MaxValue1.nValue == 0) //�Ҳ������ܵĹ��ߵ�
		{
			return FALSE;
		}
		
		if(MaxValue1.nAngle < nMaxAngle * nMaxDist)
		{
			MaxValue1.nDist = MaxValue1.nAngle/nMaxAngle;
			MaxValue1.nAngle = MaxValue1.nAngle%nMaxAngle;
		}
		else
		{
			MaxValue1.nAngle -= nMaxAngle * nMaxDist;
			
			MaxValue1.nDist = MaxValue1.nAngle/nMaxAngle;
			MaxValue1.nDist *= -1;
			
			
			MaxValue1.nAngle = MaxValue1.nAngle%nMaxAngle;
		}
		
		// �����������pInfoRet�ṹָ��
		pInfoRet[nLine].nAngle = MaxValue1.nAngle*2;
		pInfoRet[nLine].nDist = MaxValue1.nDist;
		pInfoRet[nLine].nPixels = MaxValue1.nValue;	
		
		if(pInfoRet[nLine].nDist < 0)
		{
			pInfoRet[nLine].nAngle = pInfoRet[nLine].nAngle - 180;
			pInfoRet[nLine].nDist = pInfoRet[nLine].nDist*(-1);
			
		}
		
		// �����������㣬ΪѰ����һ����ֵ��׼��
		for(nDist = (-1)*nMaxDisAllow; nDist <= nMaxDisAllow; nDist ++)
		{
			for(nAngle = (-1)*nMaxAngleAllow; nAngle <= nMaxAngleAllow; nAngle ++)
			{
				int nThisDist = MaxValue1.nDist + nDist;
				int nThisAngle = MaxValue1.nAngle + nAngle;
				
				
				
				nThisAngle *= 2;
				
				if(nThisAngle < 0 && nThisAngle >= -180)
				{
					nThisAngle += 180;
					nThisDist *= -1;
				}
				if(nThisAngle >= 180 && nThisAngle < 360)
				{
					nThisAngle -= 180; 
					nThisDist *= -1;
				}
				
				
				
				if(fabs(nThisDist) <= nMaxDist
					&& nThisAngle >= 0 && nThisAngle <= nMaxAngle*2)
				{
					nThisAngle /= 2;
					if(nThisDist >= 0)
					{
						pTransArea[nThisDist*nMaxAngle + nThisAngle] = 0;
					}
					else
					{
						nThisDist = fabs(nThisDist);
						pTransArea[nMaxDist*nMaxAngle + nThisDist*nMaxAngle + nThisAngle] = 0;
					}
				}
			}//for nAngle
		}//for nDist
	}//for nLine

	
	delete []pTransArea; //�ͷż�������ռ�
	
	
	return TRUE;
}
/*
BOOL CImgProcess::Hough(SLineInfo *pInfoRet, int nLineRet)
{
	//�任��ĳߴ�
	int iMaxDist;
	int iMaxAngleNumber;

	//�任�������
	int iDist;
	int iAngleNumber;

	int i, j;//ѭ������
	

	//����ֵ
	unsigned char pixel;

	//�洢�任���е��������ֵ
	SMaxValue MaxValue1;
	SMaxValue MaxValue2;
	

	//����任��ĳߴ�
	//������
	iMaxDist = (int) sqrt(GetWidthPixel()*GetWidthPixel() + GetHeight()*GetHeight());

	//�Ƕȴ�0��180��ÿ��2��
	iMaxAngleNumber = 90;

	int nAreaNum = iMaxAngleNumber * iMaxDist;
	int *pTransArea = new int[nAreaNum];
	
	memset(pTransArea, 0, nAreaNum * sizeof(int));

	for(j = 0; j <GetHeight(); j++)
	{
		for(i = 0;i <GetWidthPixel(); i++)
		{

			pixel = GetGray(i, j);
			// ָ��Դͼ������j�У���i�����ص�ָ��			
			

			//Ŀ��ͼ���к���0��255��������Ҷ�ֵ
			if(pixel != 255 && pixel != 0)
				return FALSE;

			//����Ǻڵ㣬���ڱ任��Ķ�Ӧ�����ϼ�1
			if(pixel == 0)
			{
				//ע�ⲽ����2��
				for(iAngleNumber=0; iAngleNumber<iMaxAngleNumber; iAngleNumber++)
				{
					iDist = (int) fabs(i*cos(iAngleNumber*2*PI/180.0) + \
						j*sin(iAngleNumber*2*PI/180.0));
				
					//�任��Ķ�Ӧ���ϼ�1
					*(pTransArea+iDist*iMaxAngleNumber+iAngleNumber) = \
						*(pTransArea+iDist*iMaxAngleNumber+iAngleNumber) +1;
				}
			}
		
		}
	}
				
	//�ҵ��任���е��������ֵ��
	MaxValue1.nValue=0;
	MaxValue2.nValue=0;
	
	//�ҵ���һ�����ֵ��
	for (iDist=0; iDist<iMaxDist;iDist++)
	{
		for(iAngleNumber=0; iAngleNumber<iMaxAngleNumber; iAngleNumber++)
		{
			if((int)*(pTransArea+iDist*iMaxAngleNumber+iAngleNumber)>MaxValue1.nValue)
			{
				MaxValue1.nValue = (int)*(pTransArea+iDist*iMaxAngleNumber+iAngleNumber);
				MaxValue1.nDist = iDist;
				MaxValue1.nAngle = iAngleNumber;
			}

		}
	}

	//����һ�����ֵ�㸽������
	for (iDist = -9;iDist < 10;iDist++)
	{
		for(iAngleNumber=-1; iAngleNumber<2; iAngleNumber++)
		{
			if(iDist+MaxValue1.nDist>=0 && iDist+MaxValue1.nDist<iMaxDist \
				&& iAngleNumber+MaxValue1.nAngle>=0 && iAngleNumber+MaxValue1.nAngle<=iMaxAngleNumber)
			{
				*(pTransArea+(iDist+MaxValue1.nDist)*iMaxAngleNumber+\
					(iAngleNumber+MaxValue1.nAngle))=0;
			}
		}
	}

	
	pInfoRet[0].nAngle = 2*MaxValue1.nAngle;
	pInfoRet[0].nDist = MaxValue1.nDist;

	for(j = 0; j <GetHeight(); j++)
		{
			for(i = 0;i <GetWidthPixel(); i++)
			{	
				
				// ָ�򻺴�ͼ������j�У���i�����ص�ָ��			
				

				

				//�ڵ�һ��ֱ����
				iDist = (int) fabs(i*cos(MaxValue1.nAngle*2*PI/180.0) + \
							j*sin(MaxValue1.nAngle*2*PI/180.0));
				if (iDist == MaxValue1.nDist)
					pTo->SetPixel(i, j, RGB(0, 0, 0));
			}
	}

	

	// ����
	return TRUE;

}

*/


/******************* 
void CImgProcess::PixelImage(CImgProcess* pTo, int lowerThres, int upperThres)
 
 ���ܣ�
	���ػ���С���� upperThres �� lowerThres ֮�����ͨ�������ػ�������λ��Ϊԭ��������ģ�
	�˳���С���� lowerThres ����ͨ����
	�˳���С���� upperThres ����ͨ����
 ע��
	ֻ�ܴ���2ֵͼ��

 ������
 CImgProcess* pTo: Ŀ��ͼ��� CImgProcess ָ�� 
	upperThres: ������ֵ
	lowerThres: ������ֵ
 
 ����ֵ��
	��
	
*******************/
void CImgProcess::PixelImage(CImgProcess* pTo, int lowerThres, int upperThres)
{
	if(upperThres < lowerThres)
	{
		AfxMessageBox("������ֵ�������������ֵ��");
		return;
	}

	if(lowerThres < 0)
		lowerThres = 0;
	if(upperThres > 1000)
		upperThres = 1000; //Ϊ��ֹ��ȵݹ�ջ�洦���޶�upperThres�����ֵΪ1000

	CImgProcess image_bkp = *this;
	CImgProcess image_res = *pTo;
	image_res.InitPixels(255); //������ػ����ͼ�񣬳�ʼΪ��ɫ��������

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
				if(nRet == 0) // lowerThres<= ... <=upperThres
				{
					//��������������
					int xMean = 0;
					int yMean = 0;
					for(k=0; k<curConnRgnSize; k++)
					{
						image_res.SetPixel(ptVisited[k].x, ptVisited[k].y, RGB(255, 255, 255));

						xMean += ptVisited[k].x;
						yMean += ptVisited[k].y;
					}
					xMean = xMean / curConnRgnSize;
					yMean = yMean / curConnRgnSize;

					image_res.SetPixel(xMean, yMean, RGB(0, 0, 0));
		
				}
				else if( (nRet == 1) || (nRet == -1) ) // >upperThres or <lowerThres
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
TestConnRgn(CImgProcess* pImage, LPBYTE lpVisited, int nWidth, int nHeight, int x, int y, CPoint ptVisited[], int lowerThres, int upperThres, int &curConnRgnSize)
 
 ���ܣ�
	���õݹ��㷨ͳ�Ƶ� (x, y) ��������ͨ���Ĵ�С�� lowerThres �� upperThres ֮��Ĺ�ϵ
 ע��
	ֻ�ܴ���2ֵͼ��

 ������
 CImgProcess* pImage; ����ͼ��� CImgProcess ָ�� 
 LPBYTE lpVisited     ��־λ����
 int nWidth       ͼ��Ŀ��
 int nHeigh       ͼ��ĸ߶�
 int x            ��ǰ�����ĺ�����
 int y            ��ǰ������������
 Cpoint ptVisited[]     ����ѿ�����ĵ������
 int curConnRgnSize     ��ǰΪֹ���ֵ���ͨ���Ĵ�С
 
 ����ֵ��
 int
	= 0: ��ͨ����С���� lowerThres �� upperThres ֮��
	= 1: ��ͨ����С���� upperThres
	=-1: ��ͨ����С���� lowerThres
 
*******************/
int CImgProcess::TestConnRgn(CImgProcess* pImage, LPBYTE lpVisited, int nWidth, int nHeight, int x, int y, CPoint ptVisited[], int lowerThres, int upperThres, int &curConnRgnSize)
{ 
	
	if(curConnRgnSize > upperThres) //��ͨ����С�ѳ���������ֵupperThres
	   return 1;

	curConnRgnSize++; //���µ�ǰΪֹ���ֵ���ͨ���Ĵ�С
	lpVisited[nWidth*y+x]=true; //����ѷ���

    ptVisited[curConnRgnSize-1].x=x;
    ptVisited[curConnRgnSize-1].y=y; //��¼�ѷ��ʵ�����


    
	unsigned char gray;
 
    if(curConnRgnSize >= upperThres)
	   return 1;
	else
	{//����8�ڽӵĵ㣬�����Ϊ��ɫ�����壩���ݹ�����Լ�����ͨ����С+1�����������ڽӵ��8�ڽӵ㣩

		//����ĵ�
		if(y-1>=0)
		{
			gray = pImage->GetGray(x, y-1);
			if(gray==0 && lpVisited[(y-1)*nWidth+x] == false) 
	  			TestConnRgn(pImage, lpVisited, nWidth, nHeight, x, y-1, ptVisited, lowerThres, upperThres, curConnRgnSize);
		}
        if(curConnRgnSize > upperThres)
	        return 1;

		//���ϵ�
		if(y-1>=0 && x-1>=0)
		{
			gray = pImage->GetGray(x-1, y-1);
			if(gray==0 && lpVisited[(y-1)*nWidth+x-1]==false)
				TestConnRgn(pImage, lpVisited, nWidth, nHeight,x-1,y-1,ptVisited, lowerThres, upperThres, curConnRgnSize);
		}
        if(curConnRgnSize > upperThres)
	        return 1;

		//���
		if(x-1>=0)
		{
			gray = pImage->GetGray(x-1, y);
			if(gray==0 && lpVisited[y*nWidth+x-1]==false)
        		TestConnRgn(pImage, lpVisited, nWidth, nHeight, x-1, y, ptVisited, lowerThres, upperThres, curConnRgnSize);
		}
        if(curConnRgnSize > upperThres)
			return 1;

		//����
		if(y+1<nHeight && x-1>=0)
        {
			gray = pImage->GetGray(x-1, y+1);
			if(gray==0 && lpVisited[(y+1)*nWidth+x-1]==false)
			TestConnRgn(pImage, lpVisited, nWidth, nHeight, x-1, y+1, ptVisited, lowerThres, upperThres, curConnRgnSize);
		}
        if(curConnRgnSize > upperThres)
		   return 1;

		//�·�
		if( y+1<nHeight)
		{
			gray = pImage->GetGray(x, y+1);
			if(gray==0 &&lpVisited[(y+1)*nWidth+x]==false)
				TestConnRgn(pImage, lpVisited,nWidth,nHeight,x,y+1,ptVisited,lowerThres, upperThres, curConnRgnSize);
		}
        if(curConnRgnSize > upperThres)
			return 1;

		//����
		if(y+1<nHeight && x+1<nWidth)
		{
			gray = pImage->GetGray(x+1, y+1);
			if(gray==0 && lpVisited[(y+1)*nWidth+x+1]==false)
				TestConnRgn(pImage, lpVisited,nWidth,nHeight,x+1,y+1,ptVisited,lowerThres, upperThres, curConnRgnSize);
		}
        if(curConnRgnSize > upperThres)
			return 1;

		//�ұ�
		if(x+1<nWidth)
		{
			gray = pImage->GetGray(x+1, y);
			if(gray==0 && lpVisited[y*nWidth+x+1]==false)
				TestConnRgn(pImage, lpVisited, nWidth, nHeight, x+1, y, ptVisited, lowerThres, upperThres, curConnRgnSize);
		}
        if(curConnRgnSize > upperThres)
			return 1;

		//����
		if(y-1>=0 && x+1<nWidth)
		{
			gray = pImage->GetGray(x+1, y-1);
			if(gray==0 && lpVisited[(y-1)*nWidth+x+1]==false)
				TestConnRgn(pImage,lpVisited,nWidth,nHeight,x+1,y-1, ptVisited, lowerThres, upperThres, curConnRgnSize);
		}
        if(curConnRgnSize > upperThres)
			return 1;
           
	}//else

	if (curConnRgnSize < lowerThres)
		return -1; //��ͨ����С���� lowerThres


    return 0;//��ͨ����С���� lowerThres �� upperThres ֮��
}



















//***************************************ͶӰ����У�����*************************************//

/*******************
void CImgProcess::ProdMat(double** ppDbMat, double *pDbSrc2, double *pDbDest, int y, int x, int z)

���ܣ�����������ĳ˻�

ע���ú������������������ˣ�Ȼ����˵Ľ�������pDbDest�С�
    ����pDbSrc1 *�Ĵ�СΪ x*z��pDbSrc2�Ĵ�СΪ z*y��pDbDest�Ĵ�СΪ x*y

������
double  *pDbSrc1	- ָ����˾���1���ڴ�
double  *pDbSrc2	- ָ����˾���2���ڴ�
double  *pDbDest   - ��ž���������н�����ڴ�ָ��
int     x		- ����ĳߴ磬����μ�����ע
int     y		- ����ĳߴ磬����μ�����ע
int     z		- ����ĳߴ磬����μ�����ע
 
 ����ֵ:
	��
 
 *******************/
void CImgProcess::ProdMat(double** ppDbMat, double *pDbSrc2, double *pDbDest, int y, int x, int z)
{
	int nCnt = 0;
	int i,j;
	double * pDbSrc1 = new double[m_nBasePt * m_nBasePt];
	for(i=0; i<m_nBasePt; i++)
	{
		for(j=0; j<m_nBasePt; j++)
			pDbSrc1[nCnt++] = ppDbMat[i][j];
	}

	for(i=0;i<y;i++)
	{
		for(j=0;j<x;j++)
		{
			pDbDest[i*x + j] = 0;
			for(int m=0;m<z;m++)
				pDbDest[i*x + j] += pDbSrc1[i*z + m]*pDbSrc2[m*x + j];
		}
	}

	nCnt = 0;
	for(i=0; i<m_nBasePt; i++)
	{
		for(j=0; j<m_nBasePt; j++)
			ppDbMat[i][j] = pDbSrc1[nCnt++];
	}

	delete []pDbSrc1;
}


/*******************
void CImgProcess::GetProjPara(CPoint* pPointBase, CPoint* pPointSampl, double* pDbProjPara)

���ܣ����ݻ�׼��Զ���4�Զ���ȷ���任����

������
CPoint* pPointBase����׼ͼ��Ļ�׼��
CPoint* pPointSampl������ͼ��Ļ�׼��
double* pDbProjPara���任����
 
����ֵ:
	��
 
 *******************/
void CImgProcess::GetProjPara(CPoint* pPointBase, CPoint* pPointSampl, double* pDbProjPara)
{
	int i;

	//ͶӰ���Է��̵�ϵ������
	double** ppParaMat;
	ppParaMat = new double*[m_nBasePt];
	for(i=0; i<m_nBasePt; i++)
	{
		ppParaMat[i] = new double[m_nBasePt];	
	}

	for(i=0; i<m_nBasePt; i++)
	{
		ppParaMat[i][0] = pPointBase[i].x;
		ppParaMat[i][1] = pPointBase[i].y;
		ppParaMat[i][2] = pPointBase[i].x * pPointBase[i].y;
		ppParaMat[i][3] = 1;
	}

	double* pResMat;//�������
	pResMat = new double[m_nBasePt];
	for(i=0; i<m_nBasePt; i++)//����ǰ�ĸ�ϵ�� c1,c2,c3,c4
	{
		pResMat[i] = pPointSampl[i].x; //ͶӰ���Է��̵�ֵx'
	}
	
	// �������ϵ������������ķ������ͶӰ�任��ǰ4��ϵ�� c1,c2,c3,c4
	InvMat(ppParaMat, m_nBasePt);
	ProdMat(ppParaMat, pResMat, pDbProjPara, m_nBasePt, 1, m_nBasePt);//���ǰ4��ϵ��
	
	for(i=0; i<m_nBasePt; i++)//������ĸ�ϵ�� c5,c6,c7,c8
	{
		pResMat[i] = pPointSampl[i].y; //ͶӰ���Է��̵�ֵy'
	}
	// �������ϵ������������ķ������ͶӰ�任�ĺ�4��ϵ�� c5,c6,c7,c8
	ProdMat(ppParaMat, pResMat, pDbProjPara+m_nBasePt, m_nBasePt, 1, m_nBasePt);//�����4��ϵ��


	//�ͷſռ�
	delete[] pResMat;

	for(i=0; i<m_nBasePt; i++)
	{
		delete[] ppParaMat[i];	
	}
	delete[] ppParaMat;
}


/*******************
BOOL CImgProcess::InvMat(double** ppDbMat, int nLen)

���ܣ�����ppDbMat�������

ע��ppDbMat����Ϊ����

������
double** ppDbMat���������
int nLen������ppDbMat�ĳߴ�

����ֵ:
	BOOL
	=true��ִ�гɹ�
	=false����������г��ִ���
*******************/
BOOL CImgProcess::InvMat(double** ppDbMat, int nLen)
{
	double* pDbSrc = new double[nLen * nLen];
	
	int *is,*js,i,j,k;

	//����Ҫ������������
	int nCnt = 0;
	for(i=0; i<nLen; i++)
	{
		for(j=0; j<nLen; j++)
			pDbSrc[nCnt++] = ppDbMat[i][j];
	}

	double d,p;
	is = new int[nLen];
	js = new int[nLen];
	for(k=0;k<nLen;k++)
	{
		d=0.0;
		for(i=k;i<nLen;i++)
			for(j=k;j<nLen;j++)
			{
				p=fabs(pDbSrc[i*nLen + j]); //�ҵ�����ֵ����ϵ��
				if(p>d)
				{
					d = p; 

					//��¼����ֵ����ϵ�����С�������
					is[k] = i;
					js[k] = j;
				}
			}
		if(d+1.0==1.0)
		{//ϵ��ȫ��0��ϵ������Ϊ0 �󣬴�ʱΪ�������
			delete is;
			delete js;
			return FALSE;
		}
		if(is[k] != k) //��ǰ�в��������Ԫ��
			for(j=0;j<nLen;j++)
			{
				//��������Ԫ��
				p = pDbSrc[k*nLen + j];
				pDbSrc[k*nLen + j] = pDbSrc[(is[k]*nLen) + j];
				pDbSrc[(is[k])*nLen + j] = p;
			}
		if(js[k] != k) //��ǰ�в��������Ԫ��
			for(i=0; i<nLen; i++)
			{
				//��������Ԫ��
				p = pDbSrc[i*nLen + k];
				pDbSrc[i*nLen + k] = pDbSrc[i*nLen + (js[k])];
				pDbSrc[i*nLen + (js[k])] = p;
			}

		pDbSrc[k*nLen + k]=1.0/pDbSrc[k*nLen + k]; //����Ԫ�ĵ��� 
		
		// a[k,j]a[k,k] -> a[k,j]
		for(j=0; j<nLen; j++)
			if(j != k)
			{
				pDbSrc[k*nLen + j]*=pDbSrc[k*nLen + k];
			}
	
		// a[i,j] - a[i,k]a[k,j] -> a[i,j]
		for(i=0; i<nLen; i++)
			if(i != k)
				for(j=0; j<nLen; j++)
					if(j!=k)
					{
						pDbSrc[i*nLen + j] -= pDbSrc[i*nLen + k]*pDbSrc[k*nLen + j];
					}

		// -a[i,k]a[k,k] -> a[i,k]
		for(i=0; i<nLen; i++)
			if(i != k)
			{
				pDbSrc[i*nLen + k] *= -pDbSrc[k*nLen + k];
			}
	}
	for(k=nLen-1; k>=0; k--)
	{
		//�ָ��� 
		if(js[k] != k)
			for(j=0; j<nLen; j++)
			{
				p = pDbSrc[k*nLen + j];
				pDbSrc[k*nLen + j] = pDbSrc[(js[k])*nLen + j];
				pDbSrc[(js[k])*nLen + j] = p;
			}
		//�ָ���
		if(is[k] != k)
			for(i=0; i<nLen; i++)
			{
				p = pDbSrc[i*nLen + k];
				pDbSrc[i*nLen + k] = pDbSrc[i*nLen +(is[k])];
				pDbSrc[i*nLen + (is[k])] = p;
			}
	}

	//�����������ϵ������ppDbMat
	nCnt = 0;
	for(i=0; i<m_nBasePt; i++)
	{
		for(j=0; j<m_nBasePt; j++)
		{
			ppDbMat[i][j] = pDbSrc[nCnt++];
		}
	}

	//�ͷſռ�
	delete is;
	delete js;
	delete[] pDbSrc;

	return TRUE;	

}


/*******************
MYPOINT CImgProcess::ProjTrans(CPoint pt, double* pDbProjPara)

���ܣ����ݱ任�����Ե�ptʵʩͶӰ�任

������
CPoint pt��Ҫ����ͶӰ�任�ĵ�����
double* pDbProjPara���任����

����ֵ:
	MYPOINT 
*******************/
MYPOINT CImgProcess::ProjTrans(CPoint pt, double* pDbProjPara)
{
	MYPOINT retPt;
	retPt.x = pDbProjPara[0] * pt.x + pDbProjPara[1] * pt.y + pDbProjPara[2] * pt.x * pt.y + pDbProjPara[3];
	retPt.y = pDbProjPara[4] * pt.x + pDbProjPara[5] * pt.y + pDbProjPara[6] * pt.x * pt.y + pDbProjPara[7];
	return retPt;
}


/*******************
BOOL CImgProcess::ImProjRestore(CImgProcess* pTo, CPoint *pPointBase, CPoint *pPointSampl, bool bInterp)

���ܣ�ʵʩͶӰ����У��

������
CImgProcess* pTo��У׼��ͼ��� CImgProcess ָ��
CPoint *pPointBase����׼ͼ��Ļ�׼������
CPoint *pPointSampl������ͼ��Ļ�׼������
bool bInterp���Ƿ�ʹ��(˫����)��ֵ

����ֵ:
	MYPOINT 
*******************/
BOOL CImgProcess::ImProjRestore(CImgProcess* pTo, CPoint *pPointBase, CPoint *pPointSampl, bool bInterp)
{
	double* pDbProjPara = new double[m_nBasePt * 2];
	GetProjPara(pPointBase, pPointSampl, pDbProjPara);

	//�õõ��ı任ϵ����ͼ��ʵʩ�任
	int i, j;
	pTo->InitPixels(255); //���Ŀ��ͼ��
	int nHeight = pTo->GetHeight();
	int nWidth = pTo->GetWidthPixel();
	for(i=0; i<nHeight; i++)
	{
		for(j=0; j<nWidth; j++)
		{
			//��ÿ����(j, i)��������ͶӰʧ���ĵ�ptProj
			MYPOINT ptProj = ProjTrans( CPoint(j, i), pDbProjPara );
			
			
			if(bInterp)
			{
				int nGray = InterpBilinear(ptProj.x, ptProj.y); //����ͼ��ͶӰ����ͼ�񣩵Ķ�Ӧ��Ҷ�
				if(nGray >= 0)
					pTo->SetPixel(j, i, RGB(nGray, nGray, nGray));
				else
					pTo->SetPixel(j, i, RGB(255, 255, 255)); //����ͼ��Χ������ɫ
			}
			else
			{
				int ii = ptProj.y + 0.5; //�������������ڲ�ֵ
				int jj = ptProj.x + 0.5;
				if( ii>0 && ii<GetHeight() && jj>0 && jj<GetWidthPixel() )
					pTo->SetPixel(j, i, GetPixel(jj, ii));
				else
					pTo->SetPixel(j, i, RGB(255, 255, 255)); //����ͼ��Χ������ɫ
			}
		}
	}

	delete pDbProjPara;
	return TRUE;
}


/**************************************************
BOOL CImgProcess::FFT2(CImgProcess * pTo, BOOL bExpand, complex<double> * pOutput, BYTE bFillColor)
���ܣ�
	��ά���ٸ���Ҷ�任

������
	CImgProcess * pTo
		ָ�����ͼ���ָ�룬����ΪNULL�����ͼ��
	BOOL bExpand
		ָ��ʹ�ú��ַ�����ͼ��߿�������2�������ݣ�
		������Ϊtrue����ʹ��ָ����ɫ����ͼ��
		������Ϊfalse������Ҳ�͵ײ��ü�ͼ��
		Ĭ��ֵȡfalse�����ü�ͼ��
	complex<double> * pOutput
		ָ��ԭʼ��������ָ�룬Ĭ�ϲ����ԭʼ���ݣ���Ĭ��ΪNULL
	BYTE bFillColor
		��bExpand������Ϊtrueʱ���������ָ��ʹ�ú�����ɫ����ͼ��
		��bExpand������Ϊfalseʱ��������������ԡ�Ĭ��ֵΪ255����ɫ����

����ֵ��
	BOOL���ͣ�trueΪ�ɹ���falseΪʧ��
***************************************************/

BOOL CImgProcess::FFT2(CImgProcess * pTo, BOOL bExpand, complex<double> * pOutput, BYTE bFillColor)
{
	if (m_pBMIH->biBitCount!=8) return false;

	// �м����
	double	dTemp;
	
	// ѭ������
	LONG		i;
	LONG		j;
	
	// FFT2�Ŀ�Ⱥ͸߶ȣ�2�������η���
	LONG		w;
	LONG		h;
	
	int		wp;
	int		hp;
	
	// ����ֵ
	w = 1;
	h = 1;
	wp = 0;
	hp = 0;
	
	// ������и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	while(w * 2 <= GetWidthPixel())
	{
		w *= 2;
		wp++;
	}
	
	while(h * 2 <= GetHeight())
	{
		h *= 2;
		hp++;
	}

	// ���bExpand����
	if ((bExpand) && (w!=GetWidthPixel()) &&(h!=GetHeight())) {
		w *= 2; wp++;
		h *= 2; hp++;
	}
	
	// �����ڴ�
	complex<double> *TD = new complex<double>[w * h];
	complex<double> *FD = new complex<double>[w * h];
	
	// ��ֱ����
	for(i = 0; i < h; i++)
	{
		// ˮƽ����
		for(j = 0; j < w; j++)
		{
			// ��ʱ��ֵ
			if (bExpand)
			{
				if ((j<GetWidthPixel()) && (i<GetHeight()))
				{
					TD[j + w * i] = complex<double>(GetGray(j, i), 0);
				}
				else
				{
					// ����ԭͼ��Χ��ʹ�ø�����ɫ���
					TD[j + w * i] = complex<double>(bFillColor, 0);
				}
			}
			else
			{
				TD[j + w * i] = complex<double>(GetGray(j, i), 0);
			}
		}
	}
	
	for(i = 0; i < h; i++)
	{
		// ��y������п��ٸ���Ҷ�任
		FFT(&TD[w * i], &FD[w * i], wp);
	}
	
	// ����任���
	for(i = 0; i < h; i++)
	{
		for(j = 0; j < w; j++)
		{
			TD[i + h * j] = FD[j + w * i];
		}
	}
	
	for(i = 0; i < w; i++)
	{
		// ��x������п��ٸ���Ҷ�任
		FFT(&TD[i * h], &FD[i * h], hp);
	}
	
	// �����������
	if (pOutput)
	{
		// ��ֱ����
		for(i = 0; i < h; i++)
		{
			// ˮƽ����
			for(j = 0; j < w; j++)
			{
				pOutput[i * w + j] = FD[j * h + i];
			}
		}
	}

	// �������ͼ��
	if (pTo)
	{
		// �������ͼ���С
		pTo->ImResize(h, w);

		// Ѱ�ҷ����׶����任�����ֵ����Сֵ��Ϊ�Ż���������ʾ�����׼��
		// �����׶����任������ֵ����Сֵ�ֱ�������
		double dMax = 0, dMin = 1E+006;

		for (i=0; i<h; i++)
		{
			for (j=0; j<w; j++)
			{
				// ���������
				dTemp = sqrt(FD[j * h + i].real() * FD[j * h + i].real() + 
							 FD[j * h + i].imag() * FD[j * h + i].imag()) / 100;
				
				// �����任
				dTemp = log(1+dTemp);

				// Ѱ��������Сֵ
				dMax = max(dMax, dTemp);
				dMin = min(dMin, dTemp);
			}
		}

		for (i=0; i<h; i++)
		{
			for (j=0; j<w; j++)
			{
				// ���������
				dTemp = sqrt(FD[j * h + i].real() * FD[j * h + i].real() + 
							 FD[j * h + i].imag() * FD[j * h + i].imag()) / 100;
				
				// �����任
				dTemp = log(1+dTemp);

				// �ı䶯̬��Χ����һ����0~255
				dTemp = (dTemp - dMin) / (dMax - dMin) * 255;
				
				// ����Ŀ��ͼ��
				// �˴���ֱ��ȡj��i����Ϊ�˽��任���ԭ���Ƶ�����
				pTo->SetPixel((j<w/2 ? j+w/2 : j-w/2),(i<h/2 ? i+h/2 : i-h/2), RGB(dTemp, dTemp, dTemp));
			}
		}
	}

	// ɾ����ʱ����
	delete TD;
	delete FD;

	return true;
}

/**************************************************
BOOL CImgProcess::IFFT2(CImgProcess * pTo, complex<double> * pInput, long lWidth, long lHeight, long lOutW, long lOutH)

���ܣ�
	��ά���ٷ�����Ҷ�任

������
	CImgProcess * pTo
		ָ�����ͼ���ָ��
	complex<double> * pInput
		ָ�����������ָ��
	long lWidth
		������������Ҫ���з�����Ҷ�任�Ŀ��
	long lHeight
		������������Ҫ���з�����Ҷ�任�ĸ߶�
	long lOutW
		ָ�����ͼ��Ŀ�ȣ�����ʡ�ԣ�Ĭ����������������ͬ
	long lOutH
		ָ�����ͼ��ĸ߶ȣ�����ʡ�ԣ�Ĭ������������߶���ͬ
����ֵ��
	BOOL���ͣ�trueΪ�ɹ���falseΪʧ��
***************************************************/

BOOL CImgProcess::IFFT2(CImgProcess * pTo, complex<double> * pInput, long lWidth, long lHeight, long lOutW, long lOutH)
{
	if (m_pBMIH->biBitCount!=8) return false;

	// �м����
	double	dTemp;
	
	// ѭ������
	LONG		i;
	LONG		j;
	
	// IFFT2�Ŀ�Ⱥ͸߶ȣ�2�������η���
	LONG		w;
	LONG		h;
	
	int		wp;
	int		hp;
	
	// ����ֵ
	w = 1;
	h = 1;
	wp = 0;
	hp = 0;
	
	// ���ͼ��ĸ߿�
	if (lOutH == 0) lOutH = lHeight;
	if (lOutW == 0) lOutW = lWidth;
	
	// ������з�����Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	while(w * 2 <= lWidth)
	{
		w *= 2;
		wp++;
	}
	
	while(h * 2 <= lHeight)
	{
		h *= 2;
		hp++;
	}
	
	// �����ڴ�
	complex<double> *TD = new complex<double>[w * h];
	complex<double> *FD = new complex<double>[w * h];
	
	// �趨���ͼ���С
	pTo->ImResize(lOutH, lOutW);

	// ��ֱ����
	for(i = 0; i < h; i++)
	{
		// ˮƽ����
		for(j = 0; j < w; j++)
		{
			// ��Ƶ��ֵ
			FD[j + w * i] = pInput[j + w * i];
		}
	}
	
	for(i = 0; i < h; i++)
	{
		// ��y������п��ٷ�����Ҷ�任
		IFFT(&FD[w * i], &TD[w * i], wp);
	}
	
	// ����任���
	// ��ֱ����
	for(i = 0; i < h; i++)
	{
		// ˮƽ����
		for(j = 0; j < w; j++)
		{
			FD[i + h * j] = TD[j + w * i];
		}
	}
	
	for(i = 0; i < w; i++)
	{
		// ��x������п��ٷ�����Ҷ�任
		IFFT(&FD[i * h], &TD[i * h], hp);
	}
	
	// Ѱ�ҷ��任��������任�����ֵ����Сֵ��Ϊ�Ż���ʾ�����׼��
	// �����ֵ����Сֵ�ֱ�������
	double dMax = 0, dMin = 1E+006;

	for (i=0; i<lOutH; i++)
	{
		for (j=0; j<lOutW; j++)
		{
			dTemp = TD[j * h + i].real();
			
			// Ѱ��������Сֵ
			dMax = max(dMax, dTemp);
			dMin = min(dMin, dTemp);
		}
	}

	// ��
	for(i = 0; i < lOutH; i++)
	{
		// ��
		for(j = 0; j < lOutW; j++)
		{
			dTemp = TD[j * h + i].real();
			
			// �ı䶯̬��Χ����һ����0~255
			dTemp = (dTemp - dMin) / (dMax - dMin) * 255;
			
			// ����Ŀ��ͼ��
			pTo->SetPixel(j, i, RGB(dTemp, dTemp, dTemp));
		}
	}
	
	// ɾ����ʱ����
	delete TD;
	delete FD;

	return true;
}

/**************************************************
BOOL CImgProcess::FreqFilt(CImgProcess * pTo, double * pdFilter, BYTE bFillColor)

���ܣ�
	ִ��Ƶ���˲�������������ʹ����Ӧ���˾����ɺ�������dFilter�˾���

������
	CImgProcess * pTo
		ָ�����ͼ���ָ��
	double * pdFilter
		������Ƶ���˾�
	BYTE bFillColor
		��������ԭͼ��ʹ�õ���ɫ��Ĭ��Ϊ255����ɫ����
		������ͼ���Ҳ�͵ײ���Ե��������ɫ��������һ�¡�

����ֵ��
	BOOL���ͣ�trueΪ�ɹ���falseΪʧ��
***************************************************/

BOOL CImgProcess::FreqFilt(CImgProcess * pTo, double * pdFilter, BYTE bFillColor)
{
	if (m_pBMIH->biBitCount!=8) return false;

	// �����˾���С
	LONG w = GetFreqWidth();
	LONG h = GetFreqHeight();

	// ������ʱƵ��ͼ�����������洢��
	complex<double> * cdFreqImg = new complex<double>[w*h];
	
	// ���ȶ�ԭͼ����и���Ҷ�任
	FFT2(NULL, 1, cdFreqImg, bFillColor);

	// Ȼ��ִ�к����˲���������Ƶ��ͼ����˾���Ԫ�����
	for (LONG i = 0; i<w*h; i++)
	{
		cdFreqImg[i] = cdFreqImg[i] * pdFilter[i];
	}

	// ����˲�������и���Ҷ���任
	IFFT2(pTo, cdFreqImg, w, h, GetWidthPixel(), GetHeight());

	// ���ڶ�̬��Χ���⣬���ԭͼ����ڽ����ԵĻҶȷֽ磬
	// ������ȫ��֤�˲�����ڻҶȲ������ԭͼ�񱣳���ͬ��
	// ��˿�����Ҫ�����ͼ���ٽ��е����������

	// ɾ����ʱƵ��ͼ��
	delete cdFreqImg;

	return true;
}

/**************************************************
BOOL CImgProcess::FreqIdealLPF(double * pdFilter, int nFreq)

���ܣ�
	���ɶ�Ӧ������ͼ���Ƶ�������ͨ�˾�

������
	double * pdFilter
		ָ������˾���ָ��
	int nFreq
		��ֹƵ��

����ֵ��
	BOOL���ͣ�trueΪ�ɹ���falseΪʧ��
***************************************************/

BOOL CImgProcess::FreqIdealLPF(double * pdFilter, int nFreq)
{
	if (m_pBMIH->biBitCount!=8) return false;

	// �����˾���С
	LONG w = GetFreqWidth();
	LONG h = GetFreqHeight();

	// �˾���������
	for (int i=0; i<h; i++)
	{
		for (int j=0; j<w; j++)
		{
			// ������ԭ�������ĵ��˾����Լ򻯲�����������Ҫ�ֱ������˾����ĸ�����
			if (sqrt( pow((double)(i-h/2),2) + pow((double)(j-w/2),2) ) > nFreq)
			{
				// ��д��ʱ���˾����б�Ҫ��������MATLAB����ifftshift��ԭ��ƽ��
				pdFilter[(i<h/2 ? i+h/2 : i-h/2 ) * w + (j<w/2 ? j+w/2 : j-w/2)] = 0;
			}
			else
			{
				pdFilter[(i<h/2 ? i+h/2 : i-h/2 ) * w + (j<w/2 ? j+w/2 : j-w/2)] = 1;
			}
		}
	}
	
	return true;
}

/**************************************************
BOOL CImgProcess::FreqGaussLPF(double * pdFilter, double dSigma)

���ܣ�
	���ɶ�Ӧ������ͼ��ĸ�˹��ͨ�˾�

������
	double * pdFilter
		ָ������˾���ָ��
	double dSigma
		��˹�˲�����Sigma����

����ֵ��
	BOOL���ͣ�trueΪ�ɹ���falseΪʧ��
***************************************************/

BOOL CImgProcess::FreqGaussLPF(double * pdFilter, double dSigma)
{
	if (m_pBMIH->biBitCount!=8) return false;

	// �����˾���С
	LONG w = GetFreqWidth();
	LONG h = GetFreqHeight();

	// �˾���������
	for (int i=0; i<h; i++)
	{
		for (int j=0; j<w; j++)
		{
			// ������ԭ�������ĵ��˾����Լ򻯲�����������Ҫ�ֱ������˾����ĸ�����
			// ��д��ʱ���˾����б�Ҫ��������MATLAB����ifftshift��ԭ��ƽ��
			pdFilter[(i<h/2 ? i+h/2 : i-h/2 ) * w + (j<w/2 ? j+w/2 : j-w/2)] =
				exp(-(pow((double)(i-h/2),2)+pow((double)(j-w/2),2))/2/pow(dSigma,2));
		}
	}
	
	return true;
}

/**************************************************
BOOL CImgProcess::FreqGaussHPF(double * pdFilter, double dSigma)

���ܣ�
	���ɶ�Ӧ������ͼ��ĸ�˹��ͨ�˾�

������
	double * pdFilter
		ָ������˾���ָ��
	double dSigma
		��˹�˲�����Sigma����

����ֵ��
	BOOL���ͣ�trueΪ�ɹ���falseΪʧ��
***************************************************/

BOOL CImgProcess::FreqGaussHPF(double * pdFilter, double dSigma)
{
	if (m_pBMIH->biBitCount!=8) return false;

	// �����˾���С
	LONG w = GetFreqWidth();
	LONG h = GetFreqHeight();

	// �˾���������
	for (int i=0; i<h; i++)
	{
		for (int j=0; j<w; j++)
		{
			// ������ԭ�������ĵ��˾����Լ򻯲�����������Ҫ�ֱ������˾����ĸ�����
			// ��д��ʱ���˾����б�Ҫ��������MATLAB����ifftshift��ԭ��ƽ��
			pdFilter[(i<h/2 ? i+h/2 : i-h/2 ) * w + (j<w/2 ? j+w/2 : j-w/2)] =
				1 - exp(-(pow((double)(i-h/2),2)+pow((double)(j-w/2),2))/2/pow(dSigma,2));
		}
	}
	
	return true;
}

/**************************************************
BOOL CImgProcess::FreqLaplace(double * pdFilter)

���ܣ�
	���ɶ�Ӧ������ͼ���Ƶ��������˹�˲���

������
	double * pdFilter
		ָ������˾���ָ��

����ֵ��
	BOOL���ͣ�trueΪ�ɹ���falseΪʧ��
***************************************************/

BOOL CImgProcess::FreqLaplace(double * pdFilter)
{
	if (m_pBMIH->biBitCount!=8) return false;

	// �����˾���С
	LONG w = GetFreqWidth();
	LONG h = GetFreqHeight();

	// �˾���������
	for (int i=0; i<h; i++)
	{
		for (int j=0; j<w; j++)
		{
			// ������ԭ�������ĵ��˾����Լ򻯲�����������Ҫ�ֱ������˾����ĸ�����
			// ��д��ʱ���˾����б�Ҫ��������MATLAB����ifftshift��ԭ��ƽ��
			pdFilter[(i<h/2 ? i+h/2 : i-h/2 ) * w + (j<w/2 ? j+w/2 : j-w/2)] =
				- (pow((double)(i-h/2),2) + pow((double)(j-w/2),2));
		}
	}
	
	return true;
}

/**************************************************
BOOL CImgProcess::FreqGaussBRF(double * pdFilter , int nFreq, int nWidth)

���ܣ�
	���ɶ�Ӧ������ͼ��ĸ�˹�����˲���

������
	double * pdFilter
		ָ������˾���ָ��
	int nFreq
		���������Ƶ��
	int nWidth
		����Ŀ��

����ֵ��
	BOOL���ͣ�trueΪ�ɹ���falseΪʧ��
***************************************************/

BOOL CImgProcess::FreqGaussBRF(double * pdFilter , int nFreq, int nWidth)
{
	if (m_pBMIH->biBitCount!=8) return false;

	// �����˾���С
	LONG w = GetFreqWidth();
	LONG h = GetFreqHeight();

	// �˾���������
	for (int i=0; i<h; i++)
	{
		for (int j=0; j<w; j++)
		{
			// ������ԭ�������ĵ��˾����Լ򻯲�����������Ҫ�ֱ������˾����ĸ�����
			// ��д��ʱ���˾����б�Ҫ��������MATLAB����ifftshift��ԭ��ƽ��
			pdFilter[(i<h/2 ? i+h/2 : i-h/2 ) * w + (j<w/2 ? j+w/2 : j-w/2)] =
				1 - exp(-0.5 * pow( (pow((double)(i - h/2), 2) + pow((double)(j - w/2),2) - pow((double)nFreq, 2))/
				(sqrt( pow((double)(i - h/2), 2) + pow((double)(j - w/2),2) ) * nWidth ) ,2 ));
		}
	}
	
	return true;
}

/**************************************************
BOOL CImgProcess::EdgeRoberts(CImgProcess * pTo, BYTE bThre, BYTE bEdgeType, BOOL bThinning, BOOL bGOnly)

���ܣ�
	��ԵѰ�ң�ʹ��Roberts����

������
	CImgProcess * pTo
		ָ�����ͼ���ָ��
	BYTE bThre
		�϶���Ե����ֵ��Ĭ��Ϊ0�����Զ�ȷ����ֵ
	BYTE bEdgeType
		_EdgeAll-���б�Ե _EdgeH-��Ч _EdgeV-��Ч _EdgeCW-45�ȱ�Ե _EdgeCCW-135�ȱ�Ե ����-��Ч
	BOOL bThinning
		�����Ƿ���б�Եϸ����Ĭ��Ϊtrue����ִ�б�Եϸ��
	BOOL bGOnly
		�����Ƿ������ݶ�ͼ��Ĭ��Ϊfalse���������ֵ����Ķ�ֵͼ��
		���˲���Ϊtrueʱ��bThre������bThinning������������

����ֵ��
	�������ͣ�trueΪ�ɹ���falseΪʧ��
***************************************************/

BOOL CImgProcess::EdgeRoberts(CImgProcess * pTo, BYTE bThre, BYTE bEdgeType, BOOL bThinning, BOOL bGOnly)
{
	if (m_pBMIH->biBitCount!=8) return false;

	// ����ģ������
	// 45�ȱ�Ե
	const FLOAT cfRobertCW[4] = { -1, 0,
								   0, 1 };
	// 135�ȱ�Ե
	const FLOAT cfRobertCCW[4] = { 0, -1,
								   1,  0 };

	// ��ʱCImgProcess����
	CImgProcess imgTemp = *this;
	CImgProcess imgMid = *this;

	// ����ѡ��ı�Ե����Ӧ��ģ��
	switch (bEdgeType) {
		case 0:		// ���б�Ե
			Template(&imgTemp, 2, 2, 0, 0, (FLOAT*) cfRobertCW, 1);
			Template(&imgMid, 2, 2, 0, 0, (FLOAT*) cfRobertCCW, 1);
			imgTemp = imgTemp + imgMid;
			break;

		case 1:		// ˮƽ��Ե������
			return false;

		case 2:		// ��ֱ��Ե������
			return false;
		
		case 3:		// 45�ȱ�Ե
			Template(&imgTemp, 2, 2, 0, 0, (FLOAT*) cfRobertCW, 1);
			break;
		
		case 4:		// 135�ȱ�Ե
			Template(&imgTemp, 2, 2, 0, 0, (FLOAT*) cfRobertCCW, 1);
			break;
		
		default:			// ��������
			return false;
	};
	
	if (bGOnly)
	{
		// ������ݶ�
		*pTo = imgTemp;
	}
	else
	{
		// ����ָ����ֵ������ֵ��
		if (bThre)
		{
			imgTemp.Threshold(pTo, bThre);
		}
		else			// �Զ���ֵ��
		{
			imgTemp.AutoThreshold(pTo);
		};

		if (bThinning)
		{
			// ��һ�η�ɫ��Ϊ��Եϸ��׼��
			pTo->LinTran(&imgTemp, -1, 255);

			// ��Եϸ��
			imgTemp.Thining();

			// �ڶ��η�ɫ���õ����ս��
			imgTemp.LinTran(pTo, -1, 255);
		}
	}

	return true;
}

/**************************************************
BOOL CImgProcess::EdgeSobel(CImgProcess * pTo, BYTE bThre, BYTE bEdgeType, BOOL bThinning, BOOL bGOnly)

���ܣ�
	��ԵѰ�ң�ʹ��Sobel����

������
	CImgProcess * pTo
		ָ�����ͼ���ָ��
	BYTE bThre
		�϶���Ե����ֵ��Ĭ��Ϊ0�����Զ�ȷ����ֵ
	BYTE bEdgeType
		_EdgeAll-���б�Ե _EdgeH-ˮƽ��Ե _EdgeV-��ֱ��Ե _EdgeCW-45�ȱ�Ե _EdgeCCW-135�ȱ�Ե ����-��Ч
	BOOL bThinning
		�����Ƿ���б�Եϸ����Ĭ��Ϊtrue����ִ�б�Եϸ��
	BOOL bGOnly
		�����Ƿ������ݶ�ͼ��Ĭ��Ϊfalse���������ֵ����Ķ�ֵͼ��
		���˲���Ϊtrueʱ��bThre������bThinning������������

����ֵ��
	�������ͣ�trueΪ�ɹ���falseΪʧ��
***************************************************/
BOOL CImgProcess::EdgeSobel(CImgProcess * pTo, BYTE bThre, BYTE bEdgeType, BOOL bThinning, BOOL bGOnly)
{
	if (m_pBMIH->biBitCount!=8) return false;

	// ����ģ������
	// ˮƽ��Ե
	const float cfSobelH[9] = {
		-1, -1, -1,
		 0,  0,  0,
		 1,  1,  1 };
	// ��ֱ��Ե
	const float cfSobelV[9] = {
		-1,  0,  1,
		-1,  0,  1,
		-1,  0,  1 };
	// 45�ȱ�Ե
	const float cfSobelCW[9] = {
		-1, -1,  0,
		-1,  0,  1,
		 0,  1,  1 };
	// 135�ȱ�Ե
	const float cfSobelCCW[9] = {
		 0,  1,  1,
		-1,  0,  1,
		-1, -1,  0 };
	
	// ��ʱCImgProcess����
	CImgProcess imgTemp = *this;
	CImgProcess imgMid = *this;

	// ����ѡ��ı�Ե����Ӧ��ģ��
	switch (bEdgeType) {
		case 0:		// ���б�Ե
			Template(&imgTemp, 3, 3, 1, 1, (float*)cfSobelH, 1);
			Template(&imgMid, 3, 3, 1, 1, (float*)cfSobelV, 1);
			imgTemp = imgTemp + imgMid;
			Template(&imgMid, 3, 3, 1, 1, (float*)cfSobelCW, 1);
			imgTemp = imgTemp + imgMid;
			Template(&imgMid, 3, 3, 1, 1, (float*)cfSobelCCW, 1);
			imgTemp = imgTemp + imgMid;
			break;
		
		case 1:		// ˮƽ��Ե
			Template(&imgTemp, 3, 3, 1, 1, (float*)cfSobelH, 1);
			break;
		
		case 2:		// ��ֱ��Ե
			Template(&imgTemp, 3, 3, 1, 1, (float*)cfSobelV, 1);
			break;
		
		case 3:		// 45�ȱ�Ե
			Template(&imgTemp, 3, 3, 1, 1, (float*)cfSobelCW, 1);
			break;
		
		case 4:		// 135�ȱ�Ե
			Template(&imgTemp, 3, 3, 1, 1, (float*)cfSobelCCW, 1);
			break;
		
		default:			// ��������
			return false;
	};
	
	if (bGOnly)
	{
		// ������ݶ�
		*pTo = imgTemp;
	}
	else
	{
		// ����ָ����ֵ������ֵ��
		if (bThre)
		{
			imgTemp.Threshold(pTo, bThre);
		}
		else			// �Զ���ֵ��
		{
			imgTemp.AutoThreshold(pTo);
		};

		if (bThinning)
		{
			// ��һ�η�ɫ��Ϊ��Եϸ��׼��
			pTo->LinTran(&imgTemp, -1, 255);

			// ��Եϸ��
			imgTemp.Thining();
			
			// �ڶ��η�ɫ���õ����ս��
			imgTemp.LinTran(pTo, -1, 255);
		}
	}

	return true;
}

/**************************************************
BOOL CImgProcess::EdgePrewitt(CImgProcess * pTo, BYTE bThre, BYTE bEdgeType, BOOL bThinning, BOOL bGOnly)

���ܣ�
	��ԵѰ�ң�ʹ��Prewitt����

������
	CImgProcess * pTo
		ָ�����ͼ���ָ��
	BYTE bThre
		�϶���Ե����ֵ��Ĭ��Ϊ0�����Զ�ȷ����ֵ
	BYTE bEdgeType
		_EdgeAll-���б�Ե _EdgeH-ˮƽ��Ե _EdgeV-��ֱ��Ե _EdgeCW-45�ȱ�Ե _EdgeCCW-135�ȱ�Ե ����-��Ч
	BOOL bThinning
		�����Ƿ���б�Եϸ����Ĭ��Ϊtrue����ִ�б�Եϸ��
	BOOL bGOnly
		�����Ƿ������ݶ�ͼ��Ĭ��Ϊfalse���������ֵ����Ķ�ֵͼ��
		���˲���Ϊtrueʱ��bThre������bThinning������������

����ֵ��
	�������ͣ�trueΪ�ɹ���falseΪʧ��
***************************************************/

BOOL CImgProcess::EdgePrewitt(CImgProcess * pTo, BYTE bThre, BYTE bEdgeType, BOOL bThinning, BOOL bGOnly)
{
	if (m_pBMIH->biBitCount!=8) return false;

	// ����ģ������
	// ˮƽ��Ե
	const float cfPrewittH[9] = {
		-1, -2, -1,
		 0,  0,  0,
		 1,  2,  1 };
	// ��ֱ��Ե
	const float cfPrewittV[9] = {
		-1,  0,  1,
		-2,  0,  2,
		-1,  0,  1 };
	// 45�ȱ�Ե
	const float cfPrewittCW[9] = {
		-2, -1,  0,
		-1,  0,  1,
		 0,  1,  2 };
	// 135�ȱ�Ե
	const float cfPrewittCCW[9] = {
		 0,  1,  2,
		-1,  0,  1,
		-2, -1,  0 };

	// ��ʱCImgProcess����
	CImgProcess imgTemp = *this;
	CImgProcess imgMid = *this;

	// ����ѡ��ı�Ե����Ӧ��ģ��
	switch (bEdgeType) {
		case 0:		// ���б�Ե
			Template(&imgTemp, 3, 3, 1, 1, (float*)cfPrewittH, 1);
			Template(&imgMid, 3, 3, 1, 1, (float*)cfPrewittV, 1);
			imgTemp = imgTemp + imgMid;
			Template(&imgMid, 3, 3, 1, 1, (float*)cfPrewittCW, 1);
			imgTemp = imgTemp + imgMid;
			Template(&imgMid, 3, 3, 1, 1, (float*)cfPrewittCW, 1);
			imgTemp = imgTemp + imgMid;
			break;
		
		case 1:		// ˮƽ��Ե
			Template(&imgTemp, 3, 3, 1, 1, (float*)cfPrewittH, 1);
			break;
		
		case 2:		// ��ֱ��Ե
			Template(&imgTemp, 3, 3, 1, 1, (float*)cfPrewittV, 1);
			break;
		
		case 3:		// 45�ȱ�Ե
			Template(&imgTemp, 3, 3, 1, 1, (float*)cfPrewittCW, 1);
			break;
		
		case 4:		// 135�ȱ�Ե
			Template(&imgTemp, 3, 3, 1, 1, (float*)cfPrewittCCW, 1);
			break;
		
		default:			// ��������
			return false;
	};
	
	if (bGOnly)
	{
		// ������ݶ�
		*pTo = imgTemp;
	}
	else
	{
		// ����ָ����ֵ������ֵ��
		if (bThre)
		{
			imgTemp.Threshold(pTo, bThre);
		}
		else			// �Զ���ֵ��
		{
			imgTemp.AutoThreshold(pTo);
		};

		if (bThinning)
		{
			// ��һ�η�ɫ��Ϊ��Եϸ��׼��
			pTo->LinTran(&imgTemp, -1, 255);

			// ��Եϸ��
			imgTemp.Thining();

			// �ڶ��η�ɫ���õ����ս��
			imgTemp.LinTran(pTo, -1, 255);
		}
	}

	return true;
}

/**************************************************
void CImgProcess::EdgeLoG(CImgProcess * pTo)

���ܣ�
	����LoG���ӵı�Ե���

������
	CImgProcess * pTo
		ָ�����ͼ���ָ��

����ֵ��
	��
***************************************************/

void CImgProcess::EdgeLoG(CImgProcess * pTo)
{
	// Ӧ��ģ�嵽ͼ��
	Template(pTo, 5, 5, 2, 2, Template_Log, 1);

	
	// ��ʱCImgProcess����
	CImgProcess imgTemp = *pTo;
	
	// �Զ���ֵ��
	imgTemp.AutoThreshold(pTo);

	// ��һ�η�ɫ��Ϊ��Եϸ��׼��
	pTo->LinTran(&imgTemp, -1, 255);

	// ��Եϸ��
	imgTemp.Thining();
		
	// �ڶ��η�ɫ���õ����ս��
	imgTemp.LinTran(pTo, -1, 255);
}

// �ӻ����ת��
CImgProcess& CImgProcess::operator = (CImg& img)
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

/**************************************************
BOOL CImgProcess::EdgeCanny(CImgProcess * pTo, BYTE bThreL, BYTE bThreH, BOOL bThinning)

���ܣ�
	����Canny���ӵı�Ե���

������
	CImgProcess * pTo
		ָ�����ͼ���ָ��
	BYTE bThreL, BYTE bThreH
		�϶���Ե�ĵ���ֵ�͸���ֵ
		����һ����Ϊ0��ᱻ�Զ����ɣ����ɸ���ֵʱ���Զ����ǵ���ֵ
		Ĭ��ֵ��Ϊ0�����Զ����ɸߵ���ֵ
	BOOL bThinning
		�����Ƿ���б�Եϸ����Ĭ��Ϊtrue����ִ�б�Եϸ��

����ֵ��
	�������ͣ�trueΪ�ɹ���falseΪʧ��
***************************************************/

BOOL CImgProcess::EdgeCanny(CImgProcess * pTo, BYTE bThreL, BYTE bThreH, BOOL bThinning)
{
	int i, j;

	if (m_pBMIH->biBitCount!=8) return false;

	// �������ݶ�ֵ
	CImgProcess imgGH = *this, imgGV = *this, imgGCW = *this, imgGCCW = *this, imgGratitude = *this;

	// ʹ��Prewittģ�������������ϵ��ݶ�ֵ
	EdgePrewitt(&imgGH, 0, 1, 0, 1);
	EdgePrewitt(&imgGV, 0, 2, 0, 1);
	EdgePrewitt(&imgGCW, 0, 3, 0, 1);
	EdgePrewitt(&imgGCCW, 0, 4, 0, 1);

	// ����ݶȷ���
	BYTE * pbDirection = new BYTE [GetHeight() * GetWidthByte()];

	memset(pbDirection, 0, GetHeight() * GetWidthByte() * sizeof(BYTE));

	// Ѱ��ÿ�������ݶȷ���д���Ӧ������ݶ�ֵ
	imgGratitude.InitPixels(0);
	for (i=0; i<GetHeight(); i++)
	{
		for (j=0; j<GetWidthPixel(); j++)
		{
			BYTE gray = 0;

			if (imgGH.GetGray(j, i) > gray)
			{
				gray = imgGH.GetGray(j, i);
				pbDirection[i * GetWidthPixel() + j] = _EdgeH;
				imgGratitude.SetPixel(j, i, RGB(gray, gray, gray));
			}

			if (imgGV.GetGray(j, i) > gray)
			{
				gray = imgGV.GetGray(j, i);
				pbDirection[i * GetWidthPixel() + j] = _EdgeV;
				imgGratitude.SetPixel(j, i, RGB(gray, gray, gray));
			}

			if (imgGCW.GetGray(j, i) > gray)
			{
				gray = imgGCW.GetGray(j, i);
				pbDirection[i * GetWidthPixel() + j] = _EdgeCW;
				imgGratitude.SetPixel(j, i, RGB(gray, gray, gray));
			}

			if (imgGCCW.GetGray(j, i) > gray)
			{
				gray = imgGCCW.GetGray(j, i);
				pbDirection[i * GetWidthPixel() + j] = _EdgeCCW;
				imgGratitude.SetPixel(j, i, RGB(gray, gray, gray));
			}
		}
	}

	// ��ֵ��ʱ����ǰ��Ķ���
	CImgProcess *pImgThreL = &imgGH, *pImgThreH = &imgGV;

	// �����ֵ��������δ������ֵ�������ȡ�������ֵ
	if (bThreL > bThreH) return false;
	
	if (bThreH == 0) {
		const int nMinDiff = 20;
		int nDiffGray;

		bThreH = 1.2 * imgGratitude.DetectThreshold(100, nDiffGray);
		bThreL = 0.4 * bThreH;
		
		if(nDiffGray < nMinDiff) return false;
	}
	
	if (bThreL == 0) {
		bThreL = 0.4 * bThreH;
	}

	// ������ݶ�ͼ�񰴸ߵ�ֵ�ֱ������ֵ��
	imgGratitude.Threshold(pImgThreL, bThreL);
	imgGratitude.Threshold(pImgThreH, bThreH);

	// ��ʼ��Ŀ��ͼ��
	pTo->InitPixels(0);
	
	// ���ݵ���ֵͼ���ڸ���ֵͼ���Ͻ��б߽��޲�
	for (i=1; i<GetHeight()-1; i++)
	{
		for (j=1; j<GetWidthPixel()-1; j++)
		{
			if (pImgThreH->GetGray(j, i))
			{
				// ����ֵͼ���Ϸ��ֵ�ֱ��ȷ��
				pTo->SetPixel(j, i, RGB(255, 255, 255));
				
				// �����ݶ�������ϵ�����
				switch ( pbDirection[i * GetWidthPixel() + j] ) {
					case 1:	// ˮƽ����
						if (pImgThreL->GetGray(j+1, i))
						{
							pImgThreH->SetPixel(j+1, i, RGB(255, 255, 255));
						}
						if (pImgThreL->GetGray(j-1, i))
						{
							pImgThreH->SetPixel(j-1, i, RGB(255, 255, 255));
						}
						break;

					case 2:	// ��ֱ����
						if (pImgThreL->GetGray(j, i+1))
						{
							pImgThreH->SetPixel(j, i+1, RGB(255, 255, 255));
						}
						if (pImgThreL->GetGray(j, i-1))
						{
							pImgThreH->SetPixel(j, i-1, RGB(255, 255, 255));
						}
						break;

					case 3:	// 45�ȷ���
						if (pImgThreL->GetGray(j+1, i-1))
						{
							pImgThreH->SetPixel(j+1, i-1, RGB(255, 255, 255));
						}
						if (pImgThreL->GetGray(j-1, i+1))
						{
							pImgThreH->SetPixel(j-1, i+1, RGB(255, 255, 255));
						}
						break;

					case 4:	// 135�ȷ���
						if (pImgThreL->GetGray(j+1, i+1))
						{
							pImgThreH->SetPixel(j+1, i+1, RGB(255, 255, 255));
						}
						if (pImgThreL->GetGray(j-1, i-1))
						{
							pImgThreH->SetPixel(j-1, i-1, RGB(255, 255, 255));
						}
						break;
				}
			}//if
		}//for j
	}//for i

	if (bThinning)
	{
		// ��һ�η�ɫ��Ϊ��Եϸ��׼��
		pImgThreH->LinTran(pImgThreL, -1, 255);

		// ��Եϸ��
		pImgThreL->Thining();
		
		// �ڶ��η�ɫ���õ����ս��
		pImgThreL->LinTran(pTo, -1, 255);
	}
	else
	{
		*pTo = *pImgThreH;
	}

	delete pbDirection;

	return true;
}

/**************************************************
BOOL CImgProcess::RegionGrow(CImgProcess * pTo , int nSeedX, int nSeedY, BYTE bThre)

���ܣ�
	���������㷨

������
	CImgProcess * pTo
		ָ�����ͼ���ָ��
	int nSeedX, int nSeedY
		���ӵ������ֵ
	BYTE bThre
		����ʱʹ�õ���ֵ
����ֵ��
	�������ͣ�trueΪ�ɹ���falseΪʧ��
***************************************************/

BOOL CImgProcess::RegionGrow(CImgProcess * pTo , int nSeedX, int nSeedY, BYTE bThre)
{
	if (m_pBMIH->biBitCount!=8) return false;

	if ((nSeedX<0)||(nSeedX>GetWidthPixel())) return false;
	if ((nSeedY<0)||(nSeedY>GetHeight())) return false;

	pTo->InitPixels(0);
	pTo->SetPixel(nSeedX, nSeedY, RGB(255, 255, 255));

	// ������ʼ��Ҷ�
	BYTE bSeed = GetGray(nSeedX, nSeedY);
	// ��������Ҷ�ֵ֮��
	long int lSum = bSeed;
	// ��������ĵ�������ÿ�ΰ������з������������
	int nSuit = 1, nCount = 1;
	
	// ��ʼ��������ѭ������
	while (nCount > 0)
	{
		nCount = 0;

		for (int i=1; i<GetHeight()-1; i++)
		{	// ����
			for (int j=1; j<GetWidthPixel()-1; j++)
			{	// ����
				if (pTo->GetGray(j, i)==255)
				{	// �����ӵ�
					// ��ʼ8����ɨ��
					for (int m=i-1; m<=i+1; m++)
					{
						for (int n=j-1; n<=j+1; n++)
						{
							// �ж��Ƿ������ֵ������δ���
							if ((pTo->GetGray(n, m)==0)&&(abs(GetGray(n, m)-bSeed)<=bThre))
							{
								pTo->SetPixel(n, m, RGB(255, 255, 255));
								nCount++;
								lSum += GetGray(n, m);
							}
						}
					}
				}
			}
		}

		nSuit += nCount;

		// ����������ֵ������ʹ�øĽ�������ֵ�㷨Ϊ�ѱ�������ƽ���Ҷȣ�
		bSeed = lSum / nSuit;
	}

	return true;
}




/**************************************************
BOOL CImgProcess::GenHist(double * pdHist, int n)

���ܣ�
	����ͼ��ĻҶ�ֱ��ͼ

������
	double * pdHist
		����ĻҶ�ֱ��ͼ����
	BYTE n
		�Ҷ�ֱ��ͼ�ĻҶȼ�����������
����ֵ��
	BOOL���ͣ�trueΪ�ɹ���falseΪʧ��
***************************************************/

BOOL CImgProcess::GenHist(double * pdHist, int n)
{
	// ���ȼ��ͼ�������
	if (m_pBMIH->biBitCount!=8) return false;
	
	// ���n��Χ
	if ((n<=0)||(n>256)) return false;

	// ����ֶ�����
	double dDivider;

	memset(pdHist, 0, n * sizeof(double));
	dDivider = 256.0 / (double)n;
	
	BYTE bGray;	// ��ʱ����,�洢��ǰ������صĻҶ�ֵ
	for (int i=0; i<m_pBMIH->biHeight; i++)
	{
		for (int j=0; j<m_pBMIH->biWidth; j++)
		{
			bGray = GetGray(j, i);
			pdHist[(int)(bGray / dDivider)]++;	// ָ���ĻҶ������Լ�
		}
	};

	UINT square = m_pBMIH->biWidth * m_pBMIH->biHeight;

	for (int k=0; k<n; k++) 
	{
		pdHist[k]=pdHist[k]/square;
	}

	return true;
}

/**************************************************
BOOL CImgProcess::ParLinTran(CImgProcess * pTo, BYTE x1, BYTE x2, BYTE y1, BYTE y2)

���ܣ�
	ͼ��ĻҶȷֶ����Ա任
���ƣ�
	x1 < x2

������
	CImgProcess * pTo
		���CImgProcess�����ָ��
	BYTE x1
		�ֶ����Ա任��һ��ĺ�����
	BYTE x2
		�ֶ����Ա任�ڶ���ĺ�����
	BYTE y1
		�ֶ����Ա任��һ���������
	BYTE y2
		�ֶ����Ա任�ڶ����������
		
����ֵ��
	BOOL���ͣ�trueΪ�ɹ���falseΪʧ��
***************************************************/
BOOL CImgProcess::ParLinTran(CImgProcess * pTo, BYTE x1, BYTE x2, BYTE y1, BYTE y2)
{
	// ���ȼ��ͼ�������
	if (m_pBMIH->biBitCount!=8) return false;

	// ��������Χ
	if (x1>x2) return false;			// ������ϵ����,���ش���
	
	BYTE gray;		// ��ʱ����,�洢��ǰ������صĻҶ�ֵ
	int target;		// ��ʱ����,�洢��ǰ������ص�Ŀ��ֵ
	
	for (int i=0; i<m_pBMIH->biHeight; i++)
	{
		for (int j=0; j<m_pBMIH->biWidth; j++)
		{
			gray = GetGray(j, i);

			// ����ʽ����
			if (gray<=x1)
			{
				target = y1 * gray / x1;
			}
			else if (gray<=x2)
			{
				target = (y2-y1)*(gray-x1)/(x2-x1) + y1;
			}
			else
			{
				target = (255-y2)*(gray-x2)/(255-x2) + y2;
			};
			
			if (target < 0) target = 0;
			if (target > 255) target = 255;

			// д��Ŀ��ͼ��
			pTo->SetPixel(j, i, RGB(target, target, target));
		}
	}

	return true;
}

/**************************************************
BOOL CImgProcess::LogTran(CImgProcess* pTo, double dC)

���ܣ�
	ͼ��ĻҶȶ����任

������
	CImgProcess * pTo
		���CImgProcess�����ָ��
	double dC
		�Ҷȶ����任����Ĳ���
����ֵ��
	BOOL���ͣ�0Ϊ�ɹ�������ֵΪʧ��
***************************************************/
BOOL CImgProcess::LogTran(CImgProcess* pTo, double dC)
{
	// ���ȼ��ͼ���Ƿ���8λ�Ҷ�ͼ��
	if (m_pBMIH->biBitCount!=8) return false;
	
	BYTE gray;		// ��ʱ����,�洢��ǰ������صĻҶ�ֵ
	int target;		// ��ʱ����,�洢��ǰ������ص�Ŀ��ֵ
	
	for (int i=0; i<m_pBMIH->biHeight; i++)
	{
		for (int j=0; j<m_pBMIH->biWidth; j++)
		{
			gray = GetGray(j, i);

			// ����ʽ����
			target = dC * log( (double)(gray + 1) );
			
			if (target < 0) target = 0;
			if (target > 255) target = 255;

			// д��Ŀ��ͼ��
			pTo->SetPixel(j, i, RGB(target, target, target));
		}
	};

	return 0;
}

/**************************************************
BOOL CImgProcess::GammaTran(CImgProcess* pTo, double gamma, double comp)

���ܣ�
	ͼ���٤��任����

������
	CImgProcess * pTo
		���CImgProcess�����ָ��
	double gamma
		٤��ϵ��
	double comp
		����ϵ��,Ĭ��Ϊ0
	
����ֵ��
	BOOL���ͣ�trueΪ�ɹ���falseΪʧ��
***************************************************/

BOOL CImgProcess::GammaTran(CImgProcess* pTo, double gamma, double comp)
{
	// ���ȼ��ͼ���Ƿ���8λ�Ҷ�ͼ��
	if (m_pBMIH->biBitCount!=8) return false;
	
	BYTE gray;		// ��ʱ����,�洢��ǰ������صĻҶ�ֵ
	int target;		// ��ʱ����,�洢��ǰ������ص�Ŀ��ֵ
	
	for (int i=0; i<m_pBMIH->biHeight; i++)
	{
		for (int j=0; j<m_pBMIH->biWidth; j++)
		{
			gray = GetGray(j, i);

			target = pow( (gray+comp)/255.0, gamma ) * 255;
			
			if (target < 0) target = 0;
			if (target > 255) target = 255;

			// д��Ŀ��ͼ��
			pTo->SetPixel(j, i, RGB(target, target, target));
		}
	};

	return 0;
}

/**************************************************
BOOL CImgProcess::WindowTran(CImgProcess* pTo, BYTE lowThre, BYTE highThre)

���ܣ�
	ͼ��Ĵ��ڱ任����

������
	CImgProcess * pTo
		���CImgProcess�����ָ��
	BYTE lowThre
		��������
	BYTE highThre
		��������
	
����ֵ��
	BOOL���ͣ�trueΪ�ɹ���falseΪʧ��
***************************************************/

BOOL CImgProcess::WindowTran(CImgProcess* pTo, BYTE lowThre, BYTE highThre)
{
	// ���ȼ��ͼ���Ƿ���8λ�Ҷ�ͼ��
	if (m_pBMIH->biBitCount!=8) return false;
	
	BYTE gray;		// ��ʱ����,�洢��ǰ������صĻҶ�ֵ
	int target;		// ��ʱ����,�洢��ǰ������ص�Ŀ��ֵ
	
	for (int i=0; i<m_pBMIH->biHeight; i++)
	{
		for (int j=0; j<m_pBMIH->biWidth; j++)
		{
			gray = GetGray(j, i);

			if (gray < lowThre)
			{
				target = 0;
			}
			else if (gray > highThre)
			{
				target = 255;
			}
			else
			{
				target = gray;
			};
			
			if (target < 0) target = 0;
			if (target > 255) target = 255;

			// д��Ŀ��ͼ��
			pTo->SetPixel(j, i, RGB(target, target, target));
		}
	};

	return 0;
}

/**************************************************
BOOL CImgProcess::LinTran(CImgProcess* pTo, double dFa, double dFb)

���ܣ�
	ͼ������Ա任����

������
	CImgProcess * pTo
		���CImgProcess�����ָ��
	double dFa
		���Ա任б��
	double dFb
		���Ա任�ؾ�
	
����ֵ��
	BOOL���ͣ�trueΪ�ɹ���falseΪʧ��
***************************************************/

BOOL CImgProcess::LinTran(CImgProcess* pTo, double dFa, double dFb)
{
	// ���ȼ��ͼ���Ƿ���8λ�Ҷ�ͼ��
	if (m_pBMIH->biBitCount!=8) return false;
	
	BYTE gray;		// ��ʱ����,�洢��ǰ������صĻҶ�ֵ
	int target;		// ��ʱ����,�洢��ǰ������ص�Ŀ��ֵ
	
	for (int i=0; i<m_pBMIH->biHeight; i++)
	{
		for (int j=0; j<m_pBMIH->biWidth; j++)
		{
			gray = GetGray(j, i);

			target = dFa * gray + dFb;
			
			if (target < 0) target = 0;
			if (target > 255) target = 255;

			// д��Ŀ��ͼ��
			pTo->SetPixel(j, i, RGB(target, target, target));
		}
	};

	return true;
}

/**************************************************
BOOL CImgProcess::Histeq(CImgProcess * pTo)

���ܣ�
	ͼ��ĻҶ�ֱ��ͼ���⻯����

������
	CImgProcess * pTo
		���CImgProcess�����ָ��
	
����ֵ��
	BOOL���ͣ�trueΪ�ɹ���falseΪʧ��
***************************************************/

BOOL CImgProcess::Histeq(CImgProcess * pTo)
{
	// ���ȼ��ͼ���Ƿ���8λ�Ҷ�ͼ��
	if (m_pBMIH->biBitCount!=8) return false;
	
	BYTE gray;			// ��ʱ����,�洢��ǰ������صĻҶ�ֵ
	int target;			// ��ʱ����,�洢��ǰ������ص�Ŀ��ֵ

	double pdHist[256];	//��ʱ����,�洢�Ҷ�ֱ��ͼ
	
	double dTemp;		// ��ʱ����,�洢�ۼӵ�ֱ��ͼ����

	this->GenHist(pdHist);

	for (int i=0; i<m_pBMIH->biHeight; i++)
	{
		for (int j=0; j<m_pBMIH->biWidth; j++)
		{
			dTemp = 0;

			gray = GetGray(j, i);

			for (BYTE k=0; k<gray; k++)
			{
				dTemp+=*(pdHist + k);
			};

			target = 255 * dTemp;
			
			if (target < 0) target = 0;
			if (target > 255) target = 255;

			// д��Ŀ��ͼ��
			pTo->SetPixel(j, i, RGB(target, target, target));
		}
	};

	return true;
}

/**************************************************
BOOL CImgProcess::Histst(CImgProcess* pTo, double* pdStdHist)

���ܣ�
	ͼ��ĻҶ�ֱ��ͼ�涨������

������
	CImgProcess * pTo
		���CImgProcess�����ָ��
	double * pdStdHist
		��׼ֱ��ͼ���飨Ҫ���Ѿ���һ����ֱ��ͼ��
	
����ֵ��
	BOOL���ͣ�trueΪ�ɹ���falseΪʧ��
***************************************************/

BOOL CImgProcess::Histst(CImgProcess* pTo, double* pdStdHist)
{
	int i,j; 

	// ���ȼ��ͼ���Ƿ���8λ�Ҷ�ͼ��
	if (m_pBMIH->biBitCount!=8) return false;
	
	BYTE gray;			// ��ʱ����,�洢��ǰ������صĻҶ�ֵ
	int target;			// ��ʱ����,�洢��ǰ������ص�Ŀ��ֵ
	
	double pdHist[256];	// ��ʱ����,�洢�Ҷ�ֱ��ͼ
	this->GenHist(pdHist);

	double dTemp;		// ��ʱ����,�洢�ۼӵ�ֱ��ͼ����
	int pdTran[256];	// ��ʱ����,�洢��׼ֱ��ͼ���⻯�ı任����
	memset(pdTran, -1, sizeof(int)*256);
	
	// ���׼ֱ��ͼ�ľ��⻯�任����
	for (i=0; i<256; i++)
	{
		dTemp = 0;

		for (BYTE k=0; k<i; k++)
		{
			dTemp+=*(pdStdHist + k);
		}

		*(pdTran + (int)(0.5+255 * dTemp)) = i;
	}

	// ȥ�����⻯�任�����еļ�ϵ㡪����ֵ
	{
		i=0, j=0;
		while(i<255)
		{
			if(*(pdTran + i + 1)!=-1)
			{
				i++;
				continue;
			}
			j = 1;
			while((*(pdTran + i + j)==-1)&&((i + j)<=255))
			{
				*(pdTran + i + j)=*(pdTran + i);
				j++;
			}
		}
	}

	// ��ԭͼ�����Ƚ��лҶȾ��⻯���ٽ��й涨��
	for (i=0; i<m_pBMIH->biHeight; i++)
	{
		for (j=0; j<m_pBMIH->biWidth; j++)
		{
			dTemp = 0;

			gray = GetGray(j, i);

			for (BYTE k=0; k<gray; k++)
			{
				dTemp+=*(pdHist + k);
			};

			target = *(pdTran + (int)(255 * dTemp));
			
			if (target < 0) target = 0;
			if (target > 255) target = 255;

			// д��Ŀ��ͼ��
			pTo->SetPixel(j, i, RGB(target, target, target));
		}
	};

	return true;
}

/**************************************************
BOOL CImgProcess::Histst(CImgProcess* pTo, CImgProcess* pStd)

���ܣ�
	ͼ��ĻҶ�ֱ��ͼ�涨������

������
	CImgProcess * pTo
		���CImgProcess�����ָ��
	CImgProcess* pStd
		��׼Ŀ��ͼ��
	
����ֵ��
	BOOL���ͣ�trueΪ�ɹ���falseΪʧ��
***************************************************/

BOOL CImgProcess::Histst(CImgProcess* pTo, CImgProcess* pStd)
{
	// ��׼ͼ��ֱ��ͼ
	double pdStdHist[256];
	
	pStd->GenHist(pdStdHist);

	return Histst(pTo, pdStdHist);
}

// FFT��IFFT

/**************************************************
void CImgProcess::FFT(complex<double> * TD, complex<double> * FD, int r)

���ܣ�
	һά���ٸ���Ҷ�任

������
	complex<double> * TD
		ָ��ʱ�������ָ��
	complex<double> * FD
		ָ��Ƶ�������ָ��
	r
		2������������������
����ֵ��
	��
***************************************************/
void CImgProcess::FFT(complex<double> * TD, complex<double> * FD, int r)
{
	// ����Ҷ�任����
	LONG	count;
	
	// ѭ������
	int		i,j,k;
	
	// �м����
	int		bfsize,p;
	
	// �Ƕ�
	double	angle;
	
	complex<double> *W,*X1,*X2,*X;
	
	// ���㸵��Ҷ�任����
	count = 1 << r;
	
	// ������������洢��
	W  = new complex<double>[count / 2];
	X1 = new complex<double>[count];
	X2 = new complex<double>[count];
	
	// �����Ȩϵ��
	for(i = 0; i < count / 2; i++)
	{
		angle = -i * PI * 2 / count;
		W[i] = complex<double> (cos(angle), sin(angle));
	}
	
	// ��ʱ���д��X1
	memcpy(X1, TD, sizeof(complex<double>) * count);
	
	// ���õ����㷨���п��ٸ���Ҷ�任
	for(k = 0; k < r; k++)
	{
		for(j = 0; j < 1 << k; j++)
		{
			bfsize = 1 << (r-k);
			for(i = 0; i < bfsize / 2; i++)
			{
				p = j * bfsize;
				X2[i + p] = X1[i + p] + X1[i + p + bfsize / 2];
				X2[i + p + bfsize / 2] = (X1[i + p] - X1[i + p + bfsize / 2]) * W[i * (1<<k)];
			}
		}
		X  = X1;
		X1 = X2;
		X2 = X;
	}
	
	// ��������
	for(j = 0; j < count; j++)
	{
		p = 0;
		for(i = 0; i < r; i++)
		{
			if (j&(1<<i))
			{
				p+=1<<(r-i-1);
			}
		}
		FD[j]=X1[p];
	}
	
	// �ͷ��ڴ�
	delete W;
	delete X1;
	delete X2;
}

/**************************************************
void CImgProcess::IFFT(complex<double> * FD, complex<double> * TD, int r)

���ܣ�
	һά���ٸ���Ҷ���任

������
	complex<double> * FD
		ָ��Ƶ�������ָ��
	complex<double> * TD
		ָ��ʱ�������ָ��
	r
		2������������������
����ֵ��
	��
***************************************************/
void CImgProcess::IFFT(complex<double> * FD, complex<double> * TD, int r)
{
	// ����Ҷ�任����
	LONG	count;
	
	// ѭ������
	int		i;
	
	complex<double> *X;
	
	// ���㸵��Ҷ�任����
	count = 1 << r;
	
	// ������������洢��
	X = new complex<double>[count];
	
	// ��Ƶ���д��X
	memcpy(X, FD, sizeof(complex<double>) * count);
	
	// ����
	for(i = 0; i < count; i++)
	{
		X[i] = complex<double> (X[i].real(), -X[i].imag());
	}
	
	// ���ÿ��ٸ���Ҷ�任
	FFT(X, TD, r);
	
	// ��ʱ���Ĺ���
	for(i = 0; i < count; i++)
	{
		TD[i] = complex<double> (TD[i].real() / count, -TD[i].imag() / count);
	}
	
	// �ͷ��ڴ�
	delete X;
}










////////////////////////////********��4�� ���α任***********///////////////////////////////
/******************* 
void CImgProcess::ImMove(CImgProcess* pTo, int x, int y) 
 ���ܣ�	ƽ��ͼ��
 ע��		ͼ��Χ����
 ������	CImgProcess* pTo�������õ���ͼ��� CImgProcess ָ�� 
		int x��ˮƽ���ƾ���
 		int y����ֱ���ƾ��� 
 ����ֵ�� �� 
*******************/
void CImgProcess::ImMove(CImgProcess* pTo, int x, int y)
{
	int nHeight = pTo->GetHeight();
	int nWidth = pTo->GetWidthPixel();

	int i, j;

	if(x>nWidth || y>nHeight)
	{
		MessageBox(NULL,"����ͼƬ��С","����",MB_OK|MB_ICONERROR);
		return;
	}
  

	for(i=0;i<nWidth;i++)
	{
		for(j=0;j<nHeight;j++)
		{
			if(i-x>0 && i-x<nWidth && j-y>0 &&j-y<nHeight)
				pTo->SetPixel(i,j,GetPixel(i-x,j-y));
			else
				pTo->SetPixel(i,j,RGB(255, 255, 255));
		}//for j
	}//for i

}

/******************* 
void CImgProcess::HorMirror(CImgProcess* pTo)
 ���ܣ�	ͼ���ˮƽ����
 ע��		ͼ�����Ҿ���
 ������	CImgProcess* pTo�������õ���ͼ��� CImgProcess ָ��  
 ����ֵ��	�� 
*******************/
void CImgProcess::HorMirror(CImgProcess* pTo)
{
	int nHeight = pTo->GetHeight();
	int nWidth = pTo->GetWidthPixel();

	int i, j;
    int u;
	for(i=0;i<nWidth;i++)
	{
		u=nWidth-i-1;
		for(j=0;j<nHeight;j++)
		{
            
			pTo->SetPixel(i,j,GetPixel(u,j));
		}//for j
	}//for i
}

/******************* 
void CImgProcess::VerMirror(CImgProcess* pTo)
 ���ܣ�	ͼ�����ֱ����
 ע��		ͼ�����¾���
 ������	CImgProcess* pTo�������õ���ͼ��� CImgProcess ָ�� 
 ����ֵ�� �� 
*******************/
void CImgProcess::VerMirror(CImgProcess* pTo)
{
	int nHeight = pTo->GetHeight();
	int nWidth = pTo->GetWidthPixel();
    
	int i, j;
    int u=0;
	for(i=0;i<nWidth;i++)
	{
		
		for(j=0;j<nHeight;j++)
		{
            u=nHeight-j-1;
			pTo->SetPixel(i,j,GetPixel(i,u));
		}//for j
	}//for i
}

/******************* 
void CImgProcess::Transpose(CImgProcess * pTo)
 ���ܣ�	ͼ���ת��
 ע��		ͼ��ˮƽ��ֱ���򻥻���ͼ���С����
 ������	CImgProcess * pTo�������õ���ͼ���CImgProcessָ��
 ����ֵ��	�� 
*******************/
void CImgProcess::Transpose(CImgProcess* pTo)
{
	int nHeight = pTo->GetHeight();
	int nWidth = pTo->GetWidthPixel();
    
	int i, j;
  	for(i=0;i<nWidth;i++)
	{
		
		for(j=0;j<nHeight;j++)
		{
            if(j<nWidth && i<nHeight)
			    pTo->SetPixel(i,j,GetPixel(j,i));
			else
				pTo->SetPixel(i,j,RGB(255,255,255));
		}//for j
	}//for i
}

/******************* 
void CImgProcess::Scale(CImgProcess * pTo,double times)
 ���ܣ�	ͼ��ĵȱ�������
 ע��		����������С��ͼ���С����
 ������	CImgProcess * pTo�������õ���ͼ���CImgProcessָ�� 
		double times����������
 ����ֵ��	�� 
*******************/
void CImgProcess::Scale(CImgProcess* pTo,double times)
{
	int nHeight = pTo->GetHeight();
	int nWidth = pTo->GetWidthPixel();
    
	int i, j;
    
	for(i=0;i<nWidth;i++)
	{
		
		for(j=0;j<nHeight;j++)
		{
            if(int(i*1/times+0.5)<nWidth && int(j*1/times+0.5)<nHeight)
			    pTo->SetPixel(i,j,GetPixel(int(i*1/times+0.5),int(j*1/times+0.5)));
			else
				pTo->SetPixel(i,j,RGB(255,255,255));
		}//for j
	}//for i
}


/******************* 
void CImgProcess::Rotate(CImgProcess * pTo,float ang)
 ���ܣ�	��ԭ��Ϊ���ĵ�ͼ����ת
 ע��		Χ�����϶���˳ʱ����ת��ͼ��Χ����
 ������	CImgProcess * pTo�������õ���ͼ���CImgProcessָ��
 		float ang��˳ʱ����ת�Ƕȣ���λ�ȣ�Ҫ��ang>=0 && ang<=360 
 ����ֵ�� �� 
*******************/
void CImgProcess::Rotate(CImgProcess* pTo,float ang)
{
	int nHeight = pTo->GetHeight();
	int nWidth = pTo->GetWidthPixel();
    
	int i, j; //Ŀ��ͼ������
    int u,v; //Դͼ������

	for(i=0;i<nWidth;i++)
	{
		
		for(j=0;j<nHeight;j++)
		{
            u=int(i*cos(ang*PI/180)+j*sin(ang*PI/180)+0.5);
			v=int(j*cos(ang*PI/180)-i*sin(ang*PI/180)+0.5);
            if(u<nWidth && v<nHeight && u>=0 && v>=0)
			    pTo->SetPixel(i,j,GetPixel(u,v));
			else
				pTo->SetPixel(i,j,RGB(0,0,0));
		}//for j
	}//for i
}


/*******************
int CImgProcess::InterpBilinear(double x, double y)
���ܣ�
	˫���Բ�ֵ

������
	double x����Ҫ�����ֵ�ĺ�����
	double y����Ҫ�����ֵ��������
����ֵ:
	int ��ֵ�Ľ��
*******************/
int CImgProcess::InterpBilinear(double x, double y)
{
	if(int(y)==300)
		int cc = 1;
	
	// �ĸ����ٽ����ص�����(i1, j1), (i2, j1), (i1, j2), (i2, j2)
	int	x1, x2;
	int	y1, y2;
	
	// �ĸ����ٽ�����ֵ
	unsigned char	f1, f2, f3, f4;
	
	// ������ֵ�м�ֵ
	unsigned char	f12, f34;
	
	double	epsilon = 0.0001;
	
	// �����ĸ����ٽ����ص�����
	x1 = (int) x;
	x2 = x1 + 1;
	y1 = (int) y;
	y2 = y1 + 1;
	

	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();
	if( (x < 0) || (x > nWidth - 1) || (y < 0) || (y > nHeight - 1))
	{
		// Ҫ����ĵ㲻��Դͼ��Χ�ڣ�����-1
		return -1;
	}
	else
	{
		if (fabs(x - nWidth + 1) <= epsilon)
		{
			// Ҫ����ĵ���ͼ���ұ�Ե��
			if (fabs(y - nHeight + 1) <= epsilon)
			{
				// Ҫ����ĵ�������ͼ�������½���һ�����أ�ֱ�ӷ��ظõ�����ֵ
				f1 = (unsigned char)GetGray( x1, y1 );
				return f1;
			}
			else
			{
				// ��ͼ���ұ�Ե���Ҳ������һ�㣬ֱ��һ�β�ֵ����
				f1 = (unsigned char)GetGray(x1, y1 );
				f3 = (unsigned char)GetGray( x1, y2 );
								
				// ���ز�ֵ���
				return ((int) (f1 + (y -y1) * (f3 - f1)));
			}
		}
		else if (fabs(y - nHeight + 1) <= epsilon)
		{
			// Ҫ����ĵ���ͼ���±�Ե���Ҳ������һ�㣬ֱ��һ�β�ֵ����
			f1 = (unsigned char)GetGray( x1, y1 );
			f2 = (unsigned char)GetGray( x2, y1 );
			
			// ���ز�ֵ���
			return ((int) (f1 + (x -x1) * (f2 - f1)));
		}
		else
		{
			// �����ĸ����ٽ�����ֵ
			f1 = (unsigned char)GetGray( x1, y1 );
			f2 = (unsigned char)GetGray( x2, y1 );
			f3 = (unsigned char)GetGray( x1, y2 );
			f4 = (unsigned char)GetGray( x2, y2 );
			
			// ��ֵ1
			f12 = (unsigned char) (f1 + (x - x1) * (f2 - f1));
			
			// ��ֵ2
			f34 = (unsigned char) (f3 + (x - x1) * (f4 - f3));
			
			// ��ֵ3
			return ((int) (f12 + (y -y1) * (f34 - f12)));
		}
	}
}
















/////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////��7�� ��ɫͼ����///////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/******************* 
void CImgProcess::RGB2HSI(CImgProcess* pTo)
 
 ���ܣ�
	��һ��RGBͼ��ת��ΪHSIͼ��
 ע��
	��

 ������
	CImgProcess* pTo: Ŀ�����ͼ��� CImgProcess ָ��
	 
 ����ֵ��
	��
 *******************/
void CImgProcess::RGB2HSI(CImgProcess* pTo)
{

	int nHeight = GetHeight();//��ȡͼƬ�߶�
	int nWidth = GetWidthPixel();//��ȡͼƬ���

	int i, j;
	
	for(i=0; i<nHeight; i++)
	{
		for(j=0; j<nWidth; j++)
		{
			COLORREF RGBPixel = GetPixel(j, i);

			//��ȡRGB����
			double R = GetRValue(RGBPixel)/255.0;
			double G = GetGValue(RGBPixel)/255.0;
			double B = GetBValue(RGBPixel)/255.0;

			//RGB2HSI���㷨ת��
			double maxRGB = max(max(R,G),B);
			double minRGB = min(min(R,G),B);
			double H;
			double S;
			double I;
			double Temp1,Temp2,Radians,Angle;

			I = (R+G+B)/3;

		    if(I<0.078431)
			    S=0;
		    else if(I>0.920000)
			    S=0;
		    else
			    S=1.0-(3.0*minRGB)/(R+G+B);

			if(maxRGB==minRGB)
			{
				H = 0;
				S = 0;
			}

			Temp1 = ((R-G)+(R-B))/2;
			Temp2 = (R-G)*(R-G)+(R-B)*(G-B);
			double Q = Temp1/sqrt(Temp2);
			if(Q>0.9999999999)
				Radians = 0;
			else if(Q<-0.9999999999)
				Radians = PI;
			else
				Radians = acos(Q);
			Angle = Radians*180.0/PI;
			if(B>G)
				H = (360.0-Angle);
			else
				H = Angle;

			I = 255*I;
			S = 255*S;

			//�����������γ�HSIͼ��
			pTo->SetPixel(j, i, RGB(H, S, I));
		}//for j
	}//for i
}



/******************* 
void CImgProcess::HSI2RGB(CImgProcess *pTo)
 
 ���ܣ�	��һ��HSIͼ��ת��ΪRGBͼ��
 
 ������	CImgProcess* pTo: Ŀ�����ͼ��� CImgProcess ָ��
	 
 ����ֵ��	�� 
*******************/
void CImgProcess::HSI2RGB(CImgProcess *pTo)
{
	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	int i, j;
	
	for(i=0; i<nHeight; i++)
	{
		for(j=0; j<nWidth; j++)
		{
			COLORREF RGBPixel = GetPixel(j, i);
			//��ȡHSI����
			double H = GetRValue(RGBPixel);
			double S = GetGValue(RGBPixel)/255.0;
			double I = GetBValue(RGBPixel)/255.0;
			double R,G,B;

            //HSI2RGB���㷨ת��
			if(H>=0 && H<120)
			{
				H = H;
				B = I*(1.0-S);
				R = I*(1.0+((S*cos(H))/cos(60-H)));
				G = 3.0*I-R-B;
			}
			else if(H>=120 && H<240)
			{
				H = H-120;
				R = I*(1.0-S);
				G = I*(1.0+((S*cos(H))/cos(60-H)));
				B = 3.0*I-R-G;
			}
			else
			{
				H = H-240;
				G = I*(1.0-S);
				B = I*(1.0+((S*cos(H))/cos(60-H)));
				R = 3.0*I-B-G;				
			}
			R *=255.0;
			G *=255.0;
			B *=255.0;

			//�����������γ�RGBͼ��
			pTo->SetPixel(j, i, RGB(R, G, B));
		}
	}

}



/******************* 
void CImgProcess::Gray2RGB(CImgProcess *pTo)
 
 ���ܣ�
	��һ���Ҷ�ͼתα��ɫͼ

 ע��
	�Ҷ�ͼ�У�R��G��B��������ָ���

 ������
	CImgProcess* pTo: Ŀ�����ͼ��� CImgProcess ָ��
	 
 ����ֵ��
	��
	
*******************/
void CImgProcess::Gray2RGB(CImgProcess *pTo)
{
	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	int i, j;
	
	for(i=0; i<nHeight; i++)
	{
		for(j=0; j<nWidth; j++)
		{
			COLORREF RGBPixel = GetPixel(j, i);
			//��ȡRGB����
			double R = GetRValue(RGBPixel);
			double G = GetGValue(RGBPixel);
			double B = GetBValue(RGBPixel);

            //�Ҷ�ֵ��0��20֮�����ɫ
			if(B>=0 && B<20)
			{
				R = 0;
				G = 0;
				B = B;
			}
            //�Ҷ�ֵ��20��40֮�����ɫ
			if(G>=20 && G<40)
			{
				R = 0;
				G = G;
				B = 0;
			}
            //�Ҷ�ֵ��40��255�Ӻ�ɫ
			if(R>=40 && R<=255)
			{
				R = R;
				G = 0;
				B = 0;
			}

			//�����������γ�RGBͼ��
			pTo->SetPixel(j, i, RGB(R, G, B));
		}
	}
}



/******************* 
void CImgProcess::RGB2CMY(CImgProcess *pTo)
 ���ܣ�
	��һ��RGBͼתCMYͼ
ע��
    ����ת�����CMYͼ������ΪRGBͼ���ʽ��ʾ��
 ������
	CImgProcess* pTo: Ŀ�����ͼ��� CImgProcess ָ��
 ����ֵ��
	�� 
*******************/
void CImgProcess::RGB2CMY(CImgProcess *pTo)
{
	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	int i, j;
	
	for(i=0; i<nHeight; i++)
	{
		for(j=0; j<nWidth; j++)
		{
			COLORREF RGBPixel = GetPixel(j, i);
			//��ȡRGB����
			int R = GetRValue(RGBPixel);
			int G = GetGValue(RGBPixel);
			int B = GetBValue(RGBPixel);
			int C,M,Y;

			C = 255 - R;
			M = 255 - G;
			Y = 255 - B;

			//�����������γ�CMYͼ��
			pTo->SetPixel(j, i, RGB(C, M, Y));
		}//for j
	}//for i

}

/******************* 
void CImgProcess::CMY2RGB(CImgProcess *pTo)
 
 ���ܣ�
	��һ��CMYͼתRGBͼ

 ע��
	

 ������
	CImgProcess* pTo: Ŀ�����ͼ��� CImgProcess ָ��
	 
 ����ֵ��
	��
	
*******************/
void CImgProcess::CMY2RGB(CImgProcess *pTo)
{
	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	int i, j;
	
	for(i=0; i<nHeight; i++)
	{
		for(j=0; j<nWidth; j++)
		{
			COLORREF RGBPixel = GetPixel(j, i);
			//��ȡCMY����
			double C = GetRValue(RGBPixel)/255.0;
			double M = GetGValue(RGBPixel)/255.0;
			double Y = GetBValue(RGBPixel)/255.0;
			double R,G,B;

			R = 1 - C;
			G = 1 - M;
			B = 1 - Y;

			R *=255.0;
			G *=255.0;
			B *=255.0;

			//�����������γ�CMYͼ��
			pTo->SetPixel(j, i, RGB(R, G, B));
		}
	}
}

/******************* 
void CImgProcess::RGB2HSV(CImgProcess *pTo)
 
 ���ܣ�
	��һ��RGBͼתHSVͼ
 
 ������
	CImgProcess* pTo: Ŀ�����ͼ��� CImgProcess ָ��
	 
 ����ֵ��
	��
*******************/
void CImgProcess::RGB2HSV(CImgProcess *pTo)
{
	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	int i, j;
	
	for(i=0; i<nHeight; i++)
	{
		for(j=0; j<nWidth; j++)
		{
			COLORREF RGBPixel = GetPixel(j, i);
			//��ȡRGB����
			double R = GetRValue(RGBPixel)/255.0;
			double G = GetGValue(RGBPixel)/255.0;
			double B = GetBValue(RGBPixel)/255.0;

			//����HSV
			double H,S,V,MAX,MIN,TEMP;
			MAX = max(max(R,G),B);
			MIN = min(min(R,G),B);
			V = MAX;
			TEMP = MAX - MIN;

			if(MAX != 0)
			{
				S = TEMP/MAX;
			}
			else
			{
				S = 0;
				//H = UNDEFINEDCOLOR;
				return;
			}
           if(R == MAX)
                H = (G - B)/TEMP;
		   else if(G == MAX)
                H = 2 + (B - R)/TEMP;
           else
                H = 4 + (R - G)/TEMP;
		   H *=60;
		   if(H < 0)
			   H +=360;

		   //��HSV������Ϊ���ڼ�����Ϸ�ΧΪ[0��255]����ʾ��ͼ��
		   H /=360.0;
		   H *=255.0;
		   S *=255.0;
		   V *=255.0;

			//�����������γ�HSVͼ��
			pTo->SetPixel(j, i, RGB(H, S, V));
		}
	}
}

/******************* 
void CImgProcess::HSV2RGB(CImgProcess *pTo)
 
 ���ܣ�
	��һ��HSVͼתRGBͼ
 
 ������
	CImgProcess* pTo: Ŀ�����ͼ��� CImgProcess ָ��
	 
 ����ֵ��
	��
*******************/
void CImgProcess::HSV2RGB(CImgProcess *pTo)
{
    int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	int i, j;
	
	for(i=0; i<nHeight; i++)
	{
		for(j=0; j<nWidth; j++)
		{
			COLORREF RGBPixel = GetPixel(j, i);
			//��ȡHSV����
			double H = (GetRValue(RGBPixel)/255.0)*360.0;
			double S = GetGValue(RGBPixel)/255.0;
			double V = GetBValue(RGBPixel)/255.0;

			//����RGB
			double R,G,B,f,p,q,t,TEMP;
			int n;

			if(S == 0)
			{
				R = G = B = V;
			}

			n = floor(H/60);
			TEMP = H/60;
			f = TEMP - n;
			p = V*(1-S);
			q = V*(1-f*S);
			t = V*(1-(1-f)*S);

			switch(n)
			{
			case 0:
				R = V;
				G = t;
				B = p;
				break;
			case 1:
				R = q;
				G = V;
				B = p;
				break;
			case 2:
				R = p;
				G = V;
				B = t;
				break;
			case 3:
				R = p;
				G = q;
				B = V;
				break;
			case 4:
				R = t;
				G = p;
				B = V;
				break;
			default:  //case 5:
				R = V;
				G = p;
				B = q;
				break;
			}

		   R *=255.0;
		   G *=255.0;
		   B *=255.0;

			//�����������γ�RGBͼ��
			pTo->SetPixel(j, i, RGB(R, G, B));
		}// for j
	}//for i
}


/******************* 
void CImgProcess::RGB2YUV(CImgProcess *pTo)
 ���ܣ�
	��һ��RGBͼתYUVͼ

 ������
	CImgProcess* pTo: Ŀ�����ͼ��� CImgProcess ָ��
	 
 ����ֵ��
	��
*******************/
void CImgProcess::RGB2YUV(CImgProcess *pTo)
{
    int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	int i, j;
	
	for(i=0; i<nHeight; i++)
	{
		for(j=0; j<nWidth; j++)
		{
			COLORREF RGBPixel = GetPixel(j, i);
			//��ȡRGB����
			double R = GetRValue(RGBPixel);
			double G = GetGValue(RGBPixel);
			double B = GetBValue(RGBPixel);

			//����YUV
			double Y,U,V;
			Y = 0.299*R + 0.587*G + 0.114*B;
			U = (B - Y)*0.567;
			V = (R - Y)*0.713;

			//��ֹ���
			if(Y > 255)
				Y = 255;
			if(Y < 0)
				Y = 0;
			if(U > 255)
				U = 255;
			if(U < 0)
				U = 0;
			if(V > 255)
				V = 255;
			if(V < 0)
				V = 0;
			
			//�����������γ�YUVͼ��
			pTo->SetPixel(j, i, RGB(Y, U, V));
		}//for j
	}//for i
}

/******************* 
void CImgProcess::YUV2RGB(CImgProcess *pTo)
 
 ���ܣ�
	��һ��YUVͼ��ת��ΪRGBͼ��
 ע��
	��

 ������
	CImgProcess* pTo: Ŀ�����ͼ��� CImgProcess ָ��

	 
 ����ֵ��
	��
 ע��
       ��ʾYUVͼ��ʱ��ʹ��RGBͼ���ʽ�ڵ�������ʾ
*******************/
void CImgProcess::YUV2RGB(CImgProcess *pTo)
{
    int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	int i, j;
	
	for(i=0; i<nHeight; i++)
	{
		for(j=0; j<nWidth; j++)
		{
			COLORREF RGBPixel = GetPixel(j, i);
			//��ȡYUV����
			double Y = GetRValue(RGBPixel);
			double U = GetGValue(RGBPixel);
			double V = GetBValue(RGBPixel);

			//����RGB
			double R,G,B;
			R = Y + 1.402*V;
			G = Y - 0.344*U - 0.714*V;
			B = Y + 1.772*U;
			
			//��ֹ���
			if(R > 255)
				R = 255;
			if(R < 0)
				R = 0;
			if(G > 255)
				G = 255;
			if(G < 0)
				G = 0;
			if(B > 255)
				B = 255;
			if(B < 0)
				B = 0;

			//�����������γ�RGBͼ��
			pTo->SetPixel(j, i, RGB(R, G, B));
		}
	}
}

/******************* 
void CImgProcess::RGB2YIQ(CImgProcess *pTo)
 
 ���ܣ�
	��һ��RGBͼ��ת��ΪYIQͼ��
 ע��
	��

 ������
	CImgProcess* pTo: Ŀ�����ͼ��� CImgProcess ָ��

	 
 ����ֵ��
	��
 ע��
       ��ʾYIQͼ��ʱ��ʹ��RGBͼ���ʽ�ڵ�������ʾ
*******************/
void CImgProcess::RGB2YIQ(CImgProcess *pTo)
{
    int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	int i, j;
	
	for(i=0; i<nHeight; i++)
	{
		for(j=0; j<nWidth; j++)
		{
			COLORREF RGBPixel = GetPixel(j, i);
			//��ȡRGB����
			double R = GetRValue(RGBPixel);
			double G = GetGValue(RGBPixel);
			double B = GetBValue(RGBPixel);

			//����YUV
			double Y,I,Q;
			Y = 0.299*R + 0.587*G + 0.114*B;
			I = 0.596*R - 0.274*G - 0.322*B;
			Q = 0.211*R - 0.523*G + 0.312*B;

			//��ֹ���
			if(Y > 255)
				Y = 255;
			if(Y < 0)
				Y = 0;
			if(I > 255)
				I = 255;
			if(I < 0)
				I = 0;
			if(Q > 255)
				Q = 255;
			if(Q < 0)
				Q = 0;
			
			//�����������γ�YIQͼ��
			pTo->SetPixel(j, i, RGB(Y, I, Q));
		}
	}
}

/******************* 
void CImgProcess::YIQ2RGB(CImgProcess *pTo)
 
 ���ܣ�
	��һ��YIQͼ��ת��ΪRGBͼ��

 ������
	CImgProcess* pTo: Ŀ�����ͼ��� CImgProcess ָ��

 ����ֵ��
	�� 
*******************/
void CImgProcess::YIQ2RGB(CImgProcess *pTo)
{
    int nHeight = GetHeight();
	int nWidth = GetWidthPixel();

	int i, j;
	
	for(i=0; i<nHeight; i++)
	{
		for(j=0; j<nWidth; j++)
		{
			COLORREF RGBPixel = GetPixel(j, i);
			//��ȡYIQ����
			double Y = GetRValue(RGBPixel);
			double I = GetGValue(RGBPixel);
			double Q = GetBValue(RGBPixel);

			//����RGB
			double R,G,B;
			R = Y + 0.956*I + 0.114*Q;
			G = Y - 0.272*I - 0.647*Q;
			B = Y - 1.106*I + 1.703*Q;

			//��ֹ���
			if(R > 255)
				R = 255;
			if(R < 0)
				R = 0;
			if(G > 255)
				G = 255;
			if(G < 0)
				G = 0;
			if(B > 255)
				B = 255;
			if(B < 0)
				B = 0;
			
			//�����������γ�RGBͼ��
			pTo->SetPixel(j, i, RGB(R, G, B));
		}//for j
	}//for i
}




/*******************
void CImgProcess::TemplateMatch(CImgProcess* pTo, CImgProcess* pTemplate)
���ܣ�
	������ص�ģ��ƥ��
������
	CImgProcess* pTo��Ŀ��ͼ��� CImgProcess ָ��
	CImgProcess* pTemplate����ͼ��� CImgProcess ָ��
����ֵ��
	��
*******************/
void CImgProcess::TemplateMatch(CImgProcess* pTo, CImgProcess* pTemplate)
{	
	
	//ѭ������
	int i, j, m, n;
	
	double dSumT; //ģ��Ԫ�ص�ƽ����
	double dSumS; //ͼ��������Ԫ�ص�ƽ����
	double dSumST; //ͼ���������ģ��ĵ��	

	//��Ӧֵ
	double R;

	//��¼��ǰ�������Ӧ
	double MaxR;

	//�����Ӧ����λ��
	int nMaxX;
	int nMaxY;

	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();
	//ģ��ĸߡ���
	int nTplHeight = pTemplate->GetHeight();
	int nTplWidth = pTemplate->GetWidthPixel();

	//���� dSumT
	dSumT = 0;
	for (m = 0;m < nTplHeight ;m++)
	{
		for(n = 0;n < nTplWidth ;n++)
		{
			// ģ��ͼ���m�У���n�����صĻҶ�ֵ
			int nGray = pTemplate->GetGray(n, m);
			
			dSumT += (double)nGray*nGray;
		}
	}

	//�ҵ�ͼ���������Ӧ�ĳ���λ��
	MaxR = 0;
	for (i = 0;i < nHeight - nTplHeight +1 ;i++)
	{
		for(j = 0;j < nWidth - nTplWidth + 1;j++)
		{
			dSumST = 0;
			dSumS = 0;
	
			for (m = 0;m < nTplHeight ;m++)
			{
				for(n = 0;n < nTplWidth ;n++)
				{
					// ԭͼ���i+m�У���j+n�����صĻҶ�ֵ
					int nGraySrc  = GetGray(j+n, i+m);
			
					// ģ��ͼ���m�У���n�����صĻҶ�ֵ
					int nGrayTpl = pTemplate->GetGray(n, m);
										
					dSumS += (double)nGraySrc*nGraySrc;
					dSumST += (double)nGraySrc*nGrayTpl;
				}
			}

			R = dSumST / ( sqrt(dSumS)*sqrt(dSumT));//���������Ӧ

			//����������ԱȽ�
			if (R > MaxR)
			{
				MaxR = R;
				nMaxX = j;
				nMaxY = i;
			}
		}
	}

	pTo->InitPixels(255); //���Ŀ��ͼ��
	//���ҵ������ƥ�������Ƶ�Ŀ��ͼ��
	for (m = 0;m < nTplHeight ;m++)
	{
		for(n = 0;n < nTplWidth ;n++)
		{
			int nGray = pTemplate->GetGray(n, m);
			pTo->SetPixel(nMaxX+n, nMaxY+m, RGB(nGray, nGray, nGray));
		}
	}

}


/*******************
vector< vector<int> > CImgProcess::GetGrayMatrix(POINT ptD1, POINT ptD2)
���ܣ�
	����Ҷȹ��־���
������
	POINT ptD1���Ҷȹ��־���ռ�λ�ù�ϵ�ĵ�1����
	POINT ptD2���Ҷȹ��־���ռ�λ�ù�ϵ�ĵ�1����
����ֵ��
	��
*******************/
vector< vector<int> > CImgProcess::GetGrayMatrix(POINT ptD1, POINT ptD2)
{
	vector< vector<int> > GrayMat; //�Ҷȹ��־���

	int nHeight = GetHeight();
	int nWidth = GetWidthPixel();
	int i, j; //ѭ������
	int nGray;

	//ͳ�ƻҶȼ�����
	int nMaxGray = 0;
	for(i=0; i<nHeight; i++)
	{
		for(j=0; j<nWidth; j++)
		{
			nGray = GetGray(j, i);
			if(nGray > nMaxGray)
				nMaxGray = nGray;
		}
	}

	//��ʼ���Ҷȹ��־���
	vector<int> vecRow(nMaxGray+1, 0);
	for(i=0; i<nMaxGray+1; i++)
		GrayMat.push_back(vecRow);


	//ͳ�Ʒ��Ͽռ�λ�ù�ϵ���ֱ��������ֵgray1, gray2�����ض���Ŀ
	int i2, j2; //��(i, j)����λ�ù�ϵptD1��ptD2�ĵ�
	for(i=0; i<nHeight; i++)
	{
		for(j=0; j<nWidth; j++)
		{
			nGray = GetGray(j, i);
			
			i2 = i + ptD1.y;
			j2 = j + ptD1.x;
			if( ((i2>=0) && (i2<nHeight)) && ((j2>=0) &&(j2<nWidth)) )
			{
				int nGrayD1 = GetGray(j2, i2);
				GrayMat[nGray][nGrayD1]++;//��Ӧ������һ
			}

			i2 = i + ptD2.y;
			j2 = j + ptD2.x;
			if( ((i2>=0) && (i2<nHeight)) && ((j2>=0) &&(j2<nWidth)) )
			{
				int nGrayD2 = GetGray(j2, i2);
				GrayMat[nGray][nGrayD2]++;//��Ӧ������һ
			}
		}
	}

	//���ػҶȹ��־���
	return GrayMat;
}