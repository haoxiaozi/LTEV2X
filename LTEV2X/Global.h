#pragma once
#include<fstream>

//������������VUE.h RSU.h eNB.h������������δ�����Ĵ���

/*===========================================
*               ��������
* ==========================================*/
extern std::ofstream g_OutDRAScheduleInfo;
extern std::ofstream g_OutDRAProcessInfo;
extern std::ofstream g_OutClasterPerformInfo;
extern std::ofstream g_OutEventListInfo;
extern std::ofstream g_OutLogInfo;


/*===========================================
*               ��������
* ==========================================*/
const int gc_DRA_FBNum = 10;
const int gc_RBNum = 100;   //�Ӵ�����
const int gc_ByteNumPerRB=100;  //ÿ��RB�ɴ���ı�������
const int gc_DRA_NTTI=25; //���дؽ���һ��DRA��ռ�õ�TTI������(NTTI:Number of TTI)







/*===========================================
*               ��������
* ==========================================*/

int max(int t1, int t2);
