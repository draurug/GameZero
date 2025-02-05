#include "MainWindow.h"
#include "Settings.h"
#include "ui_MainWindow.h"
#include "TcpServer.h"
#include "ChatClient.h"
#include "ChatServer.h"
#include "Logs.h"

#include <QStringList>
#include <QMessageBox>
#include <QDebug>
#include <thread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), m_client(nullptr), io_context()
{
    ui->setupUi(this);
    connect(ui->m_sendButton, &QPushButton::clicked, this, &MainWindow::onSendButtonClicked);
    connect(ui->m_disconnect, &QPushButton::clicked, this, &MainWindow::disconnectClient);
    //connect(this, &MainWindow::onUserListSignal, this, &MainWindow::onUserListSlot, Qt::QueuedConnection);
    //нужно передать список (QStringList)
}

void MainWindow::initClient(const Settings& settings)
{
    m_client = new ChatClient(io_context,
                              [this](const boost::system::error_code& ec, std::size_t, void* data)
                              {
                                  if (ec) {
                                      LOG("Send error: " << ec.message());
                                  } else  {
                                      LOG("Message sent successfully!");
                                  }
                              });

    m_client->connect(settings.getAddress().toStdString(), std::to_string(settings.getPort()),
                      [this](const boost::system::error_code& ec, const tcp::endpoint&)
                      {
                          if (ec) {
                              LOG("Connection error: " << ec.message());
                          } else {
                              LOG("Connected successfully!");
                              m_client->sendHello(gSettings.getUsername().toStdString());
                              m_client->requestClientList();
                              m_client->doRead();
                          }
                      });

    std::thread([this]
                {
                    try
                    {
                        io_context.run();
                        LOG("Running program");
                    }
                    catch (const std::exception& e)
                    {
                        LOG("Error in io_context: " << e.what());
                    }
                }).detach();
}

MainWindow::~MainWindow()
{
    if (m_client)
    {
        m_client->sayGoodbye();
        delete m_client;
    }
    delete ui;
}

void MainWindow::onSendButtonClicked()
{
    if (!m_client)
    {
        LOG("Client is not initialized!");
        return;
    }
    QString recipient = ui->m_clientList->currentIndex().data().toString();
    QString message = ui->m_typing->text();

    if (recipient.isEmpty() || message.isEmpty())
    {
        LOG("Recipient or message is empty!");
        return;
    }

    m_client->sendMessage(recipient.toStdString(), message.toStdString());
    ui->m_typing->clear();
}

void MainWindow::displayMessage(const QString& message)
{
    ui->m_chatHistory->append(message);
}

void MainWindow::disconnectClient()
{
    if (m_client)
    {
        m_client->sayGoodbye();
        LOG("Client disconnected");
    }
}

void MainWindow::onMessageReceived(const QString& message)
{
    QString formattedMessage = QString("[%1]: %2").arg(message);
    displayMessage(formattedMessage);
}

void MainWindow::dbgStartServer()
{
    std::thread([]
                {
                    try
                    {
                        boost::asio::io_context context;
                        ChatServer server(context, 15001);
                        LOG("Server run");
                        context.run();
                        LOG("Server down");
                    }
                    catch (const std::exception& e)
                    {
                        LOG("Error in io_context: " << e.what());
                    }
                }).detach();
    sleep(1);
}
