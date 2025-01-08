#pragma once
#include "TcpClient.h"

//todo:
// 1) Клиент здоровается с сервером: {0,<name>} -> {101}
// 2) Клиент requests server about active Clients: {1} -> {102, <clientName1>, <clientName2>,...}
// ----------------------------------------------------------------------
// 3) Client send message to <clientName>: {2,<clientName>,<text>}
// 4) Server send message to Client : -> {103,<fromClientName>,<text>}
// 5) Клиент прощается с сервером: {3}


class ChatClient : public TcpClient
{
public:
    ChatClient(boost::asio::io_context& io_context, std::function<void(const boost::system::error_code&, std::size_t, void* data)> handleResponse,
               std::function<void(const std::string&)> onMessageReceived)
        :TcpClient(io_context, handleResponse, onMessageReceived){}

    virtual void handlePacket(uint8_t* data, std::size_t length) override
    {

    }
};
