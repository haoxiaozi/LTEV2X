#pragma once
#include<math.h>
#include"Global.h"
#include"Enumeration.h"

struct sMessage {

	//-----------------------TEST-----------------------
	void print();
	//-----------------------TEST-----------------------

	eMessageType messageType;
	int byteNum; //����Ϣ�ı�������
	int DRA_ONTTI;  //��DRA��ʽ�£��������Ϣ��Ҫռ�ö��ٸ�TTI

	void setMessageType(eMessageType messageType) {
		this->messageType = messageType;
		switch (messageType) {
		case PERIOD:
			byteNum = 300;
			break;
		case EMERGENCY:
			byteNum = 200;
			break;
		case DATA:
			byteNum = 1000;
			break;
		}
		DRA_ONTTI = std::ceil(static_cast<double>(byteNum) / static_cast<double>(gc_ByteNumPerRB));
	}
};