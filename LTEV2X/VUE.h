#pragma once
#include<vector>
#include<list>
#include<string>
#include"Global.h"
#include"Schedule.h"
#include"Enumeration.h"


class cVeUE {
	//-----------------------TEST-----------------------
public:
	static int m_VeUECount;
	//-----------------------TEST-----------------------
public:
	const int m_VeUEId=m_VeUECount++;//�û�ID
	int m_RSUId;//���ڵ�RSU��Id
	int m_ClusterIdx;//���ڴ�de���

	std::list<std::tuple<int,int>> m_LocationUpdateLogInfoList;

	/***************************************************************
	---------------------����ʽ��Դ����-----------------------------
	****************************************************************/

	bool m_IsScheduledUL;    //UpLink�Ƿ��ڱ�����
	sFeedbackInfo m_FeedbackUL;//��Ҫ���͸���վ�˵ķ�����Ϣ

	std::vector<double> m_CQIPredictIdeal;
	std::vector<double> m_CQIPredictRealistic;

	/***************************************************************
	---------------------�ֲ�ʽ��Դ����-----------------------------
	****************************************************************/

	int RBSelectBasedOnP2(const std::vector<std::vector<int>>&curAvaliablePatternIdx, eMessageType messageType);
	int RBEmergencySelectBasedOnP2(const std::vector<int>&curAvaliableEmergencyPatternIdx);

	std::string toString(int n);//���ڴ�ӡVeUE��Ϣ
};

inline
int cVeUE::RBSelectBasedOnP2(const std::vector<std::vector<int>>&curAvaliablePatternIdx, eMessageType messageType) {
	int size = static_cast<int>(curAvaliablePatternIdx[messageType].size());
	if (size == 0) return -1;
	return curAvaliablePatternIdx[messageType][rand() % size];
}

inline
int cVeUE::RBEmergencySelectBasedOnP2(const std::vector<int>&curAvaliableEmergencyPatternIdx) {
	int size = static_cast<int>(curAvaliableEmergencyPatternIdx.size());
	if (size == 0) return -1;
	return curAvaliableEmergencyPatternIdx[rand() % size];
}