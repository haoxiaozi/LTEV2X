#pragma once
#include<vector>
#include"RSU.h"

class ceNB {
public:
	int m_eNBId;//��վID

	std::vector<int> m_VecRSU;//�û�վ�е�RSU�������洢RSU��ID��
	std::vector<int> m_VecVeUE;

	/***************************************************************
	------------------------����ģ��--------------------------------
	****************************************************************/
	bool m_UnassignedSubband[gc_RBNum];//������Դ�飬���Ϊtrueָʾδ�������RB
	std::vector<sScheduleInfo> m_vecScheduleInfo;//������Ϣ
};