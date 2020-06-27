#include "videocapture.h"
#include "videoframedrawqueue.h"
#include "videoframesendqueue.h"

CVideoCapture::CVideoCapture():m_pVideoCapture(Q_NULLPTR),
    m_pQtThread(Q_NULLPTR),
    m_bRun(false)
{

}

CVideoCapture::~CVideoCapture()
{
    UnInit();
}

bool CVideoCapture::Init()
{
    if(false == OpenCamera())
    {
        return false;
    }

    if(false == StartCaptureThread())
    {
        return false;
    }

    return true;
}

void CVideoCapture::UnInit()
{
    StopCaptureThread();
    CloseCamera();
}

void CVideoCapture::OnThreadStart()
{
    CVideoFrameDrawQueue* pFrameDrawQueue = CVideoFrameDrawQueue::GetInstance();
    CVideoFrameSendQueue* pFrameSendQueue = CVideoFrameSendQueue::GetInstance();
    if(Q_NULLPTR == pFrameDrawQueue || Q_NULLPTR == pFrameSendQueue)
    {
        return;
    }

    Mat* pDrawFrame = Q_NULLPTR;
    Mat* pSendFrame = Q_NULLPTR;
    while(m_bRun)
    {
        pDrawFrame = pFrameDrawQueue->GetFree();
        if(Q_NULLPTR == pDrawFrame)
        {
            QThread::msleep(5);
            continue;
        }

        *m_pVideoCapture >> *pDrawFrame;
        if(true == pDrawFrame->empty())
        {
            pFrameDrawQueue->GiveBackFree(pDrawFrame);
            QThread::msleep(5);
            continue;
        }

        pSendFrame = pFrameSendQueue->GetFree();
        if(Q_NULLPTR != pSendFrame)
        {
            *pSendFrame = pDrawFrame->clone();
            pFrameSendQueue->AddSend(pSendFrame);
        }

        pFrameDrawQueue->AddUsing(pDrawFrame);
    }
}

void CVideoCapture::SetRunFlag(bool bRun)
{
    m_bRun = bRun;
}

bool CVideoCapture::OpenCamera()
{
    m_pVideoCapture = new  VideoCapture(0);
    if(Q_NULLPTR == m_pVideoCapture)
    {
        return false;
    }

    m_pVideoCapture->set(CAP_PROP_FRAME_WIDTH, 640);
    m_pVideoCapture->set(CAP_PROP_FRAME_HEIGHT, 480);
    m_pVideoCapture->set(CAP_PROP_FPS, 25);
    if(true == m_pVideoCapture->isOpened())
    {
        return true;
    }

    return false;
}

bool CVideoCapture::StartCaptureThread()
{
    m_pQtThread = new QThread();
    if(Q_NULLPTR == m_pQtThread)
    {
        return false;
    }

    this->moveToThread(m_pQtThread);
    QObject::connect(m_pQtThread,QThread::started,this,CVideoCapture::OnThreadStart);

    SetRunFlag(true);
    m_pQtThread->start();

    return true;
}

void CVideoCapture::StopCaptureThread()
{
    SetRunFlag(false);
    if(Q_NULLPTR != m_pQtThread)
    {
        m_pQtThread->wait();
        delete m_pQtThread;
        m_pQtThread = Q_NULLPTR;
    }
}

void CVideoCapture::CloseCamera()
{
    if(Q_NULLPTR != m_pVideoCapture)
    {
        m_pVideoCapture->release();
        delete m_pVideoCapture;
        m_pVideoCapture = Q_NULLPTR;
    }
}

