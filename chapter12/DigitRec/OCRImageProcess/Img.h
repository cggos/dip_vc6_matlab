// Img.h: interface for the CImg class.
//
//////////////////////////////////////////////////////////////////////




#ifndef __GRAY_H__
#define __GRAY_H__

#include "math.h"

// ����ͼ�����


// �ڼ���ͼ���Сʱ�����ù�ʽ��biSizeImage = biWidth' �� biHeight��
// ��biWidth'��������biWidth�������biWidth'������4������������ʾ
// ���ڻ����biWidth�ģ���4�������������WIDTHBYTES������������
// biWidth'
#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)

//////  0 -> 255  ��->��


class CImg  
{
public:
	// ���캯��
	CImg();
	// Copy���캯��
	CImg(CImg& gray);
	// ���ء�=���������������µĶ���
	void operator = (CImg& gray); //ͼ��ֵ

	BOOL operator == (CImg& gray); //�ж�2��ͼ���Ƿ���ͬ
	CImg operator & (CImg& gray); //ͼ��λ��
	CImg operator | (CImg& gray); //ͼ��λ��
	CImg operator + (CImg gray); //ͼ�����
	CImg operator - (CImg& gray); //ͼ�����
	CImg operator ! (); //ͼ��ɫ
	

	// ��������
	virtual ~CImg();

public:	
	
	// �ж�λͼ�Ƿ���Ч
	BOOL IsValidate() { return m_pBMIH != NULL; }

	// ���������ص�ֵ��ʼ��Ϊcolor
	void InitPixels(BYTE color);

	// ���ļ�����λͼ
	BOOL AttachFromFile(LPCTSTR lpcPathName);
	BOOL AttachFromFile(CFile &file);
	// ��λͼ���浽�ļ�
	BOOL SaveToFile(LPCTSTR lpcPathName);
	BOOL SaveToFile(CFile &file);

	// ��DC�ϻ���λͼ
	BOOL Draw(CDC* pDC);
	BOOL Draw(CDC* pDC, CRect rt);

	// �������ص�ֵ
	void SetPixel(int x, int y, COLORREF color);
	// ��ȡ���ص�ֵ
	COLORREF GetPixel(int x, int y);
	// ��ȡ�Ҷ�ֵ
	BYTE GetGray(int x, int y);


	// ��ȡһ�е��ֽ���
	int GetWidthByte();
	// ��ȡһ�е�������
	int GetWidthPixel();
	// ��ȡ�߶�
	int GetHeight();

	//�ı�λͼ�ĳߴ�
	void ImResize(int nHeight, int nWidth);
public:

	// �滭����	
	// ����ֱ��
	void Line(POINT ptStart, POINT ptEnd);
	void Line(POINT ptStart, int nLen, int nWide, BOOL bHor);
	// ���ƾ���
	void Rectangle(int x, int y, int r = 5);
	void Rectangle(POINT ptLT, int r = 5);
	void Rectangle(POINT ptLT, POINT ptRB);
	void Circle(int x, int y, int r = 5){};

public:
	// �ж��Ƿ��Ƕ�ֵͼ��
	BOOL IsBinaryImg();
	// �ж��Ƿ�������ͼ��
	BOOL IsIndexedImg();
	// 256ɫ����ͼ��ת�Ҷ�ͼ��
	bool Index2Gray();

	LPVOID GetColorTable(){return m_lpvColorTable;}
private:
	void CleanUp();	

public:
	// �ļ�����
	BITMAPINFOHEADER *m_pBMIH;
	LPBYTE  *m_lpData;
protected:
	int m_nColorTableEntries;
	LPVOID m_lpvColorTable;
};


/////////////////////////////////// inline functions ///////////////////////////////
/**************************************************
inline int CImg::GetWidthByte()

���ܣ�
	����CImgʵ���е�ͼ��ÿ��ռ�õ��ֽ���
���ƣ�
	��

������
	��
����ֵ��
	int���ͣ�����ͼ��ÿ��ռ�õ��ֽ���
***************************************************/
inline int CImg::GetWidthByte()
{
	return WIDTHBYTES((m_pBMIH->biWidth)*m_pBMIH->biBitCount);
}

