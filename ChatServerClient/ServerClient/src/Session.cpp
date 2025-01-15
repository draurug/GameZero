#include <boost/asio.hpp>
#include <memory>
#include <vector>
#include <string>
#include "Session.h"
#include "Logs.h"
#include "TcpServer.h"

// void Session::doRead()
// {
//         auto self(shared_from_this());
//         m_packet.resize(1024);
//         //вначале прочесть длину пакета, а потом сам пакет -todo
//         m_socket.async_read_some( boost::asio::buffer(m_packet),
//                                 [this, self](boost::system::error_code ec, std::size_t length)
//         {
//             // Оповещаем сервер о получении пакета
//             notifyServer(ec, length, reinterpret_cast<uint8_t*>(m_packet.data()));

//             if (!ec)
//             {
//                 std::string received_data(m_packet.data(), m_packet.data() + length);
//                 LOG("#Received from client: " << received_data);
//                 doRead();
//             }
//             else
//             {
//                 LOG("Error in doRead: " + ec.message());
//             }
//         });
// }

void Session::doRead()
{
    auto self(shared_from_this());

    // Этап 1: Читаем длину пакета (2 байта)
    m_packet.resize(2);
    boost::asio::async_read(m_socket, boost::asio::buffer(m_packet),
                           [this, self](boost::system::error_code ec, std::size_t length)
        {
            if (!ec && length == 2)
            {
                // Интерпретируем длину пакета
                uint16_t packetLength = static_cast<uint8_t>(m_packet[0]) |
                                        (static_cast<uint8_t>(m_packet[1]) << 8);

                if (packetLength > 0 && packetLength <= 0xFFFF)
                {
                    LOG("Packet Length is: " << packetLength);
                    // Этап 2: Читаем сам пакет
                    doReadPacket(packetLength);
                }
                else
                {
                    LOG("Invalid packet length: " << packetLength);
                }
            }
            else
            {
                LOG("Error reading packet length: " << ec.message());
            }
        });
}

void Session::doReadPacket(std::size_t packetLength)
{
    auto self(shared_from_this());

    // Резервируем место для данных пакета
    m_packet.resize(packetLength);
    boost::asio::async_read(
        m_socket, boost::asio::buffer(m_packet),
        [this, self](boost::system::error_code ec, std::size_t length)
        {
            if (!ec && length == m_packet.size())
            {
                // Оповещаем сервер о получении полного пакета
                notifyServer(ec, length, reinterpret_cast<uint8_t*>(m_packet.data()));

                // Преобразуем данные пакета в строку и логируем
                std::string received_data(m_packet.data(), m_packet.data() + length);
                LOG("#Received from client: " << received_data);

                // Читаем следующий пакет
                doRead();
            }
            else
            {
                LOG("Error reading packet data: " << ec.message());
            }
        });
}


void Session::doWrite(std::size_t length)
{
    auto self(shared_from_this());
    LOG("#Before writing");
    boost::asio::async_write(
        m_socket, boost::asio::buffer(m_packet, length),
        [this, self](boost::system::error_code ec, std::size_t /*length*/)
        {
            LOG("#Did write");
            try
            {
                m_socket.close();
            } catch(...){}

            if (ec)
            {
                LOG("Error in doWrite " + ec.message());
            }
        });
}

void Session::notifyServer(boost::system::error_code ec, std::size_t length, uint8_t* data)
{
    LOG("#Notifying server about received message");
    m_server.onPacketReceived(ec, length, data, shared_from_this());
}

void Session::send(const std::string& message)
{
    uint16_t length = static_cast<uint16_t>(message.size());
    std::vector<uint8_t> packet;
    packet.push_back(static_cast<char>(length & 0xFF));
    packet.push_back(static_cast<char>((length >> 8) & 0xFF));
    packet.insert(packet.end(), message.begin(), message.end());

    auto self(shared_from_this());
    boost::asio::async_write(
        m_socket, boost::asio::buffer(packet),
        [self](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (!ec)
            {
                LOG("Notification sent to client!");
            }
            else
            {
                LOG("Error sending notification: " << ec.message());
            }
        });
}
