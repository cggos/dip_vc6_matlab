#if !defined(AFX_TRAINDLG_H__D80133BB_A5BE_47D3_8622_A277B6BB287D__INCLUDED_)
#define AFX_TRAINDLG_H__D80133BB_A5BE_47D3_8622_A277B6BB287D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "neuralnet\\ValueTrack.h"




// TrainDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTrainDlg dialog

class CTrainDlg : public CDialog
{
// Construction
public:
	UINT m_nTimer; //��ʱ��ID
	
	CRect m_rt; //ͼ����ľ�������
	CNeuralNet *m_pNet; //���������
	CNeuralData m_data; //����������ݶ���

	BOOL m_bStop;//ֹͣѵ����־

	CString strDirTrain; //ѵ����������Ŀ¼
	CValueTrack *m_pTrack; //���������ٵĶ���
	

	BOOL m_bInTraining; //�Ƿ���ѵ����

	CTrainDlg(CWnd* pParent = NULL);   // standard constructor
	~CTrainDlg();
	
	void SetDir(CString strDir); //�趨ѵ������Ŀ¼
	void UpdateTrainInfo(); //����ѵ��������ߺ�ʱ���������Ϣ
	void EnableControls(BOOL bEnable); //�趨�ؼ�״̬
	bool SaveToFile(CString strFilePath); //����ѵ�������ļ�
	bool LoadFromFile(CString strFilePathName, bool bLoadAll = true); //����ѵ�������ļ�
// Dialog Data
	//{{AFX_DATA(CTrainDlg)
	enum { IDD = IDD_DIALOG_TRAIN };
	CEdit	m_EditFile;
	CEdit	m_EditTrainFile;
	CStatic	m_StaticShow;
	CStatic	m_StaticErr;
	int		m_nMaxEpoch;
	double	m_dError;
	double	m_dLearningRate;
	int		m_nNeurons;
	UINT	m_nTimeStep;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrainDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTrainDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonTrain();
	afx_msg void OnButtonStop();
	afx_msg void OnClose();
	afx_msg void OnButtonBrowse();
	afx_msg void OnButtonSaveAs();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TrainDLG_H__D80133BB_A5BE_47D3_8622_A277B6BB287D__INCLUDED_)
