#include <cmath>
#include <memory.h>
#include "IMTA.h"
#include "Global.h"


const float cIMTA::m_sacfConstantInHLoS[25] =
{
	0.621223718347157f, 0.327023628702021f, 0.498450068093346f, -0.397383499536540f, -0.317443730363866f,
	0.327023628702021f, 0.923620836374881f, 0.115165693298854f, -0.148559279659243f, 0.068169644262314f,
	0.498450068093346f, 0.115165693298854f, 0.832109040182439f, -0.183583051030518f, 0.110345826045748f,
	-0.397383499536540f, -0.148559279659243f, -0.183583051030518f, 0.856754854739268f, 0.228658865951530f,
	-0.317443730363866f, 0.068169644262314f, 0.110345826045748f, 0.228658865951530f, 0.911109927149224f
};
const float cIMTA::m_sacfConstantInHNLoS[25] =
{
	0.937210184455461f, 0.213628632168364f, -0.031342219949974f, -0.273893305722215f, 0.0f,
	0.213628632168364f, 0.976387959392929f, 0.006857515511760f, 0.031342219949974f, 0.0f,
	-0.031342219949974f, 0.006857515511760f, 0.976387959392929f, -0.213628632168364f, 0.0f,
	-0.273893305722215f, 0.031342219949974f, -0.213628632168364f, 0.937210184455461f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 1.0f
};
const float cIMTA::m_sacfConstantUMiLoS[25] =
{
	0.753065949852806f, 0.241023875447849f, 0.454091158552085f, -0.097177920212920f, -0.398944655540474f,
	0.241023875447849f, 0.929354051080552f, 0.137998056490968f, -0.242351266621617f, -0.020759074542993f,
	0.454091158552085f, 0.137998056490968f, 0.861515602815864f, -0.175603398954279f, -0.041377149612582f,
	-0.097177920212920f, -0.242351266621617f, -0.175603398954279f, 0.915728740196339f, 0.249853229004786f,
	-0.398944655540474f, -0.020759074542993f, -0.041377149612582f, 0.249853229004786f, 0.881063855850205f
};
const float cIMTA::m_sacfConstantUMiNLoS[25] =
{
	0.913514893522226f, 0.0f, 0.178007039816570f, -0.365792336017060f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
	0.178007039816570f, 0.0f, 0.967794909860288f, -0.178007039816570f, 0.0f,
	-0.365792336017060f, 0.0f, -0.178007039816570f, 0.913514893522227f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 1.0f
};
const float cIMTA::m_sacfConstantUMiO2I[25] =
{
	0.896950683840360f, 0.241943793527554f, 0.223605545793171f, -0.294861376620174f, 0.0f,
	0.241943793527554f, 0.959179465107684f, -0.030708888757422f, 0.143160464655990f, 0.0f,
	0.223605545793171f, -0.030708888757422f, 0.973505191933743f, 0.036676495468768f, 0.0f,
	-0.294861376620174f, 0.143160464655990f, 0.036676495468768f, 0.944042734529146f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 1.0f
};
const float cIMTA::m_sacfConstantSMaLoS[25] =
{
	0.854017192528818f, -0.040746823503881f, 0.423846524961968f, -0.298912118384658f, 0.0f,
	-0.040746823503881f, 0.958761974312789f, -0.023404035164252f, -0.280298812206218f, 0.0f,
	0.423846524961968f, -0.023404035164252f, 0.879509345569223f, -0.215103894600008f, 0.0f,
	-0.298912118384658f, -0.280298812206218f, -0.215103894600008f, 0.886461750943879f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 1.0f
};
const float cIMTA::m_sacConstantSMaNLoS[25] =
{
	0.888863320019977f, -0.028738390905236f, 0.394136210972923f, -0.231846394000744f, 0.0f,
	-0.028738390905236f, 0.976874642167262f, 0.011594978528380f, -0.211555181576079f, 0.0f,
	0.394136210972923f, 0.011594978528380f, 0.917601710925213f, 0.050292184084163f, 0.0f,
	-0.231846394000744f, -0.211555181576079f, 0.050292184084163f, 0.948136251262026f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 1.0f
};
const float cIMTA::m_sacfConstantUMaLoS[25] =
{
	0.806310951408682f, 0.245016774934418f, 0.479171304494613f, -0.120392914754038f, -0.213845356893992f,
	0.245016774934418f, 0.924083471541761f, -0.108566442433108f, -0.271617534928914f, 0.021766026753229f,
	0.479171304494613f, -0.108566442433108f, 0.825678402680475f, -0.271600920527001f, -0.055644441252067f,
	-0.120392914754038f, -0.271617534928914f, -0.271600920527001f, 0.915216117252956f, -0.018489442540902f,
	-0.213845356893992f, 0.021766026753229f, -0.055644441252067f, -0.018489442540902f, 0.974863190445988f
};
const float cIMTA::m_sacfConstantUMaNLoS[25] =
{
	0.913941405256431f, 0.147728073775767f, 0.318005795482914f, -0.204352240055453f, 0.0f,
	0.147728073775767f, 0.913941405256431f, 0.204352240055453f, -0.318005795482914f, 0.0f,
	0.318005795482914f, 0.204352240055453f, 0.923123353576218f, 0.070397088759368f, 0.0f,
	-0.204352240055453f, -0.318005795482914f, 0.070397088759368f, 0.923123353576218f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 1.0f
};
const float cIMTA::m_sacfConstantRMaLoS[25] =
{
	0.965925826289068f, 0.0f, 0.0f, -0.258819045102521f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	-0.258819045102521f, 0.0f, 0.0f, 0.965925826289068f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 1.0f
};
const float cIMTA::m_sacfConstantRMaNLoS[25] =
{
	0.955557150656242f, -0.173466133506044f, 0.0f, -0.238369529001059f, 0.0f,
	-0.173466133506044f, 0.938008596337461f, 0.0f, 0.300082278256296f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	-0.238369529001059f, 0.300082278256296f, 0.0f, 0.923650688258459f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 1.0f
};
const float cIMTA::m_sacfAngleOffset[m_scbySubPathNum] =
{
	7.8016217564146532088488977351458e-4f, -7.8016217564146532088488977351458e-4f,
	0.00246615023306798769219317505587f, -0.00246615023306798769219317505587f,
	0.00434936049596986930569383739507f, -0.00434936049596986930569383739507f,
	0.00648389817115893437827151120494f, -0.00648389817115893437827151120494f,
	0.00895179373347891640004160995158f, -0.00895179373347891640004160995158f,
	0.01186300292580545813435032615342f, -0.01186300292580545813435032615342f,
	0.01543569190463785077831312115652f, -0.01543569190463785077831312115652f,
	0.02003812514214689787266089371302f, -0.02003812514214689787266089371302f,
	0.02652027798405383792135548122719f, -0.02652027798405383792135548122719f,
	0.03761359070972979671228245975171f, -0.03761359070972979671228245975171f
};
const float cIMTA::m_sacfMidPathDelayOffset[m_scbyMidPathNum] =
{
	0.0f, 5.0e-9f, 10.0e-9f
};
const unsigned char cIMTA::m_sacbyMidPathIndex[m_scbySubPathNum] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 0, 0
};

