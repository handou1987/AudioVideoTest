#include "videoframedrawthread.h"
#include "videoframedrawqueue.h"

#include "opencv2/core/mat.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/types_c.h"
using namespace cv;

CVideoFrameDrawThread::CVideoFrameDrawThread():
    m_bRun(false),
    m_pQtThread(Q_NULLPTR),
    m_pVideoDisplayWidget(Q_NULLPTR)
{

}

CVideoFrameDrawThread::~CVideoFrameDrawThread()
{
    UnInit();
}

bool CVideoFrameDrawThread::Init(QWidget* videoDisplayWidget)
{
    if(Q_NULLPTR == videoDisplayWidget)
    {
        return false;
    }
    m_pVideoDisplayWidget = (CWidgetVideoDisplay*)videoDisplayWidget;

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

void CVideoFrameDrawThread::UnInit()
{
    SetRunFlag(false);
    if(Q_NULLPTR != m_pQtThread)
    {
        m_pQtThread->wait();
        delete m_pQtThread;
        m_pQtThread = Q_NULLPTR;
    }
    m_pVideoDisplayWidget = Q_NULLPTR;
}

void CVideoFrameDrawThread::SetRunFlag(bool bRun)
{
    m_bRun = bRun;
}

void CVideoFrameDrawThread::OnThreadStart()
{
    CVideoFrameDrawQueue* pFrameDrawQueue = CVideoFrameDrawQueue::GetInstance();
    if(Q_NULLPTR == pFrameDrawQueue)
    {
        return;
    }

    Mat* pDrawFrame = Q_NULLPTR;
    while(m_bRun)
    {
        pDrawFrame = pFrameDrawQueue->GetUsing();
        if(Q_NULLPTR == pDrawFrame)
        {
            QThread::msleep(5);
            continue;
        }

        if(Q_NULLPTR != m_pVideoDisplayWidget)
        {
            m_pVideoDisplayWidget->UpdateNewDrawMat(pDrawFrame);
        }
    }
}







