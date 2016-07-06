#pragma once
#include<list>
#include<vector>
#include"Schedule.h"

class Sector {
public:
	int m_CellId;//С��ID
	int m_eNBId;//��վID
	int m_SectorId;//����ID
	float m_AntennaAngle;//���߽�
	int m_AntennaTxNumber; //����ʱ����������
	int m_AntennaRxNumber; //����ʱ����������
	float m_X, m_Y;//�������С��������
	float m_AbsX, m_AbsY;//��������
	float m_RSPower;//��Ƶ����
	float m_DataPower;//���ݷ��͹���
	float m_SumPower;//�ܹ���


	/***************************************************************
	------------------------����ģ��--------------------------------
	****************************************************************/

	std::list<int> admitUserIdListDL;//�����û��������л�(�洢�û���ID)
	std::list<int> admitUserIdListUL;//�����û��������л�(�洢�û���ID)

	std::vector<sScheduleInfo> scheduleInfoPerHDL;//ÿƵ�����е�����Ϣ���洢��ǰһ�εĵ�����Ϣ��
	std::vector<sScheduleInfo> scheduleInfoPerHUL;//ÿƵ�����е�����Ϣ���洢��ǰһ�εĵ�����Ϣ��

	std::vector<std::vector<sScheduleInfo>> vecScheduleInfoUL;//�ۼ����е�����Ϣ
	std::vector<std::vector<sScheduleInfo>> vecScheduleInfoDL;//�ۼ����е�����Ϣ


	/***************************************************************
	---------------------�ֲ�ʽ��Դ����-----------------------------
	****************************************************************/


};