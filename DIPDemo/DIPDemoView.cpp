// DIPDemoView.cpp : implementation of the CDIPDemoView class
//

#include "stdafx.h"
#include "DIPDemo.h"

#include "DIPDemoDoc.h"
#include "DIPDemoView.h"
#include "mainfrm.h"

#include "DlgLinerPara.h"
#include "DlgPointThre.h"
#include "DlgPointWin.h"
#include "DlgPointThre.h"
#include "DlgGeoTran.h"
#include "DlgGeoZoom.h"
#include "DlgGeoRota.h"
#include "DlgProjRestore.h"
#include "DlgSmooth.h"
#include "DlgMidFilter.h"
#include "DlgSharpThre.h"
#include "DlgColor.h"


#include "DlgHist.h"
#include "DlgLinTrans.h"
#include "DlgLog.h"
#include "DlgGamma.h"
#include "DlgWndTran.h"
#include "DlgGrayScaling.h"
#include "DlgFreqGaussBRF.h"
#include "DlgFreqGaussHPF.h"
#include "DlgFreqGaussLPF.h"
#include "DlgFreqIdealLPF.h"
#include "DlgEdgeSobelPrewitt.h"
#include "DlgHough.h"

#include "DlgMorph.h"
#include "cDlgMorphErosion.h"
#include "cDlgMorphDilation.h"
#include "cDlgMorphOpen.h"
#include "cDlgMorphClose.h"







#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDIPDemoView

IMPLEMENT_DYNCREATE(CDIPDemoView, CScrollView)

BEGIN_MESSAGE_MAP(CDIPDemoView, CScrollView)
	//{{AFX_MSG_MAP(CDIPDemoView)
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_GEOM_TRAN, OnGeomTran)
	ON_COMMAND(ID_GEOM_MIRV, OnGeomMirv)
	ON_COMMAND(ID_GEOM_MIRH, OnGeomMirh)
	ON_COMMAND(ID_GEOM_ROTA, OnGeomRota)
	ON_COMMAND(ID_GEOM_TRPO, OnGeomTrpo)
	ON_COMMAND(ID_GEOM_ZOOM, OnGeomZoom)
	ON_COMMAND(ID_POINT_EQUA, OnPointEqua)
	ON_COMMAND(ID_POINT_LINER, OnPointLiner)
	ON_COMMAND(ID_POINT_WIND, OnPointWind)
	ON_COMMAND(ID_VIEW_INTENSITY, OnViewIntensity)
	ON_COMMAND(ID_FREQ_FOUR, OnFreqFour)
	ON_COMMAND(ID_POINT_THRE, OnPointThre)
	ON_COMMAND(ID_ENHA_SMOOTH, OnEnhaSmooth)
	ON_COMMAND(ID_ENHA_MidianF, OnENHAMidianF)
	ON_COMMAND(ID_ENHA_SHARP, OnEnhaSharp)
	ON_COMMAND(ID_ENHA_GRADSHARP, OnEnhaGradsharp)
	ON_COMMAND(ID_MORPH_EROSION, OnMorphErosion)
	ON_COMMAND(ID_MORPH_DILATION, OnMorphDilation)
	ON_COMMAND(ID_MORPH_OPEN, OnMorphOpen)
	ON_COMMAND(ID_MORPH_CLOSE, OnMorphClose)
	ON_COMMAND(ID_MORPH_THINING, OnMorphThining)
	ON_COMMAND(ID_GEOM_CALI, OnGeomCali)
	ON_COMMAND(ID_FILE_CLEAN, OnFileClean)
	ON_COMMAND(ID_COLOR_CMY2RGB, OnColorCmy2rgb)
	ON_COMMAND(ID_COLOR_RGB2HSI, OnColorRgb2hsi)
	ON_COMMAND(ID_COLOR_HSI2RGB, OnColorHsi2rgb)
	ON_COMMAND(ID_COLOR_RGB2HSV, OnColorRgb2hsv)
	ON_COMMAND(ID_COLOR_HSV2RGB, OnColorHsv2rgb)
	ON_COMMAND(ID_COLOR_RGB2YUV, OnColorRgb2yuv)
	ON_COMMAND(ID_COLOR_YUV2RGB, OnColorYuv2rgb)
	ON_COMMAND(ID_COLOR_RGB2YIQ, OnColorRgb2yiq)
	ON_COMMAND(ID_COLOR_YIQ2RGB, OnColorYiq2rgb)
	ON_COMMAND(ID_ENHA_AdaptMidianF, OnENHAAdaptMidianF)
	ON_COMMAND(ID_ENHA_HIGHENHA, OnEnhaHighenha)
	ON_COMMAND(ID_REC_TEMPLATE, OnRecTemplate)
	ON_COMMAND(ID_FEA_GRAYMAT, OnFeaGraymat)
	ON_COMMAND(ID_MORPH_TRACE, OnMorphTrace)
	ON_COMMAND(ID_FILE_REVERSE, OnFileReverse)
	ON_COMMAND(ID_MORPH_FILLRGN, OnMorphFillrgn)
	ON_COMMAND(ID_MORPH_LABELCONNRGN, OnMorphLabelconnrgn)
	ON_COMMAND(ID_MORPH_PIXEL, OnMorphPixel)
	ON_COMMAND(ID_MORPH_CONVEX, OnMorphConvex)
	ON_COMMAND(ID_MORPH_GRAYDILATE, OnMorphGraydilate)
	ON_COMMAND(ID_MORPH_GRAYERODE, OnMorphGrayerode)
	ON_COMMAND(ID_MORPH_GRAYOPEN, OnMorphGrayopen)
	ON_COMMAND(ID_MORPH_GRAYCLOSE, OnMorphGrayclose)
	ON_COMMAND(ID_MORPH_TOPHAT, OnMorphTophat)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
	ON_COMMAND(ID_POINT_STDLIN, CDIPDemoView::OnPointStdlin)
	ON_COMMAND(ID_POINT_LOG, CDIPDemoView::OnPointLog)
	ON_COMMAND(ID_POINT_GAMMA, CDIPDemoView::OnPointGamma)
	ON_COMMAND(ID_POINT_HISTST, CDIPDemoView::OnPointHistst)
	ON_COMMAND(ID_FILE_ROTATE, CDIPDemoView::OnFileRotate)
	ON_COMMAND(ID_EDGE_ROBERT, CDIPDemoView::OnEdgeRobert)
	ON_COMMAND(ID_EDGE_SOBEL, CDIPDemoView::OnEdgeSobel)
	ON_COMMAND(ID_EDGE_PREWITT, CDIPDemoView::OnEdgePrewitt)
	ON_COMMAND(ID_EDGE_LOG, CDIPDemoView::OnEdgeLog)
	ON_COMMAND(ID_EDGE_AUTOTHRE, CDIPDemoView::OnEdgeAutothre)
	ON_COMMAND(ID_FREQ_IDEALLPF, CDIPDemoView::OnFreqIdeallpf)
	ON_COMMAND(ID_FREQ_GAUSSLPF, CDIPDemoView::OnFreqGausslpf)
	ON_COMMAND(ID_FREQ_GAUSSHPF, CDIPDemoView::OnFreqGausshpf)
	ON_COMMAND(ID_FREQ_LAPL, CDIPDemoView::OnFreqLapl)
	ON_COMMAND(ID_FREQ_GAUSSBR, CDIPDemoView::OnFreqGaussbr)
	ON_COMMAND(ID_EDGE_CANNY, CDIPDemoView::OnEdgeCanny)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_EDGE_REGION_GROW, CDIPDemoView::OnEdgeRegionGrow)
	ON_COMMAND(ID_EDGE_HOUGH, CDIPDemoView::OnEdgeHough)


	ON_COMMAND(ID_FILE_CONVERTGRAY, CDIPDemoView::OnFileConvertgray)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDIPDemoView construction/destruction

CDIPDemoView::CDIPDemoView()
: m_bLBtnDblClkSrv(0)
{
	// TODO: add construction code here

}

CDIPDemoView::~CDIPDemoView()
{
}

BOOL CDIPDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDIPDemoView drawing

void CDIPDemoView::OnDraw(CDC* pDC)
{
	
	// ��ʾ�ȴ����
	BeginWaitCursor();
	
	// ��ȡ�ĵ�
	CDIPDemoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(pDoc->m_Image.IsValidate())
	{
		CPalette* pOldPalette;
		CPalette* pPalette = pDoc->GetDocPalette();

		if(pPalette!=NULL)
		{
			pOldPalette = pDC->SelectPalette(pPalette, FALSE);
			pDC->RealizePalette(); //����ϵͳ��ɫ��
		}	


		pDoc->m_Image.Draw(pDC); //����ͼ��
	
		if(pPalette!=NULL)
			pDC->SelectPalette(pOldPalette, FALSE);
	}
	// �ָ��������
	EndWaitCursor();
	
}

/////////////////////////////////////////////////////////////////////////////
// CDIPDemoView printing

BOOL CDIPDemoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// ������ҳ��Ϊһ
	pInfo->SetMaxPage(1);

	return DoPreparePrinting(pInfo);
}

void CDIPDemoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDIPDemoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CDIPDemoView diagnostics

#ifdef _DEBUG
void CDIPDemoView::AssertValid() const
{
	CView::AssertValid();
}

void CDIPDemoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDIPDemoDoc* CDIPDemoView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDIPDemoDoc)));
	return (CDIPDemoDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDIPDemoView message handlers


BOOL CDIPDemoView::OnEraseBkgnd(CDC* pDC) 
{
	// ��Ҫ��Ϊ�������Ӵ���Ĭ�ϵı���ɫ
	// ����ɫ���ĵ���Ա����m_refColorBKGָ��

	// ��ȡ�ĵ�
	CDIPDemoDoc* pDoc = GetDocument();

	// ����һ��Brush
	CBrush brush(pDoc->m_refColorBKG);                                              
                                                                                  
	// ������ǰ��Brush
	CBrush* pOldBrush = pDC->SelectObject(&brush);
	
	// ��ȡ�ػ�����
	CRect rectClip;
	pDC->GetClipBox(&rectClip);
	
	// �ػ�
	pDC->PatBlt(rectClip.left, rectClip.top, rectClip.Width(), rectClip.Height(), PATCOPY);

	// �ָ���ǰ��Brush
	pDC->SelectObject(pOldBrush);                                                  

	// ����
	return TRUE;

}



void CDIPDemoView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	
}

void CDIPDemoView::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType) 
{
	CScrollView::OnInitialUpdate();
	ASSERT(GetDocument() != NULL);
	
	SetScrollSizes(MM_TEXT, GetDocument()->GetDocSize());
}

void CDIPDemoView::OnActivateView(BOOL bActivate, CView* pActivateView,
					CView* pDeactiveView)
{
	CScrollView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	if (bActivate)
	{
		ASSERT(pActivateView == this);
		Invalidate(); //�������ͼʱ�����ػ�
	}
}









void CDIPDemoView::OnViewIntensity() 
{
	// �鿴��ǰͼ��Ҷ�ֱ��ͼ
	
	// ��ȡ�ĵ�
	CDIPDemoDoc* pDoc = GetDocument();
	
	// �������
	CImgProcess imgInput = pDoc->m_Image;

	// ���ͼ���ǻҶ�ͼ
	if (imgInput.m_pBMIH->biBitCount!=8)
	{
		AfxMessageBox("����8-bpp�Ҷ�ͼ���޷�����");
		return;
	}

	// ֱ��ͼ����
	double hist[256];

	// ����æ״̬
	BeginWaitCursor();

	// ��ȡֱ��ͼ����
	imgInput.GenHist(hist);

	CDlgHist dlg;
	dlg.m_pdHist = hist;

	if (dlg.DoModal() != IDOK)
	{
		// ����
		return;
	}

	// ������ͼ
	pDoc->UpdateAllViews(NULL);
	
	// �ָ����
	EndWaitCursor();
	
}

