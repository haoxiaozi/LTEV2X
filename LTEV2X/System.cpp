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
		if(count%100==0)ChannelGeneration();
		DRASchedule();
		m_TTI++;
	}

	/*��ӡ�¼���־��Ϣ*/
	writeEventLogInfo(g_OutEventLogInfo);


	/*��ӡ��������λ�ø�����־��Ϣ*/
	writeVeUELocationUpdateLogInfo(g_OutVeUELocationUpdateLogInfo);
}

void cSystem::configure() {//ϵͳ�����������
	/*LK*/
	//�����������

	//data.AllocMem(conf.userNum);
	conf.eNBNum = c_eNBNumber;
	conf.RoadNum = c_roadNumber;
	conf.RSUNum = c_RSUNumber;//Ŀǰֻ��ʾUE RSU��
	conf.pupr = new int[conf.RoadNum];
	conf.VeUENum = 0;
	int Lambda = (c_length + c_wide) * 2 * 3.6 / (2.5 * 15);
	srand((unsigned)time(NULL));
	for (unsigned short temp = 0; temp != conf.RoadNum; ++temp)
	{
		int k = 0;
		long double p = 1.0;
		long double l = exp(-Lambda);  /* Ϊ�˾��ȣ��Ŷ���Ϊlong double�ģ�exp(-Lambda)�ǽӽ�0��С��*/
		while (p >= l)
		{
			double u = (float)(rand() % 10000)*0.0001f;
			p *= u;
			k++;
		}
		conf.pupr[temp] = k - 1;
		conf.VeUENum = conf.VeUENum + k - 1;
		//printf("%d\n",k-1);
	}
	conf.wxNum = 36;
	conf.wyNum = 62;
	conf.ueTopoNum = (conf.wxNum + conf.wyNum) * 2 - 4;
	conf.pueTopo = new float[conf.ueTopoNum * 2];//�غ���4��
	float temp_x = -(float)c_wide / 2 + c_lane_wide;
	float temp_y = -(float)c_length / 2 + c_lane_wide;
	for (int temp = 0;temp != conf.ueTopoNum; ++temp)
	{
		if (temp>0 && temp <= 61) {
			if (temp == 60) temp_y += 6; else temp_y += 7;
		}
		else if (61<temp&&temp <= 96) {
			if (temp == 95) temp_x += 5; else temp_x += 7;
		}
		else if (96<temp&&temp <= 157) {
			if (temp == 156) temp_y -= 6; else temp_y -= 7;
		}
		else if (157<temp&&temp <= 192) {
			if (temp == 191) temp_x -= 5; else temp_x -= 7;
		}

		conf.pueTopo[temp * 2 + 0] = temp_x;
		conf.pueTopo[temp * 2 + 1] = temp_y;
		//printf("%f,%f\n",temp_x,temp_y);//֮����������ֱ�Ӷ������ʡʱ��
	}
	conf.fv = 15;//�����趨,km/h
	/*LK*/





	m_NTTI = 200;//����TTIʱ��
	conf.periodicEventNTTI = 100;
	conf.emergencyLamda = 1;
	conf.locationUpdateNTTI = 150;

	//conf.VeUENum = 2000;
	//conf.RSUNum = 36;
	//conf.eNBNum = 4;
}


