#pragma once
#include<vector>
#include<string>
#include"Global.h"
#include"Schedule.h"


class ceNB {
	/*  TEST  */
	static int s_eNBCount;
	/*  TEST  */
public:
	const int m_eNBId=s_eNBCount++;//��վId

	std::list<int> m_RSUIdList;//�û�վ�е�RSU�������洢RSU��Id��
	std::list<int> m_VeUEIdList;//�û�վ�е�VeUE�������洢VeUE��Id��

	/***************************************************************
	------------------------����ģ��--------------------------------
	****************************************************************/
	bool m_UnassignedSubband[gc_RBNum];//������Դ�飬���Ϊtrueָʾδ�������RB
	std::vector<sScheduleInfo> m_vecScheduleInfo;//������Ϣ

	std::string toString(int n);//���ڴ�ӡ��վ��Ϣ
};