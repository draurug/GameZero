#pragma once

#include "TcpServer.h"
#include "Session.h"
#include "MessageType.h"
#include <map>
#include <string>

class ChatServer : public TcpServer
{
    int m_nextClientId = 0;
    std::map<int, std::shared_ptr<Session>> m_ClientsMap;
    std::map<int, std::string> m_clientNames; // Карта имён клиентов (ID -> Имя)
    //shared_ptr if needed
public:
    ChatServer(boost::asio::io_context& io_context, unsigned short port)
        : TcpServer(io_context, port) {}

    virtual void handlePacketFromClient(uint8_t* data, std::size_t length, std::shared_ptr<Session> session) override
    {
        LOG("#handlePacketFromClient: length = " << length);
        if (length == 0) return;

        MessageType type = static_cast<MessageType>(data[0]);

        switch (type)
        {
        case ClHi:
        {
            // Клиент здоровается с сервером
            std::string name(data + 1, data + length);

            // Присваиваем ID клиенту и сохраняем его сессию
            int clientId = m_nextClientId++;
            m_ClientsMap[clientId] = session;
            m_clientNames[clientId] = name;
            session->setName(name);

            LOG("### Client '"<< name <<"': added to ClList");

            // Отправляем подтверждение клиенту
            std::vector<uint8_t> response = {static_cast<uint8_t>(SrvClientList)};
            session->send(std::string(response.begin(), response.end()));
            break;
        }
        case ClGetList:
        {
            LOG("#'ClGetList' received");
            // Клиент запрашивает список активных клиентов
            std::string clientList;
            for (const auto& [id, name] : m_clientNames)
            {
                clientList += name + ";";
            }
            LOG("### clientList: " << clientList);

            std::vector<uint8_t> response = {static_cast<uint8_t>(SrvClientList)};
            response.insert(response.end(), clientList.begin(), clientList.end());
            session->send(std::string(response.begin(), response.end()));
            LOG("#List of active users sent");
            break;
        }
        case ClSendMessage:
        {
            // Клиент отправляет сообщение другому клиенту
            auto ptr = data+1;
            for( ; ptr < data+length; ptr++ )
            {
                if ( *ptr == 0 )
                {
                    break;
                }
            }
            if ( *ptr != 0 )
            {
                LOG("#Error: no null terminator found");
                return;
            }

            std::string targetName(reinterpret_cast<char*>(data + 1), reinterpret_cast<char*>(ptr) );
            std::string message = std::string(ptr + 1, data + length);
            LOG("#handlePacketFromClient: targetName = " << targetName );

            // Ищем клиента по имени
            auto it = std::find_if(m_clientNames.begin(), m_clientNames.end(),[&targetName](const auto& pair)
            { return pair.second == targetName; });

            if (it != m_clientNames.end())
            {
                int targetId = it->first;
                auto targetSession = m_ClientsMap[targetId];

                std::vector<uint8_t> response = {static_cast<uint8_t>(SrvMessage)};
                auto sender = session->getName();
                response.insert(response.end(), sender.begin(), sender.end());
                response.push_back('\0');
                response.insert(response.end(), message.begin(), message.end());

                targetSession->send(std::string(response.begin(), response.end()));
            }
            else
            {
                LOG("#Target client not found: " << targetName);
            }
            break;
        }
        case ClBye:
        {
            // Клиент выходит из чата
            auto clientIdIt = std::find_if(m_ClientsMap.begin(), m_ClientsMap.end(),
                                           [&session](const auto& pair) { return pair.second == session; });

            if (clientIdIt != m_ClientsMap.end())
            {
                int clientId = clientIdIt->first;
                LOG("#Client disconnected: " << m_clientNames[clientId]);

                m_ClientsMap.erase(clientId);
                m_clientNames.erase(clientId);
            }
            break;
        }
        default:
            LOG("#Unknown message type received");
        }
    }

    // Обработка отключения клиента
    void onClientDisconnected(std::shared_ptr<Session> session)
    {
        auto clientIdIt = std::find_if(m_ClientsMap.begin(), m_ClientsMap.end(),
                                       [&session](const auto& pair) { return pair.second == session; });

        if (clientIdIt != m_ClientsMap.end())
        {
            int clientId = clientIdIt->first;
            LOG("#Client " << m_clientNames[clientId] << " disconnected");

            m_ClientsMap.erase(clientId);
            m_clientNames.erase(clientId);
        }
    }
};
