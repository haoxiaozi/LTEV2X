#pragma once
#include<vector>
#include"RSU.h"

class ceNB {
public:
	int m_eNBId;//基站ID

	std::vector<int> m_VecRSU;//该基站中的RSU容器（存储RSU的ID）
	std::vector<int> m_VecVeUE;

	/***************************************************************
	------------------------调度模块--------------------------------
	****************************************************************/
	bool m_UnassignedSubband[gc_RBNum];//所有资源块，标记为true指示未被分配的RB
	std::vector<sScheduleInfo> m_vecScheduleInfo;//调度信息
};