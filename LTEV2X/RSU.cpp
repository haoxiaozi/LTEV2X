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
#include"Utility.h"

using namespace std;

int cRSU::count = 0;

cRSU::cRSU() :m_DRAClusterNum(4) {
	m_DRAClusterVeUEIdList = vector<list<int>>(m_DRAClusterNum);
	m_DRA_RBIsAvailable = vector<vector<int>>(m_DRAClusterNum, vector<int>(gc_DRA_FBNum, -1));
	m_DRAScheduleList = vector<vector<list<sDRAScheduleInfo>>>(m_DRAClusterNum, vector<list<sDRAScheduleInfo>>(gc_DRA_FBNum));
}


int cRSU::DRAGetClusterIdx(int ATTI) {
	int roundATTI = ATTI%gc_DRA_NTTI; //��ATTIӳ�䵽[0-gc_DRA_NTTI)�ķ�Χ
	for (int clusterIdx = 0; clusterIdx < m_DRAClusterNum; clusterIdx++)
		if (roundATTI <= get<1>(m_DRAClusterTDRInfo[clusterIdx])) return clusterIdx;
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
	m_DRAAdmissionEventIdList.clear();
}


void cRSU::DRAGroupSizeBasedTDM() {
	/*��ʼ��*/
	m_DRAClusterTDRInfo = vector<tuple<int, int, int>>(m_DRAClusterNum, tuple<int, int, int>(0, 0, 0));

	/*����ÿ��TTIʱ϶��Ӧ��VeUE��Ŀ(double)��!!!������!!��*/
	double VUESizePerTTI = static_cast<double>(m_VeUEIdList.size()) / static_cast<double>(gc_DRA_NTTI);

	/*clusterSize�洢ÿ���ص�VeUE��Ŀ(double)��!!!������!!��*/
	std::vector<double> clusterSize(m_DRAClusterNum, 0);

	/*��ʼ��clusterSize*/
	for (int clusterIdx = 0; clusterIdx < m_DRAClusterNum; clusterIdx++)
		clusterSize[clusterIdx] = static_cast<double>(m_DRAClusterVeUEIdList[clusterIdx].size());

	/*���ȸ�������һ�����Ĵط���һ��TTI*/
	int basicNTTI = 0;
	for (int clusterIdx = 0;clusterIdx < m_DRAClusterNum;clusterIdx++) {
		if (m_DRAClusterVeUEIdList[clusterIdx].size() != 0) {//����ô���������һ��VeUE��ֱ�ӷ����һ����λ��ʱ����Դ
			get<2>(m_DRAClusterTDRInfo[clusterIdx]) = 1;
			clusterSize[clusterIdx] -= VUESizePerTTI;
			basicNTTI++;
		}
	}


	/*���˸���Ϊ�յĴط����һ��TTI�⣬ʣ��ɷ����TTI����*/
	int remainNTTI = gc_DRA_NTTI - basicNTTI;
	
	/*����ʣ�µ���Դ�飬ѭ������һʱ϶�������ǰ������ߵĴأ�����֮�󣬸��Ķ�Ӧ�ı�������ȥ��ʱ϶��Ӧ��VeUE����*/
	while (remainNTTI > 0) {
		int dex = getMaxIndex(clusterSize);
		if (dex == -1) throw Exp("������û�з����ʱ����Դ������ÿ�����ڵ�VeUE�Ѿ�Ϊ����");
		get<2>(m_DRAClusterTDRInfo[dex])++;
		remainNTTI--;
		clusterSize[dex] -= VUESizePerTTI;
	}

	/*��ʼ�������䷶Χ��������*/
	get<0>(m_DRAClusterTDRInfo[0]) = 0;
	get<1>(m_DRAClusterTDRInfo[0]) = get<0>(m_DRAClusterTDRInfo[0]) + get<2>(m_DRAClusterTDRInfo[0]) - 1;
	for (int clusterIdx = 1;clusterIdx < m_DRAClusterNum;clusterIdx++) {
		get<0>(m_DRAClusterTDRInfo[clusterIdx]) = get<1>(m_DRAClusterTDRInfo[clusterIdx - 1]) + 1;
		get<1>(m_DRAClusterTDRInfo[clusterIdx]) = get<0>(m_DRAClusterTDRInfo[clusterIdx]) + get<2>(m_DRAClusterTDRInfo[clusterIdx]) - 1;
	}
}


