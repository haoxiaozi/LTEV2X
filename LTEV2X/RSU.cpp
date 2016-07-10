#include<vector>
#include<math.h>
#include<iostream>
#include<utility>
#include<tuple>
#include"RSU.h"
#include"Exception.h"

using namespace std;

extern ofstream g_OutDRAScheduleInfo;

int cRSU::DRAGetClusterIdx(int TTI) {
	int relativeCNTI = TTI%gc_DRA_NTTI;
	for (int i = 0; i < m_DRAClusterNum; i++)
		if (relativeCNTI <= get<1>(m_DRAClusterTDRInfo[i])) return i;
	return -1;
}


int cRSU::getMaxIndex(const std::vector<double>&v) {
	double max = 0;
	int dex = -1;
	for (int i = 0; i < static_cast<int>(v.size()); i++) {
		if (v[i] > max) {
			dex = i;
			max = v[i];
		}
	}
	return dex;
}


void cRSU::DRAPerformCluster() {
	testCluster();
}


void cRSU::DRAInformationClean() {
	for (int clusterIdx = 0; clusterIdx < m_DRAClusterNum; clusterIdx++) {
		m_DRACallList[clusterIdx].clear();
		m_DRAVecCluster[clusterIdx].clear();
	}
}


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

	for (int i = 1; i < m_DRAClusterNum; i++) {
		get<1>(m_DRAClusterTDRInfo[i]) += get<1>(m_DRAClusterTDRInfo[i - 1]);
		get<0>(m_DRAClusterTDRInfo[i]) = get<1>(m_DRAClusterTDRInfo[i]) - get<2>(m_DRAClusterTDRInfo[i]) + 1;
	}

	get<0>(m_DRAClusterTDRInfo[0]) = get<1>(m_DRAClusterTDRInfo[0]) - get<2>(m_DRAClusterTDRInfo[0]) + 1;
}


void cRSU::DRABuildCallList(std::vector<cVeUE>&v) {
	DRAReaddConflictListToCallList();
	for (int clusterIdx = 0; clusterIdx < m_DRAClusterNum; ++clusterIdx) {
		for (int UEId : m_DRAVecCluster[clusterIdx]) {
			if (v[UEId].m_isHavingDataToTransmit) { //������������Ҫ����������ӵ�m_CallList����
				m_DRACallList[clusterIdx].push_back(UEId);
				v[UEId].m_isHavingDataToTransmit = false;
				//-----------------------WARN-----------------------
				//���������벻ͬ�غ�m_isHavingDataToTransmit��������δ��ɣ�������һ��������δ��������Ϣ��
				//������һ������Ҫ�ڵ�ǰ�������·��͸���Ϣ
				//-----------------------WARN-----------------------
			}
		}
	}
}


void cRSU::DRAReaddConflictListToCallList() {
	for (const tuple<int, int, int> &t : m_DRAConflictList) {
		int VEId = get<0>(t);
		int clusterIdx = get<1>(t);
		int FBIdx = get<2>(t);
		//-----------------------WARN-----------------------
		//����Ҫ����ǰ�����ֵ����м侭���˷ִأ����������Ȼ�ڸô��У��Ż������ӣ�������δ����
		//-----------------------WARN-----------------------
		m_DRACallList[clusterIdx].push_back(VEId);
	}
	/*��һTTI�ĳ�ͻ��Ϣ����������Խ������*/
	m_DRAConflictList.clear();
}


void cRSU::DRASelectBasedOnP13(int TTI, std::vector<cVeUE>&v) {
}

void cRSU::DRASelectBasedOnP23(int TTI, std::vector<cVeUE>&v) {
}