cIMTA::cIMTA(void)
{
	m_pfGain = 0;
	m_pfSinAoD = 0;
	m_pfCosAoD = 0;
	m_pfSinAoA = 0;
	m_pfCosAoA = 0;
	m_pfPhase = 0;
	m_fGainLoS = 0;
	m_fSinAoDLoS = 0;
	m_pfPhaseLoS = 0;
	//m_pfGainLoS = 0;
	//m_pfSinAoDLoS = 0;
	//m_ppfPhaseLoS = 0;
	//m_pwFFTIndex = 0;
	m_fAntGain = 0;
    m_fMaxAttenu = 0; // dBm
	m_byTxAntNum = 0;
	m_byRxAntNum = 0;
	m_bLoS = false;
	m_bBuilt = false;
	m_bEnable = false;
	m_fPLSF = 0.0f;
	m_pfTxAntSpacing = new float[m_byTxAntNum];
	m_pfTxSlantAngle = new float[m_byTxAntNum];
	m_pfRxAntSpacing = new float[m_byRxAntNum];
	m_pfRxSlantAngle = new float[m_byRxAntNum];
}

cIMTA::~cIMTA(void)
{
	//Refresh();
	if (m_pfTxAntSpacing)
	{
		delete []m_pfTxAntSpacing;
		m_pfTxAntSpacing = 0;
	}
	if (m_pfRxAntSpacing)
	{
		delete []m_pfRxAntSpacing;
		m_pfRxAntSpacing = 0;
	}
	if (m_pfTxSlantAngle)
	{
		delete []m_pfTxSlantAngle;
		m_pfTxSlantAngle = 0;
	}
	if (m_pfRxSlantAngle)
	{
		delete []m_pfRxSlantAngle;
		m_pfRxSlantAngle = 0;
	}
}

