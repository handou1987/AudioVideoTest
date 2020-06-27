#include "clienttcpsocket.h"
#include <QHostAddress.h>

CClientTcpSocket::CClientTcpSocket():m_pTcpClient(Q_NULLPTR),
    m_bConSuccess(false)
{

}

CClientTcpSocket::~CClientTcpSocket()
{
    UnInit();
}

bool CClientTcpSocket::Init()
{
    m_pTcpClient = new QTcpSocket();
    if(Q_NULLPTR == m_pTcpClient)
    {
        return false;
    }

    return true;
}

void CClientTcpSocket::UnInit()
{
    if(Q_NULLPTR != m_pTcpClient)
    {
        m_pTcpClient->close();
        delete m_pTcpClient;
        m_pTcpClient = Q_NULLPTR;
    }
    m_bConSuccess = false;
}

bool CClientTcpSocket::Connect(char* pchIp,int iPort)
{
    if(Q_NULLPTR == pchIp || iPort <= 0)
    {
        return false;
    }

    m_pTcpClient->connectToHost(QHostAddress(pchIp), iPort);
    m_bConSuccess =  m_pTcpClient->waitForConnected();
    return m_bConSuccess;
}

bool CClientTcpSocket::WriteData(const char* pData,int iSize)
{
    if(Q_NULLPTR == pData || iSize <= 0 || Q_NULLPTR == m_pTcpClient || false == m_bConSuccess)
    {
        return false;
    }

    int iCurrWrite = 0;
    int iTotalWrite = 0;
    while(iTotalWrite != iSize)
    {
        iCurrWrite = m_pTcpClient->write(pData+iTotalWrite,iSize-iTotalWrite);
        if(-1 == iCurrWrite)
        {
            return false;
        }
        iTotalWrite += iCurrWrite;
    }
    m_pTcpClient->flush();
    return true;
}

