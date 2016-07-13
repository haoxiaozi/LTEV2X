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
	static int m_VeUECount;
	//-----------------------TEST-----------------------
public:
	const int m_VeUEId=m_VeUECount++;//�û�ID
	int m_RSUId;//���ڵ�RSU��Id
	int m_ClusterIdx;//���ڴ�de���

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


	std::string toString(int n);//���ڴ�ӡVeUE��Ϣ
};