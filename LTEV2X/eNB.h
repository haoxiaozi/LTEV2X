#pragma once
#include<vector>
#include<string>
#include"Global.h"
#include"Schedule.h"


//class ceNB {
//	/*LK*/
//public:
//	ceNB(void);
//	~ceNB(void);
//	void Initialize(seNBConfigure &t_eNBConfigure);
//	void Destroy(void);
//public:
//	//unsigned short m_wCellID;
//	//unsigned short m_wBSID;
//	unsigned short m_wRoadID;
//	unsigned short m_eNBId;
//	float m_fX;
//	float m_fY;
//	float m_fAbsX;
//	float m_fAbsY;
//	//cSector *m_pSector;
//	//unsigned char m_wSecNum;
//	//eBSType m_BSType;
//	//unsigned char m_wAntNum;
//	/*LK*/
//
//
//
//
//
//
//	/*  TEST  */
//	static int s_eNBCount;
//	/*  TEST  */
//public:
//	//const int m_eNBId=s_eNBCount++;//��վId
//
//	std::list<int> m_RSUIdList;//�û�վ�е�RSU�������洢RSU��Id��
//	std::list<int> m_VeUEIdList;//�û�վ�е�VeUE�������洢VeUE��Id��
//
//	/***************************************************************
//	------------------------����ģ��--------------------------------
//	****************************************************************/
//	bool m_UnassignedSubband[gc_RBNum];//������Դ�飬���Ϊtrueָʾδ�������RB
//	std::vector<sScheduleInfo> m_vecScheduleInfo;//������Ϣ
//
//	std::string toString(int n);//���ڴ�ӡ��վ��Ϣ
//};