//////////////////////////////////////////////////////////////////////////////////////
//  ͼ�������

void CDIPDemoView::OnPointLiner() 
{
	// ���Ա任
	
	// ��ȡ�ĵ�
	CDIPDemoDoc* pDoc = GetDocument();
		
	// �������
	CImgProcess imgInput = pDoc->m_Image;

	// ���ͼ���ǻҶ�ͼ
	if (imgInput.m_pBMIH->biBitCount!=8)
	{
		AfxMessageBox("����8-bpp�Ҷ�ͼ���޷�����");
		return;
	}

	// �����Ի���
	CDlgLinerPara dlgPara;
	
	// ���Ա任��б��
	double fA;
	
	// ���Ա任�Ľؾ�
	double fB;
	
	// ��ʼ������ֵ
	dlgPara.m_fA = 2.0;
	dlgPara.m_fB = -128.0;
	
	// ��ʾ�Ի�����ʾ�û��趨����
	if (dlgPara.DoModal() != IDOK)
	{
		// ����
		return;
	}
	
	// ��ȡ�û��趨�Ĳ���
	fA = dlgPara.m_fA;
	fB = dlgPara.m_fB;
		
	// ���Ĺ����״
	BeginWaitCursor();
	
	// �������ʱ����	
	CImgProcess imgOutput = imgInput;

	// ����LinTran()�����������Ա任
	imgInput.LinTran(&imgOutput, fA, fB);

	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;
	
	// ��������
	pDoc->SetModifiedFlag(TRUE);
	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);

	// �ָ����
	EndWaitCursor();
	
}

void CDIPDemoView::OnPointThre()
{
	// ��ֵ�任
	
	// ��ȡ�ĵ�
	CDIPDemoDoc* pDoc = GetDocument();
	
	// �����Ի���
	CDlgPointThre  dlgPara;
	
	// ��ֵ
	BYTE	bThre;

	// �������
	CImgProcess imgInput = pDoc->m_Image;

	// ���ͼ���ǻҶ�ͼ
	if (imgInput.m_pBMIH->biBitCount!=8)
	{
		AfxMessageBox("����8-bpp�Ҷ�ͼ���޷�����");
		return;
	}

	// ��ʼ������ֵ
	dlgPara.m_bThre = 128;
	
	// ��ʾ�Ի�����ʾ�û��趨��ֵ
	if (dlgPara.DoModal() != IDOK)
	{
		// ����
		return;
	}
	
	// ��ȡ�û��趨����ֵ
	bThre = dlgPara.m_bThre;
	
	// ���Ĺ����״
	BeginWaitCursor();

	// �������ʱ����
	CImgProcess imgOutput = imgInput;

	// ����Threshold����������ֵ�任
	imgInput.Threshold(&imgOutput, bThre);

	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;
	
	// ��������
	pDoc->SetModifiedFlag(TRUE);
	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);
	
	// �ָ����
	EndWaitCursor();
}

void CDIPDemoView::OnPointWind() 
{
	// TODO: �ڴ���������������
	
	CDIPDemoDoc* pDoc = GetDocument();

	// �������
	CImgProcess imgInput = pDoc->m_Image;

	// ���ͼ���ǻҶ�ͼ
	if (imgInput.m_pBMIH->biBitCount!=8)
	{
		AfxMessageBox("����8-bpp�Ҷ�ͼ���޷�����");
		return;
	}

	// ֱ��ͼ����
	double dpHist[256];

	// �������ʱ����
	
	CImgProcess imgOutput = imgInput;

	// ��ȡֱ��ͼ����
	imgInput.GenHist(dpHist);

	// ������������Ի���
	CDlgWndTran dlg;
	dlg.m_bLower = 0;
	dlg.m_bUpper = 255;
	dlg.m_dHist = dpHist;
	
	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	// ���ڱ任
	imgInput.WindowTran(&imgOutput, dlg.m_bLower, dlg.m_bUpper);
	
	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;
	
	pDoc->SetModifiedFlag(true);
	
	pDoc->UpdateAllViews(NULL);
}

void CDIPDemoView::OnPointEqua() 
{
	// TODO: �ڴ���������������
	
	CDIPDemoDoc* pDoc = GetDocument();

	// �������
	CImgProcess imgInput = pDoc->m_Image;
	
	// ���ͼ���ǻҶ�ͼ
	if (imgInput.m_pBMIH->biBitCount!=8)
	{
		AfxMessageBox("����8-bpp�Ҷ�ͼ���޷�����");
		return;
	}

	// �������ʱ����	
	CImgProcess imgOutput = imgInput;

	// ֱ��ͼ���⻯
	imgInput.Histeq(&imgOutput);
	
	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;
	
	pDoc->SetModifiedFlag(true);
	
	pDoc->UpdateAllViews(NULL);
}

//////////////////////////////////////////////////////////////////////////////////////
//  ͼ�񼸺α任
//
void CDIPDemoView::OnGeomTran() 
{
	// ��ȡ�ĵ�
	CDIPDemoDoc* pDoc = GetDocument();	

	// �������
	CImgProcess imgInput = pDoc->m_Image;

	// ���ͼ���ǻҶ�ͼ
	if (imgInput.m_pBMIH->biBitCount!=8)
	{
		AfxMessageBox("����8-bpp�Ҷ�ͼ���޷�����");
		return;
	}

	LONG lXOffset;
	LONG lYOffset;
	
	// �����Ի���
	CDlgGeoTran dlgPara;
	
	// ��ʼ������ֵ
	dlgPara.m_XOffset = 100;
	dlgPara.m_YOffset = 100;
	
	// ��ʾ�Ի�����ʾ�û��趨����
	if (dlgPara.DoModal() != IDOK)
	{
		// ����
		return;
	}
	
	// ��ȡ�û��趨�Ĳ���
	lXOffset = dlgPara.m_XOffset;
	lYOffset = dlgPara.m_YOffset;
	
	// ���Ĺ����״
	BeginWaitCursor();

	// �������ʱ����	
	CImgProcess imgOutput = imgInput;

	// ����ImMove()����ʵ��ͼ��ƽ��
	imgInput.ImMove(&imgOutput, lXOffset, lYOffset);

	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;
	
	// ��������
	pDoc->SetModifiedFlag(TRUE);
	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);

	// �ָ����
	EndWaitCursor();
}

void CDIPDemoView::OnGeomMirv() 
{
	// ��ֱ����

	// ��ȡ�ĵ�
	CDIPDemoDoc* pDoc = GetDocument();	

	// �������
	CImgProcess imgInput = pDoc->m_Image;

	// ���ͼ���ǻҶ�ͼ
	if (imgInput.m_pBMIH->biBitCount!=8)
	{
		AfxMessageBox("����8-bpp�Ҷ�ͼ���޷�����");
		return;
	}
	
	// ���Ĺ����״
	BeginWaitCursor();

	// �������ʱ����	
	CImgProcess imgOutput = imgInput;

	// ����VerMirror()����ʵ��ͼ����ֱ����
	imgInput.VerMirror(&imgOutput);

	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;
	
	// ��������
	pDoc->SetModifiedFlag(TRUE);
	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);

	// �ָ����
	EndWaitCursor();

}

void CDIPDemoView::OnGeomMirh() 
{
	// ˮƽ����

	// ��ȡ�ĵ�
	CDIPDemoDoc* pDoc = GetDocument();	

	// �������
	CImgProcess imgInput = pDoc->m_Image;

	// ���ͼ���ǻҶ�ͼ
	if (imgInput.m_pBMIH->biBitCount!=8)
	{
		AfxMessageBox("����8-bpp�Ҷ�ͼ���޷�����");
		return;
	}
	
	// ���Ĺ����״
	BeginWaitCursor();

	// �������ʱ����	
	CImgProcess imgOutput = imgInput;

	// ����HorMirror()����ʵ��ͼ��ˮƽ����
	imgInput.HorMirror(&imgOutput);

	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;
	
	// ��������
	pDoc->SetModifiedFlag(TRUE);
	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);

	// �ָ����
	EndWaitCursor();

}

void CDIPDemoView::OnGeomTrpo() 
{
	// ͼ��ת��

	// ��ȡ�ĵ�
	CDIPDemoDoc* pDoc = GetDocument();	

	// �������
	CImgProcess imgInput = pDoc->m_Image;

	// ���ͼ���ǻҶ�ͼ
	if (imgInput.m_pBMIH->biBitCount!=8)
	{
		AfxMessageBox("����8-bpp�Ҷ�ͼ���޷�����");
		return;
	}

	// ���Ĺ����״
	BeginWaitCursor();

	// �������ʱ����	
	CImgProcess imgOutput = imgInput;

	// ����Transpose()����ʵ��ͼ��ת��
	imgInput.Transpose(&imgOutput);

	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;
	
	// ��������
	pDoc->SetModifiedFlag(TRUE);
	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);

	// �ָ����
	EndWaitCursor();
}

void CDIPDemoView::OnGeomZoom() 
{
	// ͼ������

	// ��ȡ�ĵ�
	CDIPDemoDoc* pDoc = GetDocument();
	

	// �������
	CImgProcess imgInput = pDoc->m_Image;

	// ���ͼ���ǻҶ�ͼ
	if (imgInput.m_pBMIH->biBitCount!=8)
	{
		AfxMessageBox("����8-bpp�Ҷ�ͼ���޷�����");
		return;
	}

	// ���ű���
	float fZoomRatio;
		
	// �����Ի���
	CDlgGeoZoom dlgPara;
	
	// ��ʼ������ֵ
	dlgPara.m_Zoom = 0.5;

	
	// ��ʾ�Ի�����ʾ�û��趨����
	if (dlgPara.DoModal() != IDOK)
	{
		// ����
		return;
	}
	
	// ��ȡ�û��趨�Ĳ���
	fZoomRatio = dlgPara.m_Zoom;
	


	// ���Ĺ����״
	BeginWaitCursor();

	// �������ʱ����	
	CImgProcess imgOutput = imgInput;
	
	// ����Scale()����ʵ��ͼ������
	imgInput.Scale(&imgOutput, fZoomRatio);

	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;
	
	// ��������
	pDoc->SetModifiedFlag(TRUE);
	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);

	// �ָ����
	EndWaitCursor();
}

void CDIPDemoView::OnGeomRota() 
{
	// ͼ����ת

	// ��ȡ�ĵ�
	CDIPDemoDoc* pDoc = GetDocument();
	

	// �������
	CImgProcess imgInput = pDoc->m_Image;

	// ���ͼ���ǻҶ�ͼ
	if (imgInput.m_pBMIH->biBitCount!=8)
	{
		AfxMessageBox("����8-bpp�Ҷ�ͼ���޷�����");
		return;
	}

	// ��ת�Ƕ�
	int iRotateAngle;
	
	// �����Ի���
	CDlgGeoRota dlgPara;
	
	// ��ʼ������ֵ
	dlgPara.m_iRotateAngle = 30;
	
	// ��ʾ�Ի�����ʾ�û��趨��ת�Ƕ�
	if (dlgPara.DoModal() != IDOK)
	{
		// ����
		return;
	}
	
	// ��ȡ�û��趨�Ĳ���
	iRotateAngle = dlgPara.m_iRotateAngle;

	
	// ���Ĺ����״
	BeginWaitCursor();

	// �������ʱ����	
	CImgProcess imgOutput = imgInput;
	
	// ����Rotate()����ʵ��ͼ����ת
	imgInput.Rotate(&imgOutput, iRotateAngle);

	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;
	
	// ��������
	pDoc->SetModifiedFlag(TRUE);
	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);
	// �ָ����
	EndWaitCursor();
}

