#include <QMutexLocker>
#include "videomatqueue.h"

#define VIDEO_MAT_QUEUE_MAXITEMCOUNT      (30)

CVideoMatQueue::CVideoMatQueue()
{

}

CVideoMatQueue::~CVideoMatQueue()
{
    UnInit();
}

CVideoMatQueue* CVideoMatQueue::GetInstance()
{
    static CVideoMatQueue obj;
    return &obj;
}

bool CVideoMatQueue::Init()
{
    for(int i = 0; i < VIDEO_MAT_QUEUE_MAXITEMCOUNT; i++)
    {
        Mat* p = new Mat();
        if(Q_NULLPTR == p)
        {
            return false;
        }

        m_freeQueue.push(p);
    }
    return true;
}

void CVideoMatQueue::UnInit()
{
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

Mat* CVideoMatQueue::GetFree()
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

void CVideoMatQueue::GiveBackFree(Mat* p)
{
    QMutexLocker lock(&m_mutexFree);
    if(Q_NULLPTR != p)
    {
        m_freeQueue.push(p);
    }
}

void CVideoMatQueue::AddUsing(Mat* p)
{
    QMutexLocker lock(&m_mutexUsing);
    if(Q_NULLPTR != p)
    {
        m_usingQueue.push(p);
    }
}

Mat* CVideoMatQueue::GetUsing()
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
