// DIPDemoDoc.cpp : implementation of the CDIPDemoDoc class
//

#include "stdafx.h"
#include "DIPDemo.h"

#include "DIPDemoDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDIPDemoDoc

IMPLEMENT_DYNCREATE(CDIPDemoDoc, CDocument)

BEGIN_MESSAGE_MAP(CDIPDemoDoc, CDocument)
	//{{AFX_MSG_MAP(CDIPDemoDoc)
	ON_COMMAND(ID_FILE_REOPEN, OnFileReopen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDIPDemoDoc construction/destruction

CDIPDemoDoc::CDIPDemoDoc()
{
	m_sizeDoc = CSize(1,1);
	m_pPalette = NULL;
	// Ĭ�ϱ���ɫ����ɫ
	m_refColorBKG = 0x00808080;	
}

CDIPDemoDoc::~CDIPDemoDoc()
{
	// �жϵ�ɫ���Ƿ����
	if (m_pPalette != NULL)
	{
		// �����ɫ��
		delete m_pPalette;
	}
}

BOOL CDIPDemoDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CDIPDemoDoc diagnostics

#ifdef _DEBUG
void CDIPDemoDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDIPDemoDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDIPDemoDoc commands

BOOL CDIPDemoDoc::CanCloseFrame(CFrameWnd* pFrame) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDocument::CanCloseFrame(pFrame);
}

void CDIPDemoDoc::DeleteContents() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_pPalette != NULL)
		m_pPalette->DeleteObject();

	CDocument::DeleteContents();
}


void CDIPDemoDoc::Init()
{
	// ��ʼ��

	// ���ͼ����Ч��ֱ�ӷ���
	if (!m_Image.IsValidate())
	{
		return;
	}
	
	// �����ĵ���С
	m_sizeDoc = CSize(m_Image.GetWidthPixel(), m_Image.GetHeight());

	// �жϵ�ɫ���Ƿ�Ϊ��
	if (m_pPalette != NULL)
	{
		// ɾ����ɫ�����
		delete m_pPalette;

		// ���õ�ɫ��Ϊ��
		m_pPalette = NULL;
	}
	
	//���ǵ�ɫ����ʾģʽ����ֱ�ӷ���
	CClientDC dc(NULL);
	if((dc.GetDeviceCaps(RASTERCAPS) & RC_PALETTE) == 0)
		return;
		
	
	// �����µ�ɫ��
	m_pPalette = new CPalette;
	
	
	int nEntries = m_Image.GetColorTableEntriesNum();
	if (nEntries == 0)
		return;

	LOGPALETTE* lpPal = (LOGPALETTE*) new BYTE[sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY)*(nEntries-1))];
	
	// ���ð汾��
	lpPal->palVersion = 0x300;
	// ������ɫ��Ŀ
	lpPal->palNumEntries = (WORD)nEntries;

	RGBQUAD *pPal = (RGBQUAD*)(m_Image.GetColorTable());
	

	int i;
	// ������ɫ��
	for (i = 0; i < nEntries; i++)
	{
		// ��ȡ��ɫ����
	
		lpPal->palPalEntry[i].peRed = pPal[i].rgbRed;
				
		// ��ȡ��ɫ����
		lpPal->palPalEntry[i].peGreen = pPal[i].rgbGreen;
				
		// ��ȡ��ɫ����
		lpPal->palPalEntry[i].peBlue = pPal[i].rgbBlue;
				
		// ����λ
		lpPal->palPalEntry[i].peFlags = 0;
	}

	// �����߼���ɫ�崴����ɫ�壬������ָ��
	m_pPalette->CreatePalette(lpPal);
	
	delete lpPal;	
}


BOOL CDIPDemoDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	DeleteContents();
	
	// ���Ĺ����״
	BeginWaitCursor();
	
	// ��ȡͼ�񲢸��ӵ�m_Image��
	if(!m_Image.AttachFromFile(lpszPathName))
	{
		EndWaitCursor();
		AfxMessageBox("���ļ�ʱ����!��ȷ����ȷ��λͼ(*.bmp)�ļ����͡�");
		return FALSE;
	}

	// �ָ������״
	EndWaitCursor();

	// �ж϶�ȡ�ɹ���
	if (!m_Image.m_lpData)
	{
		// ʧ�ܣ����ܷ�BMP��ʽ
		CString strMsg;
		strMsg = "��ȡͼ��ʱ���������ǲ�֧�ָ����͵�ͼ���ļ���";
		
		// ��ʾ����
		MessageBox(NULL, strMsg, "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);
		
		// ����FALSE
		return FALSE;
	}
	
	Init(); //��ͼ��ĳߴ�͵�ɫ����Ϣ���г�ʼ��

	// �����ļ�����
	SetPathName(lpszPathName);

	// ������ǰm_Image��m_OImage
	m_OImage = m_Image;
	
	// ��ʼ���ͱ��ΪFALSE
	SetModifiedFlag(FALSE);
	

	// ����TRUE
	return TRUE;
}

void CDIPDemoDoc::OnFileReopen() 
{
 	// ���´�ͼ�񣬷��������޸�
 
	// �жϵ�ǰͼ���Ƿ��Ѿ����Ķ�
	if (IsModified())
	{
		// ��ʾ�û��ò�������ʧ���е�ǰ���޸�
		if (MessageBox(NULL, "���´�ͼ�񽫶�ʧ���иĶ����Ƿ������", "ϵͳ��ʾ", MB_ICONQUESTION | MB_YESNO) == IDNO)
		{
			// �û�ȡ��������ֱ�ӷ���
			return;
		}
	}

	CString strPathName;
  	
	// ��ȡ��ǰ�ļ�·��
 	strPathName = GetPathName();
 

 	// ���Ĺ����״
 	BeginWaitCursor();

	if(!m_Image.AttachFromFile(strPathName))
	{
		EndWaitCursor();
		AfxMessageBox("���ļ�ʱ����!��ȷ����ȷ��λͼ(*.bmp)�ļ����͡�");
		return;
	}
	
	// �ж϶�ȡ�ɹ���
	if (!m_Image.m_lpData)
	{
		// ʧ�ܣ����ܷ�BMP��ʽ
		CString strMsg;
		strMsg = "��ȡͼ��ʱ���������ǲ�֧�ָ����͵�ͼ���ļ���";
		
		// ��ʾ����
		MessageBox(NULL, strMsg, "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);
		
		// ����
		return;
	}
	
	Init(); //��ͼ��ĳߴ�͵�ɫ����Ϣ���г�ʼ��

 	// ��ʼ������ΪFALSE
 	SetModifiedFlag(FALSE);
 	
 	// ˢ��
 	UpdateAllViews(NULL);
  	
 	// �ָ������״
 	EndWaitCursor();
	

 	// ����
 	return;
 	
}

BOOL CDIPDemoDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	if(!m_Image.SaveToFile(lpszPathName))
	{
		CString strMsg;
		strMsg = "�޷�����BMPͼ��";
		
		// ��ʾ����
		MessageBox(NULL, strMsg, "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);
		return FALSE;
	}
	

	// �ָ������״
	EndWaitCursor();
	
	// �����ͱ��ΪFALSE
	SetModifiedFlag(FALSE);

	return TRUE;
}






