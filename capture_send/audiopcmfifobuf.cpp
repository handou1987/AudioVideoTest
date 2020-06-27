#include "audiopcmfifobuf.h"
#include <string.h>

CAudioPcmFifoBuf::CAudioPcmFifoBuf()
{

}

CAudioPcmFifoBuf::~CAudioPcmFifoBuf()
{
    UnInit();
}

bool CAudioPcmFifoBuf::Init()
{
    m_iDataLen = 0;
    m_iDataBufLen = 1024*1024*5;
    m_pchDataBuf = new char[m_iDataBufLen];
    if(Q_NULLPTR == m_pchDataBuf)
    {
        return false;
    }

    memset(m_pchDataBuf,0,m_iDataBufLen);
    return true;
}

void CAudioPcmFifoBuf::UnInit()
{
    if(Q_NULLPTR != m_pchDataBuf)
    {
        delete []m_pchDataBuf;
        m_pchDataBuf = Q_NULLPTR;
    }
    m_iDataLen = 0;
    m_iDataBufLen = 0;
}

int CAudioPcmFifoBuf::GetData(char* pchBuf,int iBufSize,int iWantGet)
{
    QMutexLocker lock(&m_mutex);
    if(m_iDataLen < iWantGet || iBufSize < iWantGet)
    {
        return 0;
    }

    memset(pchBuf,0,iBufSize);
    memcpy(pchBuf,m_pchDataBuf,iWantGet);

    if(m_iDataLen > iWantGet)
    {
        memmove(m_pchDataBuf,m_pchDataBuf+iWantGet,m_iDataLen - iWantGet);
    }
    m_iDataLen -= iWantGet;
    return iWantGet;
}

int CAudioPcmFifoBuf::WriteData(char* pchData,int iDataLen)
{
    QMutexLocker lock(&m_mutex);
    if(Q_NULLPTR == pchData || iDataLen <= 0 || iDataLen > m_iDataBufLen)
    {
        return 0;
    }

    if(m_iDataLen + iDataLen > m_iDataBufLen)
    {
        int iRemove = m_iDataLen + iDataLen - m_iDataBufLen;
        if(iRemove == m_iDataBufLen)
        {
            m_iDataLen = 0;
        }
        else
        {
            memmove(m_pchDataBuf,m_pchDataBuf+iRemove,m_iDataLen-iRemove);
            m_iDataLen -= iRemove;
        }
    }

    memcpy(m_pchDataBuf+m_iDataLen,pchData,iDataLen);
    m_iDataLen += iDataLen;
    return iDataLen;
}

CAudioPcmFifoBuf* CAudioPcmFifoBuf::GetInstance()
{
    static CAudioPcmFifoBuf obj;
    return &obj;
}
