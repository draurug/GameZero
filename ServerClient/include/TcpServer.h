#pragma once
#include "ClientSession.h"
#include <boost/asio.hpp>
#include <iostream>
#include <optional>

using boost::asio::ip::tcp;

class TcpServer {
public:
    TcpServer(boost::asio::io_context& io_context, unsigned short port)
    {
        boost::asio::ip::tcp::resolver resolver(io_context);
        auto endpoint = *resolver.resolve( "0.0.0.0",std::to_string( port )).begin();
        acceptor_ = boost::asio::ip::tcp::acceptor( io_context, endpoint );

        doAccept();
    }

    void onPacketReceived(boost::system::error_code ec, std::size_t length, uint8_t* data, std::shared_ptr<ClientSession> session)
    {
        if(!ec)
        {
            LOG("#PacketReceived");
            std::string message( data, data + length);
            LOG("Received message: " + message);
            session ->send("Acknowledged: " + message);
        } else {
            LOG("Error in onPacketReceived: " + ec.message());
        }
    }
private:
    void doAccept() {
        LOG("#DoAccept\n");

        acceptor_->async_accept(

            [this](boost::system::error_code ec, tcp::socket socket) {
                if (!ec) {
                    LOG("#New client connected!\n");
                    std::make_shared<ClientSession>(std::move(socket), *this)->start();
                }
                doAccept();
            });
    }

    std::optional<tcp::acceptor> acceptor_;
};

