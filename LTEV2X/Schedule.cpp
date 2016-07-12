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
#include<string>
#include<tuple>
#include<fstream>
#include"Schedule.h"
#include"System.h"
#include"Exception.h"
#include"Definition.h"
#include"Global.h"

using namespace std;

void cSystem::centralizedSchedule() {
	//�����һ�ε�����Ϣ
	scheduleInfoClean();

	schedulePF_RP_CSI_UL();
	
}


void cSystem::scheduleInfoClean() {
	for (ceNB &_eNB : m_eNBVec) 
		_eNB.m_vecScheduleInfo.clear();
	
}


void cSystem::schedulePF_RP_CSI_UL() {
	for (ceNB &_eNB : m_eNBVec) {//��ÿһ����վ����һ�ε���
		int k = _eNB.m_RSUIdList.size();
		vector<vector<bool>> SPU(k, vector<bool>(gc_RBNum));//ÿ��RSU��һ��vector<bool>��������SPU��true������ѡ����Ӵ�
		vector<int> S;//δ������Ӵ�����(�洢�Ӵ���ID��
		
		//��ʼ���Ӵ�����S��������Ҫ����Ҫ��ÿ�ε���ǰӦ��û���Ӵ���ռ�ðɣ�
		for (int subbandId = 0; subbandId < gc_RBNum; subbandId++) 
			if (_eNB.m_UnassignedSubband)S.push_back(subbandId);
		

		//����ÿ��RSU��Ӧ��ͬ�Ӵ���PF����
		vector<sPFInfo> F;//�洢PF���ӵ�����
		for (int RSUId : _eNB.m_RSUIdList) {
			for (int subbandId = 0; subbandId < gc_RBNum; subbandId++) {
				if (_eNB.m_UnassignedSubband[subbandId] == false) continue;//���Ӵ��ѱ�����
				double t_FactorPF= log10(1 + m_RSUVec[RSUId].m_SINR[subbandId]) / m_RSUVec[RSUId].m_AccumulateThroughput;
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
	//-----------------------TEST-----------------------
	g_OutDRAScheduleInfo << "ATTI = " << left << setw(6) << m_ATTI << "RTTI = " << left << setw(6) << m_ATTI - m_STTI << endl;
	g_OutDRAScheduleInfo << "{" << endl;
	g_OutDRAProcessInfo << "ATTI = " << left << setw(6) << m_ATTI << "RTTI = " << left << setw(6) << m_ATTI - m_STTI << endl;
	g_OutDRAProcessInfo << "{" << endl;
	//-----------------------TEST-----------------------


	bool clusterFlag = (m_ATTI - m_STTI) % m_Config.locationUpdateNTTI == 0;

	/*��Դ������Ϣ���:����ÿ��RSU�ڵ�m_CallList��*/
	DRAInformationClean();

	/*���ݵ���λ�ý��зִ�*/
	DRAPerformCluster(clusterFlag);

	/*���ݴش�С����ʱ����Դ�Ļ���*/
	DRAGroupSizeBasedTDM(clusterFlag);

	/*����������������RSU�ڵ�m_VecVUE������m_CallList*/
	DRAUpdateAdmissionList();

	/*-----------------------WARN-----------------------
	* ���m_DRA_RSUSwitchEventIdList��Ϊ�գ�˵��������Ҫ����
	-----------------------WARN-----------------------*/
	if (m_DRA_RSUSwitchEventIdList.size() != 0) throw Exp("��������ȫ�����ɺ�System������л������Բ�Ϊ�գ�");
	
	g_OutDRAProcessInfo << "    �������¼������ȴ������Լ��л������Ľ�������" << endl;
	for (cRSU &_RSU : m_RSUVec) {
		_RSU.DRAWriteProcessInfo(g_OutDRAProcessInfo, 0);//д������������Ϣ
	}
	g_OutDRAProcessInfo << endl;



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



	//-----------------------TEST-----------------------
	g_OutDRAScheduleInfo << "}" << endl;
	g_OutDRAScheduleInfo << "\n\n" << endl;
	g_OutDRAProcessInfo << "}" << endl;
	g_OutDRAProcessInfo << "\n\n" << endl;
	//-----------------------TEST-----------------------
}


void cSystem::DRAInformationClean() {
	for (cRSU &_RSU : m_RSUVec)
		_RSU.DRAInformationClean();
}


void cSystem::DRAPerformCluster(bool clusterFlag) {
	if (!clusterFlag)return;

	/*�����һ�εķִ���Ϣ*/
	for (ceNB &_eNB : m_eNBVec) {
		_eNB.m_VeUEIdList.clear();
	}
	for (cRSU &_RSU : m_RSUVec) {
		_RSU.m_VeUEIdList.clear();
	}


	/*�����VeUE�����RSU*/
	for (int VeUEId = 0;VeUEId < m_Config.VUENum;VeUEId++) {
		int RSUId = rand() % m_Config.RSUNum;
		m_RSUVec[RSUId].m_VeUEIdList.push_back(VeUEId);
		m_VeUEVec[VeUEId].m_RSUId = RSUId;
	}

	/*��RSU�ڵ�VeUE����ָ���*/
	for (cRSU &_RSU : m_RSUVec) {
		//��������ϴηִصļ���
		for (int clusterIdx = 0;clusterIdx < _RSU.m_DRAClusterNum;clusterIdx++)
			_RSU.m_DRAClusterVeUEIdList[clusterIdx].clear();
		//������RSU�ڵ�VeUE������벻ͬ�Ĵ�
		for (int VeUEId : _RSU.m_VeUEIdList) {
			int clusterIdx = rand() % _RSU.m_DRAClusterNum;
			_RSU.m_DRAClusterVeUEIdList[clusterIdx].push_back(VeUEId);
			m_VeUEVec[VeUEId].m_ClusterIdx = clusterIdx;
		}
	}

	//���»�վ��VeUE����
	for (ceNB &_eNB:m_eNBVec) {
		for (int RSUId : _eNB.m_RSUIdList) {
			for (int VeUEId : m_RSUVec[RSUId].m_VeUEIdList) {
				_eNB.m_VeUEIdList.push_back(VeUEId);
			}
		}
	}

}








void cSystem::DRAGroupSizeBasedTDM(bool clusterFlag) {
	if (!clusterFlag)return;
	for (cRSU& _RSU : m_RSUVec) 
		_RSU.DRAGroupSizeBasedTDM();

	writeClusterPerformInfo(g_OutClasterPerformInfo);
}


void cSystem::DRAUpdateAdmissionList() {
	/*���ȣ�����System������¼���������*/
	for (cRSU &_RSU : m_RSUVec)
		_RSU.DRAProcessSystemLevelEventList(m_ATTI, m_STTI, m_VeUEVec, m_EventVec,m_EventTTIList);

	/*��Σ�������ȱ����������ǣ����ݷ�����֮ǰ�뿪�˵�ǰ��RSU������Ҫ�����¼����͵�System�����RSU�л�������*/
	for (cRSU &_RSU : m_RSUVec)
		_RSU.DRAProcessRSULevelScheduleInfoTable(m_ATTI, m_VeUEVec, m_EventVec, m_DRA_RSUSwitchEventIdList);

	/*Ȼ�󣬴���RSU����ĵȴ�����*/
	for (cRSU &_RSU : m_RSUVec)
		_RSU.DRAProcessRSULevelWaitingVeUEIdList(m_ATTI, m_VeUEVec, m_EventVec, m_DRA_RSUSwitchEventIdList);


	/*��󣬴���System�����RSU�л�����*/
	for (cRSU &_RSU : m_RSUVec)
		_RSU.DRAProcessSystemLevelRSUSwitchList(m_ATTI, m_VeUEVec, m_EventVec, m_DRA_RSUSwitchEventIdList);


	/*ע�⣬�����ٴδ���һ��ȴ�������ΪRSU�л�����Ὣ�л����¼�ѹ��ȴ��������߽�������*/
	for (cRSU &_RSU : m_RSUVec)
		_RSU.DRAProcessRSULevelWaitingVeUEIdList(m_ATTI, m_VeUEVec, m_EventVec, m_DRA_RSUSwitchEventIdList);
}

void cSystem::DRASelectBasedOnP13() {

}

void cSystem::DRASelectBasedOnP23() {

}

void cSystem::DRASelectBasedOnP123() {
	for (cRSU &_RSU : m_RSUVec)
		_RSU.DRASelectBasedOnP123(m_ATTI,m_VeUEVec, m_EventVec);
}




void cSystem::DRAConflictListener() {
	for (cRSU &_RSU : m_RSUVec) {
		_RSU.DRAConflictListener(m_ATTI);
	}

	g_OutDRAProcessInfo << "    �������ͻ���ĵȴ�����" << endl;
	for (cRSU &_RSU : m_RSUVec) {
		_RSU.DRAWriteProcessInfo(g_OutDRAProcessInfo, 1);
	}
	g_OutDRAProcessInfo << endl;
}
