#include<vector>
#include<math.h>
#include<iostream>
#include<utility>
#include<tuple>
#include"RSU.h"
#include"Exception.h"

using namespace std;


void cRSU::DRAPerformCluster() {
	/*
	�Ը�RSU��Χ�ڵ�UE���зִأ���õ�m_Cluster
	*/

	//����m_DRA_ETI����
	m_DRAClusterTTI = vector<tuple<int, int, int>>(m_ClusterNum,tuple<int, int, int>(0,0,1));//ÿ�������ٷ���һ����С��DRA_MTI
	int remainNTI = gc_DRA_NTTI - m_ClusterNum;//Ȼ���ʣ�µ�ʱ����Դѭ�����з���
	
	//clusterSize�洢ÿ���صĳ�����Ŀ(double����)
	std::vector<double> clusterSize(m_ClusterNum, 0);

	//VUESizePerMTIΪ1��MTIʱ϶����Ӧ�ĳ�����Ŀ(double)
	double VUESizePerMTI = 1 / static_cast<double>(gc_DRA_NTTI)*static_cast<double>(m_VecVUE.size());
	
	//����ֱ�Ӹ�ÿ���ط�����һ��MTIʱ϶�����ÿ���صĴ�С��Ҫ���е�������Ҫ��ȥVUESizePerMTI
	for (int i = 0; i < m_ClusterNum; i++) {
		clusterSize[i] = static_cast<double>(m_Cluster[i].size()) - VUESizePerMTI;
	}

	//����ʣ�µ���Դ�飬ѭ���������ǰ������ߵĴأ�����֮�󣬸��Ķ�Ӧ�ı���
	while (remainNTI > 0) {
		int dex = getMaxIndex(clusterSize);
		if (dex == -1) throw Exp("������û�з����ʱ����Դ������ÿ�����ڵĳ����Ѿ�Ϊ����");
		get<2>(m_DRAClusterTTI[dex])++;
		remainNTI--;
		clusterSize[dex] -= VUESizePerMTI;
	}

	for (int i = 0;i < m_ClusterNum;i++)
		get<1>(m_DRAClusterTTI[i]) = get<2>(m_DRAClusterTTI[i]);
	get<1>(m_DRAClusterTTI[0])--;//ʹ���䷶Χ��0��ʼ

	for (int i = 1; i < m_DRAClusterTTI.size(); i++) {
		get<1>(m_DRAClusterTTI[i]) += get<1>(m_DRAClusterTTI[i - 1]);
		get<0>(m_DRAClusterTTI[i]) = get<1>(m_DRAClusterTTI[i]) - get<2>(m_DRAClusterTTI[i]) + 1;
	}
	get<0>(m_DRAClusterTTI[0]) = get<1>(m_DRAClusterTTI[0]) - get<2>(m_DRAClusterTTI[0]) + 1;


}

int cRSU::getDRAClusterIdx() {
	int relativeCNTI = g_TTI%gc_DRA_NTTI;
	for (int i = 0; i < m_ClusterNum; i++)
		if (relativeCNTI < get<1>(m_DRAClusterTTI[i])) return i;
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




