#pragma once
#include<vector>
#include<string>
#include"RSU.h"

class ceNB {
	//-----------------------TEST-----------------------
public:
	static int count;
	//-----------------------TEST-----------------------
public:
	const int m_eNBId=count++;//��վID

	std::list<int> m_RSUIdList;//�û�վ�е�RSU�������洢RSU��ID��
	std::list<int> m_VeUEIdList;//�û�վ�е�VeUE�������洢VeUE��ID��

	/***************************************************************
	------------------------����ģ��--------------------------------
	****************************************************************/
	bool m_UnassignedSubband[gc_RBNum];//������Դ�飬���Ϊtrueָʾδ�������RB
	std::vector<sScheduleInfo> m_vecScheduleInfo;//������Ϣ

	std::string toString(int n);//���ڴ�ӡ��վ��Ϣ
};