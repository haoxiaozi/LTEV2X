/*
* =====================================================================================
*
*       Filename:  Scheduling.cpp
*
*    Description:  ����ģ��
*
*        Version:  1.0
*        Created:
*       Revision:
*       Compiler:  VS_2015
*
*         Author:  HCF
*            LIB:  ITTC
*
* =====================================================================================
*/

#include<vector>
#include<iomanip>
#include<tuple>
#include<fstream>
#include"Schedule.h"
#include"System.h"
#include"Exception.h"
#include"Definition.h"
#include"Global.h"

using namespace std;

extern ofstream g_OutDRAScheduleInfo;

void cSystem::centralizedSchedule() {
	//�����һ�ε�����Ϣ
	scheduleInfoClean();

	schedulePF_RP_CSI_UL();
	
}


void cSystem::scheduleInfoClean() {
	for (ceNB &_eNB : m_VeceNB) 
		_eNB.m_vecScheduleInfo.clear();
	
}


void cSystem::schedulePF_RP_CSI_UL() {
	for (ceNB &_eNB : m_VeceNB) {//��ÿһ����վ����һ�ε���
		int k = _eNB.m_VecRSU.size();
		vector<vector<bool>> SPU(k, vector<bool>(gc_RBNum));//ÿ��RSU��һ��vector<bool>��������SPU��true������ѡ����Ӵ�
		vector<int> S;//δ������Ӵ�����(�洢�Ӵ���ID��
		
		//��ʼ���Ӵ�����S��������Ҫ����Ҫ��ÿ�ε���ǰӦ��û���Ӵ���ռ�ðɣ�
		for (int subbandId = 0; subbandId < gc_RBNum; subbandId++) 
			if (_eNB.m_UnassignedSubband)S.push_back(subbandId);
		

		//����ÿ��RSU��Ӧ��ͬ�Ӵ���PF����
		vector<sPFInfo> F;//�洢PF���ӵ�����
		for (int RSUId : _eNB.m_VecRSU) {
			for (int subbandId = 0; subbandId < gc_RBNum; subbandId++) {
				if (_eNB.m_UnassignedSubband[subbandId] == false) continue;//���Ӵ��ѱ�����
				double t_FactorPF= log10(1 + m_VecRSU[RSUId].m_SINR[subbandId]) / m_VecRSU[RSUId].m_AccumulateThroughput;
				F.push_back(sPFInfo(RSUId, subbandId, t_FactorPF));
			}
		}

		//��ʼ�����㷨
		int p = 1;
		while (S.size() != 0) {
			sPFInfo pPFInfo = selectKthPF(F, p, 0, F.size() - 1);
			int u = pPFInfo.RSUId;
			int v = pPFInfo.SubbandId;

			if (SPU[u].size() == 0 || (v>0 && SPU[u][v - 1] || v < gc_RBNum - 1 && SPU[u][v + 1])) {
				S.erase(S.begin() + v);
				SPU[u][v] = true;
				F.erase(F.begin() + p - 1);//����ע�⣬p�������˳��������1��ʼ��
			}
			else {
				p++;
			}
		}

		//д����Ƚ����(�ٴ���
		for (int RSUId = 0; RSUId < k; RSUId++) {
			sScheduleInfo curRSUScheduleInfo;
			curRSUScheduleInfo.userId = RSUId;
			for (int subbandId = 0; subbandId < gc_RBNum; subbandId++) {
				if (SPU[RSUId][subbandId]) curRSUScheduleInfo.assignedSubbandId.push_back(subbandId);
			}
			if (curRSUScheduleInfo.assignedSubbandId.size() != 0) _eNB.m_vecScheduleInfo.push_back(curRSUScheduleInfo);
		}
	}
}


sPFInfo cSystem::selectKthPF(std::vector<sPFInfo>& v, int k,int p,int r) {
	if (p == r) return v[p];
	int q = partition(v, p, r);
	int n = q - p + 1;
	if (n == k) return v[q];
	else if (n > k) return  selectKthPF(v, k, p, q - 1);
	else return selectKthPF(v, k - n, q + 1, r);		
}

