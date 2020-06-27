#ifndef _AUDIO_CAPTURE_H
#define _AUDIO_CAPTURE_H

extern "C"
{
#include "libavformat/avformat.h"
#include "libavdevice/avdevice.h"
}

#include <QThread>

class CAudioCapture:public QThread
{
    Q_OBJECT
public:
    CAudioCapture();
    ~CAudioCapture();

    bool Init();
    void UnInit();

public slots:
    void OnThreadStart();

private:
    void SetRunFlag(bool bRun);

private:     
    QThread* m_pQtThread;
    bool m_bRun;
    AVFormatContext* m_pAudioFormatCtx;
};

#endif
