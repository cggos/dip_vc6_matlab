// TrainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DigitRec.h"
#include "TrainDlg.h"
#include "DirDialog.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTrainDlg dialog


CTrainDlg::CTrainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTrainDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTrainDlg)
	m_nMaxEpoch = 8000;
	m_dError = 0.001;
	m_dLearningRate = 0.1;
	m_nNeurons = 4;
	m_nTimeStep = 100;
	//}}AFX_DATA_INIT

	m_pNet = NULL;
	m_pTrack = NULL;
	m_bInTraining = false;
	
	
}

CTrainDlg::~CTrainDlg()
{
	if( m_pNet != NULL )
		delete m_pNet;
	
	if( m_pTrack != NULL )
		delete m_pTrack;
	
}


void CTrainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTrainDlg)
	DDX_Control(pDX, IDC_EDIT_FILE, m_EditFile);
	DDX_Control(pDX, IDC_EDIT_TRAIN_FILE, m_EditTrainFile);
	DDX_Control(pDX, IDC_STATIC_SHOW, m_StaticShow);
	DDX_Control(pDX, IDC_STATIC_ERROR, m_StaticErr);
	DDX_Text(pDX, IDC_EDIT_MAXEPOCH, m_nMaxEpoch);
	DDX_Text(pDX, IDC_EDIT_ERROR, m_dError);
	DDX_Text(pDX, IDC_EDIT_LEARNRATE, m_dLearningRate);
	DDX_Text(pDX, IDC_EDIT_HIDLYR, m_nNeurons);
	DDX_Text(pDX, IDC_EDIT_TIMESTEP, m_nTimeStep);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTrainDlg, CDialog)
	//{{AFX_MSG_MAP(CTrainDlg)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(ID_BUTTON_TRAIN, OnButtonTrain)
	ON_BN_CLICKED(ID_BUTTON_STOP, OnButtonStop)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_BN_CLICKED(IDC_BUTTON_SAVEAS, OnButtonSaveAs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrainDlg message handlers

BOOL CTrainDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_pTrack = new CValueTrack(&m_StaticErr);
	
	
	CRect rt(0, 0, IMG_WIDTH, IMG_HEIGHT); //����һ����ͼ���С��ͬ�ľ���
	m_rt = rt;


	// �����������
	int nInputs = 0;
	for(int jj=m_rt.top; jj<m_rt.bottom; jj+=RESAMPLE_LEN)
	{
		for(int ii=m_rt.left; ii<m_rt.right; ii+=RESAMPLE_LEN)
		{
			nInputs ++;
		}
	}
	m_data.Init(rt, nInputs);


	//���ð�ť��ʼ״̬
	( (CButton* )GetDlgItem(ID_BUTTON_TRAIN) )->EnableWindow(true);
	( (CButton* )GetDlgItem(ID_BUTTON_STOP) )->EnableWindow(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTrainDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	m_pTrack->Draw(); //���ƴ����������


	// Do not call CDialog::OnPaint() for painting messages
}

void CTrainDlg::UpdateTrainInfo()
{
	// ȡ�õ�ǰ�������������
	double dErr = m_pNet->GetError();
	m_pTrack->AddValue(dErr);
	m_pTrack->Draw();

	// ����ʱ���������Ϣ����ʾ�ڶԻ���ײ�
	int nEpoch = m_pNet->GetEpoch();
	CString strOut;
	strOut.Format("ʱ��:%d, ���:%f", nEpoch, dErr);
	m_StaticShow.SetWindowText(strOut);
}

void CTrainDlg::OnTimer(UINT nIDEvent) 
{
	
	if(m_bInTraining)
		UpdateTrainInfo(); // ��ʱ���ٻ��������Ϣ
	
	CDialog::OnTimer(nIDEvent);
}

void CTrainDlg::EnableControls(BOOL bEnable)
{
	( (CButton* )GetDlgItem(IDC_EDIT_TRAIN_FILE) )->EnableWindow(bEnable);
	( (CButton* )GetDlgItem(IDC_BUTTON_SAVEAS) )->EnableWindow(bEnable);
	( (CButton* )GetDlgItem(IDC_EDIT_FILE) )->EnableWindow(bEnable);
	( (CButton* )GetDlgItem(IDC_BUTTON_BROWSE) )->EnableWindow(bEnable);
	( (CButton* )GetDlgItem(IDC_EDIT_MAXEPOCH) )->EnableWindow(bEnable);
	( (CButton* )GetDlgItem(IDC_EDIT_ERROR) )->EnableWindow(bEnable);
	( (CButton* )GetDlgItem(IDC_EDIT_LEARNRATE) )->EnableWindow(bEnable);
	( (CButton* )GetDlgItem(IDC_EDIT_HIDLYR) )->EnableWindow(bEnable);
	( (CButton* )GetDlgItem(IDC_EDIT_TIMESTEP) )->EnableWindow(bEnable);
}

void CTrainDlg::OnButtonTrain() 
{
	//����UI״̬
	( (CButton* )GetDlgItem(ID_BUTTON_TRAIN) )->EnableWindow(false);
	( (CButton* )GetDlgItem(ID_BUTTON_STOP) )->EnableWindow(true);
	EnableControls(false);
	
	int nOuputs = m_data.GetClassNum(); //�����Ԫ��Ŀ(����1ofn����=�����)
	int nInputs = m_data.GetInputNum(); //���뵥Ԫ��Ŀ

	//���ö�ʱ����һ��ѵ����ʼ��ʱ���ٻ������
	m_nTimer = SetTimer(1, m_nTimeStep, NULL);

	//�������ò���
	UpdateData(true);

	//��������
	if( m_pNet != NULL )
		delete m_pNet;
	m_pNet = new CNeuralNet(nInputs, nOuputs, m_nNeurons);
	
	//�����û����������������
	m_pNet->SetMaxEpoch(m_nMaxEpoch);
	m_pNet->SetMinError(m_dError);
	m_pNet->SetLearningRate(m_dLearningRate);
				
	CString strOut; //�����Ϣ�ַ���

	//���ѵ������Ŀ¼
	CString strFilePath;
	m_EditFile.GetWindowText(strFilePath);
	if(strFilePath.GetLength()==0)
	{//û��ָ��ѵ������Ŀ¼���޷���ʼѵ��
		AfxMessageBox("û��ָ��ѵ��Ŀ¼��Ϣ���޷���ʼѵ��!");
		m_bInTraining = false;
		( (CButton* )GetDlgItem(ID_BUTTON_TRAIN) )->EnableWindow(true);
		( (CButton* )GetDlgItem(ID_BUTTON_STOP) )->EnableWindow(false);
		EnableControls(true);
		return;		
	}

	//����ѵ�������ļ�
	CString strTrainFilePath;
	m_EditTrainFile.GetWindowText(strTrainFilePath);
	if(strTrainFilePath.GetLength() == 0)
	{//û��ָ��ѵ�������ļ����޷���ʼѵ��
		AfxMessageBox("��ָ��һ�����ڱ���ѵ�������ѵ�������ļ��Ա㿪ʼѵ��!");
		m_bInTraining = false;
		( (CButton* )GetDlgItem(ID_BUTTON_TRAIN) )->EnableWindow(true);
		( (CButton* )GetDlgItem(ID_BUTTON_STOP) )->EnableWindow(false);
		EnableControls(true);
		return;		
	}
	if(!LoadFromFile(strTrainFilePath))
	{//�µ�ѵ��
		AfxMessageBox("û���ҵ�ƥ���ѵ�������ļ������ز㵥Ԫ��Ŀ�����仯!����ѵ������.");
		m_pTrack->Init();
	}
	else//�����ϴ�ѵ��
		m_pTrack->Draw();

	//�������ò���
	UpdateData(true);
	
	//�ڶԻ����������ʾ����ṹ��Ϣ
	int nNeurons = m_pNet->GetNumNeuronsPerLyr(); //���ز㵥Ԫ��Ŀ
	strOut.Format("inputs: %d outputs: %d nerons: %d",
		nInputs, nOuputs, nNeurons);
	SetWindowText(strOut);
		
	//ͳ��ѵ��ʱ�俪ʼ
	DWORD dwEsplise = ::GetTickCount();	
	m_bInTraining = true; //��ʼѵ��
	
	m_pNet->Train(m_data.GetInputSet(), m_data.GetOutputSet());//ѵ��
	
	//����ѵ���ɹ��ļ�
	CString strFilePathName;
	m_EditTrainFile.GetWindowText(strFilePathName);
	SaveToFile(strFilePathName);

	KillTimer(m_nTimer);//ɾ����ʱ��

	//������һ��ѵ��������
	UpdateTrainInfo();

	//ͳ��ѵ��ʱ�����
	dwEsplise = ::GetTickCount() - dwEsplise;

	strOut.Format(" ѵ����������ʱ��%d ��", dwEsplise/1000);
	AfxMessageBox(strOut);

	//����UI״̬
	( (CButton* )GetDlgItem(ID_BUTTON_TRAIN) )->EnableWindow(true);
	( (CButton* )GetDlgItem(ID_BUTTON_STOP) )->EnableWindow(false);
	EnableControls(true);
	
	m_bInTraining = false; //ѵ������
}



void CTrainDlg::OnButtonStop() 
{
	m_pNet->SetStopFlag(true); //����m_pNet��ֹͣѵ����־���Ӷ���ֹѵ������
	
	m_bInTraining = false;

	( (CButton* )GetDlgItem(ID_BUTTON_TRAIN) )->EnableWindow(true);
	( (CButton* )GetDlgItem(ID_BUTTON_STOP) )->EnableWindow(false);

	KillTimer(m_nTimer);
}

void CTrainDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if( m_bInTraining )
	{
		AfxMessageBox( _T("����ѵ����!������ֹͣѵ���ٹرմ���.") );
		return;
	}

	CDialog::OnClose();
}

