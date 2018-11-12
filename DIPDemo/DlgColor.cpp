// DlgColor.cpp : implementation file
//

#include "stdafx.h"
#include "DIPDemo.h"
#include "DlgColor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgColor dialog


CDlgColor::CDlgColor(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgColor::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgColor)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgColor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgColor)
	DDX_Control(pDX, IDC_COLOR_LIST, m_lstColor);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgColor, CDialog)
	//{{AFX_MSG_MAP(CDlgColor)
	ON_LBN_DBLCLK(IDC_COLOR_LIST, OnDblclkColorList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgColor message handlers

BOOL CDlgColor::OnInitDialog() 
{
	// ѭ������
	int		i;
	
	// ����Ĭ��OnInitDialog����
	CDialog::OnInitDialog();
	
	// ���α��ɫ����
	for (i = 0; i < m_nColorCount; i++)
	{
		m_lstColor.AddString(m_lpColorName + i * m_nNameLen);
	}

	// ѡ�г�ʼ�����
	m_lstColor.SetCurSel(m_nColor);
	
	// ����TRUE
	return TRUE;
}

void CDlgColor::OnDblclkColorList() 
{
	// ˫���¼���ֱ�ӵ���OnOK()��Ա����
	OnOK();
	
}

void CDlgColor::OnOK() 
{
	// �û�����ȷ����ť
	m_nColor = m_lstColor.GetCurSel();
	
	// ����Ĭ�ϵ�OnOK()����
	CDialog::OnOK();
}