bool cIMTA::Build(float t_fFrequency/*Hz*/,sLocation &t_eLocation, sAntenna &t_eAntenna,  float t_fVelocity/*km/h*/, float t_fVAngle/*degree*/,std::ofstream &t_fp)
{
	m_bBuilt = false;
	m_fAntGain = t_eAntenna.fAntGain;
	m_fMaxAttenu=t_eAntenna.fMaxAttenu * -0.1f;
	m_byTxAntNum=t_eAntenna.byTxAntNum;
	m_byRxAntNum=t_eAntenna.byRxAntNum;
	
	if (m_pfTxAntSpacing)
	{		
		delete []m_pfTxAntSpacing;
	}
	m_pfTxAntSpacing = new float[m_byTxAntNum];

		if (m_pfTxSlantAngle)
	{
		delete []m_pfTxSlantAngle;
	}
	m_pfTxSlantAngle = new float[m_byTxAntNum];

	if (m_pfRxAntSpacing)
	{
		delete []m_pfRxAntSpacing;
	}
	m_pfRxAntSpacing = new float[m_byRxAntNum];

	if (m_pfRxSlantAngle)
	{
		delete []m_pfRxSlantAngle;
	}
	m_pfRxSlantAngle = new float[m_byRxAntNum];

	for (unsigned char byTempTxAnt = 0; byTempTxAnt != m_byTxAntNum; ++ byTempTxAnt)
	{
		m_pfTxAntSpacing[byTempTxAnt] = t_eAntenna.pfTxAntSpacing[byTempTxAnt] * c_PI2;
		m_pfTxSlantAngle[byTempTxAnt] = t_eAntenna.pfTxSlantAngle[byTempTxAnt] * c_Degree2PI;
	}
	for (unsigned char byTempRxAnt = 0; byTempRxAnt != m_byRxAntNum; ++ byTempRxAnt)
	{
		m_pfRxAntSpacing[byTempRxAnt] = t_eAntenna.pfRxAntSpacing[byTempRxAnt] * c_PI2;
		m_pfRxSlantAngle[byTempRxAnt] = t_eAntenna.pfRxSlantAngle[byTempRxAnt] * c_Degree2PI;
	}
	m_fTxAngle = t_eAntenna.fTxAngle * c_Degree2PI;
	m_fRxAngle = t_eAntenna.fRxAngle * c_Degree2PI;
	m_fVelocity = t_fVelocity / 3.6f * t_fFrequency * c_PI2 / c_C;
	m_fvAngle = t_fVAngle * c_Degree2PI;

	//m_wFFTNum = 1;
	//m_byFFTOrder = 0;
	//m_wHNum = t_wHNum;
	//while (true)
	//{
	//	if (m_wHNum > m_wFFTNum)
	//	{
	//		m_wFFTNum <<= 1;
	//		++ m_byFFTOrder;
	//	}
	//	else
	//	{
	//		m_fFFTTime = 1.0f / t_fHBandWidth / m_wFFTNum;
	//		break;
	//	}
	//}

	float fTemp;
	float fSFSTD;
	float fPL1;
	float fPL2;
	float fDistanceBP = 4*(t_eLocation.fUEAntH-1)*(t_eLocation.fUEAntH-1)*t_fFrequency/c_C;
	switch (t_eLocation.eType)
	{
	case Los:
		m_bLoS = true;
		if(3<t_eLocation.fDistance&&t_eLocation.fDistance<fDistanceBP)
		{
		m_fPLSF = 22.7f * log10(t_eLocation.fDistance) + 27.0f + 20.0f * (log10(t_fFrequency) - 9.0f);//转换为GHz
		}
		else 
		{
			if(fDistanceBP<t_eLocation.fDistance&&t_eLocation.fDistance<5000)
		    {
		     m_fPLSF = 40.0f * log10(t_eLocation.fDistance) + 7.56f - 2 * 17.3f * log10(t_eLocation.fUEAntH-1) + 2.7f *(log10(t_fFrequency) - 9.0f);
		    }
			else if(t_eLocation.fDistance<3)
			{
		     m_fPLSF = 22.7f * log10(t_eLocation.fDistance) + 27.0f + 20.0f * (log10(t_fFrequency) - 9.0f);
			}
		}
		fSFSTD = 3.0f;
		//fprintf(t_fp, "%f\n", t_eLocation.fDistance);
		//fprintf(t_fp, "%f\n", m_fPLSF);
		break;
	case Nlos:
		fTemp = (2.8f - 0.0024f * t_eLocation.fDistance1) > 1.84f ? (2.8f - 0.0024f * t_eLocation.fDistance1) : 1.84f;
		if(3<t_eLocation.fDistance1&&t_eLocation.fDistance1<fDistanceBP)
		{
		fPL1 = 22.7f * log10(t_eLocation.fDistance1) + 27.0f + 20.0f *(log10(t_fFrequency) - 9.0f);
		}
		else 
		{
			if(fDistanceBP<t_eLocation.fDistance1&&t_eLocation.fDistance1<5000)
		    {
		     fPL1 = 40.0f * log10(t_eLocation.fDistance1) + 7.56f - 2 * 17.3f * log10(t_eLocation.fUEAntH-1) + 2.7f * (log10(t_fFrequency) - 9.0f);
		    }
			else if(t_eLocation.fDistance1<3)
			{
		     fPL1 = 22.7f * log10(t_eLocation.fDistance1) + 27.0f + 20.0f * (log10(t_fFrequency) - 9.0f);
			}
		}
		fPL1=fPL1 + 17.3f - 12.5f*fTemp + 10 * fTemp * log10(t_eLocation.fDistance1) + 3 * (log10(t_fFrequency) - 9.0f);
		fTemp = (2.8f - 0.0024f * t_eLocation.fDistance2) > 1.84f ? (2.8f - 0.0024f * t_eLocation.fDistance2) : 1.84f;
		if(3<t_eLocation.fDistance2&&t_eLocation.fDistance2<fDistanceBP)
		{
		fPL2 = 22.7f * log10(t_eLocation.fDistance2) + 27.0f + 20.0f * (log10(t_fFrequency) - 9.0f);
		}
		else 
		{
			if(fDistanceBP<t_eLocation.fDistance2&&t_eLocation.fDistance2<5000)
		    {
		     fPL2 = 40.0f * log10(t_eLocation.fDistance2) + 7.56f - 2 * 17.3f * log10(t_eLocation.fUEAntH-1) + 2.7f * (log10(t_fFrequency) - 9.0f);
		    }
			else if(t_eLocation.fDistance1<3)
			{
		     fPL2 = 22.7f * log10(t_eLocation.fDistance2) + 27.0f + 20.0f *(log10(t_fFrequency) - 9.0f);
			}
		}
		fPL2 = fPL2 + 17.3f - 12.5f*fTemp + 10 * fTemp * log10(t_eLocation.fDistance2) + 3 * (log10(t_fFrequency) - 9.0f);
		m_fPLSF = fPL1 < fPL2 ? fPL1 : fPL2;	
	default:
		break;
	}

	//fprintf(t_fp, "%f\n", t_eLocation.fDistance);
	//fprintf(t_fp, "%f\n", m_fPLSF);

	//m_fPLSF = pow(10.0f, -0.1f * m_fPLSF);// transform m_fPLSF,值为0了

	float fDSMean;
	float fDSSTD;
	float fASDMean;
	float fASDSTD;
	float fASAMean;
	float fASASTD;
	float fKMean;
	float fKSTD;
	const float *cpfConstant;

			if (m_bLoS)
			{
				fDSMean = -7.19f;
				fDSSTD = 0.40f;
				fASDMean = 1.20f;
				fASDSTD = 0.43f;
				fASAMean = 1.75f;
				fASASTD = 0.19f;
				fKMean = 9.0f;
				fKSTD = 5.0f;
				cpfConstant = m_sacfConstantUMiLoS;
				m_fDSRatio = 3.2f;
				m_fXPR = 9.0f;
				m_byPathNum = 12;
				m_fAoDRatio = 3.0f;
				m_fAoARatio = 17.0f;
				m_fPathShadowSTD = 3.0f;
				m_fC = 1.146f;
			}
			else
			{
				fDSMean = -6.89f;
				fDSSTD = 0.54f;
				fASDMean = 1.41f;
				fASDSTD = 0.17f;
				fASAMean = 1.84f;
				fASASTD = 0.15f;
				fKMean = 0.0f;
				fKSTD = 0.0f;
				cpfConstant = m_sacfConstantUMiNLoS;
				m_fDSRatio = 3.0f;
				m_fXPR = 8.0f;
				m_byPathNum = 19;
				m_fAoDRatio = 10.0f;
				m_fAoARatio = 22.0f;
				m_fPathShadowSTD = 3.0f;
				m_fC = 1.273f;
			}

	m_fXPR = pow(10.0f, m_fXPR * -0.05f);
	m_fPathShadowSTD /= 10.0f;

	float afTemp[5] = {0.0f};
	for (unsigned char byTemp = 0; byTemp != 5; ++ byTemp)
	{
		for (unsigned char byTempTime = 0; byTempTime != 5; ++ byTempTime)
		{
			afTemp[byTemp] += (cpfConstant[byTemp * 5 + byTempTime] * t_eLocation.afPosCor[byTempTime]);
		}
	}
	m_fDS = pow(10.0f, fDSSTD * afTemp[0] + fDSMean);
	m_fAoD = pow(10.0f, fASDSTD * afTemp[1] + fASDMean);
	m_fAoD = m_fAoD < 104.0f ? m_fAoD : 104.0f;
	m_fAoA = pow(10.0f, fASASTD * afTemp[2] + fASAMean);
	m_fAoA = m_fAoA < 104.0f ? m_fAoA : 104.0f;
	//fprintf(t_fp,"%f\n",m_fAoA);
	m_fAoD *= c_Degree2PI;
	m_fAoA *= c_Degree2PI;
	//m_fPLSF *= pow(10.0f, fSFSTD * afTemp[3] * 0.1f);
	m_fPLSF+=fSFSTD * afTemp[3];
	m_fKDB = fKSTD * afTemp[4] + fKMean;
	m_fK = pow(10.0f, m_fKDB * 0.1f);
	m_fDS *= -m_fDSRatio;	
	//
	//fprintf(t_fp, "%f\n", t_eLocation.fDistance);
	//fprintf(t_fp, "%f\n", m_fPLSF);

	if (m_bLoS)
	{
		m_fC *= (1.1035f - 0.028f * m_fKDB - 0.002f * pow(m_fKDB, 2.0f) + 0.0001f * pow(m_fKDB, 3.0f));
		m_fD = 0.7705f - 0.0433f * m_fKDB + 0.0002f * pow(m_fKDB, 2.0f) + 0.000017f * pow(m_fKDB, 3.0f);
		m_fSinAoALoS = sin(m_fTxAngle);
		m_fCosAoALoS = cos(m_fTxAngle - m_fvAngle) * m_fVelocity;
		m_fSinAoDLoS = sin(m_fRxAngle);
	}

	//m_fSinAoALoS = sin(m_fRxAngle);
	//m_fCosAoALoS = cos(m_fRxAngle);
	
	m_bBuilt = true;

	return true;
}

