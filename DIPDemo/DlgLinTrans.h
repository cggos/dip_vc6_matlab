#pragma once


// CDlgLinTrans �Ի���

class CDlgLinTrans : public CDialog
{
	DECLARE_DYNAMIC(CDlgLinTrans)

public:
	CDlgLinTrans(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgLinTrans();

// �Ի�������
	enum { IDD = IDD_DLG_LINTRANS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	BYTE m_bS1;
	BYTE m_bS2;
	BYTE m_bT1;
	BYTE m_bT2;
};
