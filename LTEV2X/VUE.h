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
	int m_VEId;//�û�ID

	sMessage m_Message;//��Ϣ

	/***************************************************************
	------------------------����ģ��--------------------------------
	****************************************************************/

	bool m_IsScheduledUL;    //UpLink�Ƿ��ڱ�����
	sFeedbackInfo m_FeedbackUL;//��Ҫ���͸���վ�˵ķ�����Ϣ

	std::vector<double> m_CQIPredictIdeal;
	std::vector<double> m_CQIPredictRealistic;

	/***************************************************************
	---------------------�ֲ�ʽ��Դ����-----------------------------
	****************************************************************/

	int RBSelectBasedOnP2(const std::vector<int> &v);
};