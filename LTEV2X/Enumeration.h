#pragma once



enum eLinkType {//��·����
	DOWNLINK,    //������·
	UPLINK      //������·
};

enum eTransMode {//����ģʽ
	INVALID_TRANSMODE,  //��Ч
	TM_1,        //???
	TM_2,        //???
	CODEBOOK,    //�����뱾
	BEAMFORMING  //����Beamforming
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


//****************������������****************//

enum eModulationType {//���Ʒ�ʽ
	BPSK = 1,
	QPSK = 2,
	_16QAM = 4,
	_64QAM = 6
};










struct sMCS {//���Ʊ��뷽ʽ�ȼ�
	eModulationType modulationType;//���Ʒ�ʽ
	float rate;//����
};


struct sMacPacket {
	int size;//MAC���Ĵ�С
	//double size_no_rate_loss;
	//double size_non_fullbuffer;//~~~
	int createTime;//MAC���Ĳ���ʱ��
	int receiveTime;//MAC���ɹ�����ʱ��
	int reTransmitionNum;//MAC���ش�����
};