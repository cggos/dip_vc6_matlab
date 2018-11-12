
////////////////////////////////////////////////////////
// neuron.h�ļ�


#ifndef __NEURON_H__
#define __NEURON_H__

#include <vector>

using namespace std;

#define NEURALNET_VERSION 0x03






// #define NEED_MOMENTUM //����Ҫ���ӳ�����뽫��һ���ע��ȥ��

#define MOMENTUM 0.6 //����ϵ�����������ӳ�����(#define NEED_MOMENTUM)��ʱ��������

typedef double WEIGHT_TYPE; //����Ȩֵ����������




struct SNeuron  // ��ϸ������Ԫ
{
	/////////////////////////////////����//////////////////////////////

	int m_nInput; //������Ŀ
	WEIGHT_TYPE *m_pWeights; //��Ӧ�����Ȩֵ����

#ifdef NEED_MOMENTUM

	WEIGHT_TYPE *m_pPrevUpdate; //�����������ʱ���ڼ�¼��һ�ε�Ȩֵ����
#endif


	// ����ֵ ���ֵ   ����S����֮���ֵ
	double m_dActivation;

	// ���ֵ
	double m_dError;



	
	/////////////////////////////////����//////////////////////////////

	void Init(int nInput)
	{
		m_nInput = nInput+1; //������һ��ƫ���������Ŀ��ʵ��������Ŀ+1
		m_pWeights = new WEIGHT_TYPE[m_nInput]; //ΪȨֵ�������ռ�

#ifdef NEED_MOMENTUM
		m_pPrevUpdate = new WEIGHT_TYPE[m_nInput]; //Ϊ��һ��Ȩֵ�������ռ�
#endif
	
		m_dActivation = 0; //��Ԫ��Ӧ(���)������Sigmoid����������
		m_dError = 0; //��Ԫ�����ֵ
	}

	~SNeuron()
	{
		//�ͷſռ�
		delete []m_pWeights;
#ifdef NEED_MOMENTUM
		delete []m_pPrevUpdate;
#endif
	}	
	
}; //SNeuron



struct SNeuronLayer //�������
{
	/////////////////////////////////����//////////////////////////////

	int m_nNeuron; //�ò����Ԫ��Ŀ
	SNeuron *m_pNeurons; //��Ԫ����



	////////////////////////////////����////////////////////////////////

	SNeuronLayer(int nNeuron, int nInputsPerNeuron)
	{
		m_nNeuron = nNeuron;
		m_pNeurons = new SNeuron[nNeuron]; //����nNeuron����Ԫ������ռ�

		for(int i=0; i<nNeuron; i++)
		{
			m_pNeurons[i].Init(nInputsPerNeuron); //��Ԫ��ʼ��
		}
	}
	~SNeuronLayer()
	{
		delete []m_pNeurons; //�ͷ���Ԫ����
	}
}; //SNeuronLayer




//////////////////����ѵ���ļ�ʱʹ��/////////////
struct NEURALNET_HEADER
{
	DWORD dwVersion; //�汾��Ϣ
	
	// ��ʼ�����������ɸ���
	int m_nInput; //����������Ŀ
	int m_nOutput; //���������Ԫ��Ŀ
	int m_nHiddenLayer; //���ز���Ŀ��DigitRec��ֻ֧��1�����ز�

	//����ÿ��ѵ��ǰ���õĲ���
	int m_nNeuronsPerLyr;	//���ز㵥Ԫ��Ŀ	
	int m_nEpochs; //ѵ��ʱ����Ŀ�����򴫲��㷨�ĵ���������
}; //NEURALNET_HEADER

#endif // __NEURON_H__