void CDIPDemoView::OnGeomCali() 
{
	// ͶӰУ��

	// ��ȡ�ĵ�
	CDIPDemoDoc* pDoc = GetDocument();	

	// �������
	CImgProcess imgInput = pDoc->m_Image;

	// ���ͼ���ǻҶ�ͼ
	if (imgInput.m_pBMIH->biBitCount!=8)
	{
		AfxMessageBox("����8-bpp�Ҷ�ͼ���޷�����");
		return;
	}

	// �������ʱ����	
	CImgProcess imgOutput = imgInput;
	
	CPoint pBasePts[4]; //��׼ͼ���׼������
	CPoint pSrcPts[4]; //����ͼ���׼������

	// �����Ի���
	CDlgProjRestore dlgPara;
		
	// ��ʼ������ͼ���׼��
	dlgPara.m_xPt1 = 108;
	dlgPara.m_yPt1 = 135;

	dlgPara.m_xPt2 = 274;
	dlgPara.m_yPt2 = 51;

	dlgPara.m_xPt3 = 295;
	dlgPara.m_yPt3 = 119;

	dlgPara.m_xPt4 = 158;
	dlgPara.m_yPt4 = 248;

	// ��ʼ����׼ͼ���׼��
	dlgPara.m_xBPt1 = 0;
	dlgPara.m_yBPt1 = 0;

	dlgPara.m_xBPt2 = 310;
	dlgPara.m_yBPt2 = 0;

	dlgPara.m_xBPt3 = 310;
	dlgPara.m_yBPt3 = 100;

	dlgPara.m_xBPt4 = 0;
	dlgPara.m_yBPt4 = 100;
	
	// ��ʾ�Ի�����ʾ�û��趨����
	if (dlgPara.DoModal() != IDOK)
	{
		// ����
		return;
	}
	
	// ��ȡ�û��趨�Ĳ���
	pBasePts[0] = CPoint(dlgPara.m_xBPt1, dlgPara.m_yBPt1);
	pBasePts[1] = CPoint(dlgPara.m_xBPt2, dlgPara.m_yBPt2);
	pBasePts[2] = CPoint(dlgPara.m_xBPt3, dlgPara.m_yBPt3);
	pBasePts[3] = CPoint(dlgPara.m_xBPt4, dlgPara.m_yBPt4);
	
	pSrcPts[0] = CPoint(dlgPara.m_xPt1, dlgPara.m_yPt1);
	pSrcPts[1] = CPoint(dlgPara.m_xPt2, dlgPara.m_yPt2);
	pSrcPts[2] = CPoint(dlgPara.m_xPt3, dlgPara.m_yPt3);
	pSrcPts[3] = CPoint(dlgPara.m_xPt4, dlgPara.m_yPt4);
	
	// ���Ĺ����״
	BeginWaitCursor();

	// ����ImProjRestore()����ʵ��ͼ��ͶӰУ��
	imgInput.ImProjRestore(&imgOutput, pBasePts, pSrcPts, 1);//ʹ��˫���Բ�ֵ��ͶӰУ��

	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;
	
	// ��������
	pDoc->SetModifiedFlag(TRUE);
	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);
	
	// �ָ����
	EndWaitCursor();
}



//////////////////////////////////////////////////////////////////////////////////////
//  ͼ����ǿ
//
void CDIPDemoView::OnEnhaSmooth() 
{
	// ͼ��ƽ��

	// ��ȡ�ĵ�
	CDIPDemoDoc* pDoc = GetDocument();
	
	// �������
	CImgProcess imgInput = pDoc->m_Image;

	// ���ͼ���ǻҶ�ͼ
	if (imgInput.m_pBMIH->biBitCount!=8)
	{
		AfxMessageBox("����8-bpp�Ҷ�ͼ���޷�����");
		return;
	}

	// �������ʱ����	
	CImgProcess imgOutput = imgInput;
	
	int	nTempH;	// ģ��߶�
	int	nTempW; // ģ����
	FLOAT fTempC;// ģ��ϵ��
	int		nTempMX;// ģ������Ԫ��X����
	int		nTempMY;// ģ������Ԫ��Y����
	
	// ģ��Ԫ�����鸳��ֵ��Ĭ��Ϊƽ��ģ�壩
	FLOAT	aValue[25] = {1.0, 1.0, 1.0, 0.0, 0.0, 
		1.0, 1.0, 1.0, 0.0, 0.0, 
		1.0, 1.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0, 0.0,};
	
	// �����Ի���
	CDlgSmooth dlgPara;
	
	// ��ʼ���Ի������ֵ
	dlgPara.m_intType = 0;
	dlgPara.m_iTempH  = 3;
	dlgPara.m_iTempW  = 3;
	dlgPara.m_iTempMX = 1;
	dlgPara.m_iTempMY = 1;
	dlgPara.m_fTempC  = (FLOAT) (1.0 / 9.0);
	dlgPara.m_fpArray = aValue;
	
	// ��ʾ�Ի�����ʾ�û��趨����
	if (dlgPara.DoModal() != IDOK)
	{
		// ����
		return;
	}
	
	// ��ȡ�û��趨�Ĳ���
	nTempH  = dlgPara.m_iTempH;
	nTempW  = dlgPara.m_iTempW;
	nTempMX = dlgPara.m_iTempMX;
	nTempMY = dlgPara.m_iTempMY;
	fTempC  = dlgPara.m_fTempC;

	
	// ���Ĺ����״
	BeginWaitCursor();
	
	// ����Template()����ʵ��ƽ���˲�
	imgInput.Template(&imgOutput, nTempH, nTempW, nTempMY, nTempMX, aValue, fTempC);

	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;
	
	// ��������
	pDoc->SetModifiedFlag(TRUE);
	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);
	// �ָ����
	EndWaitCursor();	
}

void CDIPDemoView::OnENHAMidianF() 
{
	// ��ֵ�˲�
	
	// ��ȡ�ĵ�
	CDIPDemoDoc* pDoc = GetDocument();
	
	// �������
	CImgProcess imgInput = pDoc->m_Image;

	// ���ͼ���ǻҶ�ͼ
	if (imgInput.m_pBMIH->biBitCount!=8)
	{
		AfxMessageBox("����8-bpp�Ҷ�ͼ���޷�����");
		return;
	}

	
	
	int nFilterH; // �˲����ĸ߶�
	int nFilterW; // �˲����Ŀ��
	int nFilterMX; // ����Ԫ�ص�X����
	int nFilterMY; // ����Ԫ�ص�Y����
	
	CDlgMidFilter dlgPara;// �����Ի���
	
	// ��ʼ������ֵ
	dlgPara.m_iFilterType = 0;
	dlgPara.m_iFilterH = 3;
	dlgPara.m_iFilterW = 1;
	dlgPara.m_iFilterMX = 0;
	dlgPara.m_iFilterMY = 1;
	
	// ��ʾ�Ի�����ʾ�û��趨����
	if (dlgPara.DoModal() != IDOK)
	{
		// ����
		return;
	}
	
	// ��ȡ�û����趨
	nFilterH = dlgPara.m_iFilterH;
	nFilterW = dlgPara.m_iFilterW;
	nFilterMX = dlgPara.m_iFilterMX;
	nFilterMY = dlgPara.m_iFilterMY;
	
	// ���Ĺ����״
	BeginWaitCursor();

	// �������ʱ����	
	CImgProcess imgOutput = imgInput;

	// ����MedianFilter()������ֵ�˲�
	imgInput.MedianFilter(&imgOutput, nFilterH, nFilterW, nFilterMY, nFilterMX);

	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;
	
	// ��������
	pDoc->SetModifiedFlag(TRUE);
	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);
	// �ָ����
	EndWaitCursor();
}

void CDIPDemoView::OnENHAAdaptMidianF() 
{
	// ����Ӧ��ֵ�˲�
	
	// ��ȡ�ĵ�
	CDIPDemoDoc* pDoc = GetDocument();
	
	// �������
	CImgProcess imgInput = pDoc->m_Image;

	// ���ͼ���ǻҶ�ͼ
	if (imgInput.m_pBMIH->biBitCount!=8)
	{
		AfxMessageBox("����8-bpp�Ҷ�ͼ���޷�����");
		return;
	}

	int nFilterH; // �˲����ĸ߶�
	int nFilterW; // �˲����Ŀ��
	int nFilterMX; // ����Ԫ�ص�X����
	int nFilterMY; // ����Ԫ�ص�Y����
	
	CDlgMidFilter dlgPara;// �����Ի���
	
	// ��ʼ������ֵ
	dlgPara.m_iFilterType = 0;
	dlgPara.m_iFilterH = 3;
	dlgPara.m_iFilterW = 1;
	dlgPara.m_iFilterMX = 0;
	dlgPara.m_iFilterMY = 1;
	
	// ��ʾ�Ի�����ʾ�û��趨����
	if (dlgPara.DoModal() != IDOK)
	{
		// ����
		return;
	}
	
	// ��ȡ�û����趨
	nFilterH = dlgPara.m_iFilterH;
	nFilterW = dlgPara.m_iFilterW;
	nFilterMX = dlgPara.m_iFilterMX;
	nFilterMY = dlgPara.m_iFilterMY;
	
	// ���Ĺ����״
	BeginWaitCursor();

	// �������ʱ����	
	CImgProcess imgOutput = imgInput;
	
	// ����AdaptiveMedianFilter()��������Ӧ��ֵ�˲�
	imgInput.AdaptiveMedianFilter(&imgOutput, nFilterH, nFilterW, nFilterMY, nFilterMX);

	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;
	
	// ��������
	pDoc->SetModifiedFlag(TRUE);
	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);
	// �ָ����
	EndWaitCursor();
}


void CDIPDemoView::OnEnhaGradsharp() 
{
	// Sobel�ݶ���
	
	// ��ȡ�ĵ�
	CDIPDemoDoc* pDoc = GetDocument();
	
	// �������
	CImgProcess imgInput = pDoc->m_Image;

	// ���ͼ���ǻҶ�ͼ
	if (imgInput.m_pBMIH->biBitCount!=8)
	{
		AfxMessageBox("����8-bpp�Ҷ�ͼ���޷�����");
		return;
	}

	// ���Ĺ����״
	BeginWaitCursor();

	// �������ʱ����	
	CImgProcess imgOutput = imgInput;
	
	// ����FilterSobel()��������Sobel�ݶ���
	imgInput.FilterSobel(&imgOutput);

	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;
	
	// ��������
	pDoc->SetModifiedFlag(TRUE);
	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);

	// �ָ����
	EndWaitCursor();
	
}

void CDIPDemoView::OnEnhaSharp() 
{
	// ������˹��
	
	// ��ȡ�ĵ�
	CDIPDemoDoc* pDoc = GetDocument();
	
	// �������
	CImgProcess imgInput = pDoc->m_Image;

	// ���ͼ���ǻҶ�ͼ
	if (imgInput.m_pBMIH->biBitCount!=8)
	{
		AfxMessageBox("����8-bpp�Ҷ�ͼ���޷�����");
		return;
	}

	// �������ʱ����	
	CImgProcess imgOutput = imgInput;
	
	int nTempH; // �˲����ĸ߶�
	int nTempW; // �˲����Ŀ��
	int nTempMX; // ����Ԫ�ص�X����
	int nTempMY; // ����Ԫ�ص�Y����
	FLOAT	fCoef;	// ģ��ϵ��
	
	// ���Ĺ����״
	BeginWaitCursor();
	
	// ����������˹ģ�����
	nTempW = 3;
	nTempH = 3;
	fCoef = 1.0;
	nTempMX = 1;
	nTempMY = 1;

	// ����Template()������������˹ģ����
	imgInput.Template(&imgOutput, nTempH, nTempW, nTempMY, nTempMX, Template_Laplacian2, fCoef);
	
	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;

	// ��������
	pDoc->SetModifiedFlag(TRUE);

	// ������ͼ
	pDoc->UpdateAllViews(NULL);
	
	
	// �ָ����
	EndWaitCursor();
	
}

