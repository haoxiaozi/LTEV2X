#pragma once

#include<vector>
#include<list>
#include"Schedule.h"
#include"Global.h"

class cRSU {
public:
	cRSU() :mc_DRA_MTI(0.1f), mc_DRA_NTI(25), m_ClusterNum(4) {}

	std::vector<int> m_VecVUE;//��ǰRSU��Χ�ڵ�UEid�������

	/***************************************************************
	------------------------���е���--------------------------------
	****************************************************************/
	double m_AccumulateThroughput;   //�ۼ�������
	bool m_IsScheduledUL;    //UpLink�Ƿ��ڱ�����
	bool m_IsScheduledDL;    //DownLink�Ƿ��ڱ�����
	double m_FactorPF[gc_SubbandNum];
	double m_SINR[gc_SubbandNum];
	std::list<int> m_HIndicatorUL; //��������λ��
	std::list<int> m_HIndicatorDL; //��������λ��
	int  m_RBs[gc_MaxCodewords];  //Ƶ��*�ռ�
	cServingSector m_ServingSet;
	sFeedbackInfo m_FeedbackDL;//��Ҫ���͸���վ�˵ķ�����Ϣ
	sFeedbackInfo m_FeedbackUL;//��Ҫ���͸���վ�˵ķ�����Ϣ

	std::vector<double> m_CQIPredictIdeal;
	std::vector<double> m_CQIPredictRealistic;
	int m_ServingSectorId; //�û�����������ID = С��ID*ÿС��������+С����������� ������С��

	bool isScheduled(eLinkType);//��ǰTTI���û��Ƿ񱻵���


	/***************************************************************
	---------------------�ֲ�ʽ��Դ����-----------------------------
	-------------DRA:Distributed Resource Allocation----------------
	****************************************************************/
	eRSUType m_RSUType;   //RSU������
	const int m_ClusterNum;
	const float mc_DRA_MTI; //�ֲ�ʽ��Դ������Сʱ϶:Distributed Resource Allocation Minimum Time Interval
	const int mc_DRA_NTI; //���дؽ���һ��DRA��ռ�õ�TI������(NTI:number of Time Interval)
	int m_DRA_CNTI;   //��ǰRSU������ʱ϶����ͬ��RSU���Բ�ͬ������ͬ��DRA��CNTI:Current Number of Time Interval)
	std::vector<int> m_DRA_ETI;//�洢ÿ����������ʱ��������Ҷ˵�(ETI:End Time Interval)
	std::vector<bool> m_DRA_RB;  //Ƶ����Դ��,true������Դ�����
	std::vector<std::vector<int>> m_Cluster;   //��Ŵص�������ÿ���ذ���һ��vector<int>�洢������ID
	std::vector<std::vector<int>> m_CallList;   //���vector����һ���أ��ڲ�vector<int>����Ҫ�������ݵĳ���ID


	/*--------------------�ӿں���--------------------*/
	int getDRAClusterIdx();//���ݴ˿̵�m_DRA_CNTI���ص�ǰ���Խ�����Դ����Ĵصı��
	void DRAPerformCluster();
};