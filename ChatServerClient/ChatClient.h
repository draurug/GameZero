#pragma once

#include "TcpClient.h"
#include "MessageType.h"
#include <vector>
#include <string>

class ChatClient : public TcpClient
{

public:
    ChatClient(boost::asio::io_context& io_context,
               std::function<void(const boost::system::error_code&, std::size_t, void* data)> handleResponse,
               std::function<void(const std::vector<std::string>&)> userListCallback,
               std::function<void(const std::string& sender, const std::string& message)> onMessageReceivedCallBack)
        : TcpClient(io_context, handleResponse),
            m_userListCallback(std::move(userListCallback)),
            m_onMessageReceivedCallBack(onMessageReceivedCallBack){}

    virtual void handlePacketFromServer(uint8_t* data, std::size_t length) override
    {
        if (length == 0) return;

        // Определяем тип сообщения
        MessageType type = static_cast<MessageType>(data[0]);
        data++;
        length--;

        LOG("handlePacketFromServer: " << std::string(data, data + length));
        switch (type)
        {
        case SrvClientList:
        {
            std::vector<std::string> nameList;
            const char* start = (char*) data;
            const char* end = (char*) data;
            const char* dataEnd = (char*) data + length;

            while (end < dataEnd)
            {
                if (*end == ';')
                {
                    if (end > start && end - start > 0)
                    {
                        nameList.push_back(std::string (start, end));
                    }
                    start = end + 1;
                }
                ++end;
            }

            if (start < end)
            {
                nameList.push_back(std::string (start, end));
            }

            for (const auto& name : nameList)
            {
                if (!name.empty())
                {
                    LOG("Active clients: " << name);
                }
            }
             m_userListCallback(nameList);
            break;
        }
        case SrvMessage:
        {
            // Сервер передал сообщение от другого клиента.
            std::string fromClient{ reinterpret_cast<char*>(data+1) };
            size_t separator = fromClient.find('\0');
            std::string text = std::string(data + separator + 1, data + length);

            LOG("Message from " << fromClient << ": " << text);
            m_onMessageReceivedCallBack(fromClient, text);
            break;
        }
        default:
            LOG("Unknown message type received: " << static_cast<int>(type));
        }
    }

    std::function<void(const std::vector<std::string>&)> m_userListCallback;
    std::function<void (const std::string&, std::string&)>m_onMessageReceivedCallBack;

    // Отправка приветствия серверу с указанием имени.
    void sendHello(const std::string& name)
    {
        std::vector<uint8_t> packet = {static_cast<uint8_t>(ClHi)};
        packet.insert(packet.end(), name.begin(), name.end());
        sendPacket(packet);
    }

    // Запрос списка активных клиентов.
    void requestClientList()
    {
        std::vector<uint8_t> packet = {static_cast<uint8_t>(ClGetList)};
        sendPacket(packet);
    }

    // Отправка сообщения другому клиенту.
    void sendMessage(const std::string& m_recipient, const std::string& message)
    {
        std::vector<uint8_t> packet = {static_cast<uint8_t>(ClSendMessage)};
        packet.insert(packet.end(), m_recipient.begin(), m_recipient.end());
        packet.push_back('\0'); // Разделитель между именем и текстом сообщения
        packet.insert(packet.end(), message.begin(), message.end());
        sendPacket(packet);
    }

    // Сообщение серверу о выходе из чата.
    void sayGoodbye()
    {
        std::vector<uint8_t> packet = {static_cast<uint8_t>(ClBye)};
        sendPacket(packet);
    }

private:

    // Отправка пакета через TCP-сокет.
    void sendPacket(const std::vector<uint8_t>& packet)
    {
        assert(packet.size() <= 0xFFFF);
        //LOG("Sent packet first byte: " << packet[0]);
        // Создаём буфер с размером пакета и данными
        std::vector<uint8_t>* fullPacketPtr = new std::vector<uint8_t> (2 + packet.size());
        auto & fullPacket = * fullPacketPtr;

        fullPacket[0] = packet.size() & 0xFF;           // Младший байт размера
        fullPacket[1] = (packet.size() >> 8) & 0xFF;    // Старший байт размера
        std::copy(packet.begin(), packet.end(), fullPacket.begin() + 2);

        // Асинхронная отправка полного пакета
        boost::asio::async_write(m_socket, boost::asio::buffer(fullPacket),
                                [fullPacketPtr](const boost::system::error_code& ec, std::size_t /*length*/)
                                {
                                    delete fullPacketPtr;
                                    if (ec)
                                    {
                                        LOG("Error sending packet: " << ec.message());
                                    }
                                    else
                                    {
                                        LOG("Packet sent successfully!");
                                    }
                                });
    }
};
