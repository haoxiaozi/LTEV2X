#pragma once
#include"Enumeration.h"

struct sConfig {//ϵͳ�������
public:
	int userNum;//��������
	int sectorNum;//��������
	eLinkType linkType;//��·����
	unsigned int antennaNumOfeNB;//��վ������


	int HNum;//H����
	int HNumPereNB;//ÿRB��H����
	eTransMode transType;
	int subbandNum;//�Ӵ�����
};