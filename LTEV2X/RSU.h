#pragma once

#include<vector>
#include<list>
#include<string>
#include<fstream>
#include"Schedule.h"
#include"Global.h"
#include"VUE.h"
#include"Event.h"

class cRSU {
	//-----------------------TEST-----------------------
public:
	static int count;
	cRSU();
	
	//-----------------------TEST-----------------------
public:
	/*
	* ��ǰRSU��ID
	*/
	const int m_RSUId=count++;

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
	* FB���ͷŸ���Դ��TTIʱ�̣�ATTI������TTI����ʱ�����
	* ����±����ر��
	* �ڲ��±����FB����
	* ��"TTI>m_DRA_RBIsAvailable[i][j]"�����i����Դ��j����
	*/
	std::vector<std::vector<bool>> m_DRA_RBIsAvailable;  

	/*
	* ���VeUE��ID������
	* �±����صı��
	*/
	std::vector<std::list<int>> m_DRAClusterVeUEIdList;  

	/*
	* ���ڴ�ŵ�ǰTTI�Ľ����¼�����
	*/
	std::vector<int> m_DRAAdmissionEventIdList; 

	/*
	* ��ŵ��ȵ�����Ϣ
	* ����±����ر��
	* �ڲ��±����FB���
	*/
	std::vector<std::vector<sDRAScheduleInfo*>> m_DRAScheduleInfoTable;
	
	/*
	* ��ǰʱ�̵�ǰRSU�ڴ��ڴ���״̬���¼�����
	* ����±����FB���
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
	std::list<int> m_DRAWaitingEventIdList;

	/*--------------------�ӿں���--------------------*/
	/*
	* ���ݴ˿̵�TTI���ص�ǰ������Դ����Ĵصı��
	*/
	int DRAGetClusterIdx(int ATTI);

	/*
	* ��Դ������Ϣ���
	*/
	void DRAInformationClean();

	/*
	* ���ڴش�С��ʱ�ָ���
	* ÿ�������ٷ���һ��FB
	* ʣ��FB���������з���
	*/
	void DRAGroupSizeBasedTDM();

	/*
	* ��System����ĺ����ڲ�������
	* ���ڴ���System������¼��������¼�ת������Ӧ�������У�RSU����Ľ����������RSU����ĵȴ�����
	*/
	void DRAProcessSystemLevelEventList(int ATTI, int STTI, const std::vector<cVeUE>& systemVeUEVec, const std::vector<sEvent>& systemEventList, const std::vector<std::list<int>>& systemEventTTIList);

	/*
	* ��System����ĺ����ڲ�������
	* ���ڴ���RSU����ĵ��ȱ�
	* ��������RSU�л��ĵ��¼����͵�System�����RSU�л������У����Ҫ������DRAProcessSystemLevelRSUSwitchList�ĵ���
	* ��������RSU��С���л����¼����͵�RSU����ĵȴ������У����Ҫ������DRAProcessRSULevelWaitingVeUEIdList�ĵ���
	*/
	void DRAProcessRSULevelScheduleInfoTable(int ATTI, const std::vector<cVeUE>& systemVeUEVec, const std::vector<sEvent>& systemEventVec, std::list<int> &systemDRA_RSUSwitchEventIdList);

	/*
	* ��System����ĺ����ڲ�������
	* ���ڴ���RSU����ĵȴ�����
	* ��������RSU�л����¼����͵�System�����RSU�л�������
	*/
	void DRAProcessRSULevelWaitingVeUEIdList(int ATTI, const std::vector<cVeUE>& systemVeUEVec, const std::vector<sEvent>& systemEventVec, std::list<int> &systemDRA_RSUSwitchEventIdList);


	/*
	* ��System����ĺ����ڲ�������
	* ���ڴ���System�����RSU�л��������¼�ת������Ӧ�������У�RSU����Ľ����������RSU����ĵȴ�����
	* ������Ϻ󣬸�����Ĵ�СΪ0
	*/
	void DRAProcessSystemLevelRSUSwitchList(int ATTI, const std::vector<cVeUE>& systemVeUEVec, const std::vector<sEvent>& systemEventVec, std::list<int> &systemDRA_RSUSwitchEventIdList);


	/*
	* ����P1��P3����Դ����
	*/
	void DRASelectBasedOnP13(int ATTI,std::vector<cVeUE>&systemVeUEVec, const std::vector<sEvent>& systemEventVec);

	/*
	* ����P2��P3����Դ����
	*/
	void DRASelectBasedOnP23(int ATTI,std::vector<cVeUE>&systemVeUEVec, const std::vector<sEvent>& systemEventVec);

	/*
	* ����P1��P2��P3����Դ����
	*/
	void DRASelectBasedOnP123(int ATTI,std::vector<cVeUE>&systemVeUEVec, const std::vector<sEvent>& systemEventVec);


	/*
	* ֡����ͻ
	*/
	void DRAConflictListener(int ATTI);

	/*
	* ��������Ϣд���ļ��У������ã�
	*/
	void DRAWriteScheduleInfo(std::ofstream& out,int ATTI);

	/*
	* �����������еĵ�Ϣд���ļ��У������ã�
	*/
	void DRAWriteProcessInfo(std::ofstream& out,int type, const std::vector<sEvent>& systemEventVec);

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
	* ��RSU�����CallVeUEIdList����ӷ�װ���������ڲ鿴������ã����ڵ���
	*/
	void pushToRSULevelAdmissionEventIdList(int eventId);

	/*
	* ��RSU�����WaitingVeUEIdList����ӷ�װ���������ڲ鿴������ã����ڵ���
	*/
	void pushToRSULevelWaitingEventIdList(int eventId);

	/*
	* ��System�����SwitchVeUEIdList����ӷ�װ���������ڲ鿴������ã����ڵ���
	*/
	void pushToSystemLevelRSUSwitchEventIdList(int eventId, std::list<int>& systemDRA_RSUSwitchVeUEIdList);

	/* 
	* ��System�����
	*/

	void pushToScheduleInfoTable(int clusterIdx,int FBIdx, sDRAScheduleInfo*p);

	void pullFromScheduleInfoTable(int ATTI);
};