/////////////////////////////////////////////////
// DirDialog.h�ļ�

#ifndef __DIRDIALOG_H_
#define __DIRDIALOG_H_

#include <shlobj.h>

class CDirDialog
{
public:
	CDirDialog()
	{
		memset(&m_bi, 0, sizeof(m_bi));
		
		m_bi.hwndOwner = NULL;	 
		m_bi.pidlRoot = NULL;		 	
		m_bi.pszDisplayName = m_szDisplay; 
		m_bi.lpszTitle = NULL;		 	
		m_bi.ulFlags = BIF_RETURNONLYFSDIRS;
		
		m_szPath[0] = '\0';
	}


	// ��ʾ�Ի���
	BOOL DoBrowse(HWND hWndParent, LPCTSTR pszTitle = NULL);
	// ȡ���û�ѡ���Ŀ¼����
	LPCTSTR GetPath() { return m_szPath; }

protected:
	BROWSEINFOA m_bi; 

	// ���������û�ѡ��Ŀ¼�Ļ�����
	char m_szDisplay[MAX_PATH];
	char m_szPath[MAX_PATH];
};

inline BOOL CDirDialog::DoBrowse(HWND hWndParent, LPCTSTR pszTitle)
{
	if(pszTitle == NULL)
		m_bi.lpszTitle = "ѡ��Ŀ���ļ���";
	else
		m_bi.lpszTitle = pszTitle;

	m_bi.hwndOwner = hWndParent;
	LPITEMIDLIST pItem = ::SHBrowseForFolder(&m_bi);
	if(pItem != 0)
	{
		::SHGetPathFromIDList(pItem, m_szPath);
		return TRUE;
	}

	return FALSE;
}

#endif //__DIRDIALOG_H_