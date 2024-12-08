#include <iostream>
#include <thread>
#include "Logs.h"
#include "TcpServer.h"
#include "TcpClient.h"

int main()
{
    LOG("test");
    std::thread myThread([]() {

            try {
                boost::asio::io_context io_context;
                TcpServer server(io_context, 15000);
                io_context.run();
            } catch (std::exception& e) {
                std::cerr << "Server error: " << e.what() << std::endl;
            }

        std::cout << "Hello from the thread using a lambda function!" << std::endl;
    });
    sleep(1);
    boost::asio::io_context io_context;
    TcpClient client(io_context);

    client.connect("127.0.0.1", "15000");

    try {
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "Client error: " << e.what() << std::endl;
    }
    sleep(100);
    return 0;
}