#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include "Logs.h"

using boost::asio::ip::tcp;

class TcpClient {
public:
    TcpClient(boost::asio::io_context& io_context)
        : resolver_(io_context), socket_(io_context) {}

    void connect(const std::string& host, const std::string& port) {
        auto endpoints = resolver_.resolve(host, port);

        boost::asio::async_connect(
            socket_, endpoints,
            [this](const boost::system::error_code& ec, const tcp::endpoint&) {
                if (ec) {
                    LOG("Connection error: " << ec.message());
                } else {
                    LOG("Connected successfully!");
                    doWrite();
                }
            });
    }

private:
    void doWrite() {
        std::string message = "Hello, Server!";
        boost::asio::async_write(
            socket_, boost::asio::buffer(message),
            [this](const boost::system::error_code& ec, std::size_t /*length*/) {
                if (ec) {
                    LOG("Write error: " << ec.message());
                } else {
                    LOG("Message sent: Hello, Server!");
                    doRead();
                }
            });
    }

    void doRead() {
        boost::asio::async_read(
            socket_, boost::asio::buffer(data_),
            [this](const boost::system::error_code& ec, std::size_t length) {
                if (ec) {
                    LOG("Read error: " << ec.message());
                } else {
                    LOG("Message received from server: "
                        << std::string(data_.data(), length));
                }
            });
    }

    tcp::resolver resolver_;
    tcp::socket socket_;
    std::array<char, 1024> data_;
};
