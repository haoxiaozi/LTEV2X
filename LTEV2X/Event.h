#pragma once
#include<list>
#include<string>
#include"Message.h"

struct sMessage {//��Ϣ��
	/*���ݳ�Ա*/
	eMessageType messageType;//����Ϣ������
	int byteNum; //����Ϣ�ı�������
	int DRA_ONTTI;  //��DRA��ʽ�£��������Ϣ��Ҫռ�ö��ٸ�TTI

	/*���캯��*/
	sMessage() {}
	sMessage(eMessageType messageType);

	/*���ܺ���*/
	std::string toString();
};

struct sEvent {//�¼���
public:
	static int s_EventCount;

	/*���ݳ�Ա*/
	const int eventId = s_EventCount++;//�¼�ID
	int VeUEId;//�û�ID
	int TTI;//�¼�������TTIʱ��

private :
	std::list<std::string> logTrackList;//��¼���¼���������־
public:
	sMessage message;

	/*���캯��*/
	sEvent(){}
	sEvent(int VeUEId, int TTI, eMessageType messageType);

	/*���ܺ���*/
	std::string toString();//���string���͵��¼���Ϣ
	std::string toLogString(int n);
	void addLog(std::string s);//ѹ���µ���־
};





