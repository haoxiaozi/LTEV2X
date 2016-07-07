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
	for (ceNB _eNB : m_VeceNB) {
		if (m_Config.linkType == UPLINK)
			_eNB.m_vecScheduleInfo.clear();
	}
}


void cSystem::schedulePF_RP_CSI_UL() {
	for (ceNB& _eNB : m_VeceNB) {//��ÿһ����վ����һ�ε���
		int k = _eNB.m_VecRSU.size();
		vector<vector<bool>> SPU(k, vector<bool>(gc_SubbandNum));//ÿ��RSU��һ��vector<bool>��������SPU��true������ѡ����Ӵ�
		vector<int> S;//δ������Ӵ�����(�洢�Ӵ���ID��
		
		//��ʼ���Ӵ�����S��������Ҫ����Ҫ��ÿ�ε���ǰӦ��û���Ӵ���ռ�ðɣ�
		for (int subbandId = 0; subbandId < gc_SubbandNum; subbandId++) 
			if (_eNB.m_UnassignedSubband)S.push_back(subbandId);
		

		//����ÿ��RSU��Ӧ��ͬ�Ӵ���PF����
		vector<sPFInfo> F;//�洢PF���ӵ�����
		for (int RSUId : _eNB.m_VecRSU) {
			for (int subbandId = 0; subbandId < gc_SubbandNum; subbandId++) {
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

			if (SPU[u].size() == 0 || (v>0 && SPU[u][v - 1] || v < gc_SubbandNum - 1 && SPU[u][v + 1])) {
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
			for (int subbandId = 0; subbandId < gc_SubbandNum; subbandId++) {
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









//void System::schedule_MCS() {
//	int subframe_cata;
//	int _MCS_Restriction;
//	double lp_abs_bias;
//
//	for (int sectorIdx = 0; sectorIdx != config.sectorNum; sectorIdx++) {
//		_MCS_Restriction = 30;
//		lp_abs_bias = 0;
//		if (config.linkType == DOWNLINK) {
//				list<int>::const_iterator it;
//				for (it = vecSector[sectorIdx].admitUserIdList_DL.begin(); it != vecSector[sectorIdx].admitUserIdList_DL.end(); ++it) {
//					int usr = *it;
//					int sched_wide = 0;
//					vector<double> sinr_codeword(g_c_MaxCodewords, 0.0);
//					vector<double> sinr_unq_codeword(g_c_MaxCodewords, 0.0);
//					for (int hIdx = 0; hIdx < config._H_Num; hIdx++) {
//
//						for (int i = 0; i < vecSector[sectorIdx].scheduleInfoWrite_DL[hIdx].userNum; i++) {
//							if (vecSector[sectorIdx].scheduleInfoWrite_DL[hIdx].userId[i] == usr) {
//								for (int codewords = 0; codewords != vecVUE[usr].feedback_eNB.codewords; codewords++) {
//									int subband_idx = _H_2_Subband(hIdx);
//									sinr_codeword[codewords] += positiveNumber_2_dB(vecVUE[usr]._CQI_PredictRealistic[subband_idx + codewords*config.subbandNum]);
//								}
//								sched_wide++;
//							}
//						}
//					}
//					if (sched_wide != 0) {
//						for (int codewords = 0; codewords != vecVUE[usr].feedback_eNB.codewords; codewords++) {
//							int mcs_idx;
//							int mcs;
//							vecVUE[usr].codesword_sinr_mcs[codewords] = _dB_PositiveNumber(sinr_codeword[codewords] / sched_wide + lp_abs_bias);
//							vecVUE[usr].codesword_unq_sinr_mcs[codewords] = _dB_PositiveNumber(sinr_unq_codeword[codewords] / sched_wide + lp_abs_bias);
//							mcs_idx = g_LUT_Search_MCS_Index(DOWNLINK, vecVUE[usr].codesword_sinr_mcs[codewords], BLER_TARGET);
//
//
//							mcs_idx = CHOOSE_MCS(mcs_idx);
//							mcs = MIN(mcs_idx, _MCS_Restriction);
//							if (mcs <0)
//								mcs = 0;
//							else if (mcs>DOWNLINK_MCS_LEVELS - 1)
//								mcs = DOWNLINK_MCS_LEVELS - 1;
//							vecVUE[usr].MCS[codewords] = mcs;
//						}
//					}
//				}
//			}
//	}
//}








void cSystem::DRASchedule() {

	DRAInformationClean();//��Դ������Ϣ���

	DRAbuildCallList();//������������


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
}


void cSystem::DRAGroupSizeBasedTDM() {

}


void cSystem::DRABasedOnP13() {

}

void cSystem::DRABasedOnP23() {

}

void cSystem::DRABasedOnP123() {
	for (cRSU &_RSU : m_VecRSU) {
		for (int i = 0; i < _RSU.mc_DRA_NTI; i++) {//���α���ÿһ��DRAʱ϶
			int clusterIdx = _RSU.getDRAClusterIdx();


			_RSU.m_DRA_CNTI++;//���¸�RSU��ǰ��DRAʱ��
		}
	}
}