void cRSU::DRAProcessSystemLevelEventList(int ATTI, int STTI,const std::vector<cVeUE>& systemVeUEVec, const std::vector<sEvent>& systemEventList, const std::vector<std::list<int>>& systemEventTTIList) {
	int clusterIdx = DRAGetClusterIdx(ATTI);//��ǰ�ɴ������ݵĴر��
	for (int eventId : systemEventTTIList[(ATTI-STTI)% systemEventTTIList.size()]) {
		sEvent event = systemEventList[eventId];
		int VeUEId = event.VeUEId;
		if (systemVeUEVec[VeUEId].m_RSUId != m_RSUId) {//��ǰ�¼���Ӧ��VeUE���ڵ�ǰRSU�У���������
			continue;
		}
		else {//��ǰ�¼���Ӧ��VeUE�ڵ�ǰRSU��
			if (systemVeUEVec[VeUEId].m_ClusterIdx == clusterIdx)//��ǰʱ���¼������е�VeUEǡ��λ�ڸ�RSU�ĸô��ڣ���ӵ���ǰ��������
				pushToRSULevelCallVeEventIdList(eventId);
			else//���򣬵�ǰ�¼��ڴ�ʱ�����������䣬Ӧת��ȴ�����
				pushToRSULevelWaitingEventIdList(eventId);
		}
	}
}


void cRSU::DRAProcessRSULevelWaitingVeUEIdList(int ATTI, const std::vector<cVeUE>& systemVeUEVec, const std::vector<sEvent>& systemEventVec,std::list<int> &systemDRA_RSUSwitchEventIdList) {
	int clusterIdx = DRAGetClusterIdx(ATTI);//���ڵ����еĴر��
	list<int>::iterator it = m_DRAWaitingEventIdList.begin();
	while (it != m_DRAWaitingEventIdList.end()) {
		int eventId = *it;
		int VeUEId = systemEventVec[eventId].VeUEId;
		if (systemVeUEVec[VeUEId].m_RSUId != m_RSUId) {//��VeUE�Ѿ����ڸ�RSU��Χ��
			pushToSystemLevelRSUSwitchEventIdList(eventId,systemDRA_RSUSwitchEventIdList);//������ӵ�System�����RSU�л�������
			it=m_DRAWaitingEventIdList.erase(it);//����ӳ�ͻ������ɾ��
		}
		else {//��Ȼ���ڵ�ǰRSU��Χ��
			if (systemVeUEVec[VeUEId].m_ClusterIdx == clusterIdx) {//��VeUE�������ڵ��ȵĴ���
				pushToRSULevelCallVeEventIdList(eventId);//��ӵ�RSU����ĺ���������
				it=m_DRAWaitingEventIdList.erase(it);//����ӳ�ͻ������ɾ��
			}
			else {//��VeUE���ڵ�ǰ�����ȵĴ���
				it++;
				continue; //�����ȴ�����ǰTTI��������
			}
		}
	}
}




void cRSU::DRAProcessSystemLevelRSUSwitchList(int ATTI, const std::vector<cVeUE>& systemVeUEVec, const std::vector<sEvent>& systemEventVec,std::list<int> &systemDRA_RSUSwitchEventIdList) {
	list<int>::iterator it = systemDRA_RSUSwitchEventIdList.begin();
	int clusterIdx = DRAGetClusterIdx(ATTI);//��ǰ�ɴ������ݵĴر��
	while (it != systemDRA_RSUSwitchEventIdList.end()) {
		int eventId = *it;
		int VeUEId = systemEventVec[eventId].VeUEId;
		if (systemVeUEVec[VeUEId].m_RSUId != m_RSUId) {//���л������е��¼���Ӧ��VeUE�������ڵ�ǰ�أ���������
			it++;
			continue;
		}
		else {////���л������е�VeUE�����ڵ�ǰ��
			if (systemVeUEVec[VeUEId].m_ClusterIdx == clusterIdx) {//���л������е�VeUEǡ��λ�ڸ�RSU�ĵ�ǰ���ڣ���ӵ���ǰ��������
				pushToRSULevelCallVeEventIdList(eventId);
				it=systemDRA_RSUSwitchEventIdList.erase(it);
			}
			else {//�����ڵ�ǰ�أ�ת��ȴ�����
				pushToRSULevelWaitingEventIdList(eventId);
				it=systemDRA_RSUSwitchEventIdList.erase(it);
			}
		}
	}
}


