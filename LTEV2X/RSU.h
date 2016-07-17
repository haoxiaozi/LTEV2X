#pragma once

#include<vector>
#include<list>
#include<string>
#include<fstream>
#include<tuple>
#include"Schedule.h"
#include"Global.h"
#include"VUE.h"
#include"Event.h"

class cRSU {
	//-----------------------TEST-----------------------
public:
	static int s_RSUCount;
	cRSU();
	
	//-----------------------TEST-----------------------
public:
	/*
	* ��ǰRSU��ID
	*/
	const int m_RSUId=s_RSUCount++;

	/*
	* ��ǰRSU��Χ�ڵ�VeUEId�������
	*/
	std::list<int> m_VeUEIdList;

	/***************************************************************
	------------------------���е���--------------------------------
	****************************************************************/
	double m_AccumulateThroughput;   //�ۼ�������
	bool m_IsScheduledUL;    //UpLink�Ƿ��ڱ�����
	bool m_IsScheduledDL;    //DownLink�Ƿ��ڱ�����
	double m_FactorPF[gc_RBNum];
	double m_SINR[gc_RBNum];
	sFeedbackInfo m_FeedbackUL;//��Ҫ���͸���վ�˵ķ�����Ϣ

	std::vector<double> m_CQIPredictIdeal;
	std::vector<double> m_CQIPredictRealistic;


	/***************************************************************
	---------------------�ֲ�ʽ��Դ����-----------------------------
	-------------DRA:Distributed Resource Allocation----------------
	****************************************************************/

	//-----------------------static-------------------------------
	/*
	* Pattern����������
	* ÿ��Pattern��ռ��FB������
	* ��ͬPattern����������ռ��FB��ĸ���
	* Ŀǰ�ݶ���������һ�����������¼���һ��������ҵ���¼�
	*/
	static const int s_DRAPatternTypeNum=2;

	/*
	* ��ȫƵ��ÿ��Pattern�����Ӧ��Pattern����
	* �±��Ӧ��eMessageType�ж�����¼����͵���ֵ
	* ���磬�±�0��Ӧ��PERIOD��1��Ӧ��DATA
	*/
	static const int s_DRAPatternNumPerPatternType[s_DRAPatternTypeNum];

	/*
	* ÿ��Pattern������ռ��FB����
	*/
	static const int s_DRA_FBNumPerPatternType[s_DRAPatternTypeNum];

	/*
	* ÿ��Pattern�����Ӧ��Pattern Idx���б�
	*/
	static const std::list<int> s_DRAPatternIdxList[s_DRAPatternTypeNum];


	/*
	* ����Pattern���͵�Pattern�����ܺ�
	*/
	static const int s_DRATotalPatternNum;

	//-----------------------static-------------------------------

	/*
	* RSU�����ͣ�
	* 1������ʮ��·�ڵ�RSU
	* 2���ֲ��ڵ�·�м��RSU
	*/
	eRSUType m_RSUType; 

	/*
	* һ��RSU���Ƿ�Χ�ڵĴصĸ���
	* ��RSU������"m_RSUType"�й�
	*/
	const int m_DRAClusterNum;

	/*
	* TDR:Time Domain Resource
	* �±����ر��
	* tuple�洢�ı����ĺ�������Ϊ���洢ÿ����������ʱ�������������˵㣬�Ҷ˵��Լ����䳤��
	*/
	std::vector<std::tuple<int,int,int>> m_DRAClusterTDRInfo;

	/*
	* Pattern���ͷŸ���Դ��TTIʱ�̣���TTI����ʱ�����
	* ����±����ر��
	* �ڲ��±����Pattern���
	* ��"m_DRAPatternIsAvailable[i][j]==true"�����i��Pattern��j����
	*/
	std::vector<std::vector<bool>> m_DRAPatternIsAvailable;  

	/*
	* ���ÿ���ص�VeUE��ID������
	* �±����صı��
	*/
	std::vector<std::list<int>> m_DRAClusterVeUEIdList;  

	/*
	* ���ڴ�ŵ�ǰTTI�Ľ����¼�����
	*/
	std::vector<int> m_DRAAdmitEventIdList; 

