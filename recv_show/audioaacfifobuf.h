#ifndef _AUDIO_AAC_FIFO_BUF_H
#define _AUDIO_AAC_FIFO_BUF_H

#include <QMutex>

class CAudioAacFifoBuf
{
private:
    CAudioAacFifoBuf();

public:
    ~CAudioAacFifoBuf();

    static CAudioAacFifoBuf* GetInstance();

    bool Init();
    void UnInit();

    void CopyData(char* pchDestBuf,int iDestBufSize,int& iRealyCopy);
    int WriteData(char* pchData,int iDataLen);
    void ClearHeadData(int iSize);

private:
    int m_iDataLen;
    int m_iDataBufLen;
    char* m_pchDataBuf;
    QMutex m_mutex;
};
#endif