void cSystem::initialization() {
	srand((unsigned)time(NULL));//iomanip
	m_TTI = 0;

	/*LK*/
	eNB = new ceNB[conf.eNBNum];
	Road = new cRoad[conf.RoadNum];
	veUE = new cVeUE[conf.VeUENum];
	RSU = new cRSU[conf.RSUNum];

	sRoadConfigure roadConfigure;
	for (unsigned short temp = 0;temp != conf.RoadNum; ++temp)
	{
		roadConfigure.wRoadID = temp;
		if (temp % 2 == 0)
		{
			roadConfigure.weNBNum = 1;
			roadConfigure.peNB = eNB;
			roadConfigure.weNBOffset = temp / 2;
		}
		else
		{
			roadConfigure.weNBNum = 0;
		}

		Road[temp].Initialize(roadConfigure);
	}

	//       FILE *fp1;//����һ���ļ�����ָ��
	//        fp1=fopen("eNB.txt","w+");//��׷�ӵķ�ʽ�������1.txt��Ĭ��λ����������Ŀ¼����
	//        for(int eNBIdx=0;eNBIdx!=c_eNBNumber;++eNBIdx)
	//	      fprintf(fp1,"%f\n%f
	sRSUConfigure RSUConfigure;
	for (unsigned short RSUIdx = 0;RSUIdx != conf.RSUNum;RSUIdx++)
	{

		RSUConfigure.wRSUID = RSUIdx;
		RSU[RSUIdx].Initialize(RSUConfigure);
	}
	//FILE *fp;//����һ���ļ�����ָ��
	//      fp=fopen("RSU.txt","w+");//��׷�ӵķ�ʽ�������1.txt��Ĭ��λ����������Ŀ¼����
	//      for(unsigned short RSUIdx=0;RSUIdx!=conf.RSUNum;RSUIdx++)
	//fprintf(fp,"%f\n%f\n",RSU[RSUIdx].m_fAbsX,RSU[RSUIdx].m_fAbsY);
	//fclose(fp);//�ر���

	sUEConfigure ueConfigure;
	int ueidx = 0;

	//      FILE *fp;//����һ���ļ�����ָ��
	//      fp=fopen("ue.txt","w+");//��׷�ӵķ�ʽ�������1.txt��Ĭ��λ����������Ŀ¼����

	for (unsigned short RoadIdx = 0;RoadIdx != conf.RoadNum;RoadIdx++)
	{

		for (int uprIdx = 0;uprIdx != conf.pupr[RoadIdx];uprIdx++)
		{
			ueConfigure.wRoadID = RoadIdx;
			ueConfigure.locationID = rand() % conf.ueTopoNum;
			ueConfigure.fX = conf.pueTopo[ueConfigure.locationID * 2 + 0];
			ueConfigure.fY = conf.pueTopo[ueConfigure.locationID * 2 + 1];
			ueConfigure.fAbsX = Road[RoadIdx].m_fAbsX + ueConfigure.fX;
			ueConfigure.fAbsY = Road[RoadIdx].m_fAbsY + ueConfigure.fY;
			ueConfigure.fv = conf.fv;
			//fprintf(fp,"%f\n%f\n",ueConfigure.fAbsX,ueConfigure.fAbsY);
			veUE[ueidx++].Initialize(ueConfigure);

		}
	}
	//fclose(fp);//�ر���

	/*LK*/



	/*eNB = new ceNB[conf.eNBNum];
	RSU = new cRSU[conf.RSUNum];
	veUE = new cVeUE[conf.VeUENum];*/
	
	
	
	
	m_EventTTIList = vector<list<int>>(m_NTTI);



	//����RSU�ͻ�վλ�ù̶��������RSU������վ���г�ʼ������
	for (int RSUId = 0;RSUId < conf.RSUNum;RSUId++) {
		int eNBId = rand() % conf.eNBNum;
		eNB[eNBId].m_RSUIdList.push_back(RSUId);
	}

	//ѡ��DRAģʽ
	m_DRAMode = P123;

	//�����¼�����
	buildEventList();

}

void cSystem::buildEventList() {
	/*��ʱ��˳���¼���Id��ʱ����أ�IdԽС���¼�������ʱ��ԽС�����¼�����*/

	srand((unsigned)time(NULL));//iomanip
	//�������ɸ����������������¼�����ʼʱ��
	vector<list<int>> startTTIVec(conf.periodicEventNTTI, list<int>());
	for (int VeUEId = 0; VeUEId < conf.VeUENum; VeUEId++) {
		int startTTI = rand() % conf.periodicEventNTTI;
		startTTIVec[startTTI].push_back(VeUEId);
	}

	srand((unsigned)time(NULL));//iomanip
    //����startTTIVec�������PERIOD�¼��������в�����Ӳ��ɷֲ��Ľ����¼�
	int countEmergency = 0;
	int CTTI = 0;
	while (CTTI < m_NTTI) {
		for (int TTIOffset = 0; TTIOffset < conf.periodicEventNTTI; TTIOffset++) {
			list<int>lst = startTTIVec[TTIOffset];
			for (int VeUEId : lst) {
				//----------------WRONG--------------------
				//�������p�����Ǵ������ʣ����ǵ�λ�¼��ڴ����Ĵ���
				//�������Ӳ��ɷֲ��Ľ����¼�
				double p = conf.emergencyLamda / m_NTTI;//�����ĸ���
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
		CTTI += conf.periodicEventNTTI;
	}

	cout << "countEmergency: " << countEmergency << endl;
}

