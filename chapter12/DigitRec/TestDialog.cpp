// TestDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DigitRec.h"
#include "TestDialog.h"
#include "DirDialog.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestDialog dialog


CTestDialog::CTestDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CTestDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestDialog)
	//}}AFX_DATA_INIT
	m_vecClassNames.clear();
	m_pNet = NULL;
	m_bInTest = false;
	m_bReadyToRec = false;
}

CTestDialog::~CTestDialog()
{
	if( m_pNet != NULL )
		delete m_pNet;
}

void CTestDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestDialog)
	DDX_Control(pDX, IDC_EDIT_FILE, m_EditFile);
	DDX_Control(pDX, IDC_BUTTON_TEST_BROWSE, m_ButtonTestBrowse);
	DDX_Control(pDX, IDC_EDIT_LOAD_TRAIN_FILE, m_EditLoadTrainFile);
	DDX_Control(pDX, IDC_CHECK_DIR, m_CheckDir);
	//}}AFX_DATA_MAP
}


bool CTestDialog::LoadFromFile(CString strFilePathName)
{
	//��������������Ϣ
	CFile file;
	if(!file.Open(strFilePathName, CFile::modeRead))
		return false;

	//��������ṹ��Ϣ
	int nInput, nOutput, nNeurons;
	file.Read(&nInput, sizeof(nInput));
	file.Read(&nOutput, sizeof(nOutput));
	file.Read(&nNeurons, sizeof(nNeurons));
	double dError, dLearningRate;
	int nMaxEpoch;
	file.Read(&dError, sizeof(dError));
	file.Read(&dLearningRate, sizeof(dLearningRate));
	file.Read(&nMaxEpoch, sizeof(nMaxEpoch));

	int nLen;
	char* lpBuf;
	int i;
	for(i=0; i<nOutput; i++)
	{
		file.Read(&nLen, sizeof(nLen));
		lpBuf = new char[nLen+1];
		file.Read(lpBuf, nLen);
		lpBuf[nLen] = '\0';
		CString strClassName = lpBuf;
		delete [] lpBuf;
		m_vecClassNames.push_back(strClassName);
	}

	file.Read(&nLen, sizeof(nLen));
	lpBuf = new char[nLen+1];
	file.Read(lpBuf, nLen);
	lpBuf[nLen] = '\0';
	CString strTrainPathName;
	strTrainPathName = lpBuf;
	delete [] lpBuf;
	

	DWORD dwCurPos = file.GetPosition(); //��ǰ�ļ�ָ��λ��
	file.Close();
	
	if(m_pNet != NULL)
		delete m_pNet;
	m_pNet = new CNeuralNet(nInput, nOutput, nNeurons);
	//����������ѵ��������Ϣ
	if( !m_pNet->LoadFromFile(strFilePathName, dwCurPos) )
		return false;

	
	m_EditLoadTrainFile.SetWindowText(strFilePathName);
	return  true;
}


BEGIN_MESSAGE_MAP(CTestDialog, CDialog)
	//{{AFX_MSG_MAP(CTestDialog)
	ON_BN_CLICKED(IDC_BUTTON_REC, OnButtonRec)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_LOAD, OnButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_TEST_BROWSE, OnButtonTestBrowse)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestDialog message handlers

