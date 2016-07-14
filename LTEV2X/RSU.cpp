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

int cRSU::s_RSUCount = 0;

const int cRSU::s_DRAPatternNumPerPatternType[s_DRAPatternTypeNum] = { 20,5 };

const int cRSU::s_DRA_FBNumPerPatternType[s_DRAPatternTypeNum] = { 1,5 };

const std::list<int> cRSU::s_DRAPatternIdxList[s_DRAPatternTypeNum] = { 
	Function::makeContinuousSequence(0,19),
	Function::makeContinuousSequence(20,24)
};

const int cRSU::s_DRATotalPatternNum = Function::sumArray(s_DRAPatternNumPerPatternType);


cRSU::cRSU() :m_DRAClusterNum(4) {
	m_DRAClusterVeUEIdList = vector<list<int>>(m_DRAClusterNum);
	m_DRAPatternIsAvailable = vector<vector<bool>>(m_DRAClusterNum, vector<bool>(s_DRATotalPatternNum, true));
	m_DRAScheduleInfoTable = vector<vector<sDRAScheduleInfo*>>(m_DRAClusterNum, vector<sDRAScheduleInfo*>(s_DRATotalPatternNum,nullptr));
	m_DRATransimitEventIdList = vector<list<sDRAScheduleInfo*>>(s_DRATotalPatternNum, list<sDRAScheduleInfo*>(0, nullptr));
}


