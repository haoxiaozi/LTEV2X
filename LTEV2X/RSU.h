#pragma once

#include<vector>
#include<list>
#include"Schedule.h"
#include"Global.h"

class cRSU {
public:
	cRSU() :mc_DRA_MTI(0.1f), mc_DRA_NTI(25), m_ClusterNum(4) {}

	std::vector<int> m_VecVUE;//当前RSU范围内的UEid编号容器

	/***************************************************************
	------------------------上行调度--------------------------------
	****************************************************************/
	double m_AccumulateThroughput;   //累计吞吐量
	bool m_IsScheduledUL;    //UpLink是否在被调度
	bool m_IsScheduledDL;    //DownLink是否在被调度
	double m_FactorPF[gc_SubbandNum];
	double m_SINR[gc_SubbandNum];
	std::list<int> m_HIndicatorUL; //传输数据位置
	std::list<int> m_HIndicatorDL; //传输数据位置
	int  m_RBs[gc_MaxCodewords];  //频域*空间
	cServingSector m_ServingSet;
	sFeedbackInfo m_FeedbackDL;//将要发送给基站端的反馈信息
	sFeedbackInfo m_FeedbackUL;//将要发送给基站端的反馈信息

	std::vector<double> m_CQIPredictIdeal;
	std::vector<double> m_CQIPredictRealistic;
	int m_ServingSectorId; //用户属于扇区的ID = 小区ID*每小区扇区数+小区内扇区编号 主服务小区

	bool isScheduled(eLinkType);//当前TTI该用户是否被调度


	/***************************************************************
	---------------------分布式资源管理-----------------------------
	-------------DRA:Distributed Resource Allocation----------------
	****************************************************************/
	eRSUType m_RSUType;   //RSU的类型
	const int m_ClusterNum;
	const float mc_DRA_MTI; //分布式资源分配最小时隙:Distributed Resource Allocation Minimum Time Interval
	const int mc_DRA_NTI; //所有簇进行一次DRA所占用的TI数量。(NTI:number of Time Interval)
	int m_DRA_CNTI;   //当前RSU所处的时隙，不同的RSU可以不同，用于同步DRA（CNTI:Current Number of Time Interval)
	std::vector<int> m_DRA_ETI;//存储每个簇所分配时间区间的右端点(ETI:End Time Interval)
	std::vector<bool> m_DRA_RB;  //频域资源块,true代表资源块可用
	std::vector<std::vector<int>> m_Cluster;   //存放簇的容器，每个簇包含一个vector<int>存储车辆的ID
	std::vector<std::vector<int>> m_CallList;   //外层vector代表一个簇，内层vector<int>代表要传输数据的车辆ID


	/*--------------------接口函数--------------------*/
	int getDRAClusterIdx();//根据此刻的m_DRA_CNTI返回当前可以进行资源分配的簇的编号
	void DRAPerformCluster();
};