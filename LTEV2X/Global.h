#pragma once
#include<fstream>
#include<vector>


/*===========================================
*               全域函数声明
* ==========================================*/
//产生特定分布随机数
void RandomUniform(float *t_pfArray, unsigned long t_ulNumber, float t_fUpBound, float t_fDownBound, bool t_bFlagZero);
void RandomGaussian(float *t_pfArray, unsigned long t_ulNumber, float t_fMean, float t_fStandardDeviation);
void SortBubble(float *t_pfArray, unsigned short t_wNumber, bool t_bFlagDirection, bool t_bFlagFabs);
void SelectMax(float *t_pfArray, unsigned char t_byNumber, unsigned char *t_pbyFirst, unsigned char *t_pbySecond);

//辅助函数
std::vector<int> makeContinuousSequence(int begin, int end);



/*===========================================
*             地理拓扑单元常量
* ==========================================*/
const unsigned short c_eNBNumber = 7;
const unsigned short c_roadNumber = 14;
const unsigned short c_RSUNumber = 78;
const int c_wide = 250;
const int c_length = 433;
const float c_lane_wide = 3.5;


//数值常量设置
const float c_PI = 3.1415926535897932384626433832795f;
const float c_PINeg = -3.1415926535897932384626433832795f;
const float c_PI2 = 6.283185307179586476925286766559f;
const float c_PIHalf = 1.5707963267948966192313216916398f;
const float c_Degree2PI = 0.01745329251994329576923690768489f;
const float c_SqrtHalf = 0.70710678118654752440084436210485f;
const float c_SqrtThree = 1.73205080756887729f;
const float c_C = 299792458.0f;

const float c_roadTopoRatio[c_roadNumber * 2] =
{
	-1.5f, 1.0f,
	-0.5f, 1.0f,
	0.5f, 1.0f,
	1.5f, 1.0f,
	-2.5f, 0.0f,
	-1.5f, 0.0f,
	-0.5f, 0.0f,
	0.5f, 0.0f,
	1.5f, 0.0f,
	2.5f, 0.0f,
	-1.5f,-1.0f,
	-0.5f,-1.0f,
	0.5f,-1.0f,
	1.5f,-1.0f
};
const unsigned short c_WrapAroundRoad[c_roadNumber][9] =
{
	{ 0,1,6,5,4,13,8,9,10 },
	{ 1,2,7,6,5,0,9,10,11 },
	{ 2,3,8,7,6,1,10,11,12 },
	{ 3,4,9,8,7,2,11,12,13 },
	{ 4,5,10,9,8,3,12,13,0 },
	{ 5,6,11,10,9,4,13,0,1 },
	{ 6,7,12,11,10,5,0,1,2 },
	{ 7,8,13,12,11,6,1,2,3 },
	{ 8,9,0,13,12,7,2,3,4 },
	{ 9,10,1,0,13,8,3,4,5 },
	{ 10,11,2,1,0,9,4,5,6 },
	{ 11,12,3,2,1,10,5,6,7 },
	{ 12,13,4,3,2,11,6,7,8 },
	{ 13,0,5,4,3,12,7,8,9 }

};

const float c_RSUTopoRatio[c_RSUNumber * 2] =
{
	-2.0f, 1.5f,
	-1.5f, 1.5f,
	-1.0f, 1.5f,
	-0.5f, 1.5f,
	0.0f, 1.5f,
	0.5f, 1.5f,
	1.0f, 1.5f,
	1.5f, 1.5f,
	2.0f, 1.5f,
	-2.0f, 1.2f,
	-1.0f, 1.2f,
	0.0f, 1.2f,
	1.0f, 1.2f,
	2.0f, 1.2f,
	-2.0f, 0.8f,
	-1.0f, 0.8f,
	0.0f, 0.8f,
	1.0f, 0.8f,
	2.0f, 0.8f,
	-3.0f, 0.5f,
	-2.5f, 0.5f,
	-2.0f, 0.5f,
	-1.5f, 0.5f,
	-1.0f, 0.5f,
	-0.5f, 0.5f,
	0.0f, 0.5f,
	0.5f, 0.5f,
	1.0f, 0.5f,
	1.5f, 0.5f,
	2.0f, 0.5f,
	2.5f, 0.5f,
	3.0f, 0.5f,
	-3.0f, 0.2f,
	-2.0f, 0.2f,
	-1.0f, 0.2f,
	0.0f, 0.2f,
	1.0f, 0.2f,
	2.0f, 0.2f,
	3.0f, 0.2f,
	-3.0f,-0.2f,
	-2.0f,-0.2f,
	-1.0f,-0.2f,
	0.0f,-0.2f,
	1.0f,-0.2f,
	2.0f,-0.2f,
	3.0f,-0.2f,
	-3.0f,-0.5f,
	-2.5f,-0.5f,
	-2.0f,-0.5f,
	-1.5f,-0.5f,
	-1.0f,-0.5f,
	-0.5f,-0.5f,
	0.0f,-0.5f,
	0.5f,-0.5f,
	1.0f,-0.5f,
	1.5f,-0.5f,
	2.0f,-0.5f,
	2.5f,-0.5f,
	3.0f,-0.5f,
	-2.0f,-0.8f,
	-1.0f,-0.8f,
	0.0f,-0.8f,
	1.0f,-0.8f,
	2.0f,-0.8f,
	-2.0f,-1.2f,
	-1.0f,-1.2f,
	0.0f,-1.2f,
	1.0f,-1.2f,
	2.0f,-1.2f,
	-2.0f,-1.5f,
	-1.5f,-1.5f,
	-1.0f,-1.5f,
	-0.5f,-1.5f,
	0.0f,-1.5f,
	0.5f,-1.5f,
	1.0f,-1.5f,
	1.5f,-1.5f,
	2.0f,-1.5f,
};

