// DigitRecView.cpp : implementation of the CDigitRecView class
//

#include "stdafx.h"
#include "DigitRec.h"

#include "DigitRecDoc.h"
#include "DigitRecView.h"
#include "TrainDlg.h"
#include "TestDialog.h"
#include "preprocessDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDigitRecView

IMPLEMENT_DYNCREATE(CDigitRecView, CScrollView)

BEGIN_MESSAGE_MAP(CDigitRecView, CScrollView)
	//{{AFX_MSG_MAP(CDigitRecView)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(IDM_TRAIN, OnTrain)
	ON_COMMAND(IDM_REC, OnRec)
	ON_COMMAND(IDM_PREPROCESS, OnPreprocess)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_UPDATE_COMMAND_UI(IDM_PREPROCESS, OnUpdatePreprocess)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_FILE_ROTATE, OnFileRotate)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()


//����ָ���ļ����µ�bmpͼ���ļ���Ŀ
int GetFileNum(CString strDir)
{
	int nBMPFile = 0;

	CString strToFind = strDir; 
	strToFind += "*.bmp";
		
	WIN32_FIND_DATA findData;
	HANDLE hFindFile;
	
	hFindFile = ::FindFirstFile(strToFind, &findData);

	if(hFindFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			if(findData.cFileName[0] == '.')
				continue;
				
			if(!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				nBMPFile++;	//�ҵ�1��BMP�ļ�	
		}
		while(::FindNextFile(hFindFile, &findData));
			
		::FindClose(hFindFile);
	}
	else
		return 0;

	return nBMPFile;
}	






/////////////////////////////////////////////////////////////////////////////
// CDigitRecView construction/destruction
/**********************
CDigitRecView::CDigitRecView()
���캯��
**********************/
CDigitRecView::CDigitRecView()
{
	for(int i=0; i<NUM_TRAINING; i++)
	{
		m_aryTrainingTag[i] = i % 10;
	}

}

CDigitRecView::~CDigitRecView()
{
}

BOOL CDigitRecView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	


	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDigitRecView drawing

void CDigitRecView::OnDraw(CDC* pDC)
{
	CDigitRecDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	if(m_bOut)
		{
			m_OutOCRImg.Draw(pDC);

			for(int i=0; i<m_vecRT.size(); i++)
			{
				RECT rt = m_vecRT[i];

				
				pDC->MoveTo(rt.left, rt.top);
				pDC->LineTo(rt.right, rt.top);
				
				pDC->MoveTo(rt.right, rt.top);
				pDC->LineTo(rt.right, rt.bottom);

				pDC->MoveTo(rt.right, rt.bottom);
				pDC->LineTo(rt.left, rt.bottom);

				pDC->MoveTo(rt.left, rt.bottom);
				pDC->LineTo(rt.left, rt.top);

			}
			
		}
		else
			m_OCRImg.Draw(pDC);


		
	
}

/////////////////////////////////////////////////////////////////////////////
// CDigitRecView printing

BOOL CDigitRecView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDigitRecView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDigitRecView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CDigitRecView diagnostics

#ifdef _DEBUG
void CDigitRecView::AssertValid() const
{
	CView::AssertValid();
}

void CDigitRecView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDigitRecDoc* CDigitRecView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDigitRecDoc)));
	return (CDigitRecDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDigitRecView message handlers

void CDigitRecView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	
	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	
	if(m_OCRImg.IsValidate())
	{
		sizeTotal.cx = m_OCRImg.GetWidthPixel();
		sizeTotal.cy = m_OCRImg.GetHeight();
	}
	SetScrollSizes(MM_TEXT, sizeTotal);

}

void CDigitRecView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default


	CView::OnLButtonDown(nFlags, point);
}

void CDigitRecView::OnTrain() 
{
	//����Ԥ�趨�Ի������û�����һЩ��Ҫ��ѵ��������Ϣ
	CTrainDlg trainDlg;
	trainDlg.DoModal();	
}

void CDigitRecView::OnRec() 
{
	CTestDialog testDlg;
	testDlg.DoModal(); //�������ԶԻ���
}

BOOL CDigitRecView::ReadClassLabels(CString strFilePath)
{
	FILE *fp;
	fp = fopen(strFilePath, "r");	
	if(fp == NULL)
	{
		AfxMessageBox("�޷���ָ�����ļ���");
		return FALSE;
	}

	int i = 0;
	char ch;
	while((ch=fgetc(fp))!=EOF) //�����ַ�ֱ���ļ�����
	{
		if(ch == ' ') //�����ո�
			continue;
		m_aryTrainingTag[i] = ch-48; //ת��Ϊ�������ǩ
		i++;
	}

	fclose(fp);

	return TRUE;
}

