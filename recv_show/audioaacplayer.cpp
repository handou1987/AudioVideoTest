#include "audioaacplayer.h"
#include "audioaacfifobuf.h"
#include "audiofaaddecoder.h"

CAudioAacPlayer::CAudioAacPlayer():
    m_bRun(false),
    m_qtThread(Q_NULLPTR),
    m_pAudioOutput(Q_NULLPTR),
    m_pStreamOut(Q_NULLPTR)
{

}

CAudioAacPlayer::~CAudioAacPlayer()
{
    UnInit();
}

bool CAudioAacPlayer::Init()
{
    m_qtThread = new QThread();
    if(Q_NULLPTR == m_qtThread)
    {
        return false;
    }
    moveToThread(m_qtThread);
    SetRunFlag(true);
    QObject::connect(m_qtThread,QThread::started,this,CAudioAacPlayer::OnThreadStart);
    m_qtThread->start();

    QAudioFormat audioFormat;
    audioFormat.setSampleRate(44100);
    audioFormat.setChannelCount(2);
    audioFormat.setSampleSize(16);
    audioFormat.setCodec("audio/pcm");
    audioFormat.setByteOrder(QAudioFormat::LittleEndian);
    audioFormat.setSampleType(QAudioFormat::UnSignedInt);
    QAudioDeviceInfo info = QAudioDeviceInfo::defaultOutputDevice();
    if (false == info.isFormatSupported(audioFormat))
    {
        audioFormat = info.nearestFormat(audioFormat);
    }

    m_pAudioOutput = new QAudioOutput(audioFormat, this);
    if(Q_NULLPTR == m_pAudioOutput)
    {
        return false;
    }

    m_pStreamOut = m_pAudioOutput->start();
    if(Q_NULLPTR == m_pStreamOut)
    {
        return false;
    }

    return true;
}

void CAudioAacPlayer::UnInit()
{
    SetRunFlag(false);
    if(Q_NULLPTR != m_qtThread)
    {
        m_qtThread->wait();
        delete m_qtThread;
        m_qtThread = Q_NULLPTR;
    }

    if(Q_NULLPTR != m_pAudioOutput)
    {
        m_pAudioOutput->stop();
        delete m_pAudioOutput;
        m_pAudioOutput = Q_NULLPTR;
    }

    if(Q_NULLPTR != m_pStreamOut)
    {
        delete m_pStreamOut;
        m_pStreamOut = Q_NULLPTR;
    }
}

void CAudioAacPlayer::SetRunFlag(bool bRun)
{
    m_bRun = bRun;
}

void CAudioAacPlayer::OnThreadStart()
{
    CAudioFaadDecoder decoder;
    if(false == decoder.Init())
    {
        return;
    }

    CAudioAacFifoBuf* pFifoBuf = CAudioAacFifoBuf::GetInstance();
    if(Q_NULLPTR == pFifoBuf)
    {
        return;
    }
    char chAaacBuf[1024] = {0};
    char chPcmBuf[1024*1024] = {0};

    while(m_bRun)
    {
        memset(chAaacBuf,0,sizeof(chAaacBuf));
        int iRealAacDataSize = 0;
        pFifoBuf->CopyData(chAaacBuf,sizeof(chAaacBuf),iRealAacDataSize);
        if(0 == iRealAacDataSize)
        {
            QThread::msleep(5);
            continue;
        }

        int iTotalAacHandle = 0;
        do
        {
            int iPcmBufSize = sizeof(chPcmBuf);
            memset(chPcmBuf,0,iPcmBufSize);
            int iAacHandleSize = 0;

            decoder.Decode((unsigned char*)chPcmBuf,iPcmBufSize,(unsigned char*)(chAaacBuf+iTotalAacHandle),iRealAacDataSize-iTotalAacHandle,iAacHandleSize);
            if(0 == iAacHandleSize)
            {
                break;
            }
            m_pStreamOut->write(chPcmBuf,iPcmBufSize);
            iTotalAacHandle += iAacHandleSize;
        }
        while(1);

        if(0 != iTotalAacHandle)
        {
            pFifoBuf->ClearHeadData(iTotalAacHandle);
        }
    }
}
