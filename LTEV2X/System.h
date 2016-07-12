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
	int m_STTI;//������ʼʱ��TTI��������ɵķǸ�������Ϊ��ʼʱ�̣�
	int m_TTI;//��ǰTTIʱ��
	int m_NTTI;//�����ܹ���TTI
	std::vector<ceNB> m_eNBVec;//��վ����
	std::vector<cRSU> m_RSUVec;//RSU����
	std::vector<cVeUE> m_VeUEVec;//VeUE����
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

	/*
	* DRA�����RSU�л�������Ϣ�޷��������͵�VeUEId����
	* ���ڴ���������������VeUEId
	* VeUE������Ϣ���֮ǰ�������˷ִأ��ҷ�������ԭ����ͬ�Ĵ���
	* VeUE������Ϣ��ͻ�������Ѿ���ӽ���ӦRSU�ĳ�ͻ���������ڽ�����һ���ش�֮ǰ�������˷ִأ����ҷ�������ԭ����ͬ�Ĵ���
	* �ܶ���֮���Ǵ洢��Ϣ������δ�ɹ��ҷ���VeUE����RSU�л���VeUEId
	* ��������ڽ��зִ�֮��ȫ��ת�����ӦRSU��WaitingSet��
	*/
	std::list<int> m_DRA_RSUSwitchVeUEIdList;
	/*--------------------�ӿں���--------------------*/
	void DRASchedule();

private:
	/*--------------------ʵ�ֺ���--------------------*/
	void DRAInformationClean();//��Դ������Ϣ���
	void DRAPerformCluster(bool clusterFlag);//��RSU�ڵ�VeUE���зִ�
	void DRAGroupSizeBasedTDM(bool clusterFlag);//���ڴش�С��ʱ�ָ���
	void DRAUpdateCallList();//������������


	void DRASelectBasedOnP13();//����P1��P3����Դ����
	void DRASelectBasedOnP23();//����P2��P3����Դ����
	void DRASelectBasedOnP123();//����P1��P2��P3����Դ����

	void DRAConflictListener();//֡����ͻ

	/*--------------------��������--------------------*/
	void writeClusterPerformInfo(std::ofstream &out);
	void writeEventListInfo(std::ofstream &out);
};



