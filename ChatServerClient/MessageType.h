#pragma once
#include <cstdint>

enum MessageType: uint8_t
{
    ClHi,          //1) Клиент здоровается с сервером: {0,<name>} -> {101}
    ClGetList,     //2) Клиент requests server about active Clients: {1} ->
    ClSendMessage, //3) Client send message to <clientName>: {2,<clientName>,<text>}
    ClBye,         //5) Клиент прощается с сервером: {3}

    SrvClientList, //2) Клиент получает от сервера ответ на свой запрос о клиентах -> {102, <clientName1>, <clientName2>,...}
    SrvMessage     //4) Server send message to Client : -> {103,<fromClientName>,<text>}
};
