#pragma once
#include<vector>
#include<string>
#include"RSU.h"

class ceNB {
	//-----------------------TEST-----------------------
public:
	static int count;
	ceNB();
	//-----------------------TEST-----------------------
public:
	int m_eNBId;//��վID

	std::list<int> m_RSUIdList;//�û�վ�е�RSU�������洢RSU��ID��
	std::list<int> m_VeUEList;//�û�վ�е�VeUE�������洢VeUE��ID��

	/***************************************************************
	------------------------����ģ��--------------------------------
	****************************************************************/
	bool m_UnassignedSubband[gc_RBNum];//������Դ�飬���Ϊtrueָʾδ�������RB
	std::vector<sScheduleInfo> m_vecScheduleInfo;//������Ϣ

	std::string toString();//���ڴ�ӡ��վ��Ϣ
};