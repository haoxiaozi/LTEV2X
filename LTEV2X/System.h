#pragma once
#include<vector>
#include<fstream>
#include"Schedule.h"
#include"Config.h"
#include"eNB.h"
#include"RSU.h"
#include"VUE.h"
#include"Event.h"
#include"Road.h"


class cSystem{
	//LK
	sConfigure conf;//ϵͳ��������
	//void configure();
	//void Initialization();//ϵͳ��ʼ��
	void Destroy();//ϵͳ��Դ�ͷ�
	void Process();//ϵͳ�������

public:
	void NewChannel(unsigned short UEID);
	void DeleteChannel(unsigned short UEID);
	void ChannelGeneration();//�ŵ�ˢ��
	void FreshLoc(void);
	void CalChannel(void);
	int allusers;
	int freshNum;


private:
	/*------------------���ݳ�Ա------------------*/
	int m_TTI;//��ǰ��TTIʱ��
	int m_NTTI;//�����ܹ���TTI
	ceNB* eNB;//��վ����
	cRoad *Road;
	cRSU* RSU;//RSU����
	cVeUE* veUE;//VeUE����
	std::vector<sEvent> m_EventVec;//�¼�����
	
	/*
	* ����±�Ϊʱ��ۣ�����TTI��
	* ���¼�������ͬ���¼�����������ͬʱ�̴������¼���Id������ͬ��ʱ�����
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


	/*--------------------------------------------------------------
	*                      ���е���
	* -------------------------------------------------------------*/
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



	/*--------------------------------------------------------------
	*                      �ֲ�ʽ��Դ����
	*            DRA:Distributed Resource Allocation
	* -------------------------------------------------------------*/
public :
	eDRAMode m_DRAMode;

	std::list<int> m_DRASwitchEventIdList;//���ڴ�Ž���RSU�л��ĳ�������ʱ���������


	/*--------------------�ӿں���--------------------*/
	void DRASchedule();//DRA�����ܿ�

private:
	/*--------------------ʵ�ֺ���--------------------*/
	void DRAInformationClean();//��Դ������Ϣ���
	void DRAPerformCluster(bool clusterFlag);//��RSU�ڵ�VeUE���зִ�
	void DRAGroupSizeBasedTDM(bool clusterFlag);//���ڴش�С��ʱ�ָ���
	void DRAUpdateAdmitEventIdList(bool clusterFlag);//������������


	void DRASelectBasedOnP13();//����P1��P3����Դ����
	void DRASelectBasedOnP23();//����P2��P3����Դ����
	void DRASelectBasedOnP123();//����P1��P2��P3����Դ����

	void DRAWriteScheduleInfo();//��¼������Ϣ��־
	void DRADelaystatistics();//ʱ��ͳ��
	void DRAConflictListener();//֡����ͻ

	/*--------------------��������--------------------*/
	void writeClusterPerformInfo(std::ofstream &out);//д��ִ���Ϣ����־
	void writeEventListInfo(std::ofstream &out);//д��ʱ���б����Ϣ
	void writeEventLogInfo(std::ofstream &out);//д�����¼�����־��Ϣ
	void writeVeUELocationUpdateLogInfo(std::ofstream &out); //д�����λ�ø�����־
};