/**************************************************
inline int CImg::GetWidthPixel()

���ܣ�
	����CImgʵ���е�ͼ��ÿ�е�������Ŀ��������ֱ��ʻ���
���ƣ�
	��

������
	��
����ֵ��
	int���ͣ�����ͼ��ÿ�е�������Ŀ
***************************************************/
inline int CImg::GetWidthPixel()
{
	return m_pBMIH->biWidth;
}

/**************************************************
inline int CImg::GetHeight()

���ܣ�
	����CImgʵ���е�ͼ��ÿ�е�������Ŀ��������ֱ��ʻ�߶�
���ƣ�
	��

������
	��
����ֵ��
	int���ͣ�����ͼ��ÿ�е�������Ŀ
***************************************************/
inline int CImg::GetHeight()
{
	return m_pBMIH->biHeight;
}

/**************************************************
inline BYTE CImg::GetGray(int x, int y)

���ܣ�
	����ָ������λ�����صĻҶ�ֵ

������
	int x, int y
		ָ�������غᡢ������ֵ
����ֵ��
	��������λ�õĻҶ�ֵ
***************************************************/
inline BYTE CImg::GetGray(int x, int y)
{
	COLORREF ref = GetPixel(x, y);
	BYTE r, g, b, byte;
	r = GetRValue(ref);
	g = GetGValue(ref);
	b = GetBValue(ref);

	if(r == g && r == b)
		return r;

	float ff = (r + g + b)/(3.0);

	// �ҶȻ�
	byte =  (int)ff;
				
	return byte;
}

/**************************************************
inline COLORREF CImg::GetPixel(int x, int y)

���ܣ�
	����ָ������λ�����ص���ɫֵ

������
	int x, int y
		ָ������λ�õ�����
����ֵ��
	COLERREF���ͣ�������RGB��ʽ��ʾ��ָ��λ�õ���ɫֵ
***************************************************/
inline COLORREF CImg::GetPixel(int x, int y)
{
	if(m_pBMIH->biBitCount == 8)		// 256ɫͼ
	{
		BYTE byte = m_lpData[m_pBMIH->biHeight - y - 1][x];
		return RGB(byte, byte, byte);
	}
	else if(m_pBMIH->biBitCount == 1)	// ��ɫͼ
	{
		BYTE ret = (1<<(7-x%8) & m_lpData[m_pBMIH->biHeight - y - 1][x/8]);

		// ��0�����
		RGBQUAD *p = (RGBQUAD*)m_lpvColorTable;
		if(p[0].rgbBlue != 0)
			ret = !ret;

		if(ret)
			return RGB(255, 255, 255);		// ��ɫ
		else 
			return RGB(0, 0, 0);			// ��ɫ
	}
	else if(m_pBMIH->biBitCount == 24)	// ���ͼ
	{
		COLORREF color = RGB(m_lpData[m_pBMIH->biHeight - y - 1][x*3 + 2], 
		m_lpData[m_pBMIH->biHeight - y - 1][x*3 + 1],
		m_lpData[m_pBMIH->biHeight - y - 1][x*3]);
		return color;
	}
	else
	{
		throw "not support now";
		return 0;
	}
}

inline BOOL CImg::IsBinaryImg()
{
	int i,j;
	
	for(i = 0; i < m_pBMIH->biHeight; i++)
	{
		for(j = 0; j < m_pBMIH->biWidth; j++)
		{
			if( (GetGray(j, i) != 0) && (GetGray(j, i) != 255) )//����0��255֮��ĻҶ�ֵ
				return FALSE;
		}//for j
	}//for i

	return TRUE;
}

inline BOOL CImg::IsIndexedImg()
{
	if ((m_lpvColorTable != NULL)&&(m_nColorTableEntries!=0)) {
		return true;
	}
	else {
		return false;
	}
}
#endif // __GRAY_H__
