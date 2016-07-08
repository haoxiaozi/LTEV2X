#pragma once



enum eLinkType {//��·����
	DOWNLINK,    //������·
	UPLINK      //������·
};


enum eScheduleMode {

};


enum eMessageType {
	PERIOD,  //��������Ϣ�����������Ļ�����Ϣ�����٣�λ�ã�����ȵȣ�
	EMERGENCY,   //������Ϣ�����糵����Ϣ�����ȼ��ϸߣ�
	DATA     //����ҵ��(�ı�����Ƶ�������ȵȣ�
};


enum eRSUType {
	INTERSECTION,       //����ʮ��·�����룬��������
	VERTICAL,           //������ֱ�ĵ�·��
	HORIZONTAL          //����ˮƽ�ĵ�·��    
};

enum eDRAMode {
	//P1:  Collision avoidance based on sensing
	//P2:  Enhanced random resource selection
	//P3:  Location-based resource selection
	P13,               //Combination of P1 and P3
	P23,               //Combination of P2 and P3
	P123               //Combination of P1 and P2 and P3
};


