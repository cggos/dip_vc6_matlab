#pragma once


// CDlgFreqGaussLPF �Ի���

class CDlgFreqGaussLPF : public CDialog
{
	DECLARE_DYNAMIC(CDlgFreqGaussLPF)

public:
	CDlgFreqGaussLPF(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgFreqGaussLPF();

// �Ի�������
	enum { IDD = IDD_DLG_FREQ_GAUSSLPF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// ��˹�˲���Sigma����
	double m_dSigma;
};
