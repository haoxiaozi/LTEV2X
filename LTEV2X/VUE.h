#pragma once
#include<vector>
#include<list>
#include<string>
#include<tuple>
#include"Global.h"
#include"Schedule.h"
#include"Enumeration.h"
#include"Config.h"
#include"IMTA.h"


class cVeUE {
public:
	static int m_VeUECount;

	/*--------------------------------------------------------------
	*                      地理拓扑单元
	* -------------------------------------------------------------*/
	void initialize(sUEConfigure &t_UEConfigure);
	unsigned short m_wRoadID;
	int m_locationID;
	const int m_VeUEId = m_VeUECount++;
	unsigned short m_RSUId;
	unsigned short m_ClusterIdx;
	float m_fX;
	float m_fY;
	float m_fAbsX;
	float m_fAbsY;
	float m_fv;
	float m_fvAngle;
	float m_fantennaAngle;
	//cChannelModel *channelModel;
	cIMTA *imta;


	/*--------------------------------------------------------------
	*                      集中式资源管理单元
	* -------------------------------------------------------------*/

	bool m_IsScheduledUL;    //UpLink是否在被调度
	sFeedbackInfo m_FeedbackUL;//将要发送给基站端的反馈信息

	std::vector<double> m_CQIPredictIdeal;
	std::vector<double> m_CQIPredictRealistic;

	/*--------------------------------------------------------------
	*                      分布式资源管理单元
	*            DRA:Distributed Resource Allocation
	* -------------------------------------------------------------*/
	std::tuple<int, int> m_ScheduleInterval;//该VeUE在当前簇内当前一轮调度区间
	std::list<std::tuple<int, int>> m_LocationUpdateLogInfoList;//地理位置更新日志信息

	int RBSelectBasedOnP2(const std::vector<std::vector<int>>&curAvaliablePatternIdx, eMessageType messageType);
	int RBEmergencySelectBasedOnP2(const std::vector<int>&curAvaliableEmergencyPatternIdx);

	std::string toString(int n);//用于打印VeUE信息
};

inline
int cVeUE::RBSelectBasedOnP2(const std::vector<std::vector<int>>&curAvaliablePatternIdx, eMessageType messageType) {
	int size = static_cast<int>(curAvaliablePatternIdx[messageType].size());
	if (size == 0) return -1;
	return curAvaliablePatternIdx[messageType][rand() % size];
}

inline
int cVeUE::RBEmergencySelectBasedOnP2(const std::vector<int>&curAvaliableEmergencyPatternIdx) {
	int size = static_cast<int>(curAvaliableEmergencyPatternIdx.size());
	if (size == 0) return -1;
	return curAvaliableEmergencyPatternIdx[rand() % size];
}