void cRSU::DRASelectBasedOnP13(int ATTI, std::vector<cVeUE>&systemVeUEVec, const std::vector<sEvent>& systemEventVec) {
}

void cRSU::DRASelectBasedOnP23(int ATTI, std::vector<cVeUE>&systemVeUEVec, const std::vector<sEvent>& systemEventVec) {
}

void cRSU::DRASelectBasedOnP123(int ATTI, std::vector<cVeUE>&systemVeUEVec, const std::vector<sEvent>& systemEventVec) {
	int roundATTI = ATTI%gc_DRA_NTTI;//��ATTIӳ�䵽[0-gc_DRA_NTTI)�ķ�Χ

	int clusterIdx = DRAGetClusterIdx(ATTI);
	vector<int> curAvaliableFB;//��ǰTTI���õ�Ƶ���


	for (int i = 0; i < gc_DRA_FBNum; i++)
		if (ATTI > m_DRA_RBIsAvailable[clusterIdx][i]) curAvaliableFB.push_back(i); //������ռ�õ�RB��Ŵ���


	if (curAvaliableFB.size() == 0) {//�����Դȫ���Ѿ���ռ���ˣ���ô����������ȫ��ת��ȴ�����
		for (int eventId : m_DRAAdmissionEventIdList)
			pushToRSULevelWaitingEventIdList(eventId);
		return;//ֱ�ӷ���
	}

	for (int eventId : m_DRAAdmissionEventIdList) {//�����ô��ں��������е��¼�
		int VeUEId = systemEventVec[eventId].VeUEId;
		//Ϊ��ǰ�û��ڿ��õ�RB�������ѡ��һ����ÿ���û��������ѡ�����FB��

		
		int FBId = systemVeUEVec[VeUEId].RBSelectBasedOnP2(curAvaliableFB);

		//��ȡ��ǰ�û���Ҫ�������Ϣռ�õ�ʱ϶(Occupy TTI)
		int occupiedTTI = systemEventVec[eventId].message.DRA_ONTTI;


		//���㵱ǰ��Ϣ��ռ����Դ����ͷ�ʱ��,��д��m_DRA_RBIsAvailable
		int begin = get<0>(m_DRAClusterTDRInfo[clusterIdx]),
			end = get<1>(m_DRAClusterTDRInfo[clusterIdx]),
			len = get<2>(m_DRAClusterTDRInfo[clusterIdx]);
		int nextTurnBeginTTI = ATTI - roundATTI + gc_DRA_NTTI;//��RSU��һ�ֵ��ȵ���ʼTTI����һ���صĿ�ʼʱ�̣�
		int remainTTI = end - roundATTI + 1;//��ǰһ�ַ����иô�ʣ��Ŀɷ���ʱ϶
		int overTTI = occupiedTTI - remainTTI;//��Ҫ����һ�֣����¼��ֽ��д����ʱ϶����
		if (overTTI <= 0)
			m_DRA_RBIsAvailable[clusterIdx][FBId] = max(ATTI + occupiedTTI - 1, m_DRA_RBIsAvailable[clusterIdx][FBId]);
		else {
			int n = overTTI / len;
			if (overTTI%len == 0) m_DRA_RBIsAvailable[clusterIdx][FBId] = max(nextTurnBeginTTI + end + (n - 1)*gc_DRA_NTTI, m_DRA_RBIsAvailable[clusterIdx][FBId]);
			else m_DRA_RBIsAvailable[clusterIdx][FBId] = max(nextTurnBeginTTI + begin + n*gc_DRA_NTTI + overTTI%len - 1, m_DRA_RBIsAvailable[clusterIdx][FBId]);
		}

		//д�������Ϣ
		m_DRAScheduleList[clusterIdx][FBId].push_back(sDRAScheduleInfo(ATTI, eventId, FBId, m_DRAClusterTDRInfo[clusterIdx], occupiedTTI));

	}
	DRAWriteScheduleInfo(g_OutDRAScheduleInfo);
}