void CDIPDemoView::OnEnhaHighenha() 
{
	// ����3*3��Laplacianģ��ĸ������˲�

	
	// ��ȡ�ĵ�
	CDIPDemoDoc* pDoc = GetDocument();
	
	// �������
	CImgProcess imgInput = pDoc->m_Image;

	// ���ͼ���ǻҶ�ͼ
	if (imgInput.m_pBMIH->biBitCount!=8)
	{
		AfxMessageBox("����8-bpp�Ҷ�ͼ���޷�����");
		return;
	}

	// ���Ĺ����״
	BeginWaitCursor();

	// �������ʱ����	
	CImgProcess imgOutput = imgInput;
	
	// ��ز����趨
	double dProportion = 1.8;
	int nTempH = 3;
	int nTempW = 3;
	int nTempMY = 1;
	int nTempMX = 1;
	// ����EnhanceFilter()�������и������˲�������3*3��Laplacianģ�壬���ϵ��Ϊ1.8
	imgInput.EnhanceFilter(&imgOutput, dProportion, nTempH, nTempW, nTempMY, nTempMX, Template_Laplacian2, 1);

	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;
	
	// ��������
	pDoc->SetModifiedFlag(TRUE);
	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);

	// �ָ����
	EndWaitCursor();	
}

//////////////////////////////////////////////////////////////////////////////////////
//  ͼ�������任
//

void CDIPDemoView::OnFreqFour() 
{	//���ٸ���Ҷ�任

	CDIPDemoDoc* pDoc = GetDocument();

	// �������
	CImgProcess imgInput = pDoc->m_Image;
	
	// ���ͼ���ǻҶ�ͼ
	if (imgInput.m_pBMIH->biBitCount!=8)
	{
		AfxMessageBox("����8-bpp�Ҷ�ͼ���޷�����");
		return;
	}

	BeginWaitCursor();

	// �������ʱ����
	CImgProcess imgOutput = imgInput;

	// ִ��FFT2
	imgInput.FFT2(&imgOutput,1);

	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;
	
	pDoc->SetModifiedFlag(true);
	
	pDoc->UpdateAllViews(NULL);
	
	EndWaitCursor();
}


//////////////////////////////////////////////////////////////////////////////////////
//  ��̬ѧ�任
//
void CDIPDemoView::OnMorphErosion() 
{	//��ʴ����

	// ��ȡ�ĵ�
	CDIPDemoDoc* pDoc = GetDocument();
	
	// ����ͼ��
	CImgProcess imgInput = pDoc->m_Image;
	
	// ���ͼ���Ƕ�ֵͼ
	if (!imgInput.IsBinaryImg())
	{
		AfxMessageBox("���Ƕ�ֵͼ���޷�����");
		return;
	}
		
	
	// �����Ի���
	CDlgMorph dlgPara;	
	
	// ��ʾ�Ի�����ʾ�û��趨�ṹԪ��
	if (dlgPara.DoModal() != IDOK)
	{
		// ����
		return;
	}
	
	int se[3][3]; //3*3�ṹԪ��

	//�����û������趨�ṹԪ��
	se[0][0]=dlgPara.m_se00;
	se[0][1]=dlgPara.m_se01;
	se[0][2]=dlgPara.m_se02;
	se[1][0]=dlgPara.m_se10;
	se[1][1]=dlgPara.m_se11;
	se[1][2]=dlgPara.m_se12;
	se[2][0]=dlgPara.m_se20;
	se[2][1]=dlgPara.m_se21;
	se[2][2]=dlgPara.m_se22;	
	
	
	// ���Ĺ����״
	BeginWaitCursor();

	// �������ʱ����	
	CImgProcess imgOutput = imgInput;

	//����Erode()ʵ��ͼ��ʴ
	imgInput.Erode(&imgOutput, se);
	
	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;
	
	pDoc->SetModifiedFlag(true);
	
	pDoc->UpdateAllViews(NULL);
		
	// �ָ����
	EndWaitCursor();	
}

void CDIPDemoView::OnMorphOpen() 
{	//������

	// ��ȡ�ĵ�
	CDIPDemoDoc* pDoc = GetDocument();
	
	// ����ͼ��
	CImgProcess imgInput = pDoc->m_Image;
	
	// ���ͼ���Ƕ�ֵͼ
	if (!imgInput.IsBinaryImg())
	{
		AfxMessageBox("���Ƕ�ֵͼ���޷�����");
		return;
	}
		
	
	// �����Ի���
	CDlgMorph dlgPara;	
	
	// ��ʾ�Ի�����ʾ�û��趨�ṹԪ��
	if (dlgPara.DoModal() != IDOK)
	{
		// ����
		return;
	}
	
	int se[3][3]; //3*3�ṹԪ��

	//�����û������趨�ṹԪ��
	se[0][0]=dlgPara.m_se00;
	se[0][1]=dlgPara.m_se01;
	se[0][2]=dlgPara.m_se02;
	se[1][0]=dlgPara.m_se10;
	se[1][1]=dlgPara.m_se11;
	se[1][2]=dlgPara.m_se12;
	se[2][0]=dlgPara.m_se20;
	se[2][1]=dlgPara.m_se21;
	se[2][2]=dlgPara.m_se22;	
	
	
	// ���Ĺ����״
	BeginWaitCursor();

	// �������ʱ����	
	CImgProcess imgOutput = imgInput;

	// ����Open()ʵ��ͼ��ʴ
	imgInput.Open(&imgOutput, se);
	
	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;
	
	pDoc->SetModifiedFlag(true);
	
	pDoc->UpdateAllViews(NULL);
		
	// �ָ����
	EndWaitCursor();	
	
	
}


void CDIPDemoView::OnMorphTrace() 
{	//�߽����
	

	// ��ȡ�ĵ�
	CDIPDemoDoc* pDoc = GetDocument();
	
	// ����ͼ��
	CImgProcess imgInput = pDoc->m_Image;
	
	// ���ͼ���Ƕ�ֵͼ
	if (!imgInput.IsBinaryImg())
	{
		AfxMessageBox("���Ƕ�ֵͼ���޷�����");
		return;
	}
		
	
	// ���Ĺ����״
	BeginWaitCursor();

	// �������ʱ����	
	CImgProcess imgOutput = imgInput;

	// ����TraceBoundary()����ʵ�ֱ߽����
	imgInput.TraceBoundary(&imgOutput);
	
	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;
	
	pDoc->SetModifiedFlag(true);
	
	pDoc->UpdateAllViews(NULL);
		
	// �ָ����
	EndWaitCursor();
}


void CDIPDemoView::OnMorphFillrgn() 
{	// ������������
	
	// ��ȡ�ĵ�
	CDIPDemoDoc* pDoc = GetDocument();
	
	// ����ͼ��
	CImgProcess imgInput = pDoc->m_Image;
	
	// ���ͼ���Ƕ�ֵͼ
	if (!imgInput.IsBinaryImg())
	{
		AfxMessageBox("���Ƕ�ֵͼ���޷�����");
		return;
	}
		
	// ���Ĺ����״
	BeginWaitCursor();

	// �������ʱ����	
	CImgProcess imgOutput = imgInput;

	// ��Ҫ����ʵ������������ӵ�����꣬����򵥵�ȡ���ӵ�Ϊͼ������ĵ�
	POINT ptStart;
	ptStart.x = imgInput.GetWidthPixel() / 2;
	ptStart.y = imgInput.GetHeight() / 2;

	// ����FillRgn()����ʵ���������
	imgInput.FillRgn(&imgOutput, ptStart);
	//POINT�Ͳ���ptStartΪ���ӵ㣬�����������е�����һ��
	
	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;
	
	pDoc->SetModifiedFlag(true);
	
	pDoc->UpdateAllViews(NULL);
		
	// �ָ����
	EndWaitCursor();
}

void CDIPDemoView::OnMorphLabelconnrgn() 
{	// ��ע��ͨ����
		
	// ��ȡ�ĵ�
	CDIPDemoDoc* pDoc = GetDocument();
	
	// ����ͼ��
	CImgProcess imgInput = pDoc->m_Image;
	
	// ���ͼ���Ƕ�ֵͼ
	if (!imgInput.IsBinaryImg())
	{
		AfxMessageBox("���Ƕ�ֵͼ���޷�����");
		return;
	}
		
	// ���Ĺ����״
	BeginWaitCursor();

	// �������ʱ����	
	CImgProcess imgOutput = imgInput;

	// ����LabelConnRgn()������ע��ͨ����
	imgInput.LabelConnRgn(&imgOutput);
		
	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;
	
	pDoc->SetModifiedFlag(true);
	
	pDoc->UpdateAllViews(NULL);
		
	// �ָ����
	EndWaitCursor();
}


void CDIPDemoView::OnMorphThining() 
{
	//ϸ��

	// ��ȡ�ĵ�
	CDIPDemoDoc* pDoc = GetDocument();
	
	// ����ͼ��
	CImgProcess imgInput = pDoc->m_Image;
	
	// ���ͼ���Ƕ�ֵͼ
	if (!imgInput.IsBinaryImg())
	{
		AfxMessageBox("���Ƕ�ֵͼ���޷�����");
		return;
	}

	// ���Ĺ����״
	BeginWaitCursor();

	// �������ʱ����	
	CImgProcess imgOutput = imgInput;

	// ����Thining()ʵ��ͼ��ϸ��
	imgOutput.Thining();
	
	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;
	
	pDoc->SetModifiedFlag(true);
	
	pDoc->UpdateAllViews(NULL);
		
	// �ָ����
	EndWaitCursor();
	
}


void CDIPDemoView::OnMorphPixel() 
{	//���ػ�

	// ��ȡ�ĵ�
	CDIPDemoDoc* pDoc = GetDocument();
	
	// ����ͼ��
	CImgProcess imgInput = pDoc->m_Image;
	
	// ���ͼ���Ƕ�ֵͼ
	if (!imgInput.IsBinaryImg())
	{
		AfxMessageBox("���Ƕ�ֵͼ���޷�����");
		return;
	}
		
	int nLowThres; //��ͨ�����ڴ���ֵ����ͨ�����˳�
	int nHighThres; //��ͨ�����ڴ���ֵ����ͨ�����˳�	

	// ���Ĺ����״
	BeginWaitCursor();

	// �������ʱ����	
	CImgProcess imgOutput = imgInput;

	//�����趨����ֵΪ10������ֵΪ100,��ͨ��<10����ͨ��>300����ͨ�����˳���֮��ı����ػ�
	nLowThres = 10;
	nHighThres = 300;
	
	// ����PixelImage()ʵ��ͼ��ϸ��
	imgInput.PixelImage(&imgOutput, nLowThres, nHighThres);
	
	
	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;
	
	pDoc->SetModifiedFlag(true);
	
	pDoc->UpdateAllViews(NULL);
		
	// �ָ����
	EndWaitCursor();
}


void CDIPDemoView::OnMorphConvex() 
{	//͹��
	
	// ��ȡ�ĵ�
	CDIPDemoDoc* pDoc = GetDocument();
	
	// ����ͼ��
	CImgProcess imgInput = pDoc->m_Image;
	
	// ���ͼ���Ƕ�ֵͼ
	if (!imgInput.IsBinaryImg())
	{
		AfxMessageBox("���Ƕ�ֵͼ���޷�����");
		return;
	}
		
	// ���Ĺ����״
	BeginWaitCursor();

	// �������ʱ����	
	CImgProcess imgOutput = imgInput;

	// ����Convex()��������͹��
	imgInput.Convex(&imgOutput, 1);
	//��2������Ϊ1��ʾ��Ҫ����͹�ǵ�����	

	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;
	
	pDoc->SetModifiedFlag(true);
	
	pDoc->UpdateAllViews(NULL);
		
	// �ָ����
	EndWaitCursor();
}

