#pragma once


// CDlgPointThre �Ի���

class CDlgPointThre : public CDialog
{
	DECLARE_DYNAMIC(CDlgPointThre)

public:
	CDlgPointThre(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgPointThre();

// �Ի�������
	enum { IDD = IDD_DLG_PointThre };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// ��ֵ
	BYTE m_bThre;
};
