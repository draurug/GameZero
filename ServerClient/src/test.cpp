#include <iostream>
#include <thread>
#include <vector>
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
    TcpClient client(io_context, [](const boost::system::error_code& ec, std::size_t, void* data)
                               {
                                    // if (ec) {
                                    //         LOG("Send error: " << ec.message());
                                    //     } else {
                                    //         LOG("Message sent successfully to ");
                                    //     } --??
                                 //reform in sending message (string)
                                });
    //std::string str(1000, 'f');
    std::string str = "IloveTcp";
    client.connect("127.0.0.1", "15000",
                       [&client, &str](const boost::system::error_code& ec, const tcp::endpoint&)
                        {
                           if (ec) {
                               LOG("Connection error: " << ec.message());
                           } else {
                               LOG("Connected successfully!");
                               client.send(str);
                           }
                       });

    try {
        io_context.run();
        LOG("Client run ended\n");
    } catch (std::exception& e) {
        std::cerr << "Client error: " << e.what() << std::endl;
    }

    sleep(100);
    return 0;
}

//userApplication (текст.поле, кнопка send, в нижнее поле приходит ответ)
