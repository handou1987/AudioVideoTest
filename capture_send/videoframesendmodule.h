#ifndef _VIDEO_FRAME_SENDMODULE_H
#define _VIDEO_FRAME_SENDMODULE_H

#include <QThread>

class CVideoFrameSendModule:public QThread
{
    Q_OBJECT

public:
    CVideoFrameSendModule();
    ~CVideoFrameSendModule();

    bool Init();
    void UnInit();

private:
    void SetRunFlag(bool bRun);

public slots:
    void OnThreadStart();

private:
    QThread* m_pQtThread;
    bool m_bRun;
};

#endif
