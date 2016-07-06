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
	PFInfo selectKthPF(std::vector<PFInfo>& vecF, int k, int p, int r);
	int partition(std::vector<PFInfo>& vecF, int p, int r);
	void exchange(std::vector<PFInfo>& vecF, int i, int j);



	/***************************************************************
	---------------------�ֲ�ʽ��Դ����-----------------------------
	****************************************************************/
public :
	/*--------------------�ӿں���--------------------*/
	void distributedSchedule();

private:
	void performCluster();//��RSU�ڵĳ������зִ�
	void frequencyResourceSelect();//��ÿ��RSU�ڵ�С�أ���С��ָ���ڵ�ǰRSU_TII������Դ������С�أ�������Դѡ��
};