void cRSU::DRAConflictListener(int ATTI) {
	/*-----------------------WARN-----------------------
	* ���ÿ���ص��б�Ϊ�գ�˵����һ�εĳ�ͻ��δ������ϣ�˵��������Ҫ����
	-----------------------WARN-----------------------*/
	if (m_DRAConflictInfoList.size() != 0) throw Exp("cRSU::DRAConflictListener()");


	for (int clusterIdx = 0;clusterIdx < m_DRAClusterNum;clusterIdx++) {
		for (int FBIdx = 0;FBIdx < gc_DRA_FBNum;FBIdx++) {
			list<sDRAScheduleInfo> &lst = m_DRAScheduleList[clusterIdx][FBIdx];
			if (lst.size() > 1) {//����һ��VeUE�ڵ�ǰTTI����FB�ϴ��䣬�������˳�ͻ��������ӵ���ͻ�б�ȴ����¼����������
				for (sDRAScheduleInfo &info : lst) {
					m_DRAConflictInfoList.push_back(tuple<int,int,int>(info.eventId, clusterIdx, FBIdx));
				}		
			}
			else if (lst.size() == 1) {//ֻ��һ���û��ڴ��䣬���û�����ȷ�Ĵ����������ݣ����뿪��֮ǰ��
			    /*-----------------------WARN-----------------------
				* û�п��ǵ�VeUE���ڴ����ź�ʱ���ִؽ��������һ���ص����
				*-----------------------WARN-----------------------*/

				/*�����ǰTTI==m_DRA_RBIsAvailable[clusterIdx][FBIdx]���¶�Ӧ������*/
				if (ATTI == m_DRA_RBIsAvailable[clusterIdx][FBIdx]) {
					Log::log("Transmit Succeed", lst.begin()->toLogString(0));
					m_DRAScheduleList[clusterIdx][FBIdx].clear();			
				}
			}
		}
	}

	DRAWriteProcessInfo(g_OutDRAProcessInfo, 2);

	/*�����ͻ��ά��m_DRA_RBIsAvailable�Լ�m_DRAScheduleList*/
	DRAConflictSolve(ATTI);


	if (m_DRAConflictInfoList.size() != 0) throw Exp("cRSU::DRAConflictListener");
}


void cRSU::DRAConflictSolve(int ATTI) {
	for (const tuple<int, int, int> &t : m_DRAConflictInfoList) {
		int eventId = get<0>(t);
		int clusterIdx = get<1>(t);
		int FBIdx = get<2>(t);
		m_DRAScheduleList[clusterIdx][FBIdx].clear();
		m_DRA_RBIsAvailable[clusterIdx][FBIdx] = ATTI;//����FB��Դ�ڴ�ʱ�ͷ�
		pushToRSULevelWaitingEventIdList(eventId);
	}
	m_DRAConflictInfoList.clear();
}


void cRSU::pushToRSULevelCallVeEventIdList(int eventId) {
	m_DRAAdmissionEventIdList.push_back(eventId);
}
void cRSU::pushToRSULevelWaitingEventIdList(int eventId) {
	m_DRAWaitingEventIdList.push_back(eventId);
}
void cRSU::pushToSystemLevelRSUSwitchEventIdList(int VeUEId, std::list<int>& systemDRA_RSUSwitchVeUEIdList) {
	systemDRA_RSUSwitchVeUEIdList.push_back(VeUEId);
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
				out << "                    { eventId :" << info.eventId << " ,  List: ";
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
		for (int VeUEId : m_DRAAdmissionEventIdList)
			out << VeUEId << " , ";
		out << "}" << endl;
		break;
	case 1://д��ȴ��������Ϣ
		out << "    RSU[" << m_RSUId << left << setw(15) << "] 's WaitingVeUEIdList : { ";
		for (int VeUEId : m_DRAWaitingEventIdList)
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
