#pragma once

#include"Definition.h"
#include"Numerical.h"
#include"Enumeration.h"


/*===========================================
*               ��������
* ==========================================*/
const int gc_MaxMU_MIMOUserNum = 2; //MU-MIMOʱ��ͬʱƵ��Դ����û�������������
const int gc_MaxCodewords = 2;
const int gc_SubbandNum = 100;   //�Ӵ�����
const int gc_MaxCoMP = 3;
const bool gc_ABSConfigure[10] = { 0,1,0,1,0,1,0,1,0,1 };
const double gc_PI = 3.1415926535897932384626433832795;



/*===========================================
*               ���㺯��
* ==========================================*/
double positiveNumber_2_dB(double x);//����תΪdB
double g_dB2Real(double x);//dBתΪ����


/*===========================================
*               �����
* ==========================================*/
int gt_H2Subband(int _H_Idx);
int gt_LUTCodewords2LayersNum(eTransMode mod, int layers, int codewords, int codewordsIdx);
int gt_LUTLayer2Codeword(eTransMode mod, int layers, int codewords, int layerIdx);
int gt_LUTSearchMCSIndex(eLinkType link, double sinr, double target_bler);
double gt_LUTMCSIndex2Effective(int McsIndex, eLinkType link);
double gt_LUTSNR2BLER(double snr, int mcs_level, eLinkType link);
void gt_LUTBlerPara(int mcs_level, eLinkType link, double* bmcs, double* cmcs);




/*===========================================
*               �뱾ѡ��
* ==========================================*/
class Codebook
{
private:
	Matrix precoding_2[2][4];//2Tx�뱾
	Matrix precoding_4[4][16];//4Tx�뱾
	Matrix _W[8][256];//8Tx�뱾
	Matrix w[16];
	Matrix u[16];
	int pmis_2[2];//2Tx�뱾��������Ӧÿ�㣩
	int pmis_4[4];//4Tx�뱾��������Ӧÿ�㣩
	int pmis_8[8];//8Tx�뱾��������Ӧÿ�㣩
	float getprecoding_SINR(Matrix &H, Matrix F_c, int PMI);
public:
	Codebook();
	~Codebook() {}
	//void choose(matrix& H,int nTx,int *PMI, int* RI);

	int choose_PMI(Matrix& H, int nTx, int  RI);
	int choose_RI(Matrix& H);
	Matrix getPrecodingMatrix(int nTx, int PMI, int RI);
	int pmis(int nTx, int ri);
};


class SubbandInfo {
public:
	unsigned int begin;
	unsigned int end;
	unsigned int size;
};









