#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <string>
#include "Logs.h"

using boost::asio::ip::tcp;

class ClientSession : public std::enable_shared_from_this<ClientSession>
{
public:
    explicit ClientSession(tcp::socket socket)
        : m_socket(std::move(socket))
    {
        boost::asio::socket_base::linger linger_option(true, 0);
        m_socket.set_option(linger_option);
    }

    ~ClientSession()
    {
        LOG("#Client session destroyed");
    };

    void start()
    {
        doRead();
    }


private:

    tcp::socket m_socket;
    std::array<char, 1024> m_data;

    void doRead() {
        auto self(shared_from_this());
        //посмотреть реализацию в Packet версии и аналог создать здесь (вначале читаем длину, а потом весь пакет)
        //добавить связь с сервером, чтобы оповещать сервер о получении сообщения
        // аналогично сделать для клиента
        m_socket.async_read_some(
            boost::asio::buffer(m_data),
            [this, self](boost::system::error_code ec, std::size_t length)
            {
                if (!ec) {
                    LOG( "#Received from client: " << std::string(m_data.data(), length) );
                    doWrite(length); //
                }
            });
    }

    void doWrite(std::size_t length)
    {
        auto self(shared_from_this());
        LOG("#Before writing");
        boost::asio::async_write(
            m_socket, boost::asio::buffer(m_data, length),
            [this, self](boost::system::error_code ec, std::size_t /*length*/)
            {
                LOG("#Did write");
                if (!ec)
                {
                    doRead();
                }
            });
    }
};