	/*
	* ��ŵ��ȵ�����Ϣ
	* ����±����ر��
	* �ڲ��±����Pattern���
	*/
	std::vector<std::vector<sDRAScheduleInfo*>> m_DRAScheduleInfoTable;
	
	/*
	* ��ǰʱ�̵�ǰRSU�ڴ��ڴ���״̬���¼�����
	* ����±����Pattern���
	* �ڲ���list���ڴ����ͻ
	*/
	std::vector<std::list<sDRAScheduleInfo*>> m_DRATransimitEventIdList;

	/*
	* RSU����ĵȴ��б�
	* ��ŵ���VeUEId
	* ����Դ�У�
	*		1���ִغ���System�����л�����ת���RSU����ĵȴ�����
	*		2���¼������е�ǰ���¼���������VeUEδ�ڿɷ�����Ϣ��ʱ�Σ�ת��ȴ�����
	*		3��VeUE�ڴ�����Ϣ�󣬷�����ͻ�������ͻ��ת��ȴ�����
	*/
	std::list<int> m_DRAWaitEventIdList;

	/*--------------------�ӿں���--------------------*/
	/*
	* ���ݴ˿̵�TTI���ص�ǰ������Դ����Ĵصı��
	*/
	int DRAGetClusterIdx(int TTI);

	/*
	* ��Դ������Ϣ���
	*/
	void DRAInformationClean();

	/*
	* ���ڴش�С��ʱ�ָ���
	* ÿ�������ٷ���һ��ʱ϶
	* ʣ��ʱ϶���������з���
	*/
	void DRAGroupSizeBasedTDM();

	/*
	* ��System����ĺ����ڲ�������
	* ���ڴ���System������¼��������¼�ת������Ӧ�������У�RSU����Ľ����������RSU����ĵȴ�����
	*/
	void DRAProcessSystemLevelEventList(int TTI, const std::vector<cVeUE>& systemVeUEVec, std::vector<sEvent>& systemEventVec, const std::vector<std::list<int>>& systemEventTTIList);

	/*
	* ��System����ĺ����ڲ�������
	* ���ڴ���RSU����ĵ��ȱ�
	* ��������RSU�л��ĵ��¼����͵�System�����RSU�л������У����Ҫ������DRAProcessSystemLevelSwitchList�ĵ���
	* ��������RSU��С���л����¼����͵�RSU����ĵȴ������У����Ҫ������DRAProcessRSULevelWaitVeUEIdList�ĵ���
	*/
	void DRAProcessRSULevelScheduleInfoTable(int TTI, const std::vector<cVeUE>& systemVeUEVec, std::vector<sEvent>& systemEventVec, std::list<int> &systemDRASwitchEventIdList);

	/*
	* ��System����ĺ����ڲ�������
	* ���ڴ���RSU����ĵȴ�����
	* ��������RSU�л����¼����͵�System�����RSU�л�������
	*/
	void DRAProcessRSULevelWaitEventIdList(int TTI, const std::vector<cVeUE>& systemVeUEVec, std::vector<sEvent>& systemEventVec, std::list<int> &systemDRASwitchEventIdList);

	/*
	* ��System����ĺ����ڲ�������
	* ���ڴ���System�����RSU�л��������¼�ת������Ӧ�������У�RSU����Ľ����������RSU����ĵȴ�����
	* ������Ϻ󣬸�����Ĵ�СΪ0
	*/
	void DRAProcessSystemLevelSwitchList(int TTI, const std::vector<cVeUE>& systemVeUEVec, std::vector<sEvent>& systemEventVec, std::list<int> &systemDRASwitchEventIdList);

	/*
	* ����P1��P3����Դ����
	*/
	void DRASelectBasedOnP13(int TTI,std::vector<cVeUE>&systemVeUEVec, const std::vector<sEvent>& systemEventVec);

	/*
	* ����P2��P3����Դ����
	*/
	void DRASelectBasedOnP23(int TTI,std::vector<cVeUE>&systemVeUEVec, const std::vector<sEvent>& systemEventVec);

	/*
	* ����P1��P2��P3����Դ����
	*/
	void DRASelectBasedOnP123(int TTI,std::vector<cVeUE>&systemVeUEVec, const std::vector<sEvent>& systemEventVec);

