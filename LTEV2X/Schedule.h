#pragma once
#include<vector>
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
	int VEId;//����ID
	int FBIdx;//Ƶ�����
	std::vector<std::tuple<int, int>> occupiedInterval;//��ǰ�������д����ʵ��TTI���䣨�����䣩

	sDRAScheduleInfo() {}
	sDRAScheduleInfo(int VEId,int FBIdx, std::tuple<int, int, int>ClasterTTI, int occupiedTTI) :occupiedInterval(std::vector<std::tuple<int, int>>(0)) {
		this->VEId = VEId;
		this->FBIdx = FBIdx;
		int begin = std::get<0>(ClasterTTI),
			end = std::get<1>(ClasterTTI),
			len = std::get<2>(ClasterTTI);
		int relativeTTI = g_TTI%gc_DRA_NTTI;
		int nextTurnBeginTTI = g_TTI - relativeTTI + gc_DRA_NTTI;//��RSU��һ�ֵ��ȵ���ʼTTI����һ���صĿ�ʼʱ�̣�
		int remainTTI = end - relativeTTI + 1;//��ǰһ�ֵ�����ʣ����õ�ʱ϶����
		int overTTI = occupiedTTI - remainTTI;//������ǰһ�ֵ��ȿ���ʱ϶�����Ĳ���
		if (overTTI <= 0) occupiedInterval.push_back(std::tuple<int, int>(g_TTI, g_TTI + occupiedTTI - 1));
		else {
			occupiedInterval.push_back(std::tuple<int, int>(g_TTI, g_TTI + remainTTI - 1));
			int n = overTTI / len;
			for (int i = 0; i < n; i++) occupiedInterval.push_back(std::tuple<int, int>(nextTurnBeginTTI + i*gc_DRA_NTTI + begin, nextTurnBeginTTI + begin + len - 1 + i*gc_DRA_NTTI));
			if (overTTI%len != 0) occupiedInterval.push_back(std::tuple<int, int>(nextTurnBeginTTI + n*gc_DRA_NTTI + begin, nextTurnBeginTTI + begin + n*gc_DRA_NTTI + overTTI%len - 1));
		}
	}
	

	/*------------------�����õĺ���------------------*/
	void print() {
		std::cout << "OccupiedInterval: ";
		for (std::tuple<int, int> t : occupiedInterval)
			std::cout << "[ " << std::get<0>(t) << " , " << std::get<1>(t) << " ] , ";
		std::cout << std::endl;
	}
};