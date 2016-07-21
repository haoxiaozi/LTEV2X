#pragma once
#include"Enumeration.h"
#include"Definition.h"

struct sConfigure {//ϵͳ�������
	int periodicEventNTTI;//�������¼������ڣ���λTTI��
	double emergencyLamda;//�����¼�������ֵ���ڸ����ķ��������ڵ�����ֵ��

	int locationUpdateNTTI;//����ˢ��λ�õ�����


	//LK
	float ISD;
	float fc;
	int VeUENum;//��������
	int PeUENum;//��������

	int eNBNum;//��վ����
	int RoadNum;//Road����
	int RSUNum;//RSU����
	int* pupr;//user per road array
	unsigned short wxNum;//ueTopo x�� ����
	unsigned short wyNum;//ueTopo y�� ����
	int ueTopoNum;
	float *pueTopo;//һ��Road��ue�������

	float fv;//��������

};


struct seNBConfigure
{
	sConfigure* sys_config;
	//float fAntAngle;
	//unsigned short wAntTxNum;
	//unsigned short wAntRxNum;
	unsigned short wRoadID;
	unsigned short weNBID;
	float fX;
	float fY;
	float fAbsX;
	float fAbsY;
};

struct sRoadConfigure
{
	sConfigure* sys_config;
	unsigned short wRoadID;
	void *peNB;
	unsigned short weNBNum;
	unsigned short weNBOffset;
	void *pLane;
	unsigned short wLaneNum;
	unsigned short wLaneOffset;
};


struct sUEConfigure
{
	unsigned short wRoadID;
	int locationID;
	float fX;
	float fY;
	float fAbsX;
	float fAbsY;
	float fv;
};

struct sRSUConfigure
{
	unsigned short wRSUID;
};

struct sLocation
{
	bool bManhattan;
	eLocationType eType;
	float fDistance; // m
	float fDistance1; // m
	float fDistance2; // m
	float feNBAntH; // m
	float fUEAntH; // m
	float afPosCor[5];

};

struct sAntenna
{
	float fTxAngle; // degree
	float fRxAngle; // degree
	float fMaxAttenu; // dB
	unsigned char byTxAntNum;
	unsigned char byRxAntNum;
	float *pfTxSlantAngle; // degree
	float *pfRxSlantAngle; // degree
	float *pfTxAntSpacing;
	float *pfRxAntSpacing;
	float fAntGain;
};
