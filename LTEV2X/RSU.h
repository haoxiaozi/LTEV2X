#pragma once

#include<vector>
#include<list>
#include<string>
#include<fstream>
#include"Schedule.h"
#include"Global.h"
#include"VUE.h"

class cRSU {
	//-----------------------TEST-----------------------
public:
	static int count;
	cRSU();
	void print();
	void testCluster();
	//-----------------------TEST-----------------------
public:

	int m_RSUId;
	std::vector<int> m_VecVUE;//��ǰRSU��Χ�ڵ�UEid�������

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
	eRSUType m_RSUType;   //RSU������
	const int m_DRAClusterNum;  //һ��RSU���Ƿ�Χ�ڵĴصĸ�������RSU�������йأ�
	std::vector<std::tuple<int,int,int>> m_DRAClusterTDRInfo;//TDR(Time Domain Resource)�洢ÿ����������ʱ�������������˵㣬�Ҷ˵��Լ����䳤��
	std::vector<std::vector<int>> m_DRA_RBIsAvailable;  //��"g_TTI>m_DRA_RBIsAvailable[i][j]"�����i����Դ��j����;�ڲ�vector�洢���Ƕ�Ӧ��Դ����ռ�õ�TTIʱ�̣���TTI����ʱ�����
	std::vector<std::vector<int>> m_DRAVecCluster;   //��Ŵص�������ÿ���ذ���һ��vector<int>�洢������ID
	std::vector<std::vector<int>> m_DRACallList;   //���vector����һ���أ��ڲ�vector<int>����Ҫ�������ݵĳ���ID
	std::vector<std::vector<std::list<sDRAScheduleInfo>>> m_DRAScheduleList;  //��ǰ������Ϣ��[i][j]�����i���صĵ�j��RB��
	std::list<std::tuple<int,int,int>> m_DRAConflictList; //��ͻ�б�,tuple�����ҷֱ���VEId,clusterIdx,FBIdx

	/*--------------------�ӿں���--------------------*/
	int DRAGetClusterIdx(int TTI);//���ݴ˿̵�TTI���ص�ǰ���Խ�����Դ����Ĵصı��
	void DRAInformationClean();//��Դ������Ϣ���
	void DRAPerformCluster();//���зִ�
	void DRAGroupSizeBasedTDM();//���ڴش�С��ʱ�ָ���
	void DRABuildCallList(std::vector<cVeUE>&v);//������������
	
	void DRASelectBasedOnP13(int TTI,std::vector<cVeUE>&v);//����P1��P3����Դ����
	void DRASelectBasedOnP23(int TTI,std::vector<cVeUE>&v);//����P2��P3����Դ����
	void DRASelectBasedOnP123(int TTI,std::vector<cVeUE>&v);//����P1��P2��P3����Դ����

	void DRAReaddConflictListToCallList();//����һ��TTI��ͻ���û�()������ӵ�����������
	void DRAWriteScheduleInfo(std::ofstream& out);//д������Ϣ
	void DRAConflictListener(int TTI);//֡����ͻ
	void DRAConflictSolve(int TTI);//ά��m_DRAScheduleList�Լ�m_DRA_RBIsAvailable
	/*--------------------��������--------------------*/
private:
	int getMaxIndex(const std::vector<double>&v);
};