#pragma once
#include<list>

enum EventType {//�¼�����
	EVENT_CALL_SETUP,
	EVENT_CALL_END,
	EVENT_CAC
};

class Message {//��Ϣ��
public:
	/*���ݳ�Ա*/
	int m_UserId;//�û�ID
	int m_ResponseTime;//��Ϣ��Ӧ��ʱ��

	Message() {};
	Message(int id, int time) { m_UserId = id; m_ResponseTime = time; };
	bool operator ==(const Message& other) { return m_UserId == other.m_UserId&&m_ResponseTime == other.m_ResponseTime; }
};

class EventList :public std::list<Message> {//��Ϣ�¼�����
public:
	void Insert(const Message&m) { push_back(m); }
};