void CDigitRecView::SaveToTrainDIR(CString strTrainDIR)
{
	CString strFilePath = strTrainDIR; //ѵ������Ŀ¼
	CreateDirectory(strFilePath, NULL); 
	int nFileIndex; //ָ���Ǹ���ĵڼ����ļ�
	int i, j;
	int iNew, jNew;

	//��ѵ���������ű������ѵ������Ŀ¼
	int nSize = m_vecRT.size();
	for(int nRT=0; nRT<nSize; nRT++)
	{
		strFilePath = strTrainDIR;
		RECT rt = m_vecRT[nRT];

		int nHeight = (rt.bottom-1) - (rt.top+1) + 1;
		int nWidth = (rt.right-1) - (rt.left+1) + 1;

		COCRImageProcess imgToWrite;
		imgToWrite = m_OCRImg;//����ռ�
		imgToWrite.ImResize(nHeight, nWidth); //���·���ռ�

		// �������������е����ص�ͼ�� img ��
		iNew = 0;
		jNew = 0;
		for(i=rt.top+1; i<rt.bottom; i++) //�����ڽ���ʱ������һ�����أ�ȡ����ʱ����һ������
		{
			jNew = 0;
			for(j=rt.left+1; j<rt.right; j++)
			{
				int nGray = m_OutOCRImg.GetGray(j, i);
				imgToWrite.SetPixel( jNew, iNew, RGB(nGray, nGray, nGray) );
				jNew ++;
			}// for j

			iNew ++;
		}// for i
		

		int nClass = m_aryTrainingTag[nRT];
		CString strDIR; //���Ŀ¼
		strDIR.Format("\\%d\\", nClass);
		strFilePath += strDIR;

		//���û�����Ŀ¼�����Ƚ���֮
		CreateDirectory(strFilePath, NULL);

		//ͬһ���Ŀ¼���ļ����ս���˳����
		nFileIndex = GetFileNum(strFilePath);
		CString strFile; //�ļ���
		strFile.Format("%d.bmp", nFileIndex);
		strFilePath += strFile;

		// ��Ԥ������ͼ��д��ѵ������Ŀ¼
		imgToWrite.SaveToFile(strFilePath);

	}// for nRT
}

void CDigitRecView::SaveToTestDIR(CString strTestDIR)
{
	CString strFilePath = strTestDIR+"\\"; //��������Ŀ¼
	CreateDirectory(strFilePath, NULL); 
	int nFileIndex; //ָ���Ǹ���ĵڼ����ļ�
	int i, j;
	int iNew, jNew;

	//���������������������Ŀ¼
	int nSize = m_vecRT.size();
	for(int nRT=0; nRT<nSize; nRT++)
	{
		strFilePath = strTestDIR+"\\";;
		RECT rt = m_vecRT[nRT];

		int nHeight = (rt.bottom-1) - (rt.top+1) + 1;
		int nWidth = (rt.right-1) - (rt.left+1) + 1;

		COCRImageProcess imgToWrite;
		imgToWrite = m_OCRImg;//����ռ�
		imgToWrite.ImResize(nHeight, nWidth); //���·���ռ�

		// �������������е����ص�ͼ�� img ��
		iNew = 0;
		jNew = 0;
		for(i=rt.top+1; i<rt.bottom; i++) //�����ڽ���ʱ������һ�����أ�ȡ����ʱ����һ������
		{
			jNew = 0;
			for(j=rt.left+1; j<rt.right; j++)
			{
				int nGray = m_OutOCRImg.GetGray(j, i);
				imgToWrite.SetPixel( jNew, iNew, RGB(nGray, nGray, nGray) );
				jNew ++;
			}// for j

			iNew ++;
		}// for i
		
		//���û�����Ŀ¼�����Ƚ���֮
		CreateDirectory(strFilePath, NULL);

		//Ŀ¼���ļ����ս���˳����
		nFileIndex = GetFileNum(strFilePath);
		CString strFile; //�ļ���
		strFile.Format("%d.bmp", nFileIndex);
		strFilePath += strFile;

		// ��Ԥ������ͼ��д���������Ŀ¼
		imgToWrite.SaveToFile(strFilePath);

	}// for nRT
}


