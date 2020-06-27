#ifndef _AUDIO_SENDMODULE_H
#define _AUDIO_SENDMODULE_H

#include <QThread>

class CAudioSendModule:public QThread
{
    Q_OBJECT

public:
    CAudioSendModule();
    ~CAudioSendModule();

    bool Init();
    void UnInit();

public slots:
    void OnThreadStart();

private:
    void SetRunFlag(bool bRun);

private:
    QThread* m_pQtThread;
    bool m_bRun;
};

#endif
