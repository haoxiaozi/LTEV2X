#pragma once
#include<math.h>
#include"Global.h"
#include"Enumeration.h"

struct sMessage {

	//-----------------------TEST-----------------------
	void print();
	//-----------------------TEST-----------------------

	eMessageType messageType;
	int byteNum; //该消息的比特数量
	int DRA_ONTTI;  //在DRA方式下，传输该消息需要占用多少个TTI

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