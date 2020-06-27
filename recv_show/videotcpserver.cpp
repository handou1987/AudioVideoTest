#include "videotcpserver.h"
#include "videoh264tomat.h"
#include "videomatqueue.h"

CVideoTcpServer::CVideoTcpServer(QObject *parent) : QObject(parent),
    m_pTcpServer(Q_NULLPTR),
    m_pClientSocket(Q_NULLPTR)
{

}

CVideoTcpServer::~CVideoTcpServer()
{
    UnInit();
}

void CVideoTcpServer::newClientConnect()
{
    m_pClientSocket = m_pTcpServer->nextPendingConnection();
    if(Q_NULLPTR != m_pClientSocket)
    {
        connect(m_pClientSocket,SIGNAL(readyRead()),this,SLOT(readMessage()));
        connect(m_pClientSocket,SIGNAL(disconnected()),this,SLOT(disConnect()));
    }
}

void CVideoTcpServer::readMessage()
{
#define INBUF_SIZE 4096
    static uint8_t inbuf[INBUF_SIZE + AV_INPUT_BUFFER_PADDING_SIZE];
    memset(inbuf,0,sizeof(inbuf));
    size_t data_size = (size_t)(m_pClientSocket->read((char*)inbuf,INBUF_SIZE));
    uint8_t* data = inbuf;
    if(data_size > 0)
    {
        CVideoH264ToMat::GetInstance()->GetMatFromH264Data(data,data_size);
    }
}

void CVideoTcpServer::disConnect()
{
    if(Q_NULLPTR != m_pClientSocket)
    {
        m_pClientSocket->close();
        m_pClientSocket = Q_NULLPTR;
    }
}

bool CVideoTcpServer::Init(int iPort)
{
    m_pTcpServer = new QTcpServer;
    if(Q_NULLPTR == m_pTcpServer)
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

void CVideoTcpServer::UnInit()
{
    if(Q_NULLPTR != m_pTcpServer)
    {
        m_pTcpServer->close();
        delete m_pTcpServer;
    }

    if(Q_NULLPTR != m_pClientSocket)
    {
        m_pClientSocket->close();
        m_pClientSocket = Q_NULLPTR;
    }
}
