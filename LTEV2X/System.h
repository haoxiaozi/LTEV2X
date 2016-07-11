#pragma once
#include<vector>
#include<set>
#include<fstream>
#include"Schedule.h"
#include"Config.h"
#include"VUE.h"
#include"eNB.h"
#include"Message.h"
#include"Event.h"


class cSystem{
	//-----------------------TEST-----------------------
public:

	//-----------------------TEST-----------------------
private:
	/*------------------数据成员------------------*/
	sConfig m_Config;//系统配置参数
	int m_STTI;//仿真起始时刻TTI（随机生成的非负整数作为起始时刻）
	int m_TTI;//当前TTI时刻
	int m_NTTI;//仿真总共的TTI
	std::vector<ceNB> m_VeceNB;//基站容器
	std::vector<cRSU> m_VecRSU;//RSU容器
	std::vector<cVeUE> m_VecVUE;//车辆容器
	std::vector<std::list<sEvent>> m_EventList;//呼叫发起事件链表，m_CallSetupList[i][j]代表第i个TTI的事件表
	
public:
	/*------------------系统流程控制------------------*/
	void configure();//系统仿真参数配置
	void initialization();//系统参数配置，完成系统初始化
	void destroy();//释放资源
	void process();//系统仿真流程




	/***************************************************************
	------------------------调度模块--------------------------------
	****************************************************************/
public:
	/*--------------------接口函数--------------------*/
	void centralizedSchedule();//调度总控制

private:
	/*--------------------私有实现函数--------------------*/
	void scheduleInfoClean();//清除当前扇区所有用户的调度信息
	void schedulePF_RP_CSI_UL();//上行PF-RP调度

	//线性时间选取算法
public:
	sPFInfo selectKthPF(std::vector<sPFInfo>& vecF, int k, int p, int r);
	int partition(std::vector<sPFInfo>& vecF, int p, int r);
	void exchange(std::vector<sPFInfo>& vecF, int i, int j);



	/***************************************************************
	---------------------分布式资源管理-----------------------------
	-------------DRA:Distributed Resource Allocation----------------
	****************************************************************/
public :
	eDRAMode m_DRAMode;

	/*
	* DRA情况下RSU切换导致信息无法正常发送的VEId集合
	* 用于存放以下两种情况的VEId
	* 车辆发送信息完毕之前，进行了分簇，且分入了与原来不同的簇内
	* 车辆发送信息冲突，并且已经添加进对应RSU的冲突链表，但是在进行下一次重传之前，进行了分簇，并且分入了与原来不同的簇内
	* 总而言之，是存储信息发送尚未成功且发生VE所属RSU切换的VEId
	*/
	std::set<int> m_DRA_RSUSwitchSet;
	/*--------------------接口函数--------------------*/
	void DRASchedule();

private:
	/*--------------------实现函数--------------------*/
	void DRAInformationClean();//资源分配信息清空
	void DRAPerformCluster(bool clusterFlag);//对RSU内的车辆进行分簇
	void DRAGroupSizeBasedTDM();//基于簇大小的时分复用
	void DRABuildCallList();//建立呼叫链表


	void DRASelectBasedOnP13();//基于P1和P3的资源分配
	void DRASelectBasedOnP23();//基于P2和P3的资源分配
	void DRASelectBasedOnP123();//基于P1、P2和P3的资源分配

	void DRAConflictListener();//帧听冲突

	/*--------------------辅助函数--------------------*/
	void writeClusterPerformInfo(std::ofstream &out);
	void writeEventListInfo(std::ofstream &out);
};



