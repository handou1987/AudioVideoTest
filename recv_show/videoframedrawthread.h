#ifndef _DRAW_THREAD_H
#define _DRAW_THREAD_H

#include <QThread>

#include "opencv2/core/mat.hpp"
using namespace cv;

#include "videodisplaywidget.h"

class CVideoFrameDrawThread:public QThread
{    
    Q_OBJECT
public:
    CVideoFrameDrawThread();
    ~CVideoFrameDrawThread();

    bool Init(CVideoDisplayWidget* pVideoWidget);
    void UnInit();

public slots:
    void OnThreadStart();

private:
    void SetRunFlag(bool bRun);

private:    
    bool m_bRun;
    QThread* m_pQtThread;
    CVideoDisplayWidget* m_pVideoWidget;
};

#endif
