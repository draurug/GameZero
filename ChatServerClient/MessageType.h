#pragma once
#include <cstdint>
//todo:
// 1) Клиент здоровается с сервером: {0,<name>} -> {101}
// 2) Клиент requests server about active Clients: {1} -> {102, <clientName1>, <clientName2>,...}
// ----------------------------------------------------------------------
// 3) Client send message to <clientName>: {2,<clientName>,<text>}
// 4) Server send message to Client : -> {103,<fromClientName>,<text>}
// 5) Клиент прощается с сервером: {3}


enum MessageType: uint8_t
{
    ClHi,
    ClGetList,
    ClSendMessage,
    ClBye,

    SrvClientList,
    SrvMessage
};
