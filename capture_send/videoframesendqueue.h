#ifndef _VIDEO_FRAME_SEND_QUEUE_H
#define _VIDEO_FRAME_SEND_QUEUE_H

#include "opencv2/core/mat.hpp"
using namespace cv;

#include <queue>
using namespace std;

#include <QMutex>

class CVideoFrameSendQueue
{
private:
    CVideoFrameSendQueue();
public:
    ~CVideoFrameSendQueue();

    static CVideoFrameSendQueue* GetInstance();
    bool Init();
    void UnInit();

    Mat* GetFree();
    void GiveBackFree(Mat* pMatFree);

    void AddSend(Mat* pMatSend);
    Mat* GetSend();
private:
    std::queue<Mat*> m_freeQueue;
    std::queue<Mat*> m_sendQueue;

    QMutex m_mutexFree;
    QMutex m_mutexSend;
};

#endif
