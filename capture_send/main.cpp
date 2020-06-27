#include <QApplication>
#include <QDebug>

#include "audiocapture.h"
#include "audiosendmodule.h"
#include "audiopcmfifobuf.h"

#include "mainwindow.h"
#include "videocapture.h"
#include "videoframedrawqueue.h"
#include "videoframesendqueue.h"
#include "videoframesendmodule.h"
#include "clienttcpsocket.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if(false == CVideoFrameDrawQueue::GetInstance()->Init())
    {
        return 0;
    }

    if(false == CVideoFrameSendQueue::GetInstance()->Init())
    {
        return 0;
    }

    if(false == CAudioPcmFifoBuf::GetInstance()->Init())
    {
        return 0;
    }

    CAudioSendModule audioSendMoudle;
    audioSendMoudle.Init();

    CVideoFrameSendModule videoFrameSendModule;
    videoFrameSendModule.Init();

    CAudioCapture CAudioCapture;
    CAudioCapture.Init();

    MainWindow w;
    w.show();

    CVideoCapture videocapture;
    videocapture.Init();

    return a.exec();
}
