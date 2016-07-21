#pragma once
#include<fstream>
#include<vector>
#include"Utility.h"

/*===========================================
*            �����־�ļ�������
* ==========================================*/
extern std::ofstream g_OutDRAScheduleInfo;
extern std::ofstream g_OutClasterPerformInfo;
extern std::ofstream g_OutEventListInfo;
extern std::ofstream g_OutTTILogInfo;
extern std::ofstream g_OutEventLogInfo;
extern std::ofstream g_OutVeUELocationUpdateLogInfo;


/*===========================================
*               RSU����ĳ���
* ������ΪRSU�ľ�̬������ͳһд�����﷽�����
* ==========================================*/
const int gc_DRATotalBandwidth = 20 * 1000 * 1000;//20MHz
const int gc_BandwidthOfRB = 12 * 1000 * 15;//180kHZ
const int gc_RBNum = gc_DRATotalBandwidth/ gc_BandwidthOfRB;   //RB����(111)
const int gc_BitNumPerRB=1800;  //��λ(��),����RB����Ϊ180kHz��TTIΪ10ms����˵�λTTI��λRB����ı�����Ϊ180k*10ms=1800
const int gc_DRA_NTTI=25; //���дؽ���һ��DRA��ռ�õ�TTI������(NTTI:Number of TTI)
const int gc_DRAEmergencyTotalPatternNum = 6;//��������¼���Pattern����
const int gc_DRAEmergencyFBNumPerPattern = 1;//ÿ�������¼���Patternռ�õ�FB����
const int gc_DRAPatternTypeNum = 2;//�ǽ����¼���Pattern����������
const int gc_DRAPatternNumPerPatternType[gc_DRAPatternTypeNum]= { 60,9 };//��ȫƵ��ÿ��Pattern�����Ӧ��Pattern����
const int gc_DRA_FBNumPerPatternType[gc_DRAPatternTypeNum]={ 1,5 };//ÿ��Pattern������ռ��FB����
const std::vector<int> gc_DRAPatternIdxTable[gc_DRAPatternTypeNum]={ //ÿ��Pattern�����Ӧ��Pattern Idx���б�
	makeContinuousSequence(0,59),
	makeContinuousSequence(60,68)
};

inline
int getDRATotalPatternNum() {
	int res = 0;
	for (int num : gc_DRAPatternNumPerPatternType)
		res += num;
	return res;
}
const int gc_DRATotalPatternNum = getDRATotalPatternNum();//���з�EmergencyPattern���͵�Pattern�����ܺ�




/*===========================================
*               ȫ��������
* ==========================================*/



/*===========================================
*                ������������
* ==========================================*/

inline
int max(int t1, int t2) {
	return t1 < t2 ? t2 : t1;
}

