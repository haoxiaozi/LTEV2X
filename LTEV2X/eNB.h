#pragma once
#include<vector>
#include"Sector.h"
#include"RSU.h"

class ceNB {
public:
	int m_CellId;//С��ID
	int m_eNBId;//��վID
	float m_X, m_Y;//�������
	float m_AbsX, m_AbsY;//��������
	int m_AntennaNum;//��������
	int m_SectorNum;//��������
	std::vector<int> m_VecSector;//�û�վ������������(�洢������ID)
	std::vector<int> m_VecRSU;//�û�վ�е�RSU�������洢RSU��ID��
	std::vector<int> m_VecVeUE;

	/***************************************************************
	------------------------����ģ��--------------------------------
	****************************************************************/
	bool m_UnassignedSubband[gc_RBNum];//������Դ�飬���Ϊtrueָʾδ�������RB
	std::vector<sScheduleInfo> m_vecScheduleInfo;//������Ϣ
};