int cRSU::DRAGetClusterIdx(int TTI) {
	int roundATTI = TTI%gc_DRA_NTTI; //��TTIӳ�䵽[0-gc_DRA_NTTI)�ķ�Χ
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


list<tuple<int, int>> cRSU::buildScheduleIntervalList(int TTI,sEvent event, std::tuple<int, int, int>ClasterTTI) {
	list<tuple<int,int>> scheduleIntervalList;
	int eventId = event.eventId;
	int occupiedTTI = event.message.DRA_ONTTI / cRSU::s_DRA_FBNumPerPatternType[event.message.messageType];
	int begin = std::get<0>(ClasterTTI),
		end = std::get<1>(ClasterTTI),
		len = std::get<2>(ClasterTTI);
	int roundTTI = TTI%gc_DRA_NTTI;
	int nextTurnBeginTTI = TTI - roundTTI + gc_DRA_NTTI;//��RSU��һ�ֵ��ȵ���ʼTTI����һ���صĿ�ʼʱ�̣�
	int remainTTI = end - roundTTI + 1;//��ǰһ�ֵ�����ʣ����õ�ʱ϶����
	int overTTI = occupiedTTI - remainTTI;//������ǰһ�ֵ��ȿ���ʱ϶�����Ĳ���
	if (overTTI <= 0) scheduleIntervalList.push_back(std::tuple<int, int>(TTI, TTI + occupiedTTI - 1));
	else {
		scheduleIntervalList.push_back(std::tuple<int, int>(TTI, TTI + remainTTI - 1));
		int n = overTTI / len;
		for (int i = 0; i < n; i++) scheduleIntervalList.push_back(std::tuple<int, int>(nextTurnBeginTTI + i*gc_DRA_NTTI + begin, nextTurnBeginTTI + begin + len - 1 + i*gc_DRA_NTTI));
		if (overTTI%len != 0) scheduleIntervalList.push_back(std::tuple<int, int>(nextTurnBeginTTI + n*gc_DRA_NTTI + begin, nextTurnBeginTTI + begin + n*gc_DRA_NTTI + overTTI%len - 1));
	}
	return scheduleIntervalList;
}




void cRSU::DRAInformationClean() {
	m_DRAAdmitEventIdList.clear();
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


void cRSU::DRAProcessSystemLevelEventList(int TTI,const std::vector<cVeUE>& systemVeUEVec, std::vector<sEvent>& systemEventVec, const std::vector<std::list<int>>& systemEventTTIList) {
	int clusterIdx = DRAGetClusterIdx(TTI);//��ǰ�ɴ������ݵĴر��
	for (int eventId : systemEventTTIList[TTI]) {//"�������"
		sEvent event = systemEventVec[eventId];
		int VeUEId = event.VeUEId;
		if (systemVeUEVec[VeUEId].m_RSUId != m_RSUId) {//��ǰ�¼���Ӧ��VeUE���ڵ�ǰRSU�У���������
			continue;
		}
		else {//��ǰ�¼���Ӧ��VeUE�ڵ�ǰRSU��
			if (systemVeUEVec[VeUEId].m_ClusterIdx == clusterIdx) {//��ǰʱ���¼������е�VeUEǡ��λ�ڸ�RSU�ĸô��ڣ���ӵ���ǰ��������
				pushToRSULevelAdmitEventIdList(eventId);
				//���¸��¼�����־
				ostringstream ss;
				ss << "{ TTI: " << left << setw(3) << TTI << " ; From: EventList ; To: RSU[" << m_RSUId << "]'s AdmitEventIdList }";
				systemEventVec[eventId].addLog(ss.str());
				//-----------------------OUTPUT-----------------------
				ss.str("");
				ss << "Event[" << eventId << "]��";
				ss << "{ From: EventList ; To: RSU[" << m_RSUId << "]'s AdmitEventIdList }";
				Log::log(g_OutLogInfo, "Switch", ss.str());
				//-----------------------OUTPUT-----------------------
			}
			else {//���򣬵�ǰ�¼��ڴ�ʱ�����������䣬Ӧת��ȴ�����
				pushToRSULevelWaitEventIdList(eventId);
				//���¸��¼�����־
				ostringstream ss;
				ss << "{ TTI: " << left << setw(3) << TTI << " ; From: EventList ; To: RSU[" << m_RSUId << "]'s WaitEventIdList }";
				systemEventVec[eventId].addLog(ss.str());
				//-----------------------OUTPUT-----------------------
				ss.str("");
				ss << "Event[" << eventId << "]��";
				ss << "{ From: EventList ; To: RSU[" << m_RSUId << "]'s WaitEventIdList }";
				Log::log(g_OutLogInfo, "Switch", ss.str());
				//-----------------------OUTPUT-----------------------
			}
		}
	}
}


void cRSU::DRAProcessRSULevelScheduleInfoTable(int TTI, const std::vector<cVeUE>& systemVeUEVec, std::vector<sEvent>& systemEventVec, std::list<int> &systemDRASwitchEventIdList) {
	for (int clusterIdx = 0; clusterIdx < m_DRAClusterNum; clusterIdx++) {
		for (int patternIdx = 0; patternIdx < s_DRATotalPatternNum; patternIdx++) {
			if (m_DRAScheduleInfoTable[clusterIdx][patternIdx] == nullptr) {//��ǰPattern�����¼��ڴ���
				continue;
			}
			else {
				int eventId = m_DRAScheduleInfoTable[clusterIdx][patternIdx]->eventId;
				int VeUEId = systemEventVec[eventId].VeUEId;
				if (systemVeUEVec[VeUEId].m_RSUId != m_RSUId) {//��VeUE���ڵ�ǰRSU�У�Ӧ����ѹ��System������л�����
					pushToSystemLevelSwitchEventIdList(eventId, systemDRASwitchEventIdList);

					//���ͷŸõ�����Ϣ����Դ
					delete m_DRAScheduleInfoTable[clusterIdx][patternIdx];
					deleteCount++;
					m_DRAScheduleInfoTable[clusterIdx][patternIdx] = nullptr;

					//�ͷ�Pattern��Դ
					m_DRAPatternIsAvailable[clusterIdx][patternIdx] = true;


					//���¸��¼�����־
					ostringstream ss;
					ss << "{ TTI: " << left << setw(3) << TTI << " ; From: RSU[" << m_RSUId << "]'s ScheduleTable[" << clusterIdx << "][" << patternIdx << "] ; To: SwitchList }";
					systemEventVec[eventId].addLog(ss.str());
					//-----------------------OUTPUT-----------------------
					ss.str("");
					ss << "Event[" << eventId << "]��";
					ss << "{ From: RSU[" << m_RSUId << "]'s ScheduleTable[" << clusterIdx << "][" << patternIdx << "] ; To: SwitchList }";
					Log::log(g_OutLogInfo, "Switch", ss.str());
					//-----------------------OUTPUT-----------------------
				}
				else {
					if (getClusterIdxOfVeUE(VeUEId) != clusterIdx) {//RSU�ڲ������˴��л�������ӵ��ȱ���ȡ����ѹ��ȴ�����
						pushToRSULevelWaitEventIdList(eventId);

						//���ͷŸõ�����Ϣ����Դ
						delete m_DRAScheduleInfoTable[clusterIdx][patternIdx];
						deleteCount++;
						m_DRAScheduleInfoTable[clusterIdx][patternIdx] = nullptr;

						//�ͷ�Pattern��Դ
						m_DRAPatternIsAvailable[clusterIdx][patternIdx] = true;

						//���¸��¼�����־
						ostringstream ss;
						ss << "{ TTI: " << left << setw(3) << TTI << " ; From: RSU[" << m_RSUId << "]'s ScheduleTable[" << clusterIdx << "][" << patternIdx << "] ; To: RSU[" << m_RSUId << "]'s WaitEventIdList }";
						systemEventVec[eventId].addLog(ss.str());
						//-----------------------OUTPUT-----------------------
						ss.str("");
						ss << "Event[" << eventId << "]��";
						ss << "{ From: RSU[" << m_RSUId << "]'s ScheduleTable[" << clusterIdx << "][" << patternIdx << "] ; To: RSU[" << m_RSUId << "]'s WaitEventIdList }";
						Log::log(g_OutLogInfo, "Switch", ss.str());
						//-----------------------OUTPUT-----------------------
					}
					else {//��û�з���RSU�л���Ҳû�з���RSU�ڴ��л���ʲôҲ����
						//doing nothing
					}
				}
			}
		}
	}
}


void cRSU::DRAProcessRSULevelWaitEventIdList(int TTI, const std::vector<cVeUE>& systemVeUEVec, std::vector<sEvent>& systemEventVec,std::list<int> &systemDRASwitchEventIdList) {
	int clusterIdx = DRAGetClusterIdx(TTI);//���ڵ����еĴر��
	list<int>::iterator it = m_DRAWaitEventIdList.begin();
	while (it != m_DRAWaitEventIdList.end()) {
		int eventId = *it;
		int VeUEId = systemEventVec[eventId].VeUEId;
		if (systemVeUEVec[VeUEId].m_RSUId != m_RSUId) {//��VeUE�Ѿ����ڸ�RSU��Χ��
			pushToSystemLevelSwitchEventIdList(eventId,systemDRASwitchEventIdList);//������ӵ�System�����RSU�л�������
			it=m_DRAWaitEventIdList.erase(it);//����ӵȴ�������ɾ��

			//���¸��¼�����־
			ostringstream ss;
			ss << "{ TTI: " << left << setw(3) << TTI << " ; From: RSU[" << m_RSUId << "]'s WaitEventIdList ; To: SwitchList }";
			systemEventVec[eventId].addLog(ss.str());
			//-----------------------OUTPUT-----------------------
			ss.str("");
			ss << "Event[" << eventId << "]��";
			ss << "{ From: RSU[" << m_RSUId << "]'s WaitEventIdList ; To: SwitchList }";
			Log::log(g_OutLogInfo, "Switch", ss.str());
			//-----------------------OUTPUT-----------------------
		}
		else {//��Ȼ���ڵ�ǰRSU��Χ��
			if (systemVeUEVec[VeUEId].m_ClusterIdx == clusterIdx) {//��VeUE�������ڵ��ȵĴ���
				pushToRSULevelAdmitEventIdList(eventId);//��ӵ�RSU����Ľ���������
				it=m_DRAWaitEventIdList.erase(it);//����ӳ�ͻ������ɾ��

				//���¸��¼�����־
				ostringstream ss;
				ss << "{ TTI: " << left << setw(3) << TTI << " ; From: RSU[" << m_RSUId << "]'s WaitEventIdList ; To: RSU[" << m_RSUId << "]'s AdmitEventIdList }";
				systemEventVec[eventId].addLog(ss.str());
				//-----------------------OUTPUT-----------------------
				ss.str("");
				ss << "Event[" << eventId << "]��";
				ss << "{ From: RSU[" << m_RSUId << "]'s WaitEventIdList ; To: RSU[" << m_RSUId << "]'s AdmitEventIdList }";
				Log::log(g_OutLogInfo, "Switch", ss.str());
				//-----------------------OUTPUT-----------------------
			}
			else {//��VeUE���ڵ�ǰ�����ȵĴ���
				it++;
				continue; //�����ȴ�����ǰTTI��������
			}
		}
	}
}




void cRSU::DRAProcessSystemLevelSwitchList(int TTI, const std::vector<cVeUE>& systemVeUEVec, std::vector<sEvent>& systemEventVec,std::list<int> &systemDRASwitchEventIdList) {
	list<int>::iterator it = systemDRASwitchEventIdList.begin();
	int clusterIdx = DRAGetClusterIdx(TTI);//��ǰ�ɴ������ݵĴر��
	while (it != systemDRASwitchEventIdList.end()) {
		int eventId = *it;
		int VeUEId = systemEventVec[eventId].VeUEId;
		if (systemVeUEVec[VeUEId].m_RSUId != m_RSUId) {//���л������е��¼���Ӧ��VeUE�������ڵ�ǰ�أ���������
			it++;
			continue;
		}
		else {//���л������е�VeUE�����ڵ�ǰ��
			if (systemVeUEVec[VeUEId].m_ClusterIdx == clusterIdx) {//���л������е�VeUEǡ��λ�ڸ�RSU�ĵ�ǰ���ڣ���ӵ���ǰ��������
				pushToRSULevelAdmitEventIdList(eventId);
				it=systemDRASwitchEventIdList.erase(it);

				//���¸��¼�����־
				ostringstream ss;
				ss << "{ TTI: " << left << setw(3) << TTI << " ; From: SwitchList ; To: RSU[" << m_RSUId << "]'s AdmitEventIdList }";
				systemEventVec[eventId].addLog(ss.str());
				//-----------------------OUTPUT-----------------------
				ss.str("");
				ss << "Event[" << eventId << "]��";
				ss << "{ From: SwitchList ; To: RSU[" << m_RSUId << "]'s AdmitEventIdList }";
				Log::log(g_OutLogInfo, "Switch", ss.str());
				//-----------------------OUTPUT-----------------------
			}
			else {//�����ڵ�ǰ�أ�ת��ȴ�����
				pushToRSULevelWaitEventIdList(eventId);
				it=systemDRASwitchEventIdList.erase(it);

				//���¸��¼�����־
				ostringstream ss;
				ss << "{ TTI: " << left << setw(3) << TTI << " ; From: SwitchList ; To: RSU[" << m_RSUId << "]'s WaitEventIdList }";
				systemEventVec[eventId].addLog(ss.str());
				//-----------------------OUTPUT-----------------------
				ss.str("");
				ss << "Event[" << eventId << "]��";
				ss << "{ From: SwitchList ; To: RSU[" << m_RSUId << "]'s WaitEventIdList }";
				Log::log(g_OutLogInfo, "Switch", ss.str());
				//-----------------------OUTPUT-----------------------
			}
		}
	}
}


void cRSU::DRASelectBasedOnP13(int TTI, std::vector<cVeUE>&systemVeUEVec, const std::vector<sEvent>& systemEventVec) {
}

void cRSU::DRASelectBasedOnP23(int TTI, std::vector<cVeUE>&systemVeUEVec, const std::vector<sEvent>& systemEventVec) {
}

void cRSU::DRASelectBasedOnP123(int TTI, std::vector<cVeUE>&systemVeUEVec, const std::vector<sEvent>& systemEventVec) {
	int roundATTI = TTI%gc_DRA_NTTI;//��TTIӳ�䵽[0-gc_DRA_NTTI)�ķ�Χ

	int clusterIdx = DRAGetClusterIdx(TTI);

	/*
	* ��ǰTTI���õ�Pattern����
	* �±�����Pattern����ı��
	* ÿ��list���������Pattern���õ�Pattern���
	*/
	vector<vector<int>> curAvaliablePatternIdx(s_DRAPatternTypeNum);
	int temCnt = 0;
	for (int patternTypeIdx = 0; patternTypeIdx < s_DRAPatternTypeNum; patternTypeIdx++) {
		for (int patternIdx : s_DRAPatternIdxList[patternTypeIdx]) {
			if (m_DRAPatternIsAvailable[clusterIdx][patternIdx]) {
				curAvaliablePatternIdx[patternTypeIdx].push_back(patternIdx);
				temCnt++;
			}
		}
	}

	if (temCnt == 0) {//�����Դȫ���Ѿ���ռ���ˣ���ô����������ȫ��ת��ȴ�����
		//---------------WRONG----------------
		for (int eventId : m_DRAAdmitEventIdList)
			pushToRSULevelWaitEventIdList(eventId);
		return;//ֱ�ӷ���
		//---------------WRONG----------------
	}

	for (int eventId : m_DRAAdmitEventIdList) {//�����ô��ڽ��������е��¼�

		int VeUEId = systemEventVec[eventId].VeUEId;

		//Ϊ��ǰ�û��ڿ��õĶ�Ӧ���¼����͵�Pattern�������ѡ��һ����ÿ���û��������ѡ�����Pattern��
		int patternIdx = systemVeUEVec[VeUEId].RBSelectBasedOnP2(curAvaliablePatternIdx, systemEventVec[eventId].message.messageType);

		//��ȡ��ǰ�û���Ҫ�������Ϣռ�õ�ʱ϶(Occupy TTI)
		int occupiedTTI = systemEventVec[eventId].message.DRA_ONTTI;


		//����Դ���Ϊռ��
		m_DRAPatternIsAvailable[clusterIdx][patternIdx] = false;

		//��������Ϣѹ��m_DRATransimitEventIdList��
		list<tuple<int,int>> scheduleIntervalList = buildScheduleIntervalList(TTI, systemEventVec[eventId], m_DRAClusterTDRInfo[clusterIdx]);
		m_DRATransimitEventIdList[patternIdx].push_back(new sDRAScheduleInfo(eventId, m_RSUId, patternIdx, scheduleIntervalList));
		newCount++;
	}

	pullFromScheduleInfoTable(TTI);
	
	//-----------------------OUTPUT-----------------------
	DRAWriteScheduleInfo(g_OutDRAScheduleInfo,TTI);
	//-----------------------OUTPUT-----------------------
}



void cRSU::DRAConflictListener(int TTI, std::vector<sEvent>& systemEventVec) {
	int clusterIdx = DRAGetClusterIdx(TTI);
	for (int patternIdx = 0;patternIdx < s_DRATotalPatternNum;patternIdx++) {
		list<sDRAScheduleInfo*> &lst = m_DRATransimitEventIdList[patternIdx];
		if (lst.size() > 1) {//����һ��VeUE�ڵ�ǰTTI����Pattern�ϴ��䣬�������˳�ͻ��������ӵ��ȴ��б�
			//-----------------------��ͻ����-----------------------
			for (sDRAScheduleInfo* info : lst) {
				//���Ƚ��¼�ѹ��waiting�б�
				pushToRSULevelWaitEventIdList(info->eventId);
	
				//���¸��¼�����־
				ostringstream ss;
				ss << "{ TTI: " << left << setw(3) << TTI << " ; From: RSU[" << m_RSUId << "]'s AdmitEventIdLIst ; To: RSU[" << m_RSUId << "]'s WaitEventIdList }";
				systemEventVec[info->eventId].addLog(ss.str());
				//-----------------------OUTPUT-----------------------
				ss.str("");
				ss << "Event[" << info->eventId << "]��";
				ss << "{ RSU[" << m_RSUId << "]'s AdmitEventIdLIst ; To: RSU[" << m_RSUId << "]'s WaitEventIdList }";
				Log::log(g_OutLogInfo, "Conflict", ss.str());
				//-----------------------OUTPUT-----------------------

				//�ͷ���Դ
				delete info;
				info = nullptr;
				deleteCount++;
			}
			//�ͷ���Դ
			m_DRAPatternIsAvailable[clusterIdx][patternIdx] = true;
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

				//���¸��¼�����־
				ostringstream ss;
				ss << "{ TTI: " << left << setw(3) << TTI << " ; RSU[" << m_RSUId << "]   ClusterIdx["<<clusterIdx<<"]    PatternIdx["<<patternIdx<<"] }";
				systemEventVec[info->eventId].addLog(ss.str());
				//-----------------------OUTPUT-----------------------
				ss.str("");
				ss << "Event[" << info->eventId << "]��";
				ss << "{ RSU[" << m_RSUId << "]   ClusterIdx[" << clusterIdx << "]    PatternIdx[" << patternIdx << "] }";
				Log::log(g_OutLogInfo, "Succeed", ss.str());
				//-----------------------OUTPUT-----------------------

				//ɾ���õ�����Ϣ
				delete *lst.begin();
				deleteCount++;

				//�ͷ���Դ
				m_DRAPatternIsAvailable[clusterIdx][patternIdx] = true;
			}
			else {//��������δ���꣬����ѹ��m_DRAScheduleInfoTable
				pushToScheduleInfoTable(clusterIdx, patternIdx, *lst.begin());
				*lst.begin() = nullptr;
			}
		}
		lst.clear();
	}
}




void cRSU::pushToRSULevelAdmitEventIdList(int eventId) {
	m_DRAAdmitEventIdList.push_back(eventId);
}

void cRSU::pushToRSULevelWaitEventIdList(int eventId) {
	m_DRAWaitEventIdList.push_back(eventId);
}

void cRSU::pushToSystemLevelSwitchEventIdList(int VeUEId, std::list<int>& systemDRASwitchVeUEIdList) {
	systemDRASwitchVeUEIdList.push_back(VeUEId);
}

void cRSU::pushToScheduleInfoTable(int clusterIdx, int patternIdx, sDRAScheduleInfo*p) {
	m_DRAScheduleInfoTable[clusterIdx][patternIdx] = p;
}

void cRSU::pullFromScheduleInfoTable(int TTI) {
	int clusterIdx = DRAGetClusterIdx(TTI);
	/*�����ڵ��ȱ��е�ǰ���Դ������Ϣѹ��m_DRATransimitEventIdList*/
	for (int patternIdx = 0;patternIdx < s_DRATotalPatternNum;patternIdx++) {
		if (m_DRAScheduleInfoTable[clusterIdx][patternIdx] != nullptr) {
			m_DRATransimitEventIdList[patternIdx].push_back(m_DRAScheduleInfoTable[clusterIdx][patternIdx]);
			m_DRAScheduleInfoTable[clusterIdx][patternIdx] = nullptr;
		}
	}
}


void cRSU::DRAWriteScheduleInfo(std::ofstream& out,int TTI) {
	int clusterIdx = DRAGetClusterIdx(TTI);
	out << "    RSU[" << m_RSUId << "] :" << endl;
	out << "    {" << endl;
	for (int patternIdx = 0;patternIdx < s_DRATotalPatternNum;patternIdx++) {
		out << "        Pattern[ " << left<<setw(3)<<patternIdx << "] : " << (m_DRAPatternIsAvailable[clusterIdx][patternIdx]?"Available":"Unavailable") << endl;
		for (sDRAScheduleInfo* info : m_DRATransimitEventIdList[patternIdx]) {
			out<< info->toString(3) << endl;
		}
	}
	out << "    }" << endl;

}

void cRSU::DRAWriteProcessInfo(std::ofstream& out, int type, const std::vector<sEvent>& systemEventVec) {
	switch (type) {
	case 0://д������������Ϣ
		out << "    RSU[" << m_RSUId << left << setw(15) << "] 's AdmitEventIdList : { ";
		for (int eventId : m_DRAAdmitEventIdList)
			out << "[ eventId = " << left << setw(3) << eventId << " , VeUEId = " << left << setw(3) << systemEventVec[eventId].VeUEId << " ] , ";
		out << "}" << endl;
		break;
	case 1://д��ȴ��������Ϣ
		out << "    RSU[" << m_RSUId << left << setw(15) << "] 's WaitEventIdList : { ";
		for (int eventId : m_DRAWaitEventIdList)
			out << "[ eventId = " << left << setw(3) << eventId << " , VeUEId = " << left << setw(3) << systemEventVec[eventId].VeUEId << " ] , ";
		out << "}" << endl;
		break;
	}
}


