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
	std::vector<ceNB> m_VeceNB;//��վ����
	std::vector<cVeUE> m_VecVUE;//��������
	std::vector<cRSU> m_VecRSU;//RSU����
	
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
	void DRAPerformCluster();//��RSU�ڵĳ������зִ�
	void DRAGroupSizeBasedTDM();//���ڴش�С��ʱ�ָ���
	void DRAInformationClean();//��Դ������Ϣ���
	void DRAbuildCallList();//������������


	void DRABasedOnP13();
	void DRABasedOnP23();
	void DRABasedOnP123();

	/*--------------------��������--------------------*/
};



