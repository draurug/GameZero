#pragma once

#include "Logs.h"
#include "Session.h"
#include <boost/asio.hpp>
#include <optional>

using boost::asio::ip::tcp;

class TcpServer
{
    std::optional<tcp::acceptor> m_acceptor;
public:
    TcpServer(boost::asio::io_context& io_context, unsigned short port)
    {
        boost::asio::ip::tcp::resolver resolver(io_context);
        auto endpoint = *resolver.resolve("0.0.0.0",std::to_string(port)).begin();
        m_acceptor = boost::asio::ip::tcp::acceptor(io_context, endpoint);

        doAccept();
    }

    virtual ~TcpServer()=default;

    void onPacketReceived(boost::system::error_code ec, std::size_t length, uint8_t* data, std::shared_ptr<Session> session)
    {
        if(!ec)
        {
            handlePacket(data, length, session);
            LOG("#PacketReceived");
            std::string message(data, data + length);
            LOG("Received message: " + message);
            session ->send("Acknowledged: " + message);
        }
        else
        {
            LOG("Error in onPacketReceived: " + ec.message());
        }
    }

    virtual void handlePacket(uint8_t* data, std::size_t length, std::shared_ptr<Session> session)=0;

private:
    void doAccept()
    {
        LOG("#DoAccept\n");

        m_acceptor->async_accept(

            [this](boost::system::error_code ec, tcp::socket socket)
            {
                if (!ec)
                {
                    LOG("#New client connected!\n");
                    std::make_shared<Session>(std::move(socket), *this)->start();
                }
                doAccept();
            });
    }
};

