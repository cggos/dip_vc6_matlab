// DlgFreqGaussLPF.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DIPDemo.h"
#include "DlgFreqGaussLPF.h"


// CDlgFreqGaussLPF �Ի���

IMPLEMENT_DYNAMIC(CDlgFreqGaussLPF, CDialog)

CDlgFreqGaussLPF::CDlgFreqGaussLPF(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFreqGaussLPF::IDD, pParent)
	, m_dSigma(0)
{

}

CDlgFreqGaussLPF::~CDlgFreqGaussLPF()
{
}

void CDlgFreqGaussLPF::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_dSigma);
	DDV_MinMaxDouble(pDX, m_dSigma, 0, 256);
}


BEGIN_MESSAGE_MAP(CDlgFreqGaussLPF, CDialog)
END_MESSAGE_MAP()


// CDlgFreqGaussLPF ��Ϣ�������
