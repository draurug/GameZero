#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "TcpServer.h"
#include "Logs.h"
#include <QMessageBox>
#include <QDebug>
#include <thread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
    m_client(nullptr), io_thread()
{
    ui->setupUi(this);

    // Настройка кнопки Send
    connect(ui->m_sendButton, &QPushButton::clicked, this, &MainWindow::onSendButtonClicked);

    // Инициализация TcpClient
    m_client = new TcpClient(io_context,
                             [this](const boost::system::error_code& ec, std::size_t, void* data) {
                                 if (ec) {
                                     LOG("Send error: " << ec.message());
                                 } else {
                                     LOG("Message sent successfully!");
                                 }
                             });

    // Установка соединения с сервером
    std::string str = "IloveTcp";
    m_client->connect("127.0.0.1", "15000",
                      [this, str](const boost::system::error_code& ec, const tcp::endpoint&) {
                          if (ec) {
                              LOG("Connection error: " << ec.message());
                          } else {
                              LOG("Connected successfully!");
                              m_client->send(str); // Используем член класса m_client
                          }
                      });

    // Запуск io_context в отдельном потоке
    io_thread = std::thread([this]() {
        try {
            io_context.run();
        } catch (const std::exception& e) {
            LOG("Error in io_context: " << e.what());
        }
    });
}

MainWindow::~MainWindow()
{
    // Остановка io_context и завершение потока
    io_context.stop();
    if (io_thread.joinable()) {
        io_thread.join();
    }

    delete m_client;
    delete ui;
}

void MainWindow::onSendButtonClicked()
{
    if (!m_client) {
        LOG("Client is not initialized!");
        return;
    }

    // Получение текста из текстового поля
    QString message = ui->m_messageInput->toPlainText();
    if (message.isEmpty()) {
        LOG("Cannot send an empty message!");
        return;
    }

    // Отправка сообщения
    m_client->send(message.toStdString());
    LOG("Message sent: " << message.toStdString());

    // Очистка текстового поля
    ui->m_messageInput->clear();
}

void MainWindow::onMessageReceived(const std::string &message)
{
    // Отображение полученного сообщения в текстовом поле
    QString receivedMessage = QString::fromStdString(message);
    ui->m_messageOutput->append("Server: " + receivedMessage);
    LOG("Message received: " << message);
}
