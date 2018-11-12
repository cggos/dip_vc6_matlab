#pragma once


// CDlgEdgeSobelPrewitt �Ի���

class CDlgEdgeSobelPrewitt : public CDialog
{
	DECLARE_DYNAMIC(CDlgEdgeSobelPrewitt)

public:
	CDlgEdgeSobelPrewitt(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgEdgeSobelPrewitt();

// �Ի�������
	enum { IDD = IDD_EDGE_SOBEL_PREWITT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	// ��ֵ
	BYTE m_bThre;
	// ��Ե����
	int m_nEdgeType;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedRadio5();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	int m_bThining;
	int m_bGratOnly;
	// �趨Ϊtrueʱȥ��45�Ⱥ�135�ȱ�Եѡ��
	BOOL isRobert;
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	BOOL isGauss;
	double m_dSigma;
	afx_msg void OnBnClickedCheck3();
};
