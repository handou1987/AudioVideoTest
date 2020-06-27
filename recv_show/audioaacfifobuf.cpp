#include "audioaacfifobuf.h"
#include <string.h>

CAudioAacFifoBuf::CAudioAacFifoBuf():m_iDataLen(0),
    m_iDataBufLen(0),
    m_pchDataBuf(NULL)
{

}

CAudioAacFifoBuf::~CAudioAacFifoBuf()
{
    UnInit();
}

bool CAudioAacFifoBuf::Init()
{
    m_iDataLen = 0;
    m_iDataBufLen = 1024*1024;
    m_pchDataBuf = new char[m_iDataBufLen];
    if(Q_NULLPTR == m_pchDataBuf)
    {
        return false;
    }
    memset(m_pchDataBuf,0,m_iDataBufLen);

    return true;
}

void CAudioAacFifoBuf::UnInit()
{
    if(Q_NULLPTR != m_pchDataBuf)
    {
        delete []m_pchDataBuf;
        m_pchDataBuf = Q_NULLPTR;
    }
    m_iDataLen = 0;
    m_iDataBufLen = 0;
}

void CAudioAacFifoBuf::CopyData(char* pchDestBuf,int iDestBufSize,int& iRealCopy)
{
    QMutexLocker lock(&m_mutex);
    iRealCopy = 0;
    if(Q_NULLPTR == pchDestBuf || iDestBufSize <= 0)
    {
        return;
    }

    iRealCopy = iDestBufSize >= m_iDataLen ? m_iDataLen : iDestBufSize;
    memset(pchDestBuf,0,iDestBufSize);
    memcpy(pchDestBuf,m_pchDataBuf,iRealCopy);
}

int CAudioAacFifoBuf::WriteData(char* pchData,int iDataLen)
{
    QMutexLocker lock(&m_mutex);
    if(Q_NULLPTR == pchData || iDataLen <= 0)
    {
        return 0;
    }

    if(m_iDataLen + iDataLen > m_iDataBufLen)
    {
        return 0;
    }

    memcpy(m_pchDataBuf+m_iDataLen,pchData,iDataLen);
    m_iDataLen += iDataLen;
}

CAudioAacFifoBuf* CAudioAacFifoBuf::GetInstance()
{
    static CAudioAacFifoBuf obj;
    return &obj;
}

void CAudioAacFifoBuf::ClearHeadData(int iSize)
{
    QMutexLocker lock(&m_mutex);
    if(iSize < m_iDataLen)
    {
        memmove(m_pchDataBuf,m_pchDataBuf+iSize,m_iDataLen - iSize);
        m_iDataLen -= iSize;
    }
    else if(iSize == m_iDataLen)
    {
        m_iDataLen = 0;
    }
}