void CTrainDlg::SetDir(CString strDir)
{
	// �趨����Ŀ¼�����Ϣ
	m_data.GetClassInfoFromDir(strDir);
	
	//��������ѵ������
	if(!m_data.CreateTrainingSetFromData()) 
	{
		MessageBox("����ѵ����ʱ���ִ���!");
		return;
	}
	
	// �ڱ༭����ʾ�û�ѡ����ѵ������Ŀ¼��Ϣ
	m_EditFile.SetWindowText(strDir);
}

void CTrainDlg::OnButtonBrowse() 
{
	//ѡ��ѵ����������Ŀ¼
	CDirDialog dir;
	if(!dir.DoBrowse(*this))
	{
		return;
	}
		
	SetDir(dir.GetPath());
	
}

bool CTrainDlg::SaveToFile(CString strFilePath)
{
	//��������������Ϣ
	CFile file;
	if(!file.Open(strFilePath, CFile::modeWrite|CFile::modeCreate))
		return false;
	
	//����������ṹ��Ϣ
	int nInput = m_data.GetInputNum(); //���뵥Ԫ��
	int nOutput = m_data.GetClassNum(); //����㵥Ԫ��
	int nNeurons = m_nNeurons; //���㵥Ԫ��
	file.Write(&nInput, sizeof(nInput));
	file.Write(&nOutput, sizeof(nOutput));
	file.Write(&nNeurons, sizeof(nNeurons));
	file.Write(&m_dError, sizeof(m_dError));
	file.Write(&m_dLearningRate, sizeof(m_dLearningRate));
	file.Write(&m_nMaxEpoch, sizeof(m_nMaxEpoch));

	//�������������Ϣ
	vector<CString> vecClassNames = m_data.GetVecClassNames();
	int i;
	int nLen;
	for(i = 0; i<vecClassNames.size(); i++)
	{
		nLen = vecClassNames[i].GetLength();
		file.Write( &nLen, sizeof(nLen) );
		file.Write( (LPCTSTR)vecClassNames[i], nLen );
	}

	//����ѵ��Ŀ¼�������ļ���Ϣ
	CString strPath;
	m_EditFile.GetWindowText(strPath);
	nLen = strPath.GetLength();
	file.Write( &nLen, sizeof(nLen) );
	file.Write( (LPCTSTR)strPath, strPath.GetLength() );

	file.Close();

	
	//����������ѵ��������Ϣ
	if( !m_pNet->SaveToFile(strFilePath, false) ) //�ڶ�������false��ʾ��׷��д��
		return false;
	

	return true;
}



