#pragma once
#include<vector>
#include<list>
#include"Global.h"
#include"Schedule.h"
#include"Message.h"

class cVeUE {
	//-----------------------TEST-----------------------
public:
	static int count;
	cVeUE();
	void print();
	//-----------------------TEST-----------------------
public:
	int m_VEId;//用户ID

	sMessage m_Message;//消息

	/***************************************************************
	------------------------调度模块--------------------------------
	****************************************************************/

	bool m_IsScheduledUL;    //UpLink是否在被调度
	sFeedbackInfo m_FeedbackUL;//将要发送给基站端的反馈信息

	std::vector<double> m_CQIPredictIdeal;
	std::vector<double> m_CQIPredictRealistic;

	/***************************************************************
	---------------------分布式资源管理-----------------------------
	****************************************************************/
	bool m_isHavingDataToTransmit;

	int RBSelectBasedOnP2(const std::vector<int> &v);
};