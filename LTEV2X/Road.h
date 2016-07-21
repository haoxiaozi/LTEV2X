#pragma once

#include "Constant.h"
#include "Definition.h"
#include "Config.h"
#include "Global.h"
#include "Schedule.h"
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <sstream>

using std::ifstream;
using std::cerr;
using std::endl;
using std::string;
using std::stringstream;

class ceNB
{
public:
	ceNB(void);
	~ceNB(void);
	void Initialize(seNBConfigure &t_eNBConfigure);
	void Destroy(void);
public:
	//unsigned short m_wCellID;
	//unsigned short m_wBSID;
	unsigned short m_wRoadID;
	unsigned short m_eNBId;
	float m_fX;
	float m_fY;
	float m_fAbsX;
	float m_fAbsY;
	//cSector *m_pSector;
	//unsigned char m_wSecNum;
	//eBSType m_BSType;
	//unsigned char m_wAntNum;



	/*HCF*/
	///*  TEST  */
	//static int s_eNBCount;
	///*  TEST  */
public:
	//const int m_eNBId=s_eNBCount++;//��վId

	std::list<int> m_RSUIdList;//�û�վ�е�RSU�������洢RSU��Id��
	std::list<int> m_VeUEIdList;//�û�վ�е�VeUE�������洢VeUE��Id��

								/***************************************************************
								------------------------����ģ��--------------------------------
								****************************************************************/
	bool m_UnassignedSubband[gc_RBNum];//������Դ�飬���Ϊtrueָʾδ�������RB
	std::vector<sScheduleInfo> m_vecScheduleInfo;//������Ϣ

	std::string toString(int n);//���ڴ�ӡ��վ��Ϣ
};

class cRoad
{
public:
	 cRoad(void);
	~cRoad(void);
	void Initialize(sRoadConfigure &t_RoadConfigure);
	void Destroy(void);
public:
	unsigned short m_wRoadID;
    unsigned short m_weNBNum;
	unsigned short m_weNBID;
	ceNB *m_peNB;
    float m_fAbsX;
	float m_fAbsY;
	int  m_upr;
};