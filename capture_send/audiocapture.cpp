#include "audiocapture.h"
#include "audiopcmfifobuf.h"

CAudioCapture::CAudioCapture():m_pQtThread(Q_NULLPTR),
    m_bRun(false),
    m_pAudioFormatCtx(Q_NULLPTR)
{

}

CAudioCapture::~CAudioCapture()
{
    UnInit();
}

bool CAudioCapture::Init()
{        
    avdevice_register_all();
    AVInputFormat* pAudioInputFmt = av_find_input_format("dshow");
    if(Q_NULLPTR == pAudioInputFmt)
    {
        return false;
    }

    AVDictionary* pOptions = NULL;
    if(av_dict_set(&pOptions, "audio_buffer_size", "10", 0) < 0)
    {
        return false;
    }

    av_dict_set_int(&pOptions, "rtbufsize", 1024*100, 0);

    char* pAudioUtf8Url = "audio=麦克风 (2- Realtek(R) Audio)";
    if(0 != avformat_open_input(&m_pAudioFormatCtx,pAudioUtf8Url, pAudioInputFmt,&pOptions))
    {
        return false;
    }

    if(AVMEDIA_TYPE_AUDIO != m_pAudioFormatCtx->streams[0]->codec->codec_type)
    {
        return false;
    }

    m_pQtThread = new QThread();
    if(Q_NULLPTR == m_pQtThread)
    {
        return false;
    }
    moveToThread(m_pQtThread);
    QObject::connect(m_pQtThread,QThread::started,this,CAudioCapture::OnThreadStart);
    SetRunFlag(true);
    m_pQtThread->start();
    return true;
}

void CAudioCapture::UnInit()
{
    SetRunFlag(false);
    if(Q_NULLPTR != m_pQtThread)
    {
        m_pQtThread->wait();
        delete m_pQtThread;
        m_pQtThread = Q_NULLPTR;
    }

    if(Q_NULLPTR != m_pAudioFormatCtx)
    {
        avformat_close_input(&m_pAudioFormatCtx);
        m_pAudioFormatCtx = Q_NULLPTR;
    }
}

void CAudioCapture::SetRunFlag(bool bRun)
{
    m_bRun = bRun;
}

void CAudioCapture::OnThreadStart()
{
    CAudioPcmFifoBuf* pAudioFifoBuf = CAudioPcmFifoBuf::GetInstance();
    if(Q_NULLPTR == pAudioFifoBuf)
    {
        return;
    }

    AVPacket pkt;
    pkt.data = NULL;
    pkt.size = 0;

    while(m_bRun)
    {
        if(0 ==av_read_frame(m_pAudioFormatCtx,&pkt))
        {
            if(pkt.size > 0)
            {
                pAudioFifoBuf->WriteData((char*)(pkt.data),pkt.size);
                av_free_packet(&pkt);
                pkt.data = NULL;
                pkt.size = 0;
            }
        }
        else
        {
            QThread::msleep(5);
        }
    }
}

