#pragma once
#include<list>
#include<string>
#include"Enumeration.h"

struct sMessage {//��Ϣ��
	/*���ݳ�Ա*/
	eMessageType messageType;//����Ϣ������
	int byteNum; //����Ϣ�ı�������
	int DRA_ONTTIPerFB;  //��DRA��ʽ�£��õ�λFB�������Ϣ��Ҫռ�ö��ٸ�TTI

	/*���캯��*/
	sMessage() {}
	sMessage(eMessageType messageType);

	/*���ܺ���*/
	std::string toString();
};

struct sEvent {//�¼���
public:
	static int s_EventCount;

	/*
	* �¼�Id
	* ÿ���¼�����Ψһ��Id��Id��0��ʼ���
	*/
	const int eventId = s_EventCount++;

	/*
	* �û�Id
	* ���¼���Ӧ���û���Id
	*/
	int VeUEId;//
	int TTI;//�¼�������TTIʱ��

	/*
	* ����ʱ��
	* ��һ�����شӷ��ͽڵ㵽���սڵ��ڴ�����·�Ͼ�����ʱ��
	* <<��������ģ������Ƿ�����ʵ��õ�ʱ�ӣ�)>>
	*/
	int propagationDelay;

	/*
	* ����ʱ��
	* ���ͽڵ��ڴ�����·�Ͽ�ʼ���͵�һ��������������������һ����������Ҫ��ʱ��
	*/
	int sendDelay;

	/*
	* ����ʱ��
	* ��һ���ڵ������˵���ýڵ���������������ʱ��
	* <<�Һ��������������>>
	* <<��ʱ���Ϊ�����ŵ���ռ�õ�ʱ���>>
	*/
	int processingDelay;


	/*
	* �Ŷ�ʱ��
	* �������ǰ�ȴ���ʱ��
	*/
	int queuingDelay;


private :
	std::list<std::string> logTrackList;//��¼���¼���������־
public:
	sMessage message;

	/*���캯��*/
	sEvent():propagationDelay(0), sendDelay(0), processingDelay(0), queuingDelay(0){}
	sEvent(int VeUEId, int TTI, eMessageType messageType);

	/*���ܺ���*/
	std::string toString();//���string���͵��¼���Ϣ
	std::string toLogString(int n);
	void addEventLog(int TTI, int type, int RSUId, int clusterIdx, int patternIdx);//ѹ���µ���־
};





