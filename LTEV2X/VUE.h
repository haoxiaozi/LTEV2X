#pragma once
#include<vector>
#include<list>
#include<string>
#include"Global.h"
#include"Schedule.h"
#include"Message.h"

class cVeUE {
	//-----------------------TEST-----------------------
public:
	static int count;
	cVeUE();
	//-----------------------TEST-----------------------
public:
	int m_VEId;//�û�ID
	int m_RSUId;//���ڵ�RSU��Id
	int m_ClusterIdx;//���ڴ�de���
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


	std::string toString();//���ڴ�ӡVeUE��Ϣ
};