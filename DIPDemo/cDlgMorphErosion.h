#if !defined(AFX_CDLGMORPHEROSION_H__82B6CF03_D5E6_11D4_9AF2_0000B4C23285__INCLUDED_)
#define AFX_CDLGMORPHEROSION_H__82B6CF03_D5E6_11D4_9AF2_0000B4C23285__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// cDlgMorphErosion.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// cDlgMorphErosion dialog

class cDlgMorphErosion : public CDialog
{
// Construction
public:
	cDlgMorphErosion(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(cDlgMorphErosion)
	enum { IDD = IDD_DLG_MORPHErosion };
	CButton	m_Control9;
	CButton	m_Control8;
	CButton	m_Control7;
	CButton	m_Control6;
	CButton	m_Control5;
	CButton	m_Control4;
	CButton	m_Control3;
	CButton	m_Control2;
	CButton	m_Control1;
	int		m_nMode;
	int		m_nStructure1;
	int		m_nStructure2;
	int		m_nStructure3;
	int		m_nStructure4;
	int		m_nStructure5;
	int		m_nStructure6;
	int		m_nStructure7;
	int		m_nStructure8;
	int		m_nStructure9;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cDlgMorphErosion)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(cDlgMorphErosion)
	afx_msg void Oncustom();
	afx_msg void OnVert();
	afx_msg void OnHori();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDLGMORPHEROSION_H__82B6CF03_D5E6_11D4_9AF2_0000B4C23285__INCLUDED_)
