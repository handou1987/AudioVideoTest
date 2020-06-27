#include "audiotcpserver.h"
#include "audioaacfifobuf.h"

CAudioTcpServer::CAudioTcpServer(QObject *parent) :
    QObject(parent),
    m_pTcpServer(Q_NULLPTR),
    m_pClientSocket(Q_NULLPTR)
{

}

CAudioTcpServer::~CAudioTcpServer()
{
    UnInit();
}

bool CAudioTcpServer::Init(int iPort)
{
    m_pTcpServer = new QTcpServer;
    m_pClientSocket = new QTcpSocket;

    if(Q_NULLPTR == m_pTcpServer || Q_NULLPTR == m_pClientSocket)
    {
        return false;
    }

    if(false == m_pTcpServer->listen(QHostAddress::Any,iPort))
    {
        return false;
    }

    connect(m_pTcpServer,SIGNAL(newConnection()),this,SLOT(newClientConnect()));
    return true;
}

void CAudioTcpServer::UnInit()
{
    if(Q_NULLPTR != m_pTcpServer)
    {
        m_pTcpServer->close();
        delete m_pTcpServer;
        m_pTcpServer = Q_NULLPTR;
    }

    if(Q_NULLPTR != m_pClientSocket)
    {
        m_pClientSocket->close();
        delete m_pClientSocket;
        m_pClientSocket = Q_NULLPTR;
    }
}

void CAudioTcpServer::newClientConnect()
{
    m_pClientSocket = m_pTcpServer->nextPendingConnection();
    if(Q_NULLPTR != m_pClientSocket)
    {
        connect(m_pClientSocket,SIGNAL(readyRead()),this,SLOT(readMessage()));
        connect(m_pClientSocket,SIGNAL(disconnected()),this,SLOT(disConnect()));
    }
}

void CAudioTcpServer::readMessage()
{
    CAudioAacFifoBuf* pFifoBuf = CAudioAacFifoBuf::GetInstance();
    if(Q_NULLPTR == pFifoBuf)
    {
        return;
    }

    static uint8_t inbuf[1024*10];
    memset(inbuf,0,sizeof(inbuf));
    size_t data_size = (size_t)(m_pClientSocket->read((char*)inbuf,sizeof(inbuf)));
    if(data_size > 0)
    {
        pFifoBuf->WriteData((char*)inbuf,data_size);
    }
}

void CAudioTcpServer::disConnect()
{
    if(Q_NULLPTR != m_pClientSocket)
    {
        m_pClientSocket->close();
        delete m_pClientSocket;
        m_pClientSocket = Q_NULLPTR;
    }
}