bool cIMTA::Enable(bool *t_pbEnable, std::ofstream &t_fp)
{
	//if (m_bBuilt == false)
	//{
	//	return false;
	//}
	Refresh();
    
	if (m_bLoS)
	{
		m_pfPhaseLoS = new float[m_byTxAntNum * m_byRxAntNum * 2];
	}

	m_pfGain = new float[m_byPathNum * m_scbySubPathNum];
	m_pfSinAoD = new float[m_byPathNum * m_scbySubPathNum];
	m_pfCosAoD = new float[m_byPathNum * m_scbySubPathNum];
	m_pfPhase = new float[m_byTxAntNum * m_byRxAntNum * m_byPathNum * m_scbySubPathNum * 2];	
	m_pfSinAoA = new float[m_byPathNum * m_scbySubPathNum];
	m_pfCosAoA = new float[m_byPathNum * m_scbySubPathNum];
	
	float *pfPathDelay = new float[m_byPathNum];
	float *pfPathPower = new float[m_byPathNum];
	float fPowerTotal = 0.0f;
	float *pfAoD = new float[m_byPathNum];
	float *pfAoA = new float[m_byPathNum];
//	unsigned char abyIndex[m_scbySubPathNum];
	float *pfXAoD = new float[m_byPathNum];
	float *pfXAoA = new float[m_byPathNum];
	float fPowerMax;
	unsigned char byStoreIndex = 0;
	RandomUniform(pfPathDelay, m_byPathNum, 1.0f, 0.0f, true);
	for (unsigned char byTempPath = 0; byTempPath != m_byPathNum; ++ byTempPath)
	{
		pfPathDelay[byTempPath] = m_fDS * log(pfPathDelay[byTempPath]);
	}
	SortBubble(pfPathDelay, m_byPathNum, false, false); //从小到大
	for (unsigned char byTempPath = 0; byTempPath != m_byPathNum; ++ byTempPath)
	{
		pfPathDelay[m_byPathNum - 1 - byTempPath] -= pfPathDelay[0];
	}

	RandomGaussian(pfPathPower, m_byPathNum, 0.0f, m_fPathShadowSTD);
	for (unsigned char byTempPath = 0; byTempPath != m_byPathNum; ++ byTempPath)
	{
		pfPathPower[byTempPath] = exp((m_fDSRatio - 1.0f) * pfPathDelay[byTempPath] / m_fDS) * pow(10.0f, pfPathPower[byTempPath]);
		fPowerTotal += pfPathPower[byTempPath];
	}
	for (unsigned char byTempPath = 0; byTempPath != m_byPathNum; ++ byTempPath)
	{
		pfPathPower[byTempPath] /= fPowerTotal;
	}

	SelectMax(pfPathPower, static_cast<unsigned char>(m_byPathNum), &m_byPathFirst, &m_byPathSecond);

		if (m_bLoS)
	{
		for (unsigned char byTempPath = 0; byTempPath != m_byPathNum; ++ byTempPath)
		{
			pfPathPower[byTempPath] /= (1.0f + m_fK);
		}
		pfPathPower[0] += (m_fK / (1.0f + m_fK));
		fPowerMax = pfPathPower[0] > pfPathPower[m_byPathFirst] ? pfPathPower[0] : pfPathPower[m_byPathFirst];
	}
	else
	{
		fPowerMax = pfPathPower[m_byPathFirst];
	}
	    
		RandomUniform(pfXAoD, m_byPathNum, 1.0f, -1.0f, true);
	    RandomUniform(pfXAoA, m_byPathNum, 1.0f, -1.0f, true);
		RandomGaussian(pfAoD, m_byPathNum, 0.0f, m_fAoD / 7.0f);//在winner 2 是除以5.0f
		RandomGaussian(pfAoA, m_byPathNum, 0.0f, m_fAoA / 7.0f);
		
        //FILE *fp4;//建立一个文件操作指针
        //fp4=fopen("AoA.txt","w+");//以追加的方式建立或打开1.txt，默认位置在你程序的目录下面
		for (unsigned char byTempPath = 0; byTempPath != m_byPathNum; ++ byTempPath)
		{
			if (pfXAoD[byTempPath] > 0.0f)
			{
				pfAoD[byTempPath] += (2.0f * m_fAoD * sqrt(-1.0f * log(pfPathPower[byTempPath] / fPowerMax)) / m_fC);
			}
			else
			{
				pfAoD[byTempPath] -= (2.0f * m_fAoD * sqrt(-1.0f * log(pfPathPower[byTempPath] / fPowerMax)) / m_fC);
			}
			if (pfXAoA[byTempPath] > 0.0f)
			{
				pfAoA[byTempPath] += (2.0f * m_fAoA * sqrt(-1.0f * log(pfPathPower[byTempPath] / fPowerMax)) / m_fC);
			}
			else
			{
				pfAoA[byTempPath] -= (2.0f * m_fAoA * sqrt(-1.0f * log(pfPathPower[byTempPath] / fPowerMax)) / m_fC);
			}
			pfAoD[byTempPath] += m_fTxAngle;
            pfAoA[byTempPath] += m_fRxAngle;

		}

	if (m_bLoS)
	{
		pfPathPower[0] -= (m_fK / (1.0f + m_fK));
		for (unsigned char byTempPath = 0; byTempPath != m_byPathNum; ++ byTempPath)
		{
			pfAoD[m_byPathNum - 1 - byTempPath] = pfAoD[m_byPathNum - 1 - byTempPath] - pfAoD[0] + m_fTxAngle;
			pfAoA[m_byPathNum - 1 - byTempPath] = pfAoA[m_byPathNum - 1 - byTempPath] - pfAoA[0] + m_fRxAngle;
			pfPathDelay[byTempPath] /= m_fD;
		}
	}

	//for (unsigned char byTempPath = 0; byTempPath != m_byPathNum; ++ byTempPath)
	//{
	//        while (pfAoA[byTempPath] > 2*c_PI)
	//		{
	//			pfAoA[byTempPath] -= c_PI2;
	//		}
	//		while (pfAoA[byTempPath] < 0)
	//		{
	//			pfAoA[byTempPath] += c_PI2;
	//		}
	//fprintf(t_fp,"%f\n",pfAoA[byTempPath]/c_Degree2PI);
	//}
        //FILE *fp3;//建立一个文件操作指针
        //fp3=fopen("sinAoA.txt","w+");//以追加的方式建立或打开1.txt，默认位置在你程序的目录下面
		for (unsigned char byTempPath = 0; byTempPath != m_byPathNum; ++ byTempPath)
 		{
      		for (unsigned char byTempSubPath = 0; byTempSubPath != m_scbySubPathNum; ++ byTempSubPath)
		    {
				m_pfGain[byTempPath * m_scbySubPathNum + byTempSubPath] = m_fMaxAttenu;
				m_pfGain[byTempPath * m_scbySubPathNum + byTempSubPath] = pow(10.0f, m_pfGain[byTempPath * m_scbySubPathNum + byTempSubPath]);
				m_pfGain[byTempPath * m_scbySubPathNum + byTempSubPath] *= pfPathPower[byTempPath];
				m_pfGain[byTempPath * m_scbySubPathNum + byTempSubPath] = sqrt(m_pfGain[byTempPath * m_scbySubPathNum + byTempSubPath]);
				//if (m_bLoS)
	   //        { 
    //            m_pfGain[byTempPath * m_scbySubPathNum + byTempSubPath]/=sqrt(1.0f / (1.0f + m_fK);//前面在计算Pn时候已经除过了，因而不需要再去除
				//}
			    m_pfSinAoD[byTempPath * m_scbySubPathNum + byTempSubPath] = pfAoD[byTempPath] + m_sacfAngleOffset[byTempSubPath] * m_fAoDRatio;
				m_pfSinAoD[byTempPath * m_scbySubPathNum + byTempSubPath] = sin(m_pfSinAoD[byTempPath * m_scbySubPathNum + byTempSubPath]);
			   
				//m_pfSinAoA[byTempPath * m_scbySubPathNum + byTempSubPath] = pfAoA[byTempPath] + m_sacfAngleOffset[abyIndex[byTempSubPath]] * m_fAoARatio;
			   	m_pfSinAoA[byTempPath * m_scbySubPathNum + byTempSubPath] = pfAoA[byTempPath] + m_sacfAngleOffset[byTempSubPath] * m_fAoARatio;
				m_pfCosAoA[byTempPath * m_scbySubPathNum + byTempSubPath] = cos(m_pfSinAoA[byTempPath * m_scbySubPathNum + byTempSubPath] - m_fvAngle) * m_fVelocity;
		     	m_pfSinAoA[byTempPath * m_scbySubPathNum + byTempSubPath] = sin(m_pfSinAoA[byTempPath * m_scbySubPathNum + byTempSubPath]);
				
				//fprintf(t_fp,"%d%d%f\n",byTempPath,byTempSubPath,m_pfSinAoA[byTempPath * m_scbySubPathNum + byTempSubPath]);
			}
	}
	
		if (m_bLoS)
	{
	     	m_fGainLoS= m_fMaxAttenu;
			m_fGainLoS = sqrt(m_fK / (1.0f + m_fK) * pow(10.0f, m_fGainLoS));
		}

		   //FILE *fp3;//建立一个文件操作指针
     //      fp3=fopen("sinAoA.txt","w+");//以追加的方式建立或打开1.txt，默认位置在你程序的目录下面
		   //if(m_pfSinAoA!=nullptr)
		   //fprintf(fp3,"%f\n",m_pfSinAoA);

	float *pfPhasePol = new float[m_byPathNum * m_scbySubPathNum * 4];
	float *pfSlantVV = new float[m_byTxAntNum * m_byRxAntNum];
	float *pfSlantVH = new float[m_byTxAntNum * m_byRxAntNum];
	float *pfSlantHV = new float[m_byTxAntNum * m_byRxAntNum];
	float *pfSlantHH = new float[m_byTxAntNum * m_byRxAntNum];
	for (unsigned char byTempTxAnt = 0; byTempTxAnt != m_byTxAntNum; ++ byTempTxAnt)
	{
		for (unsigned char byTempRxAnt = 0; byTempRxAnt != m_byRxAntNum; ++ byTempRxAnt)
		{
			pfSlantVV[byTempTxAnt * m_byRxAntNum + byTempRxAnt] = sin(m_pfTxSlantAngle[byTempTxAnt]) * sin(m_pfRxSlantAngle[byTempRxAnt]);
			pfSlantVH[byTempTxAnt * m_byRxAntNum + byTempRxAnt] = sin(m_pfTxSlantAngle[byTempTxAnt]) * cos(m_pfRxSlantAngle[byTempRxAnt]) * m_fXPR;
			pfSlantHV[byTempTxAnt * m_byRxAntNum + byTempRxAnt] = cos(m_pfTxSlantAngle[byTempTxAnt]) * sin(m_pfRxSlantAngle[byTempRxAnt]) * m_fXPR;
			pfSlantHH[byTempTxAnt * m_byRxAntNum + byTempRxAnt] = cos(m_pfTxSlantAngle[byTempTxAnt]) * cos(m_pfRxSlantAngle[byTempRxAnt]);
		}
	}

	RandomUniform(pfPhasePol, m_byPathNum * m_scbySubPathNum * 4, c_PI, c_PINeg, false);
	for (unsigned char byTempTxAnt = 0; byTempTxAnt != m_byTxAntNum; ++ byTempTxAnt)
	{
		for (unsigned char byTempRxAnt = 0; byTempRxAnt != m_byRxAntNum; ++ byTempRxAnt)
		{
				for (unsigned char byTempPath = 0; byTempPath != m_byPathNum; ++ byTempPath)
				{
					for (unsigned char byTempSubPath = 0; byTempSubPath != m_scbySubPathNum; ++ byTempSubPath)
					{
						m_pfPhase[byTempTxAnt * m_byRxAntNum * m_byPathNum * m_scbySubPathNum * 2 + byTempRxAnt * m_byPathNum * m_scbySubPathNum * 2 + byTempPath * m_scbySubPathNum * 2 + byTempSubPath * 2] =
							pfSlantVV[byTempTxAnt * m_byRxAntNum + byTempRxAnt] * cos(pfPhasePol[byTempPath * m_scbySubPathNum * 4 + byTempSubPath * 4]) +
							pfSlantVH[byTempTxAnt * m_byRxAntNum + byTempRxAnt] * cos(pfPhasePol[byTempPath * m_scbySubPathNum * 4 + byTempSubPath * 4 + 1]) +
							pfSlantHV[byTempTxAnt * m_byRxAntNum + byTempRxAnt] * cos(pfPhasePol[byTempPath * m_scbySubPathNum * 4 + byTempSubPath * 4 + 2]) +
							pfSlantHH[byTempTxAnt * m_byRxAntNum + byTempRxAnt] * cos(pfPhasePol[byTempPath * m_scbySubPathNum * 4 + byTempSubPath * 4 + 3]);
						m_pfPhase[byTempTxAnt * m_byRxAntNum * m_byPathNum * m_scbySubPathNum * 2 + byTempRxAnt * m_byPathNum * m_scbySubPathNum * 2 + byTempPath * m_scbySubPathNum * 2 + byTempSubPath * 2 + 1] =
							pfSlantVV[byTempTxAnt * m_byRxAntNum + byTempRxAnt] * sin(pfPhasePol[byTempPath * m_scbySubPathNum * 4 + byTempSubPath * 4]) +
							pfSlantVH[byTempTxAnt * m_byRxAntNum + byTempRxAnt] * sin(pfPhasePol[byTempPath * m_scbySubPathNum * 4 + byTempSubPath * 4 + 1]) +
							pfSlantHV[byTempTxAnt * m_byRxAntNum + byTempRxAnt] * sin(pfPhasePol[byTempPath * m_scbySubPathNum * 4 + byTempSubPath * 4 + 2]) +
							pfSlantHH[byTempTxAnt * m_byRxAntNum + byTempRxAnt] * sin(pfPhasePol[byTempPath * m_scbySubPathNum * 4 + byTempSubPath * 4 + 3]);
					}
				}
			}
		}
	if (m_bLoS)
		{
			RandomUniform(pfPhasePol, 1, c_PI, c_PINeg, false);
			for (unsigned char byTempTxAnt = 0; byTempTxAnt != m_byTxAntNum; ++ byTempTxAnt)
			{
				for (unsigned char byTempRxAnt = 0; byTempRxAnt != m_byRxAntNum; ++ byTempRxAnt)
				{
					m_pfPhaseLoS[byTempTxAnt * m_byRxAntNum * 2 + byTempRxAnt * 2] =
						(pfSlantVV[byTempTxAnt * m_byRxAntNum + byTempRxAnt] +
						pfSlantHH[byTempTxAnt * m_byRxAntNum + byTempRxAnt]) *
						cos(pfPhasePol[0]);
					m_pfPhaseLoS[byTempTxAnt * m_byRxAntNum * 2 + byTempRxAnt * 2 + 1] =
						(pfSlantVV[byTempTxAnt * m_byRxAntNum + byTempRxAnt] +
						pfSlantHH[byTempTxAnt * m_byRxAntNum + byTempRxAnt]) *
						sin(pfPhasePol[0]);
				}
			}
		}


		//fprintf(fp2,"%f\n",m_pfPhase[0]);

	delete []pfSlantVV;
	delete []pfSlantVH;
	delete []pfSlantHV;
	delete []pfSlantHH;
	delete []pfPathDelay;
	delete []pfPathPower;
	delete []pfXAoD;
	delete []pfXAoA;
	delete []pfAoD;
	delete []pfAoA;

	return true;
}

