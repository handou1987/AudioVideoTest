#include <QMutexLocker>
#include "videoframesendqueue.h"

CVideoFrameSendQueue::CVideoFrameSendQueue()
{

}

CVideoFrameSendQueue::~CVideoFrameSendQueue()
{

}

CVideoFrameSendQueue* CVideoFrameSendQueue::GetInstance()
{
    static CVideoFrameSendQueue obj;
    return &obj;
}

bool CVideoFrameSendQueue::Init()
{
    int iMaxItemCount = 90;
    for(int i = 0; i < iMaxItemCount; i++)
    {
        Mat* pNew = new Mat();
        if(Q_NULLPTR == pNew)
        {
            return false;
        }

        m_freeQueue.push(pNew);
    }
    return true;
}

void CVideoFrameSendQueue::UnInit()
{
    QMutexLocker lockFree(&m_mutexFree);
    QMutexLocker lockSend(&m_mutexSend);

    while(m_freeQueue.size())
    {
        delete m_freeQueue.front();
        m_freeQueue.pop();
    }

    while(m_sendQueue.size())
    {
        delete m_sendQueue.front();
        m_sendQueue.pop();
    }
}

Mat* CVideoFrameSendQueue::GetFree()
{
    QMutexLocker lock(&m_mutexFree);
    if(m_freeQueue.size())
    {
        Mat* p = m_freeQueue.front();
        m_freeQueue.pop();
        return p;
    }

    return Q_NULLPTR;
}

void CVideoFrameSendQueue::GiveBackFree(Mat* pMatFree)
{
    if(Q_NULLPTR != pMatFree)
    {
        pMatFree->release();
        QMutexLocker lock(&m_mutexFree);
        m_freeQueue.push(pMatFree);
    }
}

void CVideoFrameSendQueue::AddSend(Mat* pMatSend)
{
    if(Q_NULLPTR != pMatSend)
    {
        QMutexLocker lock(&m_mutexSend);
        m_sendQueue.push(pMatSend);
    }
}

Mat* CVideoFrameSendQueue::GetSend()
{
    QMutexLocker lock(&m_mutexSend);
    if(m_sendQueue.size())
    {
        Mat* p = m_sendQueue.front();
        m_sendQueue.pop();
        return p;
    }
    return Q_NULLPTR;
}
