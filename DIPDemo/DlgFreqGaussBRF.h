#pragma once


// CDlgFreqGaussBRF �Ի���

class CDlgFreqGaussBRF : public CDialog
{
	DECLARE_DYNAMIC(CDlgFreqGaussBRF)

public:
	CDlgFreqGaussBRF(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgFreqGaussBRF();

// �Ի�������
	enum { IDD = IDD_DLG_FREQ_GAUSSBRF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// �������Ƶ��
	int m_nFreq;
	// ������
	int m_nWidth;
};
