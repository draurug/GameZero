#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <string>
#include "Logs.h"

using boost::asio::ip::tcp;

class ClientSession : public std::enable_shared_from_this<ClientSession> {
public:
    explicit ClientSession(tcp::socket socket)
        : socket_(std::move(socket))
    {
        boost::asio::socket_base::linger linger_option(true, 0);
        socket_.set_option(linger_option);
    }

    ~ClientSession()
    {
        LOG("#Client session destroyed");
    };
    void start() {
        doRead();
    }


private:
    void doRead() {
        auto self(shared_from_this());
        socket_.async_read_some(
            boost::asio::buffer(data_),
            [this, self](boost::system::error_code ec, std::size_t length) {
                if (!ec) {
                    LOG( "#Received from client: " << std::string(data_.data(), length) );
                    doWrite(length);
                }
            });
    }

    void doWrite(std::size_t length) {
        auto self(shared_from_this());
        LOG("#Before writing");
        boost::asio::async_write(
            socket_, boost::asio::buffer(data_, length),
            [this, self](boost::system::error_code ec, std::size_t /*length*/) {
                LOG("#Did write");
                if (!ec) {
                    doRead();
                }
            });
    }

    tcp::socket socket_;
    std::array<char, 1024> data_;
};
