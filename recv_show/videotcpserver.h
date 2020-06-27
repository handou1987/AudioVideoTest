#ifndef _VIDEO_TCP_SERVER_H
#define _VIDEO_TCP_SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class CVideoTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit CVideoTcpServer(QObject *parent = nullptr);
    ~CVideoTcpServer();

    bool Init(int iPort);
    void UnInit();

public slots:
    void newClientConnect();
    void readMessage();
    void disConnect();

private:
    QTcpServer *m_pTcpServer;
    QTcpSocket *m_pClientSocket;
};

#endif

