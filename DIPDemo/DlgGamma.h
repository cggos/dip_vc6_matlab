#pragma once


// CDlgGamma �Ի���

class CDlgGamma : public CDialog
{
	DECLARE_DYNAMIC(CDlgGamma)

public:
	CDlgGamma(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgGamma();

// �Ի�������
	enum { IDD = IDD_DLG_GAMMA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// ����ϵ��
	double m_dEsp;
	// ٤��ϵ��
	double m_dGamma;
};
