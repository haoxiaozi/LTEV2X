#pragma once
#include<list>

enum eEventType {//�¼�����
	EVENT_CALL_SETUP,
	EVENT_CALL_END,
	EVENT_CAC
};

struct sEvent {//��Ϣ��
	/*���ݳ�Ա*/
	int m_UserId;//�û�ID
	int m_ResponseTime;//��Ϣ��Ӧ��ʱ��

	sEvent() {};
	sEvent(int id, int time) { m_UserId = id; m_ResponseTime = time; };
	bool operator ==(const sEvent& other) { return m_UserId == other.m_UserId&&m_ResponseTime == other.m_ResponseTime; }
};

class EventList :public std::list<sEvent> {//��Ϣ�¼�����
public:
	void Insert(const sEvent&m) { push_back(m); }
};

