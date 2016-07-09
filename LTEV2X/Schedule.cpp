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
	DRAbuildCallList();

	/*��ǰg_TTI��DRA�㷨*/
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
	for (cRSU &_RSU : m_VecRSU) {
		cout << _RSU.m_VecVUE.size() << endl;
		for (vector<int> &v : _RSU.m_DRACallList)
			v.clear();
		for (vector<int> &v : _RSU.m_DRAVecCluster)
			v.clear();
	}
}

void cSystem::DRAbuildCallList() {
	for (cRSU &_RSU : m_VecRSU) {
		//����m_VecRSU����m_CallList
		for (int clusterIdx = 0; clusterIdx < _RSU.m_DRAVecCluster.size(); ++clusterIdx) {
			_RSU.m_DRACallList[clusterIdx].clear();
			for (int UEId : _RSU.m_DRAVecCluster[clusterIdx]) {
				if (m_VecVUE[UEId].m_isHavingDataToTransmit) //������������Ҫ����������ӵ�m_CallList����
					_RSU.m_DRACallList[clusterIdx].push_back(UEId);
			}
		}
	}


	//-----------------------TEST-----------------------

	for (cRSU& _RSU : m_VecRSU) {
		cout << "RSU: " << _RSU.m_RSUId << " 's ClusterCallListSize  :";
		for (auto c : _RSU.m_DRAVecCluster)
			cout << c.size() << " , ";
		cout << endl;
	}
	//-----------------------TEST-----------------------
}


void cSystem::DRAPerformCluster() {
	for (cRSU& _RSU : m_VecRSU)
		_RSU.DRAPerformCluster();

	//-----------------------TEST-----------------------

	for (cRSU& _RSU : m_VecRSU) {
		cout << "RSU: " << _RSU.m_RSUId << " 's ClusterSize  :";
		for (auto c : _RSU.m_DRAVecCluster)
			cout << c.size() << " , ";
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
	int relativeTTI = g_TTI%gc_DRA_NTTI;
	for (cRSU &_RSU : m_VecRSU) {//��������RSU
		int clusterIdx = _RSU.DRAGetClusterIdx();
		vector<int> curAvaliableRB;//��ǰTTI
		for (int i = 0; i < gc_DRA_FBNum; i++)
			if (g_TTI > _RSU.m_DRA_RBIsAvailable[clusterIdx][i]) curAvaliableRB.push_back(i); //������ռ�õ�RB��Ŵ���
																										 //srand((unsigned)time(NULL));//iomanip
		for (int UEId : _RSU.m_DRACallList[clusterIdx]) {//�����ô��ں��������е��û�
													  //Ϊ��ǰ�û��ڿ��õ�RB�������ѡ��һ��
			int FBId = m_VecVUE[UEId].RBSelectBasedOnP2(curAvaliableRB);//ÿ���û��������ѡ�����FB��
			int occupiedTTI = m_VecVUE[UEId].m_Message.DRA_ONTTI;//��ȡ��ǰ�û���Ҫ�������Ϣռ�õ�ʱ϶(Occupy TTI)


			//���㵱ǰ��Ϣ��ռ����Դ����ͷ�ʱ��,��д��m_DRA_RBIsAvailable
			int begin =get<0>(_RSU.m_DRAClusterTDRInfo[clusterIdx]),
				end = get<1>(_RSU.m_DRAClusterTDRInfo[clusterIdx]),
				len = get<2>(_RSU.m_DRAClusterTDRInfo[clusterIdx]);
			int nextTurnBeginTTI = g_TTI - relativeTTI + gc_DRA_NTTI;//��RSU��һ�ֵ��ȵ���ʼTTI����һ���صĿ�ʼʱ�̣�
			int remainTTI = end - relativeTTI+1;//��ǰһ�ַ����иô�ʣ��Ŀɷ���ʱ϶
			int overTTI = occupiedTTI - remainTTI;//��Ҫ����һ�֣����¼��ֽ��д����ʱ϶����
			if (overTTI <= 0)
				_RSU.m_DRA_RBIsAvailable[clusterIdx][FBId] = max(g_TTI + occupiedTTI-1, _RSU.m_DRA_RBIsAvailable[clusterIdx][FBId]);
			else {
				int n = overTTI / len;
				if(overTTI%len==0) _RSU.m_DRA_RBIsAvailable[clusterIdx][FBId] = max(nextTurnBeginTTI + end + (n-1)*gc_DRA_NTTI, _RSU.m_DRA_RBIsAvailable[clusterIdx][FBId]);
				else _RSU.m_DRA_RBIsAvailable[clusterIdx][FBId] = max(nextTurnBeginTTI + begin + n*gc_DRA_NTTI + overTTI%len - 1, _RSU.m_DRA_RBIsAvailable[clusterIdx][FBId]);
			}

			//д�������Ϣ
			_RSU.m_DRAScheduleList[clusterIdx][FBId].push_back(sDRAScheduleInfo(UEId, _RSU.m_DRAClusterTDRInfo[clusterIdx], occupiedTTI));
			
		}
		_RSU.DRAWriteScheduleInfo(g_OutDRAScheduleInfo);
	}
}



void cSystem::DRAConflictListener() {
	for (cRSU &_RSU : m_VecRSU) {

	}
}
