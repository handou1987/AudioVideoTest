#include "audiofaaddecoder.h"
#include <string.h>

#define AUDIO_FRAME_MAX_LEN 1024*5

CAudioFaadDecoder::CAudioFaadDecoder():m_hAAC(NULL)
{
}

CAudioFaadDecoder::~CAudioFaadDecoder()
{
    UnInit();
}

int CAudioFaadDecoder::GetOneAdtsFrame(unsigned char* buffer, int buf_size,\
                                       unsigned char* data ,int* data_size)
{
    int size = 0;
    if(NULL == buffer || buf_size <= 0 || NULL == data || data_size <= 0)
    {
        return -1;
    }

    while(1)
    {
        if(buf_size  < 7 )
        {
            return -1;
        }

        if((buffer[0] == 0xff) && ((buffer[1] & 0xf0) == 0xf0) )
        {
            size |= ((buffer[3] & 0x03) <<11);     //high 2 bit
            size |= buffer[4]<<3;                //middle 8 bit
            size |= ((buffer[5] & 0xe0)>>5);        //low 3bit
            break;
        }
        --buf_size;
        ++buffer;
    }

    if(buf_size < size)
    {
        return -1;
    }

    memcpy(data, buffer, size);
    *data_size = size;
    return 0;
}

bool CAudioFaadDecoder::Init()
{
    m_hAAC = NeAACDecOpen();
    if (NULL == m_hAAC)
    {
        return false;
    }

    NeAACDecConfigurationPtr pConfig = NeAACDecGetCurrentConfiguration(m_hAAC);
    if (NULL == pConfig)
    {
        return false;
    }

    pConfig->defObjectType = LC;
    pConfig->defSampleRate = 8000;
    pConfig->outputFormat = FAAD_FMT_16BIT;
    pConfig->downMatrix = 0;
    pConfig->dontUpSampleImplicitSBR = 1;
    NeAACDecSetConfiguration(m_hAAC, pConfig);
    return true;
}

void CAudioFaadDecoder::UnInit()
{
    if (NULL != m_hAAC)
    {
        NeAACDecClose(m_hAAC);
        m_hAAC = NULL;
    }
}

void CAudioFaadDecoder::Decode(unsigned char *dstBuffer, int& dstBufferSize, unsigned char *srcBuffer, int srcBufferSize,int& iSrcHandleSize)
{
    iSrcHandleSize = 0;
    if (NULL == dstBuffer || NULL == srcBuffer)
    {
        return ;
    }

    //检查ATDS帧头是否合法
    int size = 0;
    unsigned char frame[AUDIO_FRAME_MAX_LEN] = {0};
    if(GetOneAdtsFrame(srcBuffer, srcBufferSize, frame, &size) < 0)
    {
        return;
    }

    //初始化AAC解码器
    unsigned long samplerate = 0;
    unsigned char channels = 0;
    int ret = NeAACDecInit(m_hAAC, frame, size, &samplerate, &channels);
    if (ret < 0)
    {
        return;
    }

    //解码
    unsigned char* pcm_data = NULL;
    NeAACDecFrameInfo frame_info;
    if(GetOneAdtsFrame(srcBuffer, srcBufferSize, frame, &size) == 0)
    {
        pcm_data = (unsigned char*)NeAACDecDecode(m_hAAC, &frame_info, frame, size);
        if(frame_info.error > 0)
        {
            return;
        }
        else if(pcm_data && frame_info.samples > 0)
        {
            memset(dstBuffer,0,dstBufferSize);
            dstBufferSize = frame_info.samples * frame_info.channels;
            memcpy(dstBuffer, pcm_data, dstBufferSize);
        }
        iSrcHandleSize = size;
    }
}
