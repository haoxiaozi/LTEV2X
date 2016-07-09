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
#include"Schedule.h"
#include"System.h"
#include"Exception.h"
#include"Definition.h"
#include"Global.h"

using namespace std;

extern ofstream out;

void cSystem::centralizedSchedule() {
	//�����һ�ε�����Ϣ
	scheduleInfoClean();

	schedulePF_RP_CSI_UL();
	
}


void cSystem::scheduleInfoClean() {
	for (ceNB _eNB : m_VeceNB) 
		_eNB.m_vecScheduleInfo.clear();
	
}


void cSystem::schedulePF_RP_CSI_UL() {
	for (ceNB& _eNB : m_VeceNB) {//��ÿһ����վ����һ�ε���
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

	/*----------------------
	��Դ������Ϣ���
	����ÿ��RSU�ڵ�m_CallList��
	-----------------------*/
	DRAInformationClean();


	/*----------------------
	����������������RSU�ڵ�m_CallList
	ת����m_CallList
	-----------------------*/
	DRAPerformCluster();

	DRAbuildCallList();

	switch (m_DRAMode) {
	case P13:
		DRABasedOnP13();
		break;
	case P23:
		DRABasedOnP23();
		break;
	case P123:
		DRABasedOnP123();
		break;
	}

}


void cSystem::DRAInformationClean() {
	for (cRSU &_RSU : m_VecRSU) {
		for (vector<int> curCluster : _RSU.m_CallList)
			curCluster.clear();
	}
}

void cSystem::DRAbuildCallList() {
	for (cRSU &_RSU : m_VecRSU) {
		//����m_VecRSU����m_CallList
		for (int clusterIdx = 0; clusterIdx < _RSU.m_Cluster.size(); ++clusterIdx) {
			_RSU.m_CallList[clusterIdx].clear();
			for (int UEId : _RSU.m_Cluster[clusterIdx]) {
				if (m_VecVUE[UEId].m_isHavingDataToTransmit) //������������Ҫ����������ӵ�m_CallList����
					_RSU.m_CallList[clusterIdx].push_back(UEId);
			}
		}
	}
}


void cSystem::DRAPerformCluster() {
	/*
	�ٶ��Ѿ��ִ���ϣ�ÿ��RSU��
	*/
	for (cRSU& _RSU : m_VecRSU) {
		_RSU.DRAPerformCluster();
	}
}


void cSystem::DRAGroupSizeBasedTDM() {

}


void cSystem::DRABasedOnP13() {

}

void cSystem::DRABasedOnP23() {

}

void cSystem::DRABasedOnP123() {
	int relativeTTI = g_TTI%gc_DRA_NTTI;
	cout << "relativeTTI: " << relativeTTI << endl;
	for (cRSU &_RSU : m_VecRSU) {//��������RSU
		int clusterIdx = _RSU.getDRAClusterIdx();
		cout << "clusterIdx: " << clusterIdx << endl;
		vector<int> curAvaliableRB;//��ǰTTI
		for (int i = 0; i < gc_DRA_FBNum; i++)
			if (g_TTI <= _RSU.m_DRA_RBIsAvailable[clusterIdx][i]) curAvaliableRB.push_back(i); //������ռ�õ�RB��Ŵ���
																										 //srand((unsigned)time(NULL));//iomanip
		for (int UEId : _RSU.m_CallList[clusterIdx]) {//�����ô��ں��������е��û�
													  //Ϊ��ǰ�û��ڿ��õ�RB�������ѡ��һ��
			int RBId = m_VecVUE[UEId].RBSelectBasedOnP2(curAvaliableRB);//ÿ���û��������ѡ�����RB��
			int occupiedTTI = m_VecVUE[UEId].m_Message.DRA_ONTTI;//��ȡ��ǰ�û���Ҫ�������Ϣռ�õ�ʱ϶(Occupy TTI)

															//���㵱ǰ��Ϣ��ռ����Դ����ͷ�ʱ��,��д��m_DRA_RBIsAvailable
			int remainTTI = get<1>(_RSU.m_DRAClusterTTI[clusterIdx]) - relativeTTI;//��ǰһ�ַ����иô�ʣ��Ŀɷ���ʱ϶
			int overTTI = occupiedTTI - remainTTI;//��Ҫ����һ�֣����¼��ֽ��д����ʱ϶����
			if (overTTI <= 0)
				_RSU.m_DRA_RBIsAvailable[clusterIdx][RBId] = max(g_TTI + occupiedTTI, _RSU.m_DRA_RBIsAvailable[clusterIdx][RBId]);
			else
				_RSU.m_DRA_RBIsAvailable[clusterIdx][RBId] = max(g_TTI + remainTTI + overTTI / get<2>(_RSU.m_DRAClusterTTI[clusterIdx]) * gc_DRA_NTTI + overTTI%get<2>(_RSU.m_DRAClusterTTI[clusterIdx]), _RSU.m_DRA_RBIsAvailable[clusterIdx][RBId]);


			//д�������Ϣ
			_RSU.m_DRAScheduleList[clusterIdx][RBId] = sDRAScheduleInfo(UEId, _RSU.m_DRAClusterTTI[clusterIdx], occupiedTTI);
			
		}
		_RSU.writeDRAScheduleInfo();
	}
}