void CTestDialog::OnButtonRec() 
{
	CString strTrainFile;
	m_EditFile.GetWindowText(strTrainFile);//ȡ�ò����ļ�·����Ϣ
	if(strTrainFile.GetLength() == 0)
	{
		AfxMessageBox("��û��ѡ��ʶ���ļ������Ŀ¼!");
		return;
	}
	if(!m_bReadyToRec)//û��ָ��ѵ�������ļ�
	{
		AfxMessageBox("������ָ��һ��ѵ�������ļ��Ա㿪ʼʶ��!");
		return;
	}

	m_bInTest = true; //�����б�־
	( (CButton* )GetDlgItem(IDC_BUTTON_STOP) )->EnableWindow(true); //����ֹͣ��ť

	BOOL bDir = m_CheckDir.GetCheck(); //�Ƿ�ѡ��Ŀ¼����ѡ��

	if(!bDir) //����һ���ļ�
	{
		CString strFile;
		m_EditFile.GetWindowText(strFile);
		double dConfidence; //���Ŷ�
		int nClass = m_pNet->Recognize(strFile, m_rt, dConfidence); //ʶ��
		CString str;
		CString strRec;
		if(nClass >= 0)
		{
			strRec = m_vecClassNames[nClass];
			str.Format(" ʶ����: %s  ���Ŷ�: %%%d ", strRec, (int)(dConfidence*100));
		}
		else
			str = "ʶ��ʧ��!";
				
		MessageBox(str);	
	}
	else //��������Ŀ¼
	{
		WIN32_FIND_DATA findData;
		HANDLE hFindFile;
		CString str;

		//�򿪶Ի������û�ѡ��һ������ʶ�������ļ�
		CFileDialog saveDlg(FALSE, "txt", "", NULL, "txt Files(*.txt)|*.txt||", NULL);
		if(saveDlg.DoModal() != IDOK)
		{
			( (CButton* )GetDlgItem(IDC_BUTTON_STOP) )->EnableWindow(false); //����ֹͣ��ť
			m_bInTest = false;
			return;		
		}

		CString strRecFile = saveDlg.GetPathName(); //ʶ�����ļ�
		CFile file;
		if(!file.Open(strRecFile, CFile::modeCreate|CFile::modeWrite))
		{
			MessageBox(" create result file failed!");
			( (CButton* )GetDlgItem(IDC_BUTTON_STOP) )->EnableWindow(false); //����ֹͣ��ť
			m_bInTest = false;
			return;
		}
		
		CString strDir;
		m_EditFile.GetWindowText(strDir);; //ȡ�ò���Ŀ¼·����Ϣ
		if(strDir[strDir.GetLength() - 1] != '\\')
		{
			strDir += '\\';
		}
			
		
		CString szFileName = strDir;
		szFileName += "*.bmp"; //����Ŀ��ΪĿ¼�µ�����bmp�ļ�
		
		str = " \r\n  ʶ����Ϊ: \r\n \r\n";
		file.Write(str, str.GetLength());//дʶ�����ļ�
			
		int nTotal = 0; //������������
		int nNotRec = 0; //ʶ��ʧ�ܵĴ���
		double dConfidence; //���Ŷ�
		CImg ImageTmp;
		
		hFindFile = ::FindFirstFile(szFileName, &findData);
		if(hFindFile != INVALID_HANDLE_VALUE)
		{
			do
			{
				// ����Ϊ��.����Ŀ¼����Ŀ¼������Ϊ��..����Ŀ¼������һ��Ŀ¼
				if(findData.cFileName[0] == '.')
					continue;
				
				if(!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{// �ҵ��Ĳ���Ŀ¼�����ļ�
					str = strDir;
					str += findData.cFileName;
					
					int nClass = m_pNet->Recognize(str, m_rt, dConfidence);

					CString strRec = "ʶ��ʧ��!";
					if(nClass >= 0)
						strRec = m_vecClassNames[nClass];
					else
						nNotRec ++;		

					CString strToWrite;
					strToWrite.Format(" �ļ�: %s  �����: %s  ���Ŷ�: %%%d \r\n", 
						findData.cFileName, strRec, (int)(dConfidence*100));
					file.Write(strToWrite, strToWrite.GetLength()); //д��ʶ�����ļ�

					nTotal ++; //��������+1
				}
				
			}while(::FindNextFile(hFindFile, &findData));
			::FindClose(hFindFile);
		}
		
		str.Format("\r\n ʶ�����, ����: %d, ʧ��: %d \r\n", nTotal, nNotRec);
		file.Write(str, str.GetLength());
		file.Close();
		
		MessageBox(str);//��ʾ������Ϣ

		ShellExecute(m_hWnd, "open", "notepad.exe", strRecFile, NULL, SW_SHOW); //��ʶ�����ļ�
	}
	
	m_bInTest = false; //���Խ���
}

BOOL CTestDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CRect rt(0, 0, IMG_WIDTH, IMG_HEIGHT); //����һ����ͼ���С��ͬ�ľ���
	m_rt = rt;
	
	( (CButton* )GetDlgItem(IDC_BUTTON_STOP) )->EnableWindow(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTestDialog::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	if( m_bInTest )
	{
		AfxMessageBox( _T("���ڲ���(ʶ��)��!������ֹͣ�����ٹرմ���.") );
		return;
	}

	CDialog::OnClose();
}

void CTestDialog::OnButtonLoad() 
{
	// ���û�ѡ�����(ʶ��)���õ�ѵ�������ļ�(.net�ļ�)

	CString strFilePathName;
	CFileDialog dlg(true, "net", "", NULL, "Neuralnet Config Files(*.net)|*.net||", NULL); //TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի���
	
	if( dlg.DoModal() == IDOK )
		strFilePathName = dlg.GetPathName();

	//����ѵ�������ļ�
	if( LoadFromFile(strFilePathName) )
		m_bReadyToRec = true;

}

void CTestDialog::OnButtonTestBrowse() 
{
	int bDir = m_CheckDir.GetCheck();
	if(!bDir)
	{
		CFileDialog openfile(TRUE, "bmp", "", NULL, "Bitmap Files(*.bmp)|*.bmp||", NULL);
		if(openfile.DoModal() != IDOK)
			return;

		if(!m_Img.AttachFromFile(openfile.GetPathName()))
		{
			return;
		}

		m_EditFile.SetWindowText(openfile.GetPathName());

		CRect rect;	
		GetDlgItem(IDC_STATICIMG)->GetWindowRect(rect);
			
		ScreenToClient(&rect);
			
		InvalidateRect(&rect);
	}
	else
	{
		//ѡ��ѵ����������Ŀ¼
		CDirDialog dir;
		if(!dir.DoBrowse(*this))
		{
			return;
		}
		m_EditFile.SetWindowText(dir.GetPath());
		
	}
}

CPen Text_Pen(PS_SOLID, 1, RGB(255, 0, 0));

void CTestDialog::OnPaint() 
{
	CPaintDC dc(this);	
	
	if(m_Img.IsValidate())
	{
		CRect rect;
		
		GetDlgItem(IDC_STATICIMG)->GetWindowRect(rect);
		
		ScreenToClient(&rect);
		
		m_Img.Draw(&dc, rect);

		HGDIOBJ pOld = dc.SelectObject(Text_Pen);

		dc.MoveTo(rect.left + m_rt.left, rect.top + m_rt.top);
		dc.LineTo(rect.left + m_rt.right, rect.top + m_rt.top);
		dc.LineTo(rect.left + m_rt.right, rect.top + m_rt.bottom);
		dc.LineTo(rect.left + m_rt.left, rect.top + m_rt.bottom);
		dc.LineTo(rect.left + m_rt.left, rect.top + m_rt.top);


		dc.SelectObject(pOld);
	}


	// Do not call CDialog::OnPaint() for painting messages
}

void CTestDialog::OnButtonStop() 
{
	m_bInTest = false;

	( (CButton* )GetDlgItem(IDC_BUTTON_REC) )->EnableWindow(true);
	( (CButton* )GetDlgItem(IDC_BUTTON_STOP) )->EnableWindow(false);	
}
