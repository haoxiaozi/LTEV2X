#pragma once
#include<list>
#include"Message.h"

struct sEvent {//��Ϣ��
	/*���ݳ�Ա*/
	int VEId;//�û�ID
	int callSetupTTI;//���з���ʱ��
	sMessage message;

	sEvent() {};
	sEvent(int id, int time, eMessageType messageType) { 
		VEId = id;
		callSetupTTI = time; 
		message = sMessage(messageType);
	};
};



