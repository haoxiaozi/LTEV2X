#include<iomanip>
#include<fstream>
#include"System.h"
#include"Global.h"

using namespace std;

void cSystem::process() {

	/*��������*/
	configure();

	/*�����ʼ��*/
	initialization();

	writeEventListInfo(g_OutEventListInfo);

	for (int count = 0;count < m_NTTI;count++) {
		DRASchedule();
		m_TTI++;
	}
}


void cSystem::writeClusterPerformInfo(ofstream &out) {
	out << "ATTI = " << left << setw(6) << m_TTI << "RTTI = " << left << setw(6) << m_TTI - m_STTI << endl;
	out << "{" << endl;
	//��ӡ������Ϣ
	out << "    VUE Info: " << endl;
	out << "    {" << endl;
	for (cVeUE &_VeUE : m_VecVUE) 
		out << "        " << _VeUE.toString() << endl;
	out << "    }" << endl;

	out << "\n\n\n";

	//��ӡ��վ��Ϣ
	out << "    eNB Info: " << endl;
	out << "    {" << endl;
	for (ceNB &_eNB : m_VeceNB)
		out << "        " << _eNB.toString() << endl;
	out << "    }" << endl;

	//��ӡRSU��Ϣ
	out << "    RSU Info: " << endl;
	out << "    {" << endl;
	for (cRSU &_RSU : m_VecRSU)
		out << "        " << _RSU.toString() << endl;
	out << "    }" << endl;

	out << "\n\n\n";

	//��ӡSystem��Switch����
	out << "    RSUSwitchList Info: " << endl;
	out << "    {" << endl;
	out << "        [ ";
	for (int VEId : m_DRA_RSUSwitchSet)
		out << VEId << " , ";
	out << "] " << endl;
	out << "    }" << endl;

	out << "}\n\n" << endl;
}




void cSystem::writeEventListInfo(ofstream &out) {
	//��ӡ�¼�������Ϣ
	for (int i = 0;i < m_NTTI;i++) {
		out << "ATTI = " << left << setw(6) << m_TTI << "RTTI = " << left << setw(6) << m_TTI - m_STTI << endl;
		out << "{" << endl;
		for (sEvent &e : m_EventList[i])
			out << "    [ VEId = " << left << setw(5) << e.VEId << "callSetupTTI = " << left << setw(8) << e.callSetupTTI << "eMessageType = " << left << setw(12) << (e.message.messageType == PERIOD ? "PEROID" : "ELSE") << " ] " << endl;
		out << "}\n\n" << endl;
	}
}