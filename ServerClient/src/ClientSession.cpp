#include <boost/asio.hpp>
#include <memory>
#include <vector>
#include <string>
#include "ClientSession.h"
#include "Logs.h"
#include "TcpServer.h"

void ClientSession::doRead()
{
        auto self(shared_from_this());
        //посмотреть реализацию в Packet версии и аналог создать здесь (вначале читаем длину, а потом весь пакет) done
        //добавить связь с сервером, чтобы оповещать сервер о получении сообщения done
        // аналогично сделать для клиента done
        m_data.resize(1024);
        m_socket.async_read_some(
        boost::asio::buffer(m_data),
        [this, self](boost::system::error_code ec, std::size_t length)
        {
            // Оповещаем сервер о получении пакета
            notifyServer(ec, length, reinterpret_cast<uint8_t*>(m_data.data()));

            if (!ec)
            {
                std::string received_data(m_data.data(), m_data.data() + length);
                LOG("#Received from client: " << received_data);

            }
            else
            {
                LOG("Error in doRead: " + ec.message());
            }
        });
    }

void ClientSession::doWrite(std::size_t length)
{
    auto self(shared_from_this());
    LOG("#Before writing");
    boost::asio::async_write(
        m_socket, boost::asio::buffer(m_data, length),
        [this, self](boost::system::error_code ec, std::size_t /*length*/)
        {
            LOG("#Did write");
            if (!ec)
            {
                doRead();
            }
        });
}

void ClientSession::notifyServer(boost::system::error_code ec, std::size_t length, uint8_t* data)
{
    LOG("#Notifying server about received message");
    m_server.onPacketReceived(ec, length, data, shared_from_this());
}

void ClientSession::send(const std::string& message)
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
