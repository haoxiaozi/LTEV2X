#include<vector>
#include<math.h>
#include<iostream>
#include"RSU.h"
#include"Exception.h"

using namespace std;


void cRSU::DRAPerformCluster() {
	/*
	�Ը�RSU��Χ�ڵ�UE���зִأ���õ�m_Cluster
	*/

	//����m_DRA_ETI����
	m_DRAClusterENTI = vector<int>(m_ClusterNum, 1);//ÿ�������ٷ���һ����С��DRA_MTI
	int remainNTI = mc_DRA_NTI - m_ClusterNum;//Ȼ���ʣ�µ�ʱ����Դѭ�����з���
	
	//clusterSize�洢ÿ���صĳ�����Ŀ(double����)
	std::vector<double> clusterSize(m_ClusterNum, 0);

	//VUESizePerMTIΪ1��MTIʱ϶����Ӧ�ĳ�����Ŀ(double)
	double VUESizePerMTI = 1 / static_cast<double>(mc_DRA_NTI)*static_cast<double>(m_VecVUE.size());
	
	//����ֱ�Ӹ�ÿ���ط�����һ��MTIʱ϶�����ÿ���صĴ�С��Ҫ���е�������Ҫ��ȥVUESizePerMTI
	for (int i = 0; i < m_ClusterNum; i++) {
		clusterSize[i] = static_cast<double>(m_Cluster[i].size()) - VUESizePerMTI;
	}

	//����ʣ�µ���Դ�飬ѭ���������ǰ������ߵĴأ�����֮�󣬸��Ķ�Ӧ�ı���
	while (remainNTI > 0) {
		int dex = getMaxIndex(clusterSize);
		if (dex == -1) throw Exp("������û�з����ʱ����Դ������ÿ�����ڵĳ����Ѿ�Ϊ����");
		m_DRAClusterENTI[dex]++;
		remainNTI--;
		clusterSize[dex] -= VUESizePerMTI;
	}

	m_DRAClusterNTI = m_DRAClusterENTI;//����ÿ���ط��䵽��ʱ϶����

	for (int i = 1; i < m_DRAClusterENTI.size(); i++)
		m_DRAClusterENTI[i] += m_DRAClusterENTI[i - 1];
}

int cRSU::getDRAClusterIdx() {
	int relativeCNTI = m_DRA_CNTI%mc_DRA_NTI;
	for (int i = 0; i < m_ClusterNum; i++)
		if (relativeCNTI < m_DRAClusterENTI[i]) return i;
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




