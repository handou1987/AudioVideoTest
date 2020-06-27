#ifndef _AUDIO_PCM_FIFO_BUF_H
#define _AUDIO_PCM_FIFO_BUF_H

#include <QMutex>

class CAudioPcmFifoBuf
{
public:
    CAudioPcmFifoBuf();
    ~CAudioPcmFifoBuf();

    static CAudioPcmFifoBuf* GetInstance();

    bool Init();
    void UnInit();

    //返回实际获取的数据长度
    int GetData(char* pChBuf,int iBufSize,int iWantGet);

    //返回实际写入的数据长度
    int WriteData(char* pchData,int iDataLen);

private:
    int m_iDataLen;
    int m_iDataBufLen;
    char* m_pchDataBuf;
    QMutex m_mutex;
};
#endif
