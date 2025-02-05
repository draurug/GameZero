#pragma once

#include <QMainWindow>
#include <QString>
#include <QTextEdit>
#include <QPushButton>
#include "ChatClient.h"
#include "Settings.h"

namespace Ui
{ class MainWindow; }

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Ui::MainWindow *ui;
    ChatClient* m_client;
    std::thread io_thread;
    boost::asio::io_context io_context;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void initClient(const Settings& settings);
    void disconnectClient();
    static void dbgStartServer();

private slots:
    void onSendButtonClicked();
    void displayMessage(const QString& message);
    void onMessageReceived(const QString& message);
    //void updateClientList(const std::vector<std::string>& clients);

};