bool CTrainDlg::LoadFromFile(CString strFilePathName, bool bLoadAll)
{
	//��������������Ϣ
	CFile file;
	if(!file.Open(strFilePathName, CFile::modeRead))
		return false;

	//��������ṹ��Ϣ
	int nInput, nOutput;
	file.Read(&nInput, sizeof(nInput));
	file.Read(&nOutput, sizeof(nOutput));
	file.Read(&m_nNeurons, sizeof(m_nNeurons));
	file.Read(&m_dError, sizeof(m_dError));
	file.Read(&m_dLearningRate, sizeof(m_dLearningRate));
	file.Read(&m_nMaxEpoch, sizeof(m_nMaxEpoch));

	

	int nLen;
	char* lpBuf;
	int i;
	for(i=0; i<nOutput; i++)
	{
		file.Read(&nLen, sizeof(nLen));
		lpBuf = new char[nLen+1];
		file.Read(lpBuf, nLen);
		lpBuf[nLen] = '\0';
		delete [] lpBuf;		
	}

	file.Read(&nLen, sizeof(nLen));
	lpBuf = new char[nLen+1];
	file.Read(lpBuf, nLen);
	lpBuf[nLen] = '\0';
	CString strTrainPathName;
	strTrainPathName = lpBuf;
	delete [] lpBuf;
	m_EditFile.SetWindowText(strTrainPathName);

	DWORD dwCurPos = file.GetPosition(); //��ǰ�ļ�ָ��λ��
	file.Close();
	if(!bLoadAll)
		return true;
	
	//����������ѵ��������Ϣ
	if( !m_pNet->LoadFromFile(strFilePathName, dwCurPos) )
		return false;

	
	m_EditTrainFile.SetWindowText(strFilePathName);
	return  true;
}







void CTrainDlg::OnButtonSaveAs()
{
	CFileDialog saveDlg(FALSE, "net", "", NULL, "Neuralnet Config Files(*.net)|*.net||", NULL);
	
	if(saveDlg.DoModal() == IDOK)
	{
		CString strFilePath = saveDlg.GetPathName();
		m_EditTrainFile.SetWindowText(strFilePath);	

		//�ڶ�������Ϊfalse��ʾֻ�������������Ϣ
		if(LoadFromFile(strFilePath, false))			
			UpdateData(false);

		m_pTrack->Init();
	}
	
}