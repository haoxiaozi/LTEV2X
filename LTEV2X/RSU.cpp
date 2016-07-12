#include<vector>
#include<math.h>
#include<iostream>
#include<sstream>
#include<utility>
#include<tuple>
#include<iomanip>
#include"RSU.h"
#include"Exception.h"
#include"Event.h"
#include"Global.h"
#include"Utility.h"

int newCount = 0;
int deleteCount = 0;

using namespace std;

int cRSU::count = 0;

cRSU::cRSU() :m_DRAClusterNum(4) {
	m_DRAClusterVeUEIdList = vector<list<int>>(m_DRAClusterNum);
	m_DRA_RBIsAvailable = vector<vector<int>>(m_DRAClusterNum, vector<int>(gc_DRA_FBNum, -1));
	m_DRAScheduleInfoTable = vector<vector<sDRAScheduleInfo*>>(m_DRAClusterNum, vector<sDRAScheduleInfo*>(gc_DRA_FBNum,nullptr));
	m_DRATransimitEventIdList = vector<list<sDRAScheduleInfo*>>(gc_DRA_FBNum, list<sDRAScheduleInfo*>(0, nullptr));
}


int cRSU::DRAGetClusterIdx(int ATTI) {
	int roundATTI = ATTI%gc_DRA_NTTI; //��ATTIӳ�䵽[0-gc_DRA_NTTI)�ķ�Χ
	for (int clusterIdx = 0; clusterIdx < m_DRAClusterNum; clusterIdx++)
		if (roundATTI <= get<1>(m_DRAClusterTDRInfo[clusterIdx])) return clusterIdx;
	return -1;
}


int cRSU::getMaxIndex(const std::vector<double>&clusterSize) {
	double max = 0;
	int dex = -1;
	for (int i = 0; i < static_cast<int>(clusterSize.size()); i++) {
		if (clusterSize[i] > max) {
			dex = i;
			max = clusterSize[i];
		}
	}
	return dex;
}

int cRSU::getClusterIdxOfVeUE(int VeUEId) {
	int dex = -1;
	for (int clusterIdx = 0;clusterIdx < m_DRAClusterNum;clusterIdx++) {
		for (int Id : m_DRAClusterVeUEIdList[clusterIdx])
			if (Id == VeUEId) return clusterIdx;
	}
	throw Exp("cRSU::getClusterIdxOfVeUE(int VeUEId)���ó����ڵ�ǰRSU��");
}


void cRSU::DRAInformationClean() {
	m_DRAAdmissionEventIdList.clear();
}


void cRSU::DRAGroupSizeBasedTDM() {
	/*�������������RSU����һ����ʱ*/
	if (m_VeUEIdList.size() == 0) {
		/*-----------------------WARN-----------------------
		* ����ֵΪ(0,-1,0)�ᵼ�»�ȡ��ǰ�ر��ʧ�ܣ����������ط���Ҫ����
		* ���ֱ�Ӹ�ÿ���ض���ֵΪ�������䣬����Ҳû���κ����ã����������������
		*-----------------------WARN-----------------------*/
		m_DRAClusterTDRInfo = vector<tuple<int, int, int>>(m_DRAClusterNum, tuple<int, int, int>(0, gc_DRA_NTTI-1, gc_DRA_NTTI));
		return;
	}

	/*��ʼ��*/
	m_DRAClusterTDRInfo = vector<tuple<int, int, int>>(m_DRAClusterNum, tuple<int, int, int>(0, 0, 0));

	/*����ÿ��TTIʱ϶��Ӧ��VeUE��Ŀ(double)��!!!������!!��*/
	double VeUESizePerTTI = static_cast<double>(m_VeUEIdList.size()) / static_cast<double>(gc_DRA_NTTI);

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
			clusterSize[clusterIdx] -= VeUESizePerTTI;
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
		clusterSize[dex] -= VeUESizePerTTI;
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
			if (systemVeUEVec[VeUEId].m_ClusterIdx == clusterIdx) {//��ǰʱ���¼������е�VeUEǡ��λ�ڸ�RSU�ĸô��ڣ���ӵ���ǰ��������
				pushToRSULevelAdmissionEventIdList(eventId);
				//-----------------------TEST-----------------------
				ostringstream ss;
				ss<<"Event[" << eventId << "]��From_EventList_To_AdmissiontList";
				Log::log(g_OutLogInfo, "Switch", ss.str());
				//-----------------------TEST-----------------------
			}
			else {//���򣬵�ǰ�¼��ڴ�ʱ�����������䣬Ӧת��ȴ�����
				pushToRSULevelWaitingEventIdList(eventId);
				//-----------------------TEST-----------------------
				ostringstream ss;
				ss << "Event[" << eventId << "]��From_EventList_To_WaitingList";
				Log::log(g_OutLogInfo, "Switch", ss.str());
				//-----------------------TEST-----------------------
			}
		}
	}
}


