#pragma once
#include<vector>
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


class cServingSector {
public:
	int servingSectorId[gc_MaxCoMP];
	int servingSectorNum;
	bool IsServing(int sectorId) {
		bool ret = false;
		for (int i = 0; i != servingSectorNum; i++) {
			if (servingSectorId[i] == sectorId) {
				ret = true;
				break;
			}
		}
		return ret;
	};
	void AppendSector(int sectorId) {
		if (servingSectorNum + 1 >= gc_MaxCoMP)
			throw Exp("AppendSector: sectorId large than c_maxCoMP");
		servingSectorId[servingSectorNum] = sectorId;
		servingSectorNum++;
	};
	void clear() { servingSectorNum = 0; }
};


struct PFInfo {//������PF���е����㷨����������
	int RSUId;
	int SubbandId;
	double FactorPF;
	PFInfo(int RSUId, int SubbandId, double FactorPF) {
		this->RSUId = RSUId;
		this->SubbandId = SubbandId;
		this->FactorPF = FactorPF;
	}
	PFInfo(const PFInfo& t) {
		this->RSUId = t.RSUId;
		this->SubbandId = t.SubbandId;
		this->FactorPF = t.FactorPF;
	}
	PFInfo& operator=(const PFInfo& t) {
		this->RSUId = t.RSUId;
		this->SubbandId = t.SubbandId;
		this->FactorPF = t.FactorPF;
		return *this;
	}
};


