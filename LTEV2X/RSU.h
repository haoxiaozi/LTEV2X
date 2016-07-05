#pragma once

#include<vector>
#include<list>
#include"Schedule.h"
#include"Global.h"

class cRSU {
public:
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
};