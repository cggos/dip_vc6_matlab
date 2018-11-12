// OCRImgProcess.h: interface for the COCRImageProcess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OCRIMAGEPROCESS_H__5E3CECEC_A041_49A1_9C04_531E0179083F__INCLUDED_)
#define AFX_OCRIMAGEPROCESS_H__5E3CECEC_A041_49A1_9C04_531E0179083F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImgProcess.h"



//��ͨ��ͼ������ CImgProcess ������ר�Ÿ�������ʶ��ϵͳͼ��Ԥ�������
class COCRImageProcess  : public CImgProcess
{
public:
	COCRImageProcess();
	virtual ~COCRImageProcess();

	COCRImageProcess& operator = (CImg& img);

	void DelScatterNoise(COCRImageProcess* pTo, int upperThres, int lowerThres);//ȥ����ɢ����

	//���α任
	void SlopeAdjust(COCRImageProcess* pTo); //Ŀ������ַ���������б�ȵ���
	vector<RECT> ObjectSegment(); //�����ַ����ָ�
	void ObjectNorm(COCRImageProcess* pTo, int nTargWidth, int nTargHeight, vector<RECT> &vecRT); //�����ַ�����С��һ��
	void ObjectAlign(COCRImageProcess* pTo, vector<RECT> &vecRT); //Ŀ������ַ����Ľ�������


	//������
	RECT RgnZoom(COCRImageProcess* pTo, int nTargWidth, int nTargHeight, LPRECT lpRect);//ͼ����ĳ�����������

};

#endif // !defined(AFX_OCRIMAGEPROCESS_H__5E3CECEC_A041_49A1_9C04_531E0179083F__INCLUDED_)
