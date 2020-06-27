#include <QPainter>
#include "videoframedrawthread.h"

#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/types_c.h"
using namespace cv;

#include "videomatqueue.h"

CVideoFrameDrawThread::CVideoFrameDrawThread():m_bRun(false),
    m_pQtThread(Q_NULLPTR),
    m_pVideoWidget(Q_NULLPTR)
{

}

CVideoFrameDrawThread::~CVideoFrameDrawThread()
{
    UnInit();
}

bool CVideoFrameDrawThread::Init(CVideoDisplayWidget* pVideoWidget)
{
    if(Q_NULLPTR == pVideoWidget)
    {
        return false;
    }
    m_pVideoWidget = pVideoWidget;

    m_pQtThread = new QThread();
    if(Q_NULLPTR == m_pQtThread)
    {
        return false;
    }
    moveToThread(m_pQtThread);
    QObject::connect(m_pQtThread,QThread::started,this,CVideoFrameDrawThread::OnThreadStart);
    SetRunFlag(true);
    m_pQtThread->start();
    return true;
}

void CVideoFrameDrawThread::SetRunFlag(bool bRun)
{
    m_bRun = bRun;
}

void CVideoFrameDrawThread::OnThreadStart()
{
    CVideoMatQueue* pFrameQueue = CVideoMatQueue::GetInstance();
    if(Q_NULLPTR == pFrameQueue)
    {
        return;
    }

    Mat* pMatFrame = Q_NULLPTR;
    while(m_bRun)
    {
        pMatFrame = pFrameQueue->GetUsing();
        if(Q_NULLPTR != pMatFrame && Q_NULLPTR != m_pVideoWidget)
        {
            m_pVideoWidget->UpdateNewMat(pMatFrame);
        }
        else
        {
            QThread::msleep(5);
        }
    }
}

void CVideoFrameDrawThread::UnInit()
{
    SetRunFlag(false);

    if(Q_NULLPTR != m_pQtThread)
    {
        m_pQtThread->wait();
        delete m_pQtThread;
        m_pQtThread = Q_NULLPTR;
    }
}
