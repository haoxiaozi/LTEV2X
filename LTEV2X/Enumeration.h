#pragma once


enum eLocationType { //位置类型 
	Los, 
	Nlos, 
	None 
}; 


enum eScheduleMode {

};


enum eMessageType {
	EMERGENCY = -1,   //紧急信息（比如车祸信息，优先级较高）
	PERIOD = 0,  //周期性信息（包括VeUE的基本信息，车速，位置，方向等等）
	DATA = 1     //数据业务(文本，视频，声音等等）
};


enum eRSUType {
	INTERSECTION,       //处于十字路口中央，城镇特有
	VERTICAL,           //处于竖直的道路中
	HORIZONTAL          //处于水平的道路中    
};

enum eDRAMode {
	//P1:  Collision avoidance based on sensing
	//P2:  Enhanced random resource selection
	//P3:  Location-based resource selection
	P13,               //Combination of P1 and P3
	P23,               //Combination of P2 and P3
	P123               //Combination of P1 and P2 and P3
};


