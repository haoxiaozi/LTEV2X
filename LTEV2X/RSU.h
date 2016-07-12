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
	int m_RSUId;

	/*
	* ��ǰRSU��Χ�ڵ�VeUEId�������
	*/
	std::list<int> m_VeUEList;

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
	std::vector<std::vector<int>> m_DRA_RBIsAvailable;  

	/*
	* ���VeUE��ID������
	* �±����صı��
	*/
	std::vector<std::list<int>> m_DRAClusterVeUEIdList;  

	/*
	* ���ڴ�ŵ�ǰTTI����VeUE��ID
	*/
	std::vector<int> m_DRACallVeUEIdList; 

	/*
	* ��ŵ��ȵ�����Ϣ
	* ����±����ر��
	* �ڲ��±����FB���
	*/
	std::vector<std::vector<std::list<sDRAScheduleInfo>>> m_DRAScheduleList;
	
	/*
	* ��ͻ�б�
	* �б�Ԫ����tuple���ֱ����VeUEId,ClusterIdx�Լ�FBIdx������ClusterIdx�Լ�FBIdx�����ڳ�ͻ�����ͷŶ�Ӧ��Ƶ����Դ
	*/
	std::list<std::tuple<int,int,int>> m_DRAConflictInfoList;


	/*
	* RSU����ĵȴ��б�
	* ��ŵ���VeUEId
	* ����Դ�У�
	*		1���ִغ���System�����л�����ת���RSU����ĵȴ�����
	*		2���¼������е�ǰ���¼���������VeUEδ�ڿɷ�����Ϣ��ʱ�Σ�ת��ȴ�����
	*		3��VeUE�ڴ�����Ϣ�󣬷�����ͻ�������ͻ��ת��ȴ�����
	*/
	std::list<int> m_DRAWaitingVeUEIdList;

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
	* ÿ�������ٷ���һ��FB
	* ʣ��FB���������з���
	*/
	void DRAGroupSizeBasedTDM();

	/*
	* ��System����ĺ����ڲ�������
	* ���ڴ���System������¼��������¼�ת������Ӧ�������У�RSU����ĺ����������RSU����ĵȴ�����
	*/
	void DRAProcessSystemLevelEventList(int TTI, int STTI, const std::vector<cVeUE>& systemVeUEVec, const std::vector<std::list<sEvent>>& systemEventList);//��System�����RSU�л�����ת�뵱ǰʱ�̵ĺ�������

	/*
	* ��System����ĺ����ڲ�������
	* ���ڴ���RSU����ĵȴ��������¼�ת������Ӧ�������У�RSU����ĺ����������System�����RSU�л�����
	*/
	void DRAProcessRSULevelWaitingVeUEIdList(int TTI, const std::vector<cVeUE>& systemVeUEVec, std::list<int> &systemDRA_RSUSwitchVeUEIdList);//����һ��TTI��ͻ���û�()������ӵ�����������

	/*
	* ��System����ĺ����ڲ�������
	* ���ڴ���System�����RSU�л��������¼�ת������Ӧ�������У�RSU����ĺ����������RSU����ĵȴ�����
	* ������Ϻ󣬸�����Ĵ�СΪ0
	*/
	void DRAProcessSystemLevelRSUSwitchList(int TTI, const std::vector<cVeUE>& systemVeUEVec, std::list<int> &systemDRA_RSUSwitchVeUEIdList);


	/*
	* ����P1��P3����Դ����
	*/
	void DRASelectBasedOnP13(int TTI,std::vector<cVeUE>&systemVeUEVec);

	/*
	* ����P2��P3����Դ����
	*/
	void DRASelectBasedOnP23(int TTI,std::vector<cVeUE>&systemVeUEVec);

	/*
	* ����P1��P2��P3����Դ����
	*/
	void DRASelectBasedOnP123(int TTI,std::vector<cVeUE>&systemVeUEVec);

	/*
	* ֡����ͻ
	*/
	void DRAConflictListener(int TTI);

	/*
	* ά��m_DRAScheduleList�Լ�m_DRA_RBIsAvailable
	*/
	void DRAConflictSolve(int TTI);


	/*
	* ��������Ϣд���ļ��У������ã�
	*/
	void DRAWriteScheduleInfo(std::ofstream& out);

	/*
	* �����������еĵ�Ϣд���ļ��У������ã�
	*/
	void DRAWriteProcessInfo(std::ofstream& out,int type);

	/*
	* ���ɰ���RSU��Ϣ��string
	*/
	std::string toString();

	/*--------------------��������--------------------*/
private:
	/*
	* ��DRAGroupSizeBasedTDM(...)�ڲ�������
	* �������vector�������ֵ������
	*/
	int getMaxIndex(const std::vector<double>&v);

	/*
	* ��CallVeUEIdList����ӷ�װ���������ڲ鿴������ã����ڵ���
	*/
	void pushToRSULevelCallVeUEIdList(int VeUEId);

	/*
	* ��WaitingVeUEIdList����ӷ�װ���������ڲ鿴������ã����ڵ���
	*/
	void pushToRSULevelWaitingVeUEIdList(int VeUEId);

	/*
	* ��SwitchVeUEIdList����ӷ�װ���������ڲ鿴������ã����ڵ���
	*/
	void pushToSystemLevelRSUSwitchVeUEIdList(int VeUEId, std::list<int>& systemDRA_RSUSwitchVeUEIdList);
};