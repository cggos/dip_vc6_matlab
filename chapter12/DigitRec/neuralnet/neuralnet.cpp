



#include "stdafx.h"
#include "neuralnet.h"
#include "../mainfrm.h"

// �ڵȴ��ڼ䴦����Ϣ
void WaitForIdle() 
{
	MSG msg;  
	while(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
}


CNeuralNet::CNeuralNet(int nInput, int nOutput, int nNeuronsPerLyr)
{
	m_nHiddenLayer = 1; //��ʱֻ֧��һ�����ز������
	m_nInput = nInput;
	m_nOutput = nOutput;
	m_nNeuronsPerLyr = nNeuronsPerLyr;
		
	m_pHiddenLyr = NULL;
	m_pOutLyr = NULL;

	CreateNetwork(); //Ϊ����������ռ�
	InitializeNetwork(); // ��ʼ����������
}


CNeuralNet::~CNeuralNet()
{
	if(m_pHiddenLyr != NULL)
		delete m_pHiddenLyr;
	if(m_pOutLyr != NULL)
		delete m_pOutLyr;
}


void CNeuralNet::CreateNetwork()
{	
	m_pHiddenLyr = new SNeuronLayer(m_nNeuronsPerLyr, m_nInput);
	m_pOutLyr = new SNeuronLayer(m_nOutput, m_nNeuronsPerLyr);

}



void CNeuralNet::InitializeNetwork()
{
	int i, j; //ѭ������


	//ʹ�õ�ǰʱ����Ϊ������ӣ��������Ա�֤����ÿ�����в�����ͬ��α�������
	srand((unsigned)time(NULL));
	
	
	//��ʼ�����ز�Ȩֵ
	for(i=0; i<m_pHiddenLyr->m_nNeuron; i++)
	{
		for(j=0; j<m_pHiddenLyr->m_pNeurons[i].m_nInput; j++)
		{
			m_pHiddenLyr->m_pNeurons[i].m_pWeights[j] = RandomClamped();
		
#ifdef NEED_MOMENTUM
			//��1��ʱ����ѵ����ʼ֮ǰ����û����һ�ε�Ȩֵ������Ϣ
			m_pHiddenLyr->m_pNeurons[i].m_pPrevUpdate[j] = 0;
#endif
		}
	}

	//��ʼ�������Ȩֵ
	for(i=0; i<m_pOutLyr->m_nNeuron; i++)
	{
		for(j=0; j<m_pOutLyr->m_pNeurons[i].m_nInput; j++)
		{
			m_pOutLyr->m_pNeurons[i].m_pWeights[j] = RandomClamped();

#ifdef NEED_MOMENTUM
			//��1��ʱ����ѵ����ʼ֮ǰ����û����һ�ε�Ȩֵ������Ϣ
			m_pOutLyr->m_pNeurons[i].m_pPrevUpdate[j] = 0;
#endif
		}
	}

	m_dErrorSum = 9999.0;//��ʶ��Ϊһ���ܴ��ѵ����������ѵ�������𽥼�С
	m_nEpochs = 0; //��ǰѵ��ʱ����Ŀ
}

bool CNeuralNet::SaveToFile(const char* lpszFileName, bool bCreate)
{
	CFile file;
	if( bCreate ) //�½�ģʽ
	{
		if(!file.Open(lpszFileName, CFile::modeWrite|CFile::modeCreate))
			return false;
	}
	else//׷��ģʽ
	{
		if(!file.Open(lpszFileName, CFile::modeWrite))
			return false;

		file.SeekToEnd(); //׷��д�뵽ĩβ
	}

	//д������ͷ��Ϣ
	NEURALNET_HEADER header = { 0 };
	header.dwVersion = NEURALNET_VERSION;
	header.m_nInput = m_nInput;
	header.m_nNeuronsPerLyr = m_nNeuronsPerLyr;
	header.m_nOutput = m_nOutput;
	header.m_nEpochs = m_nEpochs; 
	file.Write(&header, sizeof(header));

	//д��ѵ�������Ϣ
	file.Write(&m_dErr, sizeof(m_dErr));

	int i, j;
	//д��Ȩֵ��Ϣ
	for(i=0; i<m_pHiddenLyr->m_nNeuron; i++)
	{//���ز�Ȩֵ
		file.Write(&m_pHiddenLyr->m_pNeurons[i].m_dActivation,
			sizeof(m_pHiddenLyr->m_pNeurons[i].m_dActivation));
		file.Write(&m_pHiddenLyr->m_pNeurons[i].m_dError, 
			sizeof(m_pHiddenLyr->m_pNeurons[i].m_dError));

		for(j=0; j<m_pHiddenLyr->m_pNeurons[i].m_nInput; j++)
		{
			file.Write(&m_pHiddenLyr->m_pNeurons[i].m_pWeights[j],
				sizeof(m_pHiddenLyr->m_pNeurons[i].m_pWeights[j]));
		}
	}

	for(i=0; i<m_pOutLyr->m_nNeuron; i++)
	{//�����Ȩֵ
		file.Write(&m_pOutLyr->m_pNeurons[i].m_dActivation,
			sizeof(m_pOutLyr->m_pNeurons[i].m_dActivation));
		file.Write(&m_pOutLyr->m_pNeurons[i].m_dError, 
			sizeof(m_pOutLyr->m_pNeurons[i].m_dError));
		for(j=0; j<m_pOutLyr->m_pNeurons[i].m_nInput; j++)
		{
			file.Write(&m_pOutLyr->m_pNeurons[i].m_pWeights[j],
				sizeof(m_pOutLyr->m_pNeurons[i].m_pWeights[j]));
		}
	}
	
	file.Close();

	return true;
}

bool CNeuralNet::LoadFromFile(const char* lpszFileName, DWORD dwStartPos)
{
	CFile file;
	if(!file.Open(lpszFileName, CFile::modeRead))
		return false;

	file.Seek(dwStartPos, CFile::begin);//��λ��dwStartPosָ���Ŀ�ʼλ��

	//��������ͷ��Ϣ
	NEURALNET_HEADER header = { 0 };
	if(file.Read(&header, sizeof(header)) != sizeof(header))
		return false;
	//У��汾
	if(header.dwVersion != NEURALNET_VERSION)
		return false;
	//У�������������
	if(m_nInput != header.m_nInput
			|| m_nNeuronsPerLyr != header.m_nNeuronsPerLyr
			|| m_nOutput != header.m_nOutput)
			return false;

	m_nEpochs = header.m_nEpochs; //����ѵ��ʱ����Ŀ
	file.Read(&m_dErr , sizeof(m_dErr));//����ѵ�������Ϣ

	int i, j;
	// ��������Ȩֵ
	for(i=0; i<m_pHiddenLyr->m_nNeuron; i++)
	{//���ز�
		file.Read(&m_pHiddenLyr->m_pNeurons[i].m_dActivation,
			sizeof(m_pHiddenLyr->m_pNeurons[i].m_dActivation));
		file.Read(&m_pHiddenLyr->m_pNeurons[i].m_dError, 
			sizeof(m_pHiddenLyr->m_pNeurons[i].m_dError));

		for(j=0; j<m_pHiddenLyr->m_pNeurons[i].m_nInput; j++)
		{
			file.Read(&m_pHiddenLyr->m_pNeurons[i].m_pWeights[j],
				sizeof(m_pHiddenLyr->m_pNeurons[i].m_pWeights[j]));
		}
	}

	for(i=0; i<m_pOutLyr->m_nNeuron; i++)
	{//�����
		file.Read(&m_pOutLyr->m_pNeurons[i].m_dActivation,
			sizeof(m_pOutLyr->m_pNeurons[i].m_dActivation));
		file.Read(&m_pOutLyr->m_pNeurons[i].m_dError, 
			sizeof(m_pOutLyr->m_pNeurons[i].m_dError));
		for(j=0; j<m_pOutLyr->m_pNeurons[i].m_nInput; j++)
		{
			file.Read(&m_pOutLyr->m_pNeurons[i].m_pWeights[j],
				sizeof(m_pOutLyr->m_pNeurons[i].m_pWeights[j]));
		}
	}

	file.Close();
	return true;
}



bool CNeuralNet::CalculateOutput(vector<double> input, vector<double> &output)
{
	
	if(input.size() != m_nInput)//������������ά�����������벻���
		return false;

	int i, j;
	double nInputSum; //�����

	// �������ز����
	for(i=0; i<m_pHiddenLyr->m_nNeuron; i++)
	{
		nInputSum = 0;
		// ��˼���
		for(j=0; j<m_pHiddenLyr->m_pNeurons[i].m_nInput-1; j++)
		{
			nInputSum += m_pHiddenLyr->m_pNeurons[i].m_pWeights[j]*(input[j]);
		}
		
		// ����ƫ����
		nInputSum += m_pHiddenLyr->m_pNeurons[i].m_pWeights[j]*BIAS;

		// ����S���������
		m_pHiddenLyr->m_pNeurons[i].m_dActivation = Sigmoid(nInputSum);
	}

	// ������������
	for(i=0; i<m_pOutLyr->m_nNeuron; i++)
	{
		nInputSum = 0;
		// ��˼���
		for(j=0; j<m_pOutLyr->m_pNeurons[i].m_nInput-1; j++)
		{
			nInputSum += m_pOutLyr->m_pNeurons[i].m_pWeights[j]
				*m_pHiddenLyr->m_pNeurons[j].m_dActivation;
		}

		// ����ƫ����
		nInputSum += m_pOutLyr->m_pNeurons[i].m_pWeights[j]*BIAS;

		// ����S���������
		m_pOutLyr->m_pNeurons[i].m_dActivation = Sigmoid(nInputSum);

		// �����������
		output.push_back(m_pOutLyr->m_pNeurons[i].m_dActivation);
	}

	return true;
}

bool CNeuralNet::TrainingEpoch(vector<iovector>& SetIn, vector<iovector>& SetOut)
{
	int i, j, k;

	double WeightUpdate; //Ȩֵ������
	double err; //�����


	m_dErrorSum = 0; //�ۼ����
	for(i=0; i<SetIn.size(); i++) //�������ݶ��½������ÿ��ѵ����������Ȩֵ��
	{
		//////////////////////////////
		iovector vecOutputs;

		if(!CalculateOutput(SetIn[i], vecOutputs)) //��������������ǰ����
		{
			return false;
		}


		// ���������Ȩ��		
		for(j=0; j<m_pOutLyr->m_nNeuron; j++)
		{
			// ���������
			err = ((double)SetOut[i][j] - vecOutputs[j])*vecOutputs[j]*(1-vecOutputs[j]);
			m_pOutLyr->m_pNeurons[j].m_dError = err; //��¼�õ�Ԫ�������

			m_dErrorSum += ((double)SetOut[i][j] - vecOutputs[j])*((double)SetOut[i][j] - vecOutputs[j]); //�����ۼ����

			// ����ÿ�������Ȩ��
			for(k=0; k<m_pHiddenLyr->m_nNeuron; k++)
			{
				WeightUpdate = 
					err*m_dLearningRate*m_pHiddenLyr->m_pNeurons[k].m_dActivation;

#ifdef NEED_MOMENTUM
				// ���г������Ȩֵ������
				m_pOutLyr->m_pNeurons[j].m_pWeights[k] +=
						WeightUpdate + m_pOutLyr->m_pNeurons[j].m_pPrevUpdate[k]*MOMENTUM;

				m_pOutLyr->m_pNeurons[j].m_pPrevUpdate[k] = WeightUpdate;
#else


				//���µ�ԪȨֵ
				m_pOutLyr->m_pNeurons[j].m_pWeights[k] += WeightUpdate;

#endif
			}

			//ƫ�ø�����
			WeightUpdate = err*m_dLearningRate*BIAS;

#ifdef NEED_MOMENTUM
				// ���г������Ȩֵ������
			m_pOutLyr->m_pNeurons[j].m_pWeights[k] +=
					WeightUpdate + m_pOutLyr->m_pNeurons[j].m_pPrevUpdate[k]*MOMENTUM;

			m_pOutLyr->m_pNeurons[j].m_pPrevUpdate[k] = WeightUpdate;
#else

			//ƫ����ĸ���
			m_pOutLyr->m_pNeurons[j].m_pWeights[k] += WeightUpdate;

#endif
		}



		// �������ز�Ȩ��
		for(j=0; j<m_pHiddenLyr->m_nNeuron; j++)
		{
			err = 0;
			
			for(k=0; k<m_pOutLyr->m_nNeuron; k++)
			{
				err += m_pOutLyr->m_pNeurons[k].m_dError*m_pOutLyr->m_pNeurons[k].m_pWeights[j];
			}

			err *= m_pHiddenLyr->m_pNeurons[j].m_dActivation*(1-m_pHiddenLyr->m_pNeurons[j].m_dActivation);
			
			
			// ����ÿ�������Ȩ��
			for(k=0; k<m_pHiddenLyr->m_pNeurons[j].m_nInput-1; k++)
			{
				WeightUpdate = err*m_dLearningRate*SetIn[i][k];

#ifdef NEED_MOMENTUM
				// ���г������Ȩֵ������
				m_pHiddenLyr->m_pNeurons[j].m_pWeights[k] +=
						WeightUpdate + m_pHiddenLyr->m_pNeurons[j].m_pPrevUpdate[k]*MOMENTUM;

				m_pHiddenLyr->m_pNeurons[j].m_pPrevUpdate[k] = WeightUpdate;
#else



				m_pHiddenLyr->m_pNeurons[j].m_pWeights[k] += WeightUpdate;

#endif
			
			}
			//ƫ�ø�����
			WeightUpdate = err*m_dLearningRate*BIAS;

#ifdef NEED_MOMENTUM
				// ���г������Ȩֵ������
				m_pHiddenLyr->m_pNeurons[j].m_pWeights[k] +=
						WeightUpdate + m_pHiddenLyr->m_pNeurons[j].m_pPrevUpdate[k]*MOMENTUM;

				m_pHiddenLyr->m_pNeurons[j].m_pPrevUpdate[k] = WeightUpdate;
#else
			
			//ƫ����ĸ���
			m_pHiddenLyr->m_pNeurons[j].m_pWeights[k] += WeightUpdate;

#endif
		}
	}

	m_nEpochs ++; //ʱ������+1

	return true;
}



bool CNeuralNet::Train(vector<iovector>& SetIn, vector<iovector>& SetOut)
{
	m_bStop = FALSE; //�Ƿ�Ҫ��;ֹͣѵ��

	CString strOutMsg; //�����Ϣ

	do
	{	
		//ѵ��һ��ʱ��
		if(!TrainingEpoch(SetIn, SetOut)) 
		{
			
			strOutMsg.Format("ѵ���ڵ�%d��ʱ�����ִ���", GetEpoch());
			AfxMessageBox(strOutMsg);
			return false;
		}

		
		//����һ��ʱ����ƽ����ÿһ��ѵ����ÿ������Ĵ���
		m_dErr = GetErrorSum() / ( GetNumOutput() * SetIn.size() );

		if(m_dErr < m_dMinError)
			break; //����
		m_vecError.push_back(m_dErr); //��¼����ʱ���Ĵ����Ա�ѵ�����������ѵ���������
	
		WaitForIdle(); //��ѭ������ͣ�����Լ���Ƿ����û���������Ϣ����ҪΪ����ѵ����������;ֹͣ
		
		if(m_bStop) //���ֹͣ��־
			break;
	}
	while(m_nMaxEpoch-- > 0);
		

	return true;
}










/***********************
���ܣ�
	ʶ��ĳһ��δ֪������������������
���룺
	CString strPathName: ��ʶ�������Ĵ�ȡ·��
	rt:��ͼ��ȴ�ľ�������
	dConfidence: ������ֵ��δ����ֵ�Ĳ���Կ�����ʶ������Ŷ�
����ֵ��
	ʶ������������,����-1��ʾʶ��ʧ��
	
***********************/

int CNeuralNet::Recognize(CString strPathName, CRect rt, double &dConfidence)
{
	int nBestMatch; //�����
	
	double dMaxOut1 = 0; //������
	double dMaxOut2 = 0; //�δ����

	//�����ʶ��ͼ���ļ�
	CImg gray;
	if(!gray.AttachFromFile(strPathName))
		return -1;
	
	//����ʶ��ͼ��ת��Ϊ������ʽ
	vector<double> vecToRec;
	for(int j=rt.top; j<rt.bottom; j+= RESAMPLE_LEN)
	{
		for(int i=rt.left; i<rt.right; i+= RESAMPLE_LEN)
		{				
			int nGray = 0;
			for(int mm=j; mm<j+RESAMPLE_LEN; mm++)
			{
				for(int nn=i; nn<i+RESAMPLE_LEN; nn++)
					nGray += gray.GetGray(nn, mm);
			}
			nGray /= RESAMPLE_LEN*RESAMPLE_LEN;
		

			vecToRec.push_back(nGray/255.0);
		}
	}
	
	
	//�����������
	vector<double> outputs; //�������	
	if(!CalculateOutput(vecToRec, outputs))
	{
		AfxMessageBox("rec failed");
		return -1;
	}
	

	//Ѱ�������Ӧ�ĵ������Ԫ,��Ӧ�ĵ�Ԫ�ż�λ���
	nBestMatch = 0;
	for (int i=0; i<outputs.size(); ++i)
	{
		if (outputs[i] > dMaxOut1)
		{	
			dMaxOut2 = dMaxOut1;
			dMaxOut1 = outputs[i]; //��¼���������Ԫֵ
			
			
			nBestMatch = i;			
		}
	}

	dConfidence = dMaxOut1 - dMaxOut2; //�������Ŷ�

	return nBestMatch; //����ʶ���������ţ�
}













