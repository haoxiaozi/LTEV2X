#pragma once
#include<vector>
#include<list>
#include<utility>
#include<iostream>
#include<tuple>
#include"Global.h"

struct sScheduleInfo {
	int userId;//��ǰС���ڸ�RB�ϵ��ȵ����û���
	std::vector<int> assignedSubbandId;//���û���������Ӵ��ı��[0-gc_SubbandNum)
	sScheduleInfo() :userId(-1), assignedSubbandId(std::vector<int>(0)) {}
};


struct sFeedbackInfo {
public:
	std::vector<float> CQIWideband;//���CQI
	std::vector<float> CQISubband;//խ��CQI

	sFeedbackInfo& operator=(const sFeedbackInfo&f) {//������ֵ�����
		CQIWideband = f.CQIWideband;
		CQISubband = f.CQISubband;
		return *this;
	}
};





struct sPFInfo {//������PF���е����㷨����������
	int RSUId;
	int SubbandId;
	double FactorPF;
	sPFInfo(int RSUId, int SubbandId, double FactorPF) {
		this->RSUId = RSUId;
		this->SubbandId = SubbandId;
		this->FactorPF = FactorPF;
	}
	sPFInfo(const sPFInfo& t) {
		this->RSUId = t.RSUId;
		this->SubbandId = t.SubbandId;
		this->FactorPF = t.FactorPF;
	}
	sPFInfo& operator=(const sPFInfo& t) {
		this->RSUId = t.RSUId;
		this->SubbandId = t.SubbandId;
		this->FactorPF = t.FactorPF;
		return *this;
	}
};


struct sDRAScheduleInfo {
	int eventId;//�¼����
	int RSUId;//RSU���
	int patternIdx;//Ƶ�����
	std::list<std::tuple<int, int>> occupiedIntervalList;//��ǰVeUE���д����ʵ��TTI���䣨�����䣩

	sDRAScheduleInfo() {}
	sDRAScheduleInfo(int eventId,int RSUId,int patternIdx, const std::list<std::tuple<int, int>> &occupiedIntervalList) {
		this->eventId = eventId;
		this->RSUId = RSUId;
		this->patternIdx = patternIdx;
		this->occupiedIntervalList = occupiedIntervalList;
	}
	
	std::string toLogString(int n);

	std::string toString(int n);

	/*------------------�����õĺ���------------------*/
	void print() {
		std::cout << "OccupiedInterval: ";
		for (const std::tuple<int, int> &t : occupiedIntervalList)
			std::cout << "[ " << std::get<0>(t) << " , " << std::get<1>(t) << " ] , ";
		std::cout << std::endl;
	}
};