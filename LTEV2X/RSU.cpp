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
	m_DRA_ETI = vector<int>(m_ClusterNum, 1);//ÿ�������ٷ���һ����С��DRA_MTI
	const int remainTotalNTI = mc_DRA_NTI - m_DRA_ETI.size();//ʣ�¿ɷ����ʱ϶
	std::cout << remainTotalNTI << std::endl;
	int remainNTI = remainTotalNTI;//ʣ�µ�ʱ����Դ����Ĵ�С���з���

	//����ֱ�Ӹ�ÿ���ط�����һ��MTIʱ϶�����ÿ���صĴ�С��Ҫ���е�������Ҫ��ȥ��һ��������Ӧ��ʱ϶����double��
	std::vector<double> size(m_ClusterNum, 0);
	double temSize = 1 / static_cast<double>(mc_DRA_NTI)*static_cast<double>(m_VecVUE.size());
	for (int i = 0; i < m_ClusterNum; i++) {
		size[i] = static_cast<double>(m_Cluster[i].size()) - temSize;
	}

	//��Ӧ�ģ��ܵĳ�����ҲҪ�ı䣬Ҫ�۳���m_ClusterNum��ʱ϶����Ӧ�ĳ�����
	double totalUENum = static_cast<double>(m_VecVUE.size())- static_cast<double>(m_ClusterNum)*temSize;

	for (int i = 0; i < m_Cluster.size() - 1; i++) {
		if (size[i] <= 0) continue;//����ǰ�س���Ϊ������˵������1��ʱ϶�Ѿ��㹻��
		int tem= static_cast<int>(size[i] / totalUENum*remainTotalNTI + 0.5f);
		m_DRA_ETI[i] += tem;
		remainNTI -= tem;
		if (remainNTI < 0) {
			throw Exp("cRSU::DRAPerformCluster()");
		}
	}
	m_DRA_ETI[m_DRA_ETI.size() - 1] += remainNTI;
	for (int i = 1; i < m_DRA_ETI.size(); i++)
		m_DRA_ETI[i] += m_DRA_ETI[i - 1];
}

int cRSU::getDRAClusterIdx() {
	int relativeCNTI = m_DRA_CNTI%mc_DRA_NTI;

	return -1;
}