int cSystem::partition(std::vector<sPFInfo>& v, int p, int r) {
	int k = p - 1;
	double x = v[r].FactorPF;
	for (int j = p; j <= r - 1; j++) {
		if (v[j].FactorPF < x) {
			k++;
			exchange(v, k, j);
		}
	}
	exchange(v, k + 1, r);
	return k + 1;
}

void cSystem::exchange(std::vector<sPFInfo>& v, int i, int j) {
	sPFInfo tem = v[i];
	v[i] = v[j];
	v[j] = tem;
}


void cSystem::DRASchedule() {

	/*��Դ������Ϣ���:����ÿ��RSU�ڵ�m_CallList��*/
	DRAInformationClean();

	/*���ݵ���λ�ý��зִ�*/
	DRAPerformCluster();

	/*���ݴش�С����ʱ����Դ�Ļ���*/
	DRAGroupSizeBasedTDM();

	/*����������������RSU�ڵ�m_VecVUE������m_CallList*/
	DRABuildCallList();

	/*��ǰm_TTI��DRA�㷨*/
	switch (m_DRAMode) {
	case P13:
		DRASelectBasedOnP13();
		break;
	case P23:
		DRASelectBasedOnP23();
		break;
	case P123:
		DRASelectBasedOnP123();
		break;
	}

	/*֡����ͻ*/
	DRAConflictListener();

}


void cSystem::DRAInformationClean() {
	for (cRSU &_RSU : m_VecRSU)
		_RSU.DRAInformationClean();
}


void cSystem::DRAPerformCluster() {
	for (cRSU& _RSU : m_VecRSU)
		_RSU.DRAPerformCluster();

	//-----------------------TEST-----------------------

	for (cRSU& _RSU : m_VecRSU) {
		cout << "RSU: " << _RSU.m_RSUId << " 's Cluster  :" << endl;
		for (const vector<int>& v : _RSU.m_DRAVecCluster) {
			cout << "    :";
			for (int VEId : v)
				cout << VEId << " , ";
			cout << endl;
		}
		cout << endl;
	}
	//-----------------------TEST-----------------------
}


void cSystem::DRABuildCallList() {
	for (cRSU &_RSU : m_VecRSU)
		_RSU.DRABuildCallList(m_VecVUE);

	//-----------------------TEST-----------------------
	for (cRSU& _RSU : m_VecRSU) {
		cout << "RSU: " << _RSU.m_RSUId << " 's ClusterCallList  :" << endl;
		for (const vector<int>& v : _RSU.m_DRACallList) {
			cout << "    :";
			for(int VEId:v)
				cout << VEId << " , ";
			cout << endl;
		}
		cout << endl;
	}
	//-----------------------TEST-----------------------
}





void cSystem::DRAGroupSizeBasedTDM() {
	for (cRSU& _RSU : m_VecRSU) 
		_RSU.DRAGroupSizeBasedTDM();
}


void cSystem::DRASelectBasedOnP13() {

}

void cSystem::DRASelectBasedOnP23() {

}

void cSystem::DRASelectBasedOnP123() {
	for (cRSU &_RSU : m_VecRSU)
		_RSU.DRASelectBasedOnP123(m_TTI,m_VecVUE);
}



void cSystem::DRAConflictListener() {
	for (cRSU &_RSU : m_VecRSU) {
		_RSU.DRAConflictListener(m_TTI);
	}


	//-----------------------TEST-----------------------

	for (cRSU& _RSU : m_VecRSU) {
		cout << "RSU: " << _RSU.m_RSUId << " 's DRAConflictList  : ";
		for (tuple<int,int,int> &t : _RSU.m_DRAConflictList) {
			cout << "[ " << get<0>(t) << " , " << get<1>(t) << " , " << get<2>(t) << " ]  ,";
		}
		cout << endl;
	}
	//-----------------------TEST-----------------------
}
