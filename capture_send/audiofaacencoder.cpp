#include "audiofaacencoder.h"
#include <QString>

CAudioFaacEncoder::CAudioFaacEncoder():m_pFaacHandle(Q_NULLPTR),
    m_iPcmEleSize(0),
    m_iSingleSampleBitsNumber(0),
    m_iMaxAacOutput(0)
{    

}

CAudioFaacEncoder::~CAudioFaacEncoder()
{
    UnInit();
}

bool CAudioFaacEncoder::Init()
{
    unsigned long  nSampleRate = 44100;  // 采样率
    unsigned int  nChannels = 2;         // 声道数
    m_iSingleSampleBitsNumber = 16;      // 单样本位数
    unsigned long nInputSamples = 0;

    m_pFaacHandle = faacEncOpen(nSampleRate, nChannels, &nInputSamples, &m_iMaxAacOutput);
    if(Q_NULLPTR == m_pFaacHandle)
    {
        return false;
    }

    m_iPcmEleSize = nInputSamples * m_iSingleSampleBitsNumber / 8;

    faacEncConfigurationPtr pCfg = faacEncGetCurrentConfiguration(m_pFaacHandle);
    pCfg->inputFormat = FAAC_INPUT_16BIT;
    faacEncSetConfiguration(m_pFaacHandle, pCfg);

    return true;
}

void CAudioFaacEncoder::UnInit()
{
    if(Q_NULLPTR != m_pFaacHandle)
    {
        faacEncClose(m_pFaacHandle);
        m_pFaacHandle = Q_NULLPTR;
    }

    m_iPcmEleSize = 0;
    m_iMaxAacOutput = 0;
    m_iSingleSampleBitsNumber = 0;
}

int CAudioFaacEncoder::Encoder(char* pchPcmData,int iPcmDataLen,\
                               char* pchAacBuf,int iAacBufSize)
{
    if(Q_NULLPTR == pchPcmData || Q_NULLPTR == pchAacBuf)
    {
        return 0;
    }

    int iInputSamples = iPcmDataLen / (m_iSingleSampleBitsNumber / 8);
    if(iInputSamples <= 0)
    {
        return 0;
    }

    return faacEncEncode(m_pFaacHandle, (int*) pchPcmData, iInputSamples,\
                         (unsigned char*)pchAacBuf, iAacBufSize);
}

int CAudioFaacEncoder::GetPcmEleSize()
{
    return m_iPcmEleSize;
}

int CAudioFaacEncoder::GetAacEleMaxSize()
{
    return m_iMaxAacOutput;
}