void CDIPDemoView::OnMorphClose() 
{
	//������

	// ��ȡ�ĵ�
	CDIPDemoDoc* pDoc = GetDocument();
	
	// ����ͼ��
	CImgProcess imgInput = pDoc->m_Image;
	
	// ���ͼ���Ƕ�ֵͼ
	if (!imgInput.IsBinaryImg())
	{
		AfxMessageBox("���Ƕ�ֵͼ���޷�����");
		return;
	}
		
	
	// �����Ի���
	CDlgMorph dlgPara;	
	
	// ��ʾ�Ի�����ʾ�û��趨�ṹԪ��
	if (dlgPara.DoModal() != IDOK)
	{
		// ����
		return;
	}
	
	int se[3][3]; //3*3�ṹԪ��

	//�����û������趨�ṹԪ��
	se[0][0]=dlgPara.m_se00;
	se[0][1]=dlgPara.m_se01;
	se[0][2]=dlgPara.m_se02;
	se[1][0]=dlgPara.m_se10;
	se[1][1]=dlgPara.m_se11;
	se[1][2]=dlgPara.m_se12;
	se[2][0]=dlgPara.m_se20;
	se[2][1]=dlgPara.m_se21;
	se[2][2]=dlgPara.m_se22;	
	
	
	// ���Ĺ����״
	BeginWaitCursor();

	// �������ʱ����	
	CImgProcess imgOutput = imgInput;

	// ����Close()ʵ��ͼ�������
	imgInput.Close(&imgOutput, se);
	
	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;
	
	pDoc->SetModifiedFlag(true);
	
	pDoc->UpdateAllViews(NULL);
		
	// �ָ����
	EndWaitCursor();
		
}

void CDIPDemoView::OnMorphDilation() 
{
	//��������

	// ��ȡ�ĵ�
	CDIPDemoDoc* pDoc = GetDocument();
	
	// ����ͼ��
	CImgProcess imgInput = pDoc->m_Image;
	
	// ���ͼ���Ƕ�ֵͼ
	if (!imgInput.IsBinaryImg())
	{
		AfxMessageBox("���Ƕ�ֵͼ���޷�����");
		return;
	}
		
	
	// �����Ի���
	CDlgMorph dlgPara;	
	
	// ��ʾ�Ի�����ʾ�û��趨�ṹԪ��
	if (dlgPara.DoModal() != IDOK)
	{
		// ����
		return;
	}
	
	int se[3][3]; //3*3�ṹԪ��

	//�����û������趨�ṹԪ��
	se[0][0]=dlgPara.m_se00;
	se[0][1]=dlgPara.m_se01;
	se[0][2]=dlgPara.m_se02;
	se[1][0]=dlgPara.m_se10;
	se[1][1]=dlgPara.m_se11;
	se[1][2]=dlgPara.m_se12;
	se[2][0]=dlgPara.m_se20;
	se[2][1]=dlgPara.m_se21;
	se[2][2]=dlgPara.m_se22;	
	
	
	// ���Ĺ����״
	BeginWaitCursor();

	// �������ʱ����	
	CImgProcess imgOutput = imgInput;

	//����Dilate()ʵ��ͼ������
	imgInput.Dilate(&imgOutput, se);
	
	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;
	
	pDoc->SetModifiedFlag(true);
	
	pDoc->UpdateAllViews(NULL);
		
	// �ָ����
	EndWaitCursor();	
	
}

void CDIPDemoView::OnMorphGraydilate() 
{	//�Ҷ�����

	// ��ȡ�ĵ�
	CDIPDemoDoc* pDoc = GetDocument();
	
	// ����ͼ��
	CImgProcess imgInput = pDoc->m_Image;
	
	// ���ͼ���ǻҶ�ͼ
	if (imgInput.m_pBMIH->biBitCount!=8)
	{
		AfxMessageBox("����8-bpp�Ҷ�ͼ���޷�����");
		return;
	}
		
	
	// �����Ի���
	CDlgMorph dlgPara;	
	
	// ��ʾ�Ի�����ʾ�û��趨�ṹԪ��
	if (dlgPara.DoModal() != IDOK)
	{
		// ����
		return;
	}
	
	int** se; //�ṹԪ��
	//��3*3�ĽṹԪ��Ϊ��
	se = new int* [3];
	for(int i=0; i<3; i++)
		se[i] = new int[3];
	//�����û������趨�ṹԪ��
	se[0][0]=dlgPara.m_se00;
	se[0][1]=dlgPara.m_se01;
	se[0][2]=dlgPara.m_se02;
	se[1][0]=dlgPara.m_se10;
	se[1][1]=dlgPara.m_se11;
	se[1][2]=dlgPara.m_se12;
	se[2][0]=dlgPara.m_se20;
	se[2][1]=dlgPara.m_se21;
	se[2][2]=dlgPara.m_se22;	
	
	
	// ���Ĺ����״
	BeginWaitCursor();

	// �������ʱ����	
	CImgProcess imgOutput = imgInput;

	//����GrayDilate()ʵ�ֻҶ�����
	imgInput.GrayDilate(&imgOutput, 3, 3, 1, 1, se);
	//����se��һ��int **ָ�룬��ʾ�ṹԪ�ء����������3*3ԭ��λ�����ĵĽṹԪ��

	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;
	
	pDoc->SetModifiedFlag(true);
	
	pDoc->UpdateAllViews(NULL);
		
	// �ָ����
	EndWaitCursor();
}


void CDIPDemoView::OnMorphGrayerode() 
{	//�Ҷȸ�ʴ

	// ��ȡ�ĵ�
	CDIPDemoDoc* pDoc = GetDocument();
	
	// ����ͼ��
	CImgProcess imgInput = pDoc->m_Image;
	
	// ���ͼ���ǻҶ�ͼ
	if (imgInput.m_pBMIH->biBitCount!=8)
	{
		AfxMessageBox("����8-bpp�Ҷ�ͼ���޷�����");
		return;
	}
		
	
	// �����Ի���
	CDlgMorph dlgPara;	
	
	// ��ʾ�Ի�����ʾ�û��趨�ṹԪ��
	if (dlgPara.DoModal() != IDOK)
	{
		// ����
		return;
	}
	
	int** se; //�ṹԪ��
	//��3*3�ĽṹԪ��Ϊ��
	se = new int* [3];
	for(int i=0; i<3; i++)
		se[i] = new int[3];
	//�����û������趨�ṹԪ��
	se[0][0]=dlgPara.m_se00;
	se[0][1]=dlgPara.m_se01;
	se[0][2]=dlgPara.m_se02;
	se[1][0]=dlgPara.m_se10;
	se[1][1]=dlgPara.m_se11;
	se[1][2]=dlgPara.m_se12;
	se[2][0]=dlgPara.m_se20;
	se[2][1]=dlgPara.m_se21;
	se[2][2]=dlgPara.m_se22;	
	
	
	// ���Ĺ����״
	BeginWaitCursor();

	// �������ʱ����	
	CImgProcess imgOutput = imgInput;

	//����GrayErode()ʵ�ֻҶȸ�ʴ
	imgInput.GrayErode(&imgOutput, 3, 3, 1, 1, se);
	//����se��һ��int **ָ�룬��ʾ�ṹԪ�ء����������3*3ԭ��λ�����ĵĽṹԪ��
	
	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;
	
	pDoc->SetModifiedFlag(true);
	
	pDoc->UpdateAllViews(NULL);
		
	// �ָ����
	EndWaitCursor();
}


void CDIPDemoView::OnMorphGrayopen() 
{	//�Ҷȿ�����

	// ��ȡ�ĵ�
	CDIPDemoDoc* pDoc = GetDocument();
	
	// ����ͼ��
	CImgProcess imgInput = pDoc->m_Image;
	
	// ���ͼ���ǻҶ�ͼ
	if (imgInput.m_pBMIH->biBitCount!=8)
	{
		AfxMessageBox("����8-bpp�Ҷ�ͼ���޷�����");
		return;
	}
		
	
	// �����Ի���
	CDlgMorph dlgPara;	
	
	// ��ʾ�Ի�����ʾ�û��趨�ṹԪ��
	if (dlgPara.DoModal() != IDOK)
	{
		// ����
		return;
	}
	
	int** se; //�ṹԪ��
	//��3*3�ĽṹԪ��Ϊ��
	se = new int* [3];
	for(int i=0; i<3; i++)
		se[i] = new int[3];
	//�����û������趨�ṹԪ��
	se[0][0]=dlgPara.m_se00;
	se[0][1]=dlgPara.m_se01;
	se[0][2]=dlgPara.m_se02;
	se[1][0]=dlgPara.m_se10;
	se[1][1]=dlgPara.m_se11;
	se[1][2]=dlgPara.m_se12;
	se[2][0]=dlgPara.m_se20;
	se[2][1]=dlgPara.m_se21;
	se[2][2]=dlgPara.m_se22;	
	
	
	// ���Ĺ����״
	BeginWaitCursor();

	// �������ʱ����	
	CImgProcess imgOutput = imgInput;

	//����GrayOpen()ʵ�ֻҶȿ�
	imgInput.GrayOpen(&imgOutput, 3, 3, 1, 1, se);
	//����se��һ��int **ָ�룬��ʾ�ṹԪ�ء����������3*3ԭ��λ�����ĵĽṹԪ��
	
	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;
	
	pDoc->SetModifiedFlag(true);
	
	pDoc->UpdateAllViews(NULL);
		
	// �ָ����
	EndWaitCursor();
	
}

void CDIPDemoView::OnMorphGrayclose() 
{	//�Ҷȱ�����

	// ��ȡ�ĵ�
	CDIPDemoDoc* pDoc = GetDocument();
	
	// ����ͼ��
	CImgProcess imgInput = pDoc->m_Image;
	
	// ���ͼ���ǻҶ�ͼ
	if (imgInput.m_pBMIH->biBitCount!=8)
	{
		AfxMessageBox("����8-bpp�Ҷ�ͼ���޷�����");
		return;
	}
		
	
	// �����Ի���
	CDlgMorph dlgPara;	
	
	// ��ʾ�Ի�����ʾ�û��趨�ṹԪ��
	if (dlgPara.DoModal() != IDOK)
	{
		// ����
		return;
	}
	
	int** se; //�ṹԪ��
	//��3*3�ĽṹԪ��Ϊ��
	se = new int* [3];
	for(int i=0; i<3; i++)
		se[i] = new int[3];
	//�����û������趨�ṹԪ��
	se[0][0]=dlgPara.m_se00;
	se[0][1]=dlgPara.m_se01;
	se[0][2]=dlgPara.m_se02;
	se[1][0]=dlgPara.m_se10;
	se[1][1]=dlgPara.m_se11;
	se[1][2]=dlgPara.m_se12;
	se[2][0]=dlgPara.m_se20;
	se[2][1]=dlgPara.m_se21;
	se[2][2]=dlgPara.m_se22;	
	
	
	// ���Ĺ����״
	BeginWaitCursor();

	// �������ʱ����	
	CImgProcess imgOutput = imgInput;

	//����GrayClose()ʵ�ֻҶȱ�
	imgInput.GrayClose(&imgOutput, 3, 3, 1, 1, se);
	//����se��һ��int **ָ�룬��ʾ�ṹԪ�ء����������3*3ԭ��λ�����ĵĽṹԪ��
	
	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;
	
	pDoc->SetModifiedFlag(true);
	
	pDoc->UpdateAllViews(NULL);
		
	// �ָ����
	EndWaitCursor();
	
}

