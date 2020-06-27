#ifndef _VIDEO_H264_TO_MAT_H
#define _VIDEO_H264_TO_MAT_H

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavutil/opt.h"
#include "libavutil/imgutils.h"
#include "libavformat\avformat.h"
#include "libavutil\pixfmt.h"
#include "libswscale/swscale.h"
}

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
using namespace cv;

class CVideoH264ToMat
{
private:
    CVideoH264ToMat();

public:
    ~CVideoH264ToMat();

    static CVideoH264ToMat* GetInstance();

    bool Init();
    void UnInit();

    void GetMatFromH264Data(uint8_t* pData,int iDataSize);

private:
    void decode(AVCodecContext *dec_ctx, AVFrame *frame, AVPacket *pkt);

private:    
    AVCodec* m_pAvCodec;
    AVCodecParserContext* m_pParserContext;
    AVCodecContext* m_pCodecContext;
    AVPacket* m_pPacket;
    AVFrame* m_pFrame;

};
#endif
