#pragma once

#include<vector>
#include<list>
#include<string>
#include<fstream>
#include"Schedule.h"
#include"Global.h"

class cRSU {
	//-----------------------TEST-----------------------
public:
	static int count;
	cRSU();
	void print();
	void Cluster();
	std::string path = "E:\\LTEV2X\\output\\DRAScheduleInfo.txt";
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
	const int m_ClusterNum;  //һ��RSU���Ƿ�Χ�ڵĴصĸ�������RSU�������йأ�
	std::vector<std::tuple<int,int,int>> m_DRAClusterTTI;//�洢ÿ����������ʱ�������������˵㣬�Ҷ˵��Լ����䳤��
	std::vector<std::vector<int>> m_DRA_RBIsAvailable;  //��"m_DRA_CNTI<=m_DRA_RBIsAvailable[i][j]"�����i����Դ��j����;�ڲ�vector�洢���Ƕ�Ӧ��Դ����ռ�õ�ʱ��
	std::vector<std::vector<int>> m_Cluster;   //��Ŵص�������ÿ���ذ���һ��vector<int>�洢������ID
	std::vector<std::vector<int>> m_CallList;   //���vector����һ���أ��ڲ�vector<int>����Ҫ�������ݵĳ���ID
	std::vector<std::vector<sDRAScheduleInfo>> m_DRAScheduleList;  //��ǰ������Ϣ��[i][j]�����i���صĵ�j��RB��


	/*--------------------�ӿں���--------------------*/
	int getDRAClusterIdx();//���ݴ˿̵�m_DRA_CNTI���ص�ǰ���Խ�����Դ����Ĵصı��
	void DRAPerformCluster();//���зִأ�����ÿ���ط����Ӧ��ʱ����Դ
	void writeDRAScheduleInfo();
	/*--------------------��������--------------------*/
private:
	int getMaxIndex(const std::vector<double>&v);
};