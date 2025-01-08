#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <vector>
#include <string>
#include "Logs.h"

class TcpServer;
using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session>
{
    tcp::socket m_socket;
    std::vector<uint8_t> m_data;
    TcpServer& m_server;

public:
    explicit Session(tcp::socket socket, TcpServer& server )
        : m_socket(std::move(socket)), m_server(server)
    {
        boost::asio::socket_base::linger linger_option(true, 0);
        m_socket.set_option(linger_option);
    }

    ~Session()
    {
        LOG("#Client session destroyed");
    }

    void start()
    {
        doRead();
    }

    void send(const std::string& message);
    void doRead();
private:

    void doWrite(std::size_t length);
    void notifyServer(boost::system::error_code ec, std::size_t length, uint8_t* data);
};