void cIMTA::Calculate( float t_fT/*s */, float *t_pfTemp, float *t_pfSin, float *t_pfCos,float *t_pfH)
{
	float fCos;
	float fSin;

		//for (unsigned char byTempTxAnt = 0; byTempTxAnt != m_byTxAntNum; ++ byTempTxAnt)
		//{
		//	for (unsigned char byTempRxAnt = 0; byTempRxAnt != m_byRxAntNum; ++ byTempRxAnt)
		//	{
		//		for (unsigned char byTempPath = 0; byTempPath != m_byPathNum; ++ byTempPath)
		//		{
		//			for (unsigned char byTempSubPath = 0; byTempSubPath != m_scbySubPathNum; ++ byTempSubPath)
		//			{
		//				t_pfTemp[byTempTxAnt * m_byRxAntNum * m_byPathNum * m_scbySubPathNum + byTempRxAnt * m_byPathNum * m_scbySubPathNum + byTempPath * m_scbySubPathNum + byTempSubPath] =
		//					m_pfTxAntSpacing[byTempRxAnt] * m_pfSinAoD[byTempPath * m_scbySubPathNum + byTempSubPath] +
		//					m_pfRxAntSpacing[byTempTxAnt] * m_pfSinAoA[byTempPath * m_scbySubPathNum + byTempSubPath] +
		//					m_pfCosAoA[byTempPath * m_scbySubPathNum + byTempSubPath] * t_fT;
		//			}
		//		}
		//	}
		//}

		for (unsigned char byTempTxAnt = 0; byTempTxAnt != m_byTxAntNum; ++ byTempTxAnt)
		{
			for (unsigned char byTempRxAnt = 0; byTempRxAnt != m_byRxAntNum; ++ byTempRxAnt)
			{
				for (unsigned char byTempPath = 0; byTempPath != m_byPathNum; ++ byTempPath)
				{
						for (unsigned char byTempSubPath = 0; byTempSubPath != m_scbySubPathNum; ++ byTempSubPath)
						{
							t_pfTemp[byTempTxAnt * m_byRxAntNum * m_byPathNum * m_scbySubPathNum + byTempRxAnt * m_byPathNum * m_scbySubPathNum + byTempPath * m_scbySubPathNum + byTempSubPath] =
						            m_pfTxAntSpacing[byTempTxAnt] * m_pfSinAoD[byTempPath * m_scbySubPathNum + byTempSubPath] +
							         m_pfRxAntSpacing[byTempRxAnt] * m_pfSinAoA[byTempPath * m_scbySubPathNum + byTempSubPath] +
							         m_pfCosAoA[byTempPath * m_scbySubPathNum + byTempSubPath] * t_fT;//t_fT为时间
						
							t_pfCos[byTempTxAnt * m_byRxAntNum * m_byPathNum * m_scbySubPathNum + byTempRxAnt * m_byPathNum * m_scbySubPathNum + byTempPath * m_scbySubPathNum + byTempSubPath] =
								   cos(t_pfTemp[byTempTxAnt * m_byRxAntNum * m_byPathNum * m_scbySubPathNum + byTempRxAnt * m_byPathNum * m_scbySubPathNum + byTempPath * m_scbySubPathNum + byTempSubPath]*2*c_PI*c_C/c_FC);
							
							t_pfSin[byTempTxAnt * m_byRxAntNum * m_byPathNum * m_scbySubPathNum + byTempRxAnt * m_byPathNum * m_scbySubPathNum + byTempPath * m_scbySubPathNum + byTempSubPath] =
							       sin(t_pfTemp[byTempTxAnt * m_byRxAntNum * m_byPathNum * m_scbySubPathNum + byTempRxAnt * m_byPathNum * m_scbySubPathNum + byTempPath * m_scbySubPathNum + byTempSubPath]*2*c_PI*c_C/c_FC);

							t_pfH[byTempTxAnt * m_byRxAntNum * m_byPathNum  * 2 + byTempRxAnt * m_byPathNum  * 2 + byTempPath * 2 ] =
								m_pfGain[byTempPath * m_scbySubPathNum + byTempSubPath] *
								(t_pfCos[byTempTxAnt * m_byRxAntNum * m_byPathNum * m_scbySubPathNum + byTempRxAnt * m_byPathNum * m_scbySubPathNum + byTempPath * m_scbySubPathNum + byTempSubPath] *
								m_pfPhase[byTempTxAnt * m_byRxAntNum * m_byPathNum * m_scbySubPathNum * 2 + byTempRxAnt * m_byPathNum * m_scbySubPathNum * 2 + byTempPath * m_scbySubPathNum * 2 + byTempSubPath * 2] -
								t_pfSin[byTempTxAnt * m_byRxAntNum * m_byPathNum * m_scbySubPathNum + byTempRxAnt * m_byPathNum * m_scbySubPathNum + byTempPath * m_scbySubPathNum + byTempSubPath] *
								m_pfPhase[byTempTxAnt * m_byRxAntNum * m_byPathNum * m_scbySubPathNum * 2 + byTempRxAnt * m_byPathNum * m_scbySubPathNum * 2 + byTempPath * m_scbySubPathNum * 2 + byTempSubPath * 2+ 1]);
							t_pfH[byTempTxAnt * m_byRxAntNum * m_byPathNum  * 2 + byTempRxAnt * m_byPathNum  * 2 + byTempPath * 2 + 1] =
								m_pfGain[byTempPath * m_scbySubPathNum + byTempSubPath] *
								(t_pfSin[byTempTxAnt * m_byRxAntNum * m_byPathNum * m_scbySubPathNum + byTempRxAnt * m_byPathNum * m_scbySubPathNum + byTempPath * m_scbySubPathNum + byTempSubPath] *
								m_pfPhase[byTempTxAnt * m_byRxAntNum * m_byPathNum * m_scbySubPathNum * 2 + byTempRxAnt * m_byPathNum * m_scbySubPathNum * 2 + byTempPath * m_scbySubPathNum * 2 + byTempSubPath * 2] +
								t_pfCos[byTempTxAnt * m_byRxAntNum * m_byPathNum * m_scbySubPathNum + byTempRxAnt * m_byPathNum * m_scbySubPathNum + byTempPath * m_scbySubPathNum + byTempSubPath] *
								m_pfPhase[byTempTxAnt * m_byRxAntNum * m_byPathNum * m_scbySubPathNum * 2 + byTempRxAnt * m_byPathNum * m_scbySubPathNum * 2 + byTempPath * m_scbySubPathNum * 2 + byTempSubPath * 2+ 1]);
				    }
                    if (m_bLoS && byTempPath == 0)
				   {
					fSin = m_pfRxAntSpacing[byTempRxAnt] * m_fSinAoDLoS + m_pfTxAntSpacing[byTempTxAnt] * m_fSinAoALoS + m_fCosAoALoS * t_fT;
					fCos = cos(fSin);
					fSin = sin(fSin);
					t_pfH[byTempTxAnt * m_byRxAntNum * m_byPathNum  * 2 + byTempRxAnt * m_byPathNum  * 2 + byTempPath * 2] +=
					m_fGainLoS *
					(fCos * m_pfPhaseLoS[byTempTxAnt * m_byRxAntNum * 2 + byTempRxAnt * 2] -
					 fSin * m_pfPhaseLoS[byTempTxAnt * m_byRxAntNum * 2 + byTempRxAnt * 2 + 1]);
				    t_pfH[byTempTxAnt * m_byRxAntNum * m_byPathNum  * 2 + byTempRxAnt * m_byPathNum  * 2 + byTempPath * 2+ 1] +=
					m_fGainLoS *
					(fSin * m_pfPhaseLoS[byTempTxAnt * m_byRxAntNum * 2 + byTempRxAnt * 2] +
					fCos * m_pfPhaseLoS[byTempTxAnt * m_byRxAntNum * 2 + byTempRxAnt * 2 + 1]);
				    }
				}
			}
		}
	return;
}