void CDIPDemoView::OnMorphTophat() 
{	//��ñ�任

	// ��ȡ�ĵ�
	CDIPDemoDoc* pDoc = GetDocument();
	
	// ����ͼ��
	CImgProcess imgInput = pDoc->m_Image;
	
	// ���ͼ���ǻҶ�ͼ
	if (imgInput.m_pBMIH->biBitCount!=8)
	{
		AfxMessageBox("����8-bpp�Ҷ�ͼ���޷�����");
		return;
	}
		
	
	// �����Ի���
	CDlgMorph dlgPara;	
	
	// ��ʾ�Ի�����ʾ�û��趨�ṹԪ��
	if (dlgPara.DoModal() != IDOK)
	{
		// ����
		return;
	}
	
	int** se; //�ṹԪ��
	//��3*3�ĽṹԪ��Ϊ��
	se = new int* [3];
	for(int i=0; i<3; i++)
		se[i] = new int[3];
	//�����û������趨�ṹԪ��
	se[0][0]=dlgPara.m_se00;
	se[0][1]=dlgPara.m_se01;
	se[0][2]=dlgPara.m_se02;
	se[1][0]=dlgPara.m_se10;
	se[1][1]=dlgPara.m_se11;
	se[1][2]=dlgPara.m_se12;
	se[2][0]=dlgPara.m_se20;
	se[2][1]=dlgPara.m_se21;
	se[2][2]=dlgPara.m_se22;	
	
	
	// ���Ĺ����״
	BeginWaitCursor();

	// �������ʱ����	
	CImgProcess imgOutput = imgInput;

	//����Tophat()ʵ�ֶ�ñ�任
	imgInput.Tophat(&imgOutput, 3, 3, 1, 1, se);
	//����se��һ��int **ָ�룬��ʾ�ṹԪ�ء����������3*3ԭ��λ�����ĵĽṹԪ��
	
	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;
	
	pDoc->SetModifiedFlag(true);
	
	pDoc->UpdateAllViews(NULL);
		
	// �ָ����
	EndWaitCursor();
	
}




void CDIPDemoView::OnPointStdlin()
{
	// TODO: �ڴ���������������

	CDIPDemoDoc* pDoc = GetDocument();

	// �������
	CImgProcess imgInput = pDoc->m_Image;

	// ���ͼ���ǻҶ�ͼ
	if (imgInput.m_pBMIH->biBitCount!=8)
	{
		AfxMessageBox("����8-bpp�Ҷ�ͼ���޷�����");
		return;
	}


	// ������������Ի���
	CDlgLinTrans dlg;
	dlg.m_bS1 = 64;
	dlg.m_bS2 = 192;
	dlg.m_bT1 = 32;
	dlg.m_bT2 = 224;

	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	// �������ʱ����
	CImgProcess imgOutput = imgInput;

	// �ֶ����Ա任
	imgInput.ParLinTran(&imgOutput, dlg.m_bS1, dlg.m_bS2, dlg.m_bT1, dlg.m_bT2);
	
	// ��������ظ��ĵ���	
	pDoc->m_Image = imgOutput;
	
	pDoc->SetModifiedFlag(true);
	
	pDoc->UpdateAllViews(NULL);
}
void CDIPDemoView::OnPointLog()
{
	// �����任
	
	CDIPDemoDoc* pDoc = GetDocument();

	// �������
	CImgProcess imgInput = pDoc->m_Image;
	
	// ���ͼ���ǻҶ�ͼ
	if (imgInput.m_pBMIH->biBitCount!=8)
	{
		AfxMessageBox("����8-bpp�Ҷ�ͼ���޷�����");
		return;
	}


	// ������������Ի���
	CDlgLog dlg;
	dlg.m_dC = 20;
	
	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	// �������ʱ����	
	CImgProcess imgOutput = imgInput;

	// ʹ�ö����任����
	imgInput.LogTran(&imgOutput, dlg.m_dC);
	
	// ��������ظ��ĵ���	
	pDoc->m_Image = imgOutput;
	
	pDoc->SetModifiedFlag(true);
	
	pDoc->UpdateAllViews(NULL);

}

void CDIPDemoView::OnPointGamma()
{
	// TODO: �ڴ���������������
	
	CDIPDemoDoc* pDoc = GetDocument();

	// �������
	CImgProcess imgInput = pDoc->m_Image;

	// ���ͼ���ǻҶ�ͼ
	if (imgInput.m_pBMIH->biBitCount!=8)
	{
		AfxMessageBox("����8-bpp�Ҷ�ͼ���޷�����");
		return;
	}

	

	// ������������Ի���
	CDlgGamma dlg;
	dlg.m_dEsp = 0;
	dlg.m_dGamma = 1;
	
	if (dlg.DoModal() != IDOK)
	{
		return;
	}
	
	// �������ʱ����
	CImgProcess imgOutput = imgInput;

	// ����GammaTran����ִ��٤��任
	imgInput.GammaTran(&imgOutput, dlg.m_dGamma, dlg.m_dEsp);
	
	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;
	
	pDoc->SetModifiedFlag(true);
	
	pDoc->UpdateAllViews(NULL);

}

void CDIPDemoView::OnPointHistst()
{
	// ֱ��ͼ�涨��
	
	CDIPDemoDoc* pDoc = GetDocument();

	// �������
	CImgProcess imgInput = pDoc->m_Image;

	// ���ͼ���ǻҶ�ͼ
	if (imgInput.m_pBMIH->biBitCount!=8)
	{
		AfxMessageBox("����8-bpp�Ҷ�ͼ���޷�����");
		return;
	}

	// ��׼ͼ��
	CImgProcess * pStdImage = new CImgProcess();

	// �����׼ͼ���ԭͼ��ĻҶ�ֱ��ͼ������
	double pdStdHist[256];

	// ͨ���򿪶Ի����ȡ��׼ͼ��
	
	// �ļ�·��
	CString filePath;
	
	// ���ļ��Ի�������û����ȡ���򷵻�
	CFileDialog openDlg(TRUE, 0, 0, 4|2, "Bitmap Files (*.bmp)||");
	if (openDlg.DoModal() == IDOK)
	{
		filePath = openDlg.GetPathName();
	}
	else
	{
		return;
	}

	// ��ȡ��׼ͼ��
	pStdImage->AttachFromFile(filePath);

	// ���ͼ���ǻҶ�ͼ
	if (pStdImage->m_pBMIH->biBitCount!=8)
	{
		AfxMessageBox("����8-bpp�Ҷ�ͼ���޷�����");
		return;
	}

	// ���㲢��ʾ��׼ͼ���ֱ��ͼ������û����ȡ���򷵻�
	pStdImage->GenHist(pdStdHist);
	CDlgHist histDlg;
	histDlg.m_pdHist = pdStdHist;
	if (histDlg.DoModal() != IDOK)
	{
		return;
	}
	delete histDlg;

	// �������ʱ����
	CImgProcess imgOutput = imgInput;

	// ֱ��ͼ�涨��
	imgInput.Histst(&imgOutput, pdStdHist);

	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;
	
	delete pStdImage;

	pDoc->SetModifiedFlag(true);
	
	pDoc->UpdateAllViews(NULL);
}

void CDIPDemoView::OnFileRotate()
{
	// ����ǰ����̨ͼ��
	CDIPDemoDoc * pdoc = GetDocument();
	
	swap<CImgProcess>(pdoc->m_Image, pdoc->m_OImage);

	pdoc->SetModifiedFlag(true);
	pdoc->UpdateAllViews(NULL);
}

void CDIPDemoView::OnFileReverse() 
{	// ͼ��ɫ
	
	// ��ȡ�ĵ�
	CDIPDemoDoc* pDoc = GetDocument();
	
	// ����ͼ��
	CImgProcess imgInput = pDoc->m_Image;
	
	// ���ͼ���ǻҶ�ͼ
	if (imgInput.m_pBMIH->biBitCount!=8)
	{
		AfxMessageBox("����8-bpp�Ҷ�ͼ���޷�����");
		return;
	}
		
	// ���Ĺ����״
	BeginWaitCursor();

	// �������ʱ����
	CImgProcess imgOutput = imgInput;

	imgOutput = !imgOutput;

	pDoc->m_Image = imgOutput;
	
	pDoc->SetModifiedFlag(true);
	
	pDoc->UpdateAllViews(NULL);
		
	// �ָ����
	EndWaitCursor();
}


void CDIPDemoView::OnEdgeRobert()
{
	// TODO: �ڴ���������������
	CDIPDemoDoc* pDoc = GetDocument();

	// �������
	CImgProcess imgInput = pDoc->m_Image;

	// ���ͼ���ǻҶ�ͼ
	if (imgInput.m_pBMIH->biBitCount!=8)
	{
		AfxMessageBox("����8-bpp�Ҷ�ͼ���޷�����");
		return;
	}
	
	CDlgEdgeSobelPrewitt dlg;
	dlg.isRobert = true;

	if (dlg.DoModal()!=IDOK)
	{
		return;
	}

	// �������ʱ����
	CImgProcess imgOutput = imgInput;

	imgInput.EdgeRoberts(&imgOutput, dlg.m_bThre, dlg.m_nEdgeType, dlg.m_bThining, dlg.m_bGratOnly);

	pDoc->m_Image = imgOutput;

	pDoc->SetModifiedFlag(true);
	
	pDoc->UpdateAllViews(NULL);
}

void CDIPDemoView::OnEdgeSobel()
{
	// TODO: �ڴ���������������
	
	CDIPDemoDoc* pDoc = GetDocument();

	// �������
	CImgProcess imgInput = pDoc->m_Image;

	// ���ͼ���ǻҶ�ͼ
	if (imgInput.m_pBMIH->biBitCount!=8)
	{
		AfxMessageBox("����8-bpp�Ҷ�ͼ���޷�����");
		return;
	}

	CDlgEdgeSobelPrewitt dlg;

	if (dlg.DoModal()!=IDOK)
	{
		return;
	}

	BeginWaitCursor();

	// �������ʱ����
	CImgProcess imgOutput = imgInput;

	// Sobel��Ե���
	imgInput.EdgeSobel(&imgOutput, dlg.m_bThre, dlg.m_nEdgeType, dlg.m_bThining, dlg.m_bGratOnly);

	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;

	pDoc->SetModifiedFlag(true);
	
	pDoc->UpdateAllViews(NULL);

	// �ָ����
	EndWaitCursor();
}

void CDIPDemoView::OnEdgePrewitt()
{
 	// TODO: �ڴ���������������
 	
 	CDIPDemoDoc* pDoc = GetDocument();
 
 	// �������
 	CImgProcess imgInput = pDoc->m_Image;
 
 	// ���ͼ���ǻҶ�ͼ
 	if (imgInput.m_pBMIH->biBitCount!=8)
 	{
 		AfxMessageBox("����8-bpp�Ҷ�ͼ���޷�����");
 		return;
 	}
 	
 	CDlgEdgeSobelPrewitt dlg;
 
 	if (dlg.DoModal()!=IDOK)
 	{
 		return;
 	}
	
	BeginWaitCursor();

 	// �������ʱ����
 	CImgProcess imgOutput = imgInput;
 
 	imgInput.EdgePrewitt(&imgOutput, dlg.m_bThre, dlg.m_nEdgeType, dlg.m_bThining, dlg.m_bGratOnly);
 
 	pDoc->m_Image = imgOutput;
 
 	pDoc->SetModifiedFlag(true);
 	
 	pDoc->UpdateAllViews(NULL);

	EndWaitCursor();
}


