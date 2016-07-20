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


	/*��ӡ��������λ�ø�����־��Ϣ*/
	writeVeUELocationUpdateLogInfo(g_OutVeUELocationUpdateLogInfo);
}

void cSystem::configure() {//ϵͳ�����������
	m_NTTI = 50;//����TTIʱ��
	m_Config.periodicEventNTTI = 20;
	m_Config.emergencyLamda = 10;
	m_Config.locationUpdateNTTI = 30;

	m_Config.VUENum = 30;
	m_Config.RSUNum = 2;
	m_Config.eNBNum = 1;
}


void cSystem::initialization() {
	srand((unsigned)time(NULL));//iomanip
	m_TTI = 0;

	m_eNBVec = vector<ceNB>(m_Config.eNBNum);
	m_RSUVec = vector<cRSU>(m_Config.RSUNum);
	m_VeUEVec = vector<cVeUE>(m_Config.VUENum);
	m_EventTTIList = vector<list<int>>(m_NTTI);



	//����RSU�ͻ�վλ�ù̶��������RSU������վ���г�ʼ������
	for (int RSUId = 0;RSUId < m_Config.RSUNum;RSUId++) {
		int eNBId = rand() % m_Config.eNBNum;
		m_eNBVec[eNBId].m_RSUIdList.push_back(RSUId);
	}

	//ѡ��DRAģʽ
	m_DRAMode = P123;

	//�����¼�����
	buildEventList();

}

void cSystem::buildEventList() {
	/*��ʱ��˳���¼���ID��ʱ����أ�IDԽС���¼�������ʱ��ԽС�����¼�����*/

	srand((unsigned)time(NULL));//iomanip
	//�������ɸ����������������¼�����ʼʱ��
	vector<list<int>> startTTIVec(m_Config.periodicEventNTTI, list<int>());
	for (int VeUEId = 0; VeUEId < m_Config.VUENum; VeUEId++) {
		int startTTI = rand() % m_Config.periodicEventNTTI;
		startTTIVec[startTTI].push_back(VeUEId);
	}

	srand((unsigned)time(NULL));//iomanip
    //����startTTIVec�������PERIOD�¼��������в�����Ӳ��ɷֲ��Ľ����¼�
	int countEmergency = 0;
	int CTTI = 0;
	while (CTTI < m_NTTI) {
		for (int TTIOffset = 0; TTIOffset < m_Config.periodicEventNTTI; TTIOffset++) {
			list<int>lst = startTTIVec[TTIOffset];
			for (int VeUEId : lst) {
				//----------------WRONG--------------------
				//�������p�����Ǵ������ʣ����ǵ�λ�¼��ڴ����Ĵ���
				//�������Ӳ��ɷֲ��Ľ����¼�
				double p = m_Config.emergencyLamda / m_NTTI;//�����ĸ���
				double r = static_cast<double>(rand()) / RAND_MAX;//����[0-1)�ֲ��������
				if (r < p&&CTTI + TTIOffset < m_NTTI) {//�������С�ڸ���p������Ϊ���¼�����
					sEvent evt = sEvent(VeUEId, CTTI + TTIOffset, EMERGENCY);
					m_EventVec.push_back(evt);
					m_EventTTIList[CTTI + TTIOffset].push_back(evt.eventId);
					countEmergency++;
				}
				//----------------WRONG--------------------

				//�����������¼�
				if (CTTI + TTIOffset < m_NTTI) {
					/*-----------------------ATTENTION-----------------------
					* ����������sEvent�Ķ���Ȼ����ѹ��m_EventVec
					* ����Vector<T>.push_back��ѹ�봫�����ĸ���Ʒ����˻����sEvent�Ŀ������캯��
					* sEventĬ�ϵĿ������캯���ḳֵid��Ա������ǰ�ȫ�ģ�
					*sEvent����Զ��忽�����캯���������ڹ��캯���ĳ�ʼ�����ֿ���id��Ա
					*-----------------------ATTENTION-----------------------*/
					sEvent evt = sEvent(VeUEId, CTTI + TTIOffset, PERIOD);
					m_EventVec.push_back(evt);
					m_EventTTIList[CTTI + TTIOffset].push_back(evt.eventId);
				}	
			}
		}
		CTTI += m_Config.periodicEventNTTI;
	}

	cout << "countEmergency: " << countEmergency << endl;
}

