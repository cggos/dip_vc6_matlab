#pragma once


// CDlgHough �Ի���

class CDlgHough : public CDialog
{
	DECLARE_DYNAMIC(CDlgHough)

public:
	CDlgHough(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgHough();

// �Ի�������
	enum { IDD = IDD_HOUGH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_nNum;
};
