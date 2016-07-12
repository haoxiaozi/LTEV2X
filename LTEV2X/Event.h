#pragma once
#include<list>
#include<string>
#include"Message.h"

struct sEvent {//�¼���
	static int count;
	/*���ݳ�Ա*/
	const int eventId = count++;//�¼�ID
	int VeUEId;//�û�ID
	int ATTI;//�¼���������TTI
	int RTTI;//�¼��������TTI
	sMessage message;

	sEvent(){}

	sEvent(int VeUEId, int ATTI, int RTTI,eMessageType messageType) { 
		this->VeUEId = VeUEId;
		this->ATTI = ATTI; 
		this->RTTI = RTTI;
		message = sMessage(messageType);
	};

	std::string toString();
};