	/*
	* ֡����ͻ
	*/
	void DRAConflictListener(int TTI, std::vector<sEvent>& systemEventVec);

	/*
	* ��������Ϣд���ļ��У������ã�
	*/
	void DRAWriteScheduleInfo(std::ofstream& out,int TTI);

	/*
	* ����TTIΪ�ؼ��ֵ���־��Ϣд���ļ��У������ã�
	*/
	void DRAWriteTTILogInfo(std::ofstream& out, int TTI, int type, int eventId, int RSUId, int clusterIdx, int patternIdx);

	/*
	* ���ɰ���RSU��Ϣ��string
	*/
	std::string toString(int n);

	/*--------------------��������--------------------*/
private:
	/*
	* ��DRAGroupSizeBasedTDM(...)�ڲ�������
	* �������vector�������ֵ������
	*/
	int getMaxIndex(const std::vector<double>&clusterSize);

	/*
	* ���ڲ���ָ��VeUEId�����Ĵر��
	* ��Ҫ������������ж�VeUE�Ƿ����ڸ�RSU��ֱ����VeUE�����RSU������жϼ��ɣ�
	* ���������ʹ��ǰ���ǣ���֪�������ڴ���
	*/
	int getClusterIdxOfVeUE(int VeUEId);

	/*
	* ���㵱ǰ�¼�����Ӧ�ĵ�������
	* �ں���DRASelectBasedOnP123(...)�ڲ�������
    */
	std::list<std::tuple<int, int>> buildScheduleIntervalList(int TTI,sEvent event, std::tuple<int, int, int>ClasterTTI);

	/*
	* ��RSU�����CallVeUEIdList����ӷ�װ���������ڲ鿴������ã����ڵ���
	*/
	void pushToRSULevelAdmitEventIdList(int eventId);

	/*
	* ��RSU�����WaitVeUEIdList����ӷ�װ���������ڲ鿴������ã����ڵ���
	*/
	void pushToRSULevelWaitEventIdList(int eventId);

	/*
	* ��System�����SwitchVeUEIdList����ӷ�װ���������ڲ鿴������ã����ڵ���
	*/
	void pushToSystemLevelSwitchEventIdList(int eventId, std::list<int>& systemDRASwitchVeUEIdList);

	/* 
	* ��RSU�����ScheduleInfoTable����ӷ�װ���������ڲ鿴������ã����ڵ���
	*/
	void pushToScheduleInfoTable(int clusterIdx,int patternIdx, sDRAScheduleInfo*p);

	/*
	* ��RSU�����ScheduleInfoTable�ĵ�����װ���������ڲ鿴������ã����ڵ���
	*/
	void pullFromScheduleInfoTable(int TTI);
};


inline
void cRSU::pushToRSULevelAdmitEventIdList(int eventId) {
	m_DRAAdmitEventIdList.push_back(eventId);
}

inline
void cRSU::pushToRSULevelWaitEventIdList(int eventId) {
	m_DRAWaitEventIdList.push_back(eventId);
}

inline
void cRSU::pushToSystemLevelSwitchEventIdList(int VeUEId, std::list<int>& systemDRASwitchVeUEIdList) {
	systemDRASwitchVeUEIdList.push_back(VeUEId);
}

inline
void cRSU::pushToScheduleInfoTable(int clusterIdx, int patternIdx, sDRAScheduleInfo*p) {
	m_DRAScheduleInfoTable[clusterIdx][patternIdx] = p;
}

inline
void cRSU::pullFromScheduleInfoTable(int TTI) {
	int clusterIdx = DRAGetClusterIdx(TTI);
	/*�����ڵ��ȱ��е�ǰ���Դ������Ϣѹ��m_DRATransimitEventIdList*/
	for (int patternIdx = 0; patternIdx < s_DRATotalPatternNum; patternIdx++) {
		if (m_DRAScheduleInfoTable[clusterIdx][patternIdx] != nullptr) {
			m_DRATransimitEventIdList[patternIdx].push_back(m_DRAScheduleInfoTable[clusterIdx][patternIdx]);
			m_DRAScheduleInfoTable[clusterIdx][patternIdx] = nullptr;
		}
	}
}