void cRSU::DRASelectBasedOnP123(int TTI, std::vector<cVeUE>&v) {
	int relativeTTI = TTI%gc_DRA_NTTI;

	int clusterIdx = DRAGetClusterIdx(TTI);
	vector<int> curAvaliableFB;//��ǰTTI���õ�Ƶ���


	for (int i = 0; i < gc_DRA_FBNum; i++)
		if (TTI > m_DRA_RBIsAvailable[clusterIdx][i]) curAvaliableFB.push_back(i); //������ռ�õ�RB��Ŵ���

	for (int UEId : m_DRACallList[clusterIdx]) {//�����ô��ں��������е��û�
													 //Ϊ��ǰ�û��ڿ��õ�RB�������ѡ��һ��
		int FBId = v[UEId].RBSelectBasedOnP2(curAvaliableFB);//ÿ���û��������ѡ�����FB��
		int occupiedTTI = v[UEId].m_Message.DRA_ONTTI;//��ȡ��ǰ�û���Ҫ�������Ϣռ�õ�ʱ϶(Occupy TTI)


															 //���㵱ǰ��Ϣ��ռ����Դ����ͷ�ʱ��,��д��m_DRA_RBIsAvailable
		int begin = get<0>(m_DRAClusterTDRInfo[clusterIdx]),
			end = get<1>(m_DRAClusterTDRInfo[clusterIdx]),
			len = get<2>(m_DRAClusterTDRInfo[clusterIdx]);
		int nextTurnBeginTTI = TTI - relativeTTI + gc_DRA_NTTI;//��RSU��һ�ֵ��ȵ���ʼTTI����һ���صĿ�ʼʱ�̣�
		int remainTTI = end - relativeTTI + 1;//��ǰһ�ַ����иô�ʣ��Ŀɷ���ʱ϶
		int overTTI = occupiedTTI - remainTTI;//��Ҫ����һ�֣����¼��ֽ��д����ʱ϶����
		if (overTTI <= 0)
			m_DRA_RBIsAvailable[clusterIdx][FBId] = max(TTI + occupiedTTI - 1, m_DRA_RBIsAvailable[clusterIdx][FBId]);
		else {
			int n = overTTI / len;
			if (overTTI%len == 0) m_DRA_RBIsAvailable[clusterIdx][FBId] = max(nextTurnBeginTTI + end + (n - 1)*gc_DRA_NTTI, m_DRA_RBIsAvailable[clusterIdx][FBId]);
			else m_DRA_RBIsAvailable[clusterIdx][FBId] = max(nextTurnBeginTTI + begin + n*gc_DRA_NTTI + overTTI%len - 1, m_DRA_RBIsAvailable[clusterIdx][FBId]);
		}

		//д�������Ϣ
		m_DRAScheduleList[clusterIdx][FBId].push_back(sDRAScheduleInfo(TTI,UEId, FBId, m_DRAClusterTDRInfo[clusterIdx], occupiedTTI));

	}
	DRAWriteScheduleInfo(g_OutDRAScheduleInfo);
}







void cRSU::DRAWriteScheduleInfo(std::ofstream& out) {
	out << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<   RSU: " << m_RSUId <<"   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>"<< endl;
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


void cRSU::DRAConflictListener(int TTI) {
	//-----------------------TEST-----------------------
	/*���ÿ���ص��б�Ϊ�գ�˵����һ�εĳ�ͻ��δ������ϣ�˵��������Ҫ����*/
	if (m_DRAConflictList.size() != 0) throw Exp("cRSU::DRAConflictListener()");
	//-----------------------TEST-----------------------
	for (int clusterIdx = 0;clusterIdx < m_DRAClusterNum;clusterIdx++) {
		for (int FBIdx = 0;FBIdx < gc_DRA_FBNum;FBIdx++) {
			list<sDRAScheduleInfo> &list = m_DRAScheduleList[clusterIdx][FBIdx];
			if (list.size() > 1) {//����һ��VE�ڵ�ǰTTI����FB�ϴ��䣬�������˳�ͻ��������ӵ���ͻ�б�ȴ����¼����������
				for (sDRAScheduleInfo &info : list) {
					m_DRAConflictList.push_back(tuple<int,int,int>(info.VEId, clusterIdx, FBIdx));
				}		
			}
			else if (list.size() == 1) {//ֻ��һ���û��ڴ��䣬���û�����ȷ�Ĵ����������ݣ����뿪��֮ǰ��
			    //-----------------------WARN-----------------------
				//û�п��ǵ��������ڴ����ź�ʱ���ִؽ��������һ���ص����
				//-----------------------WARN-----------------------

				/*�����ǰTTI==m_DRA_RBIsAvailable[clusterIdx][FBIdx]���¶�Ӧ������*/
				if (TTI == m_DRA_RBIsAvailable[clusterIdx][FBIdx])
					m_DRAScheduleList[clusterIdx][FBIdx].clear();
			}
		}
	}

	/*�����ͻ��ά��m_DRA_RBIsAvailable�Լ�m_DRAScheduleList*/
	DRAConflictSolve(TTI);
}


void cRSU::DRAConflictSolve(int TTI) {
	for (const tuple<int, int, int> t : m_DRAConflictList) {
		int clusterIdx = get<1>(t);
		int FBIdx = get<2>(t);
		m_DRAScheduleList[clusterIdx][FBIdx].clear();
		m_DRA_RBIsAvailable[clusterIdx][FBIdx] = TTI;//����FB��Դ�ڴ�ʱ�ͷ�
	}
}



