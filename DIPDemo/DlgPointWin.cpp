// DlgPointWin.cpp : implementation file
//

#include "stdafx.h"
#include "DIPDemo.h"
#include "DlgPointWin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPointWin dialog


CDlgPointWin::CDlgPointWin(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPointWin::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPointWin)
	m_bLow = 0;
	m_bUp = 0;
	//}}AFX_DATA_INIT
}


void CDlgPointWin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPointWin)
	DDX_Text(pDX, IDC_EDIT_Low, m_bLow);
	DDV_MinMaxByte(pDX, m_bLow, 0, 255);
	DDX_Text(pDX, IDC_EDIT_Up, m_bUp);
	DDV_MinMaxByte(pDX, m_bUp, 0, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPointWin, CDialog)
	//{{AFX_MSG_MAP(CDlgPointWin)
	ON_EN_KILLFOCUS(IDC_EDIT_Low, OnKillfocusEDITLow)
	ON_EN_KILLFOCUS(IDC_EDIT_Up, OnKillfocusEDITUp)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPointWin message handlers

BOOL CDlgPointWin::OnInitDialog() 
{
	
	// ����Ĭ��OnInitDialog����
	CDialog::OnInitDialog();
	
	// ��ȡ����ֱ��ͼ�ı�ǩ
	CWnd* pWnd = GetDlgItem(IDC_COORD);
	
	// �����������¼�����Ч����
	pWnd->GetClientRect(m_MouseRect);
	pWnd->ClientToScreen(&m_MouseRect);
	
	CRect rect;
	GetClientRect(rect);
	ClientToScreen(&rect);
	
	m_MouseRect.top -= rect.top;
	m_MouseRect.left -= rect.left;
	
	// ���ý�������¼�����Ч����
	m_MouseRect.top += 25;
	m_MouseRect.left += 10;
	m_MouseRect.bottom = m_MouseRect.top + 255;
	m_MouseRect.right = m_MouseRect.left + 256;
	
	// ��ʼ���϶�״̬
	m_iIsDraging = 0;
	
	// ����TRUE
	return TRUE;
}

void CDlgPointWin::OnKillfocusEDITLow() 
{
	// ����
	UpdateData(TRUE);
	
	// �ж��Ƿ����޳�������
	if (m_bLow > m_bUp)
	{
		// ����
		BYTE bTemp = m_bLow;
		m_bLow = m_bUp;
		m_bUp = bTemp;
		
		// ����
		UpdateData(FALSE);
	}
	
	// �ػ�
	InvalidateRect(m_MouseRect, TRUE);
}

void CDlgPointWin::OnKillfocusEDITUp() 
{
	// ����
	UpdateData(TRUE);
	
	// �ж��Ƿ����޳�������
	if (m_bLow > m_bUp)
	{
		// ����
		BYTE bTemp = m_bLow;
		m_bLow = m_bUp;
		m_bUp = bTemp;
		
		// ����
		UpdateData(FALSE);
	}
	
	// �ػ�
	InvalidateRect(m_MouseRect, TRUE);
}

void CDlgPointWin::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// ���û�������������ʼ�϶�
	if(m_MouseRect.PtInRect(point))
	{
		if (point.x == (m_MouseRect.left + m_bLow))
		{
			
			// �����϶�״̬1���϶�����
			m_iIsDraging = 1;
			
			// ���Ĺ��
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		}
		else if (point.x == (m_MouseRect.left + m_bUp))
		{
			
			// �����϶�״̬Ϊ2���϶�����
			m_iIsDraging = 2;
			
			// ���Ĺ��
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		}
	}
	
	// Ĭ�ϵ��������������¼�
	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgPointWin::OnMouseMove(UINT nFlags, CPoint point) 
{
	// �жϵ�ǰ����Ƿ��ڻ�������
	if(m_MouseRect.PtInRect(point))
	{
		// �ж��Ƿ������϶�
		if (m_iIsDraging != 0)
		{
			// �ж������϶����޻�������
			if (m_iIsDraging == 1)
			{
				// �ж��Ƿ�����<����
				if (point.x - m_MouseRect.left < m_bUp)
				{
					// ��������
					m_bLow = (BYTE) (point.x - m_MouseRect.left);
				}
				else
				{
					// �����Ϲ����ޣ�����Ϊ����-1
					m_bLow = m_bUp - 1;
					
					// �������λ��
					point.x = m_MouseRect.left + m_bUp - 1;
				}
			}
			else
			{
				// �����϶�����
				
				// �ж��Ƿ�����>����
				if (point.x - m_MouseRect.left > m_bLow)
				{
					// ��������
					m_bUp = (BYTE) (point.x - m_MouseRect.left);
				}
				else
				{
					// �����Ϲ����ޣ�����Ϊ���ޣ�1
					m_bUp = m_bLow + 1;
					
					// �������λ��
					point.x = m_MouseRect.left + m_bLow + 1;
				}
			}
			
			// ���Ĺ��
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
			
			// ����
			UpdateData(FALSE);
			
			// �ػ�
			InvalidateRect(m_MouseRect, TRUE);
		}
		else if (point.x == (m_MouseRect.left + m_bLow) || point.x == (m_MouseRect.left + m_bUp))
		{
			// ���Ĺ��
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		}
	}
	
	// Ĭ������ƶ������¼�
	CDialog::OnMouseMove(nFlags, point);
}

