#pragma once
#include<vector>
#include<fstream>
#include"Schedule.h"
#include"Config.h"
#include"VUE.h"
#include"eNB.h"
#include"Message.h"
#include"Event.h"
#include"Utility.h"


class cSystem{
	//-----------------------TEST-----------------------
public:
	//-----------------------TEST-----------------------
private:
	/*------------------���ݳ�Ա------------------*/
	sConfig m_Config;//ϵͳ���ò���
	int m_TTI;//��ǰ��TTIʱ��
	int m_NTTI;//�����ܹ���TTI
	std::vector<ceNB> m_eNBVec;//��վ����
	std::vector<cRSU> m_RSUVec;//RSU����
	std::vector<cVeUE> m_VeUEVec;//VeUE����
	std::vector<sEvent> m_EventVec;//�¼�����
	
	/*
	* ����±�Ϊʱ��ۣ�����TTI��
	* ���¼�������ͬ���¼�����������ͬʱ�̴������¼���ID������ͬ��ʱ�����
	*/
	std::vector<std::list<int>> m_EventTTIList;//�¼���������m_EventList[i]�����i��TTI���¼���
	
public:
	/*------------------ϵͳ���̿���------------------*/
	void configure();//ϵͳ�����������
	void initialization();//ϵͳ�������ã����ϵͳ��ʼ��
	void destroy();//�ͷ���Դ
	void process();//ϵͳ��������

	

private:
	void buildEventList();


	/***************************************************************
	------------------------����ģ��--------------------------------
	****************************************************************/
public:
	/*--------------------�ӿں���--------------------*/
	void centralizedSchedule();//�����ܿ���

private:
	/*--------------------˽��ʵ�ֺ���--------------------*/
	void scheduleInfoClean();//�����ǰ���������û��ĵ�����Ϣ
	void schedulePF_RP_CSI_UL();//����PF-RP����

	//����ʱ��ѡȡ�㷨
public:
	sPFInfo selectKthPF(std::vector<sPFInfo>& vecF, int k, int p, int r);
	int partition(std::vector<sPFInfo>& vecF, int p, int r);
	void exchange(std::vector<sPFInfo>& vecF, int i, int j);



	/***************************************************************
	---------------------�ֲ�ʽ��Դ����-----------------------------
	-------------DRA:Distributed Resource Allocation----------------
	****************************************************************/
public :
	eDRAMode m_DRAMode;

	/*
	* DRA�����RSU�л�������Ϣ�޷��������͵�eventId����
	* ���ڴ���������������eventId
	* VeUE������Ϣ���֮ǰ�������˷ִأ��ҷ�������ԭ����ͬ�Ĵ���(��������δ������������������
	* VeUE������Ϣ���ֳ�ͻ�������Ѿ���ӽ���ӦRSU�ĵȺ����������ڽ�����һ���ش�֮ǰ�������˷ִأ����ҷ�������ԭ����ͬ�Ĵ���
	* �ܶ���֮���Ǵ洢��Ϣ������δ�ɹ��ҷ���VeUE����RSU�л���eventId
	* ��������ڽ��зִ�֮��ȫ��ת�����ӦRSU��WaitEventIdList����AdmitEventIdList��
	*/
	std::list<int> m_DRASwitchEventIdList;

	/*--------------------�ӿں���--------------------*/
	void DRASchedule();

private:
	/*--------------------ʵ�ֺ���--------------------*/
	void DRAInformationClean();//��Դ������Ϣ���
	void DRAPerformCluster(bool clusterFlag);//��RSU�ڵ�VeUE���зִ�
	void DRAGroupSizeBasedTDM(bool clusterFlag);//���ڴش�С��ʱ�ָ���
	void DRAUpdateAdmitEventIdList();//������������


	void DRASelectBasedOnP13();//����P1��P3����Դ����
	void DRASelectBasedOnP23();//����P2��P3����Դ����
	void DRASelectBasedOnP123();//����P1��P2��P3����Դ����

	void DRAConflictListener();//֡����ͻ

	/*--------------------��������--------------------*/
	void writeClusterPerformInfo(std::ofstream &out);
	void writeEventListInfo(std::ofstream &out);
	void writeEventLogInfo(std::ofstream &out);
};



