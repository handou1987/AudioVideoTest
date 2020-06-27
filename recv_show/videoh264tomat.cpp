#include "videoh264tomat.h"
#include "videomatqueue.h"

CVideoH264ToMat::CVideoH264ToMat():
    m_pAvCodec(Q_NULLPTR),
    m_pParserContext(Q_NULLPTR),
    m_pCodecContext(Q_NULLPTR),
    m_pFrame(Q_NULLPTR),
    m_pPacket(Q_NULLPTR)
{

}

CVideoH264ToMat::~CVideoH264ToMat()
{
    UnInit();
}

bool CVideoH264ToMat::Init()
{
    m_pPacket = av_packet_alloc();
    if (Q_NULLPTR == m_pPacket)
    {
        return false;
    }

    m_pAvCodec = avcodec_find_decoder(AV_CODEC_ID_H264);
    if (Q_NULLPTR == m_pAvCodec)
    {
        return false;
    }

    m_pParserContext = av_parser_init(m_pAvCodec->id);
    if (Q_NULLPTR == m_pParserContext)
    {
        return false;
    }

    m_pCodecContext = avcodec_alloc_context3(m_pAvCodec);
    if (Q_NULLPTR == m_pParserContext)
    {
        return false;
    }

    if (avcodec_open2(m_pCodecContext, m_pAvCodec, NULL) < 0)
    {
        return false;
    }

    m_pFrame = av_frame_alloc();
    if (Q_NULLPTR == m_pFrame)
    {
        return false;
    }

    return true;
}

void CVideoH264ToMat::UnInit()
{
    if(Q_NULLPTR != m_pParserContext)
    {
        av_parser_close(m_pParserContext);
        m_pParserContext = Q_NULLPTR;
    }

    if(Q_NULLPTR != m_pCodecContext)
    {
        avcodec_free_context(&m_pCodecContext);
        m_pCodecContext = Q_NULLPTR;
    }

    if(Q_NULLPTR != m_pFrame)
    {
        av_frame_free(&m_pFrame);
        m_pFrame = Q_NULLPTR;
    }

    if(Q_NULLPTR != m_pPacket)
    {
        av_packet_free(&m_pPacket);
        m_pPacket = Q_NULLPTR;
    }
}

void CVideoH264ToMat::GetMatFromH264Data(uint8_t* pData,int iDataSize)
{
    int data_size = iDataSize;
    uint8_t* data = pData;
    while (data_size > 0)
    {
        int ret = av_parser_parse2(m_pParserContext, m_pCodecContext, &m_pPacket->data, &m_pPacket->size,
                                   data, data_size, AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);
        if (ret < 0)
        {
            exit(1);
        }
        data      += ret;
        data_size -= ret;

        if (m_pPacket->size)
        {
            decode(m_pCodecContext, m_pFrame, m_pPacket);
        }
    }
}

void CVideoH264ToMat::decode(AVCodecContext *dec_ctx, AVFrame *frame, AVPacket *pkt)
{
    char buf[1024];
    int ret;

    ret = avcodec_send_packet(dec_ctx, pkt);
    if (ret < 0)
    {
        fprintf(stderr, "Error sending a packet for decoding\n");
        return ;
    }

    while (ret >= 0)
    {
        ret = avcodec_receive_frame(dec_ctx, frame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
            return ;
        else if (ret < 0)
        {
            return ;
        }

        if(AV_PICTURE_TYPE_I != frame->pict_type)
        {
            //return ;
        }

        fflush(stdout);
        int w = frame->width;
        int h = frame->height;

        Mat* mat = CVideoMatQueue::GetInstance()->GetFree();
        if(Q_NULLPTR != mat)
        {
            struct SwsContext *sws_ctx = NULL;
            sws_ctx = sws_getContext(frame->width, frame->height, (enum AVPixelFormat)frame->format,
                                     w, h, AV_PIX_FMT_BGR24, SWS_BICUBIC, NULL, NULL, NULL);


            mat->create(cv::Size(w, h), CV_8UC3);
            AVFrame *bgr24frame = av_frame_alloc();
            bgr24frame->data[0] = (uint8_t *)mat->data;
            avpicture_fill((AVPicture *)bgr24frame, bgr24frame->data[0], AV_PIX_FMT_BGR24, w, h);
            sws_scale(sws_ctx,
                      (const uint8_t* const*)frame->data, frame->linesize,
                      0, frame->height,
                      bgr24frame->data, bgr24frame->linesize);

            av_free(bgr24frame);
            sws_freeContext(sws_ctx);

            CVideoMatQueue::GetInstance()->AddUsing(mat);
        }
    }
}

CVideoH264ToMat* CVideoH264ToMat::GetInstance()
{
    static CVideoH264ToMat obj;
    return &obj;
}
