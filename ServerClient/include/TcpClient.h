#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include "Logs.h"

using boost::asio::ip::tcp;

class TcpClient {
public:
    TcpClient(boost::asio::io_context& io_context)
        : resolver_(io_context), socket_(io_context) {}

    void connect(const std::string& host, const std::string& port,
                const std::function<void(const boost::system::error_code& ec, const tcp::endpoint&)> &func) {
        auto endpoints = resolver_.resolve(host, port);

        boost::asio::async_connect( socket_, endpoints, func );
    }

    void close()
    {
        socket_.close();
    }

    void doWrite() {
        std::string message = "Hello, Server";
        boost::asio::async_write(
            socket_, boost::asio::buffer(message),
            [this](const boost::system::error_code& ec, std::size_t /*length*/) {
                if (ec) {
                    LOG("Write error: " << ec.message());
                } else {
                    LOG("Message sent: Hello, Server!\n");
                    doRead();
                }
            });
    }

    void doRead() {
        socket_.async_read_some(
            boost::asio::buffer(data_),
            [this](const boost::system::error_code& ec, std::size_t length) {
                if (ec) {
                    LOG("Read error: " << ec.message());
                } else {
                    LOG("Message received from server: "
                        << std::string(data_.data(), length));
                }
//todo must be removed
                close();
            });
    }

    tcp::resolver resolver_;
    tcp::socket socket_;
    std::array<char, 1024> data_;
};
