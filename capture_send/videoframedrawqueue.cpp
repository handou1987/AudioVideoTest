#include "videoframedrawqueue.h"
#include <QMutexLocker>

CVideoFrameDrawQueue::CVideoFrameDrawQueue()
{

}

CVideoFrameDrawQueue::~CVideoFrameDrawQueue()
{

}

CVideoFrameDrawQueue* CVideoFrameDrawQueue::GetInstance()
{
    static CVideoFrameDrawQueue obj;
    return &obj;
}

bool CVideoFrameDrawQueue::Init()
{
    int iQueueMaxSize = 60;
    Mat* pNew = Q_NULLPTR;
    for(int i = 0; i < iQueueMaxSize; i++)
    {
        pNew = new Mat();
        if(Q_NULLPTR == pNew)
        {
            return false;
        }
        m_freeQueue.push(pNew);
    }

    return true;
}

void CVideoFrameDrawQueue::UnInit()
{
    QMutexLocker lockFree(&m_mutexFree);
    QMutexLocker lockUsing(&m_mutexUsing);

    while(m_freeQueue.size())
    {
        delete m_freeQueue.front();
        m_freeQueue.pop();
    }

    while(m_usingQueue.size())
    {
        delete m_usingQueue.front();
        m_usingQueue.pop();
    }
}

Mat* CVideoFrameDrawQueue::GetFree()
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

void CVideoFrameDrawQueue::GiveBackFree(Mat* pFreeMat)
{
    if(Q_NULLPTR != pFreeMat)
    {
        QMutexLocker lock(&m_mutexFree);
        m_freeQueue.push(pFreeMat);
    }
}

void CVideoFrameDrawQueue::AddUsing(Mat* pUsingMat)
{
    if(Q_NULLPTR != pUsingMat)
    {
        QMutexLocker lock(&m_mutexUsing);
        m_usingQueue.push(pUsingMat);
    }
}

Mat* CVideoFrameDrawQueue::GetUsing()
{
    QMutexLocker lock(&m_mutexUsing);
    if(m_usingQueue.size())
    {
        Mat* p = m_usingQueue.front();
        m_usingQueue.pop();
        return p;
    }
    return Q_NULLPTR;
}
