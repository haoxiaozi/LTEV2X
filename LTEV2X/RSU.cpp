#include<vector>
#include<math.h>
#include<iostream>
#include<utility>
#include<tuple>
#include<iomanip>
#include"RSU.h"
#include"Exception.h"
#include"Event.h"
#include"Global.h"

using namespace std;

int cRSU::DRAGetClusterIdx(int TTI) {
	int ATTI = TTI%gc_DRA_NTTI;
	for (int i = 0; i < m_DRAClusterNum; i++)
		if (ATTI <= get<1>(m_DRAClusterTDRInfo[i])) return i;
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




void cRSU::DRAInformationClean() {
	m_DRACallVeUEIdList.clear();
}


void cRSU::DRAGroupSizeBasedTDM() {
	/*�ٶ�ÿ����������һ���������ÿ�������ٷ���һ��TTIʱ϶*/
	m_DRAClusterTDRInfo = vector<tuple<int, int, int>>(m_DRAClusterNum, tuple<int, int, int>(0, 0, 1));
	/*����ÿ���ط����һ��ʱ϶�⣬ʣ��ɷ����ʱ϶����*/
	int remainNTI = gc_DRA_NTTI - m_DRAClusterNum;

	/*clusterSize�洢ÿ���ص�VeUE��Ŀ(double����),�������ڽ������ĵ����У�VeUE����Ŀ�Ǹ�������������*/
	std::vector<double> clusterSize(m_DRAClusterNum, 0);

	/*����ÿ��TTIʱ϶��Ӧ��VeUE��Ŀ(double)��!!!������!!��*/
	double VUESizePerTTI = 1 / static_cast<double>(gc_DRA_NTTI)*static_cast<double>(m_VeUEList.size());

	/*����ֱ�Ӹ�ÿ���ط�����һ��TTIʱ϶�����ÿ���صĴ�С��Ҫ���е�������Ҫ��ȥVUESizePerMTI*/
	for (int i = 0; i < m_DRAClusterNum; i++) 
		clusterSize[i] = static_cast<double>(m_DRAClusterVeUEIdList[i].size()) - VUESizePerTTI;
	

	/*����ʣ�µ���Դ�飬ѭ������һʱ϶�������ǰ������ߵĴأ�����֮�󣬸��Ķ�Ӧ�ı�������ȥ��ʱ϶��Ӧ��VeUE����*/
	while (remainNTI > 0) {
		int dex = getMaxIndex(clusterSize);
		if (dex == -1) throw Exp("������û�з����ʱ����Դ������ÿ�����ڵ�VeUE�Ѿ�Ϊ����");
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


void cRSU::DRAProcessSystemLevelEventList(int TTI, int STTI,const std::vector<cVeUE>& systemVeUEVec, const std::vector<std::list<sEvent>>& systemEventList) {
	int clusterIdx = DRAGetClusterIdx(TTI);//��ǰ�ɴ������ݵĴر��
	for (const sEvent& event : systemEventList[(TTI-STTI)%systemEventList.size()]) {
		int VeUEId = event.VeUEId;
		if (systemVeUEVec[VeUEId].m_RSUId != m_RSUId) {//��ǰ�¼���Ӧ��VeUE���ڵ�ǰRSU�У���������
			continue;
		}
		else {//��ǰ�¼���Ӧ��VeUE�ڵ�ǰRSU��
			if (systemVeUEVec[VeUEId].m_ClusterIdx == clusterIdx)//��ǰʱ���¼������е�VeUEǡ��λ�ڸ�RSU�ĸô��ڣ���ӵ���ǰ��������
				pushToRSULevelCallVeUEIdList(VeUEId);
			else//���򣬵�ǰ�¼��ڴ�ʱ�����������䣬Ӧת��ȴ�����
				pushToRSULevelWaitingVeUEIdList(VeUEId);
		}
	}
}


void cRSU::DRAProcessRSULevelWaitingVeUEIdList(int TTI, const std::vector<cVeUE>& systemVeUEVec, std::list<int> &systemDRA_RSUSwitchVeUEIdList) {
	int clusterIdx = DRAGetClusterIdx(TTI);//���ڵ����еĴر��
	list<int>::iterator it = m_DRAWaitingVeUEIdList.begin();
	while (it != m_DRAWaitingVeUEIdList.end()) {
		int VeUEId = *it;
		if (systemVeUEVec[VeUEId].m_RSUId != m_RSUId) {//��VeUE�Ѿ����ڸ�RSU��Χ��
			pushToSystemLevelRSUSwitchVeUEIdList(VeUEId,systemDRA_RSUSwitchVeUEIdList);//������ӵ�System������л�������
			it=m_DRAWaitingVeUEIdList.erase(it);//����ӳ�ͻ������ɾ��
		}
		else {//��Ȼ���ڵ�ǰRSU��Χ��
			if (systemVeUEVec[VeUEId].m_ClusterIdx == clusterIdx) {//��VeUE�������ڵ��ȵĴ���
				pushToRSULevelCallVeUEIdList(VeUEId);//��ӵ�RSU����ĺ���������
				it=m_DRAWaitingVeUEIdList.erase(it);//����ӳ�ͻ������ɾ��
			}
			else {//��VeUE���ڵ�ǰ�����ȵĴ���
				it++;
				continue; //�����ȴ�����ǰTTI��������
			}
		}
	}
}




void cRSU::DRAProcessSystemLevelRSUSwitchList(int TTI, const std::vector<cVeUE>& systemVeUEVec, std::list<int> &systemDRA_RSUSwitchVeUEIdList) {
	list<int>::iterator it = systemDRA_RSUSwitchVeUEIdList.begin();
	int clusterIdx = DRAGetClusterIdx(TTI);//��ǰ�ɴ������ݵĴر��
	while (it != systemDRA_RSUSwitchVeUEIdList.end()) {
		int VeUEId = *it;
		if (systemVeUEVec[VeUEId].m_RSUId != m_RSUId) {//���л������е�VeUE�������ڵ�ǰ�أ���������
			it++;
			continue;
		}
		else {////���л������е�VeUE�����ڵ�ǰ��
			if (systemVeUEVec[VeUEId].m_ClusterIdx == clusterIdx) {//���л������е�VeUEǡ��λ�ڸ�RSU�ĵ�ǰ���ڣ���ӵ���ǰ��������
				pushToRSULevelCallVeUEIdList(VeUEId);
				it=systemDRA_RSUSwitchVeUEIdList.erase(it);
			}
			else {//�����ڵ�ǰ�أ�ת��ȴ�����
				pushToRSULevelWaitingVeUEIdList(VeUEId);
				it=systemDRA_RSUSwitchVeUEIdList.erase(it);
			}
		}
	}
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

	for (int VeUEId : m_DRACallVeUEIdList) {//�����ô��ں��������е��û�
		//Ϊ��ǰ�û��ڿ��õ�RB�������ѡ��һ����ÿ���û��������ѡ�����FB��

		/*-----------------------WARN-----------------------
		* û�п��ǵ����������ԴΪ0�����
		*-----------------------WARN-----------------------*/
		int FBId = v[VeUEId].RBSelectBasedOnP2(curAvaliableFB);

		//��ȡ��ǰ�û���Ҫ�������Ϣռ�õ�ʱ϶(Occupy TTI)
		int occupiedTTI = v[VeUEId].m_Message.DRA_ONTTI;


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
		m_DRAScheduleList[clusterIdx][FBId].push_back(sDRAScheduleInfo(TTI,VeUEId, FBId, m_DRAClusterTDRInfo[clusterIdx], occupiedTTI));

	}
	DRAWriteScheduleInfo(g_OutDRAScheduleInfo);
}




void cRSU::DRAConflictListener(int TTI) {
	//-----------------------TEST-----------------------
	/*���ÿ���ص��б�Ϊ�գ�˵����һ�εĳ�ͻ��δ������ϣ�˵��������Ҫ����*/
	if (m_DRAConflictInfoList.size() != 0) throw Exp("cRSU::DRAConflictListener()");
	//-----------------------TEST-----------------------
	for (int clusterIdx = 0;clusterIdx < m_DRAClusterNum;clusterIdx++) {
		for (int FBIdx = 0;FBIdx < gc_DRA_FBNum;FBIdx++) {
			list<sDRAScheduleInfo> &list = m_DRAScheduleList[clusterIdx][FBIdx];
			if (list.size() > 1) {//����һ��VeUE�ڵ�ǰTTI����FB�ϴ��䣬�������˳�ͻ��������ӵ���ͻ�б�ȴ����¼����������
				for (sDRAScheduleInfo &info : list) {
					m_DRAConflictInfoList.push_back(tuple<int,int,int>(info.VeUEId, clusterIdx, FBIdx));
				}		
			}
			else if (list.size() == 1) {//ֻ��һ���û��ڴ��䣬���û�����ȷ�Ĵ����������ݣ����뿪��֮ǰ��
			    /*-----------------------WARN-----------------------
				* û�п��ǵ�VeUE���ڴ����ź�ʱ���ִؽ��������һ���ص����
				*-----------------------WARN-----------------------*/

				/*�����ǰTTI==m_DRA_RBIsAvailable[clusterIdx][FBIdx]���¶�Ӧ������*/
				if (TTI == m_DRA_RBIsAvailable[clusterIdx][FBIdx])
					m_DRAScheduleList[clusterIdx][FBIdx].clear();
			}
		}
	}

	DRAWriteProcessInfo(g_OutDRAProcessInfo, 2);

	/*�����ͻ��ά��m_DRA_RBIsAvailable�Լ�m_DRAScheduleList*/
	DRAConflictSolve(TTI);


	if (m_DRAConflictInfoList.size() != 0) throw Exp("cRSU::DRAConflictListener");
}


void cRSU::DRAConflictSolve(int TTI) {
	for (const tuple<int, int, int> &t : m_DRAConflictInfoList) {
		int VeUEId= get<0>(t);
		int clusterIdx = get<1>(t);
		int FBIdx = get<2>(t);
		m_DRAScheduleList[clusterIdx][FBIdx].clear();
		m_DRA_RBIsAvailable[clusterIdx][FBIdx] = TTI;//����FB��Դ�ڴ�ʱ�ͷ�
		pushToRSULevelWaitingVeUEIdList(VeUEId);
	}
	m_DRAConflictInfoList.clear();
}

void cRSU::pushToRSULevelWaitingVeUEIdList(int VeUEId) {
	m_DRAWaitingVeUEIdList.push_back(VeUEId);
}
void cRSU::pushToSystemLevelRSUSwitchVeUEIdList(int VeUEId, std::list<int>& systemDRA_RSUSwitchVeUEIdList) {
	systemDRA_RSUSwitchVeUEIdList.push_back(VeUEId);
}

void cRSU::pushToRSULevelCallVeUEIdList(int VeUEId) {
	m_DRACallVeUEIdList.push_back(VeUEId);
}



void cRSU::DRAWriteScheduleInfo(std::ofstream& out) {
	out << "    RSU[" << m_RSUId << "] :" << endl;
	out << "    {" << endl;
	for (int clusterIdx = 0; clusterIdx < m_DRAClusterNum; clusterIdx++) {
		out << "        Cluster[" << clusterIdx << "] :" << endl;
		out << "        {" << endl;
		for (int FBIdx = 0; FBIdx < gc_DRA_FBNum; FBIdx++) {
			out << "            FB[" << FBIdx << "] :" << endl;
			out << "            {" << endl;
			out << "                Released TTI: " << m_DRA_RBIsAvailable[clusterIdx][FBIdx] << endl;
			int cnt = 0;
			out << "                ScheduleTTLInterval List: " << endl;
			out << "                {" << endl;
			for (sDRAScheduleInfo & info : m_DRAScheduleList[clusterIdx][FBIdx]) {
				out << "                    { VeUEId :" << info.VeUEId << " ,  List: ";
				for (const tuple<int, int> &t : info.occupiedInterval)
					out << "[ " << get<0>(t) << " , " << get<1>(t) << " ] , ";
				out << "}" << endl;
			}
			out << "                }" << endl;
			out << "            }" << endl;
		}
		out << "        }" << endl;
	}
	out << "    }" << endl;
}

void cRSU::DRAWriteProcessInfo(std::ofstream& out, int type) {
	switch (type) {
	case 0://д������������Ϣ
		out << "    RSU[" << m_RSUId <<left<<setw(15)<< "] 's CallVeUEIdList : { ";
		for (int VeUEId : m_DRACallVeUEIdList)
			out << VeUEId << " , ";
		out << "}" << endl;
		break;
	case 1://д��ȴ��������Ϣ
		out << "    RSU[" << m_RSUId << left << setw(15) << "] 's WaitingVeUEIdList : { ";
		for (int VeUEId : m_DRAWaitingVeUEIdList)
			g_OutDRAProcessInfo << VeUEId << " , ";
		out << "}" << endl;
		break;
	case 2://д���ͻ�������Ϣ
		out << "    RSU[" << m_RSUId << left << setw(15) << "] 's ConflictList : { ";
		for (const tuple<int,int,int> &t : m_DRAConflictInfoList)
			out << "[ " << get<0>(t)<<" , "<<get<1>(t)<<" , "<<get<2>(t)<<" ] ";
		out << "}" << endl;
		break;

	}
}