void CDIPDemoView::OnEdgeLog()
{
	// TODO: �ڴ���������������
	
	CDIPDemoDoc* pDoc = GetDocument();

	// �������
	CImgProcess imgInput = pDoc->m_Image;

	// ���ͼ���ǻҶ�ͼ
	if (imgInput.m_pBMIH->biBitCount!=8)
	{
		AfxMessageBox("����8-bpp�Ҷ�ͼ���޷�����");
		return;
	}


	BeginWaitCursor();

	// �������ʱ����
	CImgProcess imgOutput = imgInput;

	imgInput.EdgeLoG(&imgOutput);

	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;
	
	pDoc->SetModifiedFlag(true);
	
	pDoc->UpdateAllViews(NULL);

	EndWaitCursor();
}

void CDIPDemoView::OnEdgeAutothre()
{
	// �Զ���ֵ�ָ�
	
	CDIPDemoDoc* pDoc = GetDocument();

	// �������
	CImgProcess imgInput = pDoc->m_Image;
	
	// ���ͼ���ǻҶ�ͼ
	if (imgInput.m_pBMIH->biBitCount!=8)
	{
		AfxMessageBox("����8-bpp�Ҷ�ͼ���޷�����");
		return;
	}

	// �������ʱ����
	CImgProcess imgOutput = imgInput;

	// �Զ���ֵ��
	imgInput.AutoThreshold(&imgOutput);
	
	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;
	
	pDoc->SetModifiedFlag(true);
	
	pDoc->UpdateAllViews(NULL);
}

void CDIPDemoView::OnFreqIdeallpf()
{
	// TODO: �ڴ���������������
	CDIPDemoDoc* pDoc = GetDocument();

	// �������
	CImgProcess imgInput = pDoc->m_Image;

	// ���ͼ���ǻҶ�ͼ
	if (imgInput.m_pBMIH->biBitCount!=8)
	{
		AfxMessageBox("����8-bpp�Ҷ�ͼ���޷�����");
		return;
	}

	CDlgFreqIdealLPF dlg;
	dlg.m_nFreq = 0;

	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	BeginWaitCursor();

	// �������ʱ����
	CImgProcess imgOutput = imgInput;

	// ������Ҫ�����˾��Ĵ�С
	LONG w = imgInput.GetFreqWidth();
	LONG h = imgInput.GetFreqHeight();

	// ����Ƶ���˾�
	double * pdFreqFilt = new double[w*h];
	imgInput.FreqIdealLPF(pdFreqFilt, dlg.m_nFreq);
	
	//��Ӧ���˾�
	imgInput.FreqFilt(&imgOutput, pdFreqFilt);

	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;

	// ɾ����ʱ����
	delete pdFreqFilt;
	
	pDoc->SetModifiedFlag(true);
	
	pDoc->UpdateAllViews(NULL);
	
	EndWaitCursor();
}

void CDIPDemoView::OnFreqGausslpf()
{
	// TODO: �ڴ���������������
	CDIPDemoDoc* pDoc = GetDocument();

	// �������
	CImgProcess imgInput = pDoc->m_Image;
	
	// ���ͼ���ǻҶ�ͼ
	if (imgInput.m_pBMIH->biBitCount!=8)
	{
		AfxMessageBox("����8-bpp�Ҷ�ͼ���޷�����");
		return;
	}

	CDlgFreqGaussLPF dlg;
	dlg.m_dSigma = 0;

	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	BeginWaitCursor();

	// �������ʱ����
	CImgProcess imgOutput = imgInput;

	// ������Ҫ�����˾��Ĵ�С
	LONG w = imgInput.GetFreqWidth();
	LONG h = imgInput.GetFreqHeight();

	// ����Ƶ���˾�
	double * pdFreqFilt = new double[w*h];
	imgInput.FreqGaussLPF(pdFreqFilt, dlg.m_dSigma);
	
	//��Ӧ���˾�
	imgInput.FreqFilt(&imgOutput, pdFreqFilt);

	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;

	// ɾ����ʱ����
	delete pdFreqFilt;
	
	pDoc->SetModifiedFlag(true);
	
	pDoc->UpdateAllViews(NULL);
	
	EndWaitCursor();
}

void CDIPDemoView::OnFreqGausshpf()
{	// ��˹��ͨ�˲�

	CDIPDemoDoc* pDoc = GetDocument();

	// �������
	CImgProcess imgInput = pDoc->m_Image;

	// ���ͼ���ǻҶ�ͼ
	if (imgInput.m_pBMIH->biBitCount!=8)
	{
		AfxMessageBox("����8-bpp�Ҷ�ͼ���޷�����");
		return;
	}

	CDlgFreqGaussHPF dlg;
	dlg.m_dSigma = 0;

	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	BeginWaitCursor();

	// �������ʱ����
	CImgProcess imgOutput = imgInput;

	// ������Ҫ�����˾��Ĵ�С
	LONG w = imgInput.GetFreqWidth();
	LONG h = imgInput.GetFreqHeight();

	// ����Ƶ���˾�
	double * pdFreqFilt = new double[w*h];
	imgInput.FreqGaussHPF(pdFreqFilt, dlg.m_dSigma);
	
	//��Ӧ���˾�
	imgInput.FreqFilt(&imgOutput, pdFreqFilt);

	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;

	// ɾ����ʱ����
	delete pdFreqFilt;
	
	pDoc->SetModifiedFlag(true);
	
	pDoc->UpdateAllViews(NULL);
	
	EndWaitCursor();
}

void CDIPDemoView::OnFreqLapl()
{
	// TODO: �ڴ���������������
	CDIPDemoDoc* pDoc = GetDocument();

	// �������
	CImgProcess imgInput = pDoc->m_Image;

	// ���ͼ���ǻҶ�ͼ
	if (imgInput.m_pBMIH->biBitCount!=8)
	{
		AfxMessageBox("����8-bpp�Ҷ�ͼ���޷�����");
		return;
	}

	BeginWaitCursor();

	// �������ʱ����
	CImgProcess imgOutput = imgInput;

	// ������Ҫ�����˾��Ĵ�С
	LONG w = imgInput.GetFreqWidth();
	LONG h = imgInput.GetFreqHeight();

	// ����Ƶ���˾�
	double * pdFreqFilt = new double[w*h];
	imgInput.FreqLaplace(pdFreqFilt);
	
	//��Ӧ���˾�
	imgInput.FreqFilt(&imgOutput, pdFreqFilt);

	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;

	// ɾ����ʱ����
	delete pdFreqFilt;
	
	pDoc->SetModifiedFlag(true);
	
	pDoc->UpdateAllViews(NULL);
	
	EndWaitCursor();
}

void CDIPDemoView::OnFreqGaussbr()
{
	// TODO: �ڴ���������������

	// ���棺��ʾ���д��˲����������Ϊ����߿��Ϊ2���������ݵľ���ȫ����������������ͼ��
	// ����������ͼ��Ĵ���Ч������������֤��

	CDIPDemoDoc* pDoc = GetDocument();

	// �������
	CImgProcess imgInput = pDoc->m_Image;
	
	// ���ͼ���ǻҶ�ͼ
	if (imgInput.m_pBMIH->biBitCount!=8)
	{
		AfxMessageBox("����8-bpp�Ҷ�ͼ���޷�����");
		return;
	}

	CDlgFreqGaussBRF dlg;
	dlg.m_nFreq = 0;
	dlg.m_nWidth = 0;

	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	BeginWaitCursor();

	// �������ʱ����
	CImgProcess imgOutput = imgInput;

	// ������Ҫ�����˾��Ĵ�С
	LONG w = imgInput.GetFreqWidth();
	LONG h = imgInput.GetFreqHeight();

	// ����Ƶ���˾�
	double * pdFreqFilt = new double[w*h];
	imgInput.FreqGaussBRF(pdFreqFilt, dlg.m_nFreq, dlg.m_nWidth);
	
	//��Ӧ���˾�
	imgInput.FreqFilt(&imgOutput, pdFreqFilt);

	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;

	// ɾ����ʱ����
	delete pdFreqFilt;
	
	pDoc->SetModifiedFlag(true);
	
	pDoc->UpdateAllViews(NULL);
	
	EndWaitCursor();
}

void CDIPDemoView::OnEdgeCanny()
{
	// TODO: �ڴ���������������
	CDIPDemoDoc* pDoc = GetDocument();

	// �������
	CImgProcess imgInput = pDoc->m_Image;

	// ���ͼ���ǻҶ�ͼ
	if (imgInput.m_pBMIH->biBitCount!=8)
	{
		AfxMessageBox("����8-bpp�Ҷ�ͼ���޷�����");
		return;
	}

	BeginWaitCursor();

	// �������ʱ����
	CImgProcess imgOutput = imgInput;

	imgInput.EdgeCanny(&imgOutput);

	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;

	pDoc->SetModifiedFlag(true);
	
	pDoc->UpdateAllViews(NULL);

	EndWaitCursor();
}

void CDIPDemoView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CDIPDemoDoc* pDoc = GetDocument();

	// �������
	CImgProcess imgInput = pDoc->m_Image;

	// ���ͼ���ǻҶ�ͼ
	if (imgInput.m_pBMIH->biBitCount!=8)
	{
		AfxMessageBox("����8-bpp�Ҷ�ͼ���޷�����");
		return;
	}

	// �������ʱ����
	CImgProcess imgOutput = imgInput;
	// �����Ի���
	CDlgPointThre * dlgPara;

	switch (m_bLBtnDblClkSrv)
	{
		case 0:
			break;
		case 1:
			// ��ֵ
			BYTE	bThre;

			// ��ʼ���Ի���ͱ���
			dlgPara = new CDlgPointThre();
			dlgPara->m_bThre = 16;
			
			// ��ʾ�Ի�����ʾ�û��趨��ֵ
			if (dlgPara->DoModal() != IDOK)
			{
				// ����
				return;
			}
			
			// ��ȡ�û��趨����ֵ
			bThre = dlgPara->m_bThre;

			// ɾ���Ի���
			delete dlgPara;

			// ִ����������
			imgInput.RegionGrow(&imgOutput, point.x, point.y, bThre);
			break;

		default:
			AfxMessageBox((LPCTSTR)"����Ĳ������ã�����趨�ķ������");
	}
			

	pDoc->m_Image = imgOutput;

	pDoc->SetModifiedFlag(true);
	
	pDoc->UpdateAllViews(NULL);

	m_bLBtnDblClkSrv = 0;

	CScrollView::OnLButtonDblClk(nFlags, point);
}

void CDIPDemoView::OnEdgeRegionGrow()
{
	// ��������
	AfxMessageBox((LPCTSTR)"˫��ͼ���е��������趨����������ʼ�㡣");
	m_bLBtnDblClkSrv = 1; //��1ʹ��˫����Ϣ������������
}


void CDIPDemoView::OnEdgeHough()
{
	// TODO: �ڴ���������������
	CDIPDemoDoc* pDoc = GetDocument();

	// �������
	CImgProcess imgInput = pDoc->m_Image;
	
	// ���ͼ���Ƕ�ֵͼ��
	if (!imgInput.IsBinaryImg())
	{
		AfxMessageBox("���Ƕ�ֵͼ���޷�����");
		return;
	}

	CDlgHough dlg;
	dlg.m_nNum = 0;
	if(dlg.DoModal()!=IDOK)
	{
		return;
	}

	// Ҫ�ҵ���ֱ����Ŀ
	int nLineCount = dlg.m_nNum;
	// ֱ�߱�����ʱ����
	SLineInfo * pLines = new SLineInfo[nLineCount];

	// �������ʱ����
	CImgProcess imgOutput = imgInput;

	// Hough�任
	imgInput.Hough(pLines, nLineCount);

	// ������
	for (int k = 0; k<nLineCount; k++)//�����k��ֱ��
	{
		//ɨ��ͼ�����ֱ��
		for(int i = 0; i <imgInput.GetHeight(); i++)
		{
			for(int j = 0;j <imgInput.GetWidthPixel(); j++)
			{	
				int nDist;

				//����theta����rho
				nDist = (int) (j*cos(pLines[k].nAngle*PI/180.0) + \
							i*sin(pLines[k].nAngle*PI/180.0));
	
				if (nDist == pLines[k].nDist) //����㣨j, i����ֱ����
					imgOutput.SetPixel(j, i, RGB(255,255,255));
			}//for j
		}//for i
	}//for k

	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;

	pDoc->SetModifiedFlag(true);
	
	pDoc->UpdateAllViews(NULL);
}



