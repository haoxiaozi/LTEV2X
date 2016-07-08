#pragma once
#include<vector>
#include<list>
#include"Global.h"
#include"Schedule.h"
#include"Message.h"

class cVeUE {

public:
	int m_UserId;//�û�ID

	sMessage m_Message;//��Ϣ

	/***************************************************************
	------------------------����ģ��--------------------------------
	****************************************************************/

	bool m_IsScheduledUL;    //UpLink�Ƿ��ڱ�����
	bool m_IsScheduledDL;    //DownLink�Ƿ��ڱ�����
	sFeedbackInfo m_FeedbackDL;//��Ҫ���͸���վ�˵ķ�����Ϣ
	sFeedbackInfo m_FeedbackUL;//��Ҫ���͸���վ�˵ķ�����Ϣ

	std::vector<double> m_CQIPredictIdeal;
	std::vector<double> m_CQIPredictRealistic;

	bool isScheduled(eLinkType);//��ǰTTI���û��Ƿ񱻵���

	/***************************************************************
	---------------------�ֲ�ʽ��Դ����-----------------------------
	****************************************************************/
	bool m_isHavingDataToTransmit;

	int RBSelectBasedOnP2(const std::vector<int> &v);
};