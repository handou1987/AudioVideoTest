#ifndef _VIDEO_CAPTURE_H
#define _VIDEO_CAPTURE_H

#include <QThread>

#include "opencv2/videoio.hpp"
using namespace cv;

class CVideoCapture:public QThread
{
    Q_OBJECT
public:
    CVideoCapture();
    ~CVideoCapture();

    bool Init();
    void UnInit();

private:
    bool OpenCamera();
    void CloseCamera();
    bool StartCaptureThread();
    void StopCaptureThread();
    void SetRunFlag(bool bRun);

public slots:
    void OnThreadStart();

private:
    VideoCapture* m_pVideoCapture;
    QThread* m_pQtThread;
    bool m_bRun;
};

#endif