void CDigitRecView::OnPreprocess() 
{
	//ȡ��Ԥ������صĲ���
	int nThres; //��ֵ������ֵ
	int nUpper, nLower; //ȥ����ɢ����ʱ���ϡ����ޣ�����nUpper��С��nLower�ı���Ϊ�����˳���
	CString strLabelFilePath; // ����ǩ�ļ���·����Ϣ
	CString strDIR; //����ѵ�������ķ���Ŀ¼
	bool bTrain; //�Ƿ���ѵ�����ϵ�Ԥ����

	//����Ĭ��ֵ
	nThres = 145;
	nLower = 15;
	nUpper = 1500;

	CPreprocessDlg dlgPara;
	if( dlgPara.DoModal() != IDOK)
		return;
	else
	{//�ӶԻ�����ȡ���û�������
		nThres = dlgPara.m_nThres; //��ֵ����ֵ
		nUpper = dlgPara.m_nUpper; //ȥ������ʱ����ͨ��������
		nLower = dlgPara.m_nLower; //ȥ������ʱ����ͨ��������
		strLabelFilePath = dlgPara.m_strClassLabel; //ѵ�������������ǩ�ļ�
		strDIR = dlgPara.m_strTrainFile; //ѵ������Լ���Ŀ¼
		bTrain = dlgPara.bTrain; //ѵ��ģʽ���ǲ���ģʽ
	}

	// ������ǩ�ļ���ȡ����ǩ��Ϣ
	if(bTrain)
		ReadClassLabels(strLabelFilePath);

	
	BeginWaitCursor();

	// ͼ��Ԥ����
	COCRImageProcess OCRImgBk = m_OCRImg; //ԭʼͼ�񱸷�
	m_OutOCRImg = m_OCRImg;

	
	m_OCRImg.Threshold(&m_OutOCRImg, nThres); //��ֵ������
	m_OCRImg = m_OutOCRImg;

	m_OCRImg.DelScatterNoise(&m_OutOCRImg, nLower, nUpper);//ȥ����ɢ������
	m_OCRImg = m_OutOCRImg;		

	m_OCRImg.SlopeAdjust(&m_OutOCRImg); //��б�ȵ���
	m_OCRImg = m_OutOCRImg;
	
	m_vecRT = m_OCRImg.ObjectSegment(); //�ַ��ָ�
		
	m_OutOCRImg.ImResize(2*m_OCRImg.GetHeight(), 2*m_OCRImg.GetWidthPixel());//����Ŀ��ͼ�񣬷�ֹ��һ���󳬳���Χ
	m_OCRImg.ObjectNorm(&m_OutOCRImg, IMG_WIDTH, IMG_HEIGHT, m_vecRT); //��һ��Ϊͳһ��С
	m_OCRImg = m_OutOCRImg;

	m_OCRImg.ObjectAlign(&m_OutOCRImg, m_vecRT); //�ַ�����
	
	
	EndWaitCursor();
	m_OCRImg = OCRImgBk; //��ԭԭʼͼ��

	m_bOut = 1; //�л�����ʾ���ͼ���ģʽ
	Invalidate();

	if(bTrain)//�Ƿ���ѵ��ģʽ
		SaveToTrainDIR(strDIR); //������ѵ������Ŀ¼
	else
		SaveToTestDIR(strDIR); //����������Ŀ¼
}



void CDigitRecView::OnFileOpen() 
{
	// ���ļ��Ի���
	char szFilter[] =
		"BMP Files (*.bmp)|*.bmp|All Files (*.*)|*.*||";
 
	CFileDialog openfile(TRUE, NULL, NULL,  OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	if(openfile.DoModal() != IDOK)
		return;


	if(!m_OCRImg.AttachFromFile(openfile.GetPathName()))
	{
		MessageBox(" ����ʧ�ܣ�");
		return;
	}

	
	// �ڱ�������ʾ�ļ�·��
	CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pFrame->SetWindowText(openfile.GetPathName());

	m_bOut = 0;

	CSize sizeTotal;

	sizeTotal.cx = m_OCRImg.GetWidthPixel();
	sizeTotal.cy = m_OCRImg.GetHeight();
	SetScrollSizes(MM_TEXT, sizeTotal);



	InvalidateRect(NULL);		
}





void CDigitRecView::OnUpdatePreprocess(CCmdUI* pCmdUI) 
{
	if(!m_OCRImg.IsValidate())	
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}



void CDigitRecView::OnFileSaveAs() 
{
	if(!m_OCRImg.IsValidate())
		return;
	
	CFileDialog savefile(FALSE);
	if(!savefile.DoModal())
		return;
	
	
	if(m_bOut)
	{
		if(!m_OutOCRImg.SaveToFile(savefile.GetPathName()))
			MessageBox(" �ļ�����ʧ��");
	}
	else
	{
		if(!m_OCRImg.SaveToFile(savefile.GetPathName()))
			MessageBox(" �ļ�����ʧ��");
	}		
}



void CDigitRecView::OnFileRotate() 
{
	// ����ǰ��̨ͼ��
	m_bOut = !m_bOut;
	Invalidate();

}
