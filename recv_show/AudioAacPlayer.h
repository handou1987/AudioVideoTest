#ifndef _AUDIO_AAC_PLAYER_H
#define _AUDIO_AAC_PLAYER_H

#include <QThread>
#include <QAudioOutput>

class CAudioAacPlayer:public QThread
{    
public:
    CAudioAacPlayer();
    ~CAudioAacPlayer();

    bool Init();
    void UnInit();
    void SetRunFlag(bool bRun);

public slots:
    void OnThreadStart();

private:    
    bool m_bRun;
    QThread* m_qtThread;
    QAudioOutput* m_pAudioOutput;
    QIODevice* m_pStreamOut;
};

#endif
