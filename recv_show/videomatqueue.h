#ifndef _VIDEO_MAT_QUEUE_H
#define _VIDEO_MAT_QUEUE_H

#include "opencv2/core/mat.hpp"
using namespace cv;

#include <queue>
using namespace std;

#include <QMutex>

class CVideoMatQueue
{
private:
    CVideoMatQueue();
public:
    ~CVideoMatQueue();

    static CVideoMatQueue* GetInstance();
    bool Init();
    void UnInit();

    Mat* GetFree();
    void GiveBackFree(Mat*);

    void AddUsing(Mat*);
    Mat* GetUsing();
private:
    std::queue<Mat*> m_freeQueue;
    std::queue<Mat*> m_usingQueue;

    QMutex m_mutexFree;
    QMutex m_mutexUsing;
};

#endif
