#pragma once

#include<vector>
#include<set>
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
	void print();
	void testCluster();
	//-----------------------TEST-----------------------
public:

	int m_RSUId;
	std::set<int> m_VUESet;//��ǰRSU��Χ�ڵ�VEId�������

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
	* RSU������
	* ����ʮ��·�ڵ�RSU
	* �ֲ��ڵ�·�м��RSU
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
	* ��ų���������
	* �±����صı��
	* ���ڳ�����set��ά���������������ú�������ݽṹ�ж��¼������ж�Ӧ�¼��ĳ���ID�Ƿ�����ڸ�set�У����Ч��
	*/
	std::vector<std::set<int>> m_DRAClusterVUESet;  

	/*
	* ���ڴ�ŵ�ǰTTI���г����ı��
	*/
	std::vector<int> m_DRACallList; 

	/*
	* ��ŵ��ȵ�����Ϣ
	* ����±����ر��
	* �ڲ��±����FB���
	*/
	std::vector<std::vector<std::list<sDRAScheduleInfo>>> m_DRAScheduleList;
	
	/*
	* ��ͻ�б�
	* �б�Ԫ����tuple���ֱ����VEId,ClusterIdx�Լ�FBIdx
	* ����ClusterIdx�Լ�FBIdx�����ڳ�ͻ�����ͷŶ�Ӧ��Ƶ����Դ
	*/
	std::set<std::tuple<int,int,int>> m_DRAConflictSet;

	/*--------------------�ӿں���--------------------*/
	int DRAGetClusterIdx(int TTI);//���ݴ˿̵�TTI���ص�ǰ���Խ�����Դ����Ĵصı��
	void DRAInformationClean();//��Դ������Ϣ���
	void DRAPerformCluster();//���зִ�
	void DRAGroupSizeBasedTDM();//���ڴش�С��ʱ�ָ���
	void DRABuildCallList(int TTI,const std::vector<std::list<sEvent>>& eventList);//������������
	
	void DRASelectBasedOnP13(int TTI,std::vector<cVeUE>&v);//����P1��P3����Դ����
	void DRASelectBasedOnP23(int TTI,std::vector<cVeUE>&v);//����P2��P3����Դ����
	void DRASelectBasedOnP123(int TTI,std::vector<cVeUE>&v);//����P1��P2��P3����Դ����

	void DRAReaddConflictListToCallList(int TTI);//����һ��TTI��ͻ���û�()������ӵ�����������
	void DRAWriteScheduleInfo(std::ofstream& out);//д������Ϣ
	void DRAConflictListener(int TTI);//֡����ͻ
	void DRAConflictSolve(int TTI);//ά��m_DRAScheduleList�Լ�m_DRA_RBIsAvailable
	/*--------------------��������--------------------*/
private:
	int getMaxIndex(const std::vector<double>&v);
};