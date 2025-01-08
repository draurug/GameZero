#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "TcpServer.h"
#include "Logs.h"
#include <QMessageBox>
#include <QDebug>
#include <thread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), m_client(nullptr), io_context()
{
    ui->setupUi(this);
    dbgStartServer();

    // Настройка кнопки Send
    connect(ui->m_sendButton, &QPushButton::clicked, this, &MainWindow::onSendButtonClicked);
    connect(this, &MainWindow::onMessageReceived, ui->m_messageOutput, &QTextEdit::append, Qt::QueuedConnection);

    // Инициализация TcpClient
    m_client = new TcpClient(io_context,
                             [this](const boost::system::error_code& ec, std::size_t, void* data)
                             {
                                 if (ec) {
                                     LOG("Send error: " << ec.message());
                                 } else  {
                                     LOG("Message sent successfully!");
                                 }
                             },
                             [this](const std::string& message)
                             {
                                 LOG("onMessageReceived used successfully?");
                                 emit this->onMessageReceived(QString::fromStdString(message));
                             });

    // Установка соединения с сервером
    m_client->connect("127.0.0.1", "1500",
                      [this](const boost::system::error_code& ec, const tcp::endpoint&)
                      {
                          if (ec) {
                              LOG("Connection error: " << ec.message());
                          } else {
                              LOG("Connected successfully!");
                              m_client->doRead();
                          }
                      });
}

MainWindow::~MainWindow()
{
    delete m_client;
    delete ui;
}

void MainWindow::onSendButtonClicked()
{
    if (!m_client)
    {
        LOG("Client is not initialized!");
        return;
    }

    // Получение текста из текстового поля
    QString message = ui->m_messageInput->toPlainText();
    if (message.isEmpty())
    {
        LOG("Cannot send an empty message!");
        return;
    }

    // Отправка сообщения
    m_client->send(message.toStdString());
    LOG("Message sent: " << message.toStdString());

    // Очистка текстового поля
    ui->m_messageInput->clear();
}

void MainWindow::dbgStartServer()
{
    // Запуск io_context в отдельном потоке
    std::thread ([this]
                {
                    try
                    {
                        TcpServer server(io_context, 1500);
                        io_context.run();
                    }
                    catch (const std::exception& e)
                    {
                        LOG("Error in io_context: " << e.what());
                    }
                }).detach();
    sleep(1);
}
//todo:
// 1) Клиент здоровается с сервером: {0,<name>} -> {101}
// 2) Клиент requests server about active Clients: {1} -> {102, <clientName1>, <clientName2>,...}
// ----------------------------------------------------------------------
// 3) Client send message to <clientName>: {2,<clientName>,<text>}
// 4) Server send message to Client : -> {103,<fromClientName>,<text>}
// 5) Клиент прощается с сервером: {3}
