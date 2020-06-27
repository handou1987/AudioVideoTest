#ifndef _AUDIO_FAAC_ENCODER_H
#define _AUDIO_FAAC_ENCODER_H

#include "faac.h"

//pcm 2 aac
class CAudioFaacEncoder
{
public:
    CAudioFaacEncoder();
    ~CAudioFaacEncoder();

    bool Init();
    void UnInit();

    //返回aac的实际数据长度
    int Encoder(char* pchPcmData,int iPcmDataLen,\
                char* pchAacBuf,int iAacBufSize);

    int GetPcmEleSize();
    int GetAacEleMaxSize();

private:
    faacEncHandle m_pFaacHandle;
    int m_iPcmEleSize;
    unsigned int m_iSingleSampleBitsNumber;
    unsigned long m_iMaxAacOutput;
};

#endif
