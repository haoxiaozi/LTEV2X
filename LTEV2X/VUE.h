#pragma once
#include<vector>
#include<list>
#include"Global.h"
#include"Schedule.h"

class cVeUE {
public:
	int m_AntennaTxNum;//����������
	int m_AntennaRxNum;//����������
	float m_X, m_Y;//�������С��������
	float m_AbsX, m_AbsY;//��������
	int m_UserId;//�û�ID
	std::list<int> m_HandoverSectorList;//�û����л�С������
	bool m_IsCall;//�û����б�־
	bool m_IsCac;//�û������ɱ�־
	std::vector<float> m_LSFadingFromUE2eNB;//	�û������л�վ�Ĵ��г߶�


	/***************************************************************
	------------------------����ģ��--------------------------------
	****************************************************************/

	bool m_IsScheduledUL;    //UpLink�Ƿ��ڱ�����
	bool m_IsScheduledDL;    //DownLink�Ƿ��ڱ�����
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