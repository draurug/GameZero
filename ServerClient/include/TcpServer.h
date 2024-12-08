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

private:
    void doAccept() {
        std::cout << "DoAccept\n";

        acceptor_->async_accept(

            [this](boost::system::error_code ec, tcp::socket socket) {
                if (!ec) {
                    std::cout << "New client connected!";
                    std::make_shared<ClientSession>(std::move(socket))->start();
                }
                doAccept();
            });
    }

    std::optional<tcp::acceptor> acceptor_;
};

