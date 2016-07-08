#pragma once
#include<math.h>
#include"Global.h"
#include"Enumeration.h"

struct sMessage {
	eMessageType messageType;
	int byteNum; //����Ϣ�ı�������
	int DRA_ONTI;  //��DRA��ʽ�£��������Ϣ��Ҫռ�ö��ٸ�DRA_MTI

	void setMessageType(eMessageType messageType) {
		this->messageType = messageType;
		switch (messageType) {
		case PERIOD:
			byteNum = 200;
			break;
		case EMERGENCY:
			byteNum = 100;
			break;
		case DATA:
			byteNum = 1000;
			break;
		}
		DRA_ONTI = std::ceil(static_cast<double>(byteNum) / static_cast<double>(gc_ByteNumPerRB));
	}
};