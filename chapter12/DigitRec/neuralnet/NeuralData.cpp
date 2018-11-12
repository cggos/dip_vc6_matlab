// NeuralData.cpp: implementation of the CNeuralData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NeuralData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNeuralData::CNeuralData()
{
	CRect rt(0, 0, 0, 0);
	Init(rt, 0);
}

CNeuralData::~CNeuralData()
{

}

void CNeuralData::Clear()
{
	
	m_vecDirs.clear();
	m_vecClassNames.clear();
	m_vecSamples.clear();
	
	m_nClass = 0;
}

void CNeuralData::Init(CRect rt, int nInputs)
{
	m_rt = rt; //�趨����Χ����
	m_SetOut.clear();
	m_SetIn.clear();
	Clear(); //��ո�������

	m_nInputs = nInputs;//�趨������Ŀ
}

void CNeuralData::GetClassInfoFromDir(CString strDir)
{

	if( strDir.Right(1) != "\\" )
		strDir = strDir + "\\";
		
	CString strDirName; //����Ŀ¼���ƣ��������
	
	//����ѵ�������ļ����µķ���Ŀ¼
	WIN32_FIND_DATA findData;
	HANDLE hFindFile;
	
	CString strDirFind = strDir;
	strDirFind += "*.*";
	hFindFile = ::FindFirstFile(strDirFind, &findData);

	Clear(); //���Ŀ¼����

	if(hFindFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			if(findData.cFileName[0] == '.')
				continue;
				
			if(!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				continue; //�ҵ��Ĳ���Ŀ¼
			
	
			strDirFind = strDir + findData.cFileName;

			strDirFind += "\\";

			strDirName = findData.cFileName;
			AddData(strDirFind, strDirName);			
				
		}
		while(::FindNextFile(hFindFile, &findData));
			
		::FindClose(hFindFile);
	}
	else
	{
		AfxMessageBox("û���ҵ�ѵ������Ŀ¼���Ӷ��޷���ȡ���������Ϣ!");
		return;
	}
}


bool CNeuralData::AddData(CString strImgDir, CString strClassName)
{	
	//��������ļ�����Ŀ¼��·����Ϣ
	m_vecDirs.push_back(strImgDir);	
	
	//��Ӵ������������֣������Դ�Ÿ�����ļ��е�������Ϊ�������
	m_vecClassNames.push_back(strClassName); 

	m_nClass ++; //�����Ŀ
	return true;
}

vector<iovector >& CNeuralData::GetInputSet()
{
	return m_SetIn;
}

vector<iovector >& CNeuralData::GetOutputSet()
{
	return m_SetOut;
}

CString CNeuralData::GetClassName(int nClass)
{
	if(nClass < m_vecClassNames.size())
		return m_vecClassNames[nClass];
	else
		return "";
}








bool CNeuralData::GetSamplePaths()
{
	int nClass = m_vecDirs.size(); //�����Ŀ
	
	for(int i=0; i<nClass; i++)
	{
		// ������i�����Ŀ¼������Щ�����ļ��Ĵ�ȡ·�������� vecFiles
		vector<CString> vecFiles; //ĳһ��ѵ�������Ĵ�ȡ·��

		// ����Ŀ��Ϊ��i�����Ŀ¼�µ�ȫ�� .bmp ͼ���ļ�
		CString strToFind = m_vecDirs[i]; 
		strToFind += "*.bmp";
		
		WIN32_FIND_DATA findData;
		HANDLE hFindFile;
		CString strSamplePath; //ĳ��ѵ�������ļ��Ĵ�ȡ·��

		hFindFile = ::FindFirstFile(strToFind, &findData);

		if(hFindFile != INVALID_HANDLE_VALUE)
		{
			do
			{
				if(findData.cFileName[0] == '.')
					continue;
				
				if(!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{
					strSamplePath = m_vecDirs[i]; //ȡ�����Ŀ¼
					strSamplePath += findData.cFileName; //ȡ��������ȡ·��
					vecFiles.push_back(strSamplePath); //��ӵ�vecFiles
				}
				
			}
			while(::FindNextFile(hFindFile, &findData));
			
			::FindClose(hFindFile);
		}
		else
		{
			AfxMessageBox("û���ҵ�ѵ������ͼ���ļ�������ѵ������Ŀ¼�Ƿ���ȷ!");
			return false;
		}

		if(vecFiles.size() == 0)// ���Ŀ¼Ϊ��
			return false;

		

		m_vecSamples.push_back(vecFiles); //�������ѵ���ļ��Ĵ�ȡ·��������ȫ��ѵ������������
	}

	return true;
}



bool CNeuralData::GetTrainingSet()
{
	//���������������������������
	m_SetIn.clear();
	m_SetOut.clear();

	vector<double> vecInputs(m_nInputs, 0); //������������
	

	COCRImageProcess ocrImg; //OCRͼ�������

	int nClass = m_vecDirs.size(); //ȡ�������Ŀ



	// ����ÿһ���ѵ������ͼ��ת��Ϊ�������������ƣ��趨��Ӧ�����ǩ������Ϊ�������
	for(int i=0; i<nClass; i++)
	{
		int nSplInClass = m_vecSamples[i].size(); //����������Ŀ

		// Ϊ�� i ��ѵ�������趨�������
		vector<double> outputs(nClass, 0.1);
		outputs[i] = 0.9;

		for(int j=0; j<nSplInClass; j++)
		{
			// �趨��i���j���������������
			m_SetOut.push_back(outputs);
			

			// �趨��i���j����������������
			if(!ocrImg.AttachFromFile(m_vecSamples[i][j]))
			{
				CString strOut;
				strOut.Format("����ѵ������ͼ��%sʱ��������!", m_vecSamples[i][j]);
				AfxMessageBox(strOut);
				continue;
			}
			
			
			int nDim=0; //���������ĵ�ǰά
			int mm, nn;

			//�����ߴ�У��
			if( (ocrImg.GetHeight() != m_rt.bottom) || (ocrImg.GetWidthPixel() != m_rt.right) )
			{
				AfxMessageBox("ͼ���С��Ԥ�趨ֵ����!�������趨DigitRec.h�е�IMG_HEIGHT��IMG_WIDTG��");
				return false;
			}

			//ͼ���ز��������д洢Ϊ����
			for(int ii=m_rt.top; ii<m_rt.bottom; ii+=RESAMPLE_LEN)
			{
				for(int jj=m_rt.left; jj<m_rt.right; jj+=RESAMPLE_LEN)
				{
					int nGray = 0;
					for(mm=ii; mm<ii+RESAMPLE_LEN; mm++)
					{
						for(nn=jj; nn<jj+RESAMPLE_LEN; nn++)
							nGray += ocrImg.GetGray(nn, mm);
					}
					nGray /= RESAMPLE_LEN*RESAMPLE_LEN;
					
					vecInputs[nDim] = (double)nGray/255.0;					
					
					nDim ++;
				}// for jj
			}// for ii

			// �趨��i���j����������������			
			m_SetIn.push_back(vecInputs);
			
		}// for j
		
	} // for i

	return true;
}



bool CNeuralData::CreateTrainingSetFromData()
{ 
	//ȡ��ѵ�������Ĵ�ȡ·��
	if(GetSamplePaths() == false)
		return false;

	//ȡ������ѵ����
	if(	!GetTrainingSet() )
		return false;

	return true;
}