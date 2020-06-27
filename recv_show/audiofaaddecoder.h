#ifndef _AUDIO_FAAD_DECODER_H
#define _AUDIO_FAAD_DECODER_H

#include "faad.h"

class CAudioFaadDecoder
{
public:
    CAudioFaadDecoder();
    ~CAudioFaadDecoder();

    bool Init();
    void UnInit();
    void Decode(unsigned char *dstBuffer, int& dstBufferSize, unsigned char *srcBuffer, int srcBufferSize,int& iSrcHandleSize);
private:
    int GetOneAdtsFrame(unsigned char* buffer, int buf_size, unsigned char* data ,int* data_size);

private:
    NeAACDecHandle m_hAAC;
};

#endif