void cRSU::DRAProcessRSULevelScheduleInfoTable(int ATTI, const std::vector<cVeUE>& systemVeUEVec, const std::vector<sEvent>& systemEventVec, std::list<int> &systemDRA_RSUSwitchEventIdList) {
	int clusterIdx = DRAGetClusterIdx(ATTI);//���ڵ����еĴر��
	for (int FBIdx = 0;FBIdx < gc_DRA_FBNum;FBIdx++) {
		if (m_DRAScheduleInfoTable[clusterIdx][FBIdx] == nullptr) {//��ǰFB�����¼��ڴ���
			continue;
		}
		else {
			int eventId = m_DRAScheduleInfoTable[clusterIdx][FBIdx]->eventId;
			int VeUEId = systemEventVec[eventId].VeUEId;
			if (systemVeUEVec[VeUEId].m_RSUId != m_RSUId) {//��VeUE���ڵ�ǰRSU�У�Ӧ����ѹ��System������л�����
				pushToSystemLevelRSUSwitchEventIdList(eventId, systemDRA_RSUSwitchEventIdList);
				
				//���ͷŸõ�����Ϣ����Դ
				delete m_DRAScheduleInfoTable[clusterIdx][FBIdx];
				deleteCount++;
				m_DRAScheduleInfoTable[clusterIdx][FBIdx] = nullptr;

				//�ͷ�FB��Դ
				m_DRA_RBIsAvailable[clusterIdx][FBIdx] = ATTI;

				//-----------------------TEST-----------------------
				ostringstream ss;
				ss << "Event[" << eventId << "]��From_ScheduleTable_To_RSUSwitchList";
				Log::log(g_OutLogInfo, "Switch", ss.str());
				//-----------------------TEST-----------------------
			}
			else {
				if (getClusterIdxOfVeUE(VeUEId) != clusterIdx) {//RSU�ڲ������˴��л�������ӵ��ȱ���ȡ����ѹ��ȴ�����
					pushToRSULevelWaitingEventIdList(eventId);

					//���ͷŸõ�����Ϣ����Դ
					delete m_DRAScheduleInfoTable[clusterIdx][FBIdx];
					deleteCount++;
					m_DRAScheduleInfoTable[clusterIdx][FBIdx] = nullptr;

					//�ͷ�FB��Դ
					m_DRA_RBIsAvailable[clusterIdx][FBIdx] = ATTI;

					//-----------------------TEST-----------------------
					ostringstream ss;
					ss << "Event[" << eventId << "]��From_ScheduleTable_To_WaitingList";
					Log::log(g_OutLogInfo, "Switch", ss.str());
					//-----------------------TEST-----------------------
				}
				else {//��û�з���RSU�л���Ҳû�з���RSU�ڴ��л���ʲôҲ����
					//doing nothing
				}
			}
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
			it=m_DRAWaitingEventIdList.erase(it);//����ӵȴ�������ɾ��
			//-----------------------TEST-----------------------
			ostringstream ss;
			ss << "Event[" << eventId << "]��From_WaitingList_To_RSUSwitchList";
			Log::log(g_OutLogInfo, "Switch", ss.str());
			//-----------------------TEST-----------------------
		}
		else {//��Ȼ���ڵ�ǰRSU��Χ��
			if (systemVeUEVec[VeUEId].m_ClusterIdx == clusterIdx) {//��VeUE�������ڵ��ȵĴ���
				pushToRSULevelAdmissionEventIdList(eventId);//��ӵ�RSU����Ľ���������
				it=m_DRAWaitingEventIdList.erase(it);//����ӳ�ͻ������ɾ��
				//-----------------------TEST-----------------------
				ostringstream ss;
				ss << "Event[" << eventId << "]��From_WaitingList_To_AdmissionList";
				Log::log(g_OutLogInfo, "Switch", ss.str());
				//-----------------------TEST-----------------------
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
				pushToRSULevelAdmissionEventIdList(eventId);
				it=systemDRA_RSUSwitchEventIdList.erase(it);
				//-----------------------TEST-----------------------
				ostringstream ss;
				ss << "Event[" << eventId << "]��From_RSUSwitchList_To_AdmissionList";
				Log::log(g_OutLogInfo, "Switch", ss.str());
				//-----------------------TEST-----------------------
			}
			else {//�����ڵ�ǰ�أ�ת��ȴ�����
				pushToRSULevelWaitingEventIdList(eventId);
				it=systemDRA_RSUSwitchEventIdList.erase(it);
				//-----------------------TEST-----------------------
				ostringstream ss;
				ss << "Event[" << eventId << "]��From_RSUSwitchList_To_WaitingList";
				Log::log(g_OutLogInfo, "Switch", ss.str());
				//-----------------------TEST-----------------------
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

	for (int FBIdx = 0; FBIdx < gc_DRA_FBNum; FBIdx++) 
		if (ATTI > m_DRA_RBIsAvailable[clusterIdx][FBIdx]) curAvaliableFB.push_back(FBIdx); //������ռ�õ�RB��Ŵ���

	if (curAvaliableFB.size() == 0) {//�����Դȫ���Ѿ���ռ���ˣ���ô����������ȫ��ת��ȴ�����
		for (int eventId : m_DRAAdmissionEventIdList)
			pushToRSULevelWaitingEventIdList(eventId);
		return;//ֱ�ӷ���
	}

	for (int eventId : m_DRAAdmissionEventIdList) {//�����ô��ڽ��������е��¼�

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

		//��������Ϣѹ��m_DRATransimitEventIdList��
		m_DRATransimitEventIdList[FBId].push_back(new sDRAScheduleInfo(ATTI, eventId, m_RSUId,FBId, m_DRAClusterTDRInfo[clusterIdx], occupiedTTI));
		newCount++;
	}

	pullFromScheduleInfoTable(ATTI);
	

	DRAWriteScheduleInfo(g_OutDRAScheduleInfo);
}



void cRSU::DRAConflictListener(int ATTI) {
	int clusterIdx = DRAGetClusterIdx(ATTI);
	for (int FBIdx = 0;FBIdx < gc_DRA_FBNum;FBIdx++) {
		list<sDRAScheduleInfo*> &lst = m_DRATransimitEventIdList[FBIdx];
		if (lst.size() > 1) {//����һ��VeUE�ڵ�ǰTTI����FB�ϴ��䣬�������˳�ͻ��������ӵ��ȴ��б�
			//-----------------------��ͻ����-----------------------
			for (sDRAScheduleInfo* info : lst) {
				//���Ƚ��¼�ѹ��waiting�б�
				pushToRSULevelWaitingEventIdList(info->eventId);
				delete info;
				info = nullptr;
				deleteCount++;
			}
			//�ͷ���Դ
			m_DRA_RBIsAvailable[clusterIdx][FBIdx] = ATTI;
			//-----------------------��ͻ����-----------------------
		}
		else if (lst.size() == 1) {//ֻ��һ���û��ڴ��䣬���û�����ȷ�Ĵ����������ݣ����뿪��֮ǰ��
			sDRAScheduleInfo *info = *lst.begin();
			//ά��ռ������
			list<tuple<int, int>> &OIList = info->occupiedIntervalList;

			tuple<int, int> &firstInterval = *OIList.begin();
			get<0>(firstInterval)++;
			if (get<0>(firstInterval) > get<1>(firstInterval)) 
				OIList.erase(OIList.begin());//ɾ����һ������
					
			if(OIList.size()==0){//˵���������Ѿ��������
				//-----------------------TEST-----------------------
				ostringstream ss;
				ss << "{ EventId = " << left << setw(3) << info->eventId;
				ss << "   RSUId = " << left << setw(3) << info->RSUId;
				ss << "   FBIdx = " << info->FBIdx << " }";
				Log::log(g_OutLogInfo,"Succeed", ss.str());
				//-----------------------TEST-----------------------
				//ɾ���õ�����Ϣ
				delete *lst.begin();
				deleteCount++;
			}
			else {//��������δ���꣬����ѹ��m_DRAScheduleInfoTable
				pushToScheduleInfoTable(clusterIdx, FBIdx, *lst.begin());
				*lst.begin() = nullptr;
			}
		}
		lst.clear();
	}
}




void cRSU::pushToRSULevelAdmissionEventIdList(int eventId) {
	m_DRAAdmissionEventIdList.push_back(eventId);
}

void cRSU::pushToRSULevelWaitingEventIdList(int eventId) {
	m_DRAWaitingEventIdList.push_back(eventId);
}

void cRSU::pushToSystemLevelRSUSwitchEventIdList(int VeUEId, std::list<int>& systemDRA_RSUSwitchVeUEIdList) {
	systemDRA_RSUSwitchVeUEIdList.push_back(VeUEId);
}

void cRSU::pushToScheduleInfoTable(int clusterIdx, int FBIdx, sDRAScheduleInfo*p) {
	m_DRAScheduleInfoTable[clusterIdx][FBIdx] = p;
}

void cRSU::pullFromScheduleInfoTable(int ATTI) {
	int clusterIdx = DRAGetClusterIdx(ATTI);
	/*�����ڵ��ȱ��е�ǰ���Դ������Ϣѹ��m_DRATransimitEventIdList*/
	for (int FBIdx = 0;FBIdx < gc_DRA_FBNum;FBIdx++) {
		if (m_DRAScheduleInfoTable[clusterIdx][FBIdx] != nullptr) {
			m_DRATransimitEventIdList[FBIdx].push_back(m_DRAScheduleInfoTable[clusterIdx][FBIdx]);
			m_DRAScheduleInfoTable[clusterIdx][FBIdx] = nullptr;
		}
	}
}


void cRSU::DRAWriteScheduleInfo(std::ofstream& out) {
	out << "    RSU[" << m_RSUId << "] :" << endl;
	out << "    {" << endl;
	for (int FBIdx = 0;FBIdx < gc_DRA_FBNum;FBIdx++) {
		out << "        FB[" << FBIdx << "] :" << endl;
		for (sDRAScheduleInfo* info : m_DRATransimitEventIdList[FBIdx]) {
			out<< info->toString(3) << endl;
		}
	}
	out << "    }" << endl;

	/*out << "    RSU[" << m_RSUId << "] :" << endl;
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
	sDRAScheduleInfo *info = m_DRAScheduleInfoTable[clusterIdx][FBIdx];
	if (info != nullptr) {
	out << "                    { eventId :" << info->eventId << " ,  List: ";
	for (const tuple<int, int> &t : info->occupiedIntervalList)
	out << "[ " << get<0>(t) << " , " << get<1>(t) << " ] , ";
	out << "}" << endl;
	}
	out << "                }" << endl;
	out << "            }" << endl;
	}
	out << "        }" << endl;
	}
	out << "    }" << endl;*/
}

void cRSU::DRAWriteProcessInfo(std::ofstream& out, int type) {
	switch (type) {
	case 0://д������������Ϣ
		out << "    RSU[" << m_RSUId << left << setw(15) << "] 's CallVeUEIdList : { ";
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
	}
}


