#include "videomatframe2h264data.h"
#include <QString>
#include <QDebug>
#include<cstring>
#define H264_DATA_BUF_SIZE (1024*1024*8)


CVideoMatFrame2H264Data::CVideoMatFrame2H264Data():
    m_pH264DataBuf(Q_NULLPTR),
    m_pH264AvCodec(Q_NULLPTR),
    m_pH264CodecContext(Q_NULLPTR),
    m_pH264Packet(Q_NULLPTR),
    m_pH264Frame(Q_NULLPTR)
{

}

CVideoMatFrame2H264Data::~CVideoMatFrame2H264Data()
{
    UnInit();
}

bool CVideoMatFrame2H264Data::Init()
{    
    m_pH264DataBuf = new uint8_t[H264_DATA_BUF_SIZE];
    if(Q_NULLPTR == m_pH264DataBuf)
    {
        return false;
    }
    memset(m_pH264DataBuf,0,H264_DATA_BUF_SIZE);

    m_pH264AvCodec = avcodec_find_encoder_by_name("libx264");
    if (Q_NULLPTR == m_pH264AvCodec)
    {
        return false;
    }

    m_pH264CodecContext = avcodec_alloc_context3(m_pH264AvCodec);
    if (Q_NULLPTR == m_pH264CodecContext)
    {
        return false;
    }

    m_pH264CodecContext->bit_rate = 400000;
    m_pH264CodecContext->width = 640;
    m_pH264CodecContext->height = 480;
    m_pH264CodecContext->time_base.num = 1;
    m_pH264CodecContext->time_base.den = 25;
    m_pH264CodecContext->framerate.num = 25;
    m_pH264CodecContext->framerate.den = 1;
    m_pH264CodecContext->gop_size = 10;
    m_pH264CodecContext->max_b_frames = 1;
    m_pH264CodecContext->pix_fmt = AV_PIX_FMT_YUV420P;
    if (AV_CODEC_ID_H264 == m_pH264AvCodec->id)
    {
        av_opt_set(m_pH264CodecContext->priv_data, "preset", "fast", 0);
        av_opt_set(m_pH264CodecContext->priv_data, "tune", "zerolatency", 0);
    }

    if (0 != avcodec_open2(m_pH264CodecContext, m_pH264AvCodec, NULL))
    {
        return false;
    }

    m_pH264Packet = av_packet_alloc();
    if (Q_NULLPTR == m_pH264Packet)
    {
        return false;
    }

    m_pH264Frame = av_frame_alloc();
    if (Q_NULLPTR == m_pH264Frame)
    {
        return false;
    }

    return true;
}

void CVideoMatFrame2H264Data::UnInit()
{
    if(Q_NULLPTR != m_pH264DataBuf)
    {
        delete [] m_pH264DataBuf;
        m_pH264DataBuf = Q_NULLPTR;
    }

    if(Q_NULLPTR != m_pH264AvCodec)
    {
        m_pH264AvCodec = Q_NULLPTR;
    }

    if(Q_NULLPTR != m_pH264CodecContext)
    {
        avcodec_free_context(&m_pH264CodecContext);
        m_pH264CodecContext = Q_NULLPTR;
    }

    if(Q_NULLPTR != m_pH264Packet)
    {
        av_packet_free(&m_pH264Packet);
        m_pH264Packet = Q_NULLPTR;
    }

    if(Q_NULLPTR != m_pH264Frame)
    {
        av_frame_free(&m_pH264Frame);
        m_pH264Frame = Q_NULLPTR;
    }
}

void CVideoMatFrame2H264Data::GetH264DataFromMatFrame(Mat* pMatFrame,uint8_t*& pH264Data,int& iH264DataSize)
{
    if(Q_NULLPTR == pMatFrame || Q_NULLPTR == m_pH264Frame || Q_NULLPTR == m_pH264Packet)
    {
        return;
    }
    pH264Data = Q_NULLPTR;
    iH264DataSize = 0;

    m_pH264Frame->format = AV_PIX_FMT_YUV420P;
    m_pH264Frame->width = pMatFrame->cols;
    m_pH264Frame->height = pMatFrame->rows;
    if(0 != av_frame_get_buffer(m_pH264Frame, 0))
    {
        return;
    }

    if (0 != av_frame_make_writable(m_pH264Frame))
    {
        return;
    }

    cv::Mat matYuv;
    cv::cvtColor(*pMatFrame, matYuv, cv::COLOR_BGR2YUV_I420);
    int frame_size = pMatFrame->cols * pMatFrame->rows;
    static int index= 0;
    m_pH264Frame->data[0] = matYuv.data;
    m_pH264Frame->data[1] = matYuv.data + frame_size;
    m_pH264Frame->data[2] = matYuv.data + frame_size * 5 / 4;
    m_pH264Frame->pts = index++;

    encode(m_pH264CodecContext, m_pH264Frame, m_pH264Packet,m_pH264DataBuf,H264_DATA_BUF_SIZE,iH264DataSize);
    pH264Data = m_pH264DataBuf;
    matYuv.release();

    m_pH264Frame->data[0] = Q_NULLPTR;
    m_pH264Frame->data[1] = Q_NULLPTR;
    m_pH264Frame->data[2] = Q_NULLPTR;
    av_frame_unref(m_pH264Frame);
}

void CVideoMatFrame2H264Data::encode(AVCodecContext* pH264AvCodecContext, AVFrame* pH264Frame, AVPacket* pH264Packet,\
                                     uint8_t* pH264DataBuf,int iH264DataBufSize,int& iH264DataSize)
{
    int ret = avcodec_send_frame(pH264AvCodecContext, pH264Frame);
    if (ret < 0)
    {
        return;
    }

    iH264DataSize = 0;
    memset(pH264DataBuf,0,iH264DataBufSize);
    while (ret >= 0)
    {
        ret = avcodec_receive_packet(pH264AvCodecContext, pH264Packet);
        if (ret == AVERROR(EAGAIN))
        {
            return;
        }
        else if (ret == AVERROR_EOF)
        {
            return;
        }
        else if (ret < 0)
        {
            return;
        }

        if(iH264DataSize + pH264Packet->size < iH264DataBufSize)
        {
            memcpy(pH264DataBuf+iH264DataSize,pH264Packet->data,pH264Packet->size);
            iH264DataSize += pH264Packet->size;
        }
        av_packet_unref(pH264Packet);
    }
}

void CVideoMatFrame2H264Data::Flush(uint8_t*& pData,int& iDataSize)
{
    encode(m_pH264CodecContext, NULL, m_pH264Packet,m_pH264DataBuf,H264_DATA_BUF_SIZE,iDataSize);
    pData = m_pH264DataBuf;
}

