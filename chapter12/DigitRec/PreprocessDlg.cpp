// PreprocessDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DigitRec.h"
#include "PreprocessDlg.h"
#include "DirDialog.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CPreprocessDlg dialog


CPreprocessDlg::CPreprocessDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPreprocessDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPreprocessDlg)
	m_nThres = 145;
	m_nUpper = 1500;
	m_nLower = 15;
	m_strTrainFile = _T("");
	m_strClassLabel = _T("");
	//}}AFX_DATA_INIT
}


void CPreprocessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPreprocessDlg)
	DDX_Text(pDX, IDC_EDIT_THRES, m_nThres);
	DDV_MinMaxUInt(pDX, m_nThres, 0, 255);
	DDX_Text(pDX, IDC_EDIT_UPPER, m_nUpper);
	DDX_Text(pDX, IDC_EDIT_LOWER, m_nLower);
	DDX_Text(pDX, IDC_EDIT_DIR, m_strTrainFile);
	DDX_Text(pDX, IDC_EDIT_FILE, m_strClassLabel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPreprocessDlg, CDialog)
	//{{AFX_MSG_MAP(CPreprocessDlg)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, OnButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_DIR, OnButtonDir)
	ON_BN_CLICKED(IDC_RADIO_TEST, OnRadioTest)
	ON_BN_CLICKED(IDC_RADIO_TRAIN, OnRadioTrain)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreprocessDlg message handlers



void CPreprocessDlg::OnButtonLoad() 
{
	// �򿪶Ի������û�ѡ������ǩ�ļ���*.txt��
	CString strFilePathName;
	CFileDialog dlg(true, "txt", "", NULL, "Class Label Files(*.txt)|*.txt||", NULL); //TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���
	
	if( dlg.DoModal() == IDOK )
		strFilePathName = dlg.GetPathName();

	// ���¿ؼ�
	m_strClassLabel = strFilePathName;
	((CEdit*)GetDlgItem(IDC_EDIT_FILE))->SetWindowText(m_strClassLabel);

}
void CPreprocessDlg::OnButtonDir() 
{
	// ѡ��ѵ������Ŀ¼
	CDirDialog dir;
	if(!dir.DoBrowse(*this))
	{
		return;
	}
	m_strTrainFile = dir.GetPath();
	((CEdit*)GetDlgItem(IDC_EDIT_DIR))->SetWindowText(m_strTrainFile);
}

void CPreprocessDlg::OnRadioTest() 
{
	( (CButton* )GetDlgItem(IDC_EDIT_FILE) )->EnableWindow(false); //����ʱ����Ҫѡ�����ǩ�ļ�
	( (CButton* )GetDlgItem(IDC_BUTTON_LOAD) )->EnableWindow(false);
	bTrain = false;
}

void CPreprocessDlg::OnRadioTrain() 
{
	( (CButton* )GetDlgItem(IDC_EDIT_FILE) )->EnableWindow(true); //ѵ��ʱ��Ҫѡ�����ǩ�ļ�
	( (CButton* )GetDlgItem(IDC_BUTTON_LOAD) )->EnableWindow(true);
	bTrain = true;
}

BOOL CPreprocessDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	( (CButton* )GetDlgItem(IDC_RADIO_TRAIN) )->SetCheck(true);	
	bTrain = true;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPreprocessDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(true);

	if( bTrain && (m_strClassLabel.GetLength() == 0) )
	{
		AfxMessageBox("ѵ��ǰ����ѡ��ѵ���������ǩ�ļ�!");
		return;
	}

	if(m_strTrainFile.GetLength() == 0)
	{
		AfxMessageBox("ѵ��ǰ����ѡ�񱣴洦���������Ŀ¼!");
		return;
	}


	CDialog::OnOK();
}
