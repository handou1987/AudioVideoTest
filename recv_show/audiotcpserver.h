#ifndef _AUDIO_TCP_SERVER_
#define _AUDIO_TCP_SERVER_

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class CAudioTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit CAudioTcpServer(QObject *parent = nullptr);
    ~CAudioTcpServer();

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
