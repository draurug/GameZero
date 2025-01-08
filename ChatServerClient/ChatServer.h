#pragma once
#include "TcpServer.h"
#include "Session.h"
#include <map>

//todo:
// 1) Клиент здоровается с сервером: {0,<name>} -> {101}
// 2) Клиент requests server about active Clients: {1} -> {102, <clientName1>, <clientName2>,...}
// ----------------------------------------------------------------------
// 3) Client send message to <clientName>: {2,<clientName>,<text>}
// 4) Server send message to Client : -> {103,<fromClientName>,<text>}
// 5) Клиент прощается с сервером: {3}

class ChatServer : public TcpServer
{
    int m_nextClientId = 0;
    std::map<int, std::shared_ptr<Session>> m_ClientsMap;
    //shared_ptr if needed
public:
    ChatServer(boost::asio::io_context& io_context, unsigned short port)
        : TcpServer(io_context, port) {}

    virtual void handlePacket(uint8_t* data, std::size_t length, std::shared_ptr<Session> session) override
    {

    }

    void onClientDisconnected(int clientId)
    {
        LOG("Client " << clientId << " disconnected");
        m_ClientsMap.erase(clientId);
    }

};
