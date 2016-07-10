#include<iomanip>
#include<fstream>
#include"System.h"

using namespace std;

extern ofstream g_OutDRAScheduleInfo;

void cSystem::process() {
	srand((unsigned)time(NULL));//iomanip
	g_TTI = abs(rand());//���ѡȡ��ʼTTIʱ��
	int TTINum = 2;//����TTIʱ��
	configure();
	for (int count = 0;count < TTINum;count++) {
		g_OutDRAScheduleInfo << "\n\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=Absolute TTI: " << g_TTI << "  ,  Relative TTI: " << count << "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=" << endl;
		DRASchedule();
		g_TTI++;
	}
}