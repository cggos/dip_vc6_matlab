// ValueTrack.h: interface for the CValueTrack class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VALUETRACK_H__FE95CD67_171A_48A6_B79E_812FE2C1B32D__INCLUDED_)
#define AFX_VALUETRACK_H__FE95CD67_171A_48A6_B79E_812FE2C1B32D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CValueTrack  
{
public:
	CValueTrack(CWnd *pWnd);
	virtual ~CValueTrack();

	void AddValue(double val);
	void Draw();
	void Init()
	{
		m_nValue = 0;
	}

	CWnd *m_pWnd; //��ͼ����
	CRect m_rt; //��ͼ������

	int m_nMaxValues; //��ͼ�������ɵ�������ֵ����
	double *m_pValues; //���ֵ����
	double *m_pTemp; //��ʱ���ֵ����
	int m_nValue; //���ֵ����

private:
	// ����
	CBrush m_GroundBush;
	CBrush m_SelectBrush;
	CPen m_WavePen;
	CPen m_CoordinatePen;
	CPen m_ProcesslinePen;
	CPen m_LabelPen;
	CPen m_LabewavePen;
	CPen m_ProgramPen;

};

#endif // !defined(AFX_VALUETRACK_H__FE95CD67_171A_48A6_B79E_812FE2C1B32D__INCLUDED_)
