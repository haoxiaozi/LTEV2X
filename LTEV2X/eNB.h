#pragma once
#include<vector>
#include<string>
#include<set>
#include"RSU.h"

class ceNB {
	//-----------------------TEST-----------------------
public:
	static int count;
	ceNB();
	//-----------------------TEST-----------------------
public:
	int m_eNBId;//��վID

	std::set<int> m_RSUSet;//�û�վ�е�RSU�������洢RSU��ID��
	std::set<int> m_VUESet;//�û�վ�е�VE�������洢VE��ID��

	/***************************************************************
	------------------------����ģ��--------------------------------
	****************************************************************/
	bool m_UnassignedSubband[gc_RBNum];//������Դ�飬���Ϊtrueָʾδ�������RB
	std::vector<sScheduleInfo> m_vecScheduleInfo;//������Ϣ

	std::string toString();//���ڴ�ӡ��վ��Ϣ
};