void cIMTA::Refresh(void)
{
//	if (m_pwFFTIndex)
//	{
//		delete []m_pwFFTIndex;
//		m_pwFFTIndex = 0;
//	}
//	if (m_ppfGain)
//	{
//		for (unsigned char byTempSec = 0; byTempSec != m_byBSSecNum; ++ byTempSec)
//		{
//			if (m_ppfGain[byTempSec])
//			{
//				delete []m_ppfGain[byTempSec];
//				m_ppfGain[byTempSec] = 0;
//			}
//		}
//		delete []m_ppfGain;
//		m_ppfGain = 0;
//	}
//	if (m_ppfSinAoD)
//	{
//		for (unsigned char byTempSec = 0; byTempSec != m_byBSSecNum; ++ byTempSec)
//		{
//			if (m_ppfSinAoD[byTempSec])
//			{
//				delete []m_ppfSinAoD[byTempSec];
//				m_ppfSinAoD[byTempSec] = 0;
//			}
//		}
//		delete []m_ppfSinAoD;
//		m_ppfSinAoD = 0;
//	}
	if (m_pfGain)
	{
		delete []m_pfGain;
		m_pfGain = 0;
	}
	if (m_pfSinAoD)
	{
		delete []m_pfSinAoD;
		m_pfSinAoD = 0;
	}
	if (m_pfCosAoD)
	{
		delete []m_pfCosAoD;
		m_pfCosAoD = 0;
	}
	if (m_pfPhase)
	{
		delete []m_pfPhase;
		m_pfPhase = 0;
	}
	if (m_pfSinAoA)
	{
		delete []m_pfSinAoA;
		m_pfSinAoA = 0;
	}
	if (m_pfCosAoA)
	{
		delete []m_pfCosAoA;
		m_pfCosAoA = 0;
	}
//	if (m_pfGainLoS)
//	{
//		delete []m_pfGainLoS;
//		m_pfGainLoS = 0;
//	}
//	if (m_pfSinAoDLoS)
//	{
//		delete []m_pfSinAoDLoS;
//		m_pfSinAoDLoS = 0;
//	}
//	if (m_ppfPhase)
//	{
//		for (unsigned char byTempSec = 0; byTempSec != m_byBSSecNum; ++ byTempSec)
//		{
//			if (m_ppfPhase[byTempSec])
//			{
//				delete []m_ppfPhase[byTempSec];
//				m_ppfPhase[byTempSec] = 0;
//			}
//		}
//		delete []m_ppfPhase;
//		m_ppfPhase = 0;
//	}
//	if (m_ppfPhaseLoS)
//	{
//		for (unsigned char byTempSec = 0; byTempSec != m_byBSSecNum; ++ byTempSec)
//		{
//			if (m_ppfPhaseLoS[byTempSec])
//			{
//				delete []m_ppfPhaseLoS[byTempSec];
//				m_ppfPhaseLoS[byTempSec] = 0;
//			}
//		}
//		delete []m_ppfPhaseLoS;
//		m_ppfPhaseLoS = 0;
//	}
//	if (m_pIppFFTSpec)
//	{
//		ippsFFTFree_C_32fc(m_pIppFFTSpec);
//		m_pIppFFTSpec = 0;
//	}
//	if (m_p8uBuffer)
//	{
//		ippsFree(m_p8uBuffer);
//		m_p8uBuffer = 0;
//	}
//
//	return;
}