void CDlgPointWin::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// ���û��ͷ�������ֹͣ�϶�
	if (m_iIsDraging != 0)
	{
		// �����϶�״̬
		m_iIsDraging = 0;
	}
	
	// Ĭ���ͷ������������¼�
	CDialog::OnLButtonUp(nFlags, point);
}

void CDlgPointWin::OnPaint() 
{
	// �ַ���
	CString str;
	
	// �豸������
	CPaintDC dc(this);
	
	// ��ȡ����������ı���
	CWnd* pWnd = GetDlgItem(IDC_COORD);
	
	// ָ��
	CDC* pDC = pWnd->GetDC();
	pWnd->Invalidate();
	pWnd->UpdateWindow();
	
	pDC->Rectangle(0,0,330,300);
	
	// �������ʶ���
	CPen* pPenRed = new CPen;
	
	// ��ɫ����
	pPenRed->CreatePen(PS_SOLID,2,RGB(255,0,0));
	
	// �������ʶ���
	CPen* pPenBlue = new CPen;
	
	// ��ɫ����
	pPenBlue->CreatePen(PS_SOLID,2,RGB(0,0, 255));
	
	// �������ʶ���
	CPen* pPenGreen = new CPen;
	
	// ��ɫ����
	pPenGreen->CreatePen(PS_DOT,1,RGB(0,255,0));
	
	// ѡ�е�ǰ��ɫ���ʣ���������ǰ�Ļ���
	CGdiObject* pOldPen = pDC->SelectObject(pPenRed);
	
	// ����������
	pDC->MoveTo(10,10);
	
	// ��ֱ��
	pDC->LineTo(10,280);
	
	// ˮƽ��
	pDC->LineTo(320,280);
	
	// д����
	str.Format("0");
	pDC->TextOut(10, 281, str);
	
	str.Format("255");
	pDC->TextOut(265, 281, str);
	pDC->TextOut(11, 25, str);
	
	// ����X���ͷ
	pDC->LineTo(315,275);
	pDC->MoveTo(320,280);
	pDC->LineTo(315,285);
	
	// ����X���ͷ
	pDC->MoveTo(10,10);
	pDC->LineTo(5,15);
	pDC->MoveTo(10,10);
	pDC->LineTo(15,15);
	
	// ���ĳ���ɫ����
	pDC->SelectObject(pPenGreen);	
	
	// ���ƴ���������
	pDC->MoveTo(m_bLow + 10, 25);
	pDC->LineTo(m_bLow + 10, 280);
	
	pDC->MoveTo(m_bUp + 10, 25);
	pDC->LineTo(m_bUp + 10, 280);
	
	// ���ĳ���ɫ����
	pDC->SelectObject(pPenBlue);	
	
	// ��������ֵ
	str.Format("(%d, %d)", m_bLow, m_bLow);
	pDC->TextOut(m_bLow + 10, 281 - m_bLow, str);
	str.Format("(%d, %d)", m_bUp, m_bUp);
	pDC->TextOut(m_bUp + 10, 281 - m_bUp, str);
	
	// �����û�ָ���Ĵ��ڣ�ע��ת������ϵ��
	pDC->MoveTo(10, 280);
	pDC->LineTo(m_bLow + 10, 280);
	pDC->LineTo(m_bLow + 10, 280 - m_bLow);
	pDC->LineTo(m_bUp + 10, 280 - m_bUp);
	pDC->LineTo(m_bUp + 10, 25);
	pDC->LineTo(265, 25);
	
	// �ָ���ǰ�Ļ���
	pDC->SelectObject(pOldPen);	
	
	// ���Ʊ�Ե
	pDC->MoveTo(10,25);
	pDC->LineTo(265,25);
	pDC->LineTo(265,280);
	
	// ɾ���µĻ���
	delete pPenRed;
	delete pPenBlue;
	delete pPenGreen;
	
}

void CDlgPointWin::OnOK() 
{
	
	// �ж��Ƿ����޳�������
	if (m_bLow > m_bUp)
	{
		// ����
		BYTE bTemp = m_bLow;
		m_bLow = m_bUp;
		m_bUp = bTemp;
	}
	
	// Ĭ�ϴ����¼�
	CDialog::OnOK();
}

