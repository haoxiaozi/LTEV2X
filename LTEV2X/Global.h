#pragma once
#include<fstream>
#include<list>


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
const int gc_RBNum = 100;   //RB����
const int gc_ByteNumPerRB=100;  //ÿ��RB�ɴ���ı�������
const int gc_DRA_NTTI=25; //���дؽ���һ��DRA��ռ�õ�TTI������(NTTI:Number of TTI)
const int gc_DRAEmergencyTotalPatternNum = 3;//��������¼���Pattern����
const int gc_DRAEmergencyFBNumPerPattern = 1;//ÿ�������¼���Patternռ�õ�FB����
const int gc_DRAPatternTypeNum = 2;//�ǽ����¼���Pattern����������
const int gc_DRAPatternNumPerPatternType[gc_DRAPatternTypeNum]= { 20,5 };//��ȫƵ��ÿ��Pattern�����Ӧ��Pattern����
const int gc_DRA_FBNumPerPatternType[gc_DRAPatternTypeNum]={ 1,5 };//ÿ��Pattern������ռ��FB����
const std::list<int> gc_DRAPatternIdxTable[gc_DRAPatternTypeNum]={ //ÿ��Pattern�����Ӧ��Pattern Idx���б�
	{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19},
	{20,21,22,23,24}
};
const int gc_DRATotalPatternNum = 25;//���з�EmergencyPattern���͵�Pattern�����ܺ�




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

