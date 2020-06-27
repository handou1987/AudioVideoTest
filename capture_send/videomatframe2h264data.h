#ifndef _VIDEO_MAT_FRAME_2_H264DATA_
#define _VIDEO_MAT_FRAME_2_H264DATA_

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavutil/opt.h"
#include "libavutil/imgutils.h"
#include "libavformat/avformat.h"
#include "libavutil/pixfmt.h"
}

#include "opencv2/imgproc.hpp"
using namespace cv;

class CVideoMatFrame2H264Data
{
public:
    CVideoMatFrame2H264Data();
    ~CVideoMatFrame2H264Data();

    bool Init();
    void UnInit();

    //后两个参数为结果带出
    void GetH264DataFromMatFrame(Mat* pMatFrame,uint8_t*& pH264Data,\
                                 int& iH264DataSize);
    void Flush(uint8_t*& pData,int& iDataSize);

private:
    static void encode(AVCodecContext* pH264AvCodecContext, AVFrame* pH264Frame,\
                       AVPacket* pH264Packet,uint8_t* pH264DataBuf,\
                       int iH264DataBufSize,int& iH264DataSize);

private:
    uint8_t* m_pH264DataBuf;
    AVCodec* m_pH264AvCodec;
    AVCodecContext* m_pH264CodecContext;
    AVPacket* m_pH264Packet;
    AVFrame* m_pH264Frame;
};
#endif
