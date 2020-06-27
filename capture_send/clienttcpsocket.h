#ifndef _CLIENT_TCP_SOCKET_H
#define _CLIENT_TCP_SOCKET_H

#include <QTcpSocket>

class CClientTcpSocket
{    
public:
    CClientTcpSocket();
    ~CClientTcpSocket();

    bool Init();
    void UnInit();

    bool Connect(char* pchIp,int iPort);
    bool WriteData(const char* pData,int iSize);

private:
    QTcpSocket* m_pTcpClient;
    bool m_bConSuccess;
};
#endif
