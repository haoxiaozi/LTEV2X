#pragma once
#include<vector>
#include"Schedule.h"
#include"Config.h"
#include"VUE.h"
#include"eNB.h"
#include"Message.h"
#include"Event.h"


class cSystem{
	//-----------------------TEST-----------------------
public:
	void print();
	//-----------------------TEST-----------------------
private:
	/*------------------���ݳ�Ա------------------*/
	sConfig m_Config;//ϵͳ���ò���
	int m_STTI;//������ʼʱ��TTI��������ɵķǸ�������Ϊ��ʼʱ�̣�
	int m_TTI;//��ǰTTIʱ��
	int m_NTTI;//�����ܹ���TTI
	std::vector<ceNB> m_VeceNB;//��վ����
	std::vector<cRSU> m_VecRSU;//RSU����
	std::vector<cVeUE> m_VecVUE;//��������
	std::vector<std::list<sEvent>> m_EventList;//���з����¼�����m_CallSetupList[i][j]�����i��TTI���¼���
	
public:
	/*------------------ϵͳ���̿���------------------*/
	void configure();//ϵͳ�����������
	void initialization();//ϵͳ�������ã����ϵͳ��ʼ��
	void destroy();//�ͷ���Դ
	void process();//ϵͳ��������




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
	/*--------------------�ӿں���--------------------*/
	void DRASchedule();

private:
	/*--------------------ʵ�ֺ���--------------------*/
	void DRAInformationClean();//��Դ������Ϣ���
	void DRAPerformCluster();//��RSU�ڵĳ������зִ�
	void DRAGroupSizeBasedTDM();//���ڴش�С��ʱ�ָ���
	void DRABuildCallList();//������������


	void DRASelectBasedOnP13();//����P1��P3����Դ����
	void DRASelectBasedOnP23();//����P2��P3����Դ����
	void DRASelectBasedOnP123();//����P1��P2��P3����Դ����

	void DRAConflictListener();//֡����ͻ

	/*--------------------��������--------------------*/
};



