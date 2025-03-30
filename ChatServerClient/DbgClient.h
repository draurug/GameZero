#pragma once

#include <boost/asio.hpp>
#include <string>
#include "Logs.h"
#include "MessageType.h"

using boost::asio::ip::tcp;

class DbgClient
{
protected:
    tcp::resolver m_resolver;
    tcp::socket m_socket;
    std::vector<char> m_packet;
    std::function<void(const boost::system::error_code&, std::size_t, void* data)> m_handleResponse;
    std::function<void(const std::string&)> m_messageCallback;

public:
    DbgClient(boost::asio::io_context& io_context,
              std::function<void(const boost::system::error_code&, std::size_t, void* data)> handleResponse,
              std::function<void(const std::string&)> messageCallback)
        : m_resolver(io_context), m_socket(io_context), m_handleResponse(handleResponse), m_messageCallback(messageCallback) {}

    virtual ~DbgClient() = default;

    void connect(const std::string& host, const std::string& port,
                 const std::function<void(const boost::system::error_code& ec, const tcp::endpoint&)>& func)
    {
        LOG("Connecting to " << host << ":" << port);
        auto endpoints = m_resolver.resolve(host, port);
        boost::asio::async_connect(m_socket, endpoints,
                                   [this, func](const boost::system::error_code& ec, const tcp::endpoint& endpoint)
                                   {
                                       if (!ec){
                                           LOG("Connected to server.");
                                       } else {
                                           LOG("Connection failed: " << ec.message());
                                       }
                                       func(ec, endpoint);
                                   });
    }

    void processReceivedMessage(uint8_t* data, std::size_t length)
    {
        if (length < 2) return;

        uint8_t packetType = data[0];
        if (packetType != ClSendMessage) return;

        auto separator = std::find(data + 1, data + length, '\0');
        if (separator == data + length) return;

        std::string recipient(reinterpret_cast<char*>(data + 1), separator - (data + 1));
        std::string message(reinterpret_cast<char*>(separator + 1), data + length - (separator + 1));

        LOG("DbgClient received message from [" << recipient << "]: " << message);

        sendMessage("DbgClient", message);

        if (m_messageCallback)
        {
            m_messageCallback(message);
        }
    }

    void sendMessage(const std::string& recipient, const std::string& message)
    {

        std::vector<uint8_t> packet = {static_cast<uint8_t>(ClSendMessage)};
        packet.insert(packet.end(), recipient.begin(), recipient.end());
        packet.push_back('\0');
        packet.insert(packet.end(), message.begin(), message.end());

        sendPacket(packet);
    }

    void sendPacket(const std::vector<uint8_t>& m_packet)
    {
        boost::asio::async_write(m_socket, boost::asio::buffer(m_packet),
                                 [this, &m_packet](const boost::system::error_code& ec, std::size_t length)
                                 {
                                     if (ec)
                                     {
                                         LOG("*Write error: " << ec.message());
                                     }
                                     else
                                     {
                                         LOG("*Message sent: " << std::string(m_packet.begin() + 2, m_packet.end()));
                                     }
                                     m_handleResponse(ec, length, (void*)m_packet.data());
                                 });
    }
};
