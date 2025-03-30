#include "MainWindow.h"
#include "Settings.h"
#include "ui_MainWindow.h"
#include "ChatClient.h"
#include "DbgClient.h"
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
    connect(this, &MainWindow::onUserListSignal, this, &MainWindow::onUserListSlot, Qt::QueuedConnection);
    connect(this, &MainWindow::onMessageSignal, this, &MainWindow::onMessageSlot);
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
                              },
                              [this](const std::vector<std::string>& userList)
                              {
                                  QStringList qUserList;
                                  for (const auto& name : userList)
                                  {
                                      qUserList.append(QString::fromStdString(name));
                                  }

                                  emit onUserListSignal(qUserList);
                              },
                              [this](const std::string& sender, const std::string& message)
                              {
                                  QString formattedMessage = QString("%1: %2").arg(QString::fromStdString(sender), QString::fromStdString(message));
                                  emit onMessageSignal(formattedMessage);
                              });

    m_client->connectToServer(settings.getAddress().toStdString(), std::to_string(settings.getPort()),
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
    QString recipient = ui->m_clientList->currentItem() ? ui->m_clientList->currentItem()->text() : "";
    QString message = ui->m_typing->text();
    QString sender = gSettings.getUsername();
    if (recipient.isEmpty() || message.isEmpty())
    {
        LOG("Recipient or message is empty!");
        return;
    }

    m_client->sendMessage(recipient.toStdString(), message.toStdString());
    ui->m_typing->clear();
    ui->m_chatHistory->append(sender + " sent: \"" + message + "\" to " + recipient);
}
void MainWindow::disconnectClient()
{
    if (m_client)
    {
        m_client->sayGoodbye();
        LOG("Client disconnected");
    }
}

void MainWindow::onUserListSlot(const QStringList& users)
{
    ui->m_clientList->clear();
    ui->m_clientList->addItems(users);
    LOG("Client list updated");
}

void MainWindow::onMessageSlot(const QString& message)
{
    ui->m_chatHistory->append("DbgClient: " + message);
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

void dbgStartSecondClient()
{
    static boost::asio::io_context io_context;
    ChatClient* client2 = new ChatClient(io_context,
                              [](const boost::system::error_code& ec, std::size_t, void* data)
                              {
                                  if (ec) {
                                      LOG("Send error: " << ec.message());
                                  } else  {
                                      LOG("Message sent successfully!");
                                  }
                              },
                              [client2](const std::vector<std::string>& userList)
                              {
                                for(auto& user:userList)
                                {
                                    LOG("#111# User:: " << user);
                                }
                              },
                                [](const std::string& sender, const std::string& message)
                                {
        LOG("#111# Message from: " << sender << " is: "<< message);
                                });


    client2->connectToServer(gSettings.getAddress().toStdString(), std::to_string(gSettings.getPort()),
                              [client2](const boost::system::error_code& ec, const tcp::endpoint&)
                              {
                                  if (ec){
                                      LOG("Connection error: " << ec.message());
                                  } else {
                                      LOG("Connected successfully!");
                                      client2->sendHello("Bot");
                                      client2->requestClientList();
                                      client2->doRead();
                                  }
                              });

    std::thread([]
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
