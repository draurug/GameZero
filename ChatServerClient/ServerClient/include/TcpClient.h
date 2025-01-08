#pragma once

#include <boost/asio.hpp>
#include <string>
#include "Logs.h"

using boost::asio::ip::tcp;

class TcpClient
{
protected:
    tcp::resolver m_resolver;
    tcp::socket m_socket;
    std::array<char, 1024> m_data;
    std::vector<char> m_packet;
    std::function<void(const boost::system::error_code&, std::size_t, void* data)> m_handleResponse;
    std::function<void(const std::string&)> onMessageReceivedCallback;

public:
    TcpClient(boost::asio::io_context& io_context,
              std::function<void(const boost::system::error_code&, std::size_t, void* data)> handleResponse,
              std::function<void(const std::string&)> onMessageReceived)
        : m_resolver(io_context), m_socket(io_context), m_handleResponse(handleResponse),onMessageReceivedCallback(onMessageReceived){}

    virtual ~TcpClient()=default;

    void connect(const std::string& host, const std::string& port,
                 const std::function<void(const boost::system::error_code& ec, const tcp::endpoint&)> &func)
    {
        auto endpoints = m_resolver.resolve(host, port);
        boost::asio::async_connect( m_socket, endpoints, func );
    }

    void close()
    {
        try
        {
            m_socket.close();
        }
        catch(...){}
    }

    void doRead()
    {
        m_socket.async_read_some(
            boost::asio::buffer(m_data),
            [this](const boost::system::error_code& ec, std::size_t length)
            {
                if (ec)
                {
                    LOG("Read error: " << ec.message());
                    return;
                }

                // Обработка полученного сообщения
                std::string message(m_data.data(), length);
                LOG("Message received from server: " << message);

                // Вызываем пользовательский обработчик сообщения
                if (onMessageReceivedCallback)
                { onMessageReceivedCallback(message); }

                // Повторяем чтение для следующего сообщения
                doRead();
            });
    }

    virtual void handlePacket(uint8_t* data, std::size_t length)=0;

    //send function adding
    void send(std::string message)
    {
        // Формирование пакета с 2 байтами длины в начале
        uint16_t length = static_cast<uint16_t>(message.size());
        //m_packet.clear();
        m_packet.reserve(2 + message.size());

        // Добавляем длину пакета в начало (little-endian)
        m_packet.push_back(static_cast<char>((length) & 0xFF));
        m_packet.push_back(static_cast<char>((length >> 8) &0xFF));

        // Добавляем само сообщение
        m_packet.insert(m_packet.end(), message.begin(), message.end());

        // Отправляем сформированный пакет
        boost::asio::async_write(
            m_socket, boost::asio::buffer(m_packet),
            [this](const boost::system::error_code& ec, std::size_t length)
            {
                if (ec)
                {
                    LOG("Write error: " << ec.message());
                    m_handleResponse(ec, length, m_packet.data());
                }
                else
                {
                    LOG("Message sent: " << std::string(m_packet.begin() + 2, m_packet.end()));
                    m_handleResponse(ec, length, m_packet.data());
                }
            });
    }
};
