#pragma once


// CDlgGrayScaling �Ի���

class CDlgGrayScaling : public CDialog
{
	DECLARE_DYNAMIC(CDlgGrayScaling)

public:
	CDlgGrayScaling(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgGrayScaling();

// �Ի�������
	enum { IDD = IDD_DLG_GRAYSCAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	BYTE m_bX1;
	BYTE m_bX2;
	BYTE m_bY1;
	BYTE m_bY2;
};
