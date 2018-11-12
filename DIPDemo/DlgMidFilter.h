#if !defined(AFX_DLGMIDFILTER_H__3844F7C0_0F6D_488D_97CE_1DF381742683__INCLUDED_)
#define AFX_DLGMIDFILTER_H__3844F7C0_0F6D_488D_97CE_1DF381742683__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMidFilter.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgMidFilter dialog

class CDlgMidFilter : public CDialog
{
// Construction
public:
	CDlgMidFilter(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgMidFilter)
	enum { IDD = IDD_DLG_MidianFilter };
	
	// �˲�������
	int		m_iFilterType;
	
	// �˲����߶�
	int		m_iFilterH;
	
	// �˲������
	int		m_iFilterW;
	
	// �˲�������Ԫ��X����
	int		m_iFilterMX;
	
	// �˲�������Ԫ��Y����
	int		m_iFilterMY;
	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMidFilter)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMidFilter)
	afx_msg void OnRad1();
	afx_msg void OnRad2();
	afx_msg void OnRad3();
	afx_msg void OnRad4();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMIDFILTER_H__3844F7C0_0F6D_488D_97CE_1DF381742683__INCLUDED_)