const unsigned short c_RSUInRoad[c_roadNumber][10] =
{
	{ 0,1,2,10,15,23,22,21,14,9 },
	{ 2,3,4,11,16,25,24,23,15,10 },
	{ 4,5,6,12,17,27,26,25,16,11 },
	{ 6,7,8,13,18,29,28,27,17,12 },
	{ 19,20,21,33,40,48,47,46,39,32 },
	{ 21,22,23,34,41,50,49,48,40,33 },
	{ 23,24,25,35,42,52,51,50,41,34 },
	{ 25,26,27,36,43,54,53,52,42,35 },
	{ 27,28,29,37,44,56,55,54,43,36 },
	{ 29,30,31,38,45,58,57,56,44,37 },
	{ 48,49,50,60,65,71,70,69,64,59 },
	{ 50,51,52,61,66,73,72,71,65,60 },
	{ 52,53,54,62,67,75,74,73,66,61 },
	{ 54,55,56,67,68,77,76,75,67,62 }
};

const float c_FC = 2e9f;





/*===========================================
*            输出日志文件流声明
* ==========================================*/
extern std::ofstream g_OutDRAScheduleInfo;
extern std::ofstream g_OutClasterPerformInfo;
extern std::ofstream g_OutEventListInfo;
extern std::ofstream g_OutTTILogInfo;
extern std::ofstream g_OutEventLogInfo;
extern std::ofstream g_OutVeUELocationUpdateLogInfo;


/*===========================================
*          无线资源管理单元常量
* ==========================================*/
const int gc_DRATotalBandwidth = 20 * 1000 * 1000;//20MHz
const int gc_BandwidthOfRB = 12 * 1000 * 15;//180kHZ
const int gc_RBNum = gc_DRATotalBandwidth/ gc_BandwidthOfRB;   //RB数量(111)
const int gc_BitNumPerRB=1800;  //单位(个),由于RB带宽为180kHz，TTI为10ms，因此单位TTI单位RB传输的比特数为180k*10ms=1800
const int gc_DRA_NTTI=25; //所有簇进行一次DRA所占用的TTI数量。(NTTI:Number of TTI)
const int gc_DRAEmergencyTotalPatternNum = 6;//传输紧急事件的Pattern数量
const int gc_DRAEmergencyFBNumPerPattern = 1;//每个紧急事件的Pattern占用的FB数量
const int gc_DRAPatternTypeNum = 2;//非紧急事件的Pattern的类型种类
const int gc_DRAPatternNumPerPatternType[gc_DRAPatternTypeNum]= { 60,9 };//在全频段每个Pattern种类对应的Pattern数量
const int gc_DRA_FBNumPerPatternType[gc_DRAPatternTypeNum]={ 1,5 };//每个Pattern种类所占的FB数量
const std::vector<int> gc_DRAPatternIdxTable[gc_DRAPatternTypeNum]={ //每个Pattern种类对应的Pattern Idx的列表
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
const int gc_DRATotalPatternNum = getDRATotalPatternNum();//所有非EmergencyPattern类型的Pattern数量总和







/*===========================================
*                内敛函数定义
* ==========================================*/

inline
int max(int t1, int t2) {
	return t1 < t2 ? t2 : t1;
}

