#pragma once


// CDlgWndTran �Ի���

class CDlgWndTran : public CDialog
{
	DECLARE_DYNAMIC(CDlgWndTran)

public:
	CDlgWndTran(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgWndTran();

// �Ի�������
	enum { IDD = IDD_DLG_WNDTRAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	BYTE m_bLower;
	BYTE m_bUpper;
	afx_msg void OnPaint();
	// ֱ��ͼ����
	double* m_dHist;
};
