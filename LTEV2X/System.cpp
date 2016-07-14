#include<iomanip>
#include<fstream>
#include"System.h"
#include"Global.h"
#include"RSU.h"

using namespace std;

void cSystem::process() {

	/*��������*/
	configure();

	/*�����ʼ��*/
	initialization();

	/*��ӡ�¼�����*/
	writeEventListInfo(g_OutEventListInfo);

	for (int count = 0;count < m_NTTI;count++) {
		cout << "Current TTI = " << m_TTI << endl;
		DRASchedule();
		m_TTI++;
	}

	/*��ӡ�¼���־��Ϣ*/
	writeEventLogInfo(g_OutEventLogInfo);
}

void cSystem::configure() {//ϵͳ�����������
	m_NTTI = 50;//����TTIʱ��
	m_Config.periodicEventNTTI = 20;
	m_Config.locationUpdateNTTI = 30;

	m_Config.VUENum = 30;
	m_Config.RSUNum = 2;
	m_Config.eNBNum = 1;
}


void cSystem::initialization() {
	srand((unsigned)time(NULL));//iomanip
	m_TTI = 0;
	Log::TTI = &m_TTI;

	m_eNBVec = vector<ceNB>(m_Config.eNBNum);
	m_RSUVec = vector<cRSU>(m_Config.RSUNum);
	m_VeUEVec = vector<cVeUE>(m_Config.VUENum);
	m_EventTTIList = vector<list<int>>(m_NTTI);



	//����RSU�ͻ�վλ�ù̶��������RSU������վ���г�ʼ������
	for (int RSUId = 0;RSUId < m_Config.RSUNum;RSUId++) {
		int eNBId = rand() % m_Config.eNBNum;
		m_eNBVec[eNBId].m_RSUIdList.push_back(RSUId);
	}

	m_DRAMode = P123;

	buildEventList();

}

void cSystem::buildEventList() {
	/*��ʱ��˳���¼���ID��ʱ����أ�IDԽС���¼�������ʱ��ԽС�����¼�����*/

	//�������ɸ����������������¼�����ʼʱ��
	vector<list<int>> startTTIVec(m_Config.periodicEventNTTI, list<int>({ -1 }));
	for (int VeUEId = 0; VeUEId < m_Config.VUENum; VeUEId++) {
		int startTTI = rand() % m_Config.periodicEventNTTI;
		startTTIVec[startTTI].push_back(VeUEId);
	}

	/*����startTTIVec�������PERIOD�¼�*/
	int CTTI = 0;
	while (CTTI < m_NTTI) {
		for (int TTIOffset = 0; TTIOffset < m_Config.periodicEventNTTI; TTIOffset++) {
			list<int>lst = startTTIVec[TTIOffset];
			for (int VeUEId : lst) {
				if (VeUEId == -1) {//�Ƿ�ID�����ڴ˲���������¼�
				/*-----------------------WARN-----------------------
				* ������Բ���������¼���Ϊ�˱�֤ʼ�������е�����б�Ϊ�ռ��ɣ���
				* �ڳ�ʼ��lst��ʱ������˷Ƿ���VeUEId=-1
				*-----------------------WARN-----------------------*/
				}
				else {//�Ϸ�ID����Ӹ��¼�
					if (CTTI + TTIOffset < m_NTTI) {
						/*-----------------------WARN-----------------------
						* ����������sEvent�Ķ���Ȼ����ѹ��m_EventVec
						* ����Vector<T>.push_back��ѹ�봫�����ĸ���Ʒ����˻����sEvent�Ŀ������캯��
						* sEventĬ�ϵĿ������캯���ḳֵid��Ա������ǰ�ȫ�ģ�
						*sEvent����Զ��忽�����캯���������ڹ��캯���ĳ�ʼ�����ֿ���id��Ա
						*-----------------------WARN-----------------------*/
						sEvent evt = sEvent(VeUEId, CTTI + TTIOffset, PERIOD);
						m_EventVec.push_back(evt);
						m_EventTTIList[CTTI + TTIOffset].push_back(evt.eventId);
					}
				}
			}
		}
		CTTI += m_Config.periodicEventNTTI;
	}
}

void cSystem::writeClusterPerformInfo(ofstream &out) {
	out << "[ TTI = " << left << setw(3) << m_TTI << "]" << endl;
	out << "{" << endl;
	//��ӡVeUE��Ϣ
	out << "    VUE Info: " << endl;
	out << "    {" << endl;
	for (cVeUE &_VeUE : m_VeUEVec) 
		out << _VeUE.toString(2) << endl;
	out << "    }" << endl;

	out << "\n\n\n";

	//��ӡ��վ��Ϣ
	out << "    eNB Info: " << endl;
	out << "    {" << endl;
	for (ceNB &_eNB : m_eNBVec)
		out << _eNB.toString(2) << endl;
	out << "    }" << endl;

	//��ӡRSU��Ϣ
	out << "    RSU Info: " << endl;
	out << "    {" << endl;
	for (cRSU &_RSU : m_RSUVec)
		out << _RSU.toString(2) << endl;
	out << "    }" << endl;

	out << "\n\n\n";

	//��ӡSystem��Switch����
	out << "    SwitchList Info: " << endl;
	out << "    {" << endl;
	out << "        [ ";
	for (int VeUEId : m_DRASwitchEventIdList)
		out << VeUEId << " , ";
	out << "] " << endl;
	out << "    }" << endl;

	out << "}\n\n" << endl;
}




void cSystem::writeEventListInfo(ofstream &out) {
	for (int i = 0; i < m_NTTI; i++) {
		out << "[ TTI = " << left << setw(3) << i << " ]" << endl;
		out << "{" << endl;
		for (int eventId : m_EventTTIList[i]) {
			sEvent& e = m_EventVec[eventId];
			out << "    " << e.toString() << endl;
		}
		out << "}\n\n" << endl;
	}
}


void cSystem::writeEventLogInfo(std::ofstream &out) {
	for (int eventId = 0;eventId < static_cast<int>(m_EventVec.size());eventId++) {
		out << "Event[" << left << setw(3) << eventId << "]  ";
		out << "VeUE[" << m_EventVec[eventId].VeUEId << "]" << endl;
		out << "{" << endl;
		out << m_EventVec[eventId].toLogString(1);
		out << "}" << endl;
	}
}