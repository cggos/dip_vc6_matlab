#pragma once


// CDlgFreqGaussHPF �Ի���

class CDlgFreqGaussHPF : public CDialog
{
	DECLARE_DYNAMIC(CDlgFreqGaussHPF)

public:
	CDlgFreqGaussHPF(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgFreqGaussHPF();

// �Ի�������
	enum { IDD = IDD_FREQ_GAUSSHPF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// ��˹�˲���Sigma����
	double m_dSigma;
};
