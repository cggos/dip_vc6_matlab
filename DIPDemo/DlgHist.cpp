// DlgHist.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DIPDemo.h"
#include "DlgHist.h"


// CDlgHist �Ի���

IMPLEMENT_DYNAMIC(CDlgHist, CDialog)

CDlgHist::CDlgHist(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgHist::IDD, pParent)
	, m_pdHist(NULL)
{

}

CDlgHist::~CDlgHist()
{
	
}

void CDlgHist::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgHist, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDlgHist ��Ϣ�������

void CDlgHist::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
	// �ַ���
	CString str;

	// ��ȡ��������Ŀؼ�
	CWnd* pWnd = GetDlgItem(IDC_HIST);

	// ָ��
	CDC* pDC = pWnd->GetDC();
	pWnd->Invalidate();
	pWnd->UpdateWindow();
	
	pDC->Rectangle(0,0,320,250);
	
	// �������ʶ���
	CPen* pPenRed = new CPen;
	
	// ��ɫ����
	pPenRed->CreatePen(PS_SOLID,1,RGB(255,0,0));
	
	// �������ʶ���
	CPen* pPenBlue = new CPen;
	
	// ��ɫ����
	pPenBlue->CreatePen(PS_SOLID,1,RGB(0,0, 255));
	
	// �������ʶ���
	CPen* pPenGreen = new CPen;
	
	// ��ɫ����
	pPenGreen->CreatePen(PS_DOT,1,RGB(0,255,0));
	
	// ѡ�е�ǰ��ɫ���ʣ���������ǰ�Ļ���
	CGdiObject* pOldPen = pDC->SelectObject(pPenRed);
	
	// ����������
	pDC->MoveTo(10,10);
	
	// ��ֱ��
	pDC->LineTo(10,240);
	
	// ˮƽ��
	pDC->LineTo(310,240);
	
	// дX��̶�ֵ
	str.Format("0");
	pDC->TextOut(10, 243, str);
	str.Format("50");
	pDC->TextOut(60, 243, str);
	str.Format("100");
	pDC->TextOut(110, 243, str);
	str.Format("150");
	pDC->TextOut(160, 243, str);
	str.Format("200");
	pDC->TextOut(210, 243, str);
	str.Format("255");
	pDC->TextOut(260, 243, str);
	
	int i;

	// ����X��̶�
	for (i = 0; i < 256; i += 5)
	{
		if ((i & 1) == 0)
		{
			// 10�ı���
			pDC->MoveTo(i + 10, 240);
			pDC->LineTo(i + 10, 234);
		}
		else
		{
			// 5�ı���
			pDC->MoveTo(i + 10, 240);
			pDC->LineTo(i + 10, 236);
		}
	}
	
	// ����X���ͷ
	pDC->MoveTo(305,235);
	pDC->LineTo(310,240);
	pDC->LineTo(305,245);
	
	// ����X���ͷ
	pDC->MoveTo(10,10);
	pDC->LineTo(5,15);
	pDC->MoveTo(10,10);
	pDC->LineTo(15,15);

	double maxHist = 0;

	for (i = 0; i < 256; i++)
	{
		maxHist = max(maxHist, m_pdHist[i]);
	}

	pDC->SelectObject(pPenBlue);

	for (i = 0; i < 256; i++)
	{
		pDC->MoveTo(i + 10, 240);
		pDC->LineTo(i + 10, int(240 - m_pdHist[i] / maxHist * 240));
	};
	
	// �ָ���ǰ�Ļ���
	pDC->SelectObject(pOldPen);	
	
	// ɾ���µĻ���
	delete pPenRed;
	delete pPenBlue;
	delete pPenGreen;
}
