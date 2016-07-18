#pragma once
#include<fstream>


/*===========================================
*               ��������
* ==========================================*/
extern std::ofstream g_OutDRAScheduleInfo;
extern std::ofstream g_OutClasterPerformInfo;
extern std::ofstream g_OutEventListInfo;
extern std::ofstream g_OutTTILogInfo;
extern std::ofstream g_OutEventLogInfo;
extern std::ofstream g_OutVeUELocationUpdateLogInfo;


/*===========================================
*               ��������
* ==========================================*/


const int gc_RBNum = 100;   //�Ӵ�����
const int gc_ByteNumPerRB=100;  //ÿ��RB�ɴ���ı�������
const int gc_DRA_NTTI=25; //���дؽ���һ��DRA��ռ�õ�TTI������(NTTI:Number of TTI)




/*===========================================
*               ȫ��������
* ==========================================*/
int max(int t1, int t2);




/*===========================================
*        �򵥵ĺ�������Ϊ������ʽ
* ==========================================*/

inline
int max(int t1, int t2) {
	return t1 < t2 ? t2 : t1;
}