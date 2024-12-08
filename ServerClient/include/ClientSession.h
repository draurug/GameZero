#pragma once
#pragma once
#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using boost::asio::ip::tcp;

class ClientSession : public std::enable_shared_from_this<ClientSession> {
public:
    explicit ClientSession(tcp::socket socket)
        : socket_(std::move(socket)) {}

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
                    std::cout << "Received from client: " << std::string(data_.data(), length) << std::endl;
                    doWrite(length);
                }
            });
    }

    void doWrite(std::size_t length) {
        auto self(shared_from_this());
        boost::asio::async_write(
            socket_, boost::asio::buffer(data_, length),
            [this, self](boost::system::error_code ec, std::size_t /*length*/) {
                if (!ec) {
                    doRead();
                }
            });
    }

    tcp::socket socket_;
    std::array<char, 1024> data_;
};
