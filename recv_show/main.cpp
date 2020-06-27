#include <QApplication>

#include "mainwindow.h"

#include "videotcpserver.h"
#include "videoh264tomat.h"
#include "videomatqueue.h"

#include "audiotcpserver.h"
#include "audioaacfifobuf.h"
#include "audioaacplayer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    CVideoTcpServer videoTcpServer;
    if(false == videoTcpServer.Init(6789))
    {
        return 0;
    }

    if(false == CVideoH264ToMat::GetInstance()->Init())
    {
        return 0;
    }

    if(false == CVideoMatQueue::GetInstance()->Init())
    {
        return 0;
    }

    CAudioTcpServer audioTcpSvr;
    if(false == audioTcpSvr.Init(9876))
    {
        return 0;
    }

    if(false == CAudioAacFifoBuf::GetInstance()->Init())
    {
        return 0;
    }

    CAudioAacPlayer audioAacPlayer;
    if(false == audioAacPlayer.Init())
    {
        return 0;
    }

    MainWindow w;
    w.show();
    return a.exec();
}
