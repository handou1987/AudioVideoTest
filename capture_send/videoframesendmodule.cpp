#include "videoframesendmodule.h"
#include "videoframesendqueue.h"
#include "clienttcpsocket.h"
#include "videomatframe2h264data.h"
#include <QHostAddress>

CVideoFrameSendModule::CVideoFrameSendModule():m_pQtThread(Q_NULLPTR),
    m_bRun(false)
{

}

CVideoFrameSendModule::~CVideoFrameSendModule()
{
    UnInit();
}

bool CVideoFrameSendModule::Init()
{    
    m_pQtThread = new QThread();
    if(Q_NULLPTR == m_pQtThread)
    {
        return false;
    }
    moveToThread(m_pQtThread);
    QObject::connect(m_pQtThread,QThread::started,this,CVideoFrameSendModule::OnThreadStart);
    SetRunFlag(true);
    m_pQtThread->start();
}

void CVideoFrameSendModule::UnInit()
{
    SetRunFlag(false);
    if(Q_NULLPTR != m_pQtThread)
    {
        m_pQtThread->wait();
        delete m_pQtThread;
        m_pQtThread = Q_NULLPTR;
    }
}

void CVideoFrameSendModule::SetRunFlag(bool bRun)
{
    m_bRun = bRun;
}

void CVideoFrameSendModule::OnThreadStart()
{    
    CVideoMatFrame2H264Data x264encode;
    if(false == x264encode.Init())
    {
        return;
    }

    CVideoFrameSendQueue* pFrameSendQueue = CVideoFrameSendQueue::GetInstance();
    if(Q_NULLPTR == pFrameSendQueue)
    {
        return;
    }

    CClientTcpSocket *pClientTcpSocket = new CClientTcpSocket();
    if(Q_NULLPTR == pClientTcpSocket || false == pClientTcpSocket->Init())
    {
        return;
    }

    while(false == pClientTcpSocket->Connect("127.0.0.1",6789) && m_bRun)
    {
        QThread::msleep(1000);
    }

    Mat* pSendmat = Q_NULLPTR;
    uint8_t* pH264Data = Q_NULLPTR;
    int iSize = 0;

    while(m_bRun)
    {
        pSendmat = pFrameSendQueue->GetSend();
        if(Q_NULLPTR == pSendmat)
        {
            QThread::msleep(5);
            continue;
        }

        x264encode.GetH264DataFromMatFrame(pSendmat,pH264Data,iSize);
        if(iSize > 0)
        {
            pClientTcpSocket->WriteData((char*)pH264Data,iSize);
        }

        pFrameSendQueue->GiveBackFree(pSendmat);
    }

    x264encode.Flush(pH264Data,iSize);
    x264encode.UnInit();

    delete pClientTcpSocket;
    pClientTcpSocket = Q_NULLPTR;
}
