#ifndef _VIDEO_FRAME_DRAW_QUEUE_H
#define _VIDEO_FRAME_DRAW_QUEUE_H

#include "opencv2/core/mat.hpp"
using namespace cv;

#include <queue>
using namespace std;

#include <QMutex>

class CVideoFrameDrawQueue
{
private:
    CVideoFrameDrawQueue();
public:
    ~CVideoFrameDrawQueue();

    static CVideoFrameDrawQueue* GetInstance();
    bool Init();
    void UnInit();

    Mat* GetFree();
    void GiveBackFree(Mat* pFreeMat);

    void AddUsing(Mat* pUsingMat);
    Mat* GetUsing();

private:
    std::queue<Mat*> m_freeQueue;
    std::queue<Mat*> m_usingQueue;

    QMutex m_mutexFree;
    QMutex m_mutexUsing;
};

#endif
