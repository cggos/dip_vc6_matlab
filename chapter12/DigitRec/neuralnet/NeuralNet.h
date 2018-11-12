
////////////////////////////////////////
// neuralnet.h�ļ�


#ifndef _NEURALNET__H__
#define _NEURALNET__H__

//�ر�STL������Ϣ
#pragma warning (disable : 4786)

#include "neuralData.h"
#include <math.h>


#define BIAS 1 //ƫ����w0��ϵ��



//*************Ϊ��ʼ��Ȩֵ�����漴����*************//
#define WEIGHT_FACTOR 0.1 //һ������ 0 С�� 1 �ĸ������������޶���ʼȨֵ�ķ�Χ

//����һ��0��1֮������������
inline double RandFloat()	   {return (rand())/(RAND_MAX+1.0);}

//����һ������ -1 С�� 1�����������
inline double RandomClamped()	   {return WEIGHT_FACTOR*(RandFloat() - RandFloat());}



//�������ඨ��
class CNeuralNet
{
private:
	/////// ��ʼ������,ѵ����ʼ�����������в����޸� ////////
	int m_nInput; //���뵥Ԫ��Ŀ
	int m_nOutput; //�����Ԫ��Ŀ
	int m_nNeuronsPerLyr; //���ز㵥Ԫ��Ŀ

	// ���ز���Ŀ�������������
	int m_nHiddenLayer;

	//ѵ��������Ϣ
	int m_nMaxEpoch; //���ѵ��ʱ����Ŀ
	double m_dMinError; //�����ֵ

	///////////////////////////////////
	// ��̬����
	int m_nEpochs;
	double m_dLearningRate;
	double m_dErrorSum; //һ��ʱ�����ۼ����
	double m_dErr; //һ��ʱ����ƽ����ÿһ��ѵ����ÿ����������

	bool m_bStop;//����ѵ�������Ƿ���;ֹͣ

	SNeuronLayer *m_pHiddenLyr; //���ز�
	SNeuronLayer *m_pOutLyr; //�����

	vector<double> m_vecError; //ѵ�������ж�Ӧ�ڸ���ʱ����ѵ�����
public:
	// ���캯��
	CNeuralNet(int nInput, int nOutput, int nNeuronsPerLyr);
	~CNeuralNet();

	// ��ʼ������
	void InitializeNetwork();

	// �������������ǰ�򴫲�
	bool CalculateOutput(vector<double> input, vector<double> &output);

	// ѵ��һ��ʱ�����������
	bool TrainingEpoch(vector<iovector>& inputs, vector<iovector>& outputs);

	bool Train(vector<iovector>& SetIn, vector<iovector>& SetOut); //�������򴫲�ѵ������

	// ʶ��ĳһ��δ֪������������������
	int Recognize(CString strPathName, CRect rt, double &dConfidence);


	// ��ȡ����
	double GetErrorSum() { return m_dErrorSum; } //���ص�ǰʱ�����
	double GetError() {return m_dErr; } //����ƽ�����
	int GetEpoch() { return m_nEpochs; } //����ʱ����
	int GetNumOutput() { return m_nOutput; } //��������㵥Ԫ��Ŀ
	int GetNumInput() { return m_nInput; } //��������㵥Ԫ��Ŀ
	int GetNumNeuronsPerLyr() { return m_nNeuronsPerLyr; } //�������ز㵥Ԫ��Ŀ
		
	// �趨ѵ��������Ϣ
	void SetMaxEpoch(int nMaxEpoch) { m_nMaxEpoch = nMaxEpoch; }
	void SetMinError(double dMinError) { m_dMinError = dMinError; }
	void SetLearningRate(double dLearningRate) { m_dLearningRate = dLearningRate; }

	void SetStopFlag(bool bStop) { m_bStop = bStop; }

	// �����װ��ѵ���ļ�
	bool SaveToFile(const char* lpszFileName, bool bCreate = true); //����ѵ�����
	bool LoadFromFile(const char* lpszFileName, DWORD dwStartPos = 0); //װ��ѵ�����

protected:

	void CreateNetwork(); //�������磬Ϊ���㵥Ԫ����ռ�


	// Sigmoid ��������
	double	  Sigmoid(double netinput)
	{
		double response = 1.0; //���ƺ������ͳ̶ȵĲ���
		
		return ( 1 / ( 1 + exp(-netinput / response)));
	}
};


#endif // _NEURALNET__H__