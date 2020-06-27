#ifndef _VIDEO_FRAME_DRAW_THREAD_H
#define _VIDEO_FRAME_DRAW_THREAD_H

#include <QThread>

#include "opencv2/core/mat.hpp"
using namespace cv;

#include "widgetvideodisplay.h"

class CVideoFrameDrawThread:public QThread
{
    Q_OBJECT

public:
    CVideoFrameDrawThread();
    ~CVideoFrameDrawThread();

    static QImage Mat2QImage(Mat cvImg);

    bool Init(QWidget* videoDisplayWidget);
    void UnInit();
private:
    void SetRunFlag(bool bRun);

public slots:
    void OnThreadStart();

private:
    bool m_bRun;
    QThread* m_pQtThread;
    CWidgetVideoDisplay* m_pVideoDisplayWidget;
};

#endif
