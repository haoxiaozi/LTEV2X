#pragma once
#include<vector>
#include"Schedule.h"
#include"Config.h"
#include"VUE.h"
#include"Sector.h"
#include"eNB.h"
#include"Message.h"


class cSystem{
private:
	/*------------------���ݳ�Ա------------------*/
	sConfig m_Config;//ϵͳ���ò���
	int m_TTI;//����Ŀǰ���е���TTI
	std::vector<Sector> m_VecSector;//��������
	std::vector<ceNB> m_VeceNB;//��վ����
	std::vector<cVeUE> m_VecVUE;//��������
	std::vector<cRSU> m_VecRSU;
	EventList m_EventListCallSetup;//���з����¼�����
	EventList m_EventListHandover;//�л��¼�����
	EventList m_EventListCac;//���ɿ����¼�����
	EventList m_EnentListCallEnd;//���н����¼�����


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
	void scheduleDistribute();//�Ѳ��ֵ�����Ϣд��UE��
	void scheduleInfoClean();//�����ǰ���������û��ĵ�����Ϣ
	void feedbackInfoReceived();//��������Ϣд���վ��
	void feedbackInfoDelay();
	void feedbackInfoSent();//��������Ϣд�뵽�û�������ָ����
	void schedulePF_RP_CSI_UL();//����PF-RP����
	void scheduleMCS();//ȷ�����е����û���MCS
	void scheduleInfoDelay();

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



