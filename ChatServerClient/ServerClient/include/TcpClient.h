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

public:
    TcpClient(boost::asio::io_context& io_context,
              std::function<void(const boost::system::error_code&, std::size_t, void* data)> handleResponse)
        : m_resolver(io_context), m_socket(io_context), m_handleResponse(handleResponse){}

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
        // Этап 1: Читаем длину пакета (2 байта)
        m_packet.resize(2);
        boost::asio::async_read(m_socket, boost::asio::buffer(m_packet),
                               [this](boost::system::error_code ec, std::size_t length)
                                {
                                    if (!ec && length == 2)
                                    {
                                        // Интерпретируем длину пакета
                                        uint16_t packetLength = static_cast<uint8_t>(m_packet[0]) |
                                                                (static_cast<uint8_t>(m_packet[1]) << 8);

                                        if (packetLength > 0 && packetLength <= 0xFFFF)
                                        {
                                            LOG("#messageLength is: " << packetLength);
                                            // Этап 2: Читаем содержимое пакета
                                            doReadPacket(packetLength);
                                        }
                                        else
                                        {
                                            LOG("#Invalid packet length: " << packetLength);
                                            // Читаем следующий пакет, игнорируя текущий
                                            doRead();
                                        }
                                    }
                                    else
                                    {
                                        LOG("#Error reading packet length: " << ec.message());
                                    }
                                });
    }

    void doReadPacket(std::size_t packetLength)
    {
        m_packet.resize(packetLength);

        boost::asio::async_read(m_socket, boost::asio::buffer(m_packet),
                               [this](boost::system::error_code ec, std::size_t length)
                                {
                                    if (!ec && length == m_packet.size())
                                    {
                                       // std::string receivedData(m_packet.data(), m_packet.data() + length);
                                        LOG("Packet received from server: " << length);
                                        handlePacketFromServer(reinterpret_cast<uint8_t*>(m_packet.data()), length);
                                        doRead();
                                    }
                                    else
                                    {
                                        LOG("Error reading packet data: " << ec.message());
                                    }
                                });
    }


    virtual void handlePacketFromServer(uint8_t* data, std::size_t length)=0;

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

        LOG("#1 in dowrite" << m_packet.size());

        // Отправляем сформированный пакет
        boost::asio::async_write(m_socket, boost::asio::buffer(m_packet),
                                [this](const boost::system::error_code& ec, std::size_t length)
                                {
                                     LOG("#2 dowrite" << length);
                                    if (ec)
                                    {
                                        LOG("#Write error: " << ec.message());
                                        m_handleResponse(ec, length, m_packet.data());
                                    }
                                    else
                                    {
                                        LOG("#Message sent: " << std::string(m_packet.begin() + 2, m_packet.end()));
                                        m_handleResponse(ec, length, m_packet.data());
                                    }
                                });
    }
};
