#include "audiosendmodule.h"
#include "audiopcmfifobuf.h"
#include "audiofaacencoder.h"
#include "clienttcpsocket.h"

CAudioSendModule::CAudioSendModule():m_pQtThread(Q_NULLPTR),
    m_bRun(false)
{

}

CAudioSendModule::~CAudioSendModule()
{
    UnInit();
}

bool CAudioSendModule::Init()
{    
    m_pQtThread = new QThread();
    if(Q_NULLPTR == m_pQtThread)
    {
        return false;
    }

    moveToThread(m_pQtThread);
    QObject::connect(m_pQtThread,QThread::started,this,CAudioSendModule::OnThreadStart);
    SetRunFlag(true);
    m_pQtThread->start();
    return true;
}

void CAudioSendModule::UnInit()
{
    SetRunFlag(false);
    if(Q_NULLPTR != m_pQtThread)
    {
        m_pQtThread->wait();
        delete m_pQtThread;
        m_pQtThread = Q_NULLPTR;
    }
}

void CAudioSendModule::SetRunFlag(bool bRun)
{
    m_bRun = bRun;
}

void CAudioSendModule::OnThreadStart()
{    
    CAudioFaacEncoder encoder;
    if(false == encoder.Init())
    {
        return;
    }

    int iPcmEleSize = encoder.GetPcmEleSize();
    char* pchPcmEleBuf = new char[iPcmEleSize];
    if(Q_NULLPTR == pchPcmEleBuf)
    {
        return;
    }
    memset(pchPcmEleBuf,0,iPcmEleSize);

    int iAacEleMaxSize = encoder.GetAacEleMaxSize();
    char* pchAacMaxBuf = new char[iAacEleMaxSize];
    if(Q_NULLPTR == pchAacMaxBuf)
    {
        return;
    }
    memset(pchAacMaxBuf,0,iAacEleMaxSize);

    CClientTcpSocket *pClientTcpSocket = new CClientTcpSocket();
    if(Q_NULLPTR == pClientTcpSocket || false == pClientTcpSocket->Init())
    {
        return;
    }

    CAudioPcmFifoBuf* pFifoPcmBuf = CAudioPcmFifoBuf::GetInstance();
    if(Q_NULLPTR == pFifoPcmBuf)
    {
        return;
    }

    while(false == pClientTcpSocket->Connect("127.0.0.1",9876) && m_bRun)
    {
        QThread::msleep(1000);
    }

    int iPcmLen = 0;
    int iAacLen = 0;
    while(m_bRun)
    {
        memset(pchPcmEleBuf,0,iPcmEleSize);
        iPcmLen = pFifoPcmBuf->GetData(pchPcmEleBuf,iPcmEleSize,iPcmEleSize);
        if(iPcmLen <= 0)
        {
            QThread::msleep(5);
            continue;
        }

        memset(pchAacMaxBuf,0,iAacEleMaxSize);
        iAacLen = encoder.Encoder(pchPcmEleBuf,iPcmLen,pchAacMaxBuf,iAacEleMaxSize);
        if(iAacLen > 0)
        {
            pClientTcpSocket->WriteData(pchAacMaxBuf,iAacLen);
        }
    }
}
