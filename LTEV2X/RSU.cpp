#include<vector>
#include<math.h>
#include<iostream>
#include<utility>
#include<tuple>
#include"RSU.h"
#include"Exception.h"

using namespace std;


void cRSU::DRAPerformCluster() {
	testCluster();
}

/*�÷�������ҪĿ����Ϊ�˸���"cRSU.m_DRAClusterTDRInfo"*/
void cRSU::DRAGroupSizeBasedTDM() {
	/*�ٶ�ÿ����������һ���������ÿ�������ٷ���һ��TTIʱ϶*/
	m_DRAClusterTDRInfo = vector<tuple<int, int, int>>(m_DRAClusterNum, tuple<int, int, int>(0, 0, 1));
	/*����ÿ���ط����һ��ʱ϶�⣬ʣ��ɷ����ʱ϶����*/
	int remainNTI = gc_DRA_NTTI - m_DRAClusterNum;

	/*clusterSize�洢ÿ���صĳ�����Ŀ(double����),�������ڽ������ĵ����У���������Ŀ�Ǹ�������������*/
	std::vector<double> clusterSize(m_DRAClusterNum, 0);

	/*����ÿ��TTIʱ϶��Ӧ�ĳ�����Ŀ(double)��!!!������!!��*/
	double VUESizePerTTI = 1 / static_cast<double>(gc_DRA_NTTI)*static_cast<double>(m_VecVUE.size());

	/*����ֱ�Ӹ�ÿ���ط�����һ��TTIʱ϶�����ÿ���صĴ�С��Ҫ���е�������Ҫ��ȥVUESizePerMTI*/
	for (int i = 0; i < m_DRAClusterNum; i++) 
		clusterSize[i] = static_cast<double>(m_DRAVecCluster[i].size()) - VUESizePerTTI;
	

	/*����ʣ�µ���Դ�飬ѭ������һʱ϶�������ǰ������ߵĴأ�����֮�󣬸��Ķ�Ӧ�ı�������ȥ��ʱ϶��Ӧ�ĳ�������*/
	while (remainNTI > 0) {
		int dex = getMaxIndex(clusterSize);
		if (dex == -1) throw Exp("������û�з����ʱ����Դ������ÿ�����ڵĳ����Ѿ�Ϊ����");
		get<2>(m_DRAClusterTDRInfo[dex])++;
		remainNTI--;
		clusterSize[dex] -= VUESizePerTTI;
	}

	for (int i = 0;i < m_DRAClusterNum;i++)
		get<1>(m_DRAClusterTDRInfo[i]) = get<2>(m_DRAClusterTDRInfo[i]);

	get<1>(m_DRAClusterTDRInfo[0])--;//ʹ���䷶Χ��0��ʼ

	for (int i = 1; i < m_DRAClusterTDRInfo.size(); i++) {
		get<1>(m_DRAClusterTDRInfo[i]) += get<1>(m_DRAClusterTDRInfo[i - 1]);
		get<0>(m_DRAClusterTDRInfo[i]) = get<1>(m_DRAClusterTDRInfo[i]) - get<2>(m_DRAClusterTDRInfo[i]) + 1;
	}

	get<0>(m_DRAClusterTDRInfo[0]) = get<1>(m_DRAClusterTDRInfo[0]) - get<2>(m_DRAClusterTDRInfo[0]) + 1;
}

int cRSU::DRAGetClusterIdx() {
	int relativeCNTI = g_TTI%gc_DRA_NTTI;
	for (int i = 0; i < m_DRAClusterNum; i++)
		if (relativeCNTI <= get<1>(m_DRAClusterTDRInfo[i])) return i;
	return -1;
}


int cRSU::getMaxIndex(const std::vector<double>&v) {
	double max = 0;
	int dex = -1;
	for (int i = 0;i < v.size();i++) {
		if (v[i] > max) {
			dex = i;
			max = v[i];
		}
	}
	return dex;
}



void cRSU::DRAWriteScheduleInfo(std::ofstream& out) {
	out << "RSU: " << m_RSUId << endl;
	for (int clusterIdx = 0; clusterIdx < m_DRAClusterNum; clusterIdx++) {
		out << "  Cluster: " << clusterIdx << endl;
		for (int FBIdx = 0; FBIdx < gc_DRA_FBNum; FBIdx++) {
			out << "    FB: " << FBIdx << endl;
			out << "      Released TTI: " << m_DRA_RBIsAvailable[clusterIdx][FBIdx]<<endl;
			int cnt = 0;
			for (sDRAScheduleInfo & info : m_DRAScheduleList[clusterIdx][FBIdx]) {
				if (cnt++ == 0) out << "      ========================================" << endl;
				else out << "      ----------------------------------------" << endl;
				out << "      VEId: " << info.VEId << endl;
				out << "      OccupiedTTI: ";
				for (tuple<int, int> t : info.occupiedInterval)
					out << "[" << get<0>(t) << " , " << get<1>(t) << "] , ";
				out << endl;
			}
		}
	}
}


void cRSU::DRAConflictListener() {

	for (int clusterIdx = 0;clusterIdx < m_DRAClusterNum;clusterIdx++) {
		//-----------------------TEST-----------------------
		/*���ÿ���ص��б�Ϊ�գ�˵����һ�εĳ�ͻ��δ������ϣ�˵��������Ҫ����*/
		if (m_DRAConflictList[clusterIdx].size() != 0) throw Exp("cRSU::DRAConflictListener()");
		//-----------------------TEST-----------------------
		for (int FBIdx = 0;FBIdx < gc_DRA_FBNum;FBIdx++) {
			list<sDRAScheduleInfo> &list = m_DRAScheduleList[clusterIdx][FBIdx];
			if (list.size() > 1) {
				for (sDRAScheduleInfo &info : list) {
					m_DRAConflictList[clusterIdx].push_back(info.VEId);
				}
			}
		}
		
	}
}