void CDIPDemoView::OnFileConvertgray()
{
	// ��ȡ�ĵ����� 	
	CDIPDemoDoc* pDoc = GetDocument();

	// �������
	CImgProcess imgInput = pDoc->m_Image;

	// ���ͼ����256ɫͼ
	if (imgInput.m_pBMIH->biBitCount!=8)	{
		AfxMessageBox("����256ɫͼ���޷�����");
		return;
	}

	// 256ɫ����ͼת�Ҷ�
	imgInput.Index2Gray();

	// ��������ظ��ĵ���
	pDoc->m_Image = imgInput;

	pDoc->SetModifiedFlag(true);
	pDoc->UpdateAllViews(NULL);
}



void CDIPDemoView::OnFileClean() 
{
	// ��ȡ�ĵ����� 	
	CDIPDemoDoc* pDoc = GetDocument();

	// �������
	CImgProcess imgInput = pDoc->m_Image;
	
	// ���ͼ���ǻҶ�ͼ
	if (imgInput.m_pBMIH->biBitCount!=8)
	{
		AfxMessageBox("����8-bpp�Ҷ�ͼ���޷�����");
		return;
	}

	
	imgInput.InitPixels(255);// ���ͼ���ðף�

	// ��������ظ��ĵ���
	pDoc->m_Image = imgInput;
	
	// �������־
	pDoc->SetModifiedFlag(true);
	
	// ���ÿͻ�������Ч�������ػ��¼�
	pDoc->UpdateAllViews(NULL);
}

void CDIPDemoView::OnColorCmy2rgb() 
{
	// ����ĵ���ָ��
	CDIPDemoDoc* pDoc = GetDocument();

	// �������
	CImgProcess imgInput = pDoc->m_Image;
	
	// ���ͼ����24λͼ��
	if (imgInput.m_pBMIH->biBitCount!=24)
	{
		AfxMessageBox("����24λͼ���޷�����");
		return;
	}

	// �������ʱ����
	CImgProcess imgOutput = imgInput;

	// RGB2CMY
	imgInput.RGB2CMY(&imgOutput);


	
	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;

	pDoc->SetModifiedFlag(true);
	
	pDoc->UpdateAllViews(NULL);
}

void CDIPDemoView::OnColorRgb2hsi() 
{
	// ����ĵ���ָ��
	CDIPDemoDoc* pDoc = GetDocument();

	// �������
	CImgProcess imgInput = pDoc->m_Image;
	
	// ���ͼ����RGBͼ��
	if (imgInput.m_pBMIH->biBitCount!=24)
	{
		AfxMessageBox("����RGBͼ���޷�����");
		return;
	}

	// �������ʱ����
	CImgProcess imgOutput = imgInput;

	// RGB2HSI
	imgInput.RGB2HSI(&imgOutput);

	
	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;

	pDoc->SetModifiedFlag(true);
	
	pDoc->UpdateAllViews(NULL);
}

void CDIPDemoView::OnColorHsi2rgb() 
{
	// ����ĵ���ָ��
	CDIPDemoDoc* pDoc = GetDocument();

	// �������
	CImgProcess imgInput = pDoc->m_Image;
	
	// ���ͼ����24λͼ��
	if (imgInput.m_pBMIH->biBitCount!=24)
	{
		AfxMessageBox("����24λͼ���޷�����");
		return;
	}

	// �������ʱ����
	CImgProcess imgOutput = imgInput;

	// HSI2RGB
	imgInput.HSI2RGB(&imgOutput);
	
	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;

	pDoc->SetModifiedFlag(true);
	
	pDoc->UpdateAllViews(NULL);
}

void CDIPDemoView::OnColorRgb2hsv() 
{
	// ����ĵ���ָ��
	CDIPDemoDoc* pDoc = GetDocument();

	// �������
	CImgProcess imgInput = pDoc->m_Image;
	
	// ���ͼ����RGBͼ��
	if (imgInput.m_pBMIH->biBitCount!=24)
	{
		AfxMessageBox("����RGBͼ���޷�����");
		return;
	}

	// �������ʱ����
	CImgProcess imgOutput = imgInput;

	// RGB2HSV
	imgInput.RGB2HSV(&imgOutput);
	
	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;

	pDoc->SetModifiedFlag(true);
	
	pDoc->UpdateAllViews(NULL);
	
}

void CDIPDemoView::OnColorHsv2rgb() 
{
	// ����ĵ���ָ��
	CDIPDemoDoc* pDoc = GetDocument();

	// �������
	CImgProcess imgInput = pDoc->m_Image;
	
	// ���ͼ����24λͼ��
	if (imgInput.m_pBMIH->biBitCount!=24)
	{
		AfxMessageBox("����24λͼ���޷�����");
		return;
	}

	// �������ʱ����
	CImgProcess imgOutput = imgInput;

	// HSV2RGB
	imgInput.HSV2RGB(&imgOutput);
	
	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;

	pDoc->SetModifiedFlag(true);
	
	pDoc->UpdateAllViews(NULL);
}

void CDIPDemoView::OnColorRgb2yuv() 
{
	// ����ĵ���ָ��
	CDIPDemoDoc* pDoc = GetDocument();

	// �������
	CImgProcess imgInput = pDoc->m_Image;
	
	// ���ͼ����RGBͼ��
	if (imgInput.m_pBMIH->biBitCount!=24)
	{
		AfxMessageBox("����RGBͼ���޷�����");
		return;
	}

	// �������ʱ����
	CImgProcess imgOutput = imgInput;

	// RGB2YUV
	imgInput.RGB2YUV(&imgOutput);
	
	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;

	pDoc->SetModifiedFlag(true);
	
	pDoc->UpdateAllViews(NULL);
	
}

void CDIPDemoView::OnColorYuv2rgb() 
{
	// ����ĵ���ָ��
	CDIPDemoDoc* pDoc = GetDocument();

	// �������
	CImgProcess imgInput = pDoc->m_Image;
	
	// ���ͼ����24λͼ��
	if (imgInput.m_pBMIH->biBitCount!=24)
	{
		AfxMessageBox("����24λͼ���޷�����");
		return;
	}

	// �������ʱ����
	CImgProcess imgOutput = imgInput;

	// YUV2RGB
	imgInput.YUV2RGB(&imgOutput);
	
	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;

	pDoc->SetModifiedFlag(true);
	
	pDoc->UpdateAllViews(NULL);
	
}

void CDIPDemoView::OnColorRgb2yiq() 
{
	// ����ĵ���ָ��
	CDIPDemoDoc* pDoc = GetDocument();

	// �������
	CImgProcess imgInput = pDoc->m_Image;
	
	// ���ͼ����RGBͼ��
	if (imgInput.m_pBMIH->biBitCount!=24)
	{
		AfxMessageBox("����RGBͼ���޷�����");
		return;
	}

	// �������ʱ����
	CImgProcess imgOutput = imgInput;

	// RGB2YIQ
	imgInput.RGB2YIQ(&imgOutput);
	
	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;

	pDoc->SetModifiedFlag(true);
	
	pDoc->UpdateAllViews(NULL);
	
}

void CDIPDemoView::OnColorYiq2rgb() 
{
	// ����ĵ���ָ��
	CDIPDemoDoc* pDoc = GetDocument();

	// �������
	CImgProcess imgInput = pDoc->m_Image;
	
	// ���ͼ����24λͼ��
	if (imgInput.m_pBMIH->biBitCount!=24)
	{
		AfxMessageBox("����24λͼ���޷�����");
		return;
	}

	// �������ʱ����
	CImgProcess imgOutput = imgInput;

	// YIQ2RGB
	imgInput.YIQ2RGB(&imgOutput);
	
	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;

	pDoc->SetModifiedFlag(true);
	
	pDoc->UpdateAllViews(NULL);
}



void CDIPDemoView::OnRecTemplate() 
{	
	//ģ��ƥ��

	// ����ĵ���ָ��
	CDIPDemoDoc* pDoc = GetDocument();

	// �������
	CImgProcess imgInput = pDoc->m_Image;
	
	// ���ͼ���ǻҶ�ͼ��
	if (imgInput.m_pBMIH->biBitCount!=8)
	{
		AfxMessageBox("����8-bpp�Ҷ�ͼ���޷�����");
		return;
	}

	// �������ʱ����
	CImgProcess imgOutput = imgInput;

	CImgProcess TplImage; //ģ��ͼ��

	//�����Ի������û�ѡ��ģ��ͼ��
	CFileDialog dlg(TRUE,"bmp","*.bmp");
	if(dlg.DoModal() == IDOK)
	{
 
	 	CString strPathName;
 
		strPathName = dlg.GetPathName();
 
		TplImage.AttachFromFile(strPathName);
 	}
	else
		return;
	

	int nHeight = imgInput.GetHeight();
	int nWidth = imgInput.GetWidthPixel();
	int nTplHeight = TplImage.GetHeight();
	int nTplWidth = TplImage.GetWidthPixel();
	if(nTplHeight > nHeight || nTplWidth > nWidth )
	{
		// ��ʾ�û�
		MessageBox("ģ��ߴ����Դͼ��ߴ磡", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
	
		return;
	}

	// ���Ĺ����״
	BeginWaitCursor();

	// ����ģ��ƥ��
	imgInput.TemplateMatch(&imgOutput, &TplImage);
	
	// ��������ظ��ĵ���
	pDoc->m_Image = imgOutput;
	
	pDoc->SetModifiedFlag(true);
	
	pDoc->UpdateAllViews(NULL);
	
	// �ָ������״
	EndWaitCursor();

}

void CDIPDemoView::OnFeaGraymat() 
{	//����Ҷȹ��־���

	// ����ĵ���ָ��
	CDIPDemoDoc* pDoc = GetDocument();

	// �������
	CImgProcess imgInput = pDoc->m_Image;
	
	// ���ͼ���ǻҶ�ͼ��
	if (imgInput.m_pBMIH->biBitCount!=8)
	{
		AfxMessageBox("����8-bpp�Ҷ�ͼ���޷�����");
		return;
	}


	//�趨�������λ�ù�ϵ
	POINT ptD1, ptD2;

/*	//ˮƽ�Ҷȹ��־���
	ptD1.x = 1;
	ptD1.y = 0;
	ptD2.x = -1;
	ptD2.y = 0;
*/
	//+45�ȻҶȹ��־���
	ptD1.x = 1;
	ptD1.y = -1;
	ptD2.x = -1;
	ptD2.y = +1;

	//����Ҷȹ��־���
	vector< vector<int> > GrayMat = imgInput.GetGrayMatrix(ptD1, ptD2);
	
	CString str;

	int m = GrayMat.size();
	int n = GrayMat[0].size();

	//�����Ի�����ʾ�Ҷȹ��־���
	str.Format("{%d %d %d;%d %d %d;%d %d %d}", GrayMat[0][0], GrayMat[0][1], GrayMat[0][2], GrayMat[1][0], GrayMat[1][1], GrayMat[1][2], GrayMat[2][0], GrayMat[2][1], GrayMat[2][2]);
	AfxMessageBox